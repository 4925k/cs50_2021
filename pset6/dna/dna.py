import sys
import csv


def main():
    # check for correct number of arguments
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py <csvfile> <textfile>")

    # read str counts into memory
    strList = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            strList.append(row)

    # read dna sequences into memory
    dnaSequence = ""
    with open(sys.argv[2], "r") as file:
        dnaSequence = file.read().rstrip()

    # a dictionary to keep count of the provided str in the dna sequence
    strCount = {}
    for key in strList[0]:
        strCount[key] = 0
    strCount.pop("name")

    # count the str in dna sequnce
    # loop each key
    for key in strCount:
        length = len(key)   # length of key
        temp = 0
        # loop over sequence to compare key
        for i in range(len(dnaSequence)):
            # see if sequence matches key
            # if it does, it checks if there is next sequence
            # and keeps count to
            while key == dnaSequence[i: i + length]:
                temp += 1
                i = i + length
            if temp > strCount[key]:
                strCount[key] = temp
            temp = 0            # temp to store current max sequence

    name = "No match"
    # loop over each individual
    for row in strList:
        match = True
        # loop over each str
        for key in row:
            # skip name
            if key == "name":
                continue
            # compare count
            if strCount[key] != int(row[key]):
                match = False
        if match == True:
            name = row["name"]
            break

    print(name)


main()
