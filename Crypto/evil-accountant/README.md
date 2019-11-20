evil-accountant
===============

A cryptography CTF challenge that uses a Correlation Power Analysis (CPA)
attack to crack AES-128.

Information for CTFd
---------------------

- **Title**: Evil Accountant Attack
- **Author**: neon_spandex
- **Flag**: RITSEC{K0rr3LA710n_CAN_50M371m35_1MpLY_cau5A710N_24544110ad75a4}

In addition to the description below, the `traces.zip` file must be provided to
the CTF participants.

### Description

Your CPA sent you a message encrypted with 128-bit AES, but refuses to send you the key to decrypt it. A few days later, their maid sent you a strange JSON object in an email, and they CC'd someone named Pearson.

Ciphertext: 5z+joNqYQhpXz/2Njz+/ePD8m8+A2S3ZZ5tWIvRw1ueq4cO9EYgvQjGKeJzFtIZF9hg9/2NGIFQqgDfwxNHUiQ==

There was an issue with the traces, so you get three bytes of key for free:

0x2b????1628??????????????????????

Hint: check out Correlation Power Analysis (CPA).

Writeup
-------

This challenge involves deriving an AES-128 key using a Correlation Power
Analysis attack. The JSON blob provided includes 50 power traces of a single
round of AES on an unspecified board along with the plaintexts that were
encrypted for each of those rounds.

There are a few rough guides on this sort of attack around the internet. The
challenge solver may be able to cobble together a solution using existing tools
but it is possible (and intended) for a solution to be written from scratch -
no external power analysis tools required!

The challenge name is a play on the "evil maid attack" that references the type
of attack required - CPA (which is also a Certified Public Accountant). This
reference/joke continues in the description. Also, the CC line in the email is
a reference to the Pearson correlation coefficient, which will be needed to
perform the attack.

Three bytes of the key are provided in the description because I have been
having issues with my target board - the first, fourth, and fifth bytes are
consistently incorrect for some reason. If the attack is implemented correctly,
those bytes will be calculated as 0xcb, 0xd8, and 0x58 respectively.

### Writeup Files

These files **SHOULD NOT** be made available during the CTF!!! They reveal how
to solve the challenge, and are only included for the sake of a complete PoC.

- `evil_accountant/`: a proof of concept of the CPA attack
- `generate_ciphertext.py`: a helper script to generate the base64 ciphertext
- `run.py`: an entrypoint for the `evil_accountant` PoC

### Resources

- https://wiki.newae.com/Correlation_Power_Analysis
- https://advancedpersistentjest.com/2018/08/02/correlation-power-analysis-vs-aes/
- https://en.wikipedia.org/wiki/Pearson_correlation_coefficient#For_a_sample
- https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.pearsonr.html#scipy.stats.pearsonr