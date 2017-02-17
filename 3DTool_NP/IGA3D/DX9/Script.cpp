#include "stdafx.h"
CScript* GetScript()
{
	static CScript scr;
	return &scr;
}

//--------------------------------------------
CScript::CScript()
{
	m_bPyInit = false;
	m_pFunc = NULL;
	m_pModule = NULL;
}

CScript::~CScript()
{
	Destory();
}

bool CScript::Init()
{
	m_bPyInit = true;

	Py_Initialize();
	
	m_pModule = PyImport_ImportModule("t1");
	if( m_bPyInit && m_pModule == NULL )
	{
		m_bPyInit = false;
		OutputDebugString("Python: 没有找到模块 t1 ");
	}

	//找到脚本
	if( m_bPyInit )
	{
		//加载脚本函数
		m_pFunc = PyObject_GetAttrString( m_pModule, "fun1");
		if( m_pFunc == NULL )
		{
			m_bPyInit = false;
			OutputDebugString("Python: 没有在模块 t1 中找到 fun1");
		}
	
	}
	return true;
}

int CScript::Run()
{
	if( m_bPyInit )
	{
		PyObject* pRetVal = NULL;
		pRetVal = PyEval_CallObject( m_pFunc, NULL );
		
		//解析返回值
		int    iRetVal = 0;
		PyArg_Parse(pRetVal, "i", &iRetVal );

		return iRetVal;
		//char text[256];
		//sprintf(text, "Python: return %d", iRetVal );
		//OutputDebugString(text);
	}
}

void CScript::Destory()
{
	if( m_bPyInit )
	{
		Py_Finalize();
	}
}