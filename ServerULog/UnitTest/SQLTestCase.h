#pragma once

#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CSQLTestCase : public CppUnit::TestCase  
{
	CPPUNIT_TEST_SUITE(CSQLTestCase);		
	CPPUNIT_TEST(testSqlConntion);
	CPPUNIT_TEST_SUITE_END();	

public:
	CSQLTestCase(void);
	virtual ~CSQLTestCase(void);

	void testSqlConntion();
	

	static std::string GetSuiteName();
	//写一个注册函数, 使其在运行期生成一个Test
	static CppUnit::Test* GetSuite();
};
