
// C++ code
//
/*
 *  Interrupt and PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified June 2011 by Lex Talionis to add a function to read the timer
 *  Modified Oct 2011 by Andrew Richards to avoid certain problems:
 *  - Add (long) assignments and casts to TimerOne::read() to ensure calculations involving tmp, ICR1 and TCNT1 aren't truncated
 *  - Ensure 16 bit registers accesses are atomic - run with interrupts disabled when accessing
 *  - Remove global enable of interrupts (sei())- could be running within an interrupt routine)
 *  - Disable interrupts whilst TCTN1 == 0.  Datasheet vague on this, but experiment shows that overflow interrupt
 *    flag gets set whilst TCNT1 == 0, resulting in a phantom interrupt.  Could just set to 1, but gets inaccurate
 *    at very short durations
 *  - startBottom() added to start counter at 0 and handle all interrupt enabling.
 *  - start() amended to enable interrupts
 *  - restart() amended to point at startBottom()
 * Modiied 7:26 PM Sunday, October 09, 2011 by Lex Talionis
 *  - renamed start() to resume() to reflect it's actual role
 *  - renamed startBottom() to start(). This breaks some old code that expects start to continue counting where it left off
 *
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See Google Code project http://code.google.com/p/arduino-timerone/ for latest
 */
#ifndef TIMERONE_h
#define TIMERONE_h

#include <avr/io.h>
#include <avr/interrupt.h>

#define RESOLUTION 65536 // Timer1 is 16 bit

class TimerOne
{
public:
    // properties
    unsigned int pwmPeriod;
    unsigned char clockSelectBits;
    char oldSREG; // To hold Status Register while ints disabled

    // methods
    void initialize(long microseconds = 1000000);
    void start();
    void stop();
    void restart();
    void resume();
    unsigned long read();
    void pwm(char pin, int duty, long microseconds = -1);
    void disablePwm(char pin);
    void attachInterrupt(void (*isr)(), long microseconds = -1);
    void detachInterrupt();
    void setPeriod(long microseconds);
    void setPwmDuty(char pin, int duty);
    void (*isrCallback)();
};

extern TimerOne Timer1;
#endif

/*
 *  Interrupt and PWM utilities for 16 bit Timer1 on ATmega168/328
 *  Original code by Jesse Tane for http://labs.ideo.com August 2008
 *  Modified March 2009 by Jérôme Despatis and Jesse Tane for ATmega328 support
 *  Modified June 2009 by Michael Polli and Jesse Tane to fix a bug in setPeriod() which caused the timer to stop
 *  Modified June 2011 by Lex Talionis to add a function to read the timer
 *  Modified Oct 2011 by Andrew Richards to avoid certain problems:
 *  - Add (long) assignments and casts to TimerOne::read() to ensure calculations involving tmp, ICR1 and TCNT1 aren't truncated
 *  - Ensure 16 bit registers accesses are atomic - run with interrupts disabled when accessing
 *  - Remove global enable of interrupts (sei())- could be running within an interrupt routine)
 *  - Disable interrupts whilst TCTN1 == 0.  Datasheet vague on this, but experiment shows that overflow interrupt
 *    flag gets set whilst TCNT1 == 0, resulting in a phantom interrupt.  Could just set to 1, but gets inaccurate
 *    at very short durations
 *  - startBottom() added to start counter at 0 and handle all interrupt enabling.
 *  - start() amended to enable interrupts
 *  - restart() amended to point at startBottom()
 * Modiied 7:26 PM Sunday, October 09, 2011 by Lex Talionis
 *  - renamed start() to resume() to reflect it's actual role
 *  - renamed startBottom() to start(). This breaks some old code that expects start to continue counting where it left off
 *
 *  This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See Google Code project http://code.google.com/p/arduino-timerone/ for latest
 */
#ifndef TIMERONE_cpp
#define TIMERONE_cpp

// #include "TimerOne.h"

TimerOne Timer1; // preinstatiate

ISR(TIMER1_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
    Timer1.isrCallback();
}

void TimerOne::initialize(long microseconds)
{
    TCCR1A = 0;          // clear control register A
    TCCR1B = _BV(WGM13); // set mode 8: phase and frequency correct pwm, stop the timer
    setPeriod(microseconds);
}

void TimerOne::setPeriod(long microseconds) // AR modified for atomic access
{

    long cycles = (F_CPU / 2000000) * microseconds; // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
    if (cycles < RESOLUTION)
        clockSelectBits = _BV(CS10); // no prescale, full xtal
    else if ((cycles >>= 3) < RESOLUTION)
        clockSelectBits = _BV(CS11); // prescale by /8
    else if ((cycles >>= 3) < RESOLUTION)
        clockSelectBits = _BV(CS11) | _BV(CS10); // prescale by /64
    else if ((cycles >>= 2) < RESOLUTION)
        clockSelectBits = _BV(CS12); // prescale by /256
    else if ((cycles >>= 2) < RESOLUTION)
        clockSelectBits = _BV(CS12) | _BV(CS10); // prescale by /1024
    else
        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10); // request was out of bounds, set as maximum

    oldSREG = SREG;
    cli();                     // Disable interrupts for 16 bit register access
    ICR1 = pwmPeriod = cycles; // ICR1 is TOP in p & f correct pwm mode
    SREG = oldSREG;

    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR1B |= clockSelectBits; // reset clock select register, and starts the clock
}

void TimerOne::setPwmDuty(char pin, int duty)
{
    unsigned long dutyCycle = pwmPeriod;

    dutyCycle *= duty;
    dutyCycle >>= 10;

    oldSREG = SREG;
    cli();
    if (pin == 1 || pin == 9)
        OCR1A = dutyCycle;
    else if (pin == 2 || pin == 10)
        OCR1B = dutyCycle;
    SREG = oldSREG;
}

void TimerOne::pwm(char pin, int duty, long microseconds) // expects duty cycle to be 10 bit (1024)
{
    if (microseconds > 0)
        setPeriod(microseconds);
    if (pin == 1 || pin == 9)
    {
        DDRB |= _BV(PORTB1);   // sets data direction register for pwm output pin
        TCCR1A |= _BV(COM1A1); // activates the output pin
    }
    else if (pin == 2 || pin == 10)
    {
        DDRB |= _BV(PORTB2);
        TCCR1A |= _BV(COM1B1);
    }
    setPwmDuty(pin, duty);
    resume(); // Lex - make sure the clock is running.  We don't want to restart the count, in case we are starting the second WGM
              // and the first one is in the middle of a cycle
}

void TimerOne::disablePwm(char pin)
{
    if (pin == 1 || pin == 9)
        TCCR1A &= ~_BV(COM1A1); // clear the bit that enables pwm on PB1
    else if (pin == 2 || pin == 10)
        TCCR1A &= ~_BV(COM1B1); // clear the bit that enables pwm on PB2
}

void TimerOne::attachInterrupt(void (*isr)(), long microseconds)
{
    if (microseconds > 0)
        setPeriod(microseconds);
    isrCallback = isr;   // register the user's callback with the real ISR
    TIMSK1 = _BV(TOIE1); // sets the timer overflow interrupt enable bit
                         // might be running with interrupts disabled (eg inside an ISR), so don't touch the global state
                         //  sei();
    resume();
}

void TimerOne::detachInterrupt()
{
    TIMSK1 &= ~_BV(TOIE1); // clears the timer overflow interrupt enable bit
                           // timer continues to count without calling the isr
}

void TimerOne::resume() // AR suggested
{
    TCCR1B |= clockSelectBits;
}

void TimerOne::restart() // Depricated - Public interface to start at zero - Lex 10/9/2011
{
    start();
}

void TimerOne::start() // AR addition, renamed by Lex to reflect it's actual role
{
    unsigned int tcnt1;

    TIMSK1 &= ~_BV(TOIE1); // AR added
    GTCCR |= _BV(PSRSYNC); // AR added - reset prescaler (NB: shared with all 16 bit timers);

    oldSREG = SREG; // AR - save status register
    cli();          // AR - Disable interrupts
    TCNT1 = 0;
    SREG = oldSREG; // AR - Restore status register
    resume();
    do
    { // Nothing -- wait until timer moved on from zero - otherwise get a phantom interrupt
        oldSREG = SREG;
        cli();
        tcnt1 = TCNT1;
        SREG = oldSREG;
    } while (tcnt1 == 0);

    //  TIFR1 = 0xff;              		// AR - Clear interrupt flags
    //  TIMSK1 = _BV(TOIE1);              // sets the timer overflow interrupt enable bit
}

void TimerOne::stop()
{
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12)); // clears all clock selects bits
}

unsigned long TimerOne::read() // returns the value of the timer in microseconds
{                              // rember! phase and freq correct mode counts up to then down again
    unsigned long tmp;         // AR amended to hold more than 65536 (could be nearly double this)
    unsigned int tcnt1;        // AR added

    oldSREG = SREG;
    cli();
    tmp = TCNT1;
    SREG = oldSREG;

    char scale = 0;
    switch (clockSelectBits)
    {
    case 1: // no prescalse
        scale = 0;
        break;
    case 2: // x8 prescale
        scale = 3;
        break;
    case 3: // x64
        scale = 6;
        break;
    case 4: // x256
        scale = 8;
        break;
    case 5: // x1024
        scale = 10;
        break;
    }

    do
    { // Nothing -- max delay here is ~1023 cycles.  AR modified
        oldSREG = SREG;
        cli();
        tcnt1 = TCNT1;
        SREG = oldSREG;
    } while (tcnt1 == tmp); // if the timer has not ticked yet

    // if we are counting down add the top value to how far we have counted down
    tmp = ((tcnt1 > tmp) ? (tmp) : (long)(ICR1 - tcnt1) + (long)ICR1); // AR amended to add casts and reuse previous TCNT1
    return ((tmp * 1000L) / (F_CPU / 1000L)) << scale;
}

#endif


/*----------------------------Start Of Our code-----------------------*/
// (C) John Webb, Carl Dahlqvist group 20 (2023)
// Work package 4
// Exercise 3
// Submission Code: 41020
/*----------------------------Our code-----------------------*/

#include <Adafruit_NeoPixel.h>

// Based on the following tutorial: https://www.instructables.com/Arduino-Timer-Interrupts/
#define DATA_RATE 9600 // Define data rate for Serial

//-----definitions-----//
#define temPin A0                                                              // temp pin
#define ringPin 3                                                              // ring pin
#define LEDPin 12                                                              // LED pin
#define OFF 0                                                                  // boolean OFF value
#define ON 1                                                                   // boolean ON value
#define MAX 50                                                                 // maximum value
#define MIN -10

/*temperature values*/
#define TEMP0 0
#define TEMP1 1
#define TEMP10 10
#define TEMP11 11
#define TEMP20 20
#define TEMP21 21
#define TEMP30 30
#define TEMP31 31
#define TEMP40 40
#define TEMP41 41

                                                         // minimum value
Adafruit_NeoPixel ring = Adafruit_NeoPixel(12, ringPin, NEO_GRB + NEO_KHZ800); // declare the ring

int timer1 = 0;
int array[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // Declear our array which we will use for indexing on and off lights

void setup()
{
    Timer1.initialize(1000000); // Initialize timer with 1 second interval specified by parameter
    Timer1.attachInterrupt(tempChange);
    ring.begin(); // initiate ring
    pinMode(LEDPin, INPUT);

    Serial.begin(9600); // Begin serial
}

void loop()
{
}

void tempChange()
{
    int reading = analogRead(A0);
    float voltage = reading * 5.0195;
    voltage /= 1024.0;
    float temp = (voltage - 0.5) * 100; // converting from 10 mv per degree wit 500 mV offset
    //If statement for deciding when the red LED should be turned on
    if(temp < MAX){ // if below max we turn off
        lightLED(OFF);   // Turn OFF LED
    }else if(temp >= MAX){ // If its over or equal to our max we turn on
        lightLED(ON);   // Turn ON LED
    }

    if (temp < MIN) // MIN is -10 which we are using as breakpoint
    {
        turnOffLight(OFF); // turns off all lights from index 0
    }
    if (temp >= MIN && temp <= TEMP0)
    {
        turnOnLight(1);  // turns ON lights from index 1 backward
        turnOffLight(2); // turn off lights from index 2.
    }
    if (temp >= TEMP1 && temp <= TEMP10)
    {
        turnOnLight(3);  // turns ON lights from index 3 backward
        turnOffLight(4); // turn off lights from index 4.
    }
    if (temp >= TEMP11 && temp <= TEMP20)
    {
        turnOnLight(5);  // turns ON lights from index 5 backward
        turnOffLight(6); // turn off lights from index 6.
    }
    if (temp >= TEMP21 && temp <= TEMP30)
    {
        turnOnLight(7);  // turns ON lights from 7 backward
        turnOffLight(8); // turn off lights from index 8.
    }
    if (temp >= TEMP31 && temp < TEMP40)
    {
        turnOnLight(9);   // turns ON lights from 9 backward
        turnOffLight(10); // turn off lights from index 10.
    }
    if (temp >= TEMP41 && temp < MAX)
    {
        turnOnLight(11);  // turns ON lights from 11 backward
        turnOffLight(12); // turn off lights from index 12.
    }
    
    ring.show(); // show the newly set lights

    Serial.print("Tempeture is: "); // Print text
    Serial.println(temp);           // Print temp
}

void turnOnLight(int value) // Function for turning ON lights. It takes a paramater integer.
{

    for (int j = value; array[j]; j--) // from this position we iterate backwards in the array and turn ON lights
    {
        ring.setPixelColor(j, 51, 204, 51); // turns lights on
    }
}

void turnOffLight(int value) // Function for turning OFF lights. It takes a paramater integer.
{

    for (int i = value; array[i]; i++) // from this position we iterate trough the array and turn off lights
    {
        ring.setPixelColor(i, 0, 0, 0); // turns lights off
    }
}

void lightLED(int boolean) // function for turning on and off the red LED, takes in paramater 1 for ON and 0 for OFF.
{
    if (boolean == 1) // if paramater is 1
    {
        digitalWrite(LEDPin, HIGH); // turn on LED
    }
    else
    {
        digitalWrite(LEDPin, LOW); // turn off LED
    }
}
