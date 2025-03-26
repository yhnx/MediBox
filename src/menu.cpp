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
    if (digitalRead(CANCEL_BUT) == LOW)
    {
      delay(200);
      return CANCEL_BUT;
    }

    update_time();
  }
}

void set_time() // Set new time manually
{

  int temp_hour = 0;
  int temp_minute = 0;

  print_line("Enter the time");
  print_line("HH:MM", 2, 15, 15);
  delay(200);

  while (!CANCEL_PRESSED) // Loop until the user sets the hour
  {
    display.clearDisplay();

    print_line("Enter Hour:" + String(temp_hour));

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_hour++;
      temp_hour = temp_hour % 24;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_hour--;
      if (temp_hour < 0)
      {
        temp_hour = 23;
      }
    }
    else if (pressed == OK_BUT)
    {
      hour = temp_hour;
      break;
    }
  }
  CANCEL_PRESSED = false;

  while (!CANCEL_PRESSED) // Loop until the user sets the minute
  {
    display.clearDisplay();

    print_line("Enter Minutes:" + String(temp_minute));

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_minute++;
      temp_minute = temp_minute % 60;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_minute--;
      if (temp_minute < 0)
      {
        temp_minute = 59;
      }
    }
    else if (pressed == OK_BUT)
    {
      minute = temp_minute;
      break;
    }
  }

  CANCEL_PRESSED = false;
  print_line("Time set to " + String(hour) + ":" + String(minute));
  delay(200);
}

void set_alarm(int alarm_number) // Set new alarm manually
{
  int temp_hour = 0;
  int temp_minute = 0;

  print_line("Change Alarm " + Stringify(alarm_number + 1));
  print_line("HH:MM", 2, 15, 15);
  delay(200);

  while (!CANCEL_PRESSED) // Loop until the user sets the hour
  {
    display.clearDisplay();

    print_line("Enter Hour:" + String(temp_hour));

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_hour++;
      temp_hour = temp_hour % 24;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_hour--;
      if (temp_hour < 0)
      {
        temp_hour = 23;
      }
    }
    else if (pressed == OK_BUT)
    {
      alarm_hours[alarm_number] = temp_hour;
      break;
    }
  }
  CANCEL_PRESSED = false;

  while (!CANCEL_PRESSED) // Loop until the user sets the minute
  {
    display.clearDisplay();

    print_line("Enter Minutes:" + String(temp_minute));

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_minute++;
      temp_minute = temp_minute % 60;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_minute--;
      if (temp_minute < 0)
      {
        temp_minute = 59;
      }
    }
    else if (pressed == OK_BUT)
    {
      alarm_minutes[alarm_number] = temp_minute;
      break;
    }
  }

  CANCEL_PRESSED = false;
  print_line("Alarm " + String(alarm_number) + " set to " + String(temp_hour) + ":" + String(temp_minute));
  delay(200);
}

void run_mode(int mode)
{
  switch (mode)
  {
  case 0:
    set_time();
    break;
  case 1:
  case 2:
    set_alarm(mode - 1);
    break;
  case 3:
    alarms_activated = !alarms_activated;

    print_line("Alarms Disabled");
    delay(200);
    display.clearDisplay();

    break;
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
      run_mode(current_mode);
    }
  }
  CANCEL_PRESSED = false;
  detachInterrupt(digitalPinToInterrupt(CANCEL_BUT)); // Disable the interrupt
}
