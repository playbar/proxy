#pragma once

#include "HeaderCtrlEx.h"

// CListCtrlEx

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)

public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );

protected:
	CHeaderCtrlEx m_ctlHeader;

	//int m_iNumColumns;
	int m_iSortColumn;
	BOOL m_bSortAscending;
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	void Sort(int iColumn, BOOL bAscending);
	void SetOneselfItemData();
	void FreeItemMemory( const int iItem );
	afx_msg void OnDestroy();

};


