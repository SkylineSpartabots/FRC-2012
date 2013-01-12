#include "filters.h"

DriveSpeed::DriveSpeed()
{
	Left = 0.0;
	Right = 0.0;
}

/**
 * @param[in] left The speed of the left side of the robot.
 * @param[in] right The speed of the right side of the robot.
 */
DriveSpeed::DriveSpeed(float left, float right)
{
	Left = left;
	Right = right;
}

/**
 * @brief Squares the input of both sides to allow more
 * sensitive driving when the joystick is pushed less.
 */
DriveSpeed Filter::SquareInput(struct DriveSpeed ds)
{
	DriveSpeed new_ds = DriveSpeed();
	new_ds.Left = (ds.Left < 0) ? (-ds.Left * ds.Left) : (ds.Left * ds.Left);
	new_ds.Right = (ds.Right < 0) ? (-ds.Right * ds.Right) : (ds.Right * ds.Right);
	return new_ds;
}

DriveSpeed Filter::ReverseDirection(struct DriveSpeed ds)
{
	DriveSpeed new_ds = DriveSpeed();
	new_ds.Left = -ds.Right;
	new_ds.Right = -ds.Left;
	return new_ds;
}

DriveSpeed Filter::AddSpeedFactor(struct DriveSpeed ds, float speedFactor) 
{	
	DriveSpeed new_ds = DriveSpeed();
	new_ds.Left = speedFactor * ds.Left;
	new_ds.Right = speedFactor * ds.Right;
	return new_ds;
}

DriveSpeed Filter::Straighten(struct DriveSpeed ds)
{
	DriveSpeed new_ds = DriveSpeed();
	
	float average = (ds.Left + ds.Right) / 2;
	new_ds.Left = average;
	new_ds.Right = average;

	return new_ds;
}

DriveSpeed Filter::AddTruncation(struct DriveSpeed ds)
{
	DriveSpeed new_ds = DriveSpeed();
	
	new_ds.Left = truncate(ds.Left);
	new_ds.Right = truncate(ds.Right);
	
	return new_ds;
}

float Filter::truncate(float value)
{
	float deadzone = 0.05;
	
	if (fabs(value) > deadzone) {
		float desiredMin = 0.3;
		float negativeFactor = (value < 0) ? -1.0 : 1.0;
		value = Tools::Coerce(
				fabs(value),
				deadzone,	// Actual min value
				1.0,		// Actual max value
				desiredMin,	// Desired min value
				1.0);		// Desire max value
		value *= negativeFactor;
	}
	return value;
}
