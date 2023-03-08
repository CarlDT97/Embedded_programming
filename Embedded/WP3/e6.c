// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 3
// Exercise 6
// Submission Code: 32720

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyString(char new_string[], const char input[]) // Our function for copying a string
{
    int i;                             // Decleare the integer i used in for loop.
    for (i = 0; input[i] != '\0'; i++) // We iterate trough every index of source
    {
        new_string[i] = input[i]; // Assign new_string[i] to input[i]
    }
    new_string[i] = '\0';
}

int main(int argc, char *argv[])
{
    char stringOne[20];   // Declear the string that we use for scanf
    char stringCopy[20];  // Declear the variable string which we use for the library copy
    char option;          // Variable decleration for option for the if statement

    if (argc > 1)
    {
        char string[20];
        fgets(string, 20, stdin);
        printf("Read: %s\n", string);
        return 0;
    }
    else if(argc == 1) // if option is '2' we run the second part of our program.
    {
        printf("Enter a string : "); // print statement
        scanf("%s", stringOne);      // scan for stringOne

        strcpy(stringCopy, stringOne);                                  // Copy stringOne and declear it to string copy
        printf("Copied string using library strcpy: %s\n", stringCopy); // Print the copied string

        copyString(stringCopy, stringOne);                           // Here we use our function, we take stringOne and return stringcopy
        printf("Copied string using my function: %s\n", stringCopy); // print the copied string
    }
    return 0;
}