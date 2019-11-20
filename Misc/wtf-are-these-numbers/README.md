# What's a number?
This folder contains everything you should need to know to use this challenge in a ctf. It is kind of a silly challenge, but it teaches about base convesion, which is a subject many have a hard time grasping. The original form of this challenge uses the Radical of the Integers sequence to generate the flag, the reasons for this choice are outlined below.

## Files
 - flag.txt contains the plaintext flag
 - wtf_are_these_numbers.txt contains the encoded flag
 - hint.png contains the hint for the challenge further detail is below because there will probably be questions
 - genflag.py will output the encoded contents of flag.txt
 - bases.py decodes the flag (make sure the same sequence is being used) in a user specified file
 - writeup.docx contains a detailed writeup of how the challenge works, as well as how to complete it
 - writeup.pdf is the pdf of writeup.docx

## Script usage
### gen_flag.py
```
usage: gen_flag.py [-h] [-f FILE]

optional arguments:
  -h, --help            show this help message and exit
    -f FILE, --file FILE  Specify an alternate flag file
Outputs encoded flag to terminal if flag.txt is in running directory.
```
Don't use a flag longer than 36 characters, unless you change the sequence used.
Use only ascii characters for the flag.
### bases.py
```
usage: bases.py [-h] f

positional arguments:
  f           File to read encoded flag from

optional arguments:
  -h, --help  show this help message and exit
```
Outputs decoded flag to terminal based on user specified file for encoded flag.
Make sure the sequence matches the one used to encode.


## Hint explaination
The breakdown of the hint is relatively straightforward. The ℤ symbol is commonly understood to represent the integers. The subscript of a question mark is meant to indicate that bases of numbers are to be involved (not to be confused with the notation for cyclic subgroups of the integers, though even that might lead one down the right path). The trickiest part of the hint revolves the √ symbol, known as the radical symbol. Most people associate this with square roots, but the name is what is important here (despite it being unrelated to the actual sequence). Putting these three things together one could stumble across the proper sequence, and be well on the way to solving the challenge.

## Why the Radicals of the Integers?
Why on earth would I pick such an obscure sequence?
TLDR: there aren't actually that many good options.
The sequence of integers in order is too basic, and had been discussed with peers prior to the challeng's first appearance, so another sequence had to be chosen. This sequence could not grow too quickly, otherwise the flag would have to be kept shorter than one would like for a CTF.
The Radicals of the Integers sequence is one of the surprising few "well known" sequences that fit the bill. That's pretty much it.
Of course this would not be an issue, if base64 was in any way reasonable and established a standard which could be built off of to incorporate symbols for a number system at least up to base64. But no, base64 is a special snowflake, and A=0, not 0=0. 0=52 in base64.
