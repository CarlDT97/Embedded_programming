// (C) John Webb, Carl Dahlqvist, Ansis Pelpis, group 20 (2023)
// Work package 1
// Exercise 2
// Submission Code: 14620
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void encryption(int shift);

int main(int argc, char *argv[]) // when we run the progam we give an agrument which will set the shift. ex "13"

{
    if (argc < 2 || argc > 2)
    {  // Checks if the number of arguments given is 2.
        printf("You need to give one argument \n"); // instructions
    }
    else
    {
        printf("This program encrypt your argument. Only capital letters!\n"); // instructions
        encryption(atoi(argv[1])); // Calls the function to encrypt. We take the arggument value to define the shift.
    }
    return 0;
}

// Our encryption fuction, takes in the argv.
void encryption(int shift)
{
    printf("Enter a value to be encrypted: "); // Instructions for user.
    int y, i, z; // For the loops
    char word[100];
    scanf("%s", word); // We use "scanf" to take in the word from the user which we will encrypt
    char capital[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',};
    char lowerCase[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',};

    // Getting the length of the entered word, that we can use in the for loop of iterating through the word.
    int size = (sizeof word / sizeof word[0]);
    // Goes trough each letter of the word and replaces the word with our shift.
    for (i = 0; i < size; i++)
    {
        // When detecting the end of the word we go out of the loop
        if (word[i] == '\0')
        {
            break;
        }
        /* we go trough the alphabet[] that first finds the letter word[i] in the alphabet and then
        exchanges it with the letter that is argument value.
        */
        for (y = 0; y < 26; y++)
        {   //First check if there are any capital letters in the word. If there are we shift it with the argument value.
            if (word[i] == capital[y]) //Checks if each letter is a word in the alphabet.
            {
                int index = (y + shift) % 26; // makes sure we dont go outside of the capital letters.
                word[i] = capital[index]; //We set the word index to capital index.
                break;
            // If there are lowercase letters we shift then similarly like above.
            }else if (word[i] == lowerCase[y]){
                int index = (y + shift) % 26; // makes sure we dont go outside of the array of lower case letters.
                word[i] = lowerCase[index]; //We set the word index to lowerCase index.
                break;
            }
        }
    }
    // We print out the encrypted value.
    printf("Encrypted output: %s \n", word);
    encryption(shift); // We call the function again after printing the word, to allow the user to continue.
}