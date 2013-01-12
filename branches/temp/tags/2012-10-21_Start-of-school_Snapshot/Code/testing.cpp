#include "testing.h"

/**
 * @brief Initializes the class, and prepares the 
 * SmartDashboard.
 */
ServoController::ServoController(Servo *servo) :
	BaseController()
{
	mServo = servo;
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->PutString("(SERVO) Value <<", "0.0");
}

/**
 * @brief Runs the core code.  Grabs values from the 
 * SmartDashboard to change the servo values.  
 */
void ServoController::Run()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	float value = Tools::StringToFloat(s->GetString("(SERVO) Value <<"));
	if (value > 1.0) {
		value = 1.0;
	} else if (value < -1.0) {
		value = -1.0;
	}
	SmartDashboard::GetInstance()->Log(value, "(SERVO) amount fed:");
	mServo->Set(value);
	SmartDashboard::GetInstance()->Log(mServo->Get(), "(SERVO) amount reported:");
}
