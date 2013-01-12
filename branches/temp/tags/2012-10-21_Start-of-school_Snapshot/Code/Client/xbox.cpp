#include "xbox.h"

XboxController::XboxController(UINT32 port) : 
		Joystick(port, numAxisTypes, numButtonTypes)
{
	// Empty
}

float XboxController::GetAxis(Axis axisName)
{
	return GetRawAxis(axisName);
}

int XboxController::GetDpad(Dpad dpad)
{
	return (int) GetRawAxis(dpad);
}

bool XboxController::GetButton(Button buttonName)
{
	return GetRawButton(buttonName);
}


XboxTest::XboxTest(XboxController *xboxController) :
		BaseController()
{
	mXboxController = xboxController;
}

void XboxTest::Run()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->Log(mXboxController->GetAxis(mXboxController->LeftX), "Left stick X");
	s->Log(mXboxController->GetAxis(mXboxController->LeftY), "Left stick Y");
	s->Log(mXboxController->GetAxis(mXboxController->Bumper), "Bumper");
	s->Log(mXboxController->GetAxis(mXboxController->RightX), "RightX");
	s->Log(mXboxController->GetAxis(mXboxController->RightY), "RightY");
	
	s->Log(mXboxController->GetDpad(mXboxController->XDir), "X Dpad");
	s->Log(mXboxController->GetDpad(mXboxController->YDir), "Y Dpad");
	
	s->Log(mXboxController->GetButton(mXboxController->A), "Button A");
	s->Log(mXboxController->GetButton(mXboxController->X), "Button X");
	s->Log(mXboxController->GetButton(mXboxController->B), "Button B");
	s->Log(mXboxController->GetButton(mXboxController->Y), "Button Y");
	s->Log(mXboxController->GetButton(mXboxController->LeftBumper), "Left bumper");
	s->Log(mXboxController->GetButton(mXboxController->RightBumper), "Right bumper");
	s->Log(mXboxController->GetButton(mXboxController->Back), "Back");
	s->Log(mXboxController->GetButton(mXboxController->Start), "Start");
	s->Log(mXboxController->GetButton(mXboxController->LeftClick), "Left click");
	s->Log(mXboxController->GetButton(mXboxController->RightClick), "Right click");
}
