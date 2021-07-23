// Implements a dictionary's functionality
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];
unsigned int DictSize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    if (word == NULL)
    {
        return false;
    }
    unsigned int firstWordNum = hash(word);
    node *point = table[firstWordNum];
    while (point != NULL)
    {
        if (strcasecmp(word, point->word) == 0)
        {
            return true;
        }
        point = point->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    if (word == NULL)
    {
        return false;
    }
    int num = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        //Make the hash values more sparse
        num += (tolower(word[i]) * i);
    }

    return num % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    char *word = malloc(LENGTH + 1);
    //printf("%p", table[0]);
    //can not use fgets, it will read the new line
    while (fscanf(dict, "%s", word) != EOF)
    {
        if (isalpha(word[0]) != 0)
        {
            unsigned int hashNum = hash(word);

            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                free(n);
                return false;
            }
            strcpy(n->word, word);
            n->next = table[hashNum];
            table[hashNum] = n;
            DictSize++;
        }
    }
    free(word);
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return DictSize;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int i = 0;
    while (i < N)
    {
        node *del = table[i];
        while (del != NULL)
        {
            del = del->next;
            free(table[i]);
            table[i] = del;
        }
        i++;
    }
    return true;
}
