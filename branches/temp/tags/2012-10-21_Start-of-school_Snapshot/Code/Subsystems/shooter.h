/**
 * @file shooter.h
 * 
 * @brief Operates device that shoots basketballs
 * 
 * @warning The structure of this class is somewhat 
 * convoluted and intertwined.  The way the code is 
 * arranged here is not optimal, and should not be
 * emulated.  However, we're not fixing or restructuring
 * it because it works, and it would require too much 
 * testing to fix it.
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

// System libraries
#include <cmath>

// 3rd party libraries
#include "WPILib.h"

// Our code
#include "../sensors.h"
#include "../Definitions/components.h"
#include "../Tracking/target.h"
#include "../Client/xbox.h"
#include "elevator.h"
#include "../tools.h"

/**
 * @brief Controls the shooter.
 * 
 * @warning This class, structurally speaking, is too complex.
 * In addition, anything related to automatically shooting is 
 * either untested or buggy.
 */
class Shooter : public BaseComponent
{
protected:
	SpeedController *mTopLeftSpeedController;
	SpeedController *mTopRightSpeedController;
	SpeedController *mBottomLeftSpeedController;
	SpeedController *mBottomRightSpeedController;
	
	static const float kReductionFactor = 0.9;

public:
    Shooter(SpeedController*, SpeedController*, SpeedController*, SpeedController*);
    void SetSpeed(float);
    void SetSpeed(float, float);
};


/**
 * @brief A shooter controller which attempts to control the shooter based on input and distance.
 */
class AutomaticShooterController : public BaseController
{
protected:
	Shooter *mShooter;
	Joystick *mJoystick;
	RangeFinder *mRangeFinder;
	static const float kShooterAngle = 45;
	static const float kShooterHeight = 50;
	static const float kBasketHeight = 98;
	static const float kGravity = 386.4; // inches per second per second
	static const float kMaxSpeed = 28.373;
	static const float kMinSpeed = 22.698; // assuming that the minimum distance is 5 feet, and the maximum distance is 12 feet
	static const float kReductionFactor = 0.9;
	static const float kMaxInitialVelocity = 336;

public:
    AutomaticShooterController(Shooter*, Joystick*, RangeFinder*);
    void Run();
    float SetSpeedAutomatically();
    float CalculateSpeed(float);
	float CalculateDistance();
};

/**
 * @brief Allows you to adjust and test the precise speeds of the
 * shooter via SmartDashboard.
 * 
 * @details
 * This lets you precisely calibrate the speed of the motors via the 
 * SmartDashboard, then shoot it by using a joystick.
 */
class CalibratedShooterController : public BaseController
{
protected:
	Shooter *mShooter;
	Joystick *mJoystick;
	float mTopSpeed;
	float mBottomSpeed;
	
	void UpdatePresets();
	
public:
	CalibratedShooterController(Shooter *, Joystick *);
	void Run();
};

/**
 * @brief Controls the shooter using a joystick.
 */
class ShooterController : public BaseController
{
protected:
	Shooter *mShooter;
	Joystick *mJoystick;
	float mPresetOne;
	float mPresetTwo;
	float mPresetThree;
	void UpdatePresets();
	bool IsPressingPreset();
	float GetPreset();
	
public:
	ShooterController(Shooter *, Joystick *);
	void Run();
};

class ShooterXboxController : public BaseController
{
protected:
	Shooter *mShooter;
	XboxController *mXbox;
	Elevator *mElevator;
	float mPresetOne;
	float mPresetTwo;
	float mPresetThree;
	void UpdatePresets();
	bool IsPressingPreset();
	float GetPreset();
	
public:
	ShooterXboxController(Shooter *, Elevator *, XboxController *);
	void Run();
};



/**
 * @brief Adjusts the robot to face the target more accurately.
 * 
 * @details
 * This will take command over the drive system temporarily, and
 * will disable or feed the watchdog during the duration.
 * 
 * @warning
 * Using this class will pause the robot for approximately 0.5 
 * seconds to obtain the image, then a few more seconds to
 * move the robot into the correct position.
 */
class ShooterAdjuster : public BaseComponent
{
protected:
	Shooter *mShooter;
	Gyro *mGyro;
	TargetFinder *mTargetFinder;
	Watchdog &mWatchdog;
	static const float kRotationSpeed = 0.4;
	
	TargetUtils::Target ObtainHighestTarget();
	
	
public:
	ShooterAdjuster(Shooter *, Gyro *, TargetFinder *, Watchdog &);
	
};

#endif
