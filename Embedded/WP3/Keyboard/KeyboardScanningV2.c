// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 3
// Exercise 4
// Submission Code: 32720
#include <Keypad.h>

const byte numRows = 4; // number of rows on the keypad
const byte numCols = 4; // number of columns on the keypad

// keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'#', '0', '*', 'D'}}; // Declear a metrices/2D array of rows and colums.

byte rowPins[numRows] = {11, 10, 9, 8}; // connect to the row pinouts of the keypad
byte colPins[numCols] = {7, 6, 5, 4};   // connect to the column pinouts of the keypad

// initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
    Serial.begin(9600); // Inital
}

void loop()
{
    char keypressed = myKeypad.getKey(); // If key is pressed, this key is stored in keypressed variable

    if (keypressed != NO_KEY) // If key is not equal to NO_KEY, then this key is printed out
    {
        Serial.println(keypressed); // Print the keypressed
    }
}