/**
 * @brief Monitors status of arm motor, top limit switch, and bottom limit switch.
 */

#ifndef MOTOR_LIMIT_WATCHDOG_H_
#define MOTOR_LIMIT_WATCHDOG_H_

#include "WPILib.h"

class MotorLimitWatchdog : public Task
{
	const char * mName;
	SpeedController * mMotor;
	DigitalInput * mTopLimit;
	DigitalInput * mBottomLimit;
	Notifier * mStatusLogger;

public:
	MotorLimitWatchdog(
		const char * watchdogName,
		SpeedController * motor,
		DigitalInput * highLimit,
		DigitalInput * lowLimit);
	~MotorLimitWatchdog();

protected:
	static void TaskWrapper (void *);
	//static void LogStatus (MotorLimitWatchdog * watchdog);

	void Run();

/************************************************************************
* The following should be moved once the bug in
* SmartDashboard::AnnounceIfNecessary is fixed.
***********************************************************************/
private:
	void initFieldNames()
	{
		pTopLimitField = new char[strlen(mName) + 20];
		strcpy (pTopLimitField, mName);
		strcat (pTopLimitField, " High limit set:");
	
		pBottomLimitField = new char[strlen(mName) + 20];
		strcpy (pBottomLimitField, mName);
		strcat (pBottomLimitField, " Ligh limit set:");
	
		pMotorSpeedField = new char[strlen(mName) + 25];
		strcpy (pMotorSpeedField, mName);
		strcat (pMotorSpeedField, " Current motor speed:");
	};
	char * pTopLimitField;
	char * pBottomLimitField;
	char * pMotorSpeedField;

};




class SingleMotorLimitWatchdog : public Task
{
	const char * mName;
	SpeedController * mMotor;
	DigitalInput * mLimit;
	Notifier * mStatusLogger;

public:
	SingleMotorLimitWatchdog(
		const char * watchdogName,
		SpeedController * motor,
		DigitalInput * limit);
	~SingleMotorLimitWatchdog();

protected:
	static void TaskWrapper (void *);
	//static void LogStatus (MotorLimitWatchdog * watchdog);

	void Run();

/************************************************************************
* The following should be moved once the bug in
* SmartDashboard::AnnounceIfNecessary is fixed.
***********************************************************************/
private:
	void initFieldNames()
	{
		pLimitField = new char[strlen(mName) + 20];
		strcpy (pLimitField, mName);
		strcat (pLimitField, " High limit set:");
		
		pMotorSpeedField = new char[strlen(mName) + 25];
		strcpy (pMotorSpeedField, mName);
		strcat (pMotorSpeedField, " Current motor speed:");
	};
	char * pLimitField;
	char * pMotorSpeedField;

};

#endif // MOTOR_LIMIT_WATCHDOG_H_
