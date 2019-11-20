# Lunar Lander 1
CTF Challenge for RITSEC CTF 2019


__Category:__ Misc  
__Difficulty:__ 200  
__Author:__ knif3  
__Description:__  
```
Starman!

We are trying to troubleshoot the guidance computer. You should already have the star tables, so we just need you to double check some of the distances for us. We aren't sure about the precision though so you may need to play with it.

Good luck,
- Houston

Flag format: ritsec{}
```

__Files:__ `distances.txt`

__Hints:__  
> If you lost your copy of the STAR TABLES, you should be able to find the open source version

> https://qz.com/726338/the-code-that-took-america-to-the-moon-was-just-published-to-github-and-its-like-a-1960s-time-capsule/



## Writeup

Given the descriptions, the first step is to realize that we are missing the STAR TABLES so we try to 
find those. By searching around online, its easy to stumble across one of these two repos:

    1. https://github.com/chrislgarry/Apollo-11
    2. https://github.com/virtualagc/virtualagc

By doing a quick search for `STAR TABLES` within either of these repositories, we can find the file
`STAR_TABLES.agc`. 

We can see that there is a list of star with (X, Y, Z) coordinates.

Now by looking at the distances.txt file, we can see start to calculate the distances between the two stars. For example, the first line is `Star 18 => Star 11`.

Star 18 = (-.8608205219, .4636213989, .2098647835)
Star 11 = (.1373948084, .6813398852, .7189566241)

Plug this into the point distance formula:
```
d = ((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)^1/2
```

and we get: 1.1413116044081242


Do this for all the stars:
```
Distance between 18 and 11 = 1.1413116044081242
Distance between 5 and 26 = 1.0597301640471557
Distance between 25 and 5 = 1.1689919577023342
Distance between 1 and 33 = 1.1505951569583577
Distance between 34 and 2 = 1.0169223828043912
Distance between 18 and 25 = 0.99565724132076
Distance between 17 and 9 = 1.2364118929119945
Distance between 28 and 20 = 1.0880181780167246
Distance between 34 and 6 = 1.0183063651011686
Distance between 20 and 15 = 0.9702812898147216
Distance between 36 and 34 = 1.126431558178386
Distance between 17 and 16 = 0.9550531181686516
Distance between 9 and 11 = 0.5292890765461832
Distance between 28 and 12 = 0.9546733937327387
Distance between 13 and 16 = 1.166097811114406
Distance between 32 and 26 = 1.0431713338229598
Distance between 29 and 30 = 0.5110063572320208
Distance between 17 and 16 = 0.9550531181686516
Distance between 7 and 12 = 1.1589305536659942
Distance between 16 and 13 = 1.166097811114406
Distance between 22 and 17 = 0.9768911066069469
Distance between 11 and 18 = 1.1413116044081242
Distance between 1 and 33 = 1.1505951569583577
Distance between 5 and 29 = 1.2579780098487245
```


Now we know that the flag format is `ritsec{}`.

Looking at an ASCII table we can start to see a pattern
```
Dec  Char                           Dec  Char     Dec  Char     Dec  Char
---------                           ---------     ---------     ----------
  0  NUL (null)                      32  SPACE     64  @         96  `
  1  SOH (start of heading)          33  !         65  A         97  a
  2  STX (start of text)             34  "         66  B         98  b
  3  ETX (end of text)               35  #         67  C         99  c
  4  EOT (end of transmission)       36  $         68  D        100  d
  5  ENQ (enquiry)                   37  %         69  E        101  e
  6  ACK (acknowledge)               38  &         70  F        102  f
  7  BEL (bell)                      39  '         71  G        103  g
  8  BS  (backspace)                 40  (         72  H        104  h
  9  TAB (horizontal tab)            41  )         73  I        105  i
 10  LF  (NL line feed, new line)    42  *         74  J        106  j
 11  VT  (vertical tab)              43  +         75  K        107  k
 12  FF  (NP form feed, new page)    44  ,         76  L        108  l
 13  CR  (carriage return)           45  -         77  M        109  m
 14  SO  (shift out)                 46  .         78  N        110  n
 15  SI  (shift in)                  47  /         79  O        111  o
 16  DLE (data link escape)          48  0         80  P        112  p
 17  DC1 (device control 1)          49  1         81  Q        113  q
 18  DC2 (device control 2)          50  2         82  R        114  r
 19  DC3 (device control 3)          51  3         83  S        115  s
 20  DC4 (device control 4)          52  4         84  T        116  t
 21  NAK (negative acknowledge)      53  5         85  U        117  u
 22  SYN (synchronous idle)          54  6         86  V        118  v
 23  ETB (end of trans. block)       55  7         87  W        119  w
 24  CAN (cancel)                    56  8         88  X        120  x
 25  EM  (end of medium)             57  9         89  Y        121  y
 26  SUB (substitute)                58  :         90  Z        122  z
 27  ESC (escape)                    59  ;         91  [        123  {
 28  FS  (file separator)            60  <         92  \        124  |
 29  GS  (group separator)           61  =         93  ]        125  }
 30  RS  (record separator)          62  >         94  ^        126  ~
 31  US  (unit separator)            63  ?         95  _        127  DEL
 ```

```
114  r # 1.1413116044081242
105  i # 1.0597301640471557
116  t # 1.1689919577023342
115  s # 1.1505951569583577
101  e # 1.0169223828043912
 99  c # 0.99565724132076

....
```

We can now see that the floor(distance*100) = the character. 

Repeating this for all the distances and we get the flag:
```
ritsec{leap_4_th3_stars}
```