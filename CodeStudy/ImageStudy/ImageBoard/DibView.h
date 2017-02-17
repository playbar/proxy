#if !defined(AFX_DIBVIEW_H__0B12E5E2_AFF2_11D2_9481_000021003EA5__INCLUDED_)
#define AFX_DIBVIEW_H__0B12E5E2_AFF2_11D2_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DibView.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////
// CDibView view

class CDibView : public CScrollView
{
protected:
	CDibView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDibView)

// Attributes
public:
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
	CPoint	m_ptFreelineStart;
	int		m_nDrawCurveStep;
	BOOL	m_bDrawCurve;
	CPoint	m_ptCurve[4];
	HCURSOR	m_hCursorFreeline;
	HCURSOR	m_hCursorFill;
	HCURSOR	m_hCursorCurve;
	HCURSOR	m_hCursorGeneralDraw;
	int		m_nTextAlign;

// Operations
public:
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
	void	Rotate(int nAngle);
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
	//{{AFX_VIRTUAL(CDibView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDibView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDibView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnRealizePal(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBVIEW_H__0B12E5E2_AFF2_11D2_9481_000021003EA5__INCLUDED_)
