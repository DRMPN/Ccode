// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Funciton prototypes
void unload_helper(node *n);

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Number of words in dicitonary
int dic_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get word's first letter
    char c = tolower(word[0]);
    // Determine word's bucket
    int b = hash(&c);

    // Search for the word in dictionary
    for (node *i = table[b]; i != NULL; i = i->next)
    {
        if (strcasecmp(i->word, word) == 0)
        {
            // Found word
            return true;
        }
    }

    // The word is not in a dictionary
    return false;
}

// Hashes word to a number
// Assume every word starts from lowercase letter
unsigned int hash(const char *word)
{
    return word[0] - 97;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dic = fopen(dictionary, "r");
    if (dic == NULL)
    {
        return false;
    }

    // Read strings from a file one at a time
    char buffer[LENGTH + 1];
    while (fscanf(dic, "%s", buffer) != EOF)
    {
        // Create a new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copy buffered word in a node
        strcpy(n->word, buffer);
        n->next = NULL;

        // Hash word to obtain a bucket value
        int b = hash(buffer);

        // insert node into hash table at that location
        n->next = table[b];
        table[b] = n;

        // count words in dictionary
        dic_words++;
    }

    // Close dictionary file
    fclose(dic);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dic_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        unload_helper(table[i]);
    }

    return true;
}

// Frees node in a linked list one by one
void unload_helper(node *n)
{
    if (n != NULL)
    {
        unload_helper(n->next);
        free(n);
    }
}