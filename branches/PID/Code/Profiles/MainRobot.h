/**
 * @file robot.h
 * 
 * @brief The main entry point of the robot.
 * 
 * @details FIRST has provided a macro that takes the name of 
 * this class and runs it once deployed on the cRIO (see
 * main_entry_point.cpp).
 * 
 * This class is obliged to provide two public methods --
 * MainRobot::Autonomous() and MainRobot::OperatorControl.
 * 
 * This class is also responsible for instantiating 
 * every possible class that will be used in this program.
 * The instantiated objects are then passed as pointers to
 * any classes that need them.
 */

#ifndef MAINROBOT_H_
#define MAINROBOT_H_

// System libraries
#include <vector>

// 3rd party libraries
#include "WPILib.h"

// Program modules
#include "../Subsystems/arm.h"
#include "../sensors.h"
#include "../Subsystems/driving.h"
#include "../Definitions/components.h"
#include "../Definitions/ports.h"
#include "../Subsystems/shooter.h"
#include "../Subsystems/elevator.h"
#include "../Client/xbox.h"
#include "../communication.h"

/**
 * @brief This class bundles together everything to ultimately
 * run and control the robot.
 * 
 * @details
 * When the robot starts, a single instance of this class is 
 * created by a macro (provided by WPILib).  This class
 * must provide several methods in order to be fully
 * functional:
 *   - Autonomous()
 *   - OperatorControl()
 * 
 * It may also provide additional methods provided by
 * WPILib and SimpleRobot.
 * 
 * This class is the only one that is allowed to
 * create instances of any WPILib classes, Components,
 * and Controllers (to prevent accidental memory leaks
 * with pointers).  However, any class is allowed to
 * create instances of normal classes.
 */
class MainRobot : public SimpleRobot
{
private:
	// Safety constants
	static const double kMotorWait = 0.01;		// In seconds
	static const double kWatchdogExpiration = 1;	// In seconds
	
protected:
	// Hardware
	RobotDrive *mRobotDrive;
	AnalogChannel *mUltrasoundSensor;	// For ultrasound
	
	SpeedController *mLeftFrontDrive;
	SpeedController *mLeftBackDrive;
	SpeedController *mRightFrontDrive;
	SpeedController *mRightBackDrive;	
	
	SpeedController *mTopLeftShooter;
	SpeedController *mTopRightShooter;
	SpeedController *mBottomLeftShooter;
	SpeedController *mBottomRightShooter;
	SpeedController *mArmSpeedController;
	
	SpeedController *mElevatorSpeedController;
	DigitalInput *mElevatorBottomLimitSwitch;
	DigitalInput *mElevatorTopLimitSwitch;
	DigitalInput *mTopLimit;
	DigitalInput *mBottomLimit;
	
	// Input devices
	Joystick *mLeftJoystick;
	Joystick *mRightJoystick;
	Joystick *mTwistJoystick;
	Kinect *mKinect;
	XboxController *mXboxController;
	
	// Components
	RangeFinder *mRangeFinder;
	Shooter *mShooter;
	Elevator *mElevator;
	KinectStick *mLeftKinectStick;
	KinectStick *mRightKinectStick;
	TargetFinder *mTargetFinder;
	BaseMotorArmComponent *mArm;
	
	// Controller -- see controller.h
	vector<BaseController*> mControllerCollection;

public:
	MainRobot();
	~MainRobot();
	void RobotInit();
	void Autonomous();
	void OperatorControl();
	//void Disabled();
	
protected:
	void InitializeHardware();
	void InitializeInputDevices();
	void InitializeComponents();
	void InitializeControllers();
};

#endif

