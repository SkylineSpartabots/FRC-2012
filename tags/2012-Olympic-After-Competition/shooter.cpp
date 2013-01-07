#include "shooter.h"

/**
 * @brief Creates an instance of this class.
 *
 * @param[in] topLeftSpeedController Pointer to the top left speed controller.
 * @param[in] topRightSpeedController Pointer to the top right speed controller.
 * @param[in] bottomLeftSpeedController Pointer to the bottom left speed controller.
 * @param[in] bottomRightSpeedController Pointer to the bottom right speed controller.
 * @param[in] rangeFinder Pointer to a RangeFinder instance.
 */
Shooter::Shooter(
		SpeedController *topLeftSpeedController, 
		SpeedController *topRightSpeedController,
		SpeedController *bottomLeftSpeedController,
		SpeedController *bottomRightSpeedController, 
		RangeFinder *rangeFinder) : 
		BaseComponent()
{
	mTopLeftSpeedController = topLeftSpeedController;
	mTopRightSpeedController = topRightSpeedController;
	mBottomLeftSpeedController = bottomLeftSpeedController;
	mBottomRightSpeedController = bottomRightSpeedController;
	mRangeFinder = rangeFinder;
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
float Shooter::CalculateDistance()
{
	float distance = mRangeFinder->FromWallInches();
	return distance;
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
void Shooter::SetSpeedManually(float speed)
{
	float slowSpeed = speed * kReductionFactor;
	
	mTopLeftSpeedController->Set(slowSpeed);
	mTopRightSpeedController->Set(-1 * slowSpeed);
	mBottomLeftSpeedController->Set(-1 * speed);
	mBottomRightSpeedController->Set(speed);
}

/**
 * @brief Makes the wheels spin at a raw speed given as the parameter.
 * 
 * @param[in] speed The speed of the motor (from -1.0 to 1.0).
 */
void Shooter::SetTestSpeed(float speed)
{
	mTopLeftSpeedController->Set(speed);
	mTopRightSpeedController->Set(-1 * speed);
	mBottomLeftSpeedController->Set(-1 * speed);
	mBottomRightSpeedController->Set(speed);
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
float Shooter::SetSpeedAutomatically()
{
	float distance = Shooter::CalculateDistance();
	float speed = Shooter::CalculateSpeed(distance);
	
	float coercedSpeed = Tools::Coerce(speed, kMinSpeed, kMaxSpeed, 0, 1);
	
	SmartDashboard::GetInstance()->Log(coercedSpeed, "(SHOOTER) Auto speed ");
	
	Shooter::SetSpeedManually(coercedSpeed);
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
float Shooter::CalculateSpeed(float distance) {
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
	
	float throttle = mJoystick->GetTwist();
	s->Log(throttle, "(SHOOTER) Speed Factor ");
	
	UpdatePresets();
	if (IsPressingPreset()) {
		float out = GetPreset();
		if (out >= 0) {
			mShooter->SetSpeedManually(out);
			s->Log(out, "(SHOOTER) Preset ");
		}
	} else if (mJoystick->GetRawButton(2)) {
		mShooter->SetSpeedManually(throttle);
		s->Log(throttle, "(SHOOTER) Manual ");
	} else if (mJoystick->GetTrigger()) {
		float calculatedSpeed = mShooter->SetSpeedAutomatically();
		s->Log(calculatedSpeed, "(SHOOTER) Calculated ");
	} else {
		mShooter->SetSpeedManually(0);
	}
}

void ShooterController::UpdatePresets(void) 
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	mPresetOne = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 1 <<"));
	mPresetTwo = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 2 <<"));
	mPresetThree = Tools::StringToFloat(s->GetString("(SHOOTER) Preset 3 <<"));
}

bool ShooterController::IsPressingPreset()
{
	return mJoystick->GetRawButton(7) or mJoystick->GetRawButton(9) or mJoystick->GetRawButton(11);
}

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

/**
 * @brief Constructor for ShooterControllerTest class.
 * 
 * @param[in] shooter Pointer to shooter.
 * @param[in] joystick Pointer to joystick.
 */
ShooterControllerTest::ShooterControllerTest(Shooter *shooter, Joystick *joystick) :
		BaseController()
{
	mShooter = shooter;
	mJoystick = joystick;
}

/**
 * @brief Contains code to test the shooter.
 * 
 * @details
 * Button 3 sets the speed of the shooter to 0.0.
 * Button 4 sets the speed of the shooter to -1.0.
 * Button 5 sets the speed of the sohoter to 1.0.
 * Button 6 increases the speed by 0.01.
 * Button 7 decreases the speed by 0.01.
 * Button 10 increases the speed by 0.1.
 * Button 11 decreases the speed by 0.1.
 */
void ShooterControllerTest::Run(void)
{
	bool upSmall = mJoystick->GetRawButton(11);
	bool downSmall = mJoystick->GetRawButton(10);
	bool upBig = mJoystick->GetRawButton(6);
	bool downBig = mJoystick->GetRawButton(7);
	bool setLow = mJoystick->GetRawButton(4);
	bool setMiddle = mJoystick->GetRawButton(3);
	bool setHigh = mJoystick->GetRawButton(5);
	
	float shooterSpeed = 0.0;
	
	if ( upSmall ) {
		shooterSpeed += .01;
	} else if ( downSmall ) {
		shooterSpeed -= .01;
	} else if ( upBig ) {
		shooterSpeed += .1;
	} else if ( downBig ) {
		shooterSpeed -= .1;
	} else if ( setLow ) {
		shooterSpeed = -1.0;
	} else if ( setMiddle ) {
		shooterSpeed = 0.0;
	} else if ( setHigh ) {
		shooterSpeed = 1.0;
	}
	
	if ( shooterSpeed > 1.0 ) {
		shooterSpeed = 1.0;
	} else if ( shooterSpeed < -1.0 ) {
		shooterSpeed = -1.0;
	}
	
	SmartDashboard::GetInstance()->Log(shooterSpeed, "(SHOOTER) Test speed ");
	
	mShooter->SetTestSpeed(shooterSpeed);
}
