#include "screen.h"
#include "clock.h"
#include "global.h"
#include "menu.h"
#include "dht22.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <math.h>

// WiFi credentials
const char *ssid = "Wokwi-GUEST";
const char *password = "";

// MQTT broker details
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// MQTT topics
const char *temp_topic = "medibox220046R/temperature";
const char *intensity_topic = "medibox220046R/intensity";
const char *sampling_topic = "medibox220046R/intensitySampling";
const char *sending_topic = "medibox220046R/intensitySending";
const char *offset_topic = "medibox220046R/offset";
const char *controlfactor_topic = "medibox220046R/controlfactor";
const char *tempset_topic = "medibox220046R/tempset";

// LDR and servo parameters
#define MAX_LDR_VALUE 4095 // resolution for 12-bit
#define MIN_LDR_VALUE 0

// Default configuration values
float sampling_interval = 5.0;  // seconds
float sending_interval = 120.0; // for 2 minutes
float theta_offset = 30.0;
float gammah = 0.75;
float T_med = 30.0;

// Variables for LDR averaging
float ldr_readings_sum = 0.0;
int ldr_readings_count = 0;
unsigned long last_sample_time = 0;
unsigned long last_send_time = 0;

// Servo object from ESP32Servo library
Servo servo;

// WiFi and MQTT clients from PubSubClient library
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{
  delay(10);
  WiFi.begin(ssid, password);
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
}

// callback function for MQTT messages
void callback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  float value = message.toFloat();

  if (String(topic) == sampling_topic)
  {
    sampling_interval = constrain(value, 1.0, 60.0); // Limiting to reasonable range
  }
  else if (String(topic) == sending_topic)
  {
    sending_interval = constrain(value, 30.0, 600.0); // Limiting to 30s–10min
  }
  else if (String(topic) == offset_topic)
  {
    theta_offset = constrain(value, 0.0, 120.0);
  }
  else if (String(topic) == controlfactor_topic)
  {
    gammah = constrain(value, 0.0, 1.0);
  }
  else if (String(topic) == tempset_topic)
  {
    T_med = constrain(value, 10.0, 40.0);
  }
}

// Function to connect to MQTT broker
void reconnect()
{
  while (!client.connected())
  {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      // Subscribe to configuration topics
      client.subscribe(sampling_topic);
      client.subscribe(sending_topic);
      client.subscribe(offset_topic);
      client.subscribe(controlfactor_topic);
      client.subscribe(tempset_topic);
    }
    else
    {
      delay(5000);
    }
  }
}

void check_temp()
{
  static unsigned long last_temp_time = 0;
  if (millis() - last_temp_time >= 5000)
  { // Reading every 5 seconds
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    if (!isnan(data.temperature))
    {
      char temp_str[8];
      dtostrf(data.temperature, 6, 2, temp_str);
      client.publish(temp_topic, temp_str);
    }
    last_temp_time = millis();
  }
}

void read_ldr()
{
  if (millis() - last_sample_time >= sampling_interval * 1000)
  {
    int ldr_raw = analogRead(LDR_PIN);
    // Normalize LDR value to 0–1 (inversion)
    float intensity = 1.0 - ((float)ldr_raw - MIN_LDR_VALUE) / (MAX_LDR_VALUE - MIN_LDR_VALUE);
    intensity = constrain(intensity, 0.0, 1.0);

    ldr_readings_sum += intensity;
    ldr_readings_count++;
    last_sample_time = millis();
  }
}

void send_average_intensity()
{
  if (millis() - last_send_time >= sending_interval * 1000 && ldr_readings_count > 0)
  {
    float average_intensity = ldr_readings_sum / ldr_readings_count;
    char intensity_str[8];
    dtostrf(average_intensity, 6, 4, intensity_str);
    client.publish(intensity_topic, intensity_str);

    // Reseting them
    ldr_readings_sum = 0.0;
    ldr_readings_count = 0;
    last_send_time = millis();
  }
}

void adjust_servo()
{
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (!isnan(data.temperature) && ldr_readings_count > 0)
  {
    float I = ldr_readings_sum / ldr_readings_count; // the current average intensity
    float T = data.temperature;

    // Servo angle calculation
    float theta = theta_offset + (180.0 - theta_offset) * I * gammah * log(sampling_interval / sending_interval) * (T / T_med);
    theta = constrain(theta, 0.0, 180.0); // Ensure angle is within 180 degrees
    servo.write((int)theta);
  }
}

void setup()
{
  Serial.begin(115200);

  // Pin setup
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(CANCEL_BUT, INPUT);
  pinMode(OK_BUT, INPUT);
  pinMode(UP_BUT, INPUT);
  pinMode(DOWN_BUT, INPUT);
  pinMode(LDR_PIN, INPUT);

  // Initialize servo
  servo.attach(SERVO_PIN);
  servo.write(0); // Initial position

  // Initialize display
  initialize_display();
  display.clearDisplay();

  // Display welcome message
  String messages[] = {"Welcome", User};
  print_multi_line(messages, 2, 2, 10);

  // Initialize DHT22 sensor
  dhtSensor.setup(DHTPIN, DHTesp::DHT22);

  // Connect to WiFi
  setup_wifi();

  // Set up MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Configure time
  configTime(UTC_OFFSET, DST_OFFSET, NTP_SERVER);
  update_time();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  check_alarm();
  if (digitalRead(OK_BUT) == LOW)
  {
    delay(200);
    go_to_menu();
  }
  check_temp();
  read_ldr();
  send_average_intensity();
  adjust_servo();
}