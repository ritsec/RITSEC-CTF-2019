HINT: That data? No it's long gone. It's basically history

1. From the hint, one can infer that is has something to do with the HISTORY
2. The given binary is a tar file, when extracted it will give them a chrome "appdata" directory, similar to the vacation challenge.
3. The HISTORY file is located in userdata/default/history
4. It's an sqlite file, which can be opened with something like "SQLite Browser".
5. Once opened, the "urls" table has all of the search history
6. All of the URLs are google searches
7. One of the searches is "<something>.ritsec.club/<something>
8. That searched URL, when navigated to, is hosting the flag
9. The flag is RITSEC{SP00KY_BR0WS3R_H1ST0RY}