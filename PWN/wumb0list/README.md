The import catalog from file option has a race condition because it mmaps the given file as MAP\_SHARED.  
If you are able to swap out the file after the size is read but before the memcpy happens (where the size is read again) then you can cause an arbitrarily sized heap overflow.  

There were a few unintended bugs/solutions:
1. Stack leak with list #10 (off by one)  
2. Use after free with items in a list. If you free a catalog item when it is in a list and then re-allocate something else there you may be able to get an information disclosure.  


My solution is as follows:  
0. Start a C program that swaps out two files in order to eventually trigger the race  
1. Make two catalog entries a and b  
2. Make a list AAAAAAAAAAAAAAAA  
3. Delete list AAAAAAAAAAAAAAAA  
4. Make list BBBBBBBBBBBBBBBBBBBBBBBB  
5. Add item b to list BBBBBBBBBBBBBBBBBBBBBBBB  
6. Make list CCCCCCCCCCCCCCCCCCCCCCCC  
7. Add item b to list CCCCCCCCCCCCCCCCCCCCCCCC  
8. Delete item a  
9. Import list until race succeeds with a buffer that:  
    a. Overwrites item b's name with the address of what is at global offset table + 8  
    b. Overwrites list BBBBBBBBBBBBBBBBBBBBBBBB next pointer with GOT+0x98 (strdup-8)  
    c. Overwrites list CCCCCCCCCCCCCCCCCCCCCCCC next pointer with GOT+0x20 (mmap-8)  
10. List out enties to leak the address in the GOT. Calculate base of libc  
11. Overwrite strdup with system, mmap with setreuid using update quantity feature of lists  
12. Call import from file with a zero sized file to call (setreuid(0, 0))  
13. Call create new list with a name of /bin/sh to get a shell  
