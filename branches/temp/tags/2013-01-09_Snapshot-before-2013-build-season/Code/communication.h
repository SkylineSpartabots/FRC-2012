#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <string>
#include "WPILib.h"
#include "Definitions/components.h"

class TableTest : public BaseController
{
protected:
	NetworkTable *mTable;
	
public:
	TableTest();
	void Run();
};


#endif
