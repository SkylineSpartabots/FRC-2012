/**
 * @file arm.h
 * 
 * @brief Provides code to control the arm.
 */

#ifndef ARM_H_
#define ARM_H_

#include "WPILib.h"
#include "motorLimitWatchdog.h"
#include "components.h"


/**
 * @brief A base class to lower the arm.
 */
class BaseArmComponent : public BaseComponent
{
protected:
	static const float kMotorSpeed = 0.3;
	static const float kMotorDirection = 1.0;
	SpeedController *mSpeedController;
	
public:
	BaseArmComponent(SpeedController *);
	virtual ~BaseArmComponent();
	virtual void GoUp() = 0;
	virtual void GoDown() = 0;
	virtual void Stop() = 0;
};

class SimpleArm : public BaseArmComponent
{
public:
	SimpleArm(SpeedController *);
	void GoUp();
	void GoDown();
	void Stop();
};

/**
 * @brief Provides code to control the arm.
 * 
 * @details
 * The arm is used to lower the bridge.
 */
class GuardedArm : public BaseArmComponent
{
public:
	static const float kMotorDirection = 1.0;
	static const float kMotorSpeed = 0.2;
protected:
	DigitalInput *mTopLimit;
	DigitalInput *mBottomLimit;
	MotorLimitWatchdog *mMotorWatchdog;
public:
	GuardedArm (SpeedController *, DigitalInput *, DigitalInput *);
	~GuardedArm ();
	
	void GoUp(void);
	void GoDown(void);
	void Stop(void);
};

/**
 * @brief Interacts with joystick to control the arm.
 */
class ArmController : public BaseController
{
protected:
	BaseArmComponent *mArm;
	Joystick *mJoystick;
public:
	ArmController (BaseArmComponent *, Joystick *);
	void Run(void);
};

#endif
