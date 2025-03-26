#include "clock.h"

// time zone variables
int UTC_OFFSET = 5 * 60 * 60 + 30 * 60; // UTC+5:30 in seconds
int DST_OFFSET = 0;                     // No daylight saving time
char *NTP_SERVER = "pool.ntp.org";

// time variables
int hour = 0;
int minute = 0;
int second = 0;

int day = 0;
int month = 0;

long timeNow;
long timeLast;

bool alarms_activated = true;
int num_alarms = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 20};
bool alarm_triggered[] = {false, false};

volatile bool off_the_alarm = false;

// Note variable
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

void IRAM_ATTR InterruptIt()
{
  off_the_alarm = true;
}

void print_current_time()
{
  print_time(Stringify(hour) + ":" + Stringify(minute) + ":" + Stringify(second), 2, 15, 17);
}

void print_current_time_day()
{
  print_current_time();
  print_time(Stringify(day) + "/" + Stringify(month), 2, 35, 35);
}

// Function to automatically update the current time
void update_time()
{
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char hour_str[3];
  char min_str[3];
  char sec_str[3];
  char day_str[3];
  char month_str[3];

  // Getting the current time
  strftime(hour_str, 3, "%H", &timeinfo);
  strftime(min_str, 3, "%M", &timeinfo);
  strftime(sec_str, 3, "%S", &timeinfo);
  strftime(day_str, 3, "%d", &timeinfo);
  strftime(month_str, 3, "%m", &timeinfo);

  // Converting the strings to integers
  hour = atoi(hour_str);
  minute = atoi(min_str);
  second = atoi(sec_str);
  day = atoi(day_str);
  month = atoi(month_str);
}

void ring_alarm()
{
  display.clearDisplay();

  while (!off_the_alarm) // cancelled variable added for extra robustness
  {

    print_line("MEDICINE TIME", 2, 15, 15);
    digitalWrite(GREEN_LED, HIGH);

    for (int i = 0; i < n_notes; i++)
    {

      tone(BUZZER, notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }

    // settling stuff down
    digitalWrite(GREEN_LED, LOW);
    display.clearDisplay();
  }
}

void check_alarm() // while updating time
{
  display.clearDisplay();
  update_time();
  print_current_time_day();

  if (alarms_activated)
  {
    for (int i = 0; i < num_alarms; i++)
    {
      if (!alarm_triggered[i] && alarm_hours[i] == hour && alarm_minutes[i] == minute)
      {
        attachInterrupt(digitalPinToInterrupt(CANCEL_BUT), InterruptIt, RISING); // Configure the interrupt

        ring_alarm();

        // stopping the alarm from ringing further using the cancel button
        if (off_the_alarm)
        {
          alarm_triggered[i] = true;
          off_the_alarm = false;
          detachInterrupt(digitalPinToInterrupt(CANCEL_BUT)); // Disable the interrupt
        }
      }
    }
  }
}