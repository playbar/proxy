
#pragma  once 
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <strsafe.h>
#include <new.h>
#include <assert.h>

#define CALLBACK    __stdcall
#define WINAPI      __stdcall

#define RELEASE_COM(ixPoint)	{ if(NULL != (ixPoint)) { (ixPoint)->Release(); (ixPoint) = NULL; } }

#ifndef SAFE_DELETE
#define SAFE_DELETE( T ) {if( T ) { delete T; T = NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( T ) {if( T ) { delete[] T; T = NULL; }}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( T ) if( T ) T->Release()
#endif

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif
#endif

HRESULT WINAPI DXUTTrace( const CHAR* strFile, DWORD dwLine, HRESULT hr,
						 const WCHAR* strMsg, bool bPopMsgBox );
#define DXUT_ERR(str,hr)           DXUTTrace( __FILE__, (DWORD)__LINE__, hr, str, false )

void DXUTOutputDebugStringW( LPCWSTR strMsg, ... );
void DXUTOutputDebugStringA( LPCSTR strMsg, ... );

#ifdef UNICODE
#define DXUTOutputDebugString DXUTOutputDebugStringW
#else
#define DXUTOutputDebugString DXUTOutputDebugStringA
#endif
#define DXUTTRACE				DXUTOutputDebugString

//////////////////////////////////////////////////////////////////////////
//
////在debug下显示输出信息宏,使用前要先定义存放字符串的数组
//// Put up an assertion failure message box.
//inline void InlineShowOutPutDebug(LPCSTR file, int line,  PCSTR expr) 
//{
//	char szOutputString[ sizeof(char) *MAX_PATH];
//	memset(szOutputString, 0, sizeof(char)  * MAX_PATH );
//	SYSTEMTIME st;
//	GetLocalTime( &st );
//	char szDate[100];
//	sprintf( szDate, "%04d-%02d-%02d %02d:%02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
//	sprintf(szOutputString, "%s,  File %s, line %d ,Date:%s ",expr, file, line, szDate );
//	OutputDebugStringA(szOutputString);
//}
//
//
////在debug下显示输出信息宏,使用前要先定义存放字符串的数组
//# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
//#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
//# else
//#	define	SHOWOUTPUTDEBUG(szOutputString) InlineShowOutPutDebug(__FILE__, __LINE__, #szOutputString)
//# endif
////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------------------
// Error codes
//--------------------------------------------------------------------------------------
#define DXUTERR_NODIRECT3D              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0901)
#define DXUTERR_NOCOMPATIBLEDEVICES     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0902)
#define DXUTERR_MEDIANOTFOUND           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0903)
#define DXUTERR_NONZEROREFCOUNT         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0904)
#define DXUTERR_CREATINGDEVICE          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0905)
#define DXUTERR_RESETTINGDEVICE         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0906)
#define DXUTERR_CREATINGDEVICEOBJECTS   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0907)
#define DXUTERR_RESETTINGDEVICEOBJECTS  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0908)
#define DXUTERR_INCORRECTVERSION        MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0909)



//--------------------------------------------------------------------------------------
// A growable array
//--------------------------------------------------------------------------------------
template< typename TYPE >
class CGrowableArray
{
public:
	CGrowableArray()  { m_pData = NULL; m_nSize = 0; m_nMaxSize = 0; }
	CGrowableArray( const CGrowableArray<TYPE>& a ) { for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); }
	~CGrowableArray() { RemoveAll(); }

	const TYPE& operator[]( int nIndex ) const { return GetAt( nIndex ); }
	TYPE& operator[]( int nIndex ) { return GetAt( nIndex ); }

	CGrowableArray& operator=( const CGrowableArray<TYPE>& a ) { if( this == &a ) return *this; RemoveAll(); for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); return *this; }

	HRESULT SetSize( int nNewMaxSize );
	HRESULT Add( const TYPE& value );
	HRESULT Insert( int nIndex, const TYPE& value );
	HRESULT SetAt( int nIndex, const TYPE& value );
	TYPE&   GetAt( int nIndex ) 
	{ 
		assert( nIndex >= 0 && nIndex < m_nSize ); 
		return m_pData[nIndex]; 
	}

	int     GetSize() const { return m_nSize; }
	TYPE*   GetData() { return m_pData; }
	bool    Contains( const TYPE& value ){ return ( -1 != IndexOf( value ) ); }

	int     IndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1; }
	int     IndexOf( const TYPE& value, int iStart ) { return IndexOf( value, iStart, m_nSize - iStart ); }
	int     IndexOf( const TYPE& value, int nIndex, int nNumElements );

	int     LastIndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize-1, m_nSize ) : -1; }
	int     LastIndexOf( const TYPE& value, int nIndex ) { return LastIndexOf( value, nIndex, nIndex+1 ); }
	int     LastIndexOf( const TYPE& value, int nIndex, int nNumElements );

	HRESULT Remove( int nIndex );
	void    RemoveAll() { SetSize(0); }

protected:
	TYPE* m_pData;      // the actual array of data
	int m_nSize;        // # of elements (upperBound - 1)
	int m_nMaxSize;     // max allocated

	HRESULT SetSizeInternal( int nNewMaxSize );  // This version doesn't call ctor or dtor.
};


// This version doesn't call ctor or dtor.
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::SetSizeInternal( int nNewMaxSize )
{
	if( nNewMaxSize < 0 )
	{
		assert( false );
		return E_INVALIDARG;
	}

	if( nNewMaxSize == 0 )
	{
		// Shrink to 0 size & cleanup
		if( m_pData )
		{
			free( m_pData );
			m_pData = NULL;
		}

		m_nMaxSize = 0;
		m_nSize = 0;
	}
	else if( m_pData == NULL || nNewMaxSize > m_nMaxSize )
	{
		// Grow array
		int nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;
		nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );

		TYPE* pDataNew = (TYPE*) realloc( m_pData, nNewMaxSize * sizeof(TYPE) );
		if( pDataNew == NULL )
			return E_OUTOFMEMORY;

		m_pData = pDataNew;
		m_nMaxSize = nNewMaxSize;
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::SetSize( int nNewMaxSize )
{
	int nOldSize = m_nSize;

	if( nOldSize > nNewMaxSize )
	{
		// Removing elements. Call dtor.

		for( int i = nNewMaxSize; i < nOldSize; ++i )
			m_pData[i].~TYPE();
	}

	// Adjust buffer.  Note that there's no need to check for error
	// since if it happens, nOldSize == nNewMaxSize will be true.)
	HRESULT hr = SetSizeInternal( nNewMaxSize );

	if( nOldSize < nNewMaxSize )
	{
		// Adding elements. Call ctor.

		for( int i = nOldSize; i < nNewMaxSize; ++i )
			::new (&m_pData[i]) TYPE;
	}

	return hr;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::Add( const TYPE& value )
{
	HRESULT hr;
	if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
		return hr;

	// Construct the new element
	::new (&m_pData[m_nSize])TYPE;

	// Assign
	m_pData[m_nSize] = value;
	++m_nSize;

	return S_OK;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::Insert( int nIndex, const TYPE& value )
{
	HRESULT hr;

	// Validate index
	if( nIndex < 0 || 
		nIndex > m_nSize )
	{
		assert( false );
		return E_INVALIDARG;
	}

	// Prepare the buffer
	if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
		return hr;

	// Shift the array
	MoveMemory( &m_pData[nIndex+1], &m_pData[nIndex], sizeof(TYPE) * (m_nSize - nIndex) );

	// Construct the new element
	::new (&m_pData[nIndex]) TYPE;

	// Set the value and increase the size
	m_pData[nIndex] = value;
	++m_nSize;

	return S_OK;
}


//--------------------------------------------------------------------------------------
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::SetAt( int nIndex, const TYPE& value )
{
	// Validate arguments
	if( nIndex < 0 ||
		nIndex >= m_nSize )
	{
		assert( false );
		return E_INVALIDARG;
	}

	m_pData[nIndex] = value;
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the first occurrence
// within the section of the data array that extends from iStart and contains the 
// specified number of elements. Returns -1 if value is not found within the given 
// section.
//--------------------------------------------------------------------------------------
template< typename TYPE >
int CGrowableArray<TYPE>::IndexOf( const TYPE& value, int iStart, int nNumElements )
{
	// Validate arguments
	if( iStart < 0 || 
		iStart >= m_nSize ||
		nNumElements < 0 ||
		iStart + nNumElements > m_nSize )
	{
		assert( false );
		return -1;
	}

	// Search
	for( int i = iStart; i < (iStart + nNumElements); i++ )
	{
		if( value == m_pData[i] )
			return i;
	}

	// Not found
	return -1;
}


//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the last occurrence
// within the section of the data array that contains the specified number of elements
// and ends at iEnd. Returns -1 if value is not found within the given section.
//--------------------------------------------------------------------------------------
template< typename TYPE >
int CGrowableArray<TYPE>::LastIndexOf( const TYPE& value, int iEnd, int nNumElements )
{
	// Validate arguments
	if( iEnd < 0 || 
		iEnd >= m_nSize ||
		nNumElements < 0 ||
		iEnd - nNumElements < 0 )
	{
		assert( false );
		return -1;
	}

	// Search
	for( int i = iEnd; i > (iEnd - nNumElements); i-- )
	{
		if( value == m_pData[i] )
			return i;
	}

	// Not found
	return -1;
}



//--------------------------------------------------------------------------------------
template< typename TYPE >
HRESULT CGrowableArray<TYPE>::Remove( int nIndex )
{
	if( nIndex < 0 || 
		nIndex >= m_nSize )
	{
		assert( false );
		return E_INVALIDARG;
	}

	// Destruct the element to be removed
	m_pData[nIndex].~TYPE();

	// Compact the array and decrease the size
	MoveMemory( &m_pData[nIndex], &m_pData[nIndex+1], sizeof(TYPE) * (m_nSize - (nIndex+1)) );
	--m_nSize;

	return S_OK;
}





//////////////////////////////////////////////////////////////////////////


