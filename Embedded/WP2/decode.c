// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 2
// Exercise 4
// Submission Code: 29920

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void decimal_to_binary(int n);
int main(int argc, char **argv)
{
    // Ensuring exactly 1 additional argument is passed
    if (argc != 2)
    {
        printf("This program expects exactly 1 argument!\n");
        return 1;
    }
    // Ensuring the argument only has hexadecimal digits
    int i;
    for (i = 0; argv[1][i] != '\0'; i++)
    {
        char digit = tolower(argv[1][i]);
        if (!((digit >= '0' && digit <= '9') || (digit >= 'a' && digit <= 'f'))){ // If statement to check if the argument contain 0-9 or a-f.
            printf("Faulty program argument (incorrect hexadecimal digit %c at position %i)", digit, i); // Instructions to user
            return 1;
        }
    }
    // Ensuring the argument size doesn't exceed 1 byte so it can successfully be converted into 8 bits
    int input = (int)strtol(argv[1], NULL, 16);
    // Extract the engine on value from the input integer
    int engine_on_value = input >> 7 & 1;
    // Extract the gear position value from the input integer
    int gear_pos_value = input >> 4 & 7;
    // Extract the key position value from the input integer
    int key_pos_value = input >> 2 & 3;
    // Extract the brake 1 value from the input integer
    int brake1_value = input >> 1 & 1;
    // Extract the brake 2 value from the input integer
    int brake2_value = input & 1;
    // Prints the table

    printf("Name\t\tValue\n");
    printf("-------------------------\n");
    printf("engine_on\t%i\n", engine_on_value);
    printf("gear_pos\t%i\n", gear_pos_value);
    printf("key_pos\t\t%i\n", key_pos_value);
    printf("brake1\t\t%i\n", brake1_value);
    printf("brake2\t\t%i\n", brake2_value);
}
