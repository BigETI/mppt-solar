#ifndef __MEASUREMENTS_H__
#	define __MEASUREMENTS_H__

#	define MAX_MEASUREMENTS 5

/// <summary>
/// Measurements class
/// </summary>
class Measurements
{
private:
	/// <summary>
	/// Measurements
	/// </summary>
	double measurements[MAX_MEASUREMENTS];

	/// <summary>
	/// Measurement count
	/// </summary>
	unsigned int measurementCount;

	/// <summary>
	/// Last measurement
	/// </summary>
	double lastMeasurement;

	Measurements(const Measurements &);

	Measurements & operator = (const Measurements &);
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Measurements();
	
	/// <summary>
	/// Add measurement
	/// </summary>
	double AddMeasurement(double input);

	/// <summary>
	/// Get last measurement
	/// </summary>
	double GetLastMeasurement();
};

#endif
