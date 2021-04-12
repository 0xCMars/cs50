#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(int argc, string argv[])
{
    //alpha is used to check the key have duplicate letters or not
    int alpha[26] = {0};

    // Use key array to make the cipher translate map.
    char key[26];

    //check the amount of command-line
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    strcpy(key, argv[1]);
    // check key validity
    if (strlen(key) != 26)
    {
        printf("%lu\n", strlen(key));
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        int i = 0;
        while (key[i] != '\0')
        {
            if (isalpha(key[i]))
            {
                if (alpha[tolower(key[i]) - 97] != 0)
                {
                    printf("key must contain each character once\n");
                    return 1;
                }
                else
                {
                    alpha[tolower(key[i]) - 97]++;
                }
            }
            else
            {
                return 1;
            }
            i++;
        }
    }

    string plaintext = get_string("plaintext: ");
    int i = 0;
    printf("ciphertext: ");
    int index = 0;

    //translate the plaintext to ciphertext based on key array
    while (plaintext[i] != '\0')
    {
        if (isalpha(plaintext[i]))
        {
            //Determine the case of letters, and convert
            if (islower(plaintext[i]))
            {
                index = plaintext[i] - 97;
                printf("%c", tolower(key[index]));
            }
            else
            {
                index = plaintext[i] - 65;
                printf("%c", toupper(key[index]));
            }
        }
        // print the non-alphabet symbol
        else
        {
            printf("%c", plaintext[i]);
        }
        i++;
    }
    //after translating plaintext，add a new line
    printf("\n");

    return 0;
}