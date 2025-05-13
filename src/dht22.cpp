#include "dht22.h"

DHTesp dhtSensor;
volatile bool led_on = false;

void check_temp()
{
    TempAndHumidity dht_data = dhtSensor.getTempAndHumidity();

    bool temp_high = dht_data.temperature > 32;
    bool temp_low = dht_data.temperature < 24;
    bool hum_high = dht_data.humidity > 80;
    bool hum_low = dht_data.humidity < 65;

    // If any condition is abnormal
    if (temp_high || temp_low || hum_high || hum_low)
    {
        digitalWrite(RED_LED, HIGH);
        led_on = true;

        display.clearDisplay();
        String message[] = {"", ""};

        if (temp_high)
            message[0] = "TEMP HIGH!";
        else if (temp_low)
            message[0] = "TEMP LOW!";

        if (hum_high)
            message[1] = "HUM. HIGH!";
        else if (hum_low)
            message[1] = "HUM. LOW!";

        print_multi_line(message, 2, 2, 7);
        delay(1000);
    }
    if (led_on == true && !temp_high && !temp_low && !hum_high && !hum_low)
    {
        digitalWrite(RED_LED, LOW);
        led_on = false;
    }
}
