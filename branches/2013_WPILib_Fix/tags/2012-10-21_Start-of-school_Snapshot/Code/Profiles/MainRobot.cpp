#include "MainRobot.h"

MainRobot::MainRobot(void)
{
	InitializeHardware();
	InitializeInputDevices();
	InitializeComponents();
	InitializeControllers();
}

/**
 * @brief Initializes all hardware, input devices, and software 
 * for the entire robot.
 * 
 * @details This class will also enable the watchdog.
 */
void MainRobot::RobotInit(void)
{

	
	GetWatchdog().SetExpiration(kWatchdogExpiration);
	SmartDashboard::GetInstance()->PutString("Autonomous? <<", "0.0");
	return;
}

/**
 * @brief Initializes any code provided by WPILib meant to
 * interface directly with specific hardware components.
 * 
 * @details Currently initializes the 
 *   - Robot drive
 *   - Jaguars for the shooter
 *   - Ultrasound sensor
 *   - Gyro 
 */
void MainRobot::InitializeHardware(void)
{
	mLeftFrontDrive = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm1);
	mLeftBackDrive = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm2);
	mRightFrontDrive = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm3);
	mRightBackDrive = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm4);
	
	mRobotDrive = new RobotDrive(
			mLeftFrontDrive,
			mLeftBackDrive,
			mRightFrontDrive,
			mRightBackDrive);
	mRobotDrive->SetInvertedMotor(mRobotDrive->kFrontLeftMotor, true);
	mRobotDrive->SetInvertedMotor(mRobotDrive->kRearLeftMotor, true);
	mRobotDrive->SetInvertedMotor(mRobotDrive->kFrontRightMotor, true);
	mRobotDrive->SetInvertedMotor(mRobotDrive->kRearRightMotor, true);
	
	mTopLeftShooter = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm5);
	mTopRightShooter = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm6);
	mBottomLeftShooter = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm7);
	mBottomRightShooter = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm8);
	mElevatorSpeedController = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm9);
	mArmSpeedController = new Jaguar(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm10);
	mUltrasoundSensor = new AnalogChannel(
			Ports::Crio::Module1,
			Ports::Crio::AnalogChannel1);
	
	
	// The camera is technically a hardware component, but WPILib's
	// AxisCamera class has a built-in static method for returning
	// instances of a camera
	
	mElevatorBottomLimitSwitch = new DigitalInput(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio1);
	
	mTopLimit = new DigitalInput(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio3);
	mBottomLimit = new DigitalInput(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio2);
	return;
}

/**
 * @brief Initializes any hardware used by the laptop to send
 * data over to the robot (joysticks, etc).
 * 
 * @details Currently initializes the
 *   - Joysticks
 *   - Kinect
 */
void MainRobot::InitializeInputDevices(void)
{
	mLeftJoystick = new Joystick(
			Ports::Usb1);
	mRightJoystick = new Joystick(
			Ports::Usb2);
	
	mTwistJoystick = new Joystick(
			Ports::Usb3);
	
	//mXboxController = new XboxController(
	//		Ports::Usb4);
	/*
	mKinect = Kinect::GetInstance();
	mLeftKinectStick = new KinectStick::KinectStick(1);
	mRightKinectStick = new KinectStick::KinectStick(2);
	*/
}

/**
 * @brief Initializes any software components that 
 * bundles together hardware needed to provide 
 * additional functionality.
 * 
 * @details For example, code to run the shooter would
 * be initialized here because it uses several
 * Jaguars and a Joystick to function.
 * 
 * Ideally, each class instantiated should have BaseComponent
 * as a parent class.
 */
void MainRobot::InitializeComponents(void)
{
	mRangeFinder = new RangeFinder(mUltrasoundSensor);
	mShooter = new Shooter(
			mTopLeftShooter,
			mTopRightShooter,
			mBottomLeftShooter,
			mBottomRightShooter);
	
	mElevator = new Elevator(mElevatorBottomLimitSwitch, mElevatorSpeedController);
	//mArm = new SimpleArm(mArmSpeedController);
	//mArm = new SingleGuardedArm(mArmSpeedController, mElevatorBottomLimitSwitch);
	mArm = new GuardedArm(mArmSpeedController, mTopLimit, mBottomLimit);
}

/**
 * @brief Initializes any software that takes in user input to 
 * manipulate hardware or software, or needs to be called
 * periodically during operator control
 * 
 * @details Each object initialized in this file must inherit 
 * BaseController.  They are assigned to the heap, and appended 
 * to the end of MainRobot::mControllerCollection, 
 */
void MainRobot::InitializeControllers(void)
{
	vector<BaseController *> controllers;
	controllers.push_back(new TankJoysticks(
			mRobotDrive, 
			mLeftJoystick, 
			mRightJoystick));
	controllers.push_back(new SafetyMode(
			mRobotDrive,
			mLeftJoystick,
			mRightJoystick,
			mTwistJoystick));
	controllers.push_back(new SingleJoystick(
			mRobotDrive, 
			mTwistJoystick));
	controllers.push_back(new MinimalistDrive(
			mRobotDrive));
	controllers.push_back(new KinectAngleController(
			mRobotDrive, 
			mLeftKinectStick, 
			mRightKinectStick, 
			mKinect, 
			mShooter, 
			mArm));
	controllers.push_back(new XboxDrive(
			mRobotDrive, 
			mXboxController));
	mControllerCollection.push_back(new ControllerSwitcher(controllers));
	
	//mControllerCollection.push_back(new TankJoysticks(mRobotDrive, mLeftJoystick, mRightJoystick));
	//mControllerCollection.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	//mControllerCollection.push_back(new MinimalistDrive(mRobotDrive));
	//mControllerCollection.push_back(new XboxDrive(mRobotDrive, mXboxController));
	
	mControllerCollection.push_back(new CalibratedShooterController(mShooter, mTwistJoystick));
	mControllerCollection.push_back(new ShooterController(mShooter, mTwistJoystick));
	mControllerCollection.push_back(new ElevatorController(mElevator, mTwistJoystick));
	//mControllerCollection.push_back(new ShooterXboxController(mShooter, mElevator, mXboxController));
	//TODO: enable the above after testing.
	
	mControllerCollection.push_back(new MotorArmController(mArm, mLeftJoystick));
	
	
	
	//mControllerCollection.push_back(new XboxTest(mXboxController));
	return;
}

/**
 * @brief Destroys all hardware, input devices, and software
 * created for the entire robot.
 * 
 * @warning This is frequently out-of-date.  
 */
MainRobot::~MainRobot(void)
{
	
}


/**
 * @brief The code to be run during Autonomous mode.
 * 
 * @details
 * This is a mandatory function required for the robot to function.
 * When the 'Autonomous' toggle is selected and enabled on the 
 * FRC Dashboard, this function will run.
 * 
 * It is meant to be run once, at the start of the match, during
 * Hybrid mode, for 15 seconds.
 * 
 * @todo
 * Currently waits until Autonomous is disabled.
 */
void MainRobot::Autonomous(void)
{
	GetWatchdog().SetEnabled(true);
	//float shootSpeed = 0.39;
	//for(int i=0; i<300; i++) {
	//	mShooter->SetSpeedManually(shootSpeed);
	//	GetWatchdog().Feed();
	//	Wait(kMotorWait);
	//}
	while (IsAutonomous()) {
	//	mShooter->SetSpeedManually(shootSpeed);
	//	mElevator->MoveUp();
		GetWatchdog().Feed();
		Wait(kMotorWait);
	}
}

/**
 * @brief The code to be run during Teleoperated mode.
 * 
 * @detail
 * This is a mandatory function required for the robot to function.
 * When the 'Teleoperated' toggle is selected and enabled on the 
 * FRC Dashboard, this function will run.
 * 
 * This method will repeatedly call the 'Run' methods of any
 * class (that inherited BaseController) that has been 
 * added to mControllerCollection.  
 */
void MainRobot::OperatorControl(void) 
{
	GetWatchdog().SetEnabled(true);
	
	int collectionSize = (int) mControllerCollection.size();
	SmartDashboard::GetInstance()->Log("Test", "Test");
	while (IsOperatorControl())
	{
		for(int i=0; i<collectionSize; i++) {
			mControllerCollection.at(i)->Run();
			GetWatchdog().Feed();
			Wait(kMotorWait);
		}
	}
	return;
}


