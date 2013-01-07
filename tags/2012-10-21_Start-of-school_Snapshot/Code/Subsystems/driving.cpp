#include "driving.h"


/**
 * @brief An empty constructor -- currently does nothing.
 */
BaseJoystickController::BaseJoystickController():
		BaseController()
{
	mAreValuesSwapped = false;
}

/**
 * @brief Gets the magnitude of the robot based on the
 * throttle.
 * 
 * @param[in] joystick A pointer to the joystick to get the 
 * throttle value from.
 * 
 * @returns A number between BaseJoystickController::kSpeedFactorMin 
 * BaseJoystickController::kSpeedFactorMax. 
 */
float BaseJoystickController::GetSpeedFactor(Joystick *joystick)
{
	float rawFactor = -joystick->GetThrottle();
	float normalizedFactor = Tools::Coerce(
			rawFactor,
			-1.0,
			1.0,
			kSpeedFactorMin,
			kSpeedFactorMax);
	return normalizedFactor;
}



/**
 * @brief Stops the robot.
 */
void BaseJoystickController::Stop(RobotDrive *robotDrive)
{
	robotDrive->Drive(0.0, 0.0);
}



/**
 * @brief Initializes the object.
 * 
 * @param[in] controllers A vector containing pointers
 * to all the controllers you want to switch between.
 */
ControllerSwitcher::ControllerSwitcher(
		vector<BaseController*> controllers) :
		BaseController()
{
	mControllers = controllers;
		
	mControllerSize = (int) mControllers.size();
	
	mLabel = "(CONTROLLER) << ";
	
	SmartDashboard::GetInstance()->PutString(mLabel, "0");
}

/**
 * @brief The destructor for this object.
 * 
 * @details Currently does nothing.
 */
ControllerSwitcher::~ControllerSwitcher()
{
	// Empty
}

/**
 * @brief Runs the switcher.
 * 
 * @details Call this function repeatedly inside MainRobot::OperatorControl().
 * This interfaces with the SmartDashboard to run the controller.
 */
void ControllerSwitcher::Run()
{
	int current = (int) Tools::StringToFloat(SmartDashboard::GetInstance()->GetString(mLabel));
	
	mControllers.at(current)->Run();
}

/**
 * @brief The constructor.
 */
TestMotor::TestMotor(Joystick *joystick, SpeedController *speedController, const char* name)
{
	mJoystick = joystick;
	mSpeedController = speedController;
	mName = name;
}

/**
 * @brief Moves a motor based on joystick input.
 */
void TestMotor::Run()
{
	float speed = mJoystick->GetY();
	SmartDashboard::GetInstance()->Log(speed, mName);
	mSpeedController->Set(speed);
}

/**
 * @brief The constructor.
 */
MinimalistDrive::MinimalistDrive(RobotDrive *robotDrive) :
		BaseController()
{
	mRobotDrive = robotDrive;
}

/**
 * @brief Halts the robot from moving while still feeding all
 * relevant watchdogs.
 */
void MinimalistDrive::Run()
{
	mRobotDrive->TankDrive(0.0, 0.0);
}


/**
 * @brief A method of driving the robot via TankDrive with two
 * joysticks
 * 
 * @param[in] robotDrive A pointer to the RobotDrive object created 
 * in MyRobot::InitiateHardware
 * 
 * @param[in] leftJoystick A pointer to the left joystick 
 * (controls the left side of the robot)
 * 
 * @param[in] rightJoystick A pointer to the right joystick 
 * (controls the right side of the robot)
 */
TankJoysticks::TankJoysticks(
			RobotDrive *robotDrive,
			Joystick *leftJoystick, 
			Joystick *rightJoystick):
		BaseJoystickController()
{
	mRobotDrive = robotDrive;
	mLeftJoystick = leftJoystick;
	mRightJoystick = rightJoystick;
}

/**
 * @brief This method is called automatically during MyRobot::OperatorControl
 * 
 * @details
 * It should contain no 'Wait' statements or excessive loops.
 * 
 * This will currently also log the left and right speed values to the
 * SmartDashboard. 
 */
void TankJoysticks::Run(void)
{
	DriveSpeed driveSpeed(mLeftJoystick->GetY(), mRightJoystick->GetY());
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	driveSpeed = Filter::SquareInput(driveSpeed);
	
	s->Log(driveSpeed.Left, "SquareInputLeft");
	
	if (mRightJoystick->GetRawButton(6)) {
		mAreValuesSwapped = false;
	} else if (mRightJoystick->GetRawButton(7)) {
		mAreValuesSwapped = true;
	}
	
	if (mAreValuesSwapped) {
		driveSpeed = Filter::ReverseDirection(driveSpeed);
		SmartDashboard::GetInstance()->Log("reversed", "(TANK DRIVE) Driving orientation: ");
	} else {
		SmartDashboard::GetInstance()->Log("normal", "(TANK DRIVE) Driving orientation: ");
	}
	
	driveSpeed = Filter::AddSpeedFactor(driveSpeed, GetSpeedFactor(mLeftJoystick));
	
	s->Log(driveSpeed.Left, "SpeedFactorLeft");
	
	if (mLeftJoystick->GetRawButton(3) or mRightJoystick->GetRawButton(3)) {
		driveSpeed = Filter::Straighten(driveSpeed);	// Check both joysticks if the appropriate button is being pressed.
	}
	s->Log(driveSpeed.Left, "StraightenLeft");
	driveSpeed = Filter::AddTruncation(driveSpeed);
	
	SmartDashboard::GetInstance()->Log(driveSpeed.Left, "(TANK DRIVE) Left speed ");
	SmartDashboard::GetInstance()->Log(driveSpeed.Right, "(TANK DRIVE) Right speed ");
	
	mRobotDrive->TankDrive(driveSpeed.Left, driveSpeed.Right);
}

ArcadeJoystick::ArcadeJoystick(RobotDrive *robotDrive, Joystick *joystick) :
		BaseJoystickController()
{
	mRobotDrive = robotDrive;
	mJoystick = joystick;
}

/**
 * Deliberately not using 'RobotDrive::ArcadeDrive' so we can get the ultimate left and right values.
 */
void ArcadeJoystick::Run()
{
	float magnitude = mJoystick->GetY();
	float rotation = mJoystick->GetX();
	
	DriveSpeed driveSpeed = GetLeftAndRight(magnitude, rotation);
	driveSpeed = Filter::SquareInput(driveSpeed);
	
	if (mJoystick->GetRawButton(6)) {
		mAreValuesSwapped = false;
	} else if (mJoystick->GetRawButton(7)) {
		mAreValuesSwapped = true;
	}
	
	if (mAreValuesSwapped) {
		driveSpeed = Filter::ReverseDirection(driveSpeed);
		SmartDashboard::GetInstance()->Log("reversed", "(ARCADE DRIVE) Driving orientation: ");
	} else {
		SmartDashboard::GetInstance()->Log("normal", "(ARCADE DRIVE) Driving orientation: ");
	}
	
	driveSpeed = Filter::AddSpeedFactor(driveSpeed, GetSpeedFactor(mJoystick));
	
	driveSpeed = Filter::AddTruncation(driveSpeed);
		
	SmartDashboard::GetInstance()->Log(driveSpeed.Left, "(ARCADE DRIVE) Left speed ");
	SmartDashboard::GetInstance()->Log(driveSpeed.Right, "(ARCADE DRIVE) Right speed ");
	
	mRobotDrive->TankDrive(driveSpeed.Left, driveSpeed.Right);
}

/**
 * This is copied directly from FIRST's source code for ArcadeDrive.
 */
DriveSpeed ArcadeJoystick::GetLeftAndRight(float moveValue, float rotateValue)
{
	float leftMotorOutput;
	float rightMotorOutput;
	
	if (moveValue > 0.0)
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = max(moveValue, rotateValue);
		}
		else
		{
			leftMotorOutput = max(moveValue, -rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
	}
	else
	{
		if (rotateValue > 0.0)
		{
			leftMotorOutput = - max(-moveValue, rotateValue);
			rightMotorOutput = moveValue + rotateValue;
		}
		else
		{
			leftMotorOutput = moveValue - rotateValue;
			rightMotorOutput = - max(-moveValue, -rotateValue);
		}
	}
	return DriveSpeed(leftMotorOutput, rightMotorOutput);
}
	


/**
 * @brief A method of controlling the robot by using only
 * one joystick.
 * 
 * @details
 * This is a form of controlling the robot by using the single
 * Extreme 3D Pro joystick.  This differs from our other
 * joysticks because it comes with twisting motions.
 * 
 * @param[in] robotDrive A pointer to the robotdrive
 * @param[in] joystick A pointer to the joystick (use the Extreme
 * 3d Pro joystick)
 */
SingleJoystick::SingleJoystick(RobotDrive *robotDrive, Joystick *joystick)
{
	mRobotDrive = robotDrive;
	mJoystick = joystick;
}

/**
 * @brief This method is called automatically during MyRobot::OperatorControl
 * 
 * @details
 * It should contain no 'Wait' statements or excessive loops.
 * 
 * This will currently also log values to the SmartDashboard.
 */
void SingleJoystick::Run()
{
	float rotate = -mJoystick->GetZ();
	float speed = mJoystick->GetY();
	
	float speedFactor = GetSpeedDecreaseFactor();	
	
	rotate *= speedFactor;
	speed *= speedFactor;
	
	SmartDashboard::GetInstance()->Log(-rotate, "(ARCADE DRIVE) Rotate ");
	SmartDashboard::GetInstance()->Log(-speed, "(ARCADE DRIVE) Speed ");
	SmartDashboard::GetInstance()->Log(speedFactor, "(ARCADE DRIVE) Speed factor ");
		
	mRobotDrive->ArcadeDrive(speed, rotate);
}

/**
 * @brief Temporary.
 */

float SingleJoystick::GetSpeedDecreaseFactor(void)
{
	float rawFactor = -mJoystick->GetTwist();
	float normalizedFactor = Tools::Coerce(
			rawFactor,
			-1.0,
			1.0,
			kSpeedFactorMin,
			kSpeedFactorMax);
	return normalizedFactor;
}

SafetyMode::SafetyMode(RobotDrive *robotdrive, Joystick *leftJoystick, Joystick *rightJoystick, Joystick *safetyJoystick)
{
	mRobotDrive = robotdrive;
	mLeftJoystick = leftJoystick;
	mRightJoystick = rightJoystick;
	mSafetyJoystick = safetyJoystick;
	SmartDashboard::GetInstance()->PutString("(SAFETY) Speed << ", "0.3");
}

void SafetyMode::Run()
{
	DriveSpeed driveSpeed = DriveSpeed();
	if (mSafetyJoystick->GetRawButton(5)) {
		driveSpeed.Left = mLeftJoystick->GetY();
		driveSpeed.Right = mRightJoystick->GetY();
	}
	
	
	if (mRightJoystick->GetRawButton(6)) {
		mAreValuesSwapped = false;
	} else if (mRightJoystick->GetRawButton(7)) {
		mAreValuesSwapped = true;
	}
	
	if (mAreValuesSwapped) {
		driveSpeed = Filter::ReverseDirection(driveSpeed);
		SmartDashboard::GetInstance()->Log("reversed", "(TANK DRIVE) Driving orientation: ");
	} else {
		SmartDashboard::GetInstance()->Log("normal", "(TANK DRIVE) Driving orientation: ");
	}
	
	driveSpeed = Filter::SquareInput(driveSpeed);
	driveSpeed = Filter::AddSpeedFactor(driveSpeed, GetSpeedFactor(mLeftJoystick));
	
	if (mLeftJoystick->GetRawButton(3) or mRightJoystick->GetRawButton(3)) {
		driveSpeed = Filter::Straighten(driveSpeed);	// Check both joysticks if the appropriate button is being pressed.
	}

	float speedFactor = Tools::StringToFloat(SmartDashboard::GetInstance()->GetString("(SAFETY) Speed << "));
	driveSpeed = Filter::AddSpeedFactor(driveSpeed, speedFactor);
	
	driveSpeed = Filter::AddTruncation(driveSpeed);
	
	SmartDashboard::GetInstance()->Log(driveSpeed.Left, "(SAFETY DRIVE) Left speed ");
	SmartDashboard::GetInstance()->Log(driveSpeed.Right, "(SAFETY DRIVE) Right speed ");
	
	mRobotDrive->TankDrive(driveSpeed.Left, driveSpeed.Right);
}


/**
 * @brief A way to control the robot during Hybrid mode,
 * using the angle of the hands to the shoulders on the 
 * Y-axis.
 * 
 * @param[in] robotDrive A pointer to the robotdrive.
 * @param[in] kinect A pointer to the kinect.
 */
BaseKinectController::BaseKinectController(
		RobotDrive *robotDrive,
		Kinect *kinect) :
		BaseController()
{
	mRobotDrive = robotDrive;
	mKinect = kinect;
}

/**
 * @brief Checks to see if the player is ready to
 * operate the robot.
 * 
 * @todo
 * This assumes that the x-value of the left hand is lesser
 * then the x-value of the right hand.  This assumption 
 * needs to be tested.
 * 
 * @returns Returns false if:
 *   - The skeleton is not being tracked
 *   - The player's hands are crossed (relative 
 *     to each other)
 *     
 *
 */
bool BaseKinectController::IsPlayerReady()
{	
	bool isPlayerTracked = mKinect->GetTrackingState() == Kinect::kTracked;
	
	float leftX = mKinect->GetSkeleton().GetWristLeft().x;
	float rightX = mKinect->GetSkeleton().GetWristRight().y;
	bool areHandsCrossed = leftX > rightX;
	
	return isPlayerTracked or areHandsCrossed;
}

/**
 * @brief Halts the robot.
 */
void BaseKinectController::HaltRobot(void)
{
	mRobotDrive->TankDrive(0.0, 0.0);
}

/**
 * @brief A way to control the robot by using the Kinect during
 * Hybrid mode, using the z-distance of the hands to control 
 * the robot.
 * 
 * @param[in] robotDrive A pointer to the robotdrive.
 * @param[in] kinect A pointer to the Kinect.
 */
KinectController::KinectController(RobotDrive *robotDrive, Kinect *kinect):
		BaseKinectController(robotDrive, kinect)
{
	// Empty.
}

/**
 * @brief This method is called automatically during MyRobot::OperatorControl
 * 
 * @details
 * It should contain no 'Wait' statements or excessive loops.
 * 
 * The left hand controls the left treads.  The right hand controls the
 * right treads.  The closer the hands are to the robot, the faster
 * that side spins.  Pulling your hands back will soon make the robot back up.
 * 
 * The moment any of your hands are between the shoulders, the robot will
 * freeze.  Therefore, the safest way to immediately halt the robot is 
 * to suddenly cross your arms in an 'X' shape.
 * 
 * Raising both your hands up will make the robot shoot (like shooting a
 * basketball)
 * 
 * This will currently also log values to the SmartDashboard.
 */
void KinectController::Run(void)
{
	bool isPlayerReady = IsPlayerReady();
	SmartDashboard::GetInstance()->Log(isPlayerReady, "(KINECT) Player is ready ");
	
	if (isPlayerReady) {
		float left = GetLeftArmDistance();
		float right = GetRightArmDistance();
		mRobotDrive->TankDrive(left, right);
			
		SmartDashboard::GetInstance()->Log(left, "(KINECT) Left speed ");
		SmartDashboard::GetInstance()->Log(right, "(KINECT) Right speed ");
	} else {
		HaltRobot();		
	}
	
	bool isShooting = IsPlayerShooting();
	SmartDashboard::GetInstance()->Log(isShooting, "(KINECT) Player is shooting ");
}

/**
 * @brief Returns how far the left hand is.
 * 
 * @returns A number between -1.0 and 1.0.
 * The closer the hand is to the Kinect, the higher the 
 * returned value.
 */
float KinectController::GetLeftArmDistance(void)
{
	float originJoint = mKinect->GetSkeleton().GetShoulderLeft().z;
	float movingJoint = mKinect->GetSkeleton().GetWristLeft().z;
	
	float distance = originJoint - movingJoint;
	float output = Tools::Coerce(distance, kArmMinZ, kArmMaxZ, -1, 1);
	
	return output;
}

/**
 * @brief Returns how far the right hand is.
 * 
 * @returns A number between -1.0 and 1.0.
 * The closer the hand is to the Kinect, the higher the 
 * returned value.
 */
float KinectController::GetRightArmDistance(void)
{
	float originJoint = mKinect->GetSkeleton().GetShoulderRight().z;
	float movingJoint = mKinect->GetSkeleton().GetWristRight().z;
	
	float distance = originJoint - movingJoint;
	float output = Tools::Coerce(distance, kArmMinZ, kArmMaxZ, -1, 1);
	
	return output;	
}

/**
 * @brief Checks to see if the player is shooting.
 * 
 * @details
 * The robot will check to see if the player has made
 * a 'basketball shooting' movement to shoot
 * the ball.
 * 
 * @returns 'true' if the player's hands are
 * elevated above the head; 'false' if they aren't.
 */
bool KinectController::IsPlayerShooting(void)
{
	float rightOrigin = mKinect->GetSkeleton().GetShoulderRight().y;
	float rightMoving = mKinect->GetSkeleton().GetWristRight().y;
	
	float leftOrigin = mKinect->GetSkeleton().GetShoulderLeft().y;
	float leftMoving = mKinect->GetSkeleton().GetWristLeft().y;
	
	float rightDelta = rightMoving - rightOrigin;
	float leftDelta = leftMoving - leftOrigin;
	
	// Like shooting a basketball.
	if ((rightDelta > kShootThresholdY) or (leftDelta > kShootThresholdY)) {
		return true;
	} else {
		return false;
	}
}

/**
 * @brief Constructor for KinectAngleController class.
 * 
 * @param[in] robotDrive Pointer to RobotDrive object.
 * @param[in] leftKinectStick Pointer to left Kinectstick.
 * @param[in] rightKinectStick Pointer to right Kinectstick.
 * @param[in] kinect Pointer to the Kinect.
 * @param[in] shooter Pointer to Shooter object.
 */
KinectAngleController::KinectAngleController(RobotDrive *robotDrive, KinectStick *leftKinectStick, 
		KinectStick *rightKinectStick, Kinect *kinect, Shooter *shooter, BaseArmComponent *arm) :
		BaseKinectController(robotDrive, kinect)
{
	mLeftKinectStick = leftKinectStick;
	mRightKinectStick = rightKinectStick;
	mShooter = shooter;
	mArm = arm;
}

/**
 * @brief Checks to see if player has made a pushing-forward motion with
 * his or her right wrist to shoot manually (at full speed).
 * 
 * @returns True if the player's right wrist is 
 * a certain distance (0.3) away from his or her right shoulder.
 * 
 * False otherwise.
 */
bool KinectAngleController::IsManuallyShooting(void)
{
	float rightOrigin = mKinect->GetSkeleton().GetShoulderRight().z;
	float rightMoving = mKinect->GetSkeleton().GetWristRight().z;
	
	float rightDelta = rightMoving - rightOrigin;
	
	// Pushing right hand forward to shoot.
	if (fabs(rightDelta) > kPushThreshold) {   
		return true;
	} else {
		return false;
	}
}

/**
 * @brief Checks to see if player has made a pushing-forward motion with
 * his or her left wrist to shoot at a speed calculated from the 
 * distance between the shooter and the hoop.
 * 
 * @returns True if the player's left wrist is 
 * a certain distance (0.3) away from his or her left shoulder.
 * 
 * False otherwise.
 */
bool KinectAngleController::IsAutomaticallyShooting(void)
{
	float leftOrigin = mKinect->GetSkeleton().GetShoulderLeft().z;
	float leftMoving = mKinect->GetSkeleton().GetWristLeft().z;
	
	float leftDelta = leftMoving - leftOrigin;
	
	if (fabs(leftDelta) > kPushThreshold) {
		return true;
	} else {
		return false;
	}
}

bool KinectAngleController::IsLoweringArm(void)
{
	float headOrigin = mKinect->GetSkeleton().GetHead().x;
	float headMoving = mKinect->GetSkeleton().GetShoulderLeft().x;
	
	float headDelta = headMoving - headOrigin;
	
	if (fabs(headDelta) < kArmThreshold) {
		return true;
	} else {
		return false;
	}
}

bool KinectAngleController::IsRaisingArm(void)
{
	float headOrigin = mKinect->GetSkeleton().GetHead().x;
	float headMoving = mKinect->GetSkeleton().GetShoulderRight().x;
	
	float headDelta = headMoving - headOrigin;
	
	SmartDashboard::GetInstance()->Log(headDelta, "(KINECT) Head delta ");
	
	if (fabs(headDelta) < kArmThreshold) {
		return true;
	} else {
		return false;
	}
}
/**
 * @brief This method is called automatically during MyRobot:AutonomousControl.
 * 
 * @details
 * Uses KinectStick class to drive the robot like a tank.
 * 
 * When the arms are straight out (perpendicular to the rest of the body), 
 * the speed of the robot is set to 0.0.
 * Raising the arms in an arc increases the speed forward.
 * Lowering the arms in an arc increases the speed backward.
 * 
 * The robot will freeze when either of the driver's hands is between
 * the shoulders. Therefore, the easiest way to stop the robot
 * is to cross one's arms in an 'X' shape.
 * 
 * Moving one's right wrist a certain distance (0.3) forward will tell
 * the robot to shoot at full speed. Moving one's left wrist the same
 * distance forward will tell the robot to shoot at a speed
 * calculated from the distance between the shooter and the hoop.
 * 
 * This will log values to SmartDashboard.
 */
void KinectAngleController::Run(void)
{
	bool isPlayerReady = IsPlayerReady();
	SmartDashboard::GetInstance()->Log(isPlayerReady, "(KINECT) Player is ready ");
	
	if ( isPlayerReady ) {
		float rightY = mRightKinectStick->GetY();
		float leftY = mLeftKinectStick->GetY();
		SmartDashboard::GetInstance()->Log(rightY, "(KINECT) Right speed ");
		SmartDashboard::GetInstance()->Log(leftY, "(KINECT) Left speed ");
		mRobotDrive->TankDrive(leftY * kSpeedDecreaseFactor * -1, rightY * kSpeedDecreaseFactor * -1);
	} else {
		HaltRobot();
	}
	
	bool isManuallyShooting = IsManuallyShooting();
	//bool isAutomaticallyShooting = IsAutomaticallyShooting();
	bool isAutomaticallyShooting = false;
	
	SmartDashboard::GetInstance()->Log(isManuallyShooting, "(KINECT) Player is shooting manually ");
	//SmartDashboard::GetInstance()->Log(isAutomaticallyShooting, "(KINECT) Player is shooting automatically ");
	//SmartDashboard::GetInstance()->Log((isManuallyShooting && isAutomaticallyShooting), "(KINECT) Player is being stupid ");
	
	if (isManuallyShooting && !isAutomaticallyShooting) {
		mShooter->SetSpeed(1.0);
	//} else if (isAutomaticallyShooting && !isManuallyShooting) {
	//	mShooter->SetSpeedAutomatically();
	} else if (isAutomaticallyShooting && isManuallyShooting) {
		mShooter->SetSpeed(0.0);
	}
	
	bool isRaisingArm = IsRaisingArm();
	bool isLoweringArm = IsLoweringArm();
	
	SmartDashboard::GetInstance()->Log(isRaisingArm, "(KINECT) Player is raising arm ");
	SmartDashboard::GetInstance()->Log(isLoweringArm, "(KINECT) Player is lowering arm ");
	
	if (isRaisingArm) {
		mArm->GoUp();
	} else if (isLoweringArm) {
		mArm->GoDown();
	} else {
		mArm->Stop();
	}
}

XboxDrive::XboxDrive(RobotDrive *robotDrive, XboxController *xboxController) :
	BaseController() 
{
	mRobotDrive = robotDrive;
	mXboxController = xboxController;
}

void XboxDrive::Run() {
	float movement = mXboxController->GetAxis(mXboxController->LeftY);
	float rotation = mXboxController->GetAxis(mXboxController->RightX);

	mRobotDrive->ArcadeDrive(movement, rotation, true);
}
