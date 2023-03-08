
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
  unsigned long tmp;           // AR amended to hold more than 65536 (could be nearly double this)
  unsigned int tcnt1;          // AR added

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
// Exercise 1
// Submission Code: 41020
/*----------------------------Our code-----------------------*/


//-----pins-----
#define tempPin A0
#define bluePin 12
#define orangePin 11
#define greenPin 10
#define yellowPin 9
#define redPin 8
/*temperature values*/
#define TEMP0 0
#define TEMP1 1
#define TEMP10 10
#define TEMP11 11
#define TEMP20 20
#define TEMP21 21
#define TEMP30 30
#define TEMP31 31

float last_temp; // variable to store the last temperature value

void setup()
{
  pinMode(tempPin, INPUT);    // temp input
  pinMode(bluePin, OUTPUT);   // sets the digital pin 12 as output
  pinMode(orangePin, OUTPUT); // sets the digital pin 11 as output
  pinMode(greenPin, OUTPUT);  // sets the digital pin 10 as output
  pinMode(yellowPin, OUTPUT); // sets the digital pin 9 as output
  pinMode(redPin, OUTPUT);    // sets the digital pin 8 as output
  Serial.begin(9600);    // Begin serial
  Timer1.initialize(1000000); // Initialize timer with 1 second interval specified by parameter
  Timer1.attachInterrupt(tempChange);
}

void loop()
{
}

void tempChange()
{
  // the loop function is now empty since the interrupt handles the temperature change detection
  int reading = analogRead(tempPin);
  float voltage = reading * 5.0195;
  voltage /= 1024.0;
  float temp = (voltage - 0.5) * 100; // converting from 10 mv per degree wit 500 mV offset

  last_temp = temp; // update last_temp variable
  if (temp <= TEMP0)   // If the temp is equal or below 0C
  {
    blueLight(); // We call to turn on blue light
  }
  if (temp >= TEMP1 && temp <= TEMP10) // If the temp is between 1 celsius and 10 celsius
  {
    orangeLight(); // We call to turn on orange light
  }
  if (temp >= TEMP11 && temp <= TEMP20) // If the temp is between 11 celsius and 20 celsius
  {
    greenLight(); // We call to turn on green light
  }
  if (temp >= TEMP21 && temp <= TEMP30) // If the temp is between 21 celsius and 30 celsius
  {
    yellowLight(); // We call to turn on yellow light
  }
  if (temp >= TEMP31) // If the temp is larger or equal to 31 celsius
  {
    redLight(); // We call to turn on red light
  }
  Serial.print("Temperature is: ");
  Serial.println(last_temp); // We print out the temperature
}
void setLights(int blue, int orange, int green, int yellow, int red) // Function for setting pins to high and low
{
  digitalWrite(bluePin, blue);
  digitalWrite(orangePin, orange);
  digitalWrite(greenPin, green);
  digitalWrite(yellowPin, yellow);
  digitalWrite(redPin, red);
}

void blueLight() //Set bluePin to HIGH and rest to LOW
{
  setLights(1, 0, 0, 0, 0);
}
void orangeLight() //Set orangePin to HIGH and rest to LOW
{
  setLights(0, 1, 0, 0, 0);
}
void greenLight()//Set greenPin to HIGH and rest to LOW
{
  setLights(0, 0, 1, 0, 0);
}
void yellowLight()//Set yellowPin to HIGH and rest to LOW
{
  setLights(0, 0, 0, 1, 0);
}
void redLight()//Set redPIN to HIGH and rest to LOW
{
  setLights(0, 0, 0, 0, 1);
}
