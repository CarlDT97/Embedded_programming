// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 1
// Exercise 3
// Submission Code 14620

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER 15;
// Function for asking user if they would like to kee playing
int keepPlaying(int usrInput, int num);

int keepPlaying(int usrInput, int num)
{
    // Asks user if they would like to keep playing and provides two options.
    printf("\nWould you like to start a new round?\n1.Yes\n2.No\n");
    // Takes user input as a int and addresses to usrInput variable.
    scanf("%d", &usrInput);
    // If statement will run inner code if user enters 1 to continue.
    if (usrInput == 1)
    {
        // Generate another random number and return it.
        num = rand() % 100 + 1;
        return num;
    }
    // Else if statement will run inner code if user enters 2 to quit.
    else if (usrInput == 2)
    {
        // Thanks user for playing and informs user that the program will now exit.
        printf("\nThanks for playing! Exiting program.\n");
        // Returns 0
        return 0;
    }
    else
    {
        printf("\nYou did not enter option 1 or 2 so clearly you do not wish to continue. Goodbye!");
        return 0;
    }
}

int main()
{
    // int variable named boolean used to control the while loop for repeatedly asking for user input on wrong guesses.
    int boolean = 1;
    // Instantiating random number generator
    srand((unsigned)time(NULL));
    // Assigning a random int between 1 and 100 to num.
    int num = rand() % 100 + 1;
    // Variable for storing user input
    int usrInput;
    // Variable for storing number of guesses
    int guessCount = 0;
    // maxGuesses assigned based on MAX_NUMBER
    int maxGuesses = MAX_NUMBER;

    // While boolean = 1, while loop will prompt user for a number. Will stop when boolean is updated to 0.
    while (boolean)
    {
        // If the guess count is greater than MAX_NUMBER
        if (guessCount > maxGuesses)
        {
            printf("\nGame Over. You have exceeded the maximum of %d guesses.", maxGuesses);
            // Takes the returned value of the function
            int answer = keepPlaying(usrInput, num);
            // If answer is above zero, it means that the user decided they wanted to keep playing and a new number was randomized.
            // Since only numbers from 1 to 100 are generated, 0 is not a possibility of the randomizer.
            if (answer > 0)
            {
                // Reset guess counter
                guessCount = 0;
                // Assign the new guess number
                num = answer;
            }
            else
            {
                // If answer is 0, it means the user wanted to exit, thus we return 0 to exit the program and set boolean to 0.
                boolean = 0;
                return 0;
            }
        }
        else
        {

            // Prompt user to input number.
            printf("%s", "Enter a number between 1 and 100:\n");
            // Take user input and read input as int and point to usrInput variable
            scanf("%d", &usrInput);
            // If statement will run inner code if user enters a number outside of 1 to 100.
            if (usrInput < 1 || usrInput > 100)
            {
                // Inform user that number must be between 1 and 100.
                printf("%s", "\nMust enter a number between 1 and 100.\n");
            }
            // Else if statement will run inner code if user guesses a number lower than the computer generated number.
            else if (usrInput < num)
            {
                // Inform user that guess is lower than the number.
                printf("%s", "\nGuess is lower than the number.\n");
                // Increment guess count.
                guessCount++;
                // Inform user how many times they have guessed.
                printf("You have guessed %d times\n", guessCount);
            }
            // Else if statement will run inner code if user guesses a number higher than the computer generated number.
            else if (usrInput > num)
            {
                // Inform user that guess is higher than the number.
                printf("%s", "\nGuess is higher than the number\n");
                // Increment guess count.
                guessCount++;
                // Inform user how many times they have guessed.
                printf("You have guessed %d times\n", guessCount);
            }
            // Else if statement will run inner code if user guesses correctly.
            else if (usrInput == num)
            {
                // Inform user that guess is higher than the number and shows them their final guess count.
                printf("\nCorrect! You guessed %d times\n", guessCount);
                // Takes the returned value of the function
                int answer = keepPlaying(usrInput, num);
                // If answer is above zero, it means that the user decided they wanted to keep playing and a new number was randomized.
                // Since only numbers from 1 to 100 are generated, 0 is not a possibility of the randomizer.
                if (answer > 0)
                {
                    // Reset guess counter
                    guessCount = 0;
                    // Assign the new guess number
                    num = answer;
                }
                else
                {
                    // If answer is 0, it means the user wanted to exit, thus we return 0 to exit the program.
                    boolean = 0;
                    return 0;
                }
            }
        }
    }
}
