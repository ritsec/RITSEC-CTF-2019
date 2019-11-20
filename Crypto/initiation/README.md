# Initiation
This challenge was created to recreate one of my first ctf experiences: solving a clue to find out which cipher to use and decrypt the encoded message.
TLDR: Use copiale cipher to decrypt message, translate from German to English.

## Files
 - flag.txt contains the plaintext English flag wrapped with RITSEC{}
 - germanflag.txt contains the plaintext German flag obtained by decrypting the message
 - secret.png contains the copiale ciphertext
 - hint.jpg contains the hint intended to lead you to the copiale cipher

## hint.jpg
The hint was meant to lead you to the search terms "cult: and "germany." 
If you were particularly observant you might have noticed that the map was not current, and dated back to at least 1900.
If you were really lucky you might have connected this to the masons
All of this was done to get you to use your favorite search engine and query for "german cult cipher" or something similar.
This would lead you to find the copiale cipher, and hopefully to a page with a table to convert the ciphertext.
The one I moslty relied on was this: https://www.isi.edu/natural-language/people/copiale-11.pdf
This link is a pdf of the original writeup for how the cipher was cracked, a quick readthorugh teaches you everything you need to know about the cipher to easily decode the message.

## secret.png
This was the secret message. Originally this was going to be another text file, but the I couldn't find the upside down ribbon symbol in any of the charsets I was trying to use.
Instead I wrote it by hand, just like the masons did when they wrote their original manuscript.
Using the table you found and your knowledge of the cipher you would decipher the message which would decrypt to German.

## germanflag.txt
This flag contains the german version of the flag, which is the language the copiale cipher was originally written in.
Popping this message into google tranlate (usually) results in one of 2 messages:
    - creepy cults and old ciphers
    - scary cults and old ciphers
Wrap this in RITSEC{} and you had your flag

Note: Initially only "scary cults and old ciphers was accepted", but "creepy cults and old ciphers" was added as an alternative flag.

## flag.txt
The plaintext flag