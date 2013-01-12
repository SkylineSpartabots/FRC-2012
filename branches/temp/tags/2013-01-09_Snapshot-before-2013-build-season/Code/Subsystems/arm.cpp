#include "arm.h"

/**
 * @brief Creates an instance of this class.
 */
BaseArmComponent::BaseArmComponent()
{
	// Empty
}

BaseArmComponent::~BaseArmComponent()
{
	// Empty
}

/**
 * @brief Creates an instance of this class.
 */
BaseMotorArmComponent::BaseMotorArmComponent(SpeedController *speedController) :
		BaseArmComponent()
{
	mSpeedController = speedController;
}

/*
 * @brief Deconstructor for this class.
 */
BaseMotorArmComponent::~BaseMotorArmComponent()
{
	//Empty
}

/**
 * @brief Constructor for GuardedArm (two limit switches) class.
 * 
 * @param[in] speedController A pointer to the motor that controls the arm.
 * @param[in] topLimit A pointer to the top limit switch.
 * @param[in] bottomLimit A pointer to the bottom limit switch.
 */
GuardedArm::GuardedArm (
		SpeedController *speedController,
		DigitalInput *topLimit,
		DigitalInput *bottomLimit) :
		BaseMotorArmComponent(speedController)
{
	mTopLimit = topLimit;
	mBottomLimit = bottomLimit;
}

/**
 * @brief Deconstructor for this class.
 * 
 * @details
 * Used to kill the motor watchdog.
 */
GuardedArm::~GuardedArm ()
{
	/*
	if (mMotorWatchdog) {
		mMotorWatchdog->Stop();
		delete mMotorWatchdog;
	}
	*/
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

/**
 * @brief Sets the arm speed controller to a value.
 */
void GuardedArm::Set(float value) {
	mSpeedController->Set(value);	// Thread should theoretically prevent arm from passing too far
}

/**
 * @brief Safely sets the arm speed controller to a value.
 */
void GuardedArm::SafeSet(float value) {
	if (value < 0 && !mBottomLimit->Get()) {
		mSpeedController->Set(value);
	} else if (value > 0 && !mTopLimit->Get()) {
		mSpeedController->Set(value);
	} else {
		mSpeedController->Set(0);
	}
}

/*
 * @brief Constructor for SingleGuardedArm (one limit switch) class.
 * 
 * @param[in] speedController Pointer to arm speed controller.
 * @param[in] limit Pointer to limit switch.
 */
SingleGuardedArm::SingleGuardedArm (
		SpeedController *speedController,
		DigitalInput *limit) :
		BaseMotorArmComponent(speedController)
{
	mLimit = limit;
	
	/*mSingleMotorWatchdog = new SingleMotorLimitWatchdog(
			"SingleGuardedArm",
			mSpeedController,
			mLimit);*/
}

/**
 * @brief Deconstructor for the arm class.
 * 
 * @details
 * Used to kill the motor watchdog.
 */
SingleGuardedArm::~SingleGuardedArm ()
{/*
	if (mSingleMotorWatchdog) {
		mSingleMotorWatchdog->Stop();
		delete mSingleMotorWatchdog;
	}*/
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

/*
 * @brief Sets the arm speed controller to a value.
 */
void SingleGuardedArm::Set(float value) {
	mSpeedController->Set(value);
}

/**
 * @brief Safely sets the arm speed controller to a value.
 */
void SingleGuardedArm::SafeSet(float value)
{
	if (mSpeedController < 0 && mLimit->Get()) {
		mSpeedController->Set(0);
	} else {
		mSpeedController->Set(value);
	}
}

/*
 * @brief Constructor for SimpleArm (no limit switches) class.
 */
SimpleArm::SimpleArm(SpeedController *speedController) :
		BaseMotorArmComponent(speedController)
{
	//Empty
}

/*
 * @brief Makes the arm go up.
 */
void SimpleArm::GoUp()
{
	mSpeedController->Set(kMotorSpeed * kMotorDirection);
}

/*
 * @brief Makes the arm go down.
 */
void SimpleArm::GoDown()
{
	mSpeedController->Set(kMotorSpeed * kMotorDirection * -1);
}

/*
 * @brief Makes the arm stop, i.e. sets arm speed controller to 0.
 */
void SimpleArm::Stop()
{
	mSpeedController->Set(0);
}

/*
 * @brief Sets the arm speed controller to a value.
 */
void SimpleArm::Set(float value) 
{
	mSpeedController->Set(value);
}

/**
 * @brief Sets the arm speed controller to a value (identical to SimpleArm::Set)
 */
void SimpleArm::SafeSet(float value) {
	Set(value);
}


/////////////////////////

PneumaticTestArm::PneumaticTestArm(Compressor *compressor, Relay *spike, Solenoid *solenoid1, Solenoid *solenoid2):
		BaseArmComponent()
{
	mCompressor = compressor;
	mSpike = spike;
	mSolenoid1 = solenoid1;
	mSolenoid2 = solenoid2;	

	mCompressor->SetRelayValue(Relay::kOn);
	mSolenoid1->Set(false);
	mSolenoid2->Set(true);
}

PneumaticTestArm::~PneumaticTestArm()
{
	// Nothing
}

/**
 * @brief Moves the arm upwards.
 */
void PneumaticTestArm::GoUp()
{
	mSolenoid1->Set(false);
	mSolenoid2->Set(true);
}

/**
 * @brief Moves the arm downwards.
 */
void PneumaticTestArm::GoDown()
{
	mSolenoid1->Set(true);
	mSolenoid2->Set(false);
}

/**
 * @brief Does nothing; not implemented.
 */
void PneumaticTestArm::Stop()
{
	// Nothing
}


/////////////////////////

/**
 * @brief Creates an instance of the PneumaticArm and starts the compressor.
 */
PneumaticArm::PneumaticArm(Compressor *compressor, Solenoid *solenoid1, Solenoid *solenoid2) :
		BaseArmComponent()
{
	mCompressor = compressor;
	mSolenoid1 = solenoid1;
	mSolenoid2 = solenoid2;
	
	// Upon starting, the compressor will automatically start and stop itself
	// based on the pressure of the tank.
	mCompressor->Start();
	mSolenoid1->Set(false);
	mSolenoid2->Set(true);
}

/**
 * @brief Destructor.
 */
PneumaticArm::~PneumaticArm()
{
	// Nothing
}

/**
 * @brief Moves the arm upwards.
 */
void PneumaticArm::GoUp()
{
	mSolenoid1->Set(false);
	mSolenoid2->Set(true);
}

/**
 * @brief Moves the arm downwards.
 */
void PneumaticArm::GoDown()
{
	mSolenoid1->Set(true);
	mSolenoid2->Set(false);
}

/**
 * @brief Does nothing; not implemented.
 */
void PneumaticArm::Stop()
{
	// Nothing
}


/**
 * @brief Creates an instance of this class.
 * 
 * @param[in] arm Pointer to the arm.
 * @param[in] joystick Pointer to the joystick.
 */
ArmController::ArmController(BaseArmComponent *arm, Joystick *joystick)
{
	mArm = arm;
	mJoystick = joystick;
}

/**
 * @brief Provides a thin layer to control the arm using a 
 * joystick.  
 */
void ArmController::Run() {
	bool armUp = mJoystick->GetRawButton(7);
	bool armDown = mJoystick->GetRawButton(6);
	if ( armUp ) {
		mArm->GoUp();
	} else if ( armDown ) {
		mArm->GoDown();
	} else {
		mArm->Stop();
	}
}


/**
 * @brief Creates an instance of this class.
 * 
 * @param[in] arm Pointer to the arm.
 * @param[in] joystick Pointer to the joystick.
 */
MotorArmController::MotorArmController (BaseMotorArmComponent *arm, Joystick *joystick) {
	mArm = arm;
	mJoystick = joystick;
	mRawPower = 0.0;
	SmartDashboard::GetInstance()->PutString("(ARM) Raw power <<", Tools::FloatToString(mRawPower));
}

/**
 * @brief Provides a thin layer to control the arm using a 
 * joystick.  
 */
void MotorArmController::Run() {
	bool armUp = mJoystick->GetRawButton(7);
	bool armDown = mJoystick->GetRawButton(6);
	if ( armUp ) {
		mArm->GoUp();
	} else if ( armDown ) {
		mArm->GoDown();
	} else {
		if (mJoystick->GetRawButton(10) and mJoystick->GetRawButton(11)) {
			float speed = Tools::StringToFloat(SmartDashboard::GetInstance()->GetString("(ARM) Raw power <<"));
			mArm->SafeSet(speed);
		} else {
			mArm->Set(0);
		}
	}
}
