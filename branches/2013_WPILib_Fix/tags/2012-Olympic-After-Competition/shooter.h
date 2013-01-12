/**
 * @file shooter.h
 * 
 * Operates device that shoots basketballs by spinning wheels.
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

// System libraries
#include <cmath>

// 3rd party libraries
#include "WPILib.h"

// Our code
#include "sensors.h"
#include "components.h"
#include "target.h"

/**
 * @brief Controls the shooter.
 * 
 * @todo Investigate if the functionality of this
 * class needs to be split up.
 */
class Shooter : public BaseComponent
{
protected:
	SpeedController *mTopLeftSpeedController;
	SpeedController *mTopRightSpeedController;
	SpeedController *mBottomLeftSpeedController;
	SpeedController *mBottomRightSpeedController;
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
    Shooter(SpeedController*, SpeedController*, SpeedController*, SpeedController*, RangeFinder*);
    void SetSpeedManually(float);
    float SetSpeedAutomatically();
    void SetTestSpeed(float);
    float CalculateSpeed(float);
	float CalculateDistance();
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

/**
 * @brief Class for testing the shooter.
 */
class ShooterControllerTest : public BaseController
{
protected:
	Shooter *mShooter;
	Joystick *mJoystick;
public:
	ShooterControllerTest(Shooter *, Joystick *);
	void Run(void);
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
