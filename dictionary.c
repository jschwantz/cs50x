// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 5381

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned long hash(const char *word)
{
    //return tolower(word[0]) - 'a';
    // Found hash function below, doing mod 5381
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
        int c;

        while ((c = tolower(*word++)))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % 5381;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a new node for the word being scanned in
        node *new_node = malloc(sizeof(node));
        new_node->next = NULL;

        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);

        // Get the hash value for the word
        long hashkey = hash(word);

        // If this linked list has already been started, add to it; otherwise start it
        if (hashtable[hashkey])
        {
            new_node->next = hashtable[hashkey];
            hashtable[hashkey] = new_node;
        }
        else
        {
            hashtable[hashkey] = new_node;
        }

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int num_words = 0;

    // traverse over the linked lists for all initialized buckets, counting number of words
    for (int i = 0; i < N; i++)
    {
        if (hashtable[i])
        {
            for (node *ptr = hashtable[i]; ptr != NULL; ptr = ptr->next)
            {
                num_words++;
            }
        }
    }

    return num_words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // set the node for whichever bucket this word returns from the hash
    node *ptr = hashtable[hash(word)];
    while (ptr != NULL)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop over lists unlinking after setting the node to a temporary list
    for (int i = 0; i < N; i++)
    {
        if (hashtable[i])
        {
            node *ptr = hashtable[i];
            if (!ptr)
            {
                return false;
            }
            while (ptr != NULL)
            {
                node *temp = ptr;
                if (!temp)
                {
                    return false;
                }
                ptr = ptr->next;
                free(temp);
            }
        }
    }

    return true;
}
