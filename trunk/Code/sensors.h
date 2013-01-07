/**
 * @file sensors.h
 * 
 * @brief A collection of component classes that combine provides 
 * extra functionality by using sensors.
 */

#ifndef SENSORS_H_
#define SENSORS_H_

// 3rd party libraries
#include "WPILib.h"

// Program modules
#include "Definitions/components.h"

/**
 * @brief Reports the left and right encoder values
 * 
 * @warning
 * Relies on the encoders being calibrated before being passed in.
 * The encoders must also be started before being passed in.
 */
class SimpleEncoderTest : public BaseController
{
protected:
	Encoder *mLeftEncoder;
	Encoder *mRightEncoder;
	SmartDashboard *mS;
	
public:
	SimpleEncoderTest(Encoder*, Encoder*);
	void Run();
};


/**
 * @brief Uses the rangefinder to find the distance from the 
 * wall.
 * 
 * @details
 * This is best used when facing the wall with the hoops.
 * 
 * @warning
 * The ultrasound sensor this uses is somewhat inaccurate,
 * therefore the values returned by this class will also
 * jump around.
 */
class RangeFinder : public BaseComponent
{
protected:
	AnalogChannel *mUltrasoundSensor;
	static const INT32 kWallDistanceMin = 140;	// In inches.
	static const INT32 kWallDistanceMax = 200;	// In inches.
	
public:
	RangeFinder(AnalogChannel *);
	float FromWallInches(void);
	INT32 FromWallRaw(void);
	bool IsInShootingRange(void);
};

/**
 * @brief A thin layer to print data from the Rangefinder
 * to the SmartDashboard.
 */
class RangeFinderTest : public BaseController
{
protected:
	RangeFinder *mRangeFinder;
	
public:
	RangeFinderTest(RangeFinder *);
	void Run(void);
};

/**
 * @brief A thin layer to print data from the WPILib
 * Gyro to the SmartDashboard.
 */
class GyroTest : public BaseController
{
protected:
	Gyro *mGyro;
	
public:
	GyroTest(Gyro *);
	void Run(void);
};

/**
 * @brief A thin layer to test input and output 
 * from a limit switch or other DigitalIO
 * device.
 */
class DigitalInputTestController : public BaseController
{
protected:
	DigitalInput * mDigitalInput;
	
public:
	DigitalInputTestController(DigitalInput *);
	void Run();
};

#endif
