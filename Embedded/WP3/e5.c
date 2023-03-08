// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 3
// Exercise 5
// Submission Code: 32720

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 99 // Define our MAX value which we use in the random functions.
#define MIN 1 // Define the MIN
#define MAXIMUM 10

int main()
{
    srand(time(NULL));
    printf("Array lenght: %d \n", MAXIMUM); //Print the length of the array

    int array[MAXIMUM]; // Declear the array and set the size to the random length.

    for (int i = 0; i < MAXIMUM; i++) // We iterate trough the array
    {
        array[i] = rand() % MAX + MIN; //Then we assign each index with a random number from 1-99.
        printf("position: %p | Element: %d \n", &array[i], array[i]); //Print the adress of each element with its value.
    }

    printf("-----Each element multiplied by two------\n \n"); // Devider for the terminal

    for (int i = 0; i < MAXIMUM; i++) //We iterate trough the array
    {

        printf("position: %p | Element: %d \n", &array[i], array[i] * 2); // Print out the position of each element and muliply it by 2
    }

    printf("------------------------------------\n \n"); // Divider for better readability

    printf("- The value of the address of the array (pointer) is:  %p\n", array); // Print out the pointer for the array
    printf("- First integer in the array is: %d \n", array[0]); // We take the first element of the array and print it, there is always a index[0];
    printf("- The last integer in the array is: %d \n", array[MAXIMUM - 1]); //Print the last element of the array
    printf("- The size of an integer in bytes is: %lx \n", sizeof(array[0])); //We print the byte size of the first integer of the array
    printf("- The size of the whole array in bytes is: %lx \n", sizeof(array)); //Printing the byte size of the array

    return 0;
}