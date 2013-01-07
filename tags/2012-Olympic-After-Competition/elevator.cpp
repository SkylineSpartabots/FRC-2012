#include "elevator.h"

/**
 * @brief Creates an instance of this class.
 * 
 * @param[in] bottomLimitSwitch Pointer to the bottom limit switch.
 * @param[in] topLimitSwitch Pointer to the top limit switch.
 * @param[in] speedController Pointer to the elevator speed controller.
 */
Elevator::Elevator(DigitalInput *bottomLimitSwitch, SpeedController *speedController)
{
	mBottomLimitSwitch = bottomLimitSwitch;
	//mTopLimitSwitch = topLimitSwitch;
	mSpeedController = speedController;
}

Elevator::~Elevator()
{
	// None
}

/*
 * @brief Checks to see if there is a ball
 * in the bottom of the elevator.
 */
bool Elevator::IsBallAtBottom(void) {
	return (bool) mBottomLimitSwitch->Get();
}

/**
 * @brief Checks to see if there is a ball
 * at the top of the elevator.
 */
//bool Elevator::IsBallAtTop(void) {
	//return (bool) mTopLimitSwitch->Get();
//}

/**
 * @brief Makes the elevator stop.
 */
void Elevator::Stop(void) {
	mSpeedController->Set(0.0);
}

/**
 * @brief Makes the elevator move up.
 */
void Elevator::MoveUp(void) {
	mSpeedController->Set(kDefaultSpeed);
}

/**
 * @brief Makes the elevator move down.
 */
void Elevator::MoveDown(void)
{
	mSpeedController->Set(kDefaultSpeed * -1.0);
}


/**
 * @brief Makes an instance of this class.
 * 
 * @param[in] elevator Pointer to Elevator object.
 * @param[in] joystick Pointer to joystick.
 */
ElevatorController::ElevatorController(Elevator *elevator, Joystick *joystick)
{
	mElevator = elevator;
	mJoystick = joystick;
}

/**
 * @brief Provides a thin layer to control the elevator
 * using a joystick.
 * 
 * @warning 
 * Inaccurate details.
 * 
 * @details
 * Button 8 causes the conveyor belt to move the ball up
 * until it reaches the top. The driver must press button 9
 * to continue moving the conveyor belt, i.e. load the ball
 * into the shooter. 
 * 
 * A limit switch at the middle of the conveyor belt 
 * tells the driver if there is a ball at the middle 
 * of the conveyor belt; this way, the driver has room 
 * to decide to pick up another ball or continue
 * moving the belt (and shoot the ball).
 */
void ElevatorController::Run(void) {
	bool moveUp = mJoystick->GetRawButton(10);
	bool moveDown = mJoystick->GetRawButton(12);
	
	if (moveUp and moveDown) {
		mElevator->Stop();
	} else if (moveUp) {
		mElevator->MoveUp();
	} else if (moveDown) {
		mElevator->MoveDown();
	} else {
		mElevator->Stop();
	}
	
	//SmartDashboard::GetInstance()->Log(mElevator->IsBallAtTop(), "(ELEVATOR) Ball at top ");
    SmartDashboard::GetInstance()->Log(mElevator->IsBallAtBottom(), "(ELEVATOR) Ball at bottom ");
}
