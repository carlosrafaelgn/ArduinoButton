//******************************************************************
// Copyright (c) 2015, Carlos Rafael Gimenes das Neves
// This projected is licensed under the terms of the FreeBSD
// License. See LICENSE.txt for more details.
//
// https://github.com/carlosrafaelgn/ArduinoButton
//******************************************************************

// If your buttons are active-low (their pins become 0 when pressed,
// and 1 when released), please define ArduinoButtonPressedZero before
// including the library

#define ArduinoButtonPressedZero

#include <ArduinoButton.h>

Button button1;

// Count how many times button1 has been pressed
unsigned short times;

void setup()
{
  // Prepare the port to send data, so we can see button1 working
  Serial.begin(9600);
  
  // Set up button1 on pin 2
  button1.begin(2, button1_Press);
  
  times = 0;
}

// This function is executed whenever button1 is pressed
void button1_Press()
{
  times = times + 1;
  Serial.println(times);
}

void loop()
{
  // You must add one of these for each button you use!
  button1.process();
}
