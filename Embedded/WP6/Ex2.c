// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 6
// Exercise 2
// Submission Code: 67420
/*Pins*/
#define REDPIN 2    // Red led pin
#define YELLOWPIN 3 // Yellow led pin
#define BLUEPIN 4   // Blue led pin
#define GREENPIN 5  // Green led pin

#define SENSORPIN 6 // Sensor pin
#define BUZZERPIN 9 // Speaker pin

/*variables*/
// Declear the values for the intervals
#define maxDistance 200 // Define max distance of 200cm
#define distance150 150 // Define 150cm distance
#define distance100 100 // Define 100cm distance
#define distance50 50   // Define 50cm distance
#define distance30 30   // Define 30cm distance
#define minDistance 25  // Define 25cm distance

// Used in delay, time of delay is 0.5s
#define wait 500
#define waitTwo 2  // Used in for delay
#define waitTen 10 // Used in for delay

int distance = 0; // declear the varible distance

void setup()
{ /* Set ip the pinmode for the leds*/
    pinMode(GREENPIN, OUTPUT);
    pinMode(BLUEPIN, OUTPUT);
    pinMode(YELLOWPIN, OUTPUT);
    pinMode(REDPIN, OUTPUT);
}

void loop()
{
    distance = getDistance(SENSORPIN); // declear distance from function return value

    if (distance >= maxDistance) // Distance > then 200
    {
        light(5); // Turn off lights
        sound(0); // Turn off sound
    }
    else if (distance < maxDistance && distance > distance150) // Distance is between 200 - 150 cm
    {
        light(1);   // Turn on the green light
        sound(250); // Sound on
    }
    else if (distance < distance150 && distance > distance100) // Distance is between 150 - 100 cm
    {
        light(2);   // Turn of blue light
        sound(300); // Increase sound to 300
    }
    else if (distance < distance100 && distance > distance50) // Distance is between 100 - 50 cm
    {
        light(3);   // Turn of yellow light
        sound(400); // Increase sound to 400
    }
    else if (distance < distance50 && distance > distance30) // Distance is between 50 - 30 cm
    {
        light(4);   // Turn of red light
        sound(500); // Increase sound to 500
    }
    else if (distance < minDistance) // Distance is below 25 cm
    {                                // when we are between a distance of 25 and 30 we turn on all leds and let them blink every 0.5 second
        light(4);                    // Turn on all light
        sound(1000);                 // Turn the speaker on
        delay(wait);
        light(5); // turn off light
    }
    delay(wait);
}

void sound(int frequency) // Function for sound
{                         // takes a frequency as input and activates the speaker. A freq of 0 will turn off the speaker.

    if (frequency == 0) // If the frequency is 0
    {
        noTone(BUZZERPIN); // turn off speaker
    }
    else // We set the sound to the frequency value
    {
        tone(BUZZERPIN, frequency); // send freq to speaker
    }
}

int getDistance(int sensorPin)
{
    pinMode(SENSORPIN, OUTPUT);
    digitalWrite(SENSORPIN, LOW); // Set ultrasonic distance sensor to low
    delay(waitTwo);               // delay for 2
    // Sets the sensor pin to HIGH state for 10 microseconds
    digitalWrite(SENSORPIN, HIGH); // Set ultrasonic distance sensor to High
    delay(waitTen);                // Delay for 10
    digitalWrite(SENSORPIN, LOW);  // Set ultrasonic distance sensor to low
    pinMode(SENSORPIN, INPUT);

    return 0.01723 * pulseIn(SENSORPIN, HIGH); // gets the traveltime and converts it to CM
}

void setLights(int green, int blue, int yellow, int red) // Function for setting pins to high and low
{
    digitalWrite(GREENPIN, green);
    digitalWrite(BLUEPIN, blue);
    digitalWrite(YELLOWPIN, yellow);
    digitalWrite(REDPIN, red);
}

void light(int value) // Set GreenPin to HIGH and rest to LOW
{
    if (value == 1)
    {
        setLights(1, 0, 0, 0);
    }
    else if (value == 2)
    {
        setLights(1, 1, 0, 0);
    }
    else if (value == 3)
    {
        setLights(1, 1, 1, 0);
    }
    else if (value == 4)
    {
        setLights(1, 1, 1, 1);
    }
    else if (value == 5)
    {
        setLights(0, 0, 0, 0);
    }
}