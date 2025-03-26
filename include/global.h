#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

// OLED Parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Alarm Parameters
#define BUZZER 5
#define GREEN_LED 15

// Button Parameters
#define CANCEL_BUT 34
#define OK_BUT 32
#define UP_BUT 33
#define DOWN_BUT 35

// DHT22 Parameters
#define DHTPIN 12

extern String User; // User name

// Function Prototypes
String Stringify(int number); // Convert an integer to a 2 digit string always

#endif