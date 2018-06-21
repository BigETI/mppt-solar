#include "Arduino.h"
#include "AdafruitFixedSensorMeasurer.h"

/// <summary>
/// Constructor
/// </summary>
AdafruitFixedSensorMeasurer::AdafruitFixedSensorMeasurer(int mode, double busVoltageMultiplier, double busVoltageOffset, double currentMultiplier, double currentOffset) : measurer(mode), busVoltageMultiplier(busVoltageMultiplier), busVoltageOffset(busVoltageOffset), currentMultiplier(currentMultiplier), currentOffset(currentOffset)
{
	//
}

/// <summary>
/// Begin measurer
/// </summary>
void AdafruitFixedSensorMeasurer::Begin()
{
	measurer.begin();
}

/// <summary>
/// Measure voltage
/// </summary>
double AdafruitFixedSensorMeasurer::MeasureVoltage()
{
	return voltageMeasurements.AddMeasurement(busVoltageMultiplier * (measurer.getBusVoltage_V() + busVoltageOffset));
}

/// <summary>
/// Measure current
/// </summary>
double AdafruitFixedSensorMeasurer::MeasureCurrent()
{
	return currentMeasurements.AddMeasurement(currentMultiplier * (measurer.getCurrent_mA() + currentOffset));
}

/// <summary>
/// Get last measured voltage
/// </summary>
double AdafruitFixedSensorMeasurer::GetLastVoltage()
{
	return voltageMeasurements.GetLastMeasurement();
}

/// <summary>
/// Get last measured current
/// </summary>
double AdafruitFixedSensorMeasurer::GetLastCurrent()
{
	return currentMeasurements.GetLastMeasurement();
}
