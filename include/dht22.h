#ifndef DHT22_H
#define DHT22_H

#include <DHTesp.h>

#include "global.h"
#include "screen.h"

extern DHTesp dhtSensor; // DHT22 sensor object

void check_temp(); // Check the temperature and humidity

#endif