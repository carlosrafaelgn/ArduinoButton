#include <ArduinoButton.h>

Button button1;
Button button2;

// Count how many times button1 has been pressed
unsigned short times;

void setup()
{
  // Prepare the port to send data, so we can see button1 working
  Serial.begin(9600);
  
  // Set up button1 on pin 2
  button1.begin(2, button1_Press);
  
  // Set up button2 on pin 3
  button2.begin(3, button2_Press);
  
  times = 0;
}

// This function is executed whenever button1 is pressed
void button1_Press()
{
  times = times + 1;
  Serial.println(times);
}

// This function is executed whenever button2 is pressed
void button2_Press()
{
  // Everytime button2 is pressed, we enable/disable button1 accordingly
  if (button1.isEnabled())
  {
    button1.disable();
    Serial.println("button1 has been disabled!");
  }
  else
  {
    button1.enable();
    Serial.println("button1 has been enabled!");
  }
}

void loop()
{
  // You must add one of these for each button you use!
  button1.process();
  button2.process();
}
