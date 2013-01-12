#include "sensors.h"

SimpleEncoderTest::SimpleEncoderTest(Encoder *leftEncoder, Encoder *rightEncoder) :
		BaseController()
{
	mLeftEncoder = leftEncoder;
	mRightEncoder = rightEncoder;
	mS = SmartDashboard::GetInstance();
	
	mS->Log(mLeftEncoder->GetRate(), "Left Encoder:");
	mS->Log(mRightEncoder->GetRate(), "Right Encoder:");
}

void SimpleEncoderTest::Run()
{
	mS->Log(mLeftEncoder->GetRate(), "Left Encoder:");
	mS->Log(mRightEncoder->GetRate(), "Right Encoder:");
}



/**
 * @brief Creates an instance of the rangefinder class.
 */
RangeFinder::RangeFinder(AnalogChannel *ultrasoundSensor) :
		BaseComponent()
{
	mUltrasoundSensor = ultrasoundSensor;
	return;
}

/**
 * @brief Finds the distance from the wall in inches.
 * 
 * @returns Returns the distance from the wall to the
 * ultrasound sensor in inches.
 */
float RangeFinder::FromWallInches(void)
{
	INT32 rawDistance = FromWallRaw();
	float inchesDistance = (float) rawDistance * 0.5;		// Based on experimental data.
	return inchesDistance;
}

/**
 * @brief Finds the raw distance from the wall.
 * 
 * @details
 * Prefer using RangeFinder::FromWallInches over this one.
 * 
 * @returns The raw distance returned by the ultrasonic sensor
 */
INT32 RangeFinder::FromWallRaw(void)
{
	return mUltrasoundSensor->GetAverageValue();
}

/**
 * @brief Tests to see if the robot is positioned roughly in
 * the key.
 * 
 * @details
 * It accomplishes this by measuring the distance from
 * the wall and seeing if it falls within the distance
 * the key is from the wall.
 * 
 * The min and max distances permitted are 
 * RangeFinder::kWallDistanceMin and RangeFinder::kWallDistanceMax.
 * 
 * @warning
 * This may be inaccurate (the robot could be off at a
 * wild angle).
 * 
 * @returns Returns true if the robot is within a certain distance from the wall.
 * (...which means this should return true whenever the robot is in the key) 
 */
bool RangeFinder::IsInShootingRange(void)
{
	float inchesDistance = FromWallInches();
	bool isInRange = false;
	if ((kWallDistanceMin <= inchesDistance) and (inchesDistance <= kWallDistanceMax)) {
		isInRange = true;
	}
	return isInRange;
}


/**
 * @brief Creates an instance of the RangeFinderTest class.
 */
RangeFinderTest::RangeFinderTest(RangeFinder *rangeFinder) :
		BaseController()
{
	mRangeFinder = rangeFinder;
}

/**
 * @brief Prints the distance in inches from the ultrasound 
 * sensor to the SmartDashboard.
 */
void RangeFinderTest::Run(void)
{
	SmartDashboard::GetInstance()->Log(mRangeFinder->FromWallInches(), "(ULTRASOUND) Distance ");
}


/**
 * @brief Creates an instance of this class.
 * 
 * @param[in] gyro A pointer to a gyro
 */
GyroTest::GyroTest(Gyro *gyro) :
		BaseController()
{
	mGyro = gyro;
	mGyro->Reset();
}

/**
 * @brief Prints the value of the gyro to the SmartDashboard
 * in degrees.
 */
void GyroTest::Run()
{
	SmartDashboard::GetInstance()->Log(mGyro->GetAngle(), "(GYRO) Rotation ");
}


/**
 * @brief Creates an instance of the class.
 */
DigitalInputTestController::DigitalInputTestController(DigitalInput *digitalInput)
{
	mDigitalInput = digitalInput;
}

/**
 * @brief Prints the value of the digital input to the
 * SmartDashboard.
 */
void DigitalInputTestController::Run()
{
	SmartDashboard::GetInstance()->Log((bool) mDigitalInput->Get(), "DigitalInputTestController");
}
