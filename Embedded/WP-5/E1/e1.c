/*----------------------------Start Of Our code-----------------------*/
// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 5
// Exercise 1
// Submission Code: 53920
/*----------------------------Our code-----------------------*/


// C++ code
int pinBlinkingLED = 13; // Set pin of the blinking LED
int pinBttnLED = 12; // Set pin of the button

void setup()
{
  pinMode(pinBlinkingLED, OUTPUT); //Set blinking LED pin mode to output
  pinMode(pinBttnLED, OUTPUT); //Set button LED pin mode to output
  digitalWrite(12, HIGH); //Provide power to pin 12 so the button works
}

void loop()
{
  digitalWrite(pinBlinkingLED, HIGH); //Turn on LED
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(pinBlinkingLED, LOW); //Turn off LED
  delay(1000); // Wait for 1000 millisecond(s)
}