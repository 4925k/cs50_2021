def luhnsAlgorithm(card):
    # different sums for the positions
    sum1 = 0
    sum2 = 0
    place = True    # keep track of the digit from last
    # start fromt the last digit
    for i in reversed(card):
        if place:
            sum1 += int(i)
            place = False
        else:
            temp = int(i) * 2
            if temp > 9:
                temp = temp % 10 + 1
            sum2 += temp
            place = True

    totalSum = sum1 + sum2

    if totalSum % 10 == 0:
        return True

    return False

# check the length of the card. return true if ok.


def checkLength(card):
    if len(card) > 12 and len(card) < 17:
        return True
    return False


def main():
    # get proper input from user
    while True:
        try:
            cc = int(input("Number: "))
            break
        except ValueError:
            print("Input Integer Value")

    # convert to string for easy manipulations
    cc = str(cc)

    # check if the number fulfills lunhs algorithm and length
    if luhnsAlgorithm(cc) and checkLength(cc):
        if cc[0] == "4":
            print("VISA")
        elif cc[0] == "3":
            if cc[1] == "4" or cc[1] == "7":
                print("AMEX")
        elif cc[0] == "5":
            if int(cc[1]) > 0 and int(cc[1]) < 6:
                print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


main()