// ListCtrlEx.cpp : 实现文件
//

#include "stdafx.h"
#include "IGASSee.h"
#include "ListCtrlEx.h"
#include ".\listctrlex.h"

#ifdef _DEBUG
	#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
	#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG

struct ItemData
{
public:
  ItemData() : arrpsz( NULL ), dwData( NULL ) {}
  
  LPTSTR* arrpsz;
  DWORD dwData;
  
private:
  // ban copying.
  ItemData( const ItemData& );
  ItemData& operator=( const ItemData& );
};


bool IsNumber( LPCTSTR pszText )
{
  ASSERT_VALID_STRING(pszText);
  int iDotCount=0;
  for(int i=0;i<lstrlen(pszText);i++){
    if(!_istdigit(pszText[i])){
      if(pszText[i]!='.')
        return false;
      iDotCount++;
      if(iDotCount>1)return false;
    }
  }
  return true;
}

int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
  ASSERT_VALID_STRING(pszNumber1);
  ASSERT_VALID_STRING(pszNumber2);
  
  const double fNumber1=atof(pszNumber1);
  const double fNumber2=atof(pszNumber2);
  
  if(fNumber1<fNumber2)
    return -1;
  else if(fNumber1>fNumber2)
    return 1;
  return 0;
}

bool IsDate( LPCTSTR pszText )
{
  ASSERT_VALID_STRING( pszText );
  
  // format should be 99/99/9999.
  
  if( lstrlen( pszText ) != 10 )
    return false;
  
  return _istdigit(pszText[0])
    && _istdigit(pszText[1])
    && pszText[2]==_T('-')
    && _istdigit( pszText[ 3 ] )
    && _istdigit( pszText[ 4 ] )
    && pszText[5]==_T('-')
    && _istdigit( pszText[ 6 ] )
    && _istdigit( pszText[ 7 ] )
    && _istdigit( pszText[ 8 ] )
    && _istdigit( pszText[ 9 ] );
}


int DateCompare( const CString& strDate1, const CString& strDate2 )
{
  const int iYear1=atoi(strDate1.Mid(6, 4));
  const int iYear2=atoi(strDate2.Mid(6, 4));
  
  if(iYear1<iYear2)
    return -1;
  else if(iYear1>iYear2)
    return 1;
  
  const int iMonth1=atoi(strDate1.Mid(3, 2));
  const int iMonth2=atoi(strDate2.Mid(3, 2));
  
  if(iMonth1<iMonth2)
    return -1;  
  else if(iMonth1>iMonth2)
    return 1;
  
  const int iDay1=atoi(strDate1.Mid(0, 2));
  const int iDay2=atoi(strDate2.Mid(0, 2));
  
  if(iDay1<iDay2)
    return -1;
  else if(iDay1>iDay2)
    return 1;

  return 0;
}


// CListCtrlEx

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)
CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnclick)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CListCtrlEx 消息处理程序

int CALLBACK CListCtrlEx::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
  CListCtrlEx* pListCtrl = reinterpret_cast<CListCtrlEx*>( lParamData );
  ASSERT( pListCtrl->IsKindOf( RUNTIME_CLASS( CListCtrl ) ) );
  
  ItemData* pid1 = reinterpret_cast<ItemData*>( lParam1 );
  ItemData* pid2 = reinterpret_cast<ItemData*>( lParam2 );
  
  ASSERT( pid1 );
  ASSERT( pid2 );
  
  LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_iSortColumn ];
  LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_iSortColumn ];
  
  ASSERT_VALID_STRING( pszText1 );
  ASSERT_VALID_STRING( pszText2 );
  
  if(IsNumber(pszText1))
    return pListCtrl->m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
  else if(IsDate(pszText1))
    return pListCtrl->m_bSortAscending ? DateCompare( pszText1, pszText2 ) : DateCompare( pszText2, pszText1 );
  else
    // text.
    return pListCtrl->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
}

void CListCtrlEx::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	const int iColumn = pNMLV->iSubItem;
	Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );
	*pResult = 0;
}

void CListCtrlEx::Sort(int iColumn, BOOL bAscending)
{
  m_iSortColumn = iColumn;
  m_bSortAscending = bAscending;
  
  // show the appropriate arrow in the header control.
  // m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );
  
  VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
 
  int nItemCount = this->GetItemCount();
  for (int i = 0; i< nItemCount; i++)
  {
	  char szTemp[20];
	  sprintf(szTemp, "%d", i+1);
	  SetItemText(i,0, szTemp );
  }

 
}

void CListCtrlEx::SetOneselfItemData()
{
	CHeaderCtrl* pHeaderCtrl = this->GetHeaderCtrl();   
	ASSERT(pHeaderCtrl != NULL);
	int nColumnCount = pHeaderCtrl->GetItemCount();
	int nItemCount = this->GetItemCount();
	for(int i=0; i<nItemCount; i++)
	{
		LPTSTR* arrpsz = new LPTSTR[nColumnCount];
		for(int j=0; j<nColumnCount; j++)
		{
			CString strItem = this->GetItemText(i, j);
			arrpsz[j] = new TCHAR[strItem.GetLength() + 1];
			lstrcpy(arrpsz[j], (LPCTSTR)strItem);
		}

		ItemData* pid = new ItemData;
		pid->arrpsz = arrpsz;
		CListCtrl::SetItemData( i, reinterpret_cast<DWORD>(pid));
	}
}

void CListCtrlEx::FreeItemMemory( const int iItem )
{
  ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
  
  LPTSTR* arrpsz = pid->arrpsz;

  CHeaderCtrl* pHeaderCtrl = this->GetHeaderCtrl();   
  ASSERT(pHeaderCtrl != NULL);
  int nColumnCount = pHeaderCtrl->GetItemCount();
  
  for( int i = 0; i < nColumnCount; i++ )
    delete[] arrpsz[ i ];
  
  delete[] arrpsz;
  delete pid;
  
  VERIFY( CListCtrl::SetItemData( iItem, NULL ) );
}


void CListCtrlEx::OnDestroy()
{

	for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
    FreeItemMemory( iItem );

	CListCtrl::OnDestroy();
}
