#include "arm.h"



/**
 * @brief Creates an instance of this class.
 */
BaseArmComponent::BaseArmComponent(SpeedController *speedController) :
		BaseComponent()
{
	mSpeedController = speedController;
}

BaseArmComponent::~BaseArmComponent()
{
	//Empty
}

/**
 * @brief Constructor for Arm class.
 * 
 * @param[in] armMotor A pointer to the motor that controls the arm.
 * @param[in] topLimit A pointer to the top limit switch.
 * @param[in] bottomLimit A pointer to the bottom limit switch.
 */
GuardedArm::GuardedArm (
		SpeedController *speedController,
		DigitalInput *topLimit,
		DigitalInput *bottomLimit) :
		BaseArmComponent(speedController)
{
	mTopLimit = topLimit;
	mBottomLimit = bottomLimit;
	
	mMotorWatchdog = new MotorLimitWatchdog(
			"Arm",
			mSpeedController,
			mTopLimit,
			mBottomLimit);
}

/**
 * @brief Deconstructor for the arm class.
 * 
 * @details
 * Used to kill the motor watchdog.
 */
GuardedArm::~GuardedArm ()
{
	if (mMotorWatchdog) {
		mMotorWatchdog->Stop();
		delete mMotorWatchdog;
	}
}

/**
 * @brief Makes the arm go up. 
 * 
 * @details
 * If the top limit switch is pressed, then the arm stops going up.
 * In other words, when the arm is at maximum height, it stops automatically.
 */
void GuardedArm::GoUp()
{
	if (mTopLimit->Get()) {
		mSpeedController->Set(0);
	} else {
		mSpeedController->Set(kMotorSpeed * kMotorDirection);
	}
}

/**
 * @brief Makes the arm go down.
 * 
 * @details
 * If the bottom limit switch is pressed, then the arms stop going down.
 * In other words, when the arm is at minimum height, it stops automatically.
 */
void GuardedArm::GoDown()
{
	if (mBottomLimit->Get()) {
		mSpeedController->Set(0);
	} else {
		mSpeedController->Set(kMotorSpeed * kMotorDirection * -1);
	}
}

/**
 * @brief Makes the arm stop.
 */
void GuardedArm::Stop() {
	mSpeedController->Set(0);
}




SimpleArm::SimpleArm(SpeedController *speedController) :
		BaseArmComponent(speedController)
{
	//Empty
}

void SimpleArm::GoUp()
{
	mSpeedController->Set(kMotorSpeed * kMotorDirection);
}

void SimpleArm::GoDown()
{
	mSpeedController->Set(kMotorSpeed * kMotorDirection * -1);
}

void SimpleArm::Stop()
{
	mSpeedController->Set(0);
}



/**
 * @brief Creates an instance of this class.
 * 
 * @param[in] arm Pointer to the arm.
 * @param[in] joystick Pointer to the joystick.
 */
ArmController::ArmController (BaseArmComponent *arm, Joystick *joystick) {
	mArm = arm;
	mJoystick = joystick;
}

/**
 * @brief Provides a thin layer to control the arm using a 
 * joystick.  
 * 
 * @details
 * While button 4 is pushed, the arm goes up. 
 * While button 5 is pushed, the arm goes down.
 * While neither button is pushed, the arm stops.
 */
void ArmController::Run() {
	bool armUp = mJoystick->GetRawButton(6);
	bool armDown = mJoystick->GetRawButton(7);
	if ( armUp ) {
		mArm->GoUp();
	} else if ( armDown ) {
		mArm->GoDown();
	} else {
		mArm->Stop();
	}
}
