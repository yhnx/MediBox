#include "menu.h"

int current_mode = 0;
int max_modes = 4;
String modes[] = {"1: Set Time", "2: Set Alarm 1", "3: Set Alarm 2", "4: Disable Alarms"};

bool CANCEL_PRESSED = false;

void IRAM_ATTR InterruptMenu()
{
  CANCEL_PRESSED = true;
}

int button_pressed()
{
  while (true)
  {
    if (digitalRead(UP_BUT) == LOW)
    {
      delay(200);
      return UP_BUT;
    }
    else if (digitalRead(DOWN_BUT) == LOW)
    {
      delay(200);
      return DOWN_BUT;
    }
    if (digitalRead(OK_BUT) == LOW)
    {
      delay(200);
      return OK_BUT;
    }

    update_time();
  }
}

void go_to_menu()
{
  attachInterrupt(digitalPinToInterrupt(CANCEL_BUT), InterruptMenu, RISING); // Configure the interrupt
  while (!CANCEL_PRESSED)
  {
    display.clearDisplay();
    print_line(modes[current_mode]);

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      current_mode++;
      current_mode = current_mode % max_modes;
    }

    else if (pressed == DOWN_BUT)
    {
      current_mode--;
      if (current_mode < 0)
      {
        current_mode = max_modes - 1;
      }
    }
    else if (pressed == OK_BUT)
    {
      Serial.println(modes[current_mode]);
      // run_mode(current_mode);
    }
  }
  CANCEL_PRESSED = false;
  detachInterrupt(digitalPinToInterrupt(CANCEL_BUT)); // Disable the interrupt
}
