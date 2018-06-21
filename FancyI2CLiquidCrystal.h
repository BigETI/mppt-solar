#ifndef __FANCY_I2C_LIQUID_CRYSTAL_H__
#	define __FANCY_I2C_LIQUID_CRYSTAL_H__

#	include <LiquidCrystal_I2C.h>

/// <summary>
/// Fancy I2C liquid crystal class
/// </summary>
class FancyI2CLiquidCrystal
{
private:
	/// <summary>
	/// LCD
	/// </summary>
	LiquidCrystal_I2C lcd;

	/// <summary>
	/// COlumns
	/// </summary>
	int columns;

	/// <summary>
	/// Rows
	/// </summary>
	int rows;

	/// <summary>
	/// Get string length
	/// </summary>
	unsigned int strlen(const char *str)
	{
		unsigned int ret(0U);
		for (const char *c(str); *c != '\0'; c++)
		{
			++ret;
		}
		return ret;
	}
public:
	/// <summary>
	/// Constructor
	/// </summary>
	FancyI2CLiquidCrystal(int columns, int rows);

	/// <summary>
	/// Begin LCD
	/// </summary>
	void Begin();

	/// <summary>
	/// Print line
	/// </summary>
	void PrintLine(int index, const char *str);

	/// <summary>
	/// Print
	/// </summary>
	template <class _T> void Print(_T val)
	{
		lcd.print(val);
	}

	/// <summary>
	/// Set cursor
	/// </summary>
	void SetCursor(int column, int row);

	/// <summary>
	/// Backlight on
	/// </summary>
	void Backlight();

	/// <summary>
	/// Backlight off
	/// </summary>
	void NoBacklight();
};

#endif
