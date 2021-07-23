from cs50 import get_int


def main():
    # Prompt for a postive heigh value between 1 and 8
    height = get_int("Height: ")
    while height > 8 or height < 1:
        height = get_int("Height: ")

    # create the pyramids
    for i in range(height):
        print(" " * (height - i - 1), end='')
        print("#" * (i + 1), end='')
        print("  ", end='')
        print("#" * (i + 1), end='')
        print()


main()
