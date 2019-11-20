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

        #num = float(num) * 10**2
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

def main():
    """Read the stars from 'distances.txt' and figure out the flag
    """
    stars = LoadStars()

    flag = ""
    with open('distances.txt') as fil:
        for line in fil.readlines():
            line = line.split()
            star1 = line[1]
            star2 = line[4]

            distance = point_distance(stars[star1], stars[star2])
            distance = distance*100
            #print("Distance between {} and {} = {}".format(star1, star2, distance))
            flag += chr(int(distance))

    print(flag)

if __name__ == '__main__':
    main()
