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

  WiFi.begin("Wokwi-GUEST", "", 6); // Connect to the WiFi network
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  display.clearDisplay();
  print_line("Connected to WiFi");
  delay(400);
  display.clearDisplay();

  configTime(UTC_OFFSET, DST_OFFSET, NTP_SERVER);

  // setup the time
  update_time();

  // Display the welcome message
  print_line("Welcome to MediBox");
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
