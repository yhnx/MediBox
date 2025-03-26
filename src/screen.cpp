#include "screen.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_line(const String &text, float text_size, float row, float col) // Centers text horizontally
{
  // Calculate the width of the text
  int text_width = text.length() * text_size * 6; // 6 pixels per character width at size 1

  // Calculate the starting column to center the text horizontally
  int column = (SCREEN_WIDTH - text_width) / 2;

  // Setup the display
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);

  // Print the text
  display.println(text);
  display.display();
}

void print_multi_line(const String text[], int num_lines, int text_size, int start_row) // Supports a string array
{
  // Setup the display
  display.clearDisplay();
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);

  // Calculate the height of each line
  int line_height = text_size * 8;

  for (int i = 0; i < num_lines; i++)
  {
    // Calculate the width of the current text
    int text_width = text[i].length() * text_size * 6; // 6 pixels per character width at size 1

    // Calculate the starting column to center the text horizontally
    int column = (SCREEN_WIDTH - text_width) / 2;

    // Set the cursor for the current line
    display.setCursor(column, start_row + (i * (line_height + 5)));

    display.println(text[i]);
  }

  display.display();
}

void initialize_display() // Initialize the display
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("DISPLAY ERROR: Failed to initialize!"));
    while (true)
      ; // Halt execution
  }
  else
  {
    Serial.println(F("DISPLAY CONNECTED"));
  }
}

void print_time(const String &text, int text_size, int row, int column) // prints a given string on the display
{

  // setup the display
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);

  display.println(text);
  display.display();
}