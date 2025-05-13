#include "menu.h"

int current_mode = 0;
int max_modes = 5;
String modes[][2] = {{"Set Time", "Zone"}, {"Set Alarm", "1"}, {"Set Alarm", "2"}, {"View", "Alarms"}, {"Dis/able", "Alarm"}};

volatile bool CANCEL_PRESSED = false;

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
      temp_hour = temp_hour % 15;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_hour--;
      if (temp_hour < -12)
      {
        temp_hour = 14;
      }
    }
    else if (pressed == OK_BUT)
    {
      break;
    }
  }
  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    return;
  }

  while (!CANCEL_PRESSED) // Loop until the user sets the minute
  {
    display.clearDisplay();

    String enter_min[] = {"Enter Mins", Stringify(temp_minute)};
    print_multi_line(enter_min, 2, 2, 7);

    int pressed = button_pressed();

    if (pressed == UP_BUT)
    {
      temp_minute += 15;
      temp_minute = temp_minute % 60;
    }
    else if (pressed == DOWN_BUT)
    {
      temp_minute -= 15;
      if (temp_minute < 0)
      {
        temp_minute = 45;
      }
    }
    else if (pressed == OK_BUT)
    {
      break;
    }
  }
  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    return;
  }

  if (temp_hour < 0)
  {
    UTC_OFFSET = temp_hour * 60 * 60 - temp_minute * 60;
  }
  else
  {
    UTC_OFFSET = temp_hour * 60 * 60 + temp_minute * 60;
  }

  String message[] = {"Offset is", String(temp_hour) + ":" + Stringify(temp_minute)};
  print_multi_line(message, 2, 2, 7);

  configTime(UTC_OFFSET, DST_OFFSET, NTP_SERVER);

  delay(2000);
}

void set_alarm(int alarm_number) // Set new alarm manually
{
  int temp_hour = alarm_hours[alarm_number];
  int temp_minute = alarm_minutes[alarm_number];

  display.clearDisplay();

  while (!CANCEL_PRESSED) // Loop until the user sets the hour
  {
    display.clearDisplay();

    String enter_hour[] = {"Enter Hour", Stringify(temp_hour) + " H" + " " + Stringify(temp_minute) + " M"};
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
  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    return;
  }

  while (!CANCEL_PRESSED) // Loop until the user sets the minute
  {
    display.clearDisplay();

    String enter_min[] = {"Enter Mins", Stringify(temp_hour) + " H" + " " + Stringify(temp_minute) + " M"};
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

  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    return;
  }
  String message[] = {"Set to", Stringify(temp_hour) + ":" + Stringify(temp_minute)};
  print_multi_line(message, 2, 2, 7);
  delay(1000);
}

void view_alarms() // View alarms
{
  display.clearDisplay();

  String availability = String(alarms_activated[0] ? "ON" : "OFF");
  String message[] = {
      "Alarm 1",
      Stringify(alarm_hours[0]) + ":" + Stringify(alarm_minutes[0]),
      availability};
  print_multi_line(message, 3, 2, 5);
  delay(2000);

  display.clearDisplay();

  message[0] = "Alarm 2";
  message[1] = Stringify(alarm_hours[1]) + ":" + Stringify(alarm_minutes[1]);
  availability = String(alarms_activated[1] ? "ON" : "OFF");

  print_multi_line(message, 3, 2, 5);
  delay(2000);
}

void deactivate_alarms() // Deactivate alarms
{
  display.clearDisplay();

  int temp = 0;

  while (!CANCEL_PRESSED)
  {
    display.clearDisplay();

    String enter_hour[] = {"Change", " Status ", Stringify(temp + 1)};
    print_multi_line(enter_hour, 3, 2, 5);

    int pressed = button_pressed();

    if (pressed == UP_BUT or pressed == DOWN_BUT)
    {
      temp++;
      temp = temp % 2;
    }

    if (pressed == OK_BUT)
    {
      alarms_activated[temp] = !alarms_activated[temp];

      String availability = String(alarms_activated[temp] ? "Active" : "Deactive");
      String message[] = {"Alarm " + Stringify(temp + 1), availability};
      print_multi_line(message, 2, 2, 7);
      delay(2000);
      break;
    }
  }
  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    return;
  }
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
  case 3:
    view_alarms();
    break;
  case 4:
    deactivate_alarms();

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
  if (CANCEL_PRESSED)
  {
    CANCEL_PRESSED = false;
    detachInterrupt(digitalPinToInterrupt(CANCEL_BUT)); // Disable the interrupt
    return;
  }
}
