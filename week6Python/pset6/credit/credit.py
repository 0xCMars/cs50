from cs50 import get_string
import re


def main():
    getnumber = get_string("Number: ")
    if(len(getnumber) not in (13, 15, 16)):
        print("INVALID")
    multiTotal = 0
    evenNumber = 0
    # reversed the string
    number = getnumber[::-1]
    # We start with the lowest number
    for i in range(len(number)):
        if i % 2 == 1:
            # print(f"i: {i}")
            evenNumber = int(number[i]) * 2
            # print(f"evenNumber: {evenNumber}")
            if evenNumber >= 10:
                evenNumber = evenNumber % 10 + evenNumber // 10
            # print(f"tran evenNumber: {evenNumber}")
            multiTotal += evenNumber
        else:
            # print(f"i: {i}")
            # print(f"oddNumber: {number[i]}")
            multiTotal += int(number[i])
        # print(f"{multiTotal}")

    # print(f"{multiTotal}")
    if (multiTotal % 10 != 0):
        print("INVALID")
    else:
        # use regular expressions to validate
        if re.search("^3(4|7)", str(getnumber)):
            print("AMEX")
        elif getnumber[0] == str(4):
            print("VISA")
        elif re.search("^5(1|2|3|4|5)", str(getnumber)):
            print("MASTERCARD")


main()
