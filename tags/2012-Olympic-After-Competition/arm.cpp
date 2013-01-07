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

void GuardedArm::Set(float value) {
	mSpeedController->Set(value);	// Thread should theoretically prevent arm from passing too far
}





SingleGuardedArm::SingleGuardedArm (
		SpeedController *speedController,
		DigitalInput *limit) :
		BaseArmComponent(speedController)
{
	mLimit = limit;
	
	mSingleMotorWatchdog = new SingleMotorLimitWatchdog(
			"SingleGuardedArm",
			mSpeedController,
			mLimit);
}

/**
 * @brief Deconstructor for the arm class.
 * 
 * @details
 * Used to kill the motor watchdog.
 */
SingleGuardedArm::~SingleGuardedArm ()
{
	if (mSingleMotorWatchdog) {
		mSingleMotorWatchdog->Stop();
		delete mSingleMotorWatchdog;
	}
}

/**
 * @brief Makes the arm go up. 
 * 
 * @details
 * If the top limit switch is pressed, then the arm stops going up.
 * In other words, when the arm is at maximum height, it stops automatically.
 */
void SingleGuardedArm::GoUp()
{
	mSpeedController->Set(kUpMotorSpeed);	
}

/**
 * @brief Makes the arm go down.
 * 
 * @details
 * If the bottom limit switch is pressed, then the arms stop going down.
 * In other words, when the arm is at minimum height, it stops automatically.
 */
void SingleGuardedArm::GoDown()
{
	if (mLimit->Get()) {
		mSpeedController->Set(0);
	} else {
		mSpeedController->Set(kDownMotorSpeed);
	}
}

/**
 * @brief Makes the arm stop.
 */
void SingleGuardedArm::Stop() {
	mSpeedController->Set(0);
}
//*
void SingleGuardedArm::Set(float value) {
	mSpeedController->Set(value);
}
//*/




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

void SimpleArm::Set(float value) 
{
	mSpeedController->Set(value);
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
	mRawPower = 0.0;
	SmartDashboard::GetInstance()->PutString("(ARM) Raw power <<", Tools::FloatToString(mRawPower));
}

/**
 * @brief Provides a thin layer to control the arm using a 
 * joystick.  
 */
void ArmController::Run() {
	bool armUp = mJoystick->GetRawButton(3);
	bool armDown = mJoystick->GetRawButton(4);
	if ( armUp ) {
		mArm->GoUp();
	} else if ( armDown ) {
		mArm->GoDown();
	} else {
		mArm->Stop();
	}
	mArm->Set(mJoystick->GetY());
	if (mJoystick->GetRawButton(5) and mJoystick->GetRawButton(6)) {
		mArm->Set(Tools::StringToFloat(SmartDashboard::GetInstance()->GetString("(ARM) Raw power <<")));
	}
}
