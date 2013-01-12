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
    void SetSpeedAutomatically();
    void SetTestSpeed(float);
    float CalculateSpeed(float);
	float CalculateDistance();
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
public:
	ShooterController(Shooter *, Joystick *);
	void Run(void);
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

#endif
