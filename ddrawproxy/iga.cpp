// iga.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"

#include <windowsx.h>

LPDIRECTDRAW7          g_pDD                         =   NULL;   
LPDIRECTDRAWSURFACE7   g_pDDSPrimary                 =   NULL;       
DDCAPS                               g_ddcaps;   

bool   Is555=false;   //   是否是565模式，这个变量需要用者填写  

inline unsigned char GetRed(WORD color)   
{   
	if( Is555 )   
		return (color>>7) & 0xff;   
	else   
		return (color>>8) & 0xff;   
}   

inline   unsigned   char   GetGreen(WORD   color)   
{   
	if(   Is555   )   
		return   (color>>2)   &   0xff;   
	else   
		return   (color>>3)   &   0xff;   
}   

inline   unsigned   char   GetBlue(WORD   color)   
{   
	return   (color   &   0x1f)   <<   3;   
}   

//构造函数
IGA::IGA(LPDIRECTDRAW7 pOriginal)
{	
	m_pIGADraw = pOriginal;
	m_hBitmap = NULL;
}
//析构函数
IGA::~IGA()
{
} 

//   功能：将一个16位的DirectDraw表面，存为一张24位BMP位图   （传入主表面即截屏）   
//   输入：表面指针，输出的文件名   
//   输出：是否成功   
bool   IGA::SaveToBitmapFile(LPDIRECTDRAWSURFACE7   lpSurface,   char*   filename)   
{   
	OutputDebugString("DDRAWPROXY: SaveToBitmapFile.\r\n");
	WORD*   lpBuffer = NULL;             //   表面指针   
	int   nPitch = 0;             //   表面跨距   
	int   nWidth = 0,   nHeight = 0;     //   表面宽高   


//	DWORD RGB555Mask[4] = {0x7C00, 0x03E0, 0x001F, 0};
	DWORD RGB565Mask[4] = {0xF800, 0x07E0, 0x001F, 0};

	//   打开文件s   
	FILE*   fp;   
	if(   (fp=fopen(filename,   "wb"))   !=   NULL   )   
	{   
		//   锁定表面   
		DDSURFACEDESC2   ddsd;   
		ddsd.dwSize   =   sizeof(ddsd);   
		
		HRESULT   ddrval   =   IGA_IDirectDrawSurface7::m_pfnLock(lpSurface, NULL,   &ddsd,   DDLOCK_WAIT,   NULL);// lpSurface->Lock(   NULL,   &ddsd,   DDLOCK_WAIT,   NULL   );   
		if(   ddrval   ==   DD_OK   )   
		{   
			lpBuffer   =   (WORD   *)ddsd.lpSurface;   
			nWidth   =   ddsd.dwWidth;   
			nHeight   =   ddsd.dwHeight;   
			nPitch     =   ddsd.lPitch   >>   1;         //lPitch以Byte为单位，GraphPitch以WORD为单位。所以GraphPitch   =   lPitch   /   2;   
		}   

		//   保存文件头   
		BITMAPFILEHEADER   FileHeader;   
		FileHeader.bfType   =   'MB';
		FileHeader.bfSize   =   nWidth   *   nHeight   *   2   +   0x36;   
		FileHeader.bfReserved1   =   0;   
		FileHeader.bfReserved2   =   0;   
		FileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGB565Mask);   
		fwrite(&FileHeader,   sizeof(BITMAPFILEHEADER),   1,   fp);   

		//   保存文件信息   
		BITMAPINFOHEADER   Header;   
		Header.biSize   =   sizeof(BITMAPINFOHEADER);     //   结构的大小   
		Header.biWidth   =   nWidth;                     //   宽   
		Header.biHeight   =   -nHeight;                     //   高   
		Header.biPlanes   =   1;                         //   固定   
		Header.biBitCount   =   16;                         //   颜色数   
		Header.biCompression   =   BI_BITFIELDS;                 //   是否压缩   
		Header.biSizeImage   =   nWidth   *   nHeight   *   2;     //   图片的大小   
		Header.biXPelsPerMeter   =   0;   
		Header.biYPelsPerMeter   =   0;   
		Header.biClrUsed   =   0;   
		Header.biClrImportant   =   0;   
		fwrite(&Header,   Header.biSize,   1,   fp);   


		fwrite(&RGB565Mask, sizeof(RGB565Mask),   1,   fp);

		//   写入具体内容(从下向上存放)   
		fseek(fp,   FileHeader.bfOffBits,   SEEK_SET);

//		WORD   word;   
	//	lpBuffer   +=   nWidth   *2*   (nHeight   -   1);   
		for(int i=0; i < nHeight;   i++)   
		{   
			fwrite(lpBuffer, sizeof(WORD) * nWidth, 1, fp);

			//for(int   j=0;   j<nWidth;   j++)   
			//{   
			//	//word   =   *lpBuffer;   
			//	//fputc(   GetBlue(   word   ),   fp);     //   蓝   
			//	//fputc(   GetGreen(   word   ),   fp);     //   绿   
			//	//fputc(   GetRed(   word   ),   fp);         //   红   
			//	//lpBuffer++;   
			//}   
		//	lpBuffer   -=3*nWidth   ;     //   指针转到上一行的开始   
			lpBuffer += nPitch;
		}

		fclose(fp);   

		//   解锁表面   
		ddrval   =  IGA_IDirectDrawSurface7::m_pfnUnlock(lpSurface, NULL);   
		return   true;
	}   

	return   false;   
} 


//   功能：将一个16位的DirectDraw表面，存为一张24位BMP位图   （传入主表面即截屏）   
//   输入：表面指针，输出的文件名   
//   输出：是否成功   
bool   IGA::SaveToBitmapFile(LPDIRECTDRAWSURFACE   lpSurface,   char*   filename)   
{   
	OutputDebugString("DDRAWPROXY: SaveToBitmapFile.\r\n");
	WORD*   lpBuffer = NULL;             //   表面指针   
	int   nPitch = 0;             //   表面跨距   
	int   nWidth = 0,   nHeight = 0;     //   表面宽高   


	//	DWORD RGB555Mask[4] = {0x7C00, 0x03E0, 0x001F, 0};
	DWORD RGB565Mask[4] = {0xF800, 0x07E0, 0x001F, 0};

	//   打开文件s   
	FILE*   fp;   
	if(   (fp=fopen(filename,   "wb"))   !=   NULL   )   
	{   
		//   锁定表面   
		DDSURFACEDESC   ddsd;   
		ddsd.dwSize   =   sizeof(ddsd);   

		HRESULT   ddrval   =   IGA_IDirectDrawSurface::m_pfnLock(lpSurface, NULL,   &ddsd,   DDLOCK_WAIT,   NULL);// lpSurface->Lock(   NULL,   &ddsd,   DDLOCK_WAIT,   NULL   );   
		if(   ddrval   ==   DD_OK   )   
		{   
			lpBuffer   =   (WORD   *)ddsd.lpSurface;   
			nWidth   =   ddsd.dwWidth;   
			nHeight   =   ddsd.dwHeight;   
			nPitch     =   ddsd.lPitch   >>   1;         //lPitch以Byte为单位，GraphPitch以WORD为单位。所以GraphPitch   =   lPitch   /   2;   
		}   

		//   保存文件头   
		BITMAPFILEHEADER   FileHeader;   
		FileHeader.bfType   =   'MB';
		FileHeader.bfSize   =   nWidth   *   nHeight   *   2   +   0x36;   
		FileHeader.bfReserved1   =   0;   
		FileHeader.bfReserved2   =   0;   
		FileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGB565Mask);   
		fwrite(&FileHeader,   sizeof(BITMAPFILEHEADER),   1,   fp);   

		//   保存文件信息   
		BITMAPINFOHEADER   Header;   
		Header.biSize   =   sizeof(BITMAPINFOHEADER);     //   结构的大小   
		Header.biWidth   =   nWidth;                     //   宽   
		Header.biHeight   =   -nHeight;                     //   高   
		Header.biPlanes   =   1;                         //   固定   
		Header.biBitCount   =   16;                         //   颜色数   
		Header.biCompression   =   BI_BITFIELDS;                 //   是否压缩   
		Header.biSizeImage   =   nWidth   *   nHeight   *   2;     //   图片的大小   
		Header.biXPelsPerMeter   =   0;   
		Header.biYPelsPerMeter   =   0;   
		Header.biClrUsed   =   0;   
		Header.biClrImportant   =   0;   
		fwrite(&Header,   Header.biSize,   1,   fp);   


		fwrite(&RGB565Mask, sizeof(RGB565Mask),   1,   fp);

		//   写入具体内容(从下向上存放)   
		fseek(fp,   FileHeader.bfOffBits,   SEEK_SET);

		//		WORD   word;   
		//	lpBuffer   +=   nWidth   *2*   (nHeight   -   1);   
		for(int i=0; i < nHeight;   i++)   
		{   
			fwrite(lpBuffer, sizeof(WORD) * nWidth, 1, fp);

			//for(int   j=0;   j<nWidth;   j++)   
			//{   
			//	//word   =   *lpBuffer;   
			//	//fputc(   GetBlue(   word   ),   fp);     //   蓝   
			//	//fputc(   GetGreen(   word   ),   fp);     //   绿   
			//	//fputc(   GetRed(   word   ),   fp);         //   红   
			//	//lpBuffer++;   
			//}   
			//	lpBuffer   -=3*nWidth   ;     //   指针转到上一行的开始   
			lpBuffer += nPitch;
		}

		fclose(fp);   

		//   解锁表面   
		ddrval   =  IGA_IDirectDrawSurface::m_pfnUnlock(lpSurface, NULL);   
		return   true;
	}   

	return   false;   
} 

//显示位图
//第四种读取位图文件来显示位图:
void IGA::LoadImageResource() 
{

	OutputDebugString("DDRAWPROXY: LoadImageResource.\r\n");
	CHAR chOutputString[255];
	sprintf(chOutputString, "LoadImageResource called!");
	OutputDebugString(chOutputString);

	// TODO: Add your control notification handler code here
	/*******************通过读取位图文件来显示位图**********************/
//	CString strFileName;
//	strFileName.Format("%s","NetFriend.bmp");
	//成位图信息BITMAPINFO
//	BITMAPINFO *bitmapinfo=NULL;
//	BYTE *BmpData=NULL;
//	CFile file;//读取位图信息
	//设置文件的打开方式
//	if(!file.Open(strFileName,Cfile::modeRead|Cfile::typeBinary))
//	{
//		return;
//	}
	////BITMAP文件头结构体
	//BITMAPFILEHEADER BitmapHead;
	////读取位图文件头
	//if(file.Read(&BitmapHead,sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER))
	//{
	//	MessageBox("读取文件头失败!");
	//	return;
	//}
	//if(BitmapHead.bfType!=0x4d42)
	//{
	//	MessageBox("对不起,您读取的不是位图文件!");
	//	return;
	//}
	////位图信息
	//BITMAPINFOHEADER  BitmapInfo;
	//if(file.Read(&BitmapInfo,sizeof(BITMAPINFOHEADER))!=sizeof(BITMAPINFOHEADER))
	//{
	//	MessageBox("读取位图信息失败!");
	//	return;
	//}
	//if(BitmapInfo.biBitCount!=24)
	//{
	//	MessageBox("对不起,当前程序只支持24位位图信息!");
	//	return;
	//}
	//bitmapinfo=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
	//if(!bitmapinfo)
	//{
	//	MessageBox("内存分配失败!");
	//	return;
	//}
	///*把BMP位图信息头中的数据读取到位图信息结构中去.*/
	//memcpy(bitmapinfo,&BitmapInfo,sizeof(BITMAPINFOHEADER));
	///*用来得到位图文件的大小*/
	//DWORD dataByte=BitmapHead.bfSize-BitmapHead.bfOffBits;
	//BmpData=(BYTE*)new char[dataByte];
	//if(!BmpData)
	//{
	//	MessageBox("内存分配失败!");
	//	delete bitmapinfo;
	//	delete BmpData;
	//	return;
	//}
	//if(file.Read(BmpData,dataByte)!=dataByte)
	//{
	//	MessageBox("读取位图数据失败!");
	//	return;
	//}
	//file.Close();


	extern HINSTANCE           gl_hOriginalDll;
	extern HINSTANCE           gl_hThisInstance;

	sprintf(chOutputString, "gl_hThisInstance = 0x%p", gl_hThisInstance);
	OutputDebugString(chOutputString);

//	m_hBitmap = (HBITMAP)::LoadImage(gl_hThisInstance, "D:\\a_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	// Use LoadImage() to get the image loaded into a DIBSection
	m_hBitmap = (HBITMAP)LoadImage( NULL, "D:\\a_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmap != NULL)
	{
	//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "图片加载成功@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "图片加载失败(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapB1 = (HBITMAP)LoadImage( NULL, "D:\\b_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapB1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "图片加载成功@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "图片加载失败(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapC1 = (HBITMAP)LoadImage( NULL, "D:\\c_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "图片加载成功@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "图片加载失败(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapD1 = (HBITMAP)LoadImage( NULL, "D:\\d_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "图片加载成功@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "图片加载失败(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapE1 = (HBITMAP)LoadImage( NULL, "D:\\e_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "图片加载成功@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "图片加载失败(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}
//	CClientDC *pDC=new CClientDC(this);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	StretchDIBits(pDC->GetSafeHdc(),200,0,BitmapInfo.biWidth,BitmapInfo.biHeight,  0,0,BitmapInfo.biWidth,BitmapInfo.biHeight,BmpData,bitmapinfo,DIB_RGB_COLORS,SRCCOPY);

}

#define     SelectBitmap(hdc, hbm)  ((HBITMAP)SelectObject((hdc), (HGDIOBJ)(HBITMAP)(hbm)))

bool  IGA::ReplaceSurface(LPDIRECTDRAWSURFACE7 lpSurface, HBITMAP hBitmap)
{
	OutputDebugString("DDRAWPROXY: ReplaceSurface.\r\n");

	CHAR chOutputString[255];

	HRESULT hr;

///***********************************************
	//HBITMAP hBitmap = m_hBitmap;

	//if (m_hBitmap == NULL)
	//{
	//	LoadImageResource();
	//	hBitmap = m_hBitmap;
	//}

	if (lpSurface == NULL || hBitmap == NULL)
		return false;


	///***********************************************

	BYTE* lpBuffer;             //   表面指针
	BYTE* lpBmpBuf;
//	WORD  wPixel = 0;
	int   nPitch = 0;                 //   表面跨距   
	int   nWidth = 0, nHeight = 0;      //   表面宽高   

	DDSURFACEDESC2 ddsd;
//	BITMAP         bm;

	
//	GetObject(hBitmap, sizeof(BITMAP), &bm);
	
	//LONG	nLength = GetBitmapBits(hBitmap, 0, NULL);
	//static CHAR * pp = NULL;
	//if(!pp)
	//{
	//	pp	= new CHAR [ nLength ];
	//	::GetBitmapBits(hBitmap, nLength, pp );
	//	sprintf(chOutputString, "pp: %p, Length: %d\r\n",pp, nLength);
	//	OutputDebugString(chOutputString);
	//}

//	bm.bmWidth;
//	bm.bmHeight;

//	sprintf(chOutputString, "bmWidth : %d, bmHeight : %d", bm.bmWidth, bm.bmHeight);
//	OutputDebugString(chOutputString);


	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);

	hr = IGA_IDirectDrawSurface7::m_pfnGetSurfaceDesc(lpSurface, &ddsd);
	if (SUCCEEDED(hr))
	{
		nWidth     =  ddsd.dwWidth;
		nHeight    =  ddsd.dwHeight;

	//	sprintf(chOutputString, "nWidth : %d, nHeight : %d", nWidth, nHeight);
	//	OutputDebugString(chOutputString);

		HDC hSurface = NULL;
		hr = IGA_IDirectDrawSurface7::m_pfnGetDC(lpSurface, &hSurface);
		if (FAILED(hr))
		{
			sprintf(chOutputString, "GetDC 失败 (hr = 0x%08X)", hr);
			OutputDebugString(chOutputString);
			return false;
		}

	//	sprintf(chOutputString, "hSurface : 0x%p", hSurface);
	//	OutputDebugString(chOutputString);


		HBITMAP hPrevBitmap;
		HDC hSrcDC = ::CreateCompatibleDC(hSurface);

		hPrevBitmap = SelectBitmap(hSrcDC, hBitmap);

		BitBlt(hSurface, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hSrcDC, 0, 0, SRCCOPY);

		SelectBitmap(hSrcDC, hPrevBitmap);

		IGA_IDirectDrawSurface7::m_pfnReleaseDC(lpSurface, hSurface);
	}
	return true;
}


