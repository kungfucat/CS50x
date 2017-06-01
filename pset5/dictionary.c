/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include<stdlib.h> //needed for malloc()
#include<ctype.h> //for isalpha() function
#include<stdio.h>
#include<string.h> //for strlen() in check() function
#include "dictionary.h"

// IMPLEMENTING TRIE
typedef struct node
{
    bool is_word;
    struct node* children[27];
} node;

node* root;
unsigned int word_counter=0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    int i;
    if(root==NULL)
    {   
        printf("Dictionary is not loaded properly as first character did not exist.");
        return false;
    }
    node*current=root;
    for(i=0;i<strlen(word);i++)
    {
        // need to convert it to lower case first before we proceed
        int c=tolower(word[i]);
        if(isalpha(c)||c=='\'')
        {
            if(c=='\'')
               c=26;
            else
               c=c-97;
        }
        //Moving through the nodes for this letter, and making sure it isn't NULL
        if(current->children[c]==NULL)
           return false;
        // if it is not NULL, move to the next chunk of 27 characters   
        else
           current=current->children[c];
    }
    // checked whether the last character is indeed the end of the word
    if(current->is_word==true)
       return true;
    else
       return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    root=malloc(sizeof(node)); //using malloc in global gave error- initializer element is not a compile-time constant, using here removed the error
    
    //current is required to check on which chunk of 27 characters we currently are
    node* current=root;
    int c;
    FILE *fp=fopen(dictionary,"r");
    if(fp==NULL)
    {
        printf("\n FILE COULD NOT BE OPENED!!");
        return false;
    }    
    for(c=fgetc(fp);c!=EOF;c=fgetc(fp))
    {
        if(isalpha(c)||c=='\'')
        {
            if(c=='\'')
                c=26;
            else
                c=c-97;
        
           //make a new node(or block of 27 characters) if children pointer is not allocated
           if(current->children[c]==NULL)
           {
               //so basically set up an entire line of 27 characters at children[c]
               current->children[c]=malloc(sizeof(node));
           }
           //move the current position to the character from the list of characters in children[]
           current=current->children[c];
        }
        else if(c=='\n')
        {
            // reached the end of the word, so setting the smiley, in other words, setting true, denoting the end of the word
            current->is_word=true;
            //setting back to the first chunk of 27 characters for the next word
            current=root;
            //+1 the number of words as one more word has been loaded now
            word_counter++;
        }
    }
    
    fclose(fp);
    //loaded the dictionary
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return word_counter;
}

void deallocater (node* pt)
{
    //Modified the last form, now we recursively call deallocater for each children[i], if it is not NULL, as we need not waste time on freeing them up
    int i;
    for(i=0;i<27;i++)
    {
         if(pt->children[i]!=NULL)
            deallocater(pt->children[i]);
    }
    free(pt);
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    deallocater(root);
    return true;
}
