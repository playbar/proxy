#include "stdafx.h"
#include "SQLTestCase.h"
#include "CSQLConnection.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CSQLTestCase, CSQLTestCase::GetSuiteName());

CSQLTestCase::CSQLTestCase(void)
{
}

CSQLTestCase::~CSQLTestCase(void)
{
}

void CSQLTestCase::testSqlConntion()
{

}


std::string CSQLTestCase::GetSuiteName()
{
	//CSQLConnection	  SQLOperate;
	//SQLOperate.Connect("serverULog","192.168.1.72","serveru","serveru");

	return "CSQLTest";
}

/*
*	注意：CPlusTestCase::GetSuite()返回一个指向CppUnit::Test的指针.
*  这个指针就是整个测试的起点。
*  CppUnit::TestFactoryRegistry::getRegistry()根据TestSuite的名字返回TestFactoryRegistry工厂,
*  然后调用工厂里的makeTest()对TestSuite进行组装，将建立起一个树状的测试结构。
*/

CppUnit::Test* CSQLTestCase::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CSQLTestCase::GetSuiteName());
	return reg.makeTest();
}