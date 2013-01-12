#include "robot.h"


MainRobot::MainRobot(void)
{
	// Empty
}

/**
 * @brief Initializes all hardware, input devices, and software 
 * for the entire robot.
 * 
 * @details This class will also enable the watchdog.
 */
void MainRobot::RobotInit(void)
{
	InitializeHardware();
	InitializeInputDevices();
	InitializeComponents();
	InitializeControllers();
	
	//mTargetTask = Task(
	//		"TargetFinder",
	//		mTargetFinder->Run,
	//		Task::kDefaultPriority);
	
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
			Ports::Module1,
			Ports::Pwm1);
	mLeftBackDrive = new Jaguar(
			Ports::Module1,
			Ports::Pwm2);
	mRightFrontDrive = new Jaguar(
			Ports::Module1,
			Ports::Pwm3);
	mRightBackDrive = new Jaguar(
			Ports::Module1,
			Ports::Pwm4);
	
	mRobotDrive = new RobotDrive(
			mLeftFrontDrive,
			mLeftBackDrive,
			mRightFrontDrive,
			mRightBackDrive);
	
	mTopLeftShooter = new Jaguar(
			Ports::Module1,
			Ports::Pwm5);
	mTopRightShooter = new Jaguar(
			Ports::Module1,
			Ports::Pwm6);
	mBottomLeftShooter = new Jaguar(
			Ports::Module1,
			Ports::Pwm7);
	mBottomRightShooter = new Jaguar(
			Ports::Module1,
			Ports::Pwm8);
	mElevatorSpeedController = new Jaguar(
			Ports::Module1,
			Ports::Pwm9);
	mArmSpeedController = new Jaguar(
			Ports::Module1,
			Ports::Pwm10);
	//*
	mUltrasoundSensor = new AnalogChannel(
			Ports::Module1,
			Ports::AnalogChannel1);
	//*/
	//mYawGyro = new Gyro(
	//		Ports::Module1,
	//		Ports::AnalogChannel1);
	mPitchGyro = new Gyro(1,2);
	
	// The camera is technically a hardware component, but WPILib's
	// AxisCamera class has a built-in static method for returning
	// instances of a camera
	
	mElevatorBottomLimitSwitch = new DigitalInput(
			Ports::Module1,
			Ports::DigitalIo1);
	//mElevatorTopLimitSwitch = new DigitalInput(
			//Ports::DigitalIo2);

	//mTestDigitalInput = new DigitalInput(
				//Ports::DigitalIo9);
	
	mTopLimit = new DigitalInput(
			Ports::DigitalIo3);
	mBottomLimit = new DigitalInput(
			Ports::DigitalIo4);
	
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
	
	mKinect = Kinect::GetInstance();
	mLeftKinectStick = new KinectStick::KinectStick(1);
	mRightKinectStick = new KinectStick::KinectStick(2);
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
	//*
	mRangeFinder = new RangeFinder(mUltrasoundSensor);
	mShooter = new Shooter(
			mTopLeftShooter,
			mTopRightShooter,
			mBottomLeftShooter,
			mBottomRightShooter,
			mRangeFinder);
	
	mElevator = new Elevator(mElevatorBottomLimitSwitch, mElevatorSpeedController);
	//mTargetFinder = new TargetFinder();*/
	mArm = new SimpleArm(mArmSpeedController);
	//mArm = new SingleGuardedArm(mArmSpeedController, mBottomLimit);
	//mArm = new GuardedArm(mArmSpeedController, mTopLimit, mBottomLimit);
	//mCameraAdjuster = new CameraAdjuster(mCameraServo);*/
	//*/
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
			mRightJoystick, 
			mPitchGyro, 
			GetWatchdog()));
	controllers.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	controllers.push_back(new MinimalistDrive(mRobotDrive));
	//controllers.push_back(new KinectAngleController(mRobotDrive, mLeftKinectStick, mRightKinectStick, mKinect, mShooter, mArm));
	
	mControllerCollection.push_back(new ControllerSwitcher(controllers));

	//mControllerCollection.push_back(new TankJoysticks(mRobotDrive, mLeftJoystick, mRightJoystick, mPitchGyro, GetWatchdog()));
	//mControllerCollection.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	//mControllerCollection.push_back(new MinimalistDrive(mRobotDrive));
	
	mControllerCollection.push_back(new ShooterController(mShooter, mTwistJoystick));
	mControllerCollection.push_back(new ElevatorController(mElevator, mTwistJoystick));
	//mControllerCollection.push_back(new ArmController(mArm, mTwistJoystick));
	
	//mControllerCollection.push_back(new RangeFinderTest(mRangeFinder));
	//mControllerCollection.push_back(new GyroTest(mPitchGyro));
	//mControllerCollection.push_back(new TargetFinder());

	/* 
	mControllerCollection.push_back(new TargetSnapshotController(
			mRobotDrive,
			mTargetFinder, 
			mTwistJoystick, 
			GetWatchdog(),
			mYawGyro));
	//*/
	
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
	delete mRobotDrive;
	delete mElevatorSpeedController;
	delete mUltrasoundSensor;
	delete mYawGyro;
	delete mPitchGyro;
	
	delete mLeftJoystick;
	delete mRightJoystick;
	delete mTwistJoystick;
	
	delete mRangeFinder;
	int collectionSize = (int) mControllerCollection.size();
	
	for (int i=0; i<collectionSize; i++) {
		delete mControllerCollection.at(i);
	}
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


START_ROBOT_CLASS(MainRobot);

