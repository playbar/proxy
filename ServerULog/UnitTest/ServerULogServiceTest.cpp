#include "stdafx.h"
#include "ServerULogServiceTest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServerULogServiceTest, ServerULogServiceTest::GetSuiteName());


ServerULogServiceTest::ServerULogServiceTest(void)
{
}

ServerULogServiceTest::~ServerULogServiceTest(void)
{
}

void ServerULogServiceTest::testFindGameIDAndFileName()
{

}

std::string ServerULogServiceTest::GetSuiteName()
{
	return "ServerULogServiceTest";
}

/*
*	注意：CPlusTestCase::GetSuite()返回一个指向CppUnit::Test的指针.
*  这个指针就是整个测试的起点。
*  CppUnit::TestFactoryRegistry::getRegistry()根据TestSuite的名字返回TestFactoryRegistry工厂,
*  然后调用工厂里的makeTest()对TestSuite进行组装，将建立起一个树状的测试结构。
*/

CppUnit::Test* ServerULogServiceTest::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(ServerULogServiceTest::GetSuiteName());
	return reg.makeTest();
}
