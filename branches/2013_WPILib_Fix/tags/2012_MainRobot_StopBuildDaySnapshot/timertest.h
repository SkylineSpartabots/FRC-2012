#ifndef TIMERTEST_H_
#define TIMERTEST_H_

#include "WPILib.h"
#include "components.h"

class TimerTest : public BaseController {
protected:
	Timer mTimer;
	
public:
	TimerTest();
	virtual ~TimerTest();
	void Run();
};

#endif
