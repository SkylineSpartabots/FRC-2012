#include "shooter.h"

/**
 * @brief Creates an instance of this class.
 *
 * @param[in] topLeftSpeedController Pointer to the top left speed controller.
 * @param[in] topRightSpeedController Pointer to the top right speed controller.
 * @param[in] bottomLeftSpeedController Pointer to the bottom left speed controller.
 * @param[in] bottomRightSpeedController Pointer to the bottom right speed controller.
 */
Shooter::Shooter(
		SpeedController *topLeftSpeedController, 
		SpeedController *topRightSpeedController,
		SpeedController *bottomLeftSpeedController,
		SpeedController *bottomRightSpeedController) : 
		BaseComponent()
{
	mTopLeftSpeedController = topLeftSpeedController;
	mTopRightSpeedController = topRightSpeedController;
	mBottomLeftSpeedController = bottomLeftSpeedController;
	mBottomRightSpeedController = bottomRightSpeedController;
}

/**
 * @brief Makes the wheels spin at a certain speed, typically set during manual mode.
 * 
 * @details
 * The input should be in the range -1.0 to 1.0.
 * 
 * @todo
 * Investigate if the motor requires a positive or 
 * negative speed to spin in the correct direction to 
 * shoot.
 * 
 * @param[in] speed The speed of the motor (from -1.0 to 1.0).
 */
void Shooter::SetSpeed(float speed)
{
	float slowSpeed = speed * kReductionFactor;
	
	mTopLeftSpeedController->Set(slowSpeed);
	mTopRightSpeedController->Set(-1 * slowSpeed);
	mBottomLeftSpeedController->Set(-1 * speed);
	mBottomRightSpeedController->Set(speed);
}

/**
 * @brief Lets you calibrate exactly how much the top and bottom
 * wheels spin.
 * 
 * @details
 * The inputs should be the the range -1.0 to 1.0
 * 
 * @param[in] topSpeed The speed of the top pair of motors
 * @param[out] bottomSpeed The speed of the bottom pair of motors
 */
void Shooter::SetSpeed(float topSpeed, float bottomSpeed)
{
	mTopLeftSpeedController->Set(topSpeed);
	mTopRightSpeedController->Set(topSpeed * -1);
	mBottomLeftSpeedController->Set(bottomSpeed * -1);
	mBottomRightSpeedController->Set(bottomSpeed);
}



AutomaticShooterController::AutomaticShooterController(Shooter *shooter, Joystick *joystick, RangeFinder *rangeFinder) :
		BaseController()
{
	mShooter = shooter;
	mJoystick = joystick;
	mRangeFinder = rangeFinder;
}


void AutomaticShooterController::Run()
{
	if (mJoystick->GetTrigger()) {
		float calculatedSpeed = SetSpeedAutomatically();
		mShooter->SetSpeed(calculatedSpeed);
		SmartDashboard::GetInstance()->Log(calculatedSpeed, "(SHOOTER) Calculated ");
	}
}

/**
 * @brief Gets the position of the robot relative to the hoop and
 * attempts to aim and hit it.
 * 
 * @details
 * Takes distance between robot and hoop and
 * determines the necessary initial velocity for the ball. 
 * Converts the velocity to a valid value for the
 * speed controller and passes that value to 
 * Shooter::SetSpeedManually to set the wheels to that speed.
 * 
 * @todo
 * Investigate if this needs to be moved into another class.
 */
float AutomaticShooterController::SetSpeedAutomatically()
{
	float distance = CalculateDistance();
	float speed = CalculateSpeed(distance);
	
	float coercedSpeed = Tools::Coerce(speed, kMinSpeed, kMaxSpeed, 0, 1);
	
	SmartDashboard::GetInstance()->Log(coercedSpeed, "(SHOOTER) Auto speed ");
	
	return coercedSpeed;
}

/**
 * @brief Calculates the speed the motors need to turn based on 
 * the distance to the wall.
 * 
 * @details
 * Receives position of robot from Shooter::Shoot,
 * and performs calculations to find the speed at which the wheels
 * need to turn in order to shoot the ball the correct distance.
 * 
 * @param[in] distance Distance from shooter to hoop in inches.
 *
 * @returns Returns the speed the speedControllers need to
 * turn to fire the ball and hit the hoop (from -1.0 to 1.0).
 */
float AutomaticShooterController::CalculateSpeed(float distance) {
	float pi = 4 * atan(1);
	float height = kBasketHeight - kShooterHeight;
	float angle = ( kShooterAngle * 2 * pi ) / ( 360 ); // converts from degrees to radians
		
	// calculates how fast the ball needs to be (initialVelocity) as it leaves the shooter
	float gravity = kGravity;
	float initialVelocityNum = gravity * distance * distance;
	float initialVelocityDenom = 2 * ( height - ( distance * tan(angle) ) ) * ( cos(angle) ) * ( cos(angle) ); 
	float initialVelocity = sqrt ( initialVelocityNum / initialVelocityDenom );
		
	float speed = initialVelocity / kMaxInitialVelocity;
	// todo: experiment to find actual maximum initial velocity to which wheels can accelerate ball
	
	return speed;
}

/**
 * @brief Calculates the distance from the wall in inches.
 * 
 * @details
 * Uses both the rangefinder and camera to calculate distance
 * 
 * @todo Add code for the camera.
 * 
 * @todo Investigate if the logic in the method needs to be
 * moved elsewhere.
 */
float AutomaticShooterController::CalculateDistance()
{
	float distance = mRangeFinder->FromWallInches();
	return distance;
}

/**
 * @brief Constructor for ShooterController class.
 * 
 * @param[in] shooter Pointer to shooter.
 * @param[in] joystick Pointer to joystick.
 */
ShooterController::ShooterController(Shooter *shooter, Joystick *joystick) :
		BaseController()
{
	mShooter = shooter;
	mJoystick = joystick;
	mPresetOne = 0.24;
	mPresetTwo = 0.39;
	mPresetThree = 0.5;
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->PutString("(SHOOTER) Preset 1 <<", Tools::FloatToString(mPresetOne).c_str());
	s->PutString("(SHOOTER) Preset 2 <<", Tools::FloatToString(mPresetTwo).c_str());
	s->PutString("(SHOOTER) Preset 3 <<", Tools::FloatToString(mPresetThree).c_str());
	
	s->Log(0, "(SHOOTER) Manual ");
	s->Log(0, "(SHOOTER) Calculated ");
	s->Log(0, "(SHOOTER) Preset ");
}

/**
 * @brief Contains code to control the shooter manually and automatically.
 * 
 * @details
 * Uses a joystick for manual mode -- use button 2 to fire normally
 * and the trigger for the preset.
 */
void ShooterController::Run(void)
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	float raw_throttle = -mJoystick->GetTwist();
	float throttle = Tools::Coerce(
			raw_throttle,
			-1.0,
			1.0,
			0.0,
			1.0);
	s->Log(throttle, "(SHOOTER) Speed Factor ");
	
	UpdatePresets();
	if (IsPressingPreset()) {
		float out = GetPreset();
		if (out >= 0) {
			mShooter->SetSpeed(out);
			s->Log(out, "(SHOOTER) Preset ");
		}
	} else if (mJoystick->GetRawButton(2)) {
		mShooter->SetSpeed(throttle);
		s->Log(throttle, "(SHOOTER) Manual ");
	} else {
		mShooter->SetSpeed(0);
	}
}

/*
 * @brief Updates preset speeds for shooter buttons 
 * by taking user-entered input from SmartDashboard.
 */
void ShooterController::UpdatePresets(void) 
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	mPresetOne = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 1 <<"));
	mPresetTwo = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 2 <<"));
	mPresetThree = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 3 <<"));
}

/*
 * @brief Finds out whether the user is pressing a preset button.
 */
bool ShooterController::IsPressingPreset()
{
	return mJoystick->GetRawButton(7) or mJoystick->GetRawButton(9) or mJoystick->GetRawButton(11);
}

/*
 * @brief Finds out what button the user is pressing, 
 * and hence the preset value to which to set the shooter speed.
 */
float ShooterController::GetPreset()
{
	if (mJoystick->GetRawButton(11)) {
		return mPresetOne;
	} else if (mJoystick->GetRawButton(9)) {
		return mPresetTwo;
	} else if (mJoystick->GetRawButton(7)) {
		return mPresetThree;
	}
	return -0.1;
}


////////////////////

ShooterXboxController::ShooterXboxController(Shooter *shooter, Elevator *elevator, XboxController *xboxController) :
		BaseController()
{
	mShooter = shooter;
	mElevator = elevator;
	mXbox = xboxController;
	
	mPresetOne = 0.24;
	mPresetTwo = 0.39;
	mPresetThree = 0.5;
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->PutString("(XBOX SHOOTER) Preset 1 <<", Tools::FloatToString(mPresetOne).c_str());
	s->PutString("(XBOX SHOOTER) Preset 2 <<", Tools::FloatToString(mPresetTwo).c_str());
	s->PutString("(XBOX SHOOTER) Preset 3 <<", Tools::FloatToString(mPresetThree).c_str());
	
	s->Log(0, "(XBOX SHOOTER) Preset ");
}

void ShooterXboxController::Run(void)
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	UpdatePresets();
	if (IsPressingPreset()) {
		float out = GetPreset();
		if (out >= 0) {
			mElevator->MoveUp();
			mShooter->SetSpeed(out);
			s->Log(out, "(XBOX SHOOTER) Preset ");
		}
	} else if (mXbox->GetButton(mXbox->B)) {
		mElevator->MoveDown();
		mShooter->SetSpeed(0);
	} else {
		mElevator->Stop();
		mShooter->SetSpeed(0);
	}
}

void ShooterXboxController::UpdatePresets(void) 
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	mPresetOne = Tools::StringToFloat(s->GetString("(XBOX SHOOTER) Preset 1 <<"));
	mPresetTwo = Tools::StringToFloat(s->GetString("(XBOX SHOOTER) Preset 2 <<"));
	mPresetThree = Tools::StringToFloat(s->GetString("(XBOX SHOOTER) Preset 3 <<"));
}

bool ShooterXboxController::IsPressingPreset()
{
	return mXbox->GetButton(mXbox->A) or mXbox->GetButton(mXbox->X) or mXbox->GetButton(mXbox->Y);
}

float ShooterXboxController::GetPreset()
{
	if (mXbox->GetButton(mXbox->A)) {
		return mPresetOne;
	} else if (mXbox->GetButton(mXbox->X)) {
		return mPresetTwo;
	} else if (mXbox->GetButton(mXbox->Y)) {
		return mPresetThree;
	}
	return -0.1;
}





///////////////////////

/**
 * @brief Constructor for CalibratedShooter.
 * 
 * @param[in] shooter Pointer to the shooter
 * @param[in] joystick Pointer to a joystick
 */
CalibratedShooterController::CalibratedShooterController(Shooter *shooter, Joystick *joystick) :
		BaseController()
{
	mShooter = shooter;
	mJoystick = joystick;
	
	mTopSpeed = 0.5;
	mBottomSpeed = 0.5;
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	s->PutString("(SHOOTER) Top Speed <<", Tools::FloatToString(mTopSpeed).c_str());
	s->PutString("(SHOOTER) Bottom Speed <<", Tools::FloatToString(mBottomSpeed).c_str());
}

/**
 * @brief Updates the speeds of the top and bottom speeds based on the 
 * values found in the SmartDashboard.
 */
void CalibratedShooterController::UpdatePresets()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	mTopSpeed = Tools::StringToFloat(s->GetString("(SHOOTER) Top Speed <<"));
	mBottomSpeed = Tools::StringToFloat(s->GetString("(SHOOTER) Bottom Speed <<"));
}

/**
 * @brief Runs the code to actually power the shooter.
 * 
 * @details Use the trigger to run the shooter.
 */
void CalibratedShooterController::Run()
{
	UpdatePresets();
	
	if (mJoystick->GetTrigger()) {
		mShooter->SetSpeed(mTopSpeed, mBottomSpeed);
	} else {
		mShooter->SetSpeed(0);
	}
}
