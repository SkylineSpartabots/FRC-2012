#ifndef _TOOLS_H
#define _TOOLS_H

// Standard libraries
#include <string>
#include <sstream>

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
	float Min(int, float);
	float Max(int, float);
	float StringToFloat(std::string);
	float StringToFloat(const char *);
	std::string FloatToString(float);
}


#endif
