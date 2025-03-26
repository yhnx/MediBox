#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "global.h"



// Declare display as an external variable
extern Adafruit_SSD1306 display;

// Function declarations
void print_line(const String &text, int text_size = 2, int row = 0, int column = 0);
void print_time(const String &text, int text_size = 2, int row = 0, int column = 0);

void initialize_display();

#endif
