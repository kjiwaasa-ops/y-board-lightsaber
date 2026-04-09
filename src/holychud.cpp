#include <Arduino.h>
#include "yboard.h"
#include "math.h"

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
bool lastButton1State = false;
bool lastButton2State = false;
bool lastButton3State = false;
int last_knob_value = 0;
unsigned long lastSwingTime = 0;
const int swingCooldown = 300; // milliseconds

int red = 0;
int green = 0;
int blue = 0;
const char* swingSounds[] = {
  "lightsaber_sounds/lightsaber1.mp3",
  "lightsaber_sounds/lightsaber2.mp3",
  "lightsaber_sounds/lightsaber3.mp3",
  "lightsaber_sounds/lightsaber4.mp3",
  "lightsaber_sounds/lightsaber5.mp3"
};
const int numSounds = 5;
int prev_x = 0;
int prev_y = 0;
int prev_z = 0;
// These variables track accelerometer data. Delete them if you aren't using the accelerometer.
accelerometer_data accel_data;
int x_value;
int y_value;
int z_value;

// This function draws accelerometer data as little bars on the display

// Runs once when the board is first booted up
void setup() {
  Serial.begin(9600);
  Yboard.setup();
  randomSeed(analogRead(0));
  Yboard.play_sound_file("lightsaber_sounds/lightsaber_start.mp3");
}

void loop() {
  // Add any code you want to run repeatedly here.
  // Below are some examples of how to use different parts of the Y-Board.
  // Delete anything you don't need, and add anything you want.
  // For a full list of functions available on the Y-board, 
  // see the header file at .pio/libdeps/esp32/Y-Board v4/include/yboard.h from lines 39-330.
  
  // Erases the screen if needed. This is sort of a catch-all for weird display behavior

  // ========ACCELEROMETER========
  // get_accelerometer() returns a value from 0 to 999 for the x, y, and z axes.
  // x is pitch, y is roll, and z is a blend of the two (it's inconsistent)
  // This causes the LEDs to change color based on accelerometer data
  accel_data = Yboard.get_accelerometer();
  int x_value = accel_data.x;
  int y_value = accel_data.y;
  int z_value = accel_data.z;

  int dx = x_value - prev_x;
  int dy = y_value - prev_y;
  int dz = z_value - prev_z;

  float change = sqrt(dx*dx + dy*dy + dz*dz);

  if (change > 600 && millis() - lastSwingTime > swingCooldown)  {
    Yboard.play_sound_file(swingSounds[random(numSounds)]);
    lastSwingTime = millis();
  }
  prev_x = x_value;
  prev_y = y_value;
  prev_z = z_value;
  // =========PROVIDING VOLTAGE=========
  bool current1 = Yboard.get_button(1);

  if (current1 && !lastButton1State) {
      // button JUST pressed

      if (red == 0 && blue == 0) {
          red = 255;
          green = 0;
      } else if (blue == 0 && red == 255) {
          blue = 255;
      } else if (blue == 255 && red == 255) {
          red = 0;
      } else if (blue == 255 && red == 0) {
          blue = 0;
          green = 255;
      }

      Yboard.set_all_leds_color(red, green, blue);
      Yboard.play_sound_file("lightsaber_sounds/lightsaber_start.mp3");
  }
  lastButton1State = current1;

  bool current2 = Yboard.get_button(2);
  if (current2 && !lastButton2State) {
    Yboard.play_sound_file("lightsaber_sounds/tony-stark-was-able-to-build-this-in-a-cave!-with-a-box-of-scraps!.mp3");
  }
  lastButton2State = current2;

  bool current3 = Yboard.get_button(3);
  if (current3 && !lastButton3State) {
    Yboard.play_sound_file("lightsaber_sounds/fahhhhh-3.mp3");
  }
  lastButton3State = current3;

  int knob_reading_current = Yboard.get_knob();

  if (knob_reading_current != last_knob_value) {
    Yboard.play_sound_file("lightsaber_sounds/spinning_sabers.mp3");
  }
  last_knob_value = knob_reading_current;
}