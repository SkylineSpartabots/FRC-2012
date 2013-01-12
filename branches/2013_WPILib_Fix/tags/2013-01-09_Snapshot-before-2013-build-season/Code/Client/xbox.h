#ifndef _XBOX_CONTROLLER_H
#define _XBOX_CONTROLLER_H

#include "WPILib.h"
#include "../Definitions/components.h"

class XboxController : public Joystick
{
public:
	enum Axis {
		LeftX = 1,
		LeftY = 2,
		Bumper = 3,
		RightX = 4,
		RightY = 5
	};
	enum Dpad {
		XDir = 6,
		YDir = 7
	};
	enum Button {
		A = 1,
		B = 2,
		X = 3,
		Y = 4,
		LeftBumper = 5,
		RightBumper = 6,
		Back = 7,
		Start = 8,
		LeftClick = 9,
		RightClick = 10
	};
	
	static const UINT32 numAxisTypes = 7;
	static const UINT32 numButtonTypes = 10;
	
	XboxController(UINT32);
	
	float GetAxis(Axis);
	int GetDpad(Dpad);
	bool GetButton(Button);
};

class XboxTest : public BaseController
{
public:
	XboxTest(XboxController *);
	void Run();
private:
	XboxController *mXboxController;
};

#endif
