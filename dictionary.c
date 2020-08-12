// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
const unsigned int N = 1000;

// Number of words in dictionary
int dictWords = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char checker[strlen(word)];
    strcpy(checker, word);
    for (int a = 0; checker[a] != '\0'; a++)
    {
        checker[a] = tolower(word[a]);
    }
    int key = hash(checker);
    if (table[key] != NULL)
    {
        node *tmpNode = table[key];
        while (tmpNode != NULL)
        {
            if (strcmp(tmpNode->word, checker) == 0)
            {
                return true;
                tmpNode = tmpNode->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int c;
    int hash = 0;
    for (int a = 0; word[a] != '\0'; a++)
    {
        if (isalpha(word[a]))
        {
            c = word[a] - 'a' + 1;
        }
        else
        {
            c = 27;
        }
        hash = ((hash << 3) + c) % N;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fclose(inptr);
        printf("Could not open the dictionary file.\n");
        return false;
    }
    for (int a = 0; a < N; a++)
    {
        table[a] = NULL;
    }
    char tmpWord[LENGTH + 1];
    while (!feof(inptr))
    {
        fscanf(inptr, "%s", tmpWord);
        dictWords++;
        node *tmpNode = malloc(sizeof(node));
        if (tmpNode == NULL)
        {
            free(tmpNode);
            printf("Error while allocating memory.\n");
            return false;
        }
        strcpy(tmpNode->word, tmpWord);
        int key = hash(tmpWord);
        if (table[key] == NULL)
        {
            tmpNode->next = NULL;
            table[key] = tmpNode;
        }
        else
        {
            tmpNode->next = table[key];
            table[key] = tmpNode;
        }
    }
    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size()
{
    return dictWords - 1;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int a = 0; a < N; a++)
    {
        if (table[a] != NULL)
        {
            node *tmpNode = table[a];
            while (tmpNode != NULL)
            {
                node *freeNode = tmpNode;
                tmpNode = tmpNode->next;
                free(freeNode);
            }
        }
    }
    return true;
}
