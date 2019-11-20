# RITSEC CTF: the_doge

**Category:** Forensics
**Points:** 100

**Description:**

> Steganography is the practice of concealing messages or information within other nonsecret data and images. The doge holds the information you want, feed the doge a _treat_ to get the hidden message.

## Write-up

The passphrase: treat

The flag: RITSEC{hAppY_l1L_doG3}

The embedded data can be extracted using steghide. 

Steghide is a steganography program that is able to hide data in various kinds of image- and audio-files. The color- respectivly sample-frequencies are not changed thus making the embedding resistant against first-order statistical tests

```bash
$ steghide extract -sf the_doge.jpg
Enter passphrase: 
wrote extracted data to "doge_ctf.txt".
```

```bash
$ steghide extract -sf the_doge.jpg -xf <outputFile>
Enter passphrase: 
wrote extracted data to "output.txt".
```
