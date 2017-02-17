// IGASSeeView.cpp : CIGASSeeView 类的实现
//

#include "stdafx.h"
#include "IGASSee.h"

#include "IGASSeeDoc.h"
#include "IGASSeeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIGASSeeView

IMPLEMENT_DYNCREATE(CIGASSeeView, CView)

BEGIN_MESSAGE_MAP(CIGASSeeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIGASSeeView 构造/析构

CIGASSeeView::CIGASSeeView()
{
	// TODO: 在此处添加构造代码

}

CIGASSeeView::~CIGASSeeView()
{
}

BOOL CIGASSeeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}

// CIGASSeeView 绘制

void CIGASSeeView::OnDraw(CDC* /*pDC*/)
{
	CIGASSeeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CIGASSeeView 打印

BOOL CIGASSeeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CIGASSeeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CIGASSeeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CIGASSeeView 诊断

#ifdef _DEBUG
void CIGASSeeView::AssertValid() const
{
	CView::AssertValid();
}

void CIGASSeeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIGASSeeDoc* CIGASSeeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIGASSeeDoc)));
	return (CIGASSeeDoc*)m_pDocument;
}
#endif //_DEBUG


// CIGASSeeView 消息处理程序
