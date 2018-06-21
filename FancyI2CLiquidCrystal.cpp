#include "Arduino.h"
#include "FancyI2CLiquidCrystal.h"

/// <summary>
/// Constructor
/// </summary>
FancyI2CLiquidCrystal::FancyI2CLiquidCrystal(int columns, int rows) : lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE), columns(columns), rows(rows)
{
	//
}

/// <summary>
/// Begin LCD
/// </summary>
void FancyI2CLiquidCrystal::Begin()
{
	lcd.begin(columns, rows);
}

/// <summary>
/// Print line
/// </summary>
void FancyI2CLiquidCrystal::PrintLine(int index, const char *str)
{
	unsigned int str_len(strlen(str));
	lcd.setCursor(0, index);
	lcd.print(str);
	for (int i(str_len); i < columns; i++)
	{
		lcd.print(" ");
	}
	lcd.setCursor(str_len, index);
}

/// <summary>
/// Set cursor
/// </summary>
void FancyI2CLiquidCrystal::SetCursor(int column, int row)
{
	lcd.setCursor(column, row);
}

/// <summary>
/// Backlight on
/// </summary>
void FancyI2CLiquidCrystal::Backlight()
{
	lcd.backlight();
}

/// <summary>
/// Backlight off
/// </summary>
void FancyI2CLiquidCrystal::NoBacklight()
{
	lcd.noBacklight();
}
