/**
 * @file robot.h
 * 
 * @brief The main entry point of the robot.
 * 
 * @details FIRST has provided a macro that takes the name of 
 * this class and runs it once deployed on the cRIO (see
 * the bottom of this file).
 * 
 * This class is obliged to provide two public methods --
 * MainRobot::Autonomous() and MainRobot::OperatorControl.
 * 
 * This class is also responsible for instantiating 
 * every possible class that will be used in this program.
 * The instantiated objects are then passed as pointers to
 * any classes that need them.
 * 
 * @author Michael Lee
 */

#ifndef MYROBOT_H_
#define MYROBOT_H_

// System libraries
#include <vector>

// 3rd party libraries
#include "WPILib.h"

// Program modules
#include "arm.h"
#include "sensors.h"
#include "driving.h"
#include "components.h"
#include "shooter.h"
#include "target.h"
#include "elevator.h"
#include "timertest.h"

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
	Gyro *mGyro;
	SpeedController *mElevatorSpeedController;
	SpeedController *mTopLeftShooter;
	SpeedController *mTopRightShooter;
	SpeedController *mBottomLeftShooter;
	SpeedController *mBottomRightShooter;
	SpeedController *mArmSpeedController;
	//SpeedController *mTestMotor;
	//DigitalInput *mTestDigitalInput;
	DigitalInput *mElevatorBottomLimitSwitch;
	//DigitalInput *mElevatorTopLimitSwitch;
	
	// Input devices
	Joystick *mLeftJoystick;
	Joystick *mRightJoystick;
	Joystick *mTwistJoystick;
	Kinect *mKinect;
	
	// Components
	RangeFinder *mRangeFinder;
	Shooter *mShooter;
	Elevator *mElevator;
	//TestThreadListener *mTestThreadListener;
	KinectStick *mLeftKinectStick;
	KinectStick *mRightKinectStick;
	//TargetFinder *mTargetFinder;
	BaseArmComponent *mArm;
	
	// Controller -- see controller.h
	vector<BaseController*> mControllerCollection;
	//TargetFinder mTargetFinder;
	//Task mTargetTask;

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

