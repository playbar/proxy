
#include "C3D_CoreTest.h"
#include "3D_Core.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(C3D_CoreTest, C3D_CoreTest::GetSuiteName());
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3D_CoreTest::C3D_CoreTest()
{

}

C3D_CoreTest::~C3D_CoreTest()
{
}

void C3D_CoreTest::TestReLoad()
{
	IGA3DModule iga3D;
	iga3D.ReLoad();
	CPPUNIT_ASSERT_EQUAL(true, false);
	return;
}

std::string C3D_CoreTest::GetSuiteName()
{
	return "C3D_CoreTest";
}


CppUnit::Test* C3D_CoreTest::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(C3D_CoreTest::GetSuiteName());
	return reg.makeTest();
}

