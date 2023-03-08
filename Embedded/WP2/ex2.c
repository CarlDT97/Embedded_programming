// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 2
// Exercise 2
// Submission Code 

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
 
// Constants
#define MAX 5 
#define MAX_RANDOM 100  
 
// typedefs
typedef struct q { 
    int number; 
    struct q *next; 
    struct q *prev; 
} REGTYPE; 
 
// Function declarations
 
REGTYPE* random_list(void); 
REGTYPE* add_first(REGTYPE* temp, int data);
void print_list(REGTYPE *list);
 
int main(int argc, char *argv[]) { 

    int nr = 0; 
 
    REGTYPE *act_post , *head = NULL; 
 
    // Testing random_list()

    srand(time(0));  //   Random seed  
    head = random_list(); 
    act_post = head; 
    while(act_post != NULL){ 
        printf("\n Post nr %d : %d" , nr++, act_post->number); 
        act_post = act_post->next; 
    }   
 
    // Testing add_first()

// Create new list using the random_list function
    REGTYPE *new_list = random_list();

    printf("\n\nList before:\n");
//Print new list using print_list function
    print_list(new_list);
//Create new list and add new record to the first position in the linked list
    new_list = add_first(new_list, 10); // setting the list equal to the new head
    printf("\nList after:\n");
    print_list(new_list);
 
 // --- Free the allocated memory  --- 
 
    while((act_post=head) != NULL){ 
        head=act_post->next; 
        free(act_post); 
    } 
 
    return 0; 
} 

REGTYPE *random_list(void){ 
    int nr, i = 0; 
    // Top pointer refers to the head of the linked list
    // Old is used to point to the previous node to be able to set the next pointer to the previous node to the current node.
    REGTYPE *top, *old, *item;
    
    for(; i < MAX; i++) {
        nr = rand() % (MAX_RANDOM + 1); // generating number from 0 to 100 (modulo 101 required)
        item = (REGTYPE*)malloc(sizeof(REGTYPE)); // allocate memory for current entry
        item->number = nr; //assign the number variable of the node to the randomly generated number
        item->next = NULL; //the next value of the node is kept as NULL until the subsequent node is created
        // In the first iteration, we set both prev and next to NULL
        if(i == 0) {
            // As this is the first node in the list, there is no previous node to point to
            item->prev = NULL;
            // The top (or head) is set to the current item
            top = item;
        } else {
            // In subsequent interations, we set prev equal to the old entry, and update the old entry's next
            item->prev = old;
            // The previous nodes next pointer is set to the current node
            old->next = item;
        }
        // At the end of the loop, we need to update the old entry with the entry we just processed
        old = item;
    }
    // Return the head
    return top;
} 
 
REGTYPE *add_first(REGTYPE *temp, int data){ 

    // Allocate space for the new head
    REGTYPE *new_entry = (REGTYPE*)malloc(sizeof(REGTYPE));
    
    // Manipulate the new entry to be the new head
    new_entry->number = data;  // Set new entry number to the provided number
    new_entry->prev = NULL; // Set previous pointer to null as there will be no previous node as the new entry is the head
    new_entry->next = temp; // Set next pointer to the previous head

    // Update the old head's prev
    temp->prev = new_entry;

    return new_entry;
}

void print_list(REGTYPE *list) {

    // Same logic as in the main program. Iterate as long as the next element doesnt equal NULL and keep printing
    int nr = 0;
    REGTYPE *temp = list;
    while(temp != NULL) {
        printf("Entry %d: %d\n", nr++, temp->number);
        temp = temp->next;
    }

}