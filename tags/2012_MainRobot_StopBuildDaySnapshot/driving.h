/**
 * @file driving.h
 * 
 * @brief Contains the code used to drive the robot
 * based on user input.
 * 
 * @details
 * Code for shooting the robot or manipulating the
 * elevator is kept seperately
 * 
 * Every class in here should have 'BaseController' as
 * their parent class, so they can be placed under
 * MyRobot::mComponentCollection.
 */

#ifndef DRIVING_H_
#define DRIVING_H_

// System libraries
#include <utility.h>
#include <math.h>

// 3rd-party libraries
#include "WPILib.h"

// Our code
#include "components.h"
#include "shooter.h"





/**
 * @brief A baseclass for any controller that uses 
 * a joystick.
 * 
 * @details
 * Implements some common functions.
 */
class BaseJoystickController : public BaseController
{
protected:
	static const float kSpeedFactorMin = 0.3;
	static const float kSpeedFactorMax = 1.0;
	float squareInput(float);
	float getSpeedFactor(Joystick *);
	
public:
	BaseJoystickController();
	void Run() = 0;
};

class ControllerSwitcher : public BaseController
{
protected:
	vector <BaseController*> mControllers;
	vector <Joystick*> mJoysticks;
	int mControllerSize;
	int mJoystickSize;
	int mCurrent;
	bool mIsHeld;
	
public:
	ControllerSwitcher(vector<BaseController*>, vector<Joystick*>);
	virtual ~ControllerSwitcher();
	void Run();
};



/**
 * @brief Tests a motor using a joystick.
 * 
 * @details
 * This lets us debug an arbitary motor using a 
 * joystick.
 */
class TestMotor : public BaseJoystickController
{
protected:
	Joystick *mJoystick;
	SpeedController *mSpeedController;
	
public:
	TestMotor(Joystick *, SpeedController *);
	void Run();
};

/**
 * @brief A driving system designed to keep the robot from
 * moving without the watchdogs complaining there's no
 * signal being sent to the drive system.
 */
class MinimalistDrive : public BaseController
{
protected:
	RobotDrive *mRobotDrive;
	
public:
	MinimalistDrive(RobotDrive *);
	void Run();
};


/**
 * @brief Takes two joysticks and drives the robot, tank-style.
 * 
 * @details
 * Todo: document what every button on the Joystick does.
 */
class TankJoysticks : public BaseJoystickController
{
protected:
	Joystick *mLeftJoystick;
	Joystick *mRightJoystick;
	RobotDrive *mRobotDrive;
	
	static const float kSpeedFactorMin = 0.3;
	static const float kSpeedFactorMax = 1.0;
	
public:
	TankJoysticks(RobotDrive *, Joystick *, Joystick *);
	void Run(void);

protected:
	float GetSpeedDecreaseFactor(void);
};

/**
 * @brief Takes a single joystick and drives the robot,
 * arcade style.
 * 
 * @details
 * Todo: document what every button on the Joystick
 * does.
 */
class SingleJoystick : public BaseJoystickController
{
protected:
	RobotDrive *mRobotDrive;
	Joystick *mJoystick;
		
public:
	SingleJoystick(RobotDrive *, Joystick *);
	float GetSpeedDecreaseFactor();
	void Run(void);
};

/**
 * @brief A base class for any controller using a Kinect.
 */
class BaseKinectController : public BaseController
{
protected:
	RobotDrive *mRobotDrive;
	Kinect *mKinect;
	
public:
	BaseKinectController(RobotDrive *, Kinect *);
	virtual void Run(void) = 0;
	
protected:
	bool IsPlayerReady(void);
	void HaltRobot(void);
};


/**
 * @brief Takes a Kinect and uses hand gestures to drive the robot.
 * 
 * @details
 * This particular KinectController uses the z-axis of the hands
 * to set the speeds of each side of the robot (tank-style)
 * 
 * Todo: Document specifically what hand gestures this uses.
 */
class KinectController : public BaseKinectController
{
protected:
	static const float kArmMinZ = 0;
	static const float kArmMaxZ = 0.38;
	static const float kShootThresholdY = 0.2;
	
public:
	KinectController(RobotDrive *, Kinect *);
	void Run();
	
protected:
	bool IsPlayerShooting();
	float GetLeftArmDistance();
	float GetRightArmDistance();
};


/**
 * @brief Uses a kinect to drive the robot -- this time, based on the
 * angle of the hands to the shoulders.
 */
class KinectAngleController : public BaseKinectController
{
public:
	KinectAngleController(RobotDrive *, KinectStick *, KinectStick *, Kinect *, Shooter *);
	void Run();
	
protected:
	KinectStick *mLeftKinectStick;
	KinectStick *mRightKinectStick;
	Shooter *mShooter;
	
	static const float kPushThreshold = 0.3;
	static const float kSpeedDecreaseFactor = 0.4;
	// bool isAutoShooting();
	//bool IsPlayerReady();
	bool IsManuallyShooting();
	bool IsAutomaticallyShooting();
	
};

#endif
