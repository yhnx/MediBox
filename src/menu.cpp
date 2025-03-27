#include "menu.h"

int current_mode = 0;
int max_modes = 4;
String modes[][2] = {{"Set", "Zone"}, {"Set Alarm", "1"}, {"Set Alarm", "2"}, {"Disable", "Alarms"}};

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

void set_zone() // Set new zone manually
{

  int temp_hour = 0;
  int temp_minute = 0;

  display.clearDisplay();
  String load_message[] = {"Enter New", "Time Zone"};
  print_multi_line(load_message, 2, 2, 7);
  delay(1000);
  display.clearDisplay();

  while (!CANCEL_PRESSED) // Loop until the user sets the hour
  {
    display.clearDisplay();

    String enter_hour[] = {"Enter Hour", String(temp_hour)};
    print_multi_line(enter_hour, 2, 2, 7);

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_hour++;
      temp_hour = temp_hour % 12;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_hour--;
      if (temp_hour < -12)
      {
        temp_hour = 0;
      }
    }
    else if (pressed == OK_BUT)
    {
      break;
    }
  }
  CANCEL_PRESSED = false;

  while (!CANCEL_PRESSED) // Loop until the user sets the minute
  {
    display.clearDisplay();

    String enter_min[] = {"Enter Mins", String(temp_minute)};
    print_multi_line(enter_min, 2, 2, 7);

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_minute += 15;
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
      break;
    }
  }

  UTC_OFFSET = temp_hour * 60 * 60 + temp_minute * 60;

  Serial.println(UTC_OFFSET);

  CANCEL_PRESSED = false;

  String message[] = {"Offset is", String(temp_hour) + ":" + String(temp_minute)};
  print_multi_line(message, 2, 2, 7);

  configTime(UTC_OFFSET, DST_OFFSET, NTP_SERVER);
  delay(2000);
}

void set_alarm(int alarm_number) // Set new alarm manually
{
  int temp_hour = 0;
  int temp_minute = 0;

  display.clearDisplay();

  while (!CANCEL_PRESSED) // Loop until the user sets the hour
  {
    display.clearDisplay();

    String enter_hour[] = {"Enter Hour", String(temp_hour)};
    print_multi_line(enter_hour, 2, 2, 7);

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

    String enter_min[] = {"Enter Mins", String(temp_minute)};
    print_multi_line(enter_min, 2, 2, 7);

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
  String message[] = {"Set to ", String(temp_hour) + ":" + String(temp_minute)};
  print_multi_line(message, 2, 2, 7);
}

void run_mode(int mode)
{
  switch (mode)
  {
  case 0:
    set_zone();
    break;
  case 1:
  case 2:
    set_alarm(mode - 1);
    break;
  case 4:
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
    print_multi_line(modes[current_mode], 2, 2, 12);

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
      run_mode(current_mode);
    }
  }
  CANCEL_PRESSED = false;
  detachInterrupt(digitalPinToInterrupt(CANCEL_BUT)); // Disable the interrupt
}
