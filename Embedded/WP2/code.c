// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 2
// Exercise 4
// Submission Code: 29920
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    _Bool faulty = 0; //Boolean value for the conditions below.

    // Ensuring only 5 additional arguments are passed
    if (argc != 6)
    {
        printf("This program expects exactly 5 arguments!\n");
        return 1;
    }
    // Ensuring that engien/bit 7 takes values 0-1
    if (
        strcmp(argv[1], "0") != 0 &&
        strcmp(argv[1], "1") != 0)
    {
        printf("Faulty program argument #1 (must be 0-1)\n");
        faulty = 1;
    }

    // Ensuring that gear position/bits 4, 5 and 6 take values 0-4
    if ((strcmp(argv[2], "0") &&
        strcmp(argv[2], "1") &&
        strcmp(argv[2], "2") &&
        strcmp(argv[2], "3") &&
        strcmp(argv[2], "4")) != 0)
    {
        printf("Faulty program argument #2 (must be 0-4)\n");
        faulty = 1;
    }

    // Ensuring that key_pos/bits 2 and 3 take values 0-2
    if (
        (strcmp(argv[3], "0") &&
        strcmp(argv[3], "1") &&
        strcmp(argv[3], "2")) != 0)
    {
        printf("Faulty program argument #3 (must be 0-2)\n");
        faulty = 1;
    }

    // Ensuring that break1/bit 1 takes values 0-1
    if (
        (strcmp(argv[4], "0") &&
        strcmp(argv[4], "1")) != 0)
    {
        printf("Faulty program argument #4 (must be 0-1)\n");
        faulty = 1;
    }

    // Ensuring that break0/bit 0 takes values 0-1
    if (
        (strcmp(argv[5], "0") &&
        strcmp(argv[5], "1")) != 0)
    {
        printf("Faulty program argument #5 (must be 0-1)\n");
        faulty = 1;
    }

    if (faulty) // If faulty is equal to true end program.
    {
        return 1;
    }

    // Convert the char to int then multiplying every value by its (right-most) binary weight
    // (bit 0 multiplied by 2^0, bit 1 multiplied by 2^1 etc)
    unsigned char data =
        atoi(argv[1]) * (1 << 7) +
        atoi(argv[2]) * (1 << 4) +
        atoi(argv[3]) * (1 << 2) +
        atoi(argv[4]) * (1 << 1) +
        atoi(argv[5]) * (1 << 0);

    // Printing the byte in the corresponding base
    printf("%X\n", data);
    return 0;
}