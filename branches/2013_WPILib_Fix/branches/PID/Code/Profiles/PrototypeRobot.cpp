#include "PrototypeRobot.h"


PrototypeRobot::PrototypeRobot(void)
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
void PrototypeRobot::RobotInit(void)
{
	GetWatchdog().SetExpiration(kWatchdogExpiration);
	SmartDashboard::GetInstance()->PutString("Autonomous? <<", "5");
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
void PrototypeRobot::InitializeHardware(void)
{
	mLeftFrontDrive = new Victor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm1);
	mLeftBackDrive = new Victor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm2);
	mRightFrontDrive = new Victor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm3);
	mRightBackDrive = new Victor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm4);
	
	mElevatorMotor = new Victor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Pwm5);
	//*
	mRobotDrive = new RobotDrive(
			mLeftFrontDrive,
			mLeftBackDrive,
			mRightFrontDrive,
			mRightBackDrive);
	//*/

	mCompressor = new Compressor(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio14,
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Relay8);
	mSolenoid1 = new Solenoid(
			Ports::Crio::Module1,
			Ports::Crio::SolenoidBreakout1);
	mSolenoid2 = new Solenoid(
			Ports::Crio::Module1,
			Ports::Crio::SolenoidBreakout2);
	
	mLeftEncoder = new Encoder(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio9,
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio10);
	
	mRightEncoder = new Encoder(
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio2,
			Ports::Crio::Module1,
			Ports::DigitalSidecar::Gpio4);
	
	
	/*
	mPidDrive = new PidDrive(
				mLeftFrontDrive,
				mLeftBackDrive,
				mRightFrontDrive,
				mRightBackDrive,
				mLeftEncoder,
				mRightEncoder);
	//			*/
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
void PrototypeRobot::InitializeInputDevices(void)
{
	/*mLeftJoystick = new Joystick(
			Ports::Usb1);
	mRightJoystick = new Joystick(
			Ports::Usb2);
	
	mTwistJoystick = new Joystick(
			Ports::Usb3);
	*/
	
	mXboxController = new XboxController(
			Ports::Usb1);
	
	//mKinect = Kinect::GetInstance();
	//mLeftKinectStick = new KinectStick::KinectStick(1);
	//mRightKinectStick = new KinectStick::KinectStick(2);
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
void PrototypeRobot::InitializeComponents(void)
{
	mPneumaticArm = new PneumaticArm(mCompressor, mSolenoid1, mSolenoid2);
	mElevator = new Elevator(mElevatorMotor);
}

/**
 * @brief Initializes any software that takes in user input to 
 * manipulate hardware or software, or needs to be called
 * periodically during operator control
 * 
 * @details Each object initialized in this file must inherit 
 * BaseController.  They are assigned to the heap, and appended 
 * to the end of PrototypeRobot::mControllerCollection, 
 */
void PrototypeRobot::InitializeControllers(void)
{
	
	/*
	vector<BaseController *> controllers;
	controllers.push_back(new TankJoysticks(
			mRobotDrive, 
			mLeftJoystick, 
			mRightJoystick));
	controllers.push_back(new SingleJoystick(
			mRobotDrive, 
			mTwistJoystick));
	controllers.push_back(new MinimalistDrive(
			mRobotDrive));
	mControllerCollection.push_back(new ControllerSwitcher(controllers, "CONTROLLER >>"));
	//*/
	
	//mControllerCollection.push_back(new PidDriveController(mPidDrive, mXboxController));
	mControllerCollection.push_back(new XboxDrive(mRobotDrive, mXboxController));
	//mControllerCollection.push_back(new ArcadeJoystick(mRobotDrive, mLeftJoystick));
	
	//mControllerCollection.push_back(new TankJoysticks(mRobotDrive, mLeftJoystick, mRightJoystick, mPitchGyro, GetWatchdog()));
	//mControllerCollection.push_back(new SingleJoystick(mRobotDrive, mTwistJoystick));
	//mControllerCollection.push_back(new MinimalistDrive(mRobotDrive));
	
	//mControllerCollection.push_back(new ArmController(mPneumaticArm, mLeftJoystick));
	//mControllerCollection.push_back(new ServoController(mServo));
	//mControllerCollection.push_back(new ElevatorController(mElevator, mTwistJoystick));
	
	mControllerCollection.push_back(new EncoderTestController(mRightEncoder));
	
	
	return;
}

/**
 * @brief Destroys all hardware, input devices, and software
 * created for the entire robot.
 * 
 * @warning This is frequently out-of-date.  
 */
PrototypeRobot::~PrototypeRobot(void)
{
	// Empty
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
void PrototypeRobot::Autonomous(void)
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
void PrototypeRobot::OperatorControl(void) 
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



