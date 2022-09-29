#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include "index.h"
#include "map.h"
#include "printing.h"
#include "trie.h"


// The index structure 
struct index
{
    trie_t *trie;       // A trie that should include all of the terms
    list_t *documents;  // A list of all the document names
};

// A struct for the search result
struct search_result
{
    int size;  // The size of the document 
    char *found_search; // The query from a search 
    list_t *document_list; // A list that contains all the documents 
    list_iter_t *doc_iter; // A iterator for the documents 
    list_iter_t *pos_iter; // An iterator that iterates through all of the word locations.
    char **array; 
    int position;
};

// A struct for the documents. This is created so both the search result 
// and the index stuct can be reached 
struct document 
{
    char *document_names; // The name of a given document 
    int size;             // The size of the document 
    char **array;         // An array containg all the words from a document 
};

// Compare to ints
static inline int cmp_ints(void *a, void *b)
{
    return *((int *)a) - *((int *)b);
}

// Compares two strings without case-sensitivity 
static inline int cmp_strs(void *a, void *b)
{
    return strcasecmp((const char *)a, (const char *)b);
}

index_t *index_create()
{
    // Allocate memory and check to see whether there is enough space.
    index_t *index = malloc(sizeof(index_t));    
    if (index == NULL)                   
    {
        ERROR_PRINT("Out of memory");   
        return NULL;
    }
    
    // Make a trie, hash map, and list for the index. The trie and hash map  
    // will hold all of the words from all of the text files, and the list 
    // will hold the names of all of the documents in the index.
    index->trie = trie_create();        
    index->documents = list_create(cmp_strs);  
    return index; 
}

void index_destroy(index_t *index)
{
    // Destoy the trie, map and list, and free the index 
    trie_destroy(index -> trie);    
    list_destroy(index->documents); 
    free(index);  
}

void index_add_document(index_t *idx, char *document_name, list_t *words)
{
    // Allocate memory for the document struckt and check to see whether there is enough space.
    document_t *document = malloc(sizeof(document_t));  
    if (document == NULL)               
    {
        ERROR_PRINT("Out of memory");   
        return; 
    }

    //  Allocate memory for an array which should contain all the words in one textfile, and check to see whether there is enough space.
    document->array = malloc(list_size(words)*sizeof(char*)); 
    if (document->array == NULL)               
    {
        ERROR_PRINT("Out of memory");   
        return; 
    } 
    
    document->document_names = (char *)document_name;   // Add the documentname to the document struct 
    document->size = list_size(words);  // Find the size of the words list and save it in the document struct 
    

    list_iter_t *iter = list_createiter(words);  // creates an iter that iterates over a list 
    int i = 0;  // A varible to find the position of the word 

    while(list_hasnext(iter))
    {

        char *tmp_word = list_next(iter);  // An iterator for the words in the given document 
        
        // Add the tmp word to the array 
        document->array[i] = tmp_word;
        // Add the word to the trie

        trie_insert(idx->trie, tmp_word, i); 
        i++;  // Increase the counter 
    }

    list_addlast(idx->documents, document);     
    list_destroyiter(iter); 
}

search_result_t *index_find(index_t *idx, char *query)
{
    // Allocate memory for the search struckt and check to see whether there is enough space.
    search_result_t *search_result = malloc(sizeof(search_result_t)); 
    if (search_result == NULL)                    
    {
        ERROR_PRINT("Out of memory");   
        return NULL;
    }

    search_result->document_list = idx->documents;      // Add the douments from the index to the searchresult 
    search_result->doc_iter = list_createiter(search_result->document_list); // Create an iterator 
    search_result->found_search = query;                // Saves the query as a search 
    // search_result->position = map_get(idx->map, query); // Retrieve the location for a given query from the map

    return search_result;   // Return the struct 
}

char *autocomplete(index_t *idx, char *input, size_t size)
{
    int s = 3;  // The minimum size of the input  
    if (strlen(input) >= s) 
    {   
        // If the input does not exist, print an error.
        if((trie_find(idx->trie, input))== NULL) {
            ERROR_PRINT("Not found in the index"); 
            return NULL; 
        }
        // Find a term that matches the input if the conditions are met and the input exists.
        return trie_find(idx->trie, input); 
    }
    // If the input is invalid, print an error message. 
    else{
        ERROR_PRINT("The input is invalid");
        return NULL;
    }
}

char **result_get_content(search_result_t *res)
{
    res->position = 0;
    // Checks to see whether there are any more items in the list.
    if (!list_hasnext(res->doc_iter)){
        ERROR_PRINT("\n There is no next item in the list \n");
        return NULL; 
    }

    // Assign the next item in the list to the document struct
    document_t *res_next = list_next(res->doc_iter);

    // Set the size of the document struct to match the size of the search result struct.
    res->size = res_next->size; 
    res->array = res_next->array;

    // Return the array of words 
    return res->array;
}

int result_get_content_length(search_result_t *res)
{
    return res->size;
}

search_hit_t *result_next(search_result_t *res)
{
    // trenger en lengde og en posisjon!! 
    search_hit_t *search_hit = malloc(sizeof(search_hit_t)); 

    // Allocate memory for the search hit struckt and check to see whether there is enough space.
    if (search_hit == NULL)                  
    {
        ERROR_PRINT("Out of memory");   
        return NULL;
    }

    // As long as the posistion of the word does not have a greater valu than the lengt of current doucment 
    while (res->position < result_get_content_length(res))
    {
        // Checks if the array is empty 
        if (res->array[res->position] == NULL)
            {
            return NULL;
            }
        
        // If the word of the array is the same as the search wors, set the position and find the length
        if (cmp_strs(res->array[res->position], res->found_search)==0)
        {
            search_hit->location = res->position; 
            search_hit->len = result_get_content_length(res);
            res->position++;

            return search_hit; 
        }
        else
        {
            res->position++;
        }
    }
    return NULL; 
}
