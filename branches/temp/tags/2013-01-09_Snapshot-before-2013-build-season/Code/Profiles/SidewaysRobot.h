#ifndef SIDEWAYSROBOT_H_
#define SIDEWAYSROBOT_H_

// System libraries
#include <vector>

// 3rd party libraries
#include "WPILib.h"

// Program modules
#include "../Subsystems/driving.h"
#include "../Definitions/components.h"
#include "../Definitions/ports.h"
#include "../Client/xbox.h"

class SidewaysRobot : public SimpleRobot
{
private:
	// Safety constants
	static const double kMotorWait = 0.01;		// In seconds
	static const double kWatchdogExpiration = 1;	// In seconds
	
protected:
	// Hardware
	RobotDrive *mRobotDrive;
	
	SpeedController *mLeftFrontDrive;
	SpeedController *mLeftBackDrive;
	SpeedController *mRightFrontDrive;
	SpeedController *mRightBackDrive;	
	
	Encoder *mLeftEncoder;
	Encoder *mRightEncoder;
	
	// Input devices
	Joystick *mLeftJoystick;
	Joystick *mRightJoystick;
	Joystick *mTwistJoystick;
	XboxController *mXboxController;
	
	// Controller -- see controller.h
	vector<BaseController*> mControllerCollection;

public:
	SidewaysRobot();
	~SidewaysRobot();
	void RobotInit();
	void Autonomous();
	void OperatorControl();
	
protected:
	void InitializeHardware();
	void InitializeInputDevices();
	void InitializeComponents();
	void InitializeControllers();
};

#endif

