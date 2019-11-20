Take it to the Cleaners
## RITSEC CTF 2019
Author: Impos73r
Description: People hide things in images all the time! See if you can find what the artist forgot to take out in this one!

Flag: *RITSEC{FORENSICS_FAILS_WONT_HELP_YOU_HERE}*

## Solution:
1. Take the image and plug it into an EXIF Data viewer, http://exif.regex.info/exif.cgi is really nice.
2. Extract the *User Comment* field from the data to get RVZHRlJQe1NCRVJBRlZQRl9TTlZZRl9KQkFHX1VSWUNfTEJIX1VSRVJ9 (This is a Base64 encoded string.)
3. Base64 decode the string and get EVGFRP{SBERAFVPF_SNVYF_JBAG_URYC_LBH_URER}
4. Apply ROT13 to get the flag RITSEC{FORENSICS_FAILS_WONT_HELP_YOU_HERE}

https://www.thexifer.net/#exif-general was used to edit the metadata in the first place.
