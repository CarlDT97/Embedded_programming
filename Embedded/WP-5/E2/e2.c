/*----------------------------Start Of Our code-----------------------*/
// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 5
// Exercise 2
// Submission Code: 53920
/*----------------------------Our code-----------------------*/

#include <dht.h>

//-----pins-----
#define tempPin 12
#define bluePin 11
#define greenPin 10
#define whitePin 9
#define redPin 8
#define yellowPin 7
/*temperature values*/
#define TEMP20 20
#define TEMP21 21
#define TEMP24 24
#define TEMP25 25
#define TEMP28 28
#define TEMP29 29
#define TEMP32 32
// 20 blue, 21-24 green, white= 25-28, red 29-32, yellow = 33
float temp = 0;
dht DHT; // Creats a DHT object
void setup()
{
    pinMode(tempPin, INPUT);    // temp input
    pinMode(bluePin, OUTPUT);   // sets the digital pin 12 as output
    pinMode(greenPin, OUTPUT); // sets the digital pin 11 as output
    pinMode(whitePin, OUTPUT);  // sets the digital pin 10 as output
    pinMode(redPin, OUTPUT); // sets the digital pin 9 as output
    pinMode(yellowPin, OUTPUT);    // sets the digital pin 8 as output
    Serial.begin(9600);         // Begin serial
}

void loop()
{
    // the loop function is now empty since the interrupt handles the temperature change detection
    //int readData = DHT.read12(tempPin); // Reads the data from the sensor
    int chk = DHT.read11(tempPin);
    temp = DHT.temperature; // Gets the values of the temperature

    if (temp <= TEMP20) // If the temp is equal or below 0C
    {
        blueLight(); // We call to turn on blue light
    }
    if (temp >= TEMP21 && temp <= TEMP24) // If the temp is between 1 celsius and 10 celsius
    {
        greenLight(); // We call to turn on orange light
    }
    if (temp >= TEMP25 && temp <= TEMP28) // If the temp is between 11 celsius and 20 celsius
    {
        whiteLight(); // We call to turn on green light
    }
    if (temp >= TEMP29 && temp <= TEMP32) // If the temp is between 21 celsius and 30 celsius
    {
        redLight(); // We call to turn on yellow light
    }
    if (temp >= TEMP32) // If the temp is larger or equal to 31 celsius
    {
        yellowLight(); // We call to turn on red light
    }
    delay(500);
    Serial.print("Temperature is: ");
    Serial.println(temp); // We print out the temperature
}
void setLights(int blue, int green, int white, int red, int yellow) // Function for setting pins to high and low
{
    digitalWrite(bluePin, blue);
    digitalWrite(greenPin, green);
    digitalWrite(whitePin, white);
    digitalWrite(redPin, red);
    digitalWrite(yellowPin, yellow);
}

void blueLight() // Set bluePin to HIGH and rest to LOW
{
    setLights(1, 0, 0, 0, 0);
}
void greenLight() // Set bluePin and greenPin to HIGH and rest to LOW
{
    setLights(1, 1, 0, 0, 0);
}
void whiteLight() // Set bluePin, greenPin and whitePin to HIGH and rest to LOW
{
    setLights(1, 1, 1, 0, 0);
}
void redLight() // Set bluePin, greenPin, whitePin, and yellowPin to HIGH and redPin to LOW
{
    setLights(1, 1, 1, 1, 0);
}
void yellowLight() // Set all LED pins to high
{
    setLights(1, 1, 1, 1, 1);
}