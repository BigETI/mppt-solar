#ifndef __BUTTON_H__
#	define __BUTTON_H__

/// <summary>
/// Button class
/// </summary>
class Button
{
private:
	/// <summary>
	/// Pin
	/// </summary>
	int pin;

	/// <summary>
	/// Is pressed
	/// </summary>
	bool isPressed;

	/// <summary>
	/// Button pressed event
	/// </summary>
	void(*buttonPressedEvent)();

	/// <summary>
	/// Button released event
	/// </summary>
	void(*buttonReleasedEvent)();
public:
	/// <summary>
	/// Constructor
	/// </summary>
	Button(int pin, void(*buttonPressedEvent)(), void(*buttonReleasedEvent)());

	/// <summary>
	/// Update
	/// </summary>
	void Update();
};

#endif
