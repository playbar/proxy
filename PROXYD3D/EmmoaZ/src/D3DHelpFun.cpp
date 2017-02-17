#include "stdafx.h"
#include "D3DHelpFun.h"

#pragma warning(disable:4786)
#include <map>
static std::map < LPDIRECT3DDEVICE , long >   s_dev_ref_count;


namespace   IGA_D3D_HELP
{
	typedef HRESULT (WINAPI *LPFN_D3DXCREATETEXTUREFROMFILEA)(LPDIRECT3DDEVICE, LPCSTR, IDirect3DTexture**);	
	typedef HRESULT (WINAPI *LPFN_D3DXCREATETEXTUREFROMFILEEX)(LPDIRECT3DDEVICE,LPCSTR,UINT,UINT,UINT,DWORD,D3DFORMAT,D3DPOOL,DWORD,DWORD,D3DCOLOR,D3DXIMAGE_INFO*,PALETTEENTRY*,IDirect3DTexture** );
	typedef HRESULT (WINAPI *LPFN_D3DXSAVETEXTURETOFILEA)(LPCSTR, D3DXIMAGE_FILEFORMAT, IDirect3DTexture *, CONST PALETTEENTRY *);
   	typedef HRESULT (WINAPI *LPFN_D3DXSAVESURFACETOFILE)(LPCTSTR,D3DXIMAGE_FILEFORMAT,IDirect3DSurface*,CONST PALETTEENTRY*,CONST RECT * );

	//d3d扩展函数
	HINSTANCE                              g_hD3DX = NULL ;
	static LPFN_D3DXCREATETEXTUREFROMFILEA        s_pFun_D3DXCreateTextureFromFile   = NULL ;
	static LPFN_D3DXCREATETEXTUREFROMFILEEX       s_pFun_D3DXCreateTextureFromFileEx = NULL ;
	static LPFN_D3DXSAVETEXTURETOFILEA            s_pFun_D3DxSaveTextureToFile       = NULL ;
	static LPFN_D3DXSAVESURFACETOFILE             s_pFun_D3DxSaveSurfaceToFile       = NULL ; 
	BOOL  InitD3Dx()
	{
		if  ( g_hD3DX != NULL )  return TRUE;
		TCHAR*   p   =  NULL;
		TCHAR   szTmpBuf    [ _MAX_PATH+1 ];
		TCHAR   szModulName [ _MAX_PATH+1 ];
		
		if  ( g_hD3DX != NULL ) goto lab1;		
		
		//
		//优先系统目录,考虑到vista和xp下面的DX dll不同。
		//
		::GetSystemDirectoryA(szTmpBuf, _MAX_PATH);
		::wsprintf(szModulName,_T("%s\\%s"),szTmpBuf,_D3DX_LIB_NAME);
		g_hD3DX	    = ::LoadLibrary( szModulName );
		if  ( g_hD3DX != NULL ) goto lab1;
		
		//程序目录
		::GetModuleFileName(NULL, szTmpBuf, _MAX_PATH);
		 p	= ::_tcsrchr( szTmpBuf, '\\');
		if( p != NULL )	 *p =0;
		
		::wsprintf(szModulName,_T("%s\\%s"),szTmpBuf,_D3DX_LIB_NAME);
		g_hD3DX	    = ::LoadLibrary(szModulName );
		if  ( g_hD3DX == NULL ) return FALSE;
		
		
		
lab1:	
		s_pFun_D3DXCreateTextureFromFile	  = (LPFN_D3DXCREATETEXTUREFROMFILEA )::GetProcAddress(g_hD3DX, _T("D3DXCreateTextureFromFileA"));		
		s_pFun_D3DXCreateTextureFromFileEx    = (LPFN_D3DXCREATETEXTUREFROMFILEEX)::GetProcAddress(g_hD3DX, _T("D3DXCreateTextureFromFileExA"));    
		s_pFun_D3DxSaveTextureToFile          = (LPFN_D3DXSAVETEXTURETOFILEA)::GetProcAddress(g_hD3DX, _T("D3DXSaveTextureToFileA"));   
		s_pFun_D3DxSaveSurfaceToFile          = (LPFN_D3DXSAVESURFACETOFILE)::GetProcAddress(g_hD3DX, _T("D3DXSaveSurfaceToFileA"));   
		return TRUE;
		
	};
	VOID  UnInitD3Dx()
	{
		if ( NULL == g_hD3DX )
		{	
			s_pFun_D3DXCreateTextureFromFile   = NULL; 
			s_pFun_D3DXCreateTextureFromFileEx = NULL;
			
			::FreeLibrary(g_hD3DX);
			g_hD3DX  = NULL;
		}
	}

	struct FMTINFO
	{
		D3DFORMAT fmt;
		const TCHAR* strName;
		const TCHAR* strType;
		int   nBitsPerTexel;
		BOOL  bHasAlpha;
	};

	static const TCHAR* TYPE_UNSIGNED = TEXT("Unsigned");
	static const TCHAR* TYPE_SIGNED   = TEXT("Signed");
	static const TCHAR* TYPE_MIXED    = TEXT("Mixed");
	static const TCHAR* TYPE_FOUR_CC  = TEXT("Four CC");
	static const TCHAR* TYPE_FLOAT    = TEXT("Floating-Point");
	static const TCHAR* TYPE_IEEE     = TEXT("IEEE Floating-Point");

	const FMTINFO fmtInfoArray[] = 
	{
			D3DFMT_A4L4,            TEXT("A4L4"),           TYPE_UNSIGNED,   8,     TRUE,  
			D3DFMT_A8,              TEXT("A8"),             TYPE_UNSIGNED,   8,     TRUE,  
			D3DFMT_L8,              TEXT("L8"),             TYPE_UNSIGNED,   8,     FALSE, 
			D3DFMT_P8,              TEXT("P8"),             TYPE_UNSIGNED,   8,     FALSE, 
			D3DFMT_R3G3B2,          TEXT("R3G3B2"),         TYPE_UNSIGNED,   8,     FALSE, 
			D3DFMT_A1R5G5B5,        TEXT("A1R5G5B5"),       TYPE_UNSIGNED,   16,    TRUE,  
			D3DFMT_A4R4G4B4,        TEXT("A4R4G4B4"),       TYPE_UNSIGNED,   16,    TRUE,  
			D3DFMT_A8L8,            TEXT("A8L8"),           TYPE_UNSIGNED,   16,    TRUE,  
			D3DFMT_A8P8,            TEXT("A8P8"),           TYPE_UNSIGNED,   16,    TRUE,  
			D3DFMT_A8R3G3B2,        TEXT("A8R3G3B2"),       TYPE_UNSIGNED,   16,    TRUE,  
			//    D3DFMT_L16,             TEXT("L16"),            TYPE_UNSIGNED,   16,    FALSE, 
			D3DFMT_R5G6B5,          TEXT("R5G6B5"),         TYPE_UNSIGNED,   16,    FALSE, 
			D3DFMT_X1R5G5B5,        TEXT("X1R5G5B5"),       TYPE_UNSIGNED,   16,    FALSE, 
			D3DFMT_X4R4G4B4,        TEXT("X4R4G4B4"),       TYPE_UNSIGNED,   16,    FALSE, 
			D3DFMT_R8G8B8,          TEXT("R8G8B8"),         TYPE_UNSIGNED,   24,    FALSE, 
			D3DFMT_A2B10G10R10,     TEXT("A2B10G10R10"),    TYPE_UNSIGNED,   32,    TRUE,  
			//    D3DFMT_A2R10G10B10,     TEXT("A2R10G10B10"),    TYPE_UNSIGNED,   32,    TRUE,  
			//    D3DFMT_A8B8G8R8,        TEXT("A8B8G8R8"),       TYPE_UNSIGNED,   32,    TRUE,  
			D3DFMT_A8R8G8B8,        TEXT("A8R8G8B8"),       TYPE_UNSIGNED,   32,    TRUE,  
			D3DFMT_G16R16,          TEXT("G16R16"),         TYPE_UNSIGNED,   32,    FALSE, 
			//    D3DFMT_X8B8G8R8,        TEXT("X8B8G8R8"),       TYPE_UNSIGNED,   32,    FALSE, 
			D3DFMT_X8R8G8B8,        TEXT("X8R8G8B8"),       TYPE_UNSIGNED,   32,    FALSE, 
			//    D3DFMT_A16B16G16R16,    TEXT("A16B16G16R16"),   TYPE_UNSIGNED,   64,    TRUE,  
			
			//    D3DFMT_CxV8U8,          TEXT("CxV8U8"),         TYPE_SIGNED,     16,    FALSE, 
			D3DFMT_V8U8,            TEXT("V8U8"),           TYPE_SIGNED,     16,    FALSE, 
			D3DFMT_Q8W8V8U8,        TEXT("Q8W8V8U8"),       TYPE_SIGNED,     32,    TRUE,  
			D3DFMT_V16U16,          TEXT("V16U16"),         TYPE_SIGNED,     32,    FALSE, 
			//    D3DFMT_Q16W16V16U16,    TEXT("Q16W16V16U16"),   TYPE_SIGNED,     64,    TRUE,  
			
			D3DFMT_L6V5U5,          TEXT("L6V5U5"),         TYPE_MIXED,      16,    FALSE, 
			D3DFMT_A2W10V10U10,     TEXT("A2W10V10U10"),    TYPE_MIXED,      32,    FALSE, 
			D3DFMT_X8L8V8U8,        TEXT("X8L8V8U8"),       TYPE_MIXED,      32,    FALSE, 
			
			D3DFMT_DXT1,            TEXT("DXT1"),           TYPE_FOUR_CC,    4,     TRUE,  
			D3DFMT_DXT2,            TEXT("DXT2"),           TYPE_FOUR_CC,    8,     FALSE, 
			D3DFMT_DXT3,            TEXT("DXT3"),           TYPE_FOUR_CC,    8,     TRUE,  
			D3DFMT_DXT4,            TEXT("DXT4"),           TYPE_FOUR_CC,    8,     FALSE, 
			D3DFMT_DXT5,            TEXT("DXT5"),           TYPE_FOUR_CC,    8,     TRUE,  
			//    D3DFMT_G8R8_G8B8,       TEXT("G8R8_G8B8"),      TYPE_FOUR_CC,    16,    FALSE, 
			//    D3DFMT_R8G8_B8G8,       TEXT("R8G8_B8G8"),      TYPE_FOUR_CC,    16,    FALSE, 
			D3DFMT_UYVY,            TEXT("UYVY"),           TYPE_FOUR_CC,    16,    FALSE, 
			D3DFMT_YUY2,            TEXT("YUY2"),           TYPE_FOUR_CC,    16,    FALSE, 
			
			//    D3DFMT_R16F,            TEXT("R16F"),           TYPE_FLOAT,      16,    FALSE, 
			//    D3DFMT_G16R16F,         TEXT("G16R16F"),        TYPE_FLOAT,      32,    FALSE, 
			//    D3DFMT_A16B16G16R16F,   TEXT("A16B16G16R16F"),  TYPE_FLOAT,      64,    TRUE,  
			
			//    D3DFMT_R32F,            TEXT("R32F"),           TYPE_IEEE,       32,    FALSE, 
			//    D3DFMT_G32R32F,         TEXT("G32R32F"),        TYPE_IEEE,       64,    FALSE, 
			//    D3DFMT_A32B32G32R32F,   TEXT("A32B32G32R32F"),  TYPE_IEEE,       128,   TRUE,  
	};
	const int fmtInfoArraySize = sizeof(fmtInfoArray) / sizeof(fmtInfoArray[0]);


	UINT GetSurfaceLineBytes(D3DSURFACE_DESC* pDesc)
	{
		UINT nLineBytes = 0;
		for( int i = 0; i < fmtInfoArraySize; i++ )
		{
			if( fmtInfoArray[i].fmt == pDesc->Format )
			{
				nLineBytes = (pDesc->Width * (fmtInfoArray[i].nBitsPerTexel / sizeof(BYTE))) / 8;
				break;
			}
		}
		return nLineBytes;	
	}

	BOOL Surface_DoCompare(const void *pSource, const void *pTarget, UINT nPicth, D3DSURFACE_DESC* pDesc,BOOL  bDebug)
	{

		//	否则进行默认的比较
		try
		{

		BOOL bRet;
		UINT i;
		UINT nNewPitch;
		UINT nHeight;
		UINT nLineBytes;
		D3DFORMAT Format;
		
		nLineBytes = GetSurfaceLineBytes(pDesc);

		nHeight = pDesc->Height;
		Format  = pDesc->Format;

		switch (Format)
		{
			case D3DFMT_DXT1:
			case D3DFMT_DXT2:
			case D3DFMT_DXT3:
			case D3DFMT_DXT4:
			case D3DFMT_DXT5:
				nNewPitch = nPicth >> 2;
				break;
			default:
				nNewPitch = nPicth;
				break;
		}

		// 比较单行
		try{

			if(IsBadReadPtr(pSource,nLineBytes))
			{
				return FALSE;
			}
			if(IsBadReadPtr(pTarget,nLineBytes))
			{
				return FALSE;
			}
			bRet = (memcmp(pSource, pTarget, nLineBytes) == 0);

		}catch(...)
		{
			return FALSE;
		}

		if (!bRet)
			return FALSE;
		
		// 比较所有行
		LPBYTE lpSourceBits = (LPBYTE)pSource;
		LPBYTE lpTargetBits = (LPBYTE)pTarget;
		for (i = 1; i < nHeight; i++)
		{
			int nResult;
			try
			{
				nResult = memcmp(lpSourceBits, lpTargetBits, nLineBytes);
			}
			catch(...)
			{
				return FALSE;
				
			}
			if ( nResult!= 0) 
			{
				bRet = FALSE;
				break;
			}

			lpSourceBits  += nNewPitch;
			lpTargetBits  += nNewPitch;
		}
		return bRet;

		}
		catch(...)
		{
			START_LOG;
			LOG << "Surface_DoCompare异常"  ;
			END_LOG;
			return FALSE;
		}
	}


	/******************************************************************************************
	*            比较纹理是否相同
	*   pSource       : 
	*   pTarget       : 
	*   nPicth        : 
	*   pDesc         :
	*   返回          :成功失败
	*****************************************************************************************/

	BOOL Surface_DoReplace(CHAR *pTarget,const CHAR *pSource,  UINT nPicth, D3DSURFACE_DESC* pDesc)
	{
		
		UINT i;
		UINT nNewPitch;
		UINT nHeight;
		UINT nLineBytes;
		D3DFORMAT Format;
		
		nLineBytes = GetSurfaceLineBytes(pDesc);
		nHeight = pDesc->Height;
		Format  = pDesc->Format;
		
		switch (Format)
		{
			case D3DFMT_DXT1:
			case D3DFMT_DXT2:
			case D3DFMT_DXT3:
			case D3DFMT_DXT4:
			case D3DFMT_DXT5:
				nNewPitch = nPicth >> 2;
				break;
			default:
				nNewPitch = nPicth;
				break;
		}
		
		LPBYTE lpSourceBits = (LPBYTE)pSource;
		LPBYTE lpTargetBits = (LPBYTE)pTarget;
		for (i = 0; i < nHeight; i++)
		{
			memcpy(lpTargetBits,lpSourceBits , nLineBytes);
			
			lpSourceBits  += nNewPitch;
			lpTargetBits  += nNewPitch;
		}
		return TRUE;		
	}

	//纹理比较替换
	BOOL Texture_DoCompare( IDirect3DTexture* pTexture1, IDirect3DTexture* pTexture2,UINT  uLevel)
	{	
		ASSERT(pTexture1 != NULL  );
		ASSERT(pTexture2 != NULL  );

		BOOL  bResult = NULL;

		D3DSURFACE_DESC	targetDesc;
		D3DSURFACE_DESC	sourceDesc;
		if(FAILED(pTexture1->GetLevelDesc(uLevel, &targetDesc))
			|| FAILED(pTexture2->GetLevelDesc(uLevel, &sourceDesc))
			|| targetDesc.Width != sourceDesc.Width
			|| targetDesc.Height!= sourceDesc.Height
			|| targetDesc.Format!= sourceDesc.Format )
		{
			return	FALSE;
		}
		
		D3DLOCKED_RECT	targetRect;
		D3DLOCKED_RECT	sourceRect;
		if(FAILED(pTexture1->LockRect(uLevel, &targetRect, NULL, D3DLOCK_READONLY)))//D3DLOCK_NO_DIRTY_UPDATE
		{
			return	FALSE;
		}
		if(FAILED(pTexture2->LockRect(uLevel, &sourceRect, NULL, D3DLOCK_READONLY)))
		{
			pTexture1->UnlockRect(uLevel);
			return	FALSE;
		}		
		
		bResult = Surface_DoCompare((CHAR*)targetRect.pBits, (CHAR*&)sourceRect.pBits, targetRect.Pitch, &targetDesc);		
		
		pTexture2->UnlockRect(uLevel);
		pTexture1->UnlockRect(uLevel);

		return bResult;
	}
	BOOL Texture_DoReplace( IDirect3DTexture* pTexture1,IDirect3DTexture* pTexture2,UINT  uLevel)
	{
		ASSERT(pTexture1 != NULL  );
		ASSERT(pTexture2 != NULL  );
		
		BOOL  bResult = NULL;
		
		//
		//宽、高、格式检查
		//
		D3DSURFACE_DESC	targetDesc;
		D3DSURFACE_DESC	sourceDesc;
		if(FAILED(pTexture1->GetLevelDesc(uLevel, &targetDesc))
			|| FAILED(pTexture2->GetLevelDesc(uLevel, &sourceDesc))
			|| targetDesc.Width != sourceDesc.Width
			|| targetDesc.Height!= sourceDesc.Height
			|| targetDesc.Format!= sourceDesc.Format )
		{
			return	FALSE;
		}
		 
		D3DLOCKED_RECT	targetRect;
		D3DLOCKED_RECT	sourceRect;
		pTexture2->AddDirtyRect(NULL);
		if(FAILED(pTexture2->LockRect(uLevel, &targetRect, NULL, D3DLOCK_NO_DIRTY_UPDATE)))
		{
			return	FALSE;
		}
		if(FAILED(pTexture1->LockRect(uLevel, &sourceRect, NULL, D3DLOCK_READONLY)))
		{
			pTexture2->UnlockRect(uLevel);
			return	FALSE;
		}		
		
		bResult = Surface_DoReplace((CHAR*)targetRect.pBits, (CHAR*)sourceRect.pBits, targetRect.Pitch, &targetDesc);		
		
		pTexture1->UnlockRect(uLevel);
		pTexture2->UnlockRect(uLevel);
		
		return bResult;	
	}

	//
	//我们自己创建纹理的时候,担心引起我们拦截的无关调用
	//
	static  BOOL s_bCreating = FALSE;
	BOOL  IsCreatingTexture()
	{
		return s_bCreating;
	}

	/******************************************************************************************
	*            创建纹理
	*   
	*   pDevice       : 创建纹理的s设备 
	*   lpszFileName  : 创建纹理的dds文件名称q全路径
	*   pTexture      : 纹理指针	
	*   返回          :成功失败
	*****************************************************************************************/
    BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture)
	{			
		BOOL  bResult  = FALSE;

		ASSERT( ppTexture != NULL );

		IDirect3DTexture*   pTmpTexture    = NULL;
		if  ( s_pFun_D3DXCreateTextureFromFile != NULL )
		{
			s_bCreating = TRUE;
			HRESULT hr = (*s_pFun_D3DXCreateTextureFromFile)(pDevice, lpszFileName, &pTmpTexture);
			if( SUCCEEDED( hr ) )
			{
				*ppTexture = pTmpTexture;
				bResult = TRUE;
			}
			s_bCreating = FALSE;
		}

		START_LOG;		
		LOG << "创建纹理1 :" << lpszFileName << "  ret = " << (bResult?"TRUE":"FALSE");		
		END_LOG;
		return bResult;			
	}


	/******************************************************************************************
	*            创建纹理
	*   pDevice       : 创建纹理的s设备 
	*   lpszFileName  : 创建纹理的dds文件名称q全路径
	*   pTexture      : 纹理指针
	*   pRefTexture   : 参考的纹理指针
	*   返回          :成功失败
	*****************************************************************************************/
	BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture ,IDirect3DTexture*  pRefTexture)
	{		
		BOOL  bResult = FALSE;

		ASSERT( ppTexture    != NULL );
		ASSERT( pRefTexture != NULL );

		if  ( s_pFun_D3DXCreateTextureFromFileEx == NULL )  return FALSE;

		D3DSURFACE_DESC desc;
		UINT LevelCount = pRefTexture->GetLevelCount();			
		if( LevelCount  = 0 )  return FALSE;

		/*
		获得参考纹理的最大尺寸（0层尺寸数据最大）
		*/
		IDirect3DTexture*   pTmpTexture    = NULL;
		HRESULT hr = pRefTexture->GetLevelDesc( 0, &desc );
		if ( hr == S_OK)
		{	
			s_bCreating = TRUE;
			hr = (*s_pFun_D3DXCreateTextureFromFileEx)(pDevice,lpszFileName,desc.Width,desc.Height, LevelCount,desc.Usage, desc.Format, desc.Pool, (DWORD)D3DX_DEFAULT, (DWORD)D3DX_DEFAULT,(D3DCOLOR)0, NULL, NULL, 	&pTmpTexture);
			if  ( hr == S_OK )
			{
				 *ppTexture = pTmpTexture;
				 bResult  = TRUE;
			}
			s_bCreating = FALSE;

		}
		START_LOG;
		LOG << "创建纹理2 :" << lpszFileName << "  ret = " << (bResult?"TRUE":"FALSE");			
		END_LOG;
		return bResult;
	}

    BOOL  SaveTextureToFile(IDirect3DTexture* pTexture,LPCTSTR   lpszFileName)
	{
		if  ( s_pFun_D3DxSaveTextureToFile == NULL ) return FALSE;

		if(SUCCEEDED((*s_pFun_D3DxSaveTextureToFile)(lpszFileName, D3DXIFF_DDS, pTexture, NULL)))
		{
			return	TRUE;
		}

		return FALSE;
	}

	//保存表面到文件
	BOOL  SaveSurfaceToFile(IDirect3DSurface* pSurface,LPCTSTR   lpszFileName)
	{
		if  ( s_pFun_D3DxSaveSurfaceToFile == NULL ) return FALSE;
		
		if(SUCCEEDED((*s_pFun_D3DxSaveSurfaceToFile)(lpszFileName, D3DXIFF_DDS, pSurface, NULL,NULL)))
		{
			return	TRUE;
		}
		
		return FALSE;
	}


	/*设备引用计数*/		
	VOID  AddDeviceRefCount(LPDIRECT3DDEVICE pDevice, long  lCount)
	{
		s_dev_ref_count[ pDevice ] += lCount;
 		
	}
	VOID  DelDeviceRefCount(LPDIRECT3DDEVICE pDevice )
	{
 		std::map<LPDIRECT3DDEVICE,long>::iterator it = s_dev_ref_count.find(pDevice);
 		if  ( it != s_dev_ref_count.end())  
 			s_dev_ref_count.erase(it);
	}
	
	VOID  SetDeviceRefCount(LPDIRECT3DDEVICE pDevice,   long  lCount)
	{
		s_dev_ref_count[ pDevice ] = lCount;
		//s_dev_ref_count.insert(std::map<LPDIRECT3DDEVICE,DWORD>::value_type(pDevice,dwCount));
	}

	long GetDeviceRefCount( LPDIRECT3DDEVICE pDevice)
	{
        std::map<LPDIRECT3DDEVICE,long>::iterator it = s_dev_ref_count.find(pDevice);
 		if  ( it != s_dev_ref_count.end())  
		{
			#ifdef NEED_OUTPUT_MSG
			LOG<< "设备数量="  << it->second;
			#endif
             return it->second;
		}
		  
		return 0;
	}
// 	
// 	//改变设备引用的函数调用前后添加计算更改的引用计数
// 	DWORD BeginUpdate ( LPDIRECT3DDEVICE pDevice)
// 	{
//         ASSERT(pDevice != NULL );
// 		return pDevice->AddRef();
// 	}
//     VOID  EndUpdate   ( LPDIRECT3DDEVICE pDevice,DWORD  dwCount )
// 	{
// 		ASSERT(pDevice != NULL );
// 		DWORD  dwTmp1 = pDevice->Release();
// 		if ( dwTmp1 < dwCount)
// 		{
// 			dwTmp1 = dwCount - dwTmp1 -1;
// 			if ( dwTmp1 == 0 )  return;
// 
// 			std::map<LPDIRECT3DDEVICE,DWORD>::iterator it = s_dev_ref_count.find(pDevice);
// 			if  ( it != s_dev_ref_count.end())  
// 			{
// 				it->second -= dwTmp1 ;
// 			}		   	   
// 		}
// 		else
// 		{
//            dwTmp1 = dwTmp1 - dwCount +1;
// 		   if ( dwTmp1 == 0 )  return;
// 
// 		   std::map<LPDIRECT3DDEVICE,DWORD>::iterator it = s_dev_ref_count.find(pDevice);
// 		   if  ( it != s_dev_ref_count.end())  
// 		   {
// 			   it->second += dwTmp1 ;
// 		   }		   
// 		}
// 	}
	
	
	//设备 release是的锁和解锁
	VOID  DeviceReleaseLock    (LPDIRECT3DDEVICE pDevice)
	{
		ASSERT(pDevice != NULL );

	}
	VOID  DeviceReleaseUnLock  (LPDIRECT3DDEVICE pDevice)
	{
		ASSERT(pDevice != NULL );
	}


}
