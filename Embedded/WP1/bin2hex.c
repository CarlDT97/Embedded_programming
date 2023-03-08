// (C) John Webb, Carl Dahlqvist, Ansis Plepis group 20 (2023)
// Work package 1
// Exercise 4
// Submission Code 14620

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {

    // Ensuring program halts if exactly one argument isn't given 
    if(argc != 2) {
        printf("Program takes only 1 argument! Use \"bin2hex -h\" for usage.\n");
        return 1;
    }

    // Providing help if the h flag is provided
    char *argument = argv[1];
    if(strcmp(argument, "-h") == 0) {
        printf("This program takes one additional argument - a binary number (i.e. digits 0-1) - and returns it in hexadecimal format.\n");
        printf("e.g. \"bin2hex 11111111\" returns EE\n");
        return 0;
    }

    // Verifying that the argument has only valid digits
    int counter = 0;
    while(argument[counter] != '\0') {
        char digit = argument[counter];
        if(
            digit != '0' &&
            digit != '1'
        ) {
            printf("This program takes only binary numbers as arguments - found invalid digit \'%c\' at position %d\n", digit, counter);
            return 2;
        }
        counter++;
    }

    
    // Custom algorithm for converting bin to hex. 2^4 = 16, so 4 bits represent 1 hex number
    // First find the nearest multiple of 4 greater than the size of the binary number i.e. 101 has size 4 and is converted to 0101.
    int ceiling = (counter - 1) + (4 - (counter - 1) % 4);

    char convertedNumber[ceiling / 4]; // hex takes up 4 times less bits
    char expandedBinary[ceiling];

    // Then store the expanded binary number
    for(int i = 0; i < ceiling; i++) {
        if(i >= ceiling - counter) {
            expandedBinary[i] = argument[i - ceiling + counter];
        } else {
            expandedBinary[i] = '0';
        }
    }

    // Then group every 4 bits together and convert them into a single hex digit
    int j;
    for(j = 0; j < ceiling / 4; j ++) {
        char group[5] = { 
            expandedBinary[4 * j],
            expandedBinary[4 * j + 1],
            expandedBinary[4 * j + 2],
            expandedBinary[4 * j + 3],
        };
        char hexDigit = 
            strcmp(group, "0000") == 0 ? '0' :
            strcmp(group, "0001") == 0 ? '1' :
            strcmp(group, "0010") == 0 ? '2' :
            strcmp(group, "0011") == 0 ? '3' :
            strcmp(group, "0100") == 0 ? '4' :
            strcmp(group, "0101") == 0 ? '5' :
            strcmp(group, "0110") == 0 ? '6' :
            strcmp(group, "0111") == 0 ? '7' :
            strcmp(group, "1000") == 0 ? '8' :
            strcmp(group, "1001") == 0 ? '9' :
            strcmp(group, "1010") == 0 ? 'a' :
            strcmp(group, "1011") == 0 ? 'b' :
            strcmp(group, "1100") == 0 ? 'c' :
            strcmp(group, "1101") == 0 ? 'd' :
            strcmp(group, "1110") == 0 ? 'e' :
            'f';
        convertedNumber[j] = hexDigit; 
        printf("%c", hexDigit);
    }
    convertedNumber[j + 1] = '\0';
    // printf("%s", convertedNumber);

    return 0;
}