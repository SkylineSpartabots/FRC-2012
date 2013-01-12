/**
 * @file testing.h
 * 
 * @brief Contains experimental code which doesn't fit cleanly into other files.
 * 
 * @warning Unless otherwise noted, code in this file should be regarded as 
 * potentially buggy or broken.
 */

#ifndef _TESTING_H
#define _TESTING_H

#include "WPILib.h"
#include "Definitions/components.h"
#include "tools.h"

/**
 * @brief A experimental class used to control a servo.
 * 
 * @summary Uses the SmartDashboard to set the servo values. 
 * 
 * @warning This class does not require a Component -- 
 * although this is a controller, it manipulates the 
 * servo directly.  
 * 
 * @attention This class thus far is bug-free.
 */
class ServoController : public BaseController
{
protected:
	Servo *mServo;
	
public:
	ServoController(Servo *);
	void Run();
};

#endif
