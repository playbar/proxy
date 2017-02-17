// ImageBoardView.h : interface of the CImageBoardView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEBOARDVIEW_H__CEBADA0E_ED34_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_IMAGEBOARDVIEW_H__CEBADA0E_ED34_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"
#include "FloatDibWnd.h"


// Define a new message to handle palette
#define WM_REALIZEPAL	    (WM_USER + 0x100)

#define MD_HORZ				1
#define MD_VERT				2

#define DT_SELECT			0
#define DT_FREELINE			1
#define DT_LINE				2
#define DT_CURVE			3
#define DT_RECT_H			4
#define DT_RECT_F			5
#define DT_ROUNDRECT_H		6
#define DT_ROUNDRECT_F		7
#define DT_ELLIP_H			8
#define DT_ELLIP_F			9
#define DT_FILL				10
#define DT_TEXT				11
#define DT_PICKER			12
#define DT_ERASER			13

class CImageBoardView : public CScrollView
{
protected: // create from serialization only
	CImageBoardView();
	DECLARE_DYNCREATE(CImageBoardView)

// Attributes
public:
	CImageBoardDoc* GetDocument();
	int		m_nLengthUnit;
	CDib*	m_pDib;
	CFloatDibWnd* m_pFloatWnd;
	CRect	m_rcClip;
	CRect	m_rcRubber;
	CPoint	m_ptStart;
	BOOL	m_bDrawingRubber;
	int		m_nDrawType;
	int		m_nPenStyle;
	int		m_nPenWidth;
	COLORREF  m_crPenColor;
	COLORREF  m_crFillColor;
	CEdit	m_EditText;
	CFont*	m_pFont;
	BOOL	m_bDrawFreeline;
	BOOL	m_bErasing;
	CPoint	m_ptFreelineStart;
	int		m_nDrawCurveStep;
	BOOL	m_bDrawCurve;
	CPoint	m_ptCurve[4];
	HCURSOR	m_hCursorFreeline;
	HCURSOR	m_hCursorEraser;
	HCURSOR	m_hCursorPicker;
	HCURSOR	m_hCursorFill;
	HCURSOR	m_hCursorCurve;
	HCURSOR	m_hCursorGeneralDraw;
	int		m_nTextAlign;

// Operations
public:
	void	EdgeEnhance(int nAlgorithm);
	void	SetStatusBarLengthUnit(int nUnit);
	void	SetStatusBarCursorPosition(CPoint point);
	void	SetStatusBarImageSize(CSize size);
	void	SetLengthUnit(int nUnit);
	void	SetStatusBarBitCount(WORD wBitcount);
	void	ShowPenColor();
	void	ShowFillColor();
	void	ShowPenWidth();
	void	ShowPenStyle();
	void	ShowColorGrid();
	BOOL	AdjustSize(HDIB hDib);

	void	Initialize(CDib* pDib);
	BOOL	PointInDib(CPoint point);
	void	CreateFloatWnd(HDIB hDib, CPoint ptTopLeft);
	void	DeleteFloatWnd();
	void	MergeFloatDib();
	void	CutSelectedRect();
	void	StartDrawRubber(CPoint point);
	void	DrawRubber(CPoint point);
	void	StopDrawRubber();
	void	ClientToDib(CPoint& point);
	void	ClientToDib(CRect& rect);
	void	DibToClient(CPoint& point);
	void	DibToClient(CRect& rect);
	BOOL	AdjustPointinDib(CPoint& point);
	void	DoDrawRubber(CDC *pDC, CRect rc);
	void	DeleteFloatDib();
	void	CutFloatDib();
	BOOL	PasteDibFromClipboard();
	BOOL	PasteBitmapFromClipboard();
	BOOL	PastePalFromClipboard();
	void	Cut();
	void	Delete();
	void	CopyToClipboard();
	void	Paste();
	void	SelectPaste(int nCF);
	void	Rotate(double fAngle);
	void	Flip(int nDirection);
	void	Zoom(double fRatioX, double fRatioY);
	void	ChangeCanvasSize(int nWidth, int nHeight, int nPosition);
	void	ChangeImageSize(int nWidth, int nHeight);
	void	Crop();
	BOOL	MergeText();
	void	DrawCurve();
	void	DrawTmpCurve();

	BOOL IsClipRectExist()
	{
		return (! m_rcClip.IsRectEmpty());
	}

	void SetDrawType(int nDrawType=DT_SELECT)
	{
		MergeFloatDib();
		MergeText();
		m_nDrawType = nDrawType;
	}
	void SetPenStyle(int nStyle=PS_SOLID)
	{
		m_nPenStyle = nStyle;
	}
	void SetPenWidth(int nWidth=1)
	{
		m_nPenWidth = nWidth;
	}
	void SetPenColor(COLORREF color=RGB(0,0,0))
	{
		m_crPenColor= color;
	}
	void SetFillColor(COLORREF color=RGB(0,0,0))
	{
		m_crFillColor = color;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageBoardView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageBoardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageBoardView)
	afx_msg void OnEast();
	afx_msg void OnUpdateEast(CCmdUI* pCmdUI);
	afx_msg void OnHf1();
	afx_msg void OnUpdateHf1(CCmdUI* pCmdUI);
	afx_msg void OnHf2();
	afx_msg void OnUpdateHf2(CCmdUI* pCmdUI);
	afx_msg void OnHf3();
	afx_msg void OnUpdateHf3(CCmdUI* pCmdUI);
	afx_msg void OnHorz();
	afx_msg void OnUpdateHorz(CCmdUI* pCmdUI);
	afx_msg void OnLap1();
	afx_msg void OnUpdateLap1(CCmdUI* pCmdUI);
	afx_msg void OnLap2();
	afx_msg void OnUpdateLap2(CCmdUI* pCmdUI);
	afx_msg void OnLap3();
	afx_msg void OnUpdateLap3(CCmdUI* pCmdUI);
	afx_msg void OnLap4();
	afx_msg void OnUpdateLap4(CCmdUI* pCmdUI);
	afx_msg void OnLf1();
	afx_msg void OnUpdateLf1(CCmdUI* pCmdUI);
	afx_msg void OnLf2();
	afx_msg void OnUpdateLf2(CCmdUI* pCmdUI);
	afx_msg void OnLf3();
	afx_msg void OnUpdateLf3(CCmdUI* pCmdUI);
	afx_msg void OnNorth();
	afx_msg void OnUpdateNorth(CCmdUI* pCmdUI);
	afx_msg void OnNortheast();
	afx_msg void OnUpdateNortheast(CCmdUI* pCmdUI);
	afx_msg void OnNorthwest();
	afx_msg void OnUpdateNorthwest(CCmdUI* pCmdUI);
	afx_msg void OnSouth();
	afx_msg void OnUpdateSouth(CCmdUI* pCmdUI);
	afx_msg void OnSoutheast();
	afx_msg void OnUpdateSoutheast(CCmdUI* pCmdUI);
	afx_msg void OnSouthwest();
	afx_msg void OnUpdateSouthwest(CCmdUI* pCmdUI);
	afx_msg void OnVert();
	afx_msg void OnUpdateVert(CCmdUI* pCmdUI);
	afx_msg void OnVerthorz();
	afx_msg void OnUpdateVerthorz(CCmdUI* pCmdUI);
	afx_msg void OnWest();
	afx_msg void OnUpdateWest(CCmdUI* pCmdUI);
	afx_msg void OnMedianfilter();
	afx_msg void OnUpdateMedianfilter(CCmdUI* pCmdUI);
	afx_msg void OnInch();
	afx_msg void OnMm();
	afx_msg void OnPixel();
	afx_msg void OnSelect();
	afx_msg void OnCurve();
	afx_msg void OnLine();
	afx_msg void OnFreeline();
	afx_msg void OnRectH();
	afx_msg void OnRectF();
	afx_msg void OnEllipseH();
	afx_msg void OnEllipseF();
	afx_msg void OnText();
	afx_msg void OnFill();
	afx_msg void OnPencolor();
	afx_msg void OnSolid();
	afx_msg void OnDash();
	afx_msg void OnDot();
	afx_msg void OnFillcolor();
	afx_msg void OnUpdateSelect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurve(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFreeline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRectH(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRectF(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEllipseH(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEllipseF(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFill(CCmdUI* pCmdUI);
	afx_msg void OnUpdateText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDash(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSolid(CCmdUI* pCmdUI);
	afx_msg void OnPw1();
	afx_msg void OnUpdatePw1(CCmdUI* pCmdUI);
	afx_msg void OnPw2();
	afx_msg void OnUpdatePw2(CCmdUI* pCmdUI);
	afx_msg void OnPw3();
	afx_msg void OnUpdatePw3(CCmdUI* pCmdUI);
	afx_msg void OnPw4();
	afx_msg void OnUpdatePw4(CCmdUI* pCmdUI);
	afx_msg void OnPw5();
	afx_msg void OnUpdatePw5(CCmdUI* pCmdUI);
	afx_msg void OnPw6();
	afx_msg void OnUpdatePw6(CCmdUI* pCmdUI);
	afx_msg void OnPw7();
	afx_msg void OnUpdatePw7(CCmdUI* pCmdUI);
	afx_msg void OnPw8();
	afx_msg void OnUpdatePw8(CCmdUI* pCmdUI);
	afx_msg void OnPwOther();
	afx_msg void OnUpdatePwOther(CCmdUI* pCmdUI);
	afx_msg void OnRoundrectF();
	afx_msg void OnUpdateRoundrectF(CCmdUI* pCmdUI);
	afx_msg void OnRoundrectH();
	afx_msg void OnUpdateRoundrectH(CCmdUI* pCmdUI);
	afx_msg void OnFont();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnSelectpaste();
	afx_msg void OnUpdateSelectpaste(CCmdUI* pCmdUI);
	afx_msg void OnCrop();
	afx_msg void OnUpdateCrop(CCmdUI* pCmdUI);
	afx_msg void OnRotate();
	afx_msg void OnUpdateRotate(CCmdUI* pCmdUI);
	afx_msg void OnRotate180();
	afx_msg void OnUpdateRotate180(CCmdUI* pCmdUI);
	afx_msg void OnRotate270();
	afx_msg void OnUpdateRotate270(CCmdUI* pCmdUI);
	afx_msg void OnHmirror();
	afx_msg void OnUpdateHmirror(CCmdUI* pCmdUI);
	afx_msg void OnVmirror();
	afx_msg void OnUpdateVmirror(CCmdUI* pCmdUI);
	afx_msg void OnScale();
	afx_msg void OnUpdateScale(CCmdUI* pCmdUI);
	afx_msg void OnCanvassize();
	afx_msg void OnUpdateCanvassize(CCmdUI* pCmdUI);
	afx_msg void OnLeft();
	afx_msg void OnCenter();
	afx_msg void OnRight();
	afx_msg void OnUpdateCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRight(CCmdUI* pCmdUI);
	afx_msg void OnAdjustcolor();
	afx_msg void OnUpdateAdjustcolor(CCmdUI* pCmdUI);
	afx_msg void OnBc();
	afx_msg void OnUpdateBc(CCmdUI* pCmdUI);
	afx_msg void OnColorbits();
	afx_msg void OnUpdateColorbits(CCmdUI* pCmdUI);
	afx_msg void OnGray();
	afx_msg void OnUpdateGray(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnRed();
	afx_msg void OnUpdateRed(CCmdUI* pCmdUI);
	afx_msg void OnGreen();
	afx_msg void OnUpdateGreen(CCmdUI* pCmdUI);
	afx_msg void OnBlue();
	afx_msg void OnUpdateBlue(CCmdUI* pCmdUI);
	afx_msg void OnGetblue();
	afx_msg void OnUpdateGetblue(CCmdUI* pCmdUI);
	afx_msg void OnGetgreen();
	afx_msg void OnUpdateGetgreen(CCmdUI* pCmdUI);
	afx_msg void OnGetred();
	afx_msg void OnUpdateGetred(CCmdUI* pCmdUI);
	afx_msg void OnCopyto();
	afx_msg void OnUpdateCopyto(CCmdUI* pCmdUI);
	afx_msg void OnPastefrom();
	afx_msg void OnUpdatePastefrom(CCmdUI* pCmdUI);
	afx_msg void OnEraser();
	afx_msg void OnUpdateEraser(CCmdUI* pCmdUI);
	afx_msg void OnPicker();
	afx_msg void OnUpdatePicker(CCmdUI* pCmdUI);
	afx_msg void OnCloseH();
	afx_msg void OnUpdateCloseH(CCmdUI* pCmdUI);
	afx_msg void OnCloseV();
	afx_msg void OnUpdateCloseV(CCmdUI* pCmdUI);
	afx_msg void OnDilationH();
	afx_msg void OnUpdateDilationH(CCmdUI* pCmdUI);
	afx_msg void OnDilationV();
	afx_msg void OnUpdateDilationV(CCmdUI* pCmdUI);
	afx_msg void OnErosionH();
	afx_msg void OnUpdateErosionH(CCmdUI* pCmdUI);
	afx_msg void OnErosionV();
	afx_msg void OnUpdateErosionV(CCmdUI* pCmdUI);
	afx_msg void OnOpenH();
	afx_msg void OnUpdateOpenH(CCmdUI* pCmdUI);
	afx_msg void OnOpenV();
	afx_msg void OnUpdateOpenV(CCmdUI* pCmdUI);
	afx_msg void OnThin();
	afx_msg void OnUpdateThin(CCmdUI* pCmdUI);
	afx_msg void OnFft();
	afx_msg void OnUpdateFft(CCmdUI* pCmdUI);
	afx_msg void OnDct();
	afx_msg void OnUpdateDct(CCmdUI* pCmdUI);
	afx_msg void OnWalh();
	afx_msg void OnUpdateWalh(CCmdUI* pCmdUI);
	afx_msg void OnContourH();
	afx_msg void OnUpdateContourH(CCmdUI* pCmdUI);
	afx_msg void OnContourV();
	afx_msg void OnUpdateContourV(CCmdUI* pCmdUI);
	afx_msg void OnRotateAngle();
	afx_msg void OnUpdateRotateAngle(CCmdUI* pCmdUI);
	afx_msg void OnCorrectgamma();
	afx_msg void OnUpdateCorrectgamma(CCmdUI* pCmdUI);
	afx_msg void OnDifferenceedgedetect();
	afx_msg void OnUpdateDifferenceedgedetect(CCmdUI* pCmdUI);
	afx_msg void OnEmbossment();
	afx_msg void OnUpdateEmbossment(CCmdUI* pCmdUI);
	afx_msg void OnExposure();
	afx_msg void OnUpdateExposure(CCmdUI* pCmdUI);
	afx_msg void OnHomoedgedetect();
	afx_msg void OnUpdateHomoedgedetect(CCmdUI* pCmdUI);
	afx_msg void OnNegative();
	afx_msg void OnUpdateNegative(CCmdUI* pCmdUI);
	afx_msg void OnSobeled();
	afx_msg void OnUpdateSobeled(CCmdUI* pCmdUI);
	afx_msg void OnSpread();
	afx_msg void OnUpdateSpread(CCmdUI* pCmdUI);
	afx_msg void OnHoughed();
	afx_msg void OnUpdateHoughed(CCmdUI* pCmdUI);
	afx_msg void OnCannyed();
	afx_msg void OnUpdateCannyed(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFillcolor1();
	afx_msg void OnPencolor1();
	afx_msg void OnPenColorGrid(UINT nID);
	afx_msg LRESULT OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageBoardView.cpp
inline CImageBoardDoc* CImageBoardView::GetDocument()
   { return (CImageBoardDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEBOARDVIEW_H__CEBADA0E_ED34_11D2_9481_000021003EA5__INCLUDED_)
