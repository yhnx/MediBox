#include "dht22.h"

DHTesp dhtSensor;

void check_temp()
{
    TempAndHumidity dht_data = dhtSensor.getTempAndHumidity();

    if (dht_data.temperature > 35)
    {
        display.clearDisplay();
        print_line("TEMP HIGH!", 2);
    }
    else if (dht_data.temperature < 15)
    {
        display.clearDisplay();
        print_line("TEMP LOW!", 2);
    }

    if (dht_data.humidity > 40)
    {
        display.clearDisplay();
        print_line("HUM. HIGH!", 2, 30);
    }
    else if (dht_data.humidity < 20)
    {
        display.clearDisplay();
        print_line("HUM. LOW!", 2, 30);
    }
}