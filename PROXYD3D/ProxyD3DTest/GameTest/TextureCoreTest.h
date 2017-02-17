#pragma once

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CTextureCoreTest : public CppUnit::TestCase  
{
	CPPUNIT_TEST_SUITE(CTextureCoreTest);		
	CPPUNIT_TEST(TestTexture);

	CPPUNIT_TEST_SUITE_END();	

public:
	CTextureCoreTest(void);
	~CTextureCoreTest(void);

	void TestTexture(void);

	static std::string GetSuiteName();
	static CppUnit::Test* GetSuite();
};
