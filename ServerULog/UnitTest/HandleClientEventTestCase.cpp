// PlusTestCase.cpp: implementation of the CPlusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitTest.h"
#include "HandleClientEventTestCase.h"
#include "ServerULog.h"
#include "XMLMarkup.h"
#include "SystemTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//注册一个测试suite到一个指定的TestFactory工厂中

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CHandleClientEventTestCase, CHandleClientEventTestCase::GetSuiteName());
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHandleClientEventTestCase::CHandleClientEventTestCase()
{

}

CHandleClientEventTestCase::~CHandleClientEventTestCase()
{

}

void CHandleClientEventTestCase::testAdd()
{
	int iResult = HandleClientEvent(NULL);
	CPPUNIT_ASSERT_EQUAL(30, iResult); //检查结果是否等于30	
}

void CHandleClientEventTestCase::testHandleClientEvent()
{
	int iResult = HandleClientEvent(NULL);
	CPPUNIT_ASSERT_EQUAL(2, iResult); //检查结果是否等于30	
	CPPUNIT_ASSERT_EQUAL(2, 1);
}

void CHandleClientEventTestCase::testCreateXml()
{
	CString strID = _T("");
	CString strPwd = _T("");
	CXMLMakeUp xml;
	xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	xml.AddElem("UserInfo");
	xml.IntoElem();
	for (int i=0; i<10; i++)
	{
		//产生一个随机数，作为权限
		srand(UINT(time(NULL)+i));
		int nLevel = rand()%256;
		strID.Format("UserID%02d",i);
		strPwd.Format("UserPwd%02d",i);
		xml.AddElem("User");
		xml.AddAttrib("UserID",strID);
		xml.AddAttrib("UserPwd",strPwd);
		xml.AddAttrib("UserLevel",nLevel);      
	}
	xml.OutOfElem();
	bool bSuccess = xml.Save("UserInfo1.xml");
	CPPUNIT_ASSERT_EQUAL(true, bSuccess);

}

void CHandleClientEventTestCase::testReadXML()
{
	CXMLMakeUp xml;
	bool bLoad = false;
	bool bFind = false;
	bLoad = xml.Load("UserInfo1.xml");
	if (bLoad)
	{
		CString strID;
		CString strPwd;
		xml.ResetMainPos();
		xml.FindElem();    //UserInfo
		while (xml.FindChildElem("User"))
		{
			strID = xml.GetChildAttrib("UserID");
			strPwd = xml.GetChildAttrib("UserPwd");
			TRACE("\n----id:%s,pwd:%s-------\n",strID,strPwd);
			if (strID=="UserID00"&&strPwd=="UserPwd00")
			{
				bFind = true;
				break;
			}
		}
	}
	CPPUNIT_ASSERT_EQUAL(true, bFind);
}

void CHandleClientEventTestCase::testXmlLoad()
{
	bool bSuccess = false;
	CXMLMakeUp xml;
	BYTE  byCode[]=
	{
		0x3C,0x3F,0x78,0x6D,0x6C,0x20,0x76,0x65,0x72,0x73,0x69,0x6F,0x6E,0x3D,0x22,
		0x31,0x2E,0x30,0x22,0x20,0x65,0x6E,0x63,0x6F,0x64,0x69,0x6E,0x67,0x3D,0x22,0x55,
		0x54,0x46,0x2D,0x38,0x22,0x3F,0x3E,0x3C,0x55,0x73,0x65,0x72,0x49,0x6E,0x66,0x6F,
		0x3E,0x0D,0x0A,0x3C,0x55,0x73,0x65,0x72,0x20,0x55,0x73,0x65,0x72,0x49,0x44,0x3D,
		0x22,0x55,0x73,0x65,0x72,0x49,0x44,0x30,0x30,0x22,0x20,0x55,0x73,0x65,0x72,0x50,0x77,
		0x64,0x3D,0x22,0x55,0x73,0x65,0x72,0x50,0x77,0x64,0x30,0x30,0x22,0x20,0x55,0x73,
		0x65,0x72,0x4C,0x65,0x76,0x65,0x6C,0x3D,0x22,0x31,0x30,0x30,0x22,0x2F,0x3E,0x0D,
		0x0A,0x3C,0x2F,0x55,0x73,0x65,0x72,0x49,0x6E,0x66,0x6F,0x3E,0x0D,0x0A,0x00
	};
	int i = sizeof(byCode);
	bSuccess = xml.Load(byCode, i);
	CPPUNIT_ASSERT_EQUAL( true, bSuccess); 
}

void CHandleClientEventTestCase::testGetTodayDate()
{
	CSystemTime st;
	TCHAR szLogFile[MAX_PATH];
	st.GetTodayDate(szLogFile, sizeof(szLogFile)/ sizeof(szLogFile[0]));
	CPPUNIT_ASSERT_EQUAL(0,0);
}
void CHandleClientEventTestCase::testGetYesterDayDate()
{

}

std::string CHandleClientEventTestCase::GetSuiteName()
{
	return "HandleClientEvent";
}

/*
 *	注意：CPlusTestCase::GetSuite()返回一个指向CppUnit::Test的指针.
 *  这个指针就是整个测试的起点。
 *  CppUnit::TestFactoryRegistry::getRegistry()根据TestSuite的名字返回TestFactoryRegistry工厂,
 *  然后调用工厂里的makeTest()对TestSuite进行组装，将建立起一个树状的测试结构。
 */

CppUnit::Test* CHandleClientEventTestCase::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CHandleClientEventTestCase::GetSuiteName());
	return reg.makeTest();
}
