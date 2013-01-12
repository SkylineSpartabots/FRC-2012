/**
 * @file components.h
 *  
 * @brief Contains base classes, typedefs for ports, and useful
 * tools.
 * 
 * @details
 * In general, this file is meant to contain useful snippits of
 * code meant to be used all over the program.  
 * 
 * @section aboutBaseClasses About Base Classes
 * 
 * There are two base classes that all objects in this program
 * should probably subclass -- BaseComponent and BaseController.
 * There are four tiers of objects used in this program:
 *   - WPILib objects
 *   - Components
 *   - Controllers
 *   - MainRobot
 * 
 * WPILib objects are ones provided by FIRST -- they interface
 * directly with the hardware.
 * 
 * Components are classes we make that control specific 
 * pieces of the robot -- the shooter and the elevator
 * are examples of components.
 * 
 * Controllers are classes that take in user input and control
 * both components and WPILib objects.  They don't directly
 * control any part of the robot, but is instead a thin layer
 * that translates user input (for example, a joystick) and
 * feeds that data into a component class.
 * 
 * MainRobot is a singleton class that bundles everything
 * together.
 */

#ifndef COMPONENTS_H_
#define COMPONENTS_H_

// Standard libraries
#include <assert.h>

/**
 * @brief A base class meant to be inherited from any
 * controller class.
 * 
 * @details
 * Any class that controls components by using user input
 * should be a subclass of this.
 * 
 * All subclasses need to provide a 'Run' method -- the
 * Run method cannot contain any excessively long loops
 * or waiting periods.  During the normal operation of
 * a robot, multiple Run methods from different classes
 * will be repeatedly called in a loop until the robot is 
 * disabled.
 */
class BaseController
{
public:
	BaseController();
	virtual void Run() = 0;
};

/**
 * @brief A base class meant to be inherited from any
 * component class.
 * 
 * @details
 * Any class that controls a specific piece of the robot
 * should be a subclass of this.
 * 
 * Inheriting this class adds no extra functionality 
 * for now -- it's simply a method to help organize
 * the code.
 * 
 * Additional functionality may be provided in the
 * future.
 */
class BaseComponent
{
public:
	BaseComponent();
};


/**
 * @brief a series of typedefs to map the ports on the 
 * robot to names.
 * 
 * @details
 * There are several types of ports on the robot:
 *   - PWM ports
 *   - Digital Input/Output
 *   - Analog Breakout
 * 
 * The PWM ports are used to send data (and sometimes
 * power) to motors.
 * 
 * Digital IO ports are used to send and recieve data
 * from sensors.
 * 
 * Analog Breakout ports are also used to send and
 * recieve data from sensors.
 * 
 * This namespace also provides two more typedefs:
 * Modules and USB ports.  Modules are specific
 * chunks of the cRIO.  USB ports the ones on the
 * computer that joysticks plug into.
 * 
 * To use a port, use the double-colon syntax:
 * @code
 * Ports::Pwm1
 * Ports::Pwm2
 * @endcode
 * 
 * For example, to declare a Jaguar, you might do...
 * @code
 * Jaguar *j = new Jaguar(Ports::Pwm2);
 * @endcode
 */
namespace Ports
{
	/**
	 * @brief Used frequently for speed-controllers (including Jaguars).
	 * 
	 * @details
	 * Known uses:
	 *   - Jaguars for the robot drive
	 *   - Other speed controllers for motors
	 *   
	 * Found on the digital sidecar.
	 */
	enum Pwm
	{
		Pwm1 = 1,
		Pwm2 = 2,
		Pwm3 = 3,
		Pwm4 = 4,
		Pwm5 = 5,
		Pwm6 = 6,
		Pwm7 = 7,
		Pwm8 = 8,
		Pwm9 = 9,
		Pwm10 = 10
	};
	
	/**
	 * @brief Used frequently for sensors.  When used with a jumper, can also
	 * be used to power servos.
	 * 
	 * @details
	 * Known uses:
	 *   - Limit switches
	 *   - Servos
	 *   - Accelerometer/Gyro (?)
	 * 
	 * Found on the digital sidecar.
	 */
	enum DigitalIo
	{
		DigitalIo1 = 1,
		DigitalIo2 = 2,
		DigitalIo3 = 3,
		DigitalIo4 = 4,
		DigitalIo5 = 5,
		DigitalIo6 = 6,
		DigitalIo7 = 7,
		DigitalIo8 = 8,
		DigitalIo9 = 9,
		DigitalIo10 = 10,
		DigitalIo11 = 11,
		DigitalIo12 = 12,
		DigitalIo13 = 13,
		DigitalIo14 = 14
	};	
	
	/**
	 * @brief Used mostly for joysticks (or other forms of input)
	 * 
	 * @details
	 * Known uses:
	 *   - Joysticks
	 * 
	 * Found on the USB dongle that attaches to the laptop
	 */
	enum Usb
	{
		Usb1 = 1,
		Usb2 = 2,
		Usb3 = 3,
		Usb4 = 4	// Not sure if we actually have a 4th one
	};
	
	/**
	 * @brief Used mostly for sensors
	 * 
	 * @details
	 * Known uses:
	 *   - Ultrasound sensor
	 * 
	 * Found on top of the first module in the cRIO.
	 */
	enum AnalogBreakout
	{
		AnalogChannel1 = 1,
		AnalogChannel2 = 2,
		AnalogChannel3 = 3,
		AnalogChannel4 = 4,
		AnalogChannel5 = 5,
		AnalogChannel6 = 6,
		AnalogChannel7 = 7,
		AnalogChannel8 = 8
	};
	
	/**
	 * @brief Sections of the cRIO
	 * 
	 * @details
	 * This is a relatively new concept, which is meant
	 * to deal with how some FRC teams have 4-slot cRIOS
	 * while others have 8-slot cRIOS.
	 * 
	 * Slot -- the physical space you can plug a module
	 * in.  Module number -- something FIRST invented.
	 * 
	 * For a 8-slot cRIO:
	 *   - Slot 1: Module 1 {Analog Module 9201)
	 *   - Slot 2: Module 1 {Digital Module 9403)
	 *   - Slot 3: Module 1 {Solenoid Module 9472)
	 *   - Slot 4: Module 2 {empty)
	 *   - Slot 5: Module 2 {Analog Module 9201)
	 *   - Slot 6: Module 2 {Digital Module 9403)
	 *   - Slot 7: Module 2 {Solenoid Module 9472)
	 *   - Slot 8: Module   {empty)
	 */
	enum Module
	{
		Module1 = 1,
		Module2 = 2
	};
}

/**
 * @brief A set of useful functions that are used frequently
 * throughout the code.
 * 
 * @details
 * Use the double-colon syntax to access functions:
 * @code
 * float output = Tools::Coerce(3,0,10,0,100);
 * @endcode
 */
namespace Tools
{
double Coerce(double, double, double, double, double);
double Limit(double, double, double);
}

#endif
