thisdict = {
"a": ".-",
"b": ".-.",
"c": "...",
"d": "..-",
"e": ".",
"f": ".---",
"g": ".-..",
"h": ".--",
"i": "..",
"j": "...-",
"k": ".--.",
"l": "....",
"m": ".-.-",
"n": "..--",
"o": "-",
"p": "-.-",
"q": "-.-.",
"r": "--.",
"s": "-..",
"t": "-...",
"u": "-.",
"v": "---.",
"w": "--..",
"x": "-..-",
"y": "--",
"z": "---",
"1": ".----",
"2": "..---",
"3": "...--",
"4": "....-",
"5": ".....",
"6": "----.",
"7": "---..",
"8": "--...",
"9": "-....",
"0": "--.--",
"&": "----"
}

flag = "m0rse&wa5&your&ban3&la5t&y3ar&so&n0w&l3ts&try&bain"
output = ""

for char in flag:
  output += thisdict.get(char)
  output += "--..-."

output = output.replace("-", "D")
output = output.replace(".", "F")
print(output)
