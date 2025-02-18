#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define LCD_COLUMNS 16
#define LCD_ROWS 2

extern LiquidCrystal_I2C lcd;

void writeToDisplay(String linea1, String linea2);

#endif
