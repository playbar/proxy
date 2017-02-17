// PlusTestCase.h: interface for the CPlusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_)
#define AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CHandleClientEventTestCase : public CppUnit::TestCase  
{
	//通过这几个宏，我们就把CPlusTestCase和testAdd注册到了测试列表当中.
	CPPUNIT_TEST_SUITE(CHandleClientEventTestCase);		//声明一个测试包
    CPPUNIT_TEST(testAdd);					//声明一个测试用例
	CPPUNIT_TEST(testHandleClientEvent);
	CPPUNIT_TEST(testCreateXml);
	CPPUNIT_TEST(testReadXML);
	CPPUNIT_TEST(testXmlLoad);
	CPPUNIT_TEST(testGetTodayDate);
	CPPUNIT_TEST(testGetYesterDayDate);
    CPPUNIT_TEST_SUITE_END();	
public:
	CHandleClientEventTestCase();
	virtual ~CHandleClientEventTestCase();
	void testAdd(); //测试方法
	void testHandleClientEvent();
	void testCreateXml();
	void testReadXML();
	void testXmlLoad();
	void testGetTodayDate();
	void testGetYesterDayDate();

	static std::string GetSuiteName();

	//写一个注册函数, 使其在运行期生成一个Test
	static CppUnit::Test* GetSuite();
};

#endif // !defined(AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_)
