
//*----------------------------Start Of Our code-----------------------*/
// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 5
// Exercise 2
// Submission Code: 53920
/*----------------------------Our code-----------------------*/
#include <dht.h>

#define tempPin 12

float Temp = 0;        // variable for storing temp
int lightSensor = 0;   // variable for storing light intensity
int periodicity = 0.5; // periodicity is half a second
int redLEDPin = 9;    // store pin for red LED in variable
int greenLEDPin = 10;   // store pin for green LED in variable
int blueLEDPin = 11;    // store pin for blue LED in variable

int minLight = 0; // Row 1 in table, 0% light intensity

int lowLightMin = 1;  // Row 2 in table, 1% light intensity lower bound
int lowLightMax = 20; // Row 2 in table, 20% light intensity upper bound

int medLightMin = 21; // Row 3 in table, 21% light intensity lower bound
int medLightMax = 60; // Row 3 in table, 60% light intensity upper bound

int highLightMin = 61;  // Row 4 in table, 61% light intensity lower bound
int highLightMax = 100; // Row 4 in table, 100% light intensity upper bound

dht DHT; // Creats a DHT object

void printTempAndLight(float temp, int lightLevel);

void setup()
{
    pinMode(A1, INPUT); // analog light intensity input
    Serial.begin(9600); // begin serial communication to be able to print temp
}

void loop()
{
    lightSensor = analogRead(A1);                   // read analog of A1 and store value to lightSensor
    lightSensor = map(lightSensor, 6, 679, 0, 100); // Map light sensor analog from 0 to 100

    int chk = DHT.read11(tempPin);
    float temp = DHT.temperature; // Gets the values of the temperature

    // also some diff att different values but that is not relevant for this
    printTempAndLight(temp, lightSensor);

    //----Normal dependency indicates temp and light intensity values are appropriate for each case----
    // If temp is between -12 C and 0 C, and light intensity is between 1% and 20%
    if (-12 <= temp && temp <= 0 && (lowLightMin <= lightSensor && lightSensor <= lowLightMax))
    {
        greenLamp(); // Green lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is between 0 C and 20 C, and light intensity is between 21% and 60%
    if (0 <= temp && temp <= 20 && (medLightMin <= lightSensor && lightSensor <= medLightMax))
    {
        greenLamp(); // Green lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is greater than or equal to 21 C, and light intensity is between 61% and 100%
    if (temp >= 21 && (highLightMin <= lightSensor && lightSensor <= highLightMax))
    {
        greenLamp(); // Green lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is less than -12 C, and light intensity is 0%
    if (temp < -12 && lightSensor == minLight)
    {
        greenLamp(); // Green lamp is turned to HIGH and others are turned to LOW
    }

    //----Temp higher than it should be in relation to light intensity----
    // If temp is greater than 0 C, and light intensity is between 1% and 20%
    if (0 < temp && (lowLightMin <= lightSensor && lightSensor <= lowLightMax))
    {
        // Temp too high
        redLamp(); // Red lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is greater than 20 C, and light intensity is between 21% and 60%
    if (20 < temp && (medLightMin <= lightSensor && lightSensor <= medLightMax))
    {
        // Temp too high
        redLamp(); // Red lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is greater than -12 C, and light intensity is 0%
    if (-12 < temp && lightSensor == minLight)
    {
        // Temp too high
        redLamp(); // Red lamp is turned to HIGH and others are turned to LOW
    }

    //----Temp lower than it should be in relation to light intensity----
    // If temp is less than -12 C, and light intensity is between 1% and 20%
    if (temp < -12 && (lowLightMin <= lightSensor && lightSensor <= lowLightMax))
    {
        // Temp too low
        blueLamp(); // Blue lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is less than 0 C, and light intensity is between 21% and 60%
    if (temp < 0 && (medLightMin <= lightSensor && lightSensor <= medLightMax))
    {
        // Temp too low
        blueLamp(); // Blue lamp is turned to HIGH and others are turned to LOW
    }
    // If temp is less than 21 C, and light intensity is between 61% and 100%
    if (temp < 21 && (highLightMin <= lightSensor && lightSensor <= highLightMax))
    {
        // Temp too low
        blueLamp(); // Blue lamp is turned to HIGH and others are turned to LOW
    }
    delay(1000 * periodicity); // 1000ms * 0.5s periodicity gives us a delay of 500ms
}

void redLamp()
{
    digitalWrite(redLEDPin, HIGH);  // Write HIGH to red LED
    digitalWrite(greenLEDPin, LOW); // Write LOW to green LED
    digitalWrite(blueLEDPin, LOW);  // Write LOW to blue LED
}

void greenLamp()
{
    digitalWrite(redLEDPin, LOW);    // Write LOW to red LED
    digitalWrite(greenLEDPin, HIGH); // Write HIGH to green LED
    digitalWrite(blueLEDPin, LOW);   // Write LOW to blue LED
}

void blueLamp()
{
    digitalWrite(redLEDPin, LOW);   // Write LOW to red LED
    digitalWrite(greenLEDPin, LOW); // Write LOW to green LED
    digitalWrite(blueLEDPin, HIGH); // Write HIGH to blue LED
}

void printTempAndLight(float temp, int lightLevel){
    delay(1000);
    Serial.print(temp);
    Serial.println(" degrees Celcius");
    Serial.print("Light intensity: ");
    Serial.println(lightLevel);
}