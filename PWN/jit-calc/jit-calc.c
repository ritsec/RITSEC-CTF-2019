#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <sys/mman.h>

const bool RAX = 1;
const bool RBX = 0;
const int MAX_SIZE = 1000;
const int PAGE_SIZE = 0x8000;

#define NUM_FUNCTIONS 8

void init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}


void writeMovImm64(bool reg, uint64_t imm64, uint8_t * pos) {
    *(pos) = 0x48;
    if (reg == RAX) {
        *(pos + 1) = 0xb8;
    } else {
        *(pos + 1) = 0xbb;
    }
    //Little endian
    *(pos + 2) = imm64 & 0xff;
    *(pos + 3) = (imm64 >> (8 * 1)) & 0xff;
    *(pos + 4) = (imm64 >> (8 * 2)) & 0xff;
    *(pos + 5) = (imm64 >> (8 * 3)) & 0xff;
    *(pos + 6) = (imm64 >> (8 * 4)) & 0xff;
    *(pos + 7) = (imm64 >> (8 * 5)) & 0xff;
    *(pos + 8) = (imm64 >> (8 * 6)) & 0xff;
    *(pos + 9) = (imm64 >> (8 * 7)) & 0xff;
}


void writeAddReg(bool reg1, bool reg2, uint8_t * pos) {
    *(pos) = 0x48;
    *(pos + 1) = 0x01;
    if (reg1 == RAX) {
        if (reg2 == RAX) {
            *(pos + 2) = 0xc0;
        } else {
            *(pos + 2) = 0xd8;
        }
    } else {
        if (reg2 == RAX) {
            *(pos + 2) = 0xc3;
        } else {
            *(pos + 2) = 0xdb;
        }
    }
}

void writeRet(uint8_t * pos) {
    *(pos) = 0xc3;
}

uint64_t callCode(uint8_t * pos) {
    typedef uint64_t( * func)(void);
    uint64_t(*funcPtr)();
    funcPtr = pos;

    return (*funcPtr)();
}

//Menus
void print_main_menu() {
    puts("1: Change Index");
    puts("2: Write code");
    puts("3: Exit");
    puts("4: Run code");
}

void print_edit_menu() {
    puts("1: Finish Function");
    puts("2: Write Addition");
    puts("3: Write Constant Value");
}

void print_add_menu() {
    puts("1: Add Register 1 to Register 2");
    puts("2: Add Register 2 to Register 1");
    puts("3: Add Register 1 to Register 1");
    puts("4: Add Register 2 to Register 2");
}

void print_mov_menu() {
    puts("1: Store to register 1");
    puts("2: Store to register 2");
}

void editCode(uint8_t * code) {
    uint8_t * upperLimit = code + MAX_SIZE - 15; //Don't want to get too close!
    int offset = 0;

    while (code + offset < upperLimit) {
        print_edit_menu();
        char line[512];
        memset(line, 0, 512);
        fgets(line, 511, stdin);
        if (line[0] == '1') {
            writeRet(code);
            break;
        } else if (line[0] == '2') {
            print_add_menu();
            memset(line, 0, 512);
            fgets(line, 511, stdin);
            if (line[0] == '1') {
                writeAddReg(RBX, RAX, code);
            } else if (line[0] == '2') {
                writeAddReg(RAX, RBX, code);
            } else if (line[0] == '3') {
                writeAddReg(RAX, RAX, code);
            } else {
                writeAddReg(RBX, RBX, code);
            }
            code += 3;
        } else if (line[0] == '3') {
            print_mov_menu();
            memset(line, 0, 512);
            fgets(line, 511, stdin);
            bool reg = RAX;
            if (line[0] == 2) {
                reg = RBX;
            }
            uint64_t val = 0;
            printf("Enter the constant:\n");
            memset(line, 0, 512);
            fgets(line, 511, stdin);
            val = strtoull(line, NULL, 10);
            writeMovImm64(reg, val, code);
            code += 10;
        }

    }
}

void loop() {
    uint8_t * codeBasePage = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    int index = 0;
    char line[512];

    short codeOffsets[NUM_FUNCTIONS] = {};
    for (int i = 0; i < NUM_FUNCTIONS; i++) {
        codeOffsets[i] = MAX_SIZE * i;
    }

    while (1) {
        printf("Current index: %i\n", index);
        print_main_menu();

        memset(line, 0, 512);
        fgets(line, 511, stdin);
        line[strlen(line) - 1] = '\0';
        printf("Using option %s.\n", line);
        if (!strcmp(line, "1")) {
            printf("What index would you like to change to (0-9)\n");
            memset(line, 0, 512);
            fgets(line, 511, stdin);
            int res = atoi(line);
            if (res >= 0 && res < NUM_FUNCTIONS) {
                index = res;
            }
        } else if (!strcmp(line, "2")) {
            //Make writeable
            mprotect(codeBasePage, PAGE_SIZE, PROT_READ | PROT_WRITE);
            editCode(codeBasePage + codeOffsets[index]);
        } else if (!strcmp(line, "3")) {
            exit(1);
        } else {
            //Make executable
            mprotect(codeBasePage, PAGE_SIZE, PROT_READ | PROT_EXEC);
            printf("Result: %llx\n", callCode(codeBasePage + codeOffsets[index]));
        }
    }
}

int main() {
    init();
    printf("Welcome to our super fast JIT calculator\n");
    printf("If you're looking for fast computation, you came to the right place!\n");
    printf("None of that slow interpreted stuff here\n\n\n");
    printf("Notice: You can only use %i bytes per function, so we provided %i spaces for functions.\n", MAX_SIZE, NUM_FUNCTIONS);
    uint8_t * code = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    writeMovImm64(RAX, 0x123456789, code);
    writeRet(code + 10);

    mprotect(code, 0x1000, PROT_READ | PROT_EXEC);
    printf("Code result: %llx\n", callCode(code));
    loop();
}
