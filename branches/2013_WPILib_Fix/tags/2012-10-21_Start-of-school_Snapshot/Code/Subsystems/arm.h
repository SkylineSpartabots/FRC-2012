/**
 * @file arm.h
 * 
 * @brief Provides code to control the arm.
 */

#ifndef ARM_H_
#define ARM_H_

#include "WPILib.h"
#include "../Definitions/components.h"
#include "../tools.h"

class BaseArmComponent : public BaseComponent
{
public:
	BaseArmComponent();
	virtual ~BaseArmComponent();
	virtual void GoUp() = 0;
	virtual void GoDown() = 0;
	virtual void Stop() = 0;
};

/**
 * @brief A base class to lower the arm using limit switches.
 */
class BaseMotorArmComponent : public BaseArmComponent
{
protected:
	static const float kMotorSpeed = 0.3;
	static const float kMotorDirection = 1.0;
	SpeedController *mSpeedController;
	
public:
	BaseMotorArmComponent(SpeedController *);
	virtual ~BaseMotorArmComponent();
	virtual void Set(float) = 0;
	virtual void SafeSet(float) = 0;
};

/**
 * @brief A simple arm object that does not use limit switches.
 * 
 * @details Later, replace all isntances of SimpleArm with GuardedArm
 * once all limit switches are installed.
 * @todo Depreciate this class
 * @warning Misuse of this class could result in a damaged robot.
 */
class SimpleArm : public BaseMotorArmComponent
{
public:
	SimpleArm(SpeedController *);
	void GoUp();
	void GoDown();
	void Stop();
	void Set(float);
	void SafeSet(float);
};

/**
 * @brief Provides code to control the arm with two limit switches.
 * 
 * @details
 * The arm is used to lower the bridge.
 */
class GuardedArm : public BaseMotorArmComponent
{
public:
	static const float kMotorDirection = 1.0;
	static const float kMotorSpeed = 0.2;
protected:
	DigitalInput *mTopLimit;
	DigitalInput *mBottomLimit;
	
public:
	GuardedArm (SpeedController *, DigitalInput *, DigitalInput *);
	~GuardedArm ();
	
	void GoUp(void);
	void GoDown(void);
	void Stop(void);
	void Set(float);
	void SafeSet(float);
};

/*
 * @brief Provides code to control the arm with one limit switch.
 */
class SingleGuardedArm : public BaseMotorArmComponent
{
public:
	static const float kUpMotorSpeed = 1;
	static const float kDownMotorSpeed = -1;
protected:
	DigitalInput *mLimit;
	//SingleMotorLimitWatchdog *mSingleMotorWatchdog;
public:
	SingleGuardedArm (SpeedController *, DigitalInput *);
	~SingleGuardedArm ();
	
	void GoUp();
	void GoDown();
	void Stop();
	void Set(float);
	void SafeSet(float);
};

class PneumaticTestArm: public BaseArmComponent
{
private:
	Compressor* mCompressor;
	Relay* mSpike;
	Solenoid* mSolenoid1;
	Solenoid* mSolenoid2;
	
public:
	PneumaticTestArm(Compressor *, Relay *, Solenoid *, Solenoid*);
	~PneumaticTestArm();
	
	void GoUp();
	void GoDown();
	void Stop();
};

/**
 * @brief Creates a pneumatics-based arm controller.
 */
class PneumaticArm : public BaseArmComponent
{
private:
	Compressor* mCompressor;
	Solenoid* mSolenoid1;
	Solenoid* mSolenoid2;
	
public:
	PneumaticArm(Compressor *, Solenoid *, Solenoid *);
	~PneumaticArm();
	
	void GoUp();
	void GoDown();
	void Stop();
};

class ArmController : public BaseController
{
protected:
	BaseArmComponent *mArm;
	Joystick *mJoystick;
	
public:
	ArmController(BaseArmComponent *, Joystick *);
	void Run(void);
};

/**
 * @brief Interacts with joystick to control a motor-based arm.
 */
class MotorArmController : public BaseController
{
protected:
	BaseMotorArmComponent *mArm;
	Joystick *mJoystick;
	float mRawPower;
public:
	MotorArmController(BaseMotorArmComponent *, Joystick *);
	void Run(void);
};

#endif
