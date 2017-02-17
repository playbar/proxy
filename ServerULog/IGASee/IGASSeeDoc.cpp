// IGASSeeDoc.cpp :  CIGASSeeDoc 类的实现
//

#include "stdafx.h"
#include "IGASSee.h"

#include "IGASSeeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIGASSeeDoc

IMPLEMENT_DYNCREATE(CIGASSeeDoc, CDocument)

BEGIN_MESSAGE_MAP(CIGASSeeDoc, CDocument)
END_MESSAGE_MAP()


// CIGASSeeDoc 构造/析构

CIGASSeeDoc::CIGASSeeDoc()
{
	// TODO: 在此添加一次性构造代码

}

CIGASSeeDoc::~CIGASSeeDoc()
{
}

BOOL CIGASSeeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CIGASSeeDoc 序列化

void CIGASSeeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CIGASSeeDoc 诊断

#ifdef _DEBUG
void CIGASSeeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIGASSeeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIGASSeeDoc 命令
