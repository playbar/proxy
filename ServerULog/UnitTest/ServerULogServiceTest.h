#pragma once

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class ServerULogServiceTest : public CppUnit::TestCase  
{
	CPPUNIT_TEST_SUITE(ServerULogServiceTest);		//声明一个测试包
	CPPUNIT_TEST(testFindGameIDAndFileName);					//声明一个测试用例

	CPPUNIT_TEST_SUITE_END();	
public:
	ServerULogServiceTest(void);
	virtual ~ServerULogServiceTest(void);

	void testFindGameIDAndFileName();

	static std::string GetSuiteName();
	static CppUnit::Test* GetSuite();
};
