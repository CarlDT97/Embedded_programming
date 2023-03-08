// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 4
// Exercise 4
// Submission Code 

#include <stdio.h> 

// Function declarations
int search_number(int number, int tab[], int size); // Returns the index of the specified number in the parameter or returns -1 if not found
void sort(int size, int tab[]); // Sorts table using selection sort
void swap(int *a, int *b); // Used in the selection sort function to swap values between indexes
void printTable(int tab[], int size); // Prints the sorted table

int main(){
    int test[] = {1,2,34,5,67,3,23,12,13,10};  // Declare test array
    int searchNumber = 13; // Define an integer to find the index of within the table
    int size = sizeof(test)/sizeof(test[0]); // Find size of the test table
    printf("\n%d index before sorting: %d", searchNumber, search_number(searchNumber, test, size)); //Print the index of the number that was searched for
    sort(size, test); // sort the test array
    printTable(test, size); // print the resulting table after sorting
    printf("\n%d index after sorting: %d", searchNumber, search_number(searchNumber, test, size)); //Print the index of the search number after sorting
}

int search_number(int number, int tab[], int size){
    for (int i = 0; i < size; i++)
    {
       if(tab[i]==number){//Compare each element at index i to the number
       //If number at index i is equal to the search number, return index i
           return i;
       }
    }
    //If the number is not found, return -1
    return -1;
}

//Selection Sort
void sort(int size, int tab[]){
    for (int i = 0; i < size - 1; i++) // size - 1 because the first element will be considered the minimum to begin with.
    {
        int minIndex = i; //Set first element to minimum
        for (int j = i + 1; j < size; j++)
        {
            //For each element in the array, compare it to the element at the minimum index
            if (tab[j]<tab[minIndex])
            {
                minIndex=j; //if element at index j is smaller than the minimum,  index j becomes the new minimum index
            }
        }
        //Swap element at minIndex with the element at index j using addressing so that the minimum element is always first in the array
        swap(&tab[minIndex], &tab[i]);
    }
}

void swap(int *a, int *b){
    int temp = *a; // int 'a' is stored in a temp variable to allow the swapping to occur as to not overwrite the value of 'a' while 'a' is given the value of 'b'
    *a = *b; // 'a' is given the value of 'b'
    *b = temp; // 'b' is given the value of 'a' which was stored in the temp variable
}

void printTable(int tab[], int size) {
printf("\nSorted Array: ");
// For all elements in array, print the element at every index
  for (int i = 0; i < size; i++) {
    printf("%d  ", tab[i]);
  }
}
