#include "MotorLimitWatchdog.h"

/**
 * @brief Monitors status of arm motor, top limit switch, and bottom limit switch.
 * 
 * @param[in] watchdogName ???
 * 
 * @param[in] motor Pointer to the arm motor.
 * 
 * @param[in] topLimit Pointer to the top limit switch.
 * 
 * @param[in] bottomLimit Pointer to the bottom limit switch.
 */
MotorLimitWatchdog::MotorLimitWatchdog(
		const char * watchdogName,
		SpeedController * motor,
		DigitalInput * topLimit,
		DigitalInput * bottomLimit):
    Task("MotorLimitWatchDog", (FUNCPTR)MotorLimitWatchdog::TaskWrapper)
{
	mName = watchdogName;
	mMotor = motor;
	mTopLimit = topLimit;
	mBottomLimit = bottomLimit;

	initFieldNames();

	mStatusLogger = new Notifier((TimerEventHandler)MotorLimitWatchdog::LogStatus, this);
	mStatusLogger->StartPeriodic(1.0);

	Task::Start((UINT32)this);
}

/**
 * @brief Deconstructor for MotorLimitWatchdog class.
 */

MotorLimitWatchdog::~MotorLimitWatchdog()
{
	if (mStatusLogger)
	{
		mStatusLogger->Stop();
		delete mStatusLogger;
	}
        
}

/**
 * @brief Static wrapper function to callback non-static member function.
 */

void MotorLimitWatchdog::TaskWrapper(void* ThisObject)
{
	// explicit cast pointer to Class pointer
	MotorLimitWatchdog * myself = (MotorLimitWatchdog*) ThisObject;

	// call member function
	myself->Run();
}

/************************************************************************
 * The following should be moved once the bug in
 * SmartDashboard::AnnounceIfNecessary is fixed and replaced with the
 * original method commented out below.
 ***********************************************************************/

/**
 * @brief Logs status of arm motor, top limit switch, and bottom limit switch to SmartDashboard.
 * 
 * @param[in] obj Pointer to MotorLimitWatchdog instance.
 */

void MotorLimitWatchdog::LogStatus (MotorLimitWatchdog * obj)
{
	SmartDashboard::Log((bool)(obj->mTopLimit->Get()), obj->pTopLimitField);
	SmartDashboard::Log((bool)(obj->mBottomLimit->Get()), obj->pBottomLimitField);
	SmartDashboard::Log(obj->mMotor->Get(), obj->pMotorSpeedField);
}

/*
void MotorLimitWatchdog::LogStatus (MotorLimitWatchdog * obj)
{
        char * pStatusStr = new char[strlen(obj->m_name) + 32];
        char * pStatusText;
        
        pStatusText = strcpy (pStatusStr, obj->m_name);
        pStatusText += strlen(pStatusStr);
        strcat(pStatusText++, " ");
        
        strcpy(pStatusText, "High limit set:");
        SmartDashboard::Log((bool)(obj->m_highLimit->Get()), pStatusStr);

        strcpy(pStatusText, "Ligh limit set:");
        SmartDashboard::Log((bool)(obj->m_lowLimit->Get()), pStatusStr);

        strcpy(pStatusText, "Current motor speed:");
        SmartDashboard::Log(obj->m_motor->Get(), pStatusStr);

        delete pStatusStr;
}
*/

// todo Figure out what this does and document it.

void MotorLimitWatchdog::Run()
{
	bool bLimitSwitchHit;
	
	// Initialize the limit switch state
	if (mTopLimit->Get() || mBottomLimit->Get()) 
	{
		bLimitSwitchHit = true;
	} else {
		bLimitSwitchHit = false;
	}

	// So long as the task is still active, keep looping
	while(this->Verify())
	{
		// While one of the limit switches are pressed hang out here until
		//  one of the limit switches is released or the task ends
		while (bLimitSwitchHit && this->Verify())
		{
			if (!mTopLimit->Get() && !mBottomLimit->Get()) 
			{
				bLimitSwitchHit = false;
			}
		}

		// While neither of the limit switches is pressed hang out here until
		//  one of the limit switches is pressed or the task ends
		while (!bLimitSwitchHit && this->Verify())
		{
			if (mTopLimit->Get() || mBottomLimit->Get())
				{
					mMotor->Set(0);
					bLimitSwitchHit = true;
				}
		}
	}
}
