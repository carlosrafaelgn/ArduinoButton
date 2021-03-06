//******************************************************************
// Copyright (c) 2015, Carlos Rafael Gimenes das Neves
// This projected is licensed under the terms of the FreeBSD
// License. See LICENSE.txt for more details.
//
// https://github.com/carlosrafaelgn/ArduinoButton
//******************************************************************

#include <ArduinoButton.h>

Button button1;

// Count how many times button1 has been pressed
unsigned short times;

// Count how many times button1 has been released
unsigned short timesReleased;

void setup()
{
  // Prepare the port to send data, so we can see button1 working
  Serial.begin(9600);
  
  // Set up button1 on pin 2
  button1.begin(2, button1_Press, button1_Release);
  
  times = 0;
  timesReleased = 0;
}

// This function is executed whenever button1 is pressed
void button1_Press()
{
  times = times + 1;
  Serial.print(times);
  Serial.print(" - ");
  Serial.println(timesReleased);
}

// This function is executed whenever button1 is pressed
void button1_Release()
{
  timesReleased = timesReleased + 1;
  Serial.print(times);
  Serial.print(" - ");
  Serial.println(timesReleased);
}

void loop()
{
  // You must add one of these for each button you use!
  button1.process();
}
