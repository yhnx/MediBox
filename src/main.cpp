#include "screen.h"
#include "clock.h"
#include "global.h"
#include "menu.h"

void setup()
{
  Serial.begin(115200); // Use a higher baud rate for ESP boards

  // Pin setup
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(CANCEL_BUT, INPUT);

  // Initialize the display
  initialize_display();

  // setup the time
  get_time_wifi();

  // Display the welcome message
  print_line("Welcome to MediBox");
}

void loop()
{
  // check_alarm();
  if (digitalRead(OK_BUT) == LOW)
  {
    delay(200);
    Serial.println("OK Button Pressed");
    go_to_menu();
  }
}
