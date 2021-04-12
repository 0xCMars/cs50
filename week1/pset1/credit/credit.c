#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long creditNumber = get_long("Number: ");

    // calculate the number length
    int length = 0;
    long number = creditNumber;
    while (number != 0)
    {
        number = number / 10;
        length++;
    }

    // check the length is valid or not
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    // checksum
    // multiTotal mean the total of the digit number
    int multiTotal = 0;
    int oddNumber = 0;
    int evenNumber = 0;
    number = creditNumber;

    // each time we deal with the last two number, multi and add at the same time
    while (number != 0)
    {
        // get the last number first
        oddNumber = number % 10;
        // get the second to last number
        evenNumber = (number % 100) / 10 * 2;
        // what we need is evenNumber digits not themselves
        if (evenNumber >= 10)
        {
            evenNumber = evenNumber / 10 + evenNumber % 10;
        }
        // add those together
        multiTotal = oddNumber + evenNumber + multiTotal;

        number = number / 100;
    }

    // determine what is the result of creditNumber
    if (multiTotal % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        if (length == 13)
        {
            int firstNumber = creditNumber / 1000000000000;
            if (firstNumber == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (length == 15)
        {
            int firstNumber = creditNumber / 10000000000000;
            if (firstNumber == 34 || firstNumber == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (length == 16)
        {
            int firstNumber = creditNumber / 100000000000000;
            if (firstNumber > 50 && firstNumber < 56)
            {
                printf("MASTERCARD\n");
            }
            // because visa card is beging with 4, which mean the first two digits
            // greater than 39 less than 50
            else if (firstNumber > 39 && firstNumber < 50)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }

}
