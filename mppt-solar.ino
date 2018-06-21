#include <TimerOne.h>
#include "FancyI2CLiquidCrystal.h"
#include "AdafruitFixedSensorMeasurer.h"
#include "Button.h"

/// <summary>
/// Shutdown digital pin (D1)
/// </summary>
const int shutdownDigitalPin(1);

/// <summary>
/// Load disconnect digital pin (D2)
/// </summary>
const int loadDisconnectDigitalPin(2);

/// <summary>
/// Move cursor up digital pin (D3)
/// </summary>
const int moveCursorUpDigitalPin(3);

/// <summary>
/// Move cursor down digital pin (D4)
/// </summary>
const int moveCursorDownDigitalPin(4);

/// <summary>
/// PWM signal digital pin (D9)
/// </summary>
const int pwmSignalDigitalPin(9);

/// <summary>
/// Scenes count
/// </summary>
const int scenesCount(4);

/// <summary>
/// Maximal inactive loop count
/// </summary>
const int maxInactiveLoopCount(10800);

/// <summary>
/// Battery minimum voltage
/// </summary>
const double batteryMinVoltage(21.6);

/// <summary>
/// Battery maximal voltage
/// </summary>
const double batteryMaxVoltage(27.36);

/// <summary>
/// Load reconnect voltage
/// </summary>
const double loadReconnectVoltage(25.36);

/// <summary>
/// Duty cycle step
/// </summary>
int dutyCycleStep(1);

/// <summary>
/// Currently is positive
/// </summary>
bool currentIsPositive(false);

/// <summary>
/// Current duty cycle
/// </summary>
int currentDutyCycle(0x39A);

/// <summary>
/// Screen update loop count
/// </summary>
int screenUpdateLoopCount(0);

/// <summary>
/// Done nothing loop count
/// </summary>
int doneNothingLoopCount(0);

/// <summary>
/// Scenes
/// </summary>
void(*scenes[scenesCount])();

/// <summary>
/// Voltage input
/// </summary>
double voltageIn;

/// <summary>
/// Current input
/// </summary>
double currentIn;

/// <summary>
/// Voltage output
/// </summary>
double voltageOut;

/// <summary>
/// Current output
/// </summary>
double currentOut;

/// <summary>
/// Is inactive loop count
/// </summary>
int isInactiveLoopCount(0);

/// <summary>
/// Selected scene index
/// </summary>
int selectedSceneIndex(0);

/// <summary>
/// Is load disconnected
/// </summary>
bool isLoadDisconnected(false);

/// <summary>
/// LCD
/// </summary>
FancyI2CLiquidCrystal lcd(20, 4);

/// <summary>
/// Sensor input measurer
/// </summary>
AdafruitFixedSensorMeasurer sensorInputMeasurer(0x41, 2.499698795180723, 0.0, 0.0083375020843755, 0.4);

/// <summary>
/// Sensor output measurer
/// </summary>
AdafruitFixedSensorMeasurer sensorOutputMeasurer(0x42, 2.499698795180723, 0.0, 0.0083640013382402, 0.4);

/// <summary>
/// Up button
/// </summary>
Button upButton(moveCursorUpDigitalPin, []()
{
	scenes[selectedSceneIndex]();
}, []()
{
	if (isInactiveLoopCount >= maxInactiveLoopCount)
	{
		lcd.Backlight();
	}
	else
	{
		--selectedSceneIndex;
		if (selectedSceneIndex < 0)
		{
			selectedSceneIndex = scenesCount - 1;
		}
	}
	scenes[selectedSceneIndex]();
	isInactiveLoopCount = 0;
});

/// <summary>
/// Down button
/// </summary>
Button downButton(moveCursorDownDigitalPin, []()
{
	scenes[selectedSceneIndex]();
}, []()
{
	if (isInactiveLoopCount >= maxInactiveLoopCount)
	{
		lcd.Backlight();
	}
	else
	{
		++selectedSceneIndex;
		if (selectedSceneIndex >= scenesCount)
		{
			selectedSceneIndex = 0;
		}
	}
	scenes[selectedSceneIndex]();
	isInactiveLoopCount = 0;
});

/// <summary>
/// Clamp value
/// </summary>
template <class T> T Clamp(T val, T min, T max)
{
	return ((val < min) ? min : ((val > max) ? max : val));
}

/// <summary>
/// Optimize duty cycle
/// </summary>
void OptimizeDutyCycle(bool isPositive)
{
	if (currentIsPositive == isPositive)
	{
		dutyCycleStep = Clamp(dutyCycleStep + 1, 0, 100);
	}
	else
	{
		currentIsPositive = isPositive;
		dutyCycleStep = 1;
	}
}

/// <summary>
/// Get next MPPT duty cycle
/// </summary>
int GetNextMPPTDutyCycle(double voltage, double current, double lastVoltage, double lastCurrent, int dutyCycle)
{
	int ret(Clamp(dutyCycle, 0x0, 0x3FF));
	double delta_input_voltage(voltage - lastVoltage);
	double delta_input_power((voltage * current) - (lastVoltage * lastCurrent));
	if (delta_input_power <= -0.001)
	{
		if (delta_input_voltage < 0.0)
		{
			ret -= dutyCycleStep / 10;
			OptimizeDutyCycle(false);
		}
		else
		{
			ret += dutyCycleStep / 10;
			OptimizeDutyCycle(true);
		}
	}
	else if (delta_input_power >= 0.001)
	{
		if (delta_input_voltage < 0.0)
		{
			ret += dutyCycleStep / 10;
			OptimizeDutyCycle(true);
		}
		else
		{
			ret -= dutyCycleStep / 10;
			OptimizeDutyCycle(false);
		}
	}
	return Clamp(ret, 0x0, 0x3FF);
}

// Arduino setup
void setup()
{
	// 20 us <=> 50 kHz
	Timer1.initialize(20);
	pinMode(shutdownDigitalPin, OUTPUT);
	pinMode(loadDisconnectDigitalPin, OUTPUT);
	pinMode(moveCursorUpDigitalPin, INPUT);
	pinMode(moveCursorDownDigitalPin, INPUT);
	pinMode(pwmSignalDigitalPin, OUTPUT);
	digitalWrite(shutdownDigitalPin, HIGH);
	digitalWrite(loadDisconnectDigitalPin, HIGH);
	lcd.Begin();
	sensorInputMeasurer.Begin();
	sensorOutputMeasurer.Begin();
	scenes[0] = []()
	{
		lcd.PrintLine(0, "===== POWER IN =====");
		lcd.PrintLine(1, "Voltage: ");
		lcd.Print(voltageIn);
		lcd.Print(" V");
		lcd.PrintLine(2, "Current: ");
		lcd.Print(currentIn);
		lcd.Print(" A");
		lcd.PrintLine(3, "Wattage: ");
		lcd.Print(voltageIn * currentIn);
		lcd.Print(" W");
	};
	scenes[1] = []()
	{
		lcd.PrintLine(0, "==== BATTERY IN ====");
		lcd.PrintLine(1, "Voltage: ");
		lcd.Print(isLoadDisconnected ? "true" : "false");
		//lcd.Print(voltageOut);
		lcd.Print(" V");
		lcd.PrintLine(2, "Current: ");
		lcd.Print(currentOut);
		lcd.Print(" A");
		lcd.PrintLine(3, "Wattage: ");
		lcd.Print(voltageOut * currentOut);
		lcd.Print(" W");
	};
	scenes[2] = []()
	{
		lcd.PrintLine(0, "=== BATTERY STATS ==");
		lcd.PrintLine(1, ".__________________.");
		lcd.SetCursor(6, 1);
		lcd.Print(((Clamp(voltageOut, batteryMinVoltage, batteryMaxVoltage) - batteryMinVoltage) * 100.0) / (batteryMaxVoltage - batteryMinVoltage));
		lcd.Print("%");
		lcd.PrintLine(2, "|                  |");
		lcd.SetCursor(1, 2);
		for (int i(0), sz(((Clamp(voltageOut, batteryMinVoltage, batteryMaxVoltage) - batteryMinVoltage) * 18.0) / (batteryMaxVoltage - batteryMinVoltage)); i < sz; i++)
		{
			lcd.Print("=");
			//lcd.Print("░");
		}
		lcd.PrintLine(3, "'------------------'");
	};
	scenes[3] = []()
	{
		lcd.PrintLine(0, "==== EFFICIENCY ====");
		lcd.PrintLine(1, "");
		lcd.PrintLine(2, "      ");
		lcd.Print(Clamp(((voltageOut * currentOut) * 100.0) / (voltageIn * currentIn), 0.0, 100.0));
		lcd.Print("%");
		lcd.PrintLine(3, "");
	};
}

// Arduino loop
void loop()
{
	// MPPT
	double last_voltage(sensorInputMeasurer.GetLastVoltage());
	double last_current(sensorInputMeasurer.GetLastCurrent());
	voltageIn = sensorInputMeasurer.MeasureVoltage();
	currentIn = sensorInputMeasurer.MeasureCurrent();
	voltageOut = sensorOutputMeasurer.MeasureVoltage();
	currentOut = sensorOutputMeasurer.MeasureCurrent();
	double delta_power((voltageIn * currentIn) - (last_voltage * last_current));
	int last_duty_cycle(currentDutyCycle);
	currentDutyCycle = GetNextMPPTDutyCycle(voltageIn, currentIn, last_voltage, last_current, currentDutyCycle);
	if (currentDutyCycle == last_duty_cycle)
	{
		++doneNothingLoopCount;
	}
	else
	{
		doneNothingLoopCount = 0;
	}
	if (voltageOut > batteryMaxVoltage)
	{
		if (!isLoadDisconnected)
		{
			isLoadDisconnected = true;
			digitalWrite(loadDisconnectDigitalPin, LOW);
		}
	}
	if (voltageOut < loadReconnectVoltage)
	{
		if (isLoadDisconnected)
		{
			isLoadDisconnected = false;
			digitalWrite(loadDisconnectDigitalPin, HIGH);
		}
	}
	if (doneNothingLoopCount >= 160)
	{
		if ((doneNothingLoopCount % 5) == 0)
		{
			currentDutyCycle = Clamp(currentDutyCycle + 1, 0, 0x3FF);
		}
	}
	Timer1.pwm(pwmSignalDigitalPin, currentDutyCycle);
	upButton.Update();
	downButton.Update();
	++screenUpdateLoopCount;
	if ((screenUpdateLoopCount % 180) == 0)
	{
		screenUpdateLoopCount = 0;
		scenes[selectedSceneIndex]();
	}
	isInactiveLoopCount = Clamp(isInactiveLoopCount + 1, 0, maxInactiveLoopCount + 1);
	if (isInactiveLoopCount == maxInactiveLoopCount)
	{
		lcd.NoBacklight();
	}
}
