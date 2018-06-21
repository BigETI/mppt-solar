#include "Arduino.h"
#include "Measurements.h"

/// <summary>
/// Default constructor
/// </summary>
Measurements::Measurements() : measurementCount(0U), lastMeasurement(0.0)
{
	for (int i(0); i < MAX_MEASUREMENTS; i++)
	{
		measurements[i] = 0.0;
	}
}

/// <summary>
/// Add measurement
/// </summary>
double Measurements::AddMeasurement(double input)
{
	lastMeasurement = 0.0;
	if (measurementCount < MAX_MEASUREMENTS)
	{
		measurements[measurementCount] = input;
		++measurementCount;
	}
	else
	{
		for (int i(1); i < MAX_MEASUREMENTS; i++)
		{
			measurements[i - 1] = measurements[i];
		}
		measurements[MAX_MEASUREMENTS - 1] = input;
	}
	for (int i(0); i < measurementCount; i++)
	{
		lastMeasurement += measurements[i];
	}
	lastMeasurement /= static_cast<double>(measurementCount);
	return lastMeasurement;
}

/// <summary>
/// Get last measurement
/// </summary>
double Measurements::GetLastMeasurement()
{
	return lastMeasurement;
}
