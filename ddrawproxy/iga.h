#pragma once
#include "ddraw.h"
extern bool   Is555;

class IGA
{
public:
	IGA(LPDIRECTDRAW7 pOriginal);
	~IGA();
	bool   SaveToBitmapFile(LPDIRECTDRAWSURFACE7   lpSurface,   char*   filename);
	bool   SaveToBitmapFile(LPDIRECTDRAWSURFACE    lpSurface,   char*   filename);
	void   LoadImageResource();

	bool   ReplaceSurface(LPDIRECTDRAWSURFACE7 lpSurface, HBITMAP hBitmap);

private:
	LPDIRECTDRAW7	m_pIGADraw;

public:
	HBITMAP         m_hBitmap;
	HBITMAP         m_hBitmapB1;
	HBITMAP         m_hBitmapC1,m_hBitmapD1, m_hBitmapE1;
};

//bool   SaveToBitmapFile(LPDIRECTDRAWSURFACE7   lpSurface,   char*   filename) ;