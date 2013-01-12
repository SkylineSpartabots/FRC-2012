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
 * @brief A base class to lower the arm using limit switches.
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
	virtual void Set(float) = 0;
};

/**
 * @brief A simple arm object that does not use limit switches.
 * 
 * @details Later, replace all isntances of SimpleArm with GuardedArm
 * once all limit switches are installed.
 * @todo Depreciate this class
 * @warning Misuse of this class could result in a damaged robot.
 */
class SimpleArm : public BaseArmComponent
{
public:
	SimpleArm(SpeedController *);
	void GoUp();
	void GoDown();
	void Stop();
	void Set(float);
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
	void Set(float);
};




class SingleGuardedArm : public BaseArmComponent
{
public:
	static const float kUpMotorSpeed = 3.0;
	static const float kDownMotorSpeed = -1.0;
protected:
	DigitalInput *mLimit;
	SingleMotorLimitWatchdog *mSingleMotorWatchdog;
public:
	SingleGuardedArm (SpeedController *, DigitalInput *);
	~SingleGuardedArm ();
	
	void GoUp();
	void GoDown();
	void Stop();
	void Set(float);
};








/**
 * @brief Interacts with joystick to control the arm.
 */
class ArmController : public BaseController
{
protected:
	BaseArmComponent *mArm;
	Joystick *mJoystick;
	float mRawPower;
public:
	ArmController (BaseArmComponent *, Joystick *);
	void Run(void);
};

#endif
