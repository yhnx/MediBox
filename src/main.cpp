#include "screen.h"
#include "clock.h"
#include "global.h"
#include "menu.h"
#include "dht22.h"

void setup()
{
  Serial.begin(115200); // Use a higher baud rate for ESP boards

  // Pin setup
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(CANCEL_BUT, INPUT);
  pinMode(OK_BUT, INPUT);
  pinMode(UP_BUT, INPUT);
  pinMode(DOWN_BUT, INPUT);

  // Initialize the display
  initialize_display();

  // Initialize the DHT22 sensor
  dhtSensor.setup(DHTPIN, DHTesp::DHT22);

  display.clearDisplay();

  // Display the welcome message
  // Display the welcome message
  String messages[] = {"Welcome", User};
  print_multi_line(messages, 2, 2, 10);

  WiFi.begin("Wokwi-GUEST", "", 6); // Connect to the WiFi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    String setup[] = {"Connecting", "WiFi..."};
    print_multi_line(setup, 2, 2, 10);
  }

  String connected[] = {"Connected", "to WiFi"};
  print_multi_line(connected, 2, 2, 10);
  delay(400);
  display.clearDisplay();

  configTime(UTC_OFFSET, DST_OFFSET, NTP_SERVER);

  // setup the time
  update_time();
}

void loop()
{
  check_alarm();
  if (digitalRead(OK_BUT) == LOW)
  {
    delay(200);
    go_to_menu();
  }
  check_temp();
}
