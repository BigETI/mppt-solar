#ifndef __ADAFRUIT_FIXED_SENSOR_MEASURER_H__
#	define __ADAFRUIT_FIXED_SENSOR_MEASURER_H__

#	include <Adafruit_INA219.h>
#	include "Measurements.h"

/// <summary>
/// Adafruit fixed sensor measurer class
/// </summary>
class AdafruitFixedSensorMeasurer
{
private:
	/// <summary>
	/// Measurer
	/// </summary>
	Adafruit_INA219 measurer;

	/// <summary>
	/// Bus voltage multiplier
	/// </summary>
	double busVoltageMultiplier;

	/// <summary>
	/// Bus voltage offset
	/// </summary>
	double busVoltageOffset;

	/// <summary>
	/// CUrrent multiplier
	/// </summary>
	double currentMultiplier;

	/// <summary>
	/// Current offset
	/// </summary>
	double currentOffset;

	/// <summary>
	/// VOltage measurements
	/// </summary>
	Measurements voltageMeasurements;

	/// <summary>
	/// Current measurements
	/// </summary>
	Measurements currentMeasurements;

	/// <summary>
	/// Measurement current count
	/// </summary>
	unsigned int measurementCurrentCount;

	AdafruitFixedSensorMeasurer();
	AdafruitFixedSensorMeasurer(const AdafruitFixedSensorMeasurer &);

	AdafruitFixedSensorMeasurer & operator = (const AdafruitFixedSensorMeasurer &);
public:
	/// <summary>
	/// Constructor
	/// </summary>
	AdafruitFixedSensorMeasurer(int mode, double busVoltageMultiplier, double busVoltageOffset, double currentMultiplier, double currentOffset);

	/// <summary>
	/// Begin measurer
	/// </summary>
	void Begin();

	/// <summary>
	/// Measure voltage
	/// </summary>
	double MeasureVoltage();

	/// <summary>
	/// Measure current
	/// </summary>
	double MeasureCurrent();

	/// <summary>
	/// Get last measured voltage
	/// </summary>
	double GetLastVoltage();

	/// <summary>
	/// Get last measured current
	/// </summary>
	double GetLastCurrent();
};

#endif
