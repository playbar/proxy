#pragma once

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class C3D_CoreTest : public CppUnit::TestCase  
{
	CPPUNIT_TEST_SUITE(C3D_CoreTest);		
	CPPUNIT_TEST(TestReLoad);
	//CPPUNIT_TEST(testCreateXml);
	//CPPUNIT_TEST(testReadXML);
	//CPPUNIT_TEST(testXmlLoad);
	//CPPUNIT_TEST(testGetTodayDate);
	//CPPUNIT_TEST(testGetYesterDayDate);

	CPPUNIT_TEST_SUITE_END();	

public:
	C3D_CoreTest(void);
	~C3D_CoreTest(void);
	void TestReLoad(void);

	static std::string GetSuiteName();
	static CppUnit::Test* GetSuite();
};
