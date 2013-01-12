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


#endif
