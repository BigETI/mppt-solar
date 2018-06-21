#include "Arduino.h"
#include "Button.h"

/// <summary>
/// Constructor
/// </summary>
Button::Button(int pin, void(*buttonPressedEvent)(), void(*buttonReleasedEvent)()) : pin(pin), isPressed(false), buttonPressedEvent(buttonPressedEvent), buttonReleasedEvent()
{
	//
}

/// <summary>
/// Update
/// </summary>
void Button::Update()
{
	if (digitalRead(pin))
	{
		if (!isPressed)
		{
			isPressed = true;
			if (buttonPressedEvent)
			{
				buttonPressedEvent();
			}
		}
	}
	else
	{
		if (isPressed)
		{
			isPressed = false;
			if (buttonReleasedEvent)
			{
				buttonReleasedEvent();
			}
		}
	}
}
