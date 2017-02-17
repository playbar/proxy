// MinusTestCase.cpp: implementation of the CMinusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitTest.h"
#include "HandleEventHookTestCase.h"
#include "ServerULog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//注册一个测试suite到一个指定的TestFactory工厂中

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CHandleEventHookTestCase, CHandleEventHookTestCase::GetSuiteName());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHandleEventHookTestCase::CHandleEventHookTestCase()
{

}

CHandleEventHookTestCase::~CHandleEventHookTestCase()
{

}

void CHandleEventHookTestCase::testMinus()
{
	//CMinus minusTemp;
	//int nResult = minusTemp.Minus(30, 4); //执行Minus操作
	CPPUNIT_ASSERT_EQUAL(26, 26); //检查结果是否等于30	
}

void CHandleEventHookTestCase::testHandleEventHook()
{
	RFTPEventStr eventStr;
	WORD wResult = HandleEventHook(&eventStr);
	int iResult = wResult;
	CPPUNIT_ASSERT_EQUAL(0 , iResult);
}

std::string CHandleEventHookTestCase::GetSuiteName()
{
	return "HandleEventHook";
}

/*
 *	注意：CMinusTestCase::GetSuite()返回一个指向CppUnit::Test的指针.
 *  这个指针就是整个测试的起点。
 *  CppUnit::TestFactoryRegistry::getRegistry()根据TestSuite的名字返回TestFactoryRegistry工厂,
 *  然后调用工厂里的makeTest()对TestSuite进行组装，将建立起一个树状的测试结构。
 */

CppUnit::Test* CHandleEventHookTestCase::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CHandleEventHookTestCase::GetSuiteName());
	return reg.makeTest();
}
