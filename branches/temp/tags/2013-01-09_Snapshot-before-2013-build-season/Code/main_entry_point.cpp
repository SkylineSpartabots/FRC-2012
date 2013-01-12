/**
 * @file main_entry_point.cpp
 * 
 * @brief The main entry point of the program.
 * 
 * @summary We can select at compile-time the robot class we want to run.
 */

#include "Profiles/MainRobot.h"
#include "Profiles/PrototypeRobot.h"
#include "Profiles/SidewaysRobot.h"

#define PROTOTYPE 1
#define MAINROBOT 2
#define SIDEWAYSROBOT 3
/// Do not modify above

// Pick which robot you want to run here.
// Your choices:
//
// #define ROBOT MAINROBOT 
// ...to run the main robot.
//
// #define ROBOT PROTOTYPE
// ...to run the prototype robot.
//
// Pick which one you want and replace the line
// below.

//#define ROBOT MAINROBOT
//#define ROBOT PROTOTYPE
#define ROBOT SIDEWAYSROBOT

/// Do not modify below

#if ROBOT == MAINROBOT
	START_ROBOT_CLASS(MainRobot);
#elif ROBOT == PROTOTYPE
	START_ROBOT_CLASS(PrototypeRobot);
#elif ROBOT == SIDEWAYSROBOT
	START_ROBOT_CLASS(SidewaysRobot);
#endif

