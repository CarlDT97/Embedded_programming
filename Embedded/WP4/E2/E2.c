
#include <Servo.h>

#define servoPort A0

Servo servo; // Declare servo

int degrees = 0; // variable for storing the position/direction of the servo

int timer = 0; // timer variable which will be used to time the cycles of the interrupt

// seconds variable for keeping track of the seconds passed and printing to Serial
int seconds = 0;

void setup()
{
    // Disable interrupts
    cli();

    // Begin serial for print statements
    Serial.begin(9600);

    // Servo attached to the defined port
    servo.attach(servoPort);
    // Set the motor to 0 as the starting point
    servo.write(0);

    // Setting timer2 interrupt at 16kHz

    // Reset registers
    TCCR2A = 0; // Set TCCR2A to 0
    TCCR2B = 0; // Likewise for TCCR2B

    TCNT2 = 0; // initialize counter value to 0, can otherwise be used for preloading the timer

    // Setting compare match register to 124 (max is 256 as timer 2 is 8bit)
    OCR2A = 124;

    // Enable CTC mode which is done with WGM21 in the case of timer 2
    TCCR2A |= (1 << WGM21);

    // Use CS21 to set the prescalar to 8
    // will be multiplied by the compare match register + 1
    TCCR2B |= (1 << CS21);

    // Enable compare match interrupts at the value stored in the compare match register (OCR2A)
    TIMSK2 |= (1 << OCIE2A);

    // re-allow interrupts on timer
    sei();
}

void loop() {}

// interrupt function to be called at each interrupt
ISR(TIMER2_COMPA_vect)
{
    // timer used as a counter to count the cycles
    timer++;

    // timer 2 interrupts at 16kHz
    // Thus 1 second will be equal to 16000 cycles
    if (timer == 16000)
    {
        seconds++; // Increment seconds counter
        Serial.print(seconds);
        Serial.println(" seconds");

        if (degrees == 180) // If the motor has turned 180 degrees
        {
            // Reset motor to its starting position
            servo.write(0);
            // Reset the degress variable
            degrees = 0;
        }
        // The servo will move 180 degrees every 30 seconds
        // 180/30=6
        // Thus each movement of the servo is made in 6 degree increments
        degrees += 6;
        servo.write(degrees);

        timer = 0; // reset timer so it can count another cycle
    }
}
