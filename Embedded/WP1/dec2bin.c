// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 1
// Exercise 4
// Submission Code 14620

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    // Ensuring program halts if exactly one argument isn't given 
    if(argc != 2) {
        printf("Program takes only 1 argument! Use \"dec2bin -h\" for usage.\n");
        return 1;
    }

    // Providing help if the h flag is provided
    char *argument = argv[1];
    if(strcmp(argument, "-h") == 0) {
        printf("This program takes one additional argument - a decimal number (i.e. digits 0-9) - and returns it in binary format.\n");
        printf("e.g. \"dec2bin 255\" returns 11111111\n");
        return 0;
    }

    // Verifying that the argument has only valid digits
    int counter = 0;
    while(argument[counter] != '\0') {
        char digit = argument[counter];
        if(
            digit != '0' &&
            digit != '1' &&
            digit != '2' &&
            digit != '3' &&
            digit != '4' &&
            digit != '5' &&
            digit != '6' &&
            digit != '7' &&
            digit != '8' &&
            digit != '9'
        ) {
            printf("This program takes only decimal numbers as arguments - found invalid digit \'%c\' at position %d\n", digit, counter);
            return 2;
        }
        counter++;
    }

    // Storing program input (*char) in a long int (max size) and computing the necessary size
    long input = atol(argument);
    /*
      x < 2^8 means input needs 8 bits (needs char)
      x < 2^16 means input needs 16 bits (needs short)
      x < 2^32 means input needs 32 bits (needs int)
      x < 2^64 means input needs 64 bits (needs long)
    */ 
    int inputSize = 
        input < pow(2, 8) ? 8 :
        input < pow(2, 16) ? 16 :
        input < pow(2, 32) ? 32 : 64;

    // Declaring the resulting character arrays for storing digits
    char tempNumber[inputSize];
    char convertedNumber[inputSize];
    char digits[2] = "01";

    // Converting base 10 to base 2 using conversion algorithm from Kochan's Programming in C (p.141)
    long dividend = input;
    int index = 0;
    do {
        int remainder = dividend % 2;
        tempNumber[index] = digits[remainder];
        index++;
        dividend /= 2;
    } while(dividend != 0);

    // Setting the remaining entries to zeroes
    for(; index < inputSize; index++) {
        tempNumber[index] = '0';
    }

    // Reversing the order of the number
    for(int i = 0; i < inputSize; i++) {
        convertedNumber[i] = tempNumber[inputSize - i - 1];
    }

    for(int i = 0; i < inputSize; i++) {
        printf("%c", convertedNumber[i]);
    }
    printf("\0");

    // int fd[2];
    // if(pipe(fd) == -1) {
    //     printf("Unable to open pipe.\n");
    //     return 1;
    // }
    // close(fd[0]);
    // write(fd[1], convertedNumber, sizeof(convertedNumber));
    // close(fd[1]);
    return 0;
}
