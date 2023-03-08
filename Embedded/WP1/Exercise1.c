// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 1
// Exercise 1
// Submission Code: 14620

#include <stdio.h>

int main()
{
    // Instantiate variable a for holding the input.
    int num;
    // While is consistently true, thus will continue to ask for user input so long as the program continues to run.
    while (1)
    {
        // Printing a prompt asking the user to enter a number between 1 and 5.
        printf("%s", "Enter an integer between 1 and 5:\n");
        // Taking keyboard input of type int and pointing to variable num.
        scanf("%d", &num);

        // If variable 'num' is equal to a number from 1 to 5, a corresponding sentence will be printed.
        if (num == 1)
        {
            printf("%s", "A partridge in a pear tree\n");
        }
        else if (num == 2)
        {
            printf("%s", "Two turtle-doves\n");
        }
        else if (num == 3)
        {
            printf("%s", "Three french hens\n");
        }
        else if (num == 4)
        {
            printf("%s", "Four calling birds\n");
        }
        else if (num == 5)
        {
            printf("%s", "Five golden rings.\n");
        }
        else
        {
            // If variable 'num' is anything else than a number between 1 and 5, the program will exit.
            return 0;
        }
    }
}