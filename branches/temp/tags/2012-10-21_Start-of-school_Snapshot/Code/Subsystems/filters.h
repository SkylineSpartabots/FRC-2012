#ifndef _FILTERS_H
#define _FILTERS_H

// System libraries
#include <utility.h>
#include <math.h>
#include <algorithm>

#include "../Definitions/Components.h"
#include "../tools.h"

struct DriveSpeed
{
	DriveSpeed();
	DriveSpeed(float, float);
	float Left;
	float Right;
};

namespace Filter
{
	struct DriveSpeed SquareInput(struct DriveSpeed);
	struct DriveSpeed ReverseDirection(struct DriveSpeed);
	
	struct DriveSpeed AddSpeedFactor(struct DriveSpeed, float);
	struct DriveSpeed Straighten(struct DriveSpeed);
	struct DriveSpeed AddTruncation(struct DriveSpeed);
	
	float truncate(float);
}

#endif
