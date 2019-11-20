#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>

/* This challenge consists of two bugs:
 * 1. Double fetch (overwrite)
 * 2. Unsafe unlink (arbitrary write)
 * In order to trigger the double fetch you need to provide a shopping list 
 * in the correct format and change it before the copy is done but after the 
 * malloc is performed. This way you can corrupt entries already in the list 
 *
 * To exploit the following steps will need to be performed
 * 1. Enter two different items, put the second on one shopping list
 * 2. Remove the first item
 * 3. Create a list in a file of size 1. Malloc will be called re-using the
 *    recently freed entry.
 * 4. Change the file as it is being loaded so that a double fetch occurs and
 *    data in the next (still used) chunk is corrupted.
 * 5. Write into the global offset table.... something.
 *
 * TODO: give some kind of memory leak? perhaps read the "next" entry?
 *
 * */

// very unsafe boys
#define __unlink(n, fd, bk) { fd = (n)->fd; \
                              bk = (n)->bk; \
                              fd->bk = bk; \
                              bk->fd = fd; }
#define __frontlink(n, s) { (n)->bk = s; \
                            (n)->fd = (s)->fd; \
                            (s)->fd->bk = n; \
                            (s)->fd = n; }

#ifdef DEBUG
#define dbg(x) x
#else
#define dbg(x)
#endif

#define HEADER_COOKIE (0x4092384748393046)
#define MAX_ENTRIES 30
#define MAX_LISTS 10

typedef struct _item_t {
    struct _item_t *fd;
    struct _item_t *bk;
    char *name;
    uint64_t id;
} item_t, *catalog;

typedef struct _list_entry_t {
    struct _list_entry_t *next;
    struct _item_t *item;
    size_t quantity;
} list_entry_t;

typedef struct _list_t {
    char *name;
    struct _list_entry_t *listhead;
} list_t;

typedef struct _file_header_t {
    uint64_t hdr_cookie;
    size_t entries;
} file_header_t;

typedef struct _file_entry_t {
    uint64_t id;
    size_t namelen;
    char name[0];
} file_entry_t;

item_t sentinal;

char get_choice()
{
    char choice;
    scanf("%c%*c", &choice);
    return choice;
}

size_t get_str(char *buf, size_t bufsz)
{
    size_t n = 0;
    char *p;
    for (p = buf; *(p-1) != '\n' && n < bufsz; p++, n++)
        *p = getchar();
    *(p-1) = '\0';
    return n-1;
}

uint64_t get_id(char *prompt)
{
    char buf[20];
    prompt = prompt?:"Enter an ID: ";
    uint64_t id;
    printf("%s", prompt);
    get_str(buf, sizeof(buf));
    
    return strtoul(buf, NULL, 10);
}

static inline void *_malloc(size_t sz)
{
    void *ret = malloc(sz);
    dbg(printf("Allocated %lu bytes (%lu real) @ %p\n", sz, (*(unsigned long*)(ret-8)) & ~7, ret));
    return ret;
}

static inline void _free(void *p)
{
    dbg(printf("Freeing %p of size %lu\n", p, (*(unsigned long*)(p-8)) & ~7));
    free(p);
}

void das_main_menu()
{
    puts("Le Menu...");
    puts("1. Catalog management");
    puts("2. List management");
    puts("3. Give me a shell");
    puts("5. Where did 4 go?");
    puts("F. Press F to pay respects");
    puts("7. GET ME OUTTA HERE FAM");
}

void das_catalog_menu()
{
    puts("Manage ur catalog");
    puts("1. New catalog item");
    puts("2. Delete catalog item");
    puts("3. View catalog");
    puts("4. Import catalog from file");
    puts("5. Back");
}

void das_list_menu()
{
    puts("Manage ur lists");
    puts("1. New list");
    puts("2. Delete a list");
    puts("3. View lists");
    puts("4. View a list");
    puts("5. Add an item to a list");
    puts("6. Remove an item from a list");
    puts("7. Set quantity");
    puts("8. Back");
}

void make_list(list_t *list, char *name)
{
    list->listhead = NULL;
    list->name = strdup(name);
    assert(list->name != NULL);
}

void free_list(list_t *list)
{
    list_entry_t *cursor = list->listhead;
    if (list->name == NULL){
        return;
    }
    _free(list->name);
    list->name = NULL;
    while (cursor != NULL){
        _free(cursor);
        cursor = cursor->next;
    }
}

void add_item_to_list(item_t* item, list_t *list, size_t quantity)
{
    if (list->name == NULL){
        return;
    }
    list_entry_t *le = list->listhead;
    list_entry_t *new = (list_entry_t*)_malloc(sizeof(list_entry_t));
    assert(new != NULL);
    new->quantity = quantity;
    new->next = NULL;
    new->item = item;

    if (le == NULL){
        list->listhead = new;
        return;
    }

    while (le->next != NULL)
        le = le->next;
    le->next = new;
}

void remove_item_from_list(item_t* item, list_t *list)
{
    list_entry_t *cursor = list->listhead, *prev;
    if (list->name == NULL){
        return;
    }
    if (cursor == NULL){
        puts("Empty list");
        return;
    }
    if (cursor->item == item){
        list->listhead = cursor->next;
    } else {
        while (cursor != NULL && cursor->item != item){
            prev = cursor;
            cursor = cursor->next;
        }
        if (cursor != NULL){
            prev->next = cursor->next;
        }
    }
    if (cursor != NULL)
        _free(cursor);
}

void print_list(list_t *list)
{
    list_entry_t *cursor = list->listhead;
    if (list->name == NULL){
        return;
    }
    if (cursor == NULL){
        puts("Empty list");
        return;
    }
    printf("List %s\n-----------------\n", list->name);
    while (cursor != NULL){
        printf("%lu: %s * %lu\n", cursor->item->id, cursor->item->name, cursor->quantity);
        cursor = cursor->next;
    }
}

void print_lists(list_t *lists, size_t nlists)
{
    size_t nitems;
    list_entry_t *cursor;
    for (size_t i = 0; i < nlists; i++){
        if (lists[i].name != NULL){
            nitems = 0;
            cursor = lists[i].listhead;
            while (cursor != NULL){
                nitems += cursor->quantity;
                cursor = cursor->next;
            }
            printf("%lu: %s -- %lu", i, lists[i].name, nitems);
            dbg(printf(" -- &name: %p, listhead: %p", lists[i].name, lists[i].listhead));
            puts("");
        }
    }
}

void set_quantity(list_t *list, size_t quantity, uint64_t id)
{
    list_entry_t *cursor = list->listhead;
    while (cursor != NULL && cursor->item->id != id)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        return;
    }
    cursor->quantity = quantity;
}

void make_new_catalog_entry(char *name, size_t *namelen, uint64_t id, catalog cat)
{
    item_t *i = (item_t*)_malloc(sizeof(item_t));
    assert(i != NULL);

    if (namelen == 0){
        i->name = strdup(name);
    } else {
        if ((*namelen)+1 < *namelen){
            // overflow protection
            free(i);
            return;
        }
        i->name = (char*)_malloc(*namelen+1);
        assert(i->name != NULL);
        dbg(getchar());
        dbg(printf("Copied %lu bytes\n", *namelen));
        printf("\tAdding %s with id %lu\n", name, id);
        memcpy(i->name, name, *namelen);
        i->name[*namelen] = '\0';
    }
    i->id = id;
    __frontlink(i, cat);
}

void import_catalog(char *filename, catalog cat)
{
    file_entry_t* d = NULL;
    file_header_t h;
    dbg(printf("Opening %s\n", filename));
    int fd = open(filename, O_RDONLY);
    if (fd < 0){
        puts("No file by that name can be opened");
        return;
    }

    int mapsize = lseek(fd, 0, SEEK_END);
    if (mapsize == -1){
        dbg(puts("Invalid file"));
        return;
    }
    void *mem = mmap(NULL, mapsize, PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED || mem == NULL){
        dbg(puts("Mapping failed"));
        close(fd);
        return;
    }

    memcpy(&h, mem, sizeof(file_header_t));
    if (h.hdr_cookie != HEADER_COOKIE || h.entries > MAX_ENTRIES){
        dbg(puts("Invalid cookie or too many entries"));
        goto end;
    }
    d = (file_entry_t*)((file_header_t*)mem + 1);
    for (size_t i = 0; i < h.entries; i++)
    {
        make_new_catalog_entry(d->name, &d->namelen, d->id, cat);
        // this is also a bug, but whatever, man
        d = (file_entry_t*)((char *)d + d->namelen + sizeof(file_entry_t));
    }
    dbg(puts("Import success"));

end:
    munmap(mem, mapsize);
    close(fd);
}

void print_catalog(catalog cat)
{
    for (item_t *i = cat->fd; i != &sentinal; i = i->fd){
        printf("%lu: %s", i->id, i->name);
        dbg(printf(" -- fd: %p, bk: %p", i->fd, i->bk));
        puts("");
    }
}

item_t *find_item_by_id(catalog cat, uint64_t id)
{
    for (item_t *i = cat->fd; i != &sentinal; i = i->fd){
        if (i->id == id)
            return i;
    }
    return NULL;
}

void remove_item_by_id(catalog cat, uint64_t id)
{
    item_t *i, *fd, *bk;
    i = find_item_by_id(cat, id);
    if (i == NULL){
        printf("Item with id %lu not found\n", id);
        return;
    }
    __unlink(i, fd, bk);
    _free(i->name);
    _free(i);
}

int main(int argc, char** argv)
{
    list_t lists[MAX_LISTS];
    catalog cat = &sentinal;
    item_t *item;
    uint64_t id, l, q;
    int quit = 0, i;
    char choice = 0;
    char inbuf[1024];
    void (*menu)();
    menu = das_main_menu;
    sentinal.fd = sentinal.bk = &sentinal;
    memset(lists, 0, sizeof(list_t)*MAX_LISTS);
    dbg(printf("Sentinal is at %p\n", &sentinal));
    puts("wumb0's stupid shopping list manager");
    puts("It sucks. So pwn it, mmmk?");

    while (!quit){
        puts("");
        menu();
        printf("\nChoose wisely: ");

        choice = get_choice();
        puts("");
        if (menu == das_main_menu){
            switch (choice){
                case '1':
                    menu = das_catalog_menu;
                    break;
                case '2':
                    menu = das_list_menu;
                    break;
                case '3':
                    printf("OK!\n# ");
                    fflush(stdout);
                    sleep(20);
                    puts("Yeah it's not going to be that easy, lad.");
                    break;
                case '4':
                    puts("You found it!");
                    puts("RC3-WAIT-WRONG-CTF ¯\\_(ツ)_/¯");
                    break;
                case '5':
                    puts("It died.");
                    break;
                case 'F':
                    // lmao
                    puts("rip");
                    _free(das_list_menu);
                    break;
                case '7':
                    quit = 1;
                    break;
                default:
                    break;
            }
        } else if (menu == das_catalog_menu){
            switch (choice){
                case '1':
                    id = get_id(NULL);
                    printf("Enter a name: ");
                    get_str(inbuf, sizeof(inbuf));
                    make_new_catalog_entry(inbuf, 0, id, cat);
                    break;
                case '2':
                    print_catalog(cat);
                    id = get_id(NULL);
                    remove_item_by_id(cat, id);
                    break;
                case '3':
                    print_catalog(cat);
                    break;
                case '4':
                    printf("Give me a file to import: ");
                    get_str(inbuf, sizeof(inbuf));
                    import_catalog(inbuf, cat);
                    break;
                case '5':
                    menu = das_main_menu;
                    break;
                default:
                    break;
            }

        } else if (menu == das_list_menu){
            switch (choice){
                case '1':
                    for (i = 0; i < MAX_LISTS && lists[i].name != NULL; i++){}
                    if (i == MAX_LISTS){
                        puts("All lists in use!");
                        break;
                    }
                    printf("List name: ");
                    get_str(inbuf, sizeof(inbuf));
                    dbg(printf("Adding list %s at index %d\n", inbuf, i));
                    make_list(&lists[i], inbuf);
                    break;
                case '2':
                    print_lists(lists, MAX_LISTS);
                    id = get_id("Enter list number: ");
                    if (id > MAX_LISTS)
                        break;
                    free_list(&lists[id]);
                    break;
                case '3':
                    print_lists(lists, MAX_LISTS);
                    break;
                case '4':
                    print_lists(lists, MAX_LISTS);
                    id = get_id("Enter list number: ");
                    if (id > MAX_LISTS)
                        break;
                    print_list(&lists[id]);
                    break;
                case '5':
                    print_lists(lists, MAX_LISTS);
                    l = get_id("Enter list number: ");
                    if (l > MAX_LISTS || lists[l].name == NULL)
                        break;
                    print_catalog(cat);
                    id = get_id(NULL);
                    item = find_item_by_id(cat, id);
                    if (item == NULL)
                        break;
                    dbg(printf("Got item %s @ %p\n", item->name, item));
                    q = get_id("Enter quantity: ");
                    add_item_to_list(item, &lists[l], q);
                    break;
                case '6':
                    print_lists(lists, MAX_LISTS);
                    l = get_id("Enter list number: ");
                    if (l > MAX_LISTS || lists[l].name == NULL)
                        break;
                    print_list(&lists[l]);
                    id = get_id(NULL);
                    item = find_item_by_id(cat, id);
                    if (item == NULL)
                        break;
                    dbg(printf("Got item %s @ %p\n", item->name, item));
                    remove_item_from_list(item, &lists[l]);
                    break;
                case '7':
                    l = get_id("Enter list number: ");
                    if (l > MAX_LISTS || lists[l].name == NULL)
                        break;
                    id = get_id(NULL);
                    q = get_id("Enter quantity: ");
                    set_quantity(&lists[l], q, id);
                    break;
                case '8':
                    menu = das_main_menu;
                    break;
                default:
                    break;
            }
            
        } else {
            puts("You shouldn't have come here...");
            exit(69);
        }
    }
    puts("ok bye.");
}
