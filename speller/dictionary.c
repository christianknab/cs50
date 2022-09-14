// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

int word_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int hash_value = hash(word);
    node *first_word = table[hash_value];
    for (node *tmp = first_word; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open Dictionary file
    FILE *dictionary_ptr = fopen(dictionary, "r");

    // Check dictionary contains information
    if (dictionary == NULL)
    {
        return 1;
    }

    // Stores the next word from the dictionary file
    char next_word[LENGTH + 1];

    // Scans dictionary
    while (fscanf(dictionary_ptr, "%s", next_word) != EOF)
    {
        // Creates space for a new node
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        // Copies the current word in dictionary to the node's attribute "word"
        strcpy(n->word, next_word);

        // Stores a hash code
        int hash_index = hash(next_word);

        // Points to the first element in the table at that index
        n->next = table[hash_index];

        // The index of the table now points towards the node
        table[hash_index] = n;

        // Increment word counter
        word_counter++;
    }

    //Close Dictionary file
    fclose(dictionary_ptr);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (word_counter > 0)
    {
        return word_counter;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];

        while (n != NULL)
        {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }

        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }

    return false;
}
