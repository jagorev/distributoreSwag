#include "display.h"

// Create an LCD object with the I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

void writeToDisplay(String line1, String line2)
{
  lcd.setCursor(0, 0); // Set the cursor to the first column, first row
  lcd.print(line1);    // Print message on the first row
  lcd.setCursor(0, 1); // Set the cursor to the first column, second row
  lcd.print(line2);    // Print message on the second row
}
