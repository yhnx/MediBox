#include "screen.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_line(const String &text, int text_size, int row, int column) //prints a given string on the display
{
  display.clearDisplay();

  //setup the display
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);


  display.println(text);
  display.display();


  delay(500);
}


void initialize_display() // Initialize the display
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("DISPLAY ERROR: Failed to initialize!"));
    while (true); // Halt execution
  }
  else
  {
     Serial.println(F("DISPLAY CONNECTED"));
  }
}

void print_time(const String &text, int text_size, int row, int column) //prints a given string on the display
{

  //setup the display
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);


  display.println(text);
  display.display();


}