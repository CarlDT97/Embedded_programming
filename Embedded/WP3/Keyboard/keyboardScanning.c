// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 4
// Exercise 6
// Submission Code: 32720

const int ROWS = 4; // Declear the number of rows of the keypad
const int COLS = 4; // Declear the number of colums of the keypad
char value[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}}; // Declear a metrices/2D array of rows and colums.

int rowPins[ROWS] = {11, 10, 9, 8}; // declear the pin values for rows
int colPins[COLS] = {7, 6, 5, 4};   // Declear the pins for the colums

void setup()
{
    Serial.begin(9600); // Inital

    for (int i = 0; i < COLS; i++)
    { // for loop trough the colums to set the pinmode to OUTPUT
        pinMode(colPins[i], OUTPUT);
    }
    for (int i = 0; i < ROWS; i++)
    { // Loop trough the rows and set the pinmode to Input
        pinMode(rowPins[i], INPUT_PULLUP);
    }
}

char getKey()
{ // Function for getting the colPin and rowPin

    for (int j = 0; j < COLS; j++)
    {                                  // loop trough the colums
        digitalWrite(colPins[j], LOW); // digitalWrite colPins LOW

        for (int i = 0; i < ROWS; i++)
        { // loop trough rows
            if (digitalRead(rowPins[i]) == LOW)
            {                                   // If the rowPins is LOW then we write the colPin
                digitalWrite(colPins[j], HIGH); //
                return value[i][j];             // We return the value[][], exemple we get row[1] & col[1] we print '4'
            }
        }
        digitalWrite(colPins[j], HIGH); // digitalWrite colPins HIGH
    }
    return ' ';
}

void loop()
{
    char key = getKey();
    if (key != ' ')
    {
        Serial.println(key);
        delay(500); // Delay with half of a second.
    }
}