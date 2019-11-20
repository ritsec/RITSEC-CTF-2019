# Lunar Lander 2
CTF Challenge for RITSEC CTF 2019


__Category:__ Misc  
__Difficulty:__ 500 (Originally 300)  
__Author:__ knif3  
__Description:__  
```
Starman!

It seems like the guidance computer is malfunctioning again. Can you validate the outputs and verify what is going on? Unfortunately, the only copy of the source was accidently deleted. We were able to declassify this old backup though. Make sure you don't lose it or share it with anyone.

Good luck,
- Houston

Flag format: RITSEC{}
```

__Files:__ `LANDER_CODE_MANUEL.pdf`

__Hints:__  
> You do actually need to compile and run the program

> You do not need to do any decode/calculations


## Writeup

Step 1 is to transcribe the data. From that, we get DIAG.AGC which appears to be some form of assembly. Hopefully after doing Lunar Lander 1, you will realize that the source code for the Apollo computers is online. Realize that this is probably a source code file for that.

Looking into the Apollo Guidance Computer (AGC), you will most likely stumble upon [this repository](https://github.com/virtualagc/virtualagc) and/or [this website](http://www.ibiblio.org/apollo/).

They provide two tools that are of value to us:
  1. yaYUL - an AGC assembler
  2. yaAGC - a core-rope binary emulator

You can follow the github to get these installed. I have also made a simple Dockerfile to run it as well.

Compile the binary:
```
./yaYUL DIAG.AGC
```

This will output `DIAG.AGC.bin`. We can run this with
```
./yaAGC --core=DIAG.AGC.bin
```

Looking at the ASM, we can see that it is making lots of `WRITE` calls. We
can look up what this does on [the ASM reference page](https://www.ibiblio.org/apollo/assembly_language_manual.html). We see that WRITE will send it to IO channels.

There is a link to [here](https://www.ibiblio.org/apollo/assembly_language_manual.html#io_channels) which then links to the [developer page](https://www.ibiblio.org/apollo/developer.html)

According to the developer page, we can see that yaAGC simulates channels using IP packets.

> As mentioned above, yaAGC and yaAGS send data to simulated hardware using the send function, and the simulated hardware receives it using the recv function and vice versa. Every write to an "output channel" by the AGC or AGS results in a send.
https://www.ibiblio.org/apollo/developer.html#sendrecv_Protocol


A sample command is even provided to display the output data.
```
socat TCP:127.0.0.1:19799 | hexdump -C
```

This port however is not opened up by default, so we can list the ports.
```
netstat -ltp
```

`yaAGC` is listening on many different ports. If we connect to any of them we will get the following output


```
# nc localhost 19700 | xxd
0004b3a0: 0672 86c9 0473 87df 0d5f 81da 0a41 86e3  .r...s..._...A..
0004b3b0: 0472 82d2 0845 87f4 085f 86d9 0b63 80f0  .r...E..._...c..
0004b3c0: 0361 87ca 036c 83c4 056c 80f6 0d69 84e8  .a...l...l...i..
0004b3d0: 0b6e 84ed 0c67 84d2 0c7d 82d9 0e52 86d0  .n...g...}...R..
0004b3e0: 0949 81e1 0854 82fe 0d53 85fc 0045 83d7  .I...T...S...E..
0004b3f0: 0b43 80d8 017b 80c1 0b54 85f0 0668 82cf  .C...{...T...h..
0004b400: 0b65 80c7 0b5f 82f1 0e73 81e0 0874 81fe  .e..._...s...t..
0004b410: 0361 86d5 0672 86c9 0473 87df 0d5f 81da  .a...r...s..._..
0004b420: 0a41 86e3 0472 82d2 0845 87f4 085f 86d9  .A...r...E..._..
0004b430: 0b63 80f0 0361 87ca 036c 83c4 056c 80f6  .c...a...l...l..
0004b440: 0d69 84e8 0b6e 84ed 0c67 84d2 0c7d 82d9  .i...n...g...}..
0004b450: 0e52 86d0 0949 81e1 0854 82fe 0d53 85fc  .R...I...T...S..
0004b460: 0045 83d7 0b43 80d8 017b 80c1 0b54 85f0  .E...C...{...T..
0004b470: 0668 82cf 0b65 80c7 0b5f 82f1 0e73 81e0  .h...e..._...s..
0004b480: 0874 81fe 0361 86d5 0672 86c9 0473 87df  .t...a...r...s..
0004b490: 0d5f 81da 0a41 86e3 0472 82d2 0845 87f4  ._...A...r...E..
0004b4a0: 085f 86d9 0b63 80f0 0361 87ca 036c 83c4  ._...c...a...l..
```

This data will constantly by writing out. Every 4th byte is a character in the flag.

Final flag: `RITSEC{The_stars_ArE_calling}` 