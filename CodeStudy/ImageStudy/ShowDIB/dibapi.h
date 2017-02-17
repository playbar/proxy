/******************************************************************************** 
 *  DIBAPI.H
 * 
 *  Header file for Device-Independent Bitmap (DIB) API.  Provides 
 *  function prototypes and constants for DIB functions 
 ********************************************************************************/ 

#ifndef __DIBAPI_H_
#define __DIBAPI_H_
//////////////////////////////////////////////////////////

// Dib Header Marker - used in writing DIBs to files 
#define DIB_HEADER_MARKER	((WORD) ('M' << 8) | 'B') 

/* DIB constants */ 
#define PALVERSION			0x300 

/* DIB Macros*/ 
// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits" 
// parameter is the bit count for the scanline (biWidth * biBitCount), 
// and this macro returns the number of DWORD-aligned bytes needed  
// to hold those bits. 
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER)) 
#define RECTWIDTH(lpRect)   ((lpRect)->right - (lpRect)->left) 
#define RECTHEIGHT(lpRect)  ((lpRect)->bottom - (lpRect)->top) 
 
/* function prototypes */ 
HANDLE LoadDIB (LPCTSTR); 
BOOL SaveDIB (HANDLE, LPCTSTR); 
HANDLE ReadDIBFile(HANDLE); 
WORD DIBNumColors (LPBYTE lpDIB); 
HPALETTE CreateDIBPalette(LPBYTE lpDIB);
BOOL DisplayPalette(HDC hDC, LPRECT lpRect, HPALETTE hPal);
BOOL PaintDIB(HDC, LPRECT, HANDLE, LPRECT, HPALETTE, DWORD);

//////////////////////////////////////////////////////////
#endif	//__DIBAPI_H_