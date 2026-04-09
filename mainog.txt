#include <Arduino.h>
#include "yboard.h"

// These pins are found on the back of the Y-board
// They can be used to either detect or provide voltage
// Examples are provided at the end of loop()
#define PIN_1 8
#define PIN_2 15

/*
This variable simplifies printing to the screen. If multiple functions are controlling the screen, we want a clear way to
erase ONLY the functions we aren't using while printing our relevant messages. Every time we print something to the screen, we
set this variable to 1 (true). If this variable is ever true, we clear the screen at the beginning of loop()
*/
int need_to_erase = 0;

// If we only want something to run once (such as printing a quick message after pressing a button), we can update this variable.
// I know that sounds confusing, but see the "PROVIDING VOLTAGE" section at the bottom of loop() for a good example.
// Feel free to rename this variable or create similar ones.
int button_pressed = 0;

// These variables track accelerometer data. Delete them if you aren't using the accelerometer.
accelerometer_data accel_data;
int x_value;
int y_value;
int z_value;

// This function draws accelerometer data as little bars on the display
void drawInfo() {

  // Draw Title
  Yboard.display.setCursor(0, 0);
  Yboard.display.clearDisplay();
  Yboard.display.write("Accelerate!");

  // Get Acceleration Info
  float x = -accel_data.x * 0.02;
  float y = accel_data.y * 0.02;
  float z = -accel_data.z * 0.02;
  int text_size = 1;

  // Draw Characters
  Yboard.display.drawChar(0, 10, 'x', 1, 0, text_size);
  Yboard.display.drawChar(0, 20, 'y', 1, 0, text_size);
  Yboard.display.drawChar(0, 30, 'z', 1, 0, text_size);

  // Draw Acceleration Bars
  if (x > 0) // x
    Yboard.display.fillRect(8, 10, x, 8, 1);
  else if (x < 0)
    Yboard.display.drawRect(8, 10, -x, 8, 1);

  if (y > 0) // y
    Yboard.display.fillRect(8, 20, y, 8, 1);
  else if (y < 0)
    Yboard.display.drawRect(8, 20, -y, 8, 1);

  if (z > 0) // z
    Yboard.display.fillRect(8, 30, z, 8, 1);
  else if (z < 0)
    Yboard.display.drawRect(8, 30, -z, 8, 1);

  Yboard.display.display();
}

// Runs once when the board is first booted up
void setup() {
  Serial.begin(9600);
  Yboard.setup();
    
  // Currently, PIN_1 (#8) is set up to detect voltage and PIN_2 (#15) is set up to provide it
  // Modify this if you'd like
  pinMode(PIN_1, INPUT);
  pinMode(PIN_2, OUTPUT);

  //Add any code you just want to run once here.
}

void loop() {
  // Add any code you want to run repeatedly here.
  // Below are some examples of how to use different parts of the Y-Board.
  // Delete anything you don't need, and add anything you want.
  // For a full list of functions available on the Y-board, 
  // see the header file at .pio/libdeps/esp32/Y-Board v4/include/yboard.h from lines 39-330.
  
  // Erases the screen if needed. This is sort of a catch-all for weird display behavior
  if(need_to_erase) {
    Yboard.display.clearDisplay(); 
    Yboard.display.display();
    need_to_erase=0;
  }


  // ========LEDS========
  Yboard.set_led_brightness(128);       // Set the brightness of all LEDs to 50% (128 out of 255)
  Yboard.set_all_leds_color(0, 255, 0); // Set all LEDs to green
  Yboard.set_led_color(1, 255, 0, 0);   // Set the first LED to red
  // ====================


  // ========BUTTONS AND SWITCHES========
  bool button_1;  // Declare variables like these at the top of your file (before setup()) so you can use them to store values.
  bool switch_1;  //

  // Use these commands to get values from the buttons and switches.
  button_1 = Yboard.get_button(1);  
  switch_1 = Yboard.get_switch(1);
  // Then use those values however you need them
  // ====================================
  

  // ========ROTARY ENCODER (KNOB)========
  int knob_position;        // Declare variables like these at the top of your file (before setup()) so you can use them to store values.
  bool knob_button_pressed; //

  // Use these commands to get values from the rotary encoder (knob).
  knob_position = Yboard.get_knob();
  knob_button_pressed = Yboard.get_knob_button();
  // Then use those values however you need them
  // =====================================


  // ========DISPLAY========
  Yboard.display.clearDisplay(); // Clear the display
  Yboard.display.setTextSize(1); // Set text size to 1 (smallest)
  Yboard.display.setTextColor(SSD1306_WHITE); // Set text color to white
  Yboard.display.setCursor(0, 0); // Set cursor to top-left corner
  Yboard.display.println("Hello, Y-Board!"); // Print text to the display

  // You can also draw shapes on the display. Here are some examples:
  Yboard.display.drawLine(0, 10, 127, 10, SSD1306_WHITE); // Draw a horizontal line across the display
  Yboard.display.drawRect(10, 15, 50, 30, SSD1306_WHITE); // Draw a rectangle
  Yboard.display.fillRect(70, 15, 50, 30, SSD1306_WHITE); // Draw a filled rectangle
  Yboard.display.drawCircle(30, 50, 10, SSD1306_WHITE); // Draw a circle
  Yboard.display.fillCircle(90, 50, 10, SSD1306_WHITE); // Draw a filled circle
  Yboard.display.drawTriangle(110, 40, 120, 60, 100, 60, SSD1306_WHITE); // Draw a triangle
  Yboard.display.fillTriangle(50, 40, 60, 60, 40, 60, SSD1306_WHITE); // Draw a filled triangle

  //ANY time you make changes to the display, they won't be shown until you call this function:
  Yboard.display.display(); // Update the display to show the changes
  //(Including if you clear the display)
  // =======================

  // ========ACCELEROMETER========
  // get_accelerometer() returns a value from 0 to 999 for the x, y, and z axes.
  // x is pitch, y is roll, and z is a blend of the two (it's inconsistent)
  // This causes the LEDs to change color based on accelerometer data
  accel_data = Yboard.get_accelerometer();
  x_value = accel_data.x;
  y_value = accel_data.y;
  z_value = accel_data.z;
  Yboard.set_all_leds_color(abs(x_value * 0.255), abs(y_value * 255), abs((z_value+(PI / 4)) * 255));
  drawInfo();
  // I think the z_value looks best when adding pi/4, you may disagree. Do as you please!
  // ============================


  // ========PLAY NOTES========
  // This plays a song on the speaker. The notes are specified in the string.
  Yboard.play_notes("T160 V2 O5 F D. C#8 D. C#8 D8 B8- O4 G8 F"); // BYU Fight Song
  // See lab 7 for info on how these strings work
  // ==========================


  // Use these functions--digitalWrite() and digitalRead()--to detect and provide voltage for anything else
  // A good place to start is with something you build on your breadboard :)

  // =========READING EXTERNAL VOLTAGE=========
  if (digitalRead(PIN_1)){ // digitalRead keeps an eye on PIN_1 (pin 8). Feel free to change between PIN_1 and PIN_2
    Yboard.set_all_leds_color(255,0,0); // If voltage is detected in PIN_1, then all the Yboard LEDs will be set to red.
  } else {
    Yboard.set_all_leds_color(0,0,0); // If voltage is NOT detected in PIN_1, then all the Yboard LEDs will be turned off.
  }
  // Feel free to change or mess around with any of these values/functions
  // ==========================


  // =========PROVIDING VOLTAGE=========
  if (Yboard.get_button(1)) {  // the get_button analyzes the state of button 1. If it is pressed it report "TRUE" otherwise it will report "FALSE."

    Yboard.display.setCursor(3, 3); // These lines just print the button state to the screen
    Yboard.display.println("Button 1 pressed!");
    Yboard.display.display();
    button_pressed = 1;

    digitalWrite(PIN_2, HIGH);  // this command will provide voltage from PIN_2 (pin 15) when we press button 1.
  } else {
    digitalWrite(PIN_2, LOW);   // This stops providing voltage from PIN_2 when button 1 isn't being pressed.

    /* EXPLAINING THESE VARIABLES (or why I made this way too complicated)
    When we release the button, we obviously want to erase the message saying the button is pressed.
    One option is to simply clear the screen whenever our button isn't pressed, but that would erase everything else too.
    Another option is to rapidly erase and redraw the message each loop, but that creates an awful flicker.

    Here, we use the variable button_pressed to track if we've pressed down button 1.
    The program will only erase the screen if:
    1. We've pressed the button recently.
    2. The button is not currently pressed.
    3. The following logic has not run since the last time we released button 1.

    Setting button_pressed to 0 here lets our program know that 
    we've already accomplished what we needed to (erasing the screen).
    Afterward, button_pressed will remain 0 until we press button 1 again, after which our program will 
    run this code again when we next release it.

    IT'S OK IF YOU DON'T UNDERSTAND THIS! This is just me programming in my weird little way. If you want to understand/modify
    it, just reach out to me at ibingham@student.byu.edu
    */  
    if (button_pressed) {
      need_to_erase = 1;
      button_pressed = 0;
    }
  }
  // ==========================

  
}