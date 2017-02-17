// xImalpha.cpp : Alpha channel functions
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

#if CXIMAGE_SUPPORT_ALPHA

////////////////////////////////////////////////////////////////////////////////
/**
 * \sa AlphaSetMax
 */
BYTE CxImage::AlphaGetMax() const
{
	return m_stImgInfo.nAlphaMax;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets global Alpha (opacity) value applied to the whole image,
 * valid only for painting functions.
 * \param nAlphaMax: can be from 0 to 255
 */
void CxImage::AlphaSetMax(BYTE nAlphaMax)
{
	m_stImgInfo.nAlphaMax=nAlphaMax;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the image has a valid alpha channel.
 */
bool CxImage::AlphaIsValid()
{
	return m_pAlpha!=0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Enables the alpha palette, so the Draw() function changes its behavior.
 */
void CxImage::AlphaPaletteEnable(bool enable)
{
	m_stImgInfo.bAlphaPaletteEnabled=enable;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * True if the alpha palette is enabled for painting.
 */
bool CxImage::AlphaPaletteIsEnabled()
{
	return m_stImgInfo.bAlphaPaletteEnabled;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the alpha channel to full transparent. AlphaSet(0) has the same effect
 */
void CxImage::AlphaClear()
{
	if (m_pAlpha)	memset(m_pAlpha,0,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the alpha level for the whole image.
 * \param level : from 0 (transparent) to 255 (opaque)
 */
void CxImage::AlphaSet(BYTE level)
{
	if (m_pAlpha)	memset(m_pAlpha,level,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Allocates an empty (opaque) alpha channel.
 */
bool CxImage::AlphaCreate()
{
	if (m_pAlpha==NULL) {
		m_pAlpha = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
		if (m_pAlpha) memset(m_pAlpha,255,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	}
	return (m_pAlpha!=0);
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::AlphaDelete()
{
	if (m_pAlpha) { free(m_pAlpha); m_pAlpha=0; }
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::AlphaInvert()
{
	if (m_pAlpha) {
		BYTE *iSrc=m_pAlpha;
		long n=m_bmpHeadInfo.biHeight*m_bmpHeadInfo.biWidth;
		for(long i=0; i < n; i++){
			*iSrc=(BYTE)~(*(iSrc));
			iSrc++;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Imports an existing alpa channel from another image with the same width and height.
 */
bool CxImage::AlphaCopy(CxImage &from)
{
	if (from.m_pAlpha == NULL || m_bmpHeadInfo.biWidth != from.m_bmpHeadInfo.biWidth || m_bmpHeadInfo.biHeight != from.m_bmpHeadInfo.biHeight) return false;
	if (m_pAlpha==NULL) m_pAlpha = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	if (m_pAlpha==NULL) return false;
	memcpy(m_pAlpha,from.m_pAlpha,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	m_stImgInfo.nAlphaMax=from.m_stImgInfo.nAlphaMax;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Creates the alpha channel from a gray scale image.
 */
bool CxImage::AlphaSet(CxImage &from)
{
	if (!from.IsGrayScale() || m_bmpHeadInfo.biWidth != from.m_bmpHeadInfo.biWidth || m_bmpHeadInfo.biHeight != from.m_bmpHeadInfo.biHeight) return false;
	if (m_pAlpha==NULL) m_pAlpha = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	BYTE* src = from.m_stImgInfo.pImage;
	BYTE* dst = m_pAlpha;
	if (src==NULL || dst==NULL) return false;
	for (long y=0; y<m_bmpHeadInfo.biHeight; y++){
		memcpy(dst,src,m_bmpHeadInfo.biWidth);
		dst += m_bmpHeadInfo.biWidth;
		src += from.m_stImgInfo.dwEffWidth;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the alpha level for a single pixel 
 */
void CxImage::AlphaSet(const long x,const long y,const BYTE level)
{
	if (m_pAlpha && IsInside(x,y)) m_pAlpha[x+y*m_bmpHeadInfo.biWidth]=level;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the alpha level for a single pixel 
 */
BYTE CxImage::AlphaGet(const long x,const long y)
{
	if (m_pAlpha && IsInside(x,y)) return m_pAlpha[x+y*m_bmpHeadInfo.biWidth];
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns pointer to alpha data for pixel (x,y).
 *
 * \author ***bd*** 2.2004
 */
BYTE* CxImage::AlphaGetPointer(const long x,const long y)
{
	if (m_pAlpha && IsInside(x,y)) return m_pAlpha+x+y*m_bmpHeadInfo.biWidth;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Get alpha value without boundscheck (a bit faster). Pixel must be inside the image.
 *
 * \author ***bd*** 2.2004
 */
BYTE CxImage::BlindAlphaGet(const long x,const long y)
{
#ifdef _DEBUG
	if (!IsInside(x,y) || (m_pAlpha==0))
  #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
		throw 0;
  #else
		return 0;
  #endif
#endif
	return m_pAlpha[x+y*m_bmpHeadInfo.biWidth];
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Resets the alpha palette 
 */
void CxImage::AlphaPaletteClear()
{
	RGBQUAD c;
	for(WORD ip=0; ip<m_bmpHeadInfo.biClrUsed;ip++){
		c=GetPaletteColor((BYTE)ip);
		c.rgbReserved=0;
		SetPaletteColor((BYTE)ip,c);
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the image has a valid alpha palette. 
 */
bool CxImage::AlphaPaletteIsValid()
{
	RGBQUAD c;
	for(WORD ip=0; ip<m_bmpHeadInfo.biClrUsed;ip++){
		c=GetPaletteColor((BYTE)ip);
		if (c.rgbReserved != 0) return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Blends the alpha channel and the alpha palette with the pixels. The result is a 24 bit image.
 * The background color can be selected using SetTransColor().
 */
void CxImage::AlphaStrip()
{
	bool bAlphaPaletteIsValid = AlphaPaletteIsValid();
	bool bAlphaIsValid = AlphaIsValid();
	if (!(bAlphaIsValid || bAlphaPaletteIsValid)) return;
	RGBQUAD c;
	long a, a1;
	if (m_bmpHeadInfo.biBitCount==24){
		for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
			for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
				c = BlindGetPixelColor(x,y);
				if (bAlphaIsValid) a=(BlindAlphaGet(x,y)*m_stImgInfo.nAlphaMax)/255; else a=m_stImgInfo.nAlphaMax;
				a1 = 256-a;
				c.rgbBlue = (BYTE)((c.rgbBlue * a + a1 * m_stImgInfo.nBkgndColor.rgbBlue)>>8);
				c.rgbGreen = (BYTE)((c.rgbGreen * a + a1 * m_stImgInfo.nBkgndColor.rgbGreen)>>8);
				c.rgbRed = (BYTE)((c.rgbRed * a + a1 * m_stImgInfo.nBkgndColor.rgbRed)>>8);
				BlindSetPixelColor(x,y,c);
			}
		}
		AlphaDelete();
	} else {
		CxImage tmp(m_bmpHeadInfo.biWidth,m_bmpHeadInfo.biHeight,24);
		if (!tmp.IsValid()){
			strcpy(m_stImgInfo.szLastError,tmp.GetLastError());
			return;
		}

		for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
			for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
				c = BlindGetPixelColor(x,y);
				if (bAlphaIsValid) a=(BlindAlphaGet(x,y)*m_stImgInfo.nAlphaMax)/255; else a=m_stImgInfo.nAlphaMax;
				if (bAlphaPaletteIsValid) a=(c.rgbReserved*a)/255;
				a1 = 256-a;
				c.rgbBlue = (BYTE)((c.rgbBlue * a + a1 * m_stImgInfo.nBkgndColor.rgbBlue)>>8);
				c.rgbGreen = (BYTE)((c.rgbGreen * a + a1 * m_stImgInfo.nBkgndColor.rgbGreen)>>8);
				c.rgbRed = (BYTE)((c.rgbRed * a + a1 * m_stImgInfo.nBkgndColor.rgbRed)>>8);
				tmp.BlindSetPixelColor(x,y,c);
			}
		}
		Transfer(tmp);
	}
	return;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::AlphaFlip()
{
	if (!m_pAlpha) return false;

	BYTE *buff = (BYTE*)malloc(m_bmpHeadInfo.biWidth);
	if (!buff) return false;

	BYTE *iSrc,*iDst;
	iSrc = m_pAlpha + (m_bmpHeadInfo.biHeight-1)*m_bmpHeadInfo.biWidth;
	iDst = m_pAlpha;
	for (long i=0; i<(m_bmpHeadInfo.biHeight/2); ++i)
	{
		memcpy(buff, iSrc, m_bmpHeadInfo.biWidth);
		memcpy(iSrc, iDst, m_bmpHeadInfo.biWidth);
		memcpy(iDst, buff, m_bmpHeadInfo.biWidth);
		iSrc-=m_bmpHeadInfo.biWidth;
		iDst+=m_bmpHeadInfo.biWidth;
	}

	free(buff);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::AlphaMirror()
{
	if (!m_pAlpha) return false;
	BYTE* pAlpha2 = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	if (!pAlpha2) return false;
	BYTE *iSrc,*iDst;
	long wdt=m_bmpHeadInfo.biWidth-1;
	iSrc=m_pAlpha + wdt;
	iDst=pAlpha2;
	for(long y=0; y < m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x <= wdt; x++)
			*(iDst+x)=*(iSrc-x);
		iSrc+=m_bmpHeadInfo.biWidth;
		iDst+=m_bmpHeadInfo.biWidth;
	}
	free(m_pAlpha);
	m_pAlpha=pAlpha2;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Exports the alpha channel in a 8bpp grayscale image. 
 */
bool CxImage::AlphaSplit(CxImage *dest)
{
	if (!m_pAlpha || !dest) return false;

	CxImage tmp(m_bmpHeadInfo.biWidth,m_bmpHeadInfo.biHeight,8);
	if (!tmp.IsValid()){
		strcpy(m_stImgInfo.szLastError,tmp.GetLastError());
		return false;
	}

	for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
			tmp.BlindSetPixelIndex(x,y,m_pAlpha[x+y*m_bmpHeadInfo.biWidth]);
		}
	}

	tmp.SetGrayPalette();
	dest->Transfer(tmp);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Exports the alpha palette channel in a 8bpp grayscale image. 
 */
bool CxImage::AlphaPaletteSplit(CxImage *dest)
{
	if (!AlphaPaletteIsValid() || !dest) return false;

	CxImage tmp(m_bmpHeadInfo.biWidth,m_bmpHeadInfo.biHeight,8);
	if (!tmp.IsValid()){
		strcpy(m_stImgInfo.szLastError,tmp.GetLastError());
		return false;
	}

	for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
			tmp.BlindSetPixelIndex(x,y,BlindGetPixelColor(x,y).rgbReserved);
		}
	}

	tmp.SetGrayPalette();
	dest->Transfer(tmp);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Merge in the alpha layer the transparent color mask
 * (previously set with SetTransColor or SetTransIndex) 
 */
bool CxImage::AlphaFromTransparency()
{
	if (!IsValid() || !IsTransparent())
		return false;

	AlphaCreate();

	for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
			if (IsTransparent(x,y)){
				AlphaSet(x,y,0);
			}
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_ALPHA
