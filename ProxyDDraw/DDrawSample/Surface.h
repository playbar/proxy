#ifndef CSURFACE_H
#define CSURFACE_H

#include <ddraw.h>

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
