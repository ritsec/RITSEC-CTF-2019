"""
This script will generate a distances.txt file from the STAR_TABLES that correlates to the
given flag.

Author: knif3
"""
import random
import string

def LoadStars(fn="STAR_TABLES.agc"):
    """
    Convert the star table into a usable dictionary
    """
    star_dict = {}
    with open(fn) as fil:
        data = fil.readlines()

    for line in data:
        if "# STAR " not in line:
            continue
        lined = line.split()
        star = lined[5]
        num = lined[1]
        if star not in star_dict:
            star_dict[star] = []

        num = float(num)
        star_dict[star].append(num)
    return star_dict


def point_distance(star1, star2):
    """
    3d point distance formula
    """
    return (
        (star2[0] - star1[0])**2 +
        (star2[1] - star1[1])**2 +
        (star2[2] - star1[2])**2
    )**(1/2)


def getStarDistances(stars):
    """Get the distance between all the stars.
    Returns a dictionary of star pairs mapped to the distances
    """
    distances = {}
    for i in stars.keys():
        for j in stars.keys():
            if i == j:
                continue
            dist = point_distance(stars[i], stars[j])
            dist = dist*10**2
            dist = int(dist)

            if dist not in distances:
                distances[dist] = []
            distances[dist].append((i, j))
    return distances

def main():
    # Charset is !"%'()*,-.013456789:;<>?@ABCDEFGHIJKLMNOQSUVWXYZ[\]_`abcdefghijklmnopqrstuvwxyz{|}~
    flag = "ritsec{xxxxx_xxxxx_xxxxx_xxxxx}"

    stars = LoadStars()
    distances = getStarDistances(stars)
    charset = ""

    # Validate the flag against the charset.
    # Because we cant change the star numbers, we are only able to use certain characters
    # Thankfully, most of them are within the given dataset
    charset = "".join(sorted(chr(i) for i in distances if chr(i) in string.printable))    
    for char in flag:
        if ord(char) not in distances:
            print("Missing distance for '{}'. Please try another character within the charset".format(char))
            print("\tCharset: ",charset)
            quit(1)

    # Because many star pairs are the same approx distance
    # the same flag can have many different output files
    # This doesnt break anything
    with open("distances.txt", "w") as fil:
        for char in flag:
            stars = random.choice(distances[ord(char)])
            fil.write("Star {} => Star {}\n".format(*stars))
    print("Distances written to 'distances.txt'")
main()