// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

//Create hash table and word count
node *table[N];
int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word
    // Index hash table at that hash value

    // Traverse linked list at that index looking for the word one node at a time
    node *cursor = table[hash(word)];

    while (cursor != NULL)
    {

        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    //If not found
    return false;
}

// Hashes word to a number to index into hash table
unsigned int hash(const char *word)
{
    //Convert first letter of word to index
        if (word[0] >= 'a' && word[0] <= 'z')
        {
            int hash = word[0] - 97;
            return hash;
        }


        else if (word[0] >= 'A' && word[0] <= 'Z')
        {
            int hash = word[0] - 65;
            return hash;
        }

        else
        {
            return 0;
        }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //Open dictionary file
    FILE *dict_file = fopen(dictionary, "r");

    if (dict_file == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    //Read strings from dictionary file one at a time
    char file_word[LENGTH];

    while ((fscanf(dict_file, "%s", file_word)) != EOF)
    {
        //Create new node for each word
        //Allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //Copy word into new node and add to word count
        strcpy(n->word, file_word);
        word_count++;

        //Insert new node into hash table at location given by hash function
        n->next = table[hash(file_word)];
        table[hash(file_word)] = n;
    }

    //free memory
    //free(n);
    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //Create two variables to iterate over linked list and free memory, without losing rest of list
    node *cursor;
    node* temp;

    //Iterate over every linked list in hash table
    for (int i = 0; i < N; i++)
    {
        //first node in linked list
        cursor = table[i];
        //node *temp = cursor;

        //loop:
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    free(cursor);
    return true;
}
