// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 6
// Exercise 1
// Submission Code: 67420

#define PIN1 5
#define PIN2 6
#define ENCODER_A 2
#define ENCODER_B 3

int maxPosition = 2299;
int maxAngle = 359;

int encA = 0; // signal from the A encoder
int encB = 0; // signal from the B encoder

int position = 0;        // Position value that is changed on interrupt
int degrees = 0;         // Position of the motor in degrees
int desiredPosition = 0; // Target position in degrees

int motorSpeed = 0; // Value for storing speed of the motor

int kp = 5; // Tuning constant

int difference = 0; // used to check the distance error between current position and desired
int u = 0;          // variable for holding signal to be fed to the motor

int calcControlSignal(int difference);
int getInput();

void setup()
{
    Serial.begin(9600); // Begin reading serial

    pinMode(ENCODER_A, INPUT_PULLUP); // INPUT_PULLUP used to keep signal HIGH
    pinMode(ENCODER_B, INPUT_PULLUP); // INPUT_PULLUP used to keep signal HIGH
    pinMode(PIN1, OUTPUT);            // ASSIGN OUTPUT PIN1
    pinMode(PIN2, OUTPUT);            // ASSIGN OUTPUT PIN2

    // Attach interrupt to encoder A to change position for every tick when encoder A is RISING
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), ISR_updatePosition, RISING);

    // Motor must be started in order for tinkercad to work (bug)
    analogWrite(PIN1, 10);
    analogWrite(PIN2, 10);
}

void loop()
{
    // Set motor to "zero" value but not quite zero for same reason as setup
    analogWrite(PIN1, 10);
    analogWrite(PIN2, 10);

    // Check done to see if motor has reached maximum rotation in either direction
    if (position > maxPosition)
    {
        // Reset counters for degree and position
        degrees -= maxAngle;
        position -= maxPosition;
    }
    if (position < 0)
    {
        // Reset counters for degree and position
        degrees += maxAngle;
        position += maxPosition;
    }

    Serial.print("The current position of the motor is: ");
    Serial.print(degrees);
    Serial.print("\n");

    // Get input from user for desired motor position
    desiredPosition = getInput();

    // Calcualte the discrepancy between the current position and the target position
    difference = degrees - desiredPosition;

    // While there is a difference, run loop. When difference is zero, loop will be false
    while (difference)
    {
        // Motor speed is calculate via the difference * the tuning constant
        motorSpeed = calcControlSignal(difference);

        // If motor speed is greater than 0, must be turning clockwise
        if (motorSpeed >= 0)
        {
            analogWrite(PIN2, 0);
            analogWrite(PIN1, motorSpeed);
        }
        // Otherwise, motor must be turning counter clockwise
        else
        {
            analogWrite(PIN1, 0);
            analogWrite(PIN2, -motorSpeed);
        }

        // Map motor position to degrees
        degrees = map(position, 0, maxPosition, 0, maxAngle);

        // Update discrepancy between desired position and degrees
        difference = degrees - desiredPosition;

        Serial.print("Current discrepancy: ");
        Serial.println(difference);

        // if there is no longer a difference, stop the motor
        if (!difference)
        {
            analogWrite(PIN2, 0);
        }
    }
}

int calcControlSignal(int difference)
{
    u = kp * difference; // calculate motor signal using provided equation

    // Keep motor signal from having an absolute value over 255
    if (u > 254)
    {
        return 255;
    }
    else if (u < -254)
    { // cover case from negative end of the spectrum
        return -255;
    }
    else
        return u;
}

int getInput()
{
    Serial.print("Enter desired motor position: \n");
    int takeInput = 0;
    char tab[3];
    int input = -1;

    while (!takeInput)
    {
        // read three bytes and store them in the buffer table
        takeInput = Serial.readBytes(tab, 3);
        // parse bytes to integer
        input = atoi(&tab[0]);
    }
    // Return the input integer
    return input;
}

void ISR_updatePosition()
{
    encB = digitalRead(ENCODER_B); // read value rom encoder B

    // if signal from encoderB is greater than 0,
    // motor is rotating clockwise and position should be incremented
    if (encB > 0)
    {
        position++;
    }
    // Otherwise, if encoder b is less than 0,
    else
    {
        // means motor is rotating clockwise and we decrement position
        position--;
    }
}