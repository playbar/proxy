// DEMOII9_3.CPP - 3D textured cube rotating with flat shading and lights
// READ THIS!
// To compile make sure to include DDRAW.LIB, DSOUND.LIB,
// DINPUT.LIB, DINPUT8.LIB, WINMM.LIB in the project link list, and of course 
// the C++ source modules T3DLIB1-7.CPP and the headers T3DLIB1-7.H
// be in the working directory of the compiler

// INCLUDES ///////////////////////////////////////////////

#define DEBUG_ON

#define INITGUID       // make sure al the COM interfaces are available
// instead of this you can include the .LIB file
// DXGUID.LIB

#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h> 
#include <malloc.h> 
#include <memory.h>
#include <string.h>  
#include <stdarg.h>
#include <stdio.h>   
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <KS.H>


#include <ddraw.h>  // directX includes 
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <dinput.h>
#include "T3DLIB1.h" // game library includes
#include "T3DLIB2.h"
#include "T3DLIB3.h"
#include "T3DLIB4.h"
#include "T3DLIB5.h"
#include "T3DLIB6.h"
#include "T3DLIB7.h"



// DEFINES ////////////////////////////////////////////////

// defines for windows interface
#define WINDOW_CLASS_NAME "WIN3DCLASS"  // class name
#define WINDOW_TITLE      "T3D Graphics Console Ver 2.0"
#define WINDOW_WIDTH      800  // size of window
#define WINDOW_HEIGHT     600

#define WINDOW_BPP        16    // bitdepth of window (8,16,24 etc.)
// note: if windowed and not
// fullscreen then bitdepth must
// be same as system bitdepth
// also if 8-bit the a pallete
// is created and attached

#define WINDOWED_APP      0   // 0 not windowed, 1 windowed

// create some constants for ease of access
#define AMBIENT_LIGHT_INDEX   0 // ambient light index
#define INFINITE_LIGHT_INDEX  1 // infinite light index
#define POINT_LIGHT_INDEX     2 // point light index
#define SPOT_LIGHT1_INDEX     4 // point light index
#define SPOT_LIGHT2_INDEX     3 // spot light index

// PROTOTYPES /////////////////////////////////////////////

BITMAP_FILE      bitmap;  

// game console
int Game_Init(void *parms=NULL);
int Game_Shutdown(void *parms=NULL);
int Game_Main(void *parms=NULL);

#define DIB_HEADER_MARKER	((WORD) ('M' << 8) | 'B') 

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle           = NULL; // save the window handle
HINSTANCE main_instance           = NULL; // save the instance
char buffer[2048];                        // used to print text

// initialize camera position and direction
POINT4D  cam_pos    = {0,0,0,1};
POINT4D  cam_target = {0,0,0,1};
VECTOR4D cam_dir    = {0,0,0,1};

// all your initialization code goes here...
VECTOR4D vscale={1.0,1.0,1.0,1}, 
vpos = {0,0,0,1}, 
vrot = {0,0,0,1};

CAM4DV1        cam;       // the single camera

OBJECT4DV2     obj;

RENDERLIST4DV2 rend_list; // the render list

RGBAV1 white, gray, black, red, green, blue;

// FUNCTIONS //////////////////////////////////////////////

UCHAR * g_zoomBmpBuffer =  NULL;

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) // for padding
#define HDIB				HANDLE

HANDLE ReadDIBFile(HANDLE hFile) 
{ 
	BITMAPFILEHEADER    bmfHeader; 
	DWORD               dwBitsSize; 
	UINT                nNumColors;   // Number of colors in table 
	HANDLE              hDIB;         
	HANDLE              hDIBtmp;      // Used for GlobalRealloc() //MPB 
	LPBITMAPINFOHEADER  lpbi; 
	DWORD               offBits; 
	DWORD               dwRead; 

	// get length of DIB in bytes for use when reading 

	dwBitsSize = GetFileSize(hFile, NULL); 

	// Allocate memory for header & color table. We'll enlarge this 
	// memory as needed. 
	//int i  = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	//hDIB = GlobalAlloc(GMEM_MOVEABLE,90 ); 
	//GlobalFree(hDIB);

	hDIB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD))); 

	if (!hDIB) 
		return NULL; 

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 

	if (!lpbi)  
	{ 
		GlobalFree(hDIB); 
		return NULL; 
	} 

	// read the BITMAPFILEHEADER from our file 

	if (!ReadFile(hFile, (LPBYTE)&bmfHeader, sizeof (BITMAPFILEHEADER), 
		&dwRead, NULL)) 
		goto ErrExit; 

	if (sizeof (BITMAPFILEHEADER) != dwRead) 
		goto ErrExit; 

	if (bmfHeader.bfType != 0x4d42)  // 'BM' 
		goto ErrExit; 

	// read the BITMAPINFOHEADER 

	if (!ReadFile(hFile, (LPBYTE)lpbi, sizeof(BITMAPINFOHEADER), &dwRead, 
		NULL)) 
		goto ErrExit; 

	if (sizeof(BITMAPINFOHEADER) != dwRead) 
		goto ErrExit; 

	// Check to see that it's a Windows DIB -- an OS/2 DIB would cause 
	// strange problems with the rest of the DIB API since the fields 
	// in the header are different and the color table entries are 
	// smaller. 
	// 
	// If it's not a Windows DIB (e.g. if biSize is wrong), return NULL. 

	if (lpbi->biSize == sizeof(BITMAPCOREHEADER)) 
		goto ErrExit; 

	// Now determine the size of the color table and read it.  Since the 
	// bitmap bits are offset in the file by bfOffBits, we need to do some 
	// special processing here to make sure the bits directly follow 
	// the color table (because that's the format we are susposed to pass 
	// back) 

	if (!(nNumColors = (UINT)lpbi->biClrUsed)) 
	{ 
		// no color table for 24-bit, default size otherwise 

		if (lpbi->biBitCount > 8)	//(lpbi->biBitCount != 24) 
			nNumColors = 1 << lpbi->biBitCount; // standard size table 
	} 

	// fill in some default values if they are zero 

	if (lpbi->biClrUsed == 0) 
		lpbi->biClrUsed = nNumColors; 

	if (lpbi->biSizeImage == 0) 
	{ 
		lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 
			31) & ~31) >> 3) * lpbi->biHeight; 
	} 

	// get a proper-sized buffer for header, color table and bits 

	GlobalUnlock(hDIB); 
	hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * 
		sizeof(RGBQUAD) + lpbi->biSizeImage, 0); 

	if (!hDIBtmp) // can't resize buffer for loading 
		goto ErrExitNoUnlock; //MPB 
	else 
	{
		//GlobalFree(hDIB);
		hDIB = hDIBtmp; 
	}
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 

	// read the color table 

	ReadFile (hFile, (LPBYTE)(lpbi) + lpbi->biSize, 
		nNumColors * sizeof(RGBQUAD), &dwRead, NULL); 

	// offset to the bits from start of DIB header 

	offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD); 

	// If the bfOffBits field is non-zero, then the bits might *not* be 
	// directly following the color table in the file.  Use the value in 
	// bfOffBits to seek the bits. 

	if (bmfHeader.bfOffBits != 0L) 
		SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN); 

	if (ReadFile(hFile, (LPBYTE)lpbi + offBits, lpbi->biSizeImage, &dwRead, 
		NULL)) 
		goto OKExit; 

ErrExit: 
	GlobalUnlock(hDIB);     

ErrExitNoUnlock:     
	GlobalFree(hDIB); 
	return NULL; 

OKExit: 
	GlobalUnlock(hDIB); 
	return hDIB; 
} 

HDIB LoadDIB(LPCTSTR lpFileName) 
{ 
	HDIB        hDIB; 
	HANDLE      hFile; 

	// Set the cursor to a hourglass, in case the loading operation 
	// takes more than a sec, the user will know what's going on. 

	SetCursor(LoadCursor(NULL, IDC_WAIT)); 

	if ((hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
		NULL)) != INVALID_HANDLE_VALUE) 
	{ 
		hDIB = ReadDIBFile(hFile); 
		CloseHandle(hFile); 
		SetCursor(LoadCursor(NULL, IDC_ARROW)); 
		return hDIB; 
	} 
	else 
	{ 
		SetCursor(LoadCursor(NULL, IDC_ARROW)); 
		return NULL; 
	} 
} 
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 

WORD DIBNumColors(LPBYTE lpDIB) 
{ 
	WORD wBitCount;  // DIB bit count 

	// If this is a Windows-style DIB, the number of colors in the 
	// color table can be less than the number of bits per pixel 
	// allows for (i.e. lpbi->biClrUsed can be set to some value). 
	// If this is the case, return the appropriate value. 


	if (IS_WIN30_DIB(lpDIB)) 
	{ 
		DWORD dwClrUsed; 

		dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed; 
		if (dwClrUsed) 

			return (WORD)dwClrUsed; 
	} 

	if (IS_WIN30_DIB(lpDIB)) 
		wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
	else 
		wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 

	// return number of colors based on bits per pixel 

	switch (wBitCount) 
	{ 
	case 1: 
		return 2; 

	case 4: 
		return 16; 

	case 8: 
		return 256; 

	default: 
		return 0; 
	} 
} 
WORD PaletteSize(LPBYTE lpDIB) 
{ 
	// calculate the size required by the palette 
	if (IS_WIN30_DIB (lpDIB)) 
		return (DIBNumColors(lpDIB) * sizeof(RGBQUAD)); 
	else 
		return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 


BOOL WriteDIBFile(HDIB hDib, HANDLE fh) 
{ 
	BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
	LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
	DWORD               dwDIBSize; 
	DWORD               dwWritten; 

	if (!hDib) 
		return FALSE; 

	// Get a pointer to the DIB memory, the first of which contains 
	// a BITMAPINFO structure 

	lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	if (!lpBI) 
	{ 
		CloseHandle(fh); 
		return FALSE; 
	} 

	// Check to see if we're dealing with an OS/2 DIB.  If so, don't 
	// save it because our functions aren't written to deal with these 
	// DIBs. 

	if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
	{ 
		GlobalUnlock(hDib); 
		CloseHandle(fh); 
		return FALSE; 
	} 

	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 


	dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   

	// Now calculate the size of the image 

	// It's an RLE bitmap, we can't calculate size, so trust the biSizeImage 
	// field 

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
		dwDIBSize += lpBI->biSizeImage; 
	else 
	{ 
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only 

		// It's not RLE, so size is Width (DWORD aligned) * Height 

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
			lpBI->biHeight; 

		dwDIBSize += dwBmBitsSize; 
		lpBI->biSizeImage = dwBmBitsSize; 
	} 


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER) 

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 

	// Now, calculate the offset the actual bitmap bits will be in 
	// the file -- It's the Bitmap file header plus the DIB header, 
	// plus the size of the color table. 

	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
		PaletteSize((LPBYTE)lpBI); 

	// Write the file header 

	WriteFile(fh, (LPBYTE)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

	// Write the DIB header and the bits -- use local version of 
	// MyWrite, so we can write more than 32767 bytes of data 

	WriteFile(fh, (LPBYTE)lpBI, dwDIBSize, &dwWritten, NULL); 

	GlobalUnlock(hDib); 

	if (dwWritten == 0) 
		return FALSE; // oops, something happened in the write 
	else 
		return TRUE; // Success code 
} 

BOOL SaveDIB(HDIB hDib, LPCTSTR lpFileName) 
{ 
	HANDLE              fh;         // file handle for opened file 

	if (!hDib) 
		return FALSE; 

	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE; 

	BOOL bSuccess = WriteDIBFile(hDib, fh);

	CloseHandle(fh);
	return bSuccess;
}
///////////////////////////////////////////////////////////////////
// Function name    : ZoomOutBmp
// Description      : creates a new bitmap which is a grayscale
//                    zoomed out version of the original
// Return type      : HDIB - handle to a new bitmap
// Argument         : double zoom - number of times to zoom out
// Argument         : HDIB hSrcDIB - handle to a source bitmap
///////////////////////////////////////////////////////////////////
HDIB ZoomOutBmp(double zoom, HDIB hSrcDIB)
{
	if (hSrcDIB == NULL) // nothing to do
		return NULL;

	if (zoom < 1) // no zoomin in this function
		return NULL;

	LPSTR pSrcDIB = (LPSTR) ::GlobalLock((HGLOBAL) hSrcDIB);

	BITMAPINFOHEADER& bmihSrc = *(BITMAPINFOHEADER*)pSrcDIB;
	//ASSERT(bmihSrc.biBitCount == 1); // only monochrome bitmaps supported
	LPSTR pSrcBits = (LPSTR) (pSrcDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*2);

	BITMAPINFOHEADER bmihDst = bmihSrc;
	bmihDst.biWidth = (LONG)(bmihDst.biWidth / zoom + 0.5);
	bmihDst.biHeight = (LONG)(bmihDst.biHeight / zoom + 0.5);
	bmihDst.biBitCount = 8; // grayscale in any case
	bmihDst.biClrUsed = 0;

	// prepare destination bitmap
	DWORD dwDIBSize = sizeof(bmihDst) + sizeof(RGBQUAD)*256 +
		WIDTHBYTES(bmihDst.biWidth * bmihDst.biBitCount) * bmihDst.biHeight;

	bmihDst.biSizeImage = dwDIBSize;

	// allocate space for the new bitmap
	HDIB hDstDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIBSize);

	if (hDstDIB == 0) {
		::GlobalUnlock((HGLOBAL) hSrcDIB);
		return NULL;
	}

	LPSTR pDstDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDstDIB);

	// copy header
	memcpy(pDstDIB, &bmihDst, sizeof(bmihDst));

	// prepare grayscale palette
	int i = 0;
	for ( i=0; i < (1 << bmihDst.biBitCount); i++) 
	{
		RGBQUAD& palEntry = *(RGBQUAD*)(pDstDIB + sizeof(bmihDst) + i * sizeof(RGBQUAD));
		palEntry.rgbRed = palEntry.rgbGreen = palEntry.rgbBlue = i;
	}

	LPSTR pDstBits = (LPSTR) (pDstDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256);

	// now fill the bits
	LPSTR curSrcLineBits, curDstLineBits;
	int j, k;
	int scale = (int)(zoom + 0.5); // integer zoom out factor, i.e. 1:5
	int hBase, vBase;
	unsigned char value;

	// for every _scale_ lines in a source bitmap we will get one line
	// in the destination bitmap. Similarly for _scale_ columns in the
	// source we'll obtain one destination column.

	for (int strip=0; strip < bmihDst.biHeight; strip++) 
	{ // for every dst line

		curDstLineBits = pDstBits + strip * WIDTHBYTES(bmihDst.biWidth * bmihDst.biBitCount);
		vBase = int(strip * zoom + 0.5);

		for (i=0; i < scale; i++) 
		{  // accumulate _scale_ rows

			curSrcLineBits = pSrcBits + (vBase + i) * WIDTHBYTES(bmihSrc.biWidth * bmihSrc.biBitCount);

			// prepare horizontally condensed lines for this strip
			for (j=0; j < bmihDst.biWidth; j++) 
			{ // for all bits in line

				hBase = int(j * zoom + 0.5); // mapped index on source
				for (k=0; k < scale; k++) 
				{ // accumulate _scale_ columns

					value = (curSrcLineBits[(hBase+k)/8] & (1 << (7 - (hBase+k)%8))) ? 0xff : 0;
					curDstLineBits[j] += value / scale / scale; // main accumulator
				}
			}
		}

	}

	// unlock memory
	::GlobalUnlock((HGLOBAL) hSrcDIB);
	::GlobalUnlock((HGLOBAL) hDstDIB);

	return hDstDIB;
}

void StrechPixels(int N1, int N2, UCHAR* src_pixels, UCHAR * dest_pixels)
{
	//ASSERT(N1 <= N2);            // N1 must <= N2

	int p1 = 0, count = 0;
	for (int p2 = 0; p2 < N2; p2++)
	{
		dest_pixels[p2] = src_pixels[p1];
		count += N1;
		while (count >= N2)
		{
			count -= N2;
			p1++;
		}
	}
}




bool ZoomBmpBuffer(UCHAR * pInData, int nSrcWidth, int nSrcHeigth,int bpp, UCHAR * pOutData, int nDstWidth, int nDstHeight)
{

	int i = 0;
	for (int yh = 0; yh < nDstHeight / 2; yh ++)
	{

		UCHAR * pOutDataTemp = &pOutData[yh * nDstWidth * 3];
		if (i ==0)
		{
			i = 1;
			memset(pOutDataTemp, 0x00, nDstWidth *3  );
		}
		else
		{
			i = 0;
			memset(pOutDataTemp, 0xff, nDstWidth *3 );
		}
		/*UCHAR * pInDataTemp =  &pInData[yh * nSrcWidth * 3];
		UCHAR * pOutDataTemp = &pOutData[yh * nDstWidth * 3];
		memcpy(pOutDataTemp, pInDataTemp, yh * nDstWidth * 3);*/
	}

	//memset(pOutData, 0, nDstWidth *3 * 28);
	//UCHAR * pOutTemp = &pOutData[nDstWidth * 3 *28];
	//memset(pOutTemp, 0xff, nDstWidth *3  * 100);

	//////////////////////////////////////////////////////////////////////////
	/*
	double tmpwscale, tmphscale;
	int int_ImgPixByte = 3;

	tmpwscale = (double)(nSrcWidth) / (double)(nDstWidth);
	tmphscale = (double)(nSrcHeigth) /(double)(nDstHeight);

	double   tempnewx,tempnewy;   
	double   tempdiffx,tempdiffy;   
	int   tempnewxi,tempnewyi;   
	double   tempf00,tempf10,tempf01,tempf11;   
	double   temprgb;   
	int   tmprgbint;   
	UCHAR   *OldScanLine, *OldAddScan, *NewScanLine;   
	//UCHAR * pInDataTemp = (UCHAR *) pInData;
	//UCHAR * pOutDataTemp = (UCHAR *) pOutData;
	int   Multi_old;   
	int   Multi_new; 

	for(int   i=0;i< nDstHeight; i++)   
	{   
		tempnewy   =  double(i)*tmphscale;   
		if(tempnewy<0.0)   
			tempnewy=0.0;   
		else if(tempnewy >= nSrcHeigth)   
			tempnewy = nSrcHeigth-1;   

		tempnewyi   =   floor(tempnewy);   
		tempdiffy   =   tempnewy-tempnewyi;  

		OldScanLine = &pInData[tempnewyi];
		OldAddScan  = &pInData[tempnewyi + 1];
		NewScanLine = &pOutData[i]; 

		//OldScanLine  = static_cast<UCHAR*>(pInData[tempnewyi]);  //  static_cast<Byte*>(ShowPicImage->Picture->Bitmap->ScanLine[tempnewyi]);   
		//OldAddScan   = static_cast<UCHAR*>(pInData[tempnewyi + 1]);  //static_cast<Byte*>(ShowPicImage->Picture->Bitmap->ScanLine[tempnewyi+1]);   
		//NewScanLine  = static_cast<UCHAR*>(pOutData[i]);  //static_cast<Byte*>(newbmp->ScanLine[i]);   

		for(int   j=0;j< nSrcWidth; j++)   
		{   
			//Application->ProcessMessages();   
			tempnewx   =   double(j)*tmpwscale;   
			if(tempnewx<0.0)   
				tempnewx=0.0;   
			else if(tempnewx>=nSrcWidth)   
				tempnewx= nSrcWidth - 1;

			tempnewxi   =   floor(tempnewx);   
			tempdiffx   =   tempnewx-tempnewxi;   

			Multi_old   =   tempnewxi   *  3;   // int_ImgPixByte;   
			Multi_new   =   j   * 3;            //  int_ImgPixByte;   
			//tmpmulti   =   j   *   int_ImgPixByte;   
			for(int   k=0;k<3;k++)   
			{   
				//Ë«ÏßÐÔ²åÖµ   
				//Application->ProcessMessages();   
				tempf00   =   OldScanLine[Multi_old+k];   
				if(( tempnewyi < nSrcHeigth - 1 )&& (tempnewxi<nSrcWidth - 1))   
				{   
					tempf10   =   OldScanLine[Multi_old+k+int_ImgPixByte];   
					tempf01   =   OldAddScan[Multi_old+k];   
					tempf11   =   OldAddScan[Multi_old+k+int_ImgPixByte];   
				}   
				else   if((tempnewyi==nSrcHeigth-1)&&(tempnewxi<nSrcWidth-1))   
				{   
					tempf10   =   OldScanLine[Multi_old+k+int_ImgPixByte];   
					tempf01   =   0.0;   
					tempf11   =   0.0;   
					tempdiffy   =   0.0;   
				}   
				else   if((tempnewyi<nSrcHeigth-1)&&(tempnewxi==nSrcWidth-1))   
				{   
					tempf10   =   0.0;   
					tempf01   =   OldAddScan[Multi_old+k];   
					tempf11   =   0.0;   
					tempdiffx   =   0.0;   
				}   
				else   
				{   
					tempf10=0.0;   
					tempf01=0.0;   
					tempf11=0.0;   
					tempdiffx=0.0;   
					tempdiffy=0.0;   
				}   
				temprgb   =(tempf10-tempf00)*tempdiffx   
					+(tempf01-tempf00)*tempdiffy   
					+(tempf11+tempf00-tempf01-tempf10)*tempdiffx*tempdiffy   
					+tempf00;   
				tmprgbint   =   floor(   temprgb+0.5   );   
				NewScanLine[Multi_new+k]   =   tmprgbint;   
			}   
		}   
	}   

*/
	//////////////////////////////////////////////////////////////////////////
	//for (int xH = 0; xH < nDstHeight; xH ++)
	//{

	//	StrechPixels(nDstWidth, nSrcWidth, pInDataTemp, pOutDataTemp);
	//	//memcpy(pOutDataTemp, pInDataTemp, nDstWidth * 3);
	//	pOutDataTemp = pOutDataTemp + nDstWidth * 3;
	//	pInDataTemp = pInDataTemp + nSrcWidth * 3;
	//	//for (int yW = 0; yW < nDstWidth ; yW ++)
	//	//{
	//	//	//UCHAR blue  = pInDataTemp[ xH * nSrcWidth*3 + yW * 3 + 0 ] >>3;
	//	//	//UCHAR green = pInDataTemp[ xH * nSrcWidth*3 + yW * 3 + 1 ] >>2;
	//	//	////UCHAR red   = pInDataTemp[ xH * nSrcWidth*3 + yW * 3 + 2 ] >>3;
	//	//	//USHORT pixel = _RGB16BIT565(red, green, blue );
	//	//	//pOutDataTemp[ xH * nDstWidth *2 + yW *2    ] = pixel;
	//	//	//pOutDataTemp[ xH * nDstWidth *2 + yW *2  + 0  ] = blue;
	//	//	//pOutDataTemp[ xH * nDstWidth *2 + yW *2  + 1  ] = red;
	//	//	pOutDataTemp[xH * nDstWidth *3 + yW *3 + 0 ] = pInDataTemp[xH * nSrcWidth*3 + yW * 3 + 0] ;
	//	//	pOutDataTemp[xH * nDstWidth *3 + yW *3 + 1 ] = pInDataTemp[xH * nSrcWidth*3 + yW * 3 + 1 ] ;
	//	//	pOutDataTemp[xH * nDstWidth *3 + yW *3 + 2 ] = pInDataTemp[xH * nSrcWidth*3 + yW * 3 + 2] ;
	//	//	

	//	//}
	//}

	//pOutDataTemp[128 * 129] = 0x00;
	//memcpy(pOutData, pInData, 128* 128 );
	//int hscale = (int)(nSrcHeigth / nDstHeight + 0.5); 
	//int wscale = (int)(nSrcWidth / nDstWidth + 0.5);

	//UCHAR * curSrcLineBits = NULL;
	//UCHAR * curDstLineBits = NULL;
	//int   hBase = 0, vBase = 0;
	//unsigned char value = 0;

	//int i = 0;
	//for (int xH=0; xH < nDstHeight; xH++) 
	//{ 
	//	curDstLineBits = pOutData + xH * WIDTHBYTES(nDstWidth * bpp >> 3);
	//	curSrcLineBits = pInData +  xH * WIDTHBYTES(nSrcWidth * bpp >> 3);
	//	memcpy(curDstLineBits, curSrcLineBits,WIDTHBYTES(nDstWidth * bpp >> 3 ) );
	//	// prepare horizontally condensed lines for this strip
	//	//for (int yW=0; yW < nDstWidth; yW++) 
	//	//{ 
	//	//	//value  = curSrcLineBits[yW ];
	//	//	//curDstLineBits[yW] += value  ; 
	//	//	memcpy(curDstLineBits, curSrcLineBits, 3);
	//	//}

	//}

	return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, 
							UINT msg, 
							WPARAM wparam, 
							LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   // handle to a device context

	// what is the message 
	switch(msg)
	{	
	case WM_CREATE: 
		{
			// do initialization stuff here
			return(0);
		} break;

	case WM_PAINT:
		{
			// start painting
			hdc = BeginPaint(hwnd,&ps);

			// end painting
			EndPaint(hwnd,&ps);
			return(0);
		} break;

	case WM_DESTROY: 
		{
			// kill the application			
			PostQuitMessage(0);
			return(0);
		} break;

	default:break;

	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
				   HINSTANCE hprevinstance,
				   LPSTR lpcmdline,
				   int ncmdshow)
{
	// this is the winmain function

	WNDCLASS winclass;	// this will hold the class we create
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	PAINTSTRUCT ps;     // generic paintstruct

	// first fill in the window class stucture
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);



	// create the window, note the test to see if WINDOWED_APP is
	// true to select the appropriate window flags
	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
		WINDOW_TITLE,	 // title
		(WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)),
		0,0,	   // x,y
		WINDOW_WIDTH,  // width
		WINDOW_HEIGHT, // height
		NULL,	   // handle to parent 
		NULL,	   // handle to menu
		hinstance,// instance
		NULL)))	// creation parms
		return(0);

	// save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance      = hinstance;

	// resize the window so that client is really width x height
	if (WINDOWED_APP)
	{
		// now resize the window, so the client area is the actual size requested
		// since there may be borders and controls if this is going to be a windowed app
		// if the app is not windowed then it won't matter
		RECT window_rect = {0,0,WINDOW_WIDTH-1,WINDOW_HEIGHT-1};

		// make the call to adjust window_rect
		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(main_window_handle),
			GetMenu(main_window_handle) != NULL,  
			GetWindowExStyle(main_window_handle));

		// save the global client offsets, they are needed in DDraw_Flip()
		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;

		// now resize the window with a call to MoveWindow()
		MoveWindow(main_window_handle,
			0, // x position
			0, // y position
			window_rect.right - window_rect.left, // width
			window_rect.bottom - window_rect.top, // height
			FALSE);

		// show the window, so there's no garbage on first render
		ShowWindow(main_window_handle, SW_SHOW);
	} // end if windowed

	// perform all game console specific initialization
	Game_Init();

	// disable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	// enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if

		// main game processing goes here
		Game_Main();

	} // end while

	// shutdown game and release all resources
	Game_Shutdown();

	// enable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE, NULL, 0);

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

// T3D II GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

void InitOBJECT4DV2()
{

	VECTOR4D_INITXYZ(&vscale,20.00,20.00,20.00);
	Load_OBJECT4DV2_COB(&obj,"cube_flat_textured_01_my.cob",  
		&vscale, &vpos, &vrot, VERTEX_FLAGS_SWAP_YZ | 
		VERTEX_FLAGS_TRANSFORM_LOCAL | 
		VERTEX_FLAGS_TRANSFORM_LOCAL_WORLD );

}

int Game_Init(void *parms)
{
	// this function is where you do all the initialization 
	// for your game

	int index = 0; // looping var

	g_zoomBmpBuffer = new UCHAR[128 * 128 * 3];
	// start up DirectDraw (replace the parms as you desire)
	DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOWED_APP);

	// initialize directinput
	DInput_Init();

	// acquire the keyboard 
	DInput_Init_Keyboard();

	// add calls to acquire other directinput devices here...

	// initialize directsound and directmusic
	DSound_Init();
	DMusic_Init();

	// hide the mouse
	if (!WINDOWED_APP)
		ShowCursor(FALSE);

	// seed random number generator
	srand(Start_Clock()); 

	Load_Bitmap_File( &bitmap, "test24.bmp");

	Open_Error_File("ERROR.TXT");

	// initialize math engine
	Build_Sin_Cos_Tables();

	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // the euler model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		&cam_target,      // no target
		200.0,      // near and far clipping planes
		12000.0,
		120.0,      // field of view in degrees
		WINDOW_WIDTH,   // size of final screen viewport
		WINDOW_HEIGHT);


	// load flat shaded cube
	InitOBJECT4DV2();
	
	//ZoomBmpBuffer(bitmap.buffer, bitmap.bitmapinfoheader.biWidth, bitmap.bitmapinfoheader.biHeight,
	//			24, g_zoomBmpBuffer, 128, 128);

	//obj_flat_cube.texture->buffer = g_zoomBmpBuffer;
	obj.texture->buffer = bitmap.buffer;
	//obj_flat_cube.texture->bpp	  = bitmap.bitmapinfoheader.biBitCount <<1 ;




	// set up lights
	Reset_Lights_LIGHTV1();

	// create some working colors
	white.rgba = _RGBA32BIT(255,255,255,0);
	gray.rgba  = _RGBA32BIT(100,100,100,0);
	black.rgba = _RGBA32BIT(0,0,0,0);
	red.rgba   = _RGBA32BIT(255,0,0,0);
	green.rgba = _RGBA32BIT(0,255,0,0);
	blue.rgba  = _RGBA32BIT(0,0,255,0);

	// ambient light
	Init_Light_LIGHTV1(AMBIENT_LIGHT_INDEX,   
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_AMBIENT,  // ambient light type
		gray, black, black,    // color for ambient term only
		NULL, NULL,            // no need for pos or dir
		0,0,0,                 // no need for attenuation
		0,0,0);                // spotlight info NA

	VECTOR4D dlight_dir = {-1,0,-1,0};

	// directional light
	Init_Light_LIGHTV1(INFINITE_LIGHT_INDEX,  
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_INFINITE, // infinite light type
		black, gray, black,    // color for diffuse term only
		NULL, &dlight_dir,     // need direction only
		0,0,0,                 // no need for attenuation
		0,0,0);                // spotlight info NA


	VECTOR4D plight_pos = {0,200,0,0};

	// point light
	Init_Light_LIGHTV1(POINT_LIGHT_INDEX,
		LIGHTV1_STATE_ON,      // turn the light on
		LIGHTV1_ATTR_POINT,    // pointlight type
		black, green, black,   // color for diffuse term only
		&plight_pos, NULL,     // need pos only
		0,.001,0,              // linear attenuation only
		0,0,1);                // spotlight info NA


	VECTOR4D slight2_pos = {0,200,0,0};
	VECTOR4D slight2_dir = {-1,0,-1,0};

	// spot light2
	Init_Light_LIGHTV1(SPOT_LIGHT2_INDEX,
		LIGHTV1_STATE_ON,         // turn the light on
		LIGHTV1_ATTR_SPOTLIGHT2,  // spot light type 2
		black, red, black,      // color for diffuse term only
		&slight2_pos, &slight2_dir, // need pos only
		0,.001,0,                 // linear attenuation only
		0,0,1);    


	// create lookup for lighting engine
	RGB_16_8_IndexedRGB_Table_Builder(DD_PIXEL_FORMAT565,  // format we want to build table for
		palette,             // source palette
		rgblookup);          // lookup table


	// return success
	return(1);

} // end Game_Init

///////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	// shut everything down

	// release all your resources created for the game here....

	// now directsound
	DSound_Stop_All_Sounds();
	DSound_Delete_All_Sounds();
	DSound_Shutdown();

	// directmusic
	DMusic_Delete_All_MIDI();
	DMusic_Shutdown();

	// shut down directinput
	DInput_Release_Keyboard();

	// shutdown directinput
	DInput_Shutdown();

	// shutdown directdraw last
	DDraw_Shutdown();

	Close_Error_File();

	Unload_Bitmap_File(&bitmap);

	// return success
	return(1);
} // end Game_Shutdown

//////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	static MATRIX4X4 mrot;   // general rotation matrix

	// these are used to create a circling camera
	static float view_angle = 0; 
	static float camera_distance = 6000;
	static VECTOR4D pos = {0,0,0,0};
	static float tank_speed;
	static float turning = 0;
	// state variables for different rendering modes and help
	static int wireframe_mode = 1;
	static int backface_mode  = 1;
	static int lighting_mode  = 1;
	static int help_mode      = 1;
	static int zsort_mode     = 1;

	char work_string[256]; // temp string

	int index; // looping var

	// start the timing clock
	Start_Clock();

	// clear the drawing surface 
	DDraw_Fill_Surface(lpddsback, 0);

	// draw the sky
	//Draw_Rectangle(0,0, WINDOW_WIDTH, WINDOW_HEIGHT/2, RGB16Bit(0,35,50), lpddsback);

	// draw the ground
	//Draw_Rectangle(0,WINDOW_HEIGHT/2-1, WINDOW_WIDTH, WINDOW_HEIGHT, RGB16Bit(20,12,0), lpddsback);

	// read keyboard and other devices here
	DInput_Read_Keyboard();

	// game logic here...

	// reset the render list
	Reset_RENDERLIST4DV2(&rend_list);

	// modes and lights

	// wireframe mode
	if (keyboard_state[DIK_W])
	{
		// toggle wireframe mode
		if (++wireframe_mode > 1)
			wireframe_mode=0;
		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// backface removal
	if (keyboard_state[DIK_B])
	{
		// toggle backface removal
		backface_mode = -backface_mode;
		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// lighting
	if (keyboard_state[DIK_L])
	{
		// toggle lighting engine completely
		lighting_mode = -lighting_mode;
		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// toggle ambient light
	if (keyboard_state[DIK_A])
	{
		// toggle ambient light
		if (lights[AMBIENT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[AMBIENT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;

		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// toggle infinite light
	if (keyboard_state[DIK_I])
	{
		// toggle ambient light
		if (lights[INFINITE_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[INFINITE_LIGHT_INDEX].state = LIGHTV1_STATE_ON;

		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// toggle point light
	if (keyboard_state[DIK_P])
	{
		// toggle point light
		if (lights[POINT_LIGHT_INDEX].state == LIGHTV1_STATE_ON)
			lights[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[POINT_LIGHT_INDEX].state = LIGHTV1_STATE_ON;

		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if


	// toggle spot light
	if (keyboard_state[DIK_S])
	{
		// toggle spot light
		if (lights[SPOT_LIGHT2_INDEX].state == LIGHTV1_STATE_ON)
			lights[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_OFF;
		else
			lights[SPOT_LIGHT2_INDEX].state = LIGHTV1_STATE_ON;

		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if


	// help menu
	if (keyboard_state[DIK_H])
	{
		// toggle help menu 
		help_mode = -help_mode;
		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	// z-sorting
	if (keyboard_state[DIK_Z])
	{
		// toggle z sorting
		zsort_mode = -zsort_mode;
		Wait_Clock(100); // wait, so keyboard doesn't bounce
	} // end if

	static float plight_ang = 0, slight_ang = 0; // angles for light motion

	// move point light source in ellipse around game world
	lights[POINT_LIGHT_INDEX].pos.x = 1000*Fast_Cos(plight_ang);
	lights[POINT_LIGHT_INDEX].pos.y = 100;
	lights[POINT_LIGHT_INDEX].pos.z = 1000*Fast_Sin(plight_ang);

	if ((plight_ang+=3) > 360)
		plight_ang = 0;

	// move spot light source in ellipse around game world
	lights[SPOT_LIGHT2_INDEX].pos.x = 1000*Fast_Cos(slight_ang);
	lights[SPOT_LIGHT2_INDEX].pos.y = 200;
	lights[SPOT_LIGHT2_INDEX].pos.z = 1000*Fast_Sin(slight_ang);

	if ((slight_ang-=5) < 0)
		slight_ang = 360;

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	// use these to rotate objects
	static float x_ang = 0, y_ang = 0, z_ang = 0;


	//////////////////////////////////////////////////////////////////////////
	// flat shaded textured cube

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV2(&obj);

	// set position of constant shaded cube
	obj.world_pos.x = 0;
	obj.world_pos.y = 0;
	obj.world_pos.z = 150;

	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang, z_ang, &mrot);

	// rotate the local coords of the object
	Transform_OBJECT4DV2(&obj, &mrot, TRANSFORM_LOCAL_TO_TRANS,1);

	// perform world transform
	Model_To_World_OBJECT4DV2(&obj, TRANSFORM_TRANS_ONLY);

	// insert the object into render list
	Insert_OBJECT4DV2_RENDERLIST4DV2(&rend_list, &obj,0);

	// update rotation angles
	/*if ((x_ang+=1) > 360) 
		x_ang = 0;
	if ((y_ang+=2) > 360) 
		y_ang = 0;
	if ((z_ang+=3) > 360) 
		z_ang = 0;*/

	// remove backfaces
	if (backface_mode==1)
		Remove_Backfaces_RENDERLIST4DV2(&rend_list, &cam);

	// light scene all at once 
	if (lighting_mode==1)
		Light_RENDERLIST4DV2_World16(&rend_list, &cam, lights, 4);

	// apply world to camera transform
	World_To_Camera_RENDERLIST4DV2(&rend_list, &cam);

	// sort the polygon list (hurry up!)
	if (zsort_mode == 1)
		Sort_RENDERLIST4DV2(&rend_list,  SORT_POLYLIST_AVGZ);

	// apply camera to perspective transformation
	Camera_To_Perspective_RENDERLIST4DV2(&rend_list, &cam);

	// apply screen transform
	Perspective_To_Screen_RENDERLIST4DV2(&rend_list, &cam);

	sprintf(work_string,"Lighting [%s]: Ambient=%d, Infinite=%d, Point=%d, Spot=%d | Zsort [%s], BckFceRM [%s]", 
		((lighting_mode == 1) ? "ON" : "OFF"),
		lights[AMBIENT_LIGHT_INDEX].state,
		lights[INFINITE_LIGHT_INDEX].state, 
		lights[POINT_LIGHT_INDEX].state,
		lights[SPOT_LIGHT2_INDEX].state,
		((zsort_mode == 1) ? "ON" : "OFF"),
		((backface_mode == 1) ? "ON" : "OFF"));

	Draw_Text_GDI(work_string, 0, WINDOW_HEIGHT-34, RGB(0,255,0), lpddsback);

	// draw instructions
	Draw_Text_GDI("Press ESC to exit. Press <H> for Help.", 0, 0, RGB(0,255,0), lpddsback);

	// should we display help
	int text_y = 16;
	if (help_mode==1)
	{
		// draw help menu
		Draw_Text_GDI("<A>..............Toggle ambient light source.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<I>..............Toggle infinite light source.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<P>..............Toggle point light source.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<S>..............Toggle spot light source.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<W>..............Toggle wire frame/solid mode.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<B>..............Toggle backface removal.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<H>..............Toggle Help.", 0, text_y+=12, RGB(255,255,255), lpddsback);
		Draw_Text_GDI("<ESC>............Exit demo.", 0, text_y+=12, RGB(255,255,255), lpddsback);

	} // end help

	// lock the back buffer
	DDraw_Lock_Back_Surface();

	// reset number of polys rendered
	debug_polys_rendered_per_frame = 0;

	// render the object

	if (wireframe_mode  == 0)
	{
		Draw_RENDERLIST4DV2_Wire16(&rend_list, back_buffer, back_lpitch);
	}
	else if (wireframe_mode  == 1)
	{
		Draw_RENDERLIST4DV2_Solid16(&rend_list, back_buffer, back_lpitch);
	}


	/*int bitHeight = bitmap.bitmapinfoheader.biHeight;
	int bitWidth  = bitmap.bitmapinfoheader.biWidth;
	for (int index_y = 0; index_y < bitHeight; index_y ++)
	{
		for ( int index_x = 0; index_x < bitWidth; index_x ++)
		{
			UCHAR blue  = (bitmap.buffer[index_y * bitWidth * 3 + index_x * 3 + 0]) >>3;
			UCHAR green = (bitmap.buffer[index_y * bitWidth * 3 + index_x * 3 + 1])>>2;
			UCHAR red   = (bitmap.buffer[index_y * bitWidth * 3 + index_x * 3 + 2]) >>3;
			USHORT pixel = _RGB16BIT565(red, green, blue );
			back_buffer[index_x + 200     + (index_y + 200) *back_lpitch ] = pixel;
		}
	}*/
	// unlock the back buffer
	DDraw_Unlock_Back_Surface();

	// flip the surfaces
	DDraw_Flip();

	// sync to 30ish fps
	Wait_Clock(30);

	// check of user is trying to exit
	if (KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
	{
		PostMessage(main_window_handle, WM_DESTROY,0,0);
	} // end if

	// return success
	return(1);

} // end Game_Main

//////////////////////////////////////////////////////////