#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");
    int i = 0;
    int letter_count = 0;
    int word_count = 0;
    int sentence_count = 0;
    while (text[i] != '\0')
    {
        // Judge the text[i] and count the letter word sentence
        if (isalpha(text[i]))
        {
            letter_count++;
        }
        //space mean a word is end
        else if (isspace(text[i]))
        {
            word_count++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
        i++;
    }
    //The last word is end with symbol not space
    word_count++;

    // count the index
    float L = (float)letter_count / word_count * 100;
    float S = (float)sentence_count / word_count * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int index_round = round(index);

    //judge what is output by compare index
    if (index_round > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index_round < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index_round);
    }

}