from cs50 import get_string


def main():
    text = get_string("Text: ")
    letter = 0
    words = 0
    sentence = 0
    for word in text:
        # Judge the word and count the letter word sentence
        if word.isalpha():
            letter += 1
        # space mean a word is end
        elif word == " ":
            words += 1
        elif word == '.' or word == '!' or word == '?':
            sentence += 1

    # The last word is end with symbol not space
    words += 1

    # count the index
    L = letter / words * 100
    S = sentence / words * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    index = round(index)

    # judge what is output by compare index
    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


main()
