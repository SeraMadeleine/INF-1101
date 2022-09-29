#include <set.h>
#include <list.h> 
#include <stdlib.h>
#include <common.h>
#include <printing.h>

// Make a struct for a set 
struct set {
    cmpfunc_t cmpfunc; 
    list_t *list;  
}; 

// Make a struct for the set iter  
struct set_iter{
    list_iter_t *list_iter; 
};

// Creates a ampty new set using the given comparison function to compare elements of the set.
set_t *set_create(cmpfunc_t cmpfunc){
    set_t *set = malloc(sizeof(set_t));  // allocate memory 
    // Checks if there is room to allocate memory
    if (set == NULL)
    {
        return NULL;
    } 
    set->list = list_create(cmpfunc);  // Use the list create function to make a set 
    set->cmpfunc = cmpfunc; 

    return set; // Returns the set 
}

void set_destroy(set_t *set){
    list_destroy(set->list);    //  Destroys the given set by using the list function
    free(set);  // Free the set after destroying everything in the set 
}

int set_size(set_t *set){
    int size = list_size(set->list);
    return size;   // Return the size of the list  
}

void set_add(set_t *set, void *elem){
    // Checks that the set does not contain the element 
    if (!list_contains(set->list, elem))
        {
        list_addfirst(set->list, elem); 
        }
        
    list_sort(set->list);   // Sort the element 
}

// Returns 1 if the given list contains the given element, 0 otherwise.
int set_contains(set_t *set, void *elem){
    int a = list_contains(set->list, elem); // Checks if the set contains an element 
    return a; 
}

set_t *set_union(set_t *a, set_t *b){
    // Creates a set that contains all the elements from two given sets
    set_t *union_AB = set_create(a->cmpfunc);    
    set_iter_t *iter_A = set_createiter(a);     // Iter to iterate through the set A 
    set_iter_t *iter_B = set_createiter(b);     // Iter to iterate through the set B

    while(set_hasnext(iter_A))                  // As long as the set A has a next element 
    {
        void *elem_A = set_next(iter_A);        // Iterate to the next element in the set
        // If the set does not contain the element, add the element to the set
        if(!set_contains(union_AB, elem_A))   
        {
            set_add(union_AB, elem_A); 
        } 
    }

    while (set_hasnext(iter_B))                 // As long as the set B has a next element 
    {
        void *elem_B = set_next(iter_B);       // Iterate to the next element in the set
        // If the set does not contain the element, add the element to the set
        if(!set_contains(union_AB, elem_B))    
        {
            set_add(union_AB, elem_B); 
        } 
    }
    // Destoy the iterator to free memory
    set_destroyiter(iter_A);    
    set_destroyiter(iter_B); 
    return union_AB; 
}

set_t *set_intersection(set_t *a, set_t *b){
    set_t *intersect_AB = set_create(a->cmpfunc); 
    set_iter_t *iter_A = set_createiter(a);     // Iter to iterate through the set A 
    // set_iter_t *iter_B = set_createiter(b);     // Iter to iterate through the set B

    while(set_hasnext(iter_A))
    {
        void *elem_A = set_next(iter_A);        // Iterate to the next element in the set
        // If set B contains the given element from A, add the element to the new set
        if(set_contains(b, elem_A))
        {
            set_add(intersect_AB, elem_A); 
        }
    }
    // Destoy the iterator to free memory
    set_destroyiter(iter_A); 
    // set_destroyiter(iter_B); 
    return intersect_AB; 
}

set_t *set_difference(set_t *a, set_t *b){
    // Returns a set of the elements that set containing the elements found in A but not in B. 
    set_t *difference_AB = set_create(a->cmpfunc); 
    set_iter_t *iter_A = set_createiter(a);     // Creates an iter that can iterate through set a
    // set_iter_t *iter_B = set_createiter(b);     // Creates an iter that can iterate through set b

    while(set_hasnext(iter_A))
    {
        void *elem_A = set_next(iter_A);        // Iterate to the next element in the set
        // If the element is in set A but not in set B, add the element 
        if(set_contains(a, elem_A) && !set_contains(b, elem_A))
        {
            set_add(difference_AB, elem_A); 
        }
    }
    // Destoy the iterator to free memory
    set_destroyiter(iter_A); 
    // set_destroyiter(iter_B); 
    return difference_AB; 
}

set_t *set_copy(set_t *set){
    // Creates a new set containging every elemnt in a given set, by using iterators
    set_t *newset = set_create(set->cmpfunc); // make a new set 
    set_iter_t *iter = set_createiter(set); // Iter to iterate through the new set
    void *new_elem = set_next(iter); // Iterate to the next element in the set

    while (new_elem != NULL) // As long as the end of the list is not reached
    {
        // If the set dont contain the new element, add it 
        if (!set_contains(newset, new_elem))  
        {
            set_add(newset, new_elem); 
        }
        new_elem = set_next(iter);  // Iterate to the next element in the set
    }
    // Destoy the iterator to free memory
    set_destroyiter(iter);
    return newset;
}

// Creates a new set iterator 
set_iter_t *set_createiter(set_t *set){
    set_iter_t *iter = malloc(sizeof(set_t));
    if(iter == NULL)
        return NULL; 

    iter->list_iter = list_createiter(set->list);
    return iter; 
}

// Function to destory a set iterator to free memory
void set_destroyiter(set_iter_t *iter){
    free(iter->list_iter);
    free(iter);     // fri the iterator 
}

// Function to see if you have reacht the end of the set 
int set_hasnext(set_iter_t *iter){
    int a = list_hasnext(iter->list_iter); 
    return a; 
}

// Return the next element in the set 
void *set_next(set_iter_t *iter){
    void *next = list_next(iter->list_iter);
    return next;
}
