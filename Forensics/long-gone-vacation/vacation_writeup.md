HINT: These are my favorite places to visit

1. You you run `file` on the the binary you can see it's a tar file. Step one is to extract it.
2. Once it's extracted you can see that it's a directory with a bunch of stuff in it, it's a Google Chrome appdata dump.
3. In UserData/Default/Bookmarks (which can be figured out by "MY FAVORITE PLACES TO VISIT"), there's a json blob, which is the bookmarks of the browser.
4. When sorted by date_added (the default sort of the blob), the bookmark "Name" are R, I, T, S, E, C..... which are the flag
5. Flag is RITSEC{CHR0M3_BM_FTW}