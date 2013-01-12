#include "communication.h"

TableTest::TableTest() :
		BaseController()
{
	mTable = NetworkTable::GetTable("testTable");
}

void TableTest::Run()
{
	mTable->BeginTransaction();
	int testInt = mTable->GetInt("testInt");
	std::string testString = mTable->GetString("testString");
	mTable->EndTransaction();
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->Log(testInt, "testInt");
	s->Log(testString.c_str(), "testString");
}
