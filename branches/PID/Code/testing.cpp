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


EncoderTestController::EncoderTestController(Encoder *encoder) :
		BaseController()
{
	mEncoder = encoder;
	printf("Testing Encoder");
	SmartDashboard::GetInstance()->Log("Encoder test initialized", "(ENCODER TEST) status:");
	SmartDashboard::GetInstance()->PutString("(ENCODER TEST) command <<", "start");
	//mPreviousCommand = "deactivate";
}

void EncoderTestController::Run()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	string command = s->GetString("(ENCODER TEST) command <<");
	
	if (command != mPreviousCommand) {
		if (command == "start") {
			mEncoder->Start();
			s->Log("Starting encoder", "(ENCODER TEST) status:");
		} else if (command == "stop") {
			mEncoder->Stop();
			s->Log("Stopping encoder", "(ENCODER TEST) status:");
		} else if (command == "reset") {
			mEncoder->Reset();
			s->Log("Resetting encoder distance to zero", "(ENCODER TEST) status:");
		} else {
			s->Log("Unknown command", "(ENCODER TEST) status:");
		}
		mPreviousCommand = command;
	}
	
	s->Log(mEncoder->Get(), "(ENCODER TEST) Get:");
	s->Log(mEncoder->GetRaw(), "(ENCODER TEST) GetRaw:");
	s->Log(mEncoder->GetPeriod(), "(ENCODER TEST) GetPeriod:");
	s->Log(mEncoder->GetStopped(), "(ENCODER TEST) GetStopped:");
	s->Log(mEncoder->GetDirection(), "(ENCODER TEST) GetDirection:");
	s->Log(mEncoder->GetDistance(), "(ENCODER TEST) GetDistance:");
	s->Log(mEncoder->GetRate(), "(ENCODER TEST) GetRate:");
	s->Log(mEncoder->PIDGet(), "(ENCODER TEST) PIDGet:");
}
