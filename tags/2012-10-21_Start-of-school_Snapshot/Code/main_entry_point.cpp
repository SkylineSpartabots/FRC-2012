/**
 * @file main_entry_point.cpp
 * 
 * @brief The main entry point of the program.
 * 
 * @summary We can select at compile-time the robot class we want to run.
 */

#include "Profiles/MainRobot.h"
#include "Profiles/PrototypeRobot.h"

//#define PROTOTYPE

#ifndef PROTOTYPE
START_ROBOT_CLASS(MainRobot);
#endif

#ifdef PROTOTYPE
START_ROBOT_CLASS(PrototypeRobot);
#endif

