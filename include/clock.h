#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>    // For struct tm, strftime
#include <stdlib.h>  // For atoi
#include "screen.h"
#include "global.h"

extern int day;
extern int hour;
extern int minute;
extern int second;





void print_current_time();
void update_time();
void ring_alarm();
void check_alarm();
void get_time_wifi();

#endif