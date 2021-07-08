def main():
    text = input("Text: ")
    index = colemanLiau(text)
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def letter(text):
    count = 0
    # loop over each character
    for c in text:
        # count alphabets
        if c.isalpha():
            count += 1
    return count


def word(text):
    # count is one to include the last word
    count = 1
    # loop over each character
    for c in text:
        # counts a word if it finds a whitespace
        if c == " ":
            count += 1
    return count


def sentence(text):
    count = 0
    # loop over each character
    for c in text:
        # counts a sentence if it finds dot, exclamation or question mark
        if c == "." or c == "!" or c == "?":
            count += 1
    return count


def colemanLiau(text):
    # get number of letters, words and sentences in text
    letters = letter(text)
    words = word(text)
    sentences = sentence(text)

    # calculate avereage numbers of letter and sentences per 100 words
    l = letters / words * 100
    s = sentences / words * 100

    # use the coleman liau formula
    index = 0.0588 * l - 0.296 * s - 15.8

    return round(index)


main()