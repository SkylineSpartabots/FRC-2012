#include "robot.h"

//#define ROBOT_MULTIPLE_CONTROLLERS

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
	mRobotDrive = new RobotDrive(
			Ports::Pwm1,		// Left front 
			Ports::Pwm2, 		// Left back
			Ports::Pwm3, 		// Right front
			Ports::Pwm4);		// Right back
	
	mTopLeftShooter = new Jaguar(
			Ports::Pwm5);
	mTopRightShooter = new Jaguar(
			Ports::Pwm6);
	mBottomLeftShooter = new Jaguar(
			Ports::Pwm7);
	mBottomRightShooter = new Jaguar(
			Ports::Pwm8);
	mElevatorSpeedController = new Jaguar(
			Ports::Pwm9);
	mArmSpeedController = new Victor(
			Ports::Pwm10);
	
	mUltrasoundSensor = new AnalogChannel(
			Ports::Module1,
			Ports::AnalogChannel1);
	mGyro = new Gyro(
			Ports::Module1,
			Ports::AnalogChannel2);
	// The camera is technically a hardware component, but WPILib's
	// AxisCamera class has a built-in static method for returning
	// instances of a camera
	
	mElevatorBottomLimitSwitch = new DigitalInput(
			Ports::DigitalIo1);
	//mElevatorTopLimitSwitch = new DigitalInput(
			//Ports::DigitalIo2);
	//mTestDigitalInput = new DigitalInput(
				//Ports::DigitalIo9);
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
			mBottomRightShooter,
			mRangeFinder);
	mLeftKinectStick = new KinectStick::KinectStick(1);
	mRightKinectStick = new KinectStick::KinectStick(2);
	mElevator = new Elevator(mElevatorBottomLimitSwitch, mElevatorSpeedController);
	//mTargetFinder = new TargetFinder();
	mArm = new SimpleArm(mArmSpeedController);
	
	
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
	/*vector<BaseController *> controllers;
	vector<Joystick *> joysticks;
	controllers.push_back(new TankJoysticks(mRobotDrive, mLeftJoystick, mRightJoystick));
	controllers.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	controllers.push_back(new MinimalistDrive(mRobotDrive));
	controllers.push_back(new KinectAngleController(mRobotDrive, mLeftKinectStick, mRightKinectStick, mKinect, mShooter));
	
	joysticks.push_back(mLeftJoystick);
	joysticks.push_back(mRightJoystick);
	joysticks.push_back(mTwistJoystick);
	*/
	//mControllerCollection.push_back(new ControllerSwitcher(controllers, joysticks));

	//mControllerCollection.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));

	mControllerCollection.push_back(new ShooterController(mShooter, mRightJoystick));
	mControllerCollection.push_back(new ElevatorController(mElevator, mRightJoystick));
	
	//mControllerCollection.push_back(new TimerTest());
	//mControllerCollection.push_back(new RangeFinderTest(mRangeFinder));
	//mControllerCollection.push_back(new GyroTest(mGyro));
	//mControllerCollection.push_back(new TargetFinder());
	//mControllerCollection.push_back(new ArmController(mArm, mRightJoystick));
	
	//mControllerCollection.push_back(new TankJoysticks(mRobotDrive, mLeftJoystick, mRightJoystick));
	//mControllerCollection.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	//mControllerCollection.push_back(new KinectController(mRobotDrive, mKinect));
	//mControllerCollection.push_back(new KinectAngleController(mRobotDrive, mLeftKinectStick, mRightKinectStick, mKinect, mShooter));
	mControllerCollection.push_back(new MinimalistDrive(mRobotDrive));
	
	//mControllerCollection.push_back(new TargetSnapshotController(mTargetFinder, mLeftJoystick, GetWatchdog()));
	//mControllerCollection.push_back(new TestMotor(mLeftJoystick, mTestMotor));
	//mControllerCollection.push_back(new TestThreadController(mTestThreadListener));
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
	delete mGyro;
	
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
	while (IsAutonomous()) {
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

