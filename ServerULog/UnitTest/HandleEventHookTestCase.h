// MinusTestCase.h: interface for the CMinusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_)
#define AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CHandleEventHookTestCase : public CppUnit::TestCase
{
	//通过这几个宏，我们就把CMinusTestCase和testMinus注册到了测试列表当中.
	CPPUNIT_TEST_SUITE(CHandleEventHookTestCase);			//声明一个测试包
    CPPUNIT_TEST(testMinus);					//声明一个测试用例
	CPPUNIT_TEST(testHandleEventHook);
    CPPUNIT_TEST_SUITE_END();	
public:
	CHandleEventHookTestCase();
	virtual ~CHandleEventHookTestCase();

	void testMinus(); //测试方法
	void testHandleEventHook();
	

	static std::string GetSuiteName();
	//写一个注册函数, 使其在运行期生成一个Test
	static CppUnit::Test* GetSuite();
};

#endif // !defined(AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_)
