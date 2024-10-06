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
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 260;

// Hash table
node *table[N];

// Declare word count for dictionary
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the text word
    unsigned int text_index = hash(word);

    // Create temp node called Trav to traverse through the linked list
    node *Trav;

    // Set Trav to the header of indexed linked list in the hash table
    Trav = table[text_index];

    // If the current Trav == NULL then it completed the list
    // Completed list when last node is null, not Trav-> since the last node has a ->next = null and
    // we want to check the last node
    while (Trav != NULL)
    {
        // Check if word exists at current Trav node
        if (strcasecmp(Trav->word, word) == 0)
        {
            return true;
        }
        // Move Trav node to next in the linked list
        Trav = Trav->next;
    }
    // If not in the linked list, then it is misspelled
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hash based on ASCII values in word
    unsigned int hash = 0;
    // Loop through chars in the word
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash += toupper(word[i]);
    }
    // Ensure the hash value does not extend outside the bounds of N
    hash = hash % N;
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dictionary_file = fopen(dictionary, "r");
    // Return false if dictionary file fails to open
    if (dictionary_file == NULL)
    {
        return false;
    }

    // PUT DICTIONARY INTO HASH TABLE

    // Declare word variable with max length
    char new_word[45];

    // Read strings from file one at a time
    // fscanf returns EOF at end of file (new_word is address of where that string is stored)
    while (fscanf(dictionary_file, "%s", new_word) != EOF)
    {
        // Create a new node for each new word by allocating memory
        node *n = malloc(sizeof(node));
        // Check if memory is successfully allocated
        if (n == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }

        // Put the new word (pointer) and pointer to next node into the node
        strcpy(n->word, new_word);
        n->next = NULL;

        // Hash word to obtain a hash value (element in table array of nodes)
        int hash_index = hash(new_word);

        // If no node already exists, make n node the header at this index
        // Table[hash_index] is the header of the linked list
        if (table[hash_index] == NULL)
        {
            // Copy the n pointer to table node so both are pointers to address of new_word
            table[hash_index] = n;
        }
        // If node already exists, insert new node to point at current header node
        else
        {
            // Make new node point to current header node
            n->next = table[hash_index];
            // Update header node to be the new node
            table[hash_index] = n;
        }
        // Increment word count
        word_count++;
    }
    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Check word count is 0, then no words have been loaded
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Create tmp node for free function, NOT cursor which will track elements in list
    node *tmp = NULL;

    // Loop through hash table looking for headers with memory
    for (int i = 0; i < N; i++)
    {
        // Check each index in table for a header node
        if (table[i] != NULL)
        {
            // Set cursor equal to the found header in hash table
            node *cursor = table[i];

            // FREE EACH NODE IN LINKED LIST
            // Loop through all nodes in the linked list
            while (cursor != NULL)
            {
                // Set tmp equal to cursor which is equal to the header of linked list
                tmp = cursor;

                // Move cursor to next element through linked list
                cursor = cursor->next;

                // Free node that tmp is still pointing to
                free(tmp);

                // Update tmp to move to next element (where cursor is pointing)
                tmp = cursor;
            }
        }
    }
    return true;
}
