// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 2
// Exercise 1
// Submission Code 29920

#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Declare functions
void move();

void turn();

// Declare enum
enum DIRECTION
{
    N,
    O,
    S,
    W
};

// Define struct ROBOT
typedef struct
{
    int xpos;
    int ypos;
    // Use enum DIRECTION as a variable for struct
    enum DIRECTION dir;
} ROBOT;

int main()
{
    while (1)
    {
        // Declare robot struct
        ROBOT robot;
        // Create variable for holding user input of x-position
        int xpos;
        // Create variable for holding user input of y-position
        int ypos;
        // Create an array of characters to hold user input string
        char str[99];
        // Set robot direction to north
        robot.dir = N;

        printf("\nEnter starting position x:\n");
        // Take user input as a decimal integer and address it to the xpos variable
        scanf("%d", &xpos);
        // If xpos is not between 0 and 99
        if (xpos < 0 || xpos > 99)
        {
            printf("Must enter a positive integer between 0 and 99.");
            // Program exits
            return 0;
        }
        else
        {
            // Robot's x coordinatine is updated to what the user supplied as input.
            robot.xpos = xpos;
            printf("Enter starting position y:\n");
            // Take user input as a decimal integer and address it to the ypos variable
            scanf("%d", &ypos);
            // If ypos is not between 0 and 99, the user will be told to enter a positive integer between 0 and 9
            if (ypos < 0 || ypos > 99)
            {
                printf("Must enter a positive integer between 0 and 99.");
                // Program exits
                return 0;
            }
            else
            {
                // Robot's y coordinatine is updated to what the user supplied as input.
                robot.ypos = ypos;
                printf("Enter a string of characters 'm' and 't' where 'm' means move and 't' means turn 90 degrees clockwise.\n");
                // Take user input of movement string
                scanf("%s", str);

                // Iterating through the string index by index reading the characters and performing actions one by one.
                for (int i = 0; i < strlen(str); i++)
                {
                    // If the char at index i is m, the robot will move.
                    if (str[i] == 'm')
                    {
                        move(&robot);
                    }
                    // If the char at index i is t, the robot will move.
                    else if (str[i] == 't')
                    {
                        turn(&robot);
                    }
                    // If the char at index i is s, the program will close.
                    else if (str[i] == 's')
                    {
                        return 0;
                    }
                }
                // Prints the robot's new position.
                printf("New position is (%d,%d)\n", robot.xpos, robot.ypos);
            }
        }
    }
}

void turn(ROBOT *robot)
{
    // If robot is ordered to turn while facing north, a 90 degree turn clockwise will leave the robot facing east.
    if (robot->dir == N)
    {
        robot->dir = O;
    }
    // If robot is ordered to turn while facing east, a 90 degree turn clockwise will leave the robot facing south.
    else if (robot->dir == O)
    {
        robot->dir = S;
    }
    // If robot is ordered to turn while facing south, a 90 degree turn clockwise will leave the robot facing west.
    else if (robot->dir == S)
    {
        robot->dir = W;
    }
    // If robot is ordered to turn while facing west, a 90 degree turn clockwise will leave the robot facing north again.
    else if (robot->dir == W)
    {
        robot->dir = N;
    }
}

void move(ROBOT *robot)
{
    // If robot direction is north:
    if (robot->dir == N)
    {
        // Assuming north means facing towards the direction of the y-axis, any movement north will increase the position on the y axis
        robot->ypos++;
    }
    else if (robot->dir == O)
    {
        // Assuming east means facing towards the direction of the x-axis, any movement east will increase the position on the x axis
        robot->xpos++;
    }
    else if (robot->dir == S)
    {
        // Assuming south means facing downwards on the y-axis, any movement south will decrease the position on the y axis
        robot->ypos--;
    }
    else
    {
        // As dir is an enum, the only other possible value is W or west.
        // Assuming west means facing towards zero, any movement west will decrease the position on the x axis
        robot->xpos--;
    }
}