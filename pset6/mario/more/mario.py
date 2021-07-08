while True:
    try:
        height = int(input("Height: "))
        if (height > 0 and height < 9):
            break
    except ValueError:
        print("Input integer value")

for i in range(height):
    # print blank spaces
    for j in range(height - i - 1):
        print(" ", end="")

    # print left bricks
    for j in range(i + 1):
        print("#", end="")

    # print gap
    print("  ", end="")

    # print right tree
    for j in range(i + 1):
        print("#", end="")
    print()