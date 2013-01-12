
#include "timertest.h"

TimerTest::TimerTest() :
		BaseController(),
		mTimer()
{
	mTimer.Start();
}

TimerTest::~TimerTest()
{
	//Empty
}

void TimerTest::Run()
{
	SmartDashboard::GetInstance()->Log(mTimer.Get(), "(TIMER) time:");
}
