// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
const unsigned int N = 26;

//number of words in the dictionary
int wordCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *trav = table[index];
    while(trav != NULL)
    {
        if (strcasecmp(word, trav->word) == 0)
        {
            return true;
        }
        trav = trav->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = tolower(word[0]) - 97;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    char word[LENGTH + 1];
    //open file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    //loop for words
    while (fscanf(file, "%s", word) != EOF)
    {
        //create new node
        node *n = malloc(sizeof(node));
        if ( n == NULL)
        {
            fclose(file);
            return false;
        }
        //get hash value
        int index = hash(word);

        //copy word to a new node
        strcpy(n->word, word);

        //add current head to new node's next
        //and mkae new node the head
        n->next = table[index];
        table[index] = n;

        //increase word count
        wordCount++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
        if (cursor == NULL && i == N -1)
        {
            return true;
        }
    }
    return false;
}
