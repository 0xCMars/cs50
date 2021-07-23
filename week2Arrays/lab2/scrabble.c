#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    int i = 0;
    int UpperWord = 0;
    int score = 0;
    // while loop until word[i] is the end character '\0'
    while (word[i] != '\0')
    {
        // determine if word[i] is a letter or not
        if (word[i] < 65 || (word[i] > 90 && word[i] < 97) || word[i] > 122)
        {
            score += 0;
        }
        else
        {
            // upper all the letter
            if (islower(word[i]))
            {
                UpperWord = toupper(word[i]);
            }
            else
            {
                UpperWord = word[i];
            }
            // use the ascii number to index the array, A is 65, and A is equal to 0 in the array
            score += POINTS[UpperWord - 65];
        }
        i++;
    }
    return score;

}