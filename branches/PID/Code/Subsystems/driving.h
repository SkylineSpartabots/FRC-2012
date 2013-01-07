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

// 3rd-party libraries
#include "WPILib.h"

// Our code
#include "../Definitions/components.h"
#include "shooter.h"
#include "arm.h"
#include "../Client/xbox.h"
#include "filters.h"




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
	
	float GetSpeedFactor(Joystick *);
	bool mAreValuesSwapped;

	/**
	 * @brief A datastructure holding the speeds of the 
	 * left and right wheels.
	 * 
	 * @details
	 * This struct was constructed mostly for organizational
	 * processes, to facilitate passing around code.
	 */
	
	float Truncate(float);
public:
	BaseJoystickController();
	void Stop(RobotDrive *);
	void Run() = 0;
};

class ArcadeJoystick : public BaseJoystickController
{
protected:
	RobotDrive *mRobotDrive;
	Joystick *mJoystick;

	DriveSpeed GetLeftAndRight(float, float);
	
public:
	ArcadeJoystick(RobotDrive*, Joystick*);
	void Run();
	
};


/**
 * @brief A container class to hold a series of
 * controllers to allow a user to switch between
 * them via the smart dashboard.
 */
class ControllerSwitcher : public BaseController
{
protected:
	vector <BaseController*> mControllers;
	int mControllerSize;
	const char *mLabel;
	
public:
	ControllerSwitcher(vector<BaseController*>, const char*);
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
	const char *mName;
	
public:
	TestMotor(Joystick *, SpeedController *, const char *);
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
};

/**
 * @brief Takes a single joystick and drives the robot,
 * arcade style.
 * 
 * @warning
 * Use TankJoysticks() -- this class is slowly starting to
 * become depreciated.
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
 * @brief A mode of controlling the robot (tank drive)
 * but specifically calibrated to be safer and for
 * demonstration purposes.
 */
class SafetyMode : public BaseJoystickController
{
protected:
	RobotDrive *mRobotDrive;
	Joystick *mLeftJoystick;
	Joystick *mRightJoystick;
	Joystick *mSafetyJoystick;

public:
	SafetyMode(RobotDrive *, Joystick *, Joystick *, Joystick *);
	void Run();
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
	KinectAngleController(RobotDrive *, KinectStick *, KinectStick *, Kinect *, Shooter *, BaseArmComponent *);
	void Run();
	
protected:
	KinectStick *mLeftKinectStick;
	KinectStick *mRightKinectStick;
	Shooter *mShooter;
	BaseArmComponent *mArm;
	
	static const float kPushThreshold = 0.3;
	static const float kSpeedDecreaseFactor = 0.4;
	static const float kArmThreshold = 0.3; // todo find actual value for threshold that works.
	// bool isAutoShooting();
	//bool IsPlayerReady();
	bool IsManuallyShooting();
	bool IsAutomaticallyShooting();
	bool IsRaisingArm();
	bool IsLoweringArm();
	
};

class XboxDrive : public BaseController
{
protected:
	RobotDrive *mRobotDrive;
	XboxController *mXboxController;
public:
	XboxDrive(RobotDrive *, XboxController *);
	void Run();	
};

class XboxDriveSingle : public BaseController
{
protected:
	RobotDrive *mRobotDrive;
	XboxController *mXboxController;
	bool isRight;
public:
	XboxDriveSingle(RobotDrive *, XboxController *);
	void Run();	
};

#endif
