#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>   // For struct tm, strftime
#include <stdlib.h> // For atoi
#include <WiFi.h>   // For getLocalTime

#include "screen.h"
#include "global.h"

extern int day;
extern int hour;
extern int minute;
extern int second;

extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarms_activated;

extern int UTC_OFFSET;
extern int DST_OFFSET; // daylight saving time
extern char* NTP_SERVER;

void print_current_time();
void update_time();
void ring_alarm();
void check_alarm();
void get_time_wifi();

#endif