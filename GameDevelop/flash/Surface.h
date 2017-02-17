#ifndef CSURFACE_H
#define CSURFACE_H

#include <ddraw.h>

//#define BITMAP_ID            0x4D42 // universal id for a bitmap
//#define MAX_COLORS_PALETTE   256
//
//// basic unsigned types
//typedef unsigned short USHORT;
//typedef unsigned short WORD;
//typedef unsigned char  UCHAR;
//typedef unsigned char  BYTE;
//
//// container structure for bitmaps .BMP file
//typedef struct BITMAP_FILE_TAG
//{
//	BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
//	BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
//	PALETTEENTRY     palette[256];      // we will store the palette here
//	UCHAR            *buffer;           // this is a pointer to the data
//
//} BITMAP_FILE, *BITMAP_FILE_PTR;
//
//// PROTOTYPES  //////////////////////////////////////////////
//
//int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);
//int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
//int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);
//
//// initializes a direct draw struct
//#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
//
//// this builds a 16 bit color value in 5.5.5 format (1-bit alpha mode)
//#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
//
//// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
//#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
//
//// this builds a 32 bit color value in A.8.8.8 format (8-bit alpha mode)
//#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

//////////////////////////////////////////////////////////////////////////

class Surface  
{
public:
	Surface();
	Surface(LPDIRECTDRAW7 pkDD, HINSTANCE hInst, const char *pcFileName, int iWidth, int iHeight, unsigned long ulColorKey = -1);
	virtual ~Surface();

	LPDIRECTDRAWSURFACE7 GetSurface() { return m_pkSurface; }
	unsigned int Width() { return m_uiWidth; }
	unsigned int Height() { return m_uiHeight; }
	void Restore() { m_pkSurface->Restore(); }

	void Destroy();
	bool Create(LPDIRECTDRAW7 pkDD, int iWidth, int iHeight, unsigned long ulColorKey = -1);
	bool LoadBitmap(HINSTANCE hInst, const char *pcFileName, int iX = 0, int iY = 0, int iWidth = 0, int iHeight = 0);

	bool Draw(LPDIRECTDRAWSURFACE7 pkDest, int iDestX = 0, int iDestY = 0, int iSrcX = 0, int iSrcY = 0, int iWidth = 0, int iHeight = 0);

protected:
	unsigned long m_ulColorKey;
	unsigned int m_uiHeight;
	unsigned int m_uiWidth;

	LPDIRECTDRAWSURFACE7 m_pkSurface;
};

#endif
