#include "TextureCoreTest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CTextureCoreTest, CTextureCoreTest::GetSuiteName());

CTextureCoreTest::CTextureCoreTest(void)
{
}

CTextureCoreTest::~CTextureCoreTest(void)
{
}

void CTextureCoreTest::TestTexture()
{
	return;
}


std::string CTextureCoreTest::GetSuiteName()
{
	return "CTextureCoreTest";
}


CppUnit::Test* CTextureCoreTest::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CTextureCoreTest::GetSuiteName());
	return reg.makeTest();
}
