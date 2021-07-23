#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for a postive heigh value between 1 and 8
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // create the pyramids

    for (int i = height - 1; i >= 0; i--)
    {
        // print the blank
        // the blank number is height - 1, and it decrease when the level down
        for (int j = i; j > 0; j--)
        {
            printf(" ");
        }

        // print the hash
        // hash number is height - blank number
        for (int k = height - i; k > 0; k--)
        {
            printf("#");
        }

        // print the 2 blank
        printf("  ");

        // print the hash in the same way above
        for (int k = height - i; k > 0; k--)
        {
            printf("#");
        }

        // print a new line
        printf("\n");
    }

}