// xImaSel.cpp : Selection functions
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

#if CXIMAGE_SUPPORT_SELECTION

////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the image has a valid selection.
 */
bool CxImage::SelectionIsValid()
{
	return m_pSelection!=0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the smallest rectangle that contains the selection 
 */
void CxImage::SelectionGetBox(RECT& r)
{
	memcpy(&r,&m_stImgInfo.rSelectionBox,sizeof(RECT));
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Empties the selection.
 */
bool CxImage::SelectionClear(BYTE level)
{
	if (m_pSelection){
		if (level==0){
			memset(m_pSelection,0,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
			m_stImgInfo.rSelectionBox.left = m_bmpHeadInfo.biWidth;
			m_stImgInfo.rSelectionBox.bottom = m_bmpHeadInfo.biHeight;
			m_stImgInfo.rSelectionBox.right = m_stImgInfo.rSelectionBox.top = 0;
		} else {
			memset(m_pSelection,level,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
			m_stImgInfo.rSelectionBox.right = m_bmpHeadInfo.biWidth;
			m_stImgInfo.rSelectionBox.top = m_bmpHeadInfo.biHeight;
			m_stImgInfo.rSelectionBox.left = m_stImgInfo.rSelectionBox.bottom = 0;
		}
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Allocates an empty selection.
 */
bool CxImage::SelectionCreate()
{
	SelectionDelete();
	m_pSelection = (BYTE*)calloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight, 1);
	return (m_pSelection!=0);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Deallocates the selction.
 */
bool CxImage::SelectionDelete()
{
	if (m_pSelection){
		free(m_pSelection);
		m_pSelection=NULL;
	}
	m_stImgInfo.rSelectionBox.left = m_bmpHeadInfo.biWidth;
	m_stImgInfo.rSelectionBox.bottom = m_bmpHeadInfo.biHeight;
	m_stImgInfo.rSelectionBox.right = m_stImgInfo.rSelectionBox.top = 0;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the coordinates are inside the selection.
 */
bool CxImage::SelectionIsInside(long x, long y)
{
	if (IsInside(x,y)){
		if (m_pSelection==NULL) return true;
		return m_pSelection[x+y*m_bmpHeadInfo.biWidth]!=0;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the coordinates are inside the selection.
 * "blind" version assumes that (x,y) is inside to the image.
 */
bool CxImage::BlindSelectionIsInside(long x, long y)
{
#ifdef _DEBUG
	if (!IsInside(x,y))
  #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
		throw 0;
  #else
		return 0;
  #endif
#endif
	if (m_pSelection==NULL) return true;
	return m_pSelection[x+y*m_bmpHeadInfo.biWidth]!=0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Adds a rectangle to the existing selection.
 */
bool CxImage::SelectionAddRect(RECT r, BYTE level)
{
	if (m_pSelection==NULL) SelectionCreate();
	if (m_pSelection==NULL) return false;

	RECT r2;
	if (r.left<r.right) {r2.left=r.left; r2.right=r.right; } else {r2.left=r.right ; r2.right=r.left; }
	if (r.bottom<r.top) {r2.bottom=r.bottom; r2.top=r.top; } else {r2.bottom=r.top ; r2.top=r.bottom; }

	if (m_stImgInfo.rSelectionBox.top <= r2.top) m_stImgInfo.rSelectionBox.top = max(0L,min(m_bmpHeadInfo.biHeight,r2.top+1));
	if (m_stImgInfo.rSelectionBox.left > r2.left) m_stImgInfo.rSelectionBox.left = max(0L,min(m_bmpHeadInfo.biWidth,r2.left));
	if (m_stImgInfo.rSelectionBox.right <= r2.right) m_stImgInfo.rSelectionBox.right = max(0L,min(m_bmpHeadInfo.biWidth,r2.right+1));
	if (m_stImgInfo.rSelectionBox.bottom > r2.bottom) m_stImgInfo.rSelectionBox.bottom = max(0L,min(m_bmpHeadInfo.biHeight,r2.bottom));

	long ymin = max(0L,min(m_bmpHeadInfo.biHeight,r2.bottom));
	long ymax = max(0L,min(m_bmpHeadInfo.biHeight,r2.top+1));
	long xmin = max(0L,min(m_bmpHeadInfo.biWidth,r2.left));
	long xmax = max(0L,min(m_bmpHeadInfo.biWidth,r2.right+1));

	for (long y=ymin; y<ymax; y++)
		memset(m_pSelection + xmin + y * m_bmpHeadInfo.biWidth, level, xmax-xmin);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Adds an ellipse to the existing selection.
 */
bool CxImage::SelectionAddEllipse(RECT r, BYTE level)
{
	if (m_pSelection==NULL) SelectionCreate();
	if (m_pSelection==NULL) return false;

	long xradius = abs(r.right - r.left)/2;
	long yradius = abs(r.top - r.bottom)/2;
	if (xradius==0 || yradius==0) return false;

	long xcenter = (r.right + r.left)/2;
	long ycenter = (r.top + r.bottom)/2;

	if (m_stImgInfo.rSelectionBox.left > (xcenter - xradius)) m_stImgInfo.rSelectionBox.left = max(0L,min(m_bmpHeadInfo.biWidth,(xcenter - xradius)));
	if (m_stImgInfo.rSelectionBox.right <= (xcenter + xradius)) m_stImgInfo.rSelectionBox.right = max(0L,min(m_bmpHeadInfo.biWidth,(xcenter + xradius + 1)));
	if (m_stImgInfo.rSelectionBox.bottom > (ycenter - yradius)) m_stImgInfo.rSelectionBox.bottom = max(0L,min(m_bmpHeadInfo.biHeight,(ycenter - yradius)));
	if (m_stImgInfo.rSelectionBox.top <= (ycenter + yradius)) m_stImgInfo.rSelectionBox.top = max(0L,min(m_bmpHeadInfo.biHeight,(ycenter + yradius + 1)));

	long xmin = max(0L,min(m_bmpHeadInfo.biWidth,xcenter - xradius));
	long xmax = max(0L,min(m_bmpHeadInfo.biWidth,xcenter + xradius + 1));
	long ymin = max(0L,min(m_bmpHeadInfo.biHeight,ycenter - yradius));
	long ymax = max(0L,min(m_bmpHeadInfo.biHeight,ycenter + yradius + 1));

	long y,yo;
	for (y=ymin; y<min(ycenter,ymax); y++){
		for (long x=xmin; x<xmax; x++){
			yo = (long)(ycenter - yradius * sqrt(1-pow((float)(x - xcenter)/(float)xradius,2)));
			if (yo<y) m_pSelection[x + y * m_bmpHeadInfo.biWidth] = level;
		}
	}
	for (y=ycenter; y<ymax; y++){
		for (long x=xmin; x<xmax; x++){
			yo = (long)(ycenter + yradius * sqrt(1-pow((float)(x - xcenter)/(float)xradius,2)));
			if (yo>y) m_pSelection[x + y * m_bmpHeadInfo.biWidth] = level;
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Inverts the selection.
 * Note: the SelectionBox is set to "full image", call SelectionGetBox before (if necessary)
 */
bool CxImage::SelectionInvert()
{
	if (m_pSelection) {
		BYTE *iSrc=m_pSelection;
		long n=m_bmpHeadInfo.biHeight*m_bmpHeadInfo.biWidth;
		for(long i=0; i < n; i++){
			*iSrc=(BYTE)~(*(iSrc));
			iSrc++;
		}

		SelectionRebuildBox();

		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Imports an existing region from another image with the same width and height.
 */
bool CxImage::SelectionCopy(CxImage &from)
{
	if (from.m_pSelection == NULL || m_bmpHeadInfo.biWidth != from.m_bmpHeadInfo.biWidth || m_bmpHeadInfo.biHeight != from.m_bmpHeadInfo.biHeight) return false;
	if (m_pSelection==NULL) m_pSelection = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	if (m_pSelection==NULL) return false;
	memcpy(m_pSelection,from.m_pSelection,m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	memcpy(&m_stImgInfo.rSelectionBox,&from.m_stImgInfo.rSelectionBox,sizeof(RECT));
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Adds a polygonal region to the existing selection. points points to an array of POINT structures.
 * Each structure specifies the x-coordinate and y-coordinate of one vertex of the polygon.
 * npoints specifies the number of POINT structures in the array pointed to by points.
 */
bool CxImage::SelectionAddPolygon(POINT *points, long npoints, BYTE level)
{
	if (points==NULL || npoints<3) return false;

	if (m_pSelection==NULL) SelectionCreate();
	if (m_pSelection==NULL) return false;

	BYTE* plocal = (BYTE*)calloc(m_bmpHeadInfo.biWidth*m_bmpHeadInfo.biHeight, 1);
	RECT localbox = {m_bmpHeadInfo.biWidth,0,0,m_bmpHeadInfo.biHeight};

	long x,y,i=0;
	POINT *current;
	POINT *next = NULL;
	POINT *start = NULL;
	//trace contour
	while (i < npoints){
		current = &points[i];
		if (current->x!=-1){
			if (i==0 || (i>0 && points[i-1].x==-1)) start = &points[i];

			if ((i+1)==npoints || points[i+1].x==-1)
				next = start;
			else
				next = &points[i+1];

			float beta;
			if (current->x != next->x){
				beta = (float)(next->y - current->y)/(float)(next->x - current->x);
				if (current->x < next->x){
					for (x=current->x; x<=next->x; x++){
						y = (long)(current->y + (x - current->x) * beta);
						if (IsInside(x,y)) plocal[x + y * m_bmpHeadInfo.biWidth] = 255;
					}
				} else {
					for (x=current->x; x>=next->x; x--){
						y = (long)(current->y + (x - current->x) * beta);
						if (IsInside(x,y)) plocal[x + y * m_bmpHeadInfo.biWidth] = 255;
					}
				}
			}
			if (current->y != next->y){
				beta = (float)(next->x - current->x)/(float)(next->y - current->y);
				if (current->y < next->y){
					for (y=current->y; y<=next->y; y++){
						x = (long)(current->x + (y - current->y) * beta);
						if (IsInside(x,y)) plocal[x + y * m_bmpHeadInfo.biWidth] = 255;
					}
				} else {
					for (y=current->y; y>=next->y; y--){
						x = (long)(current->x + (y - current->y) * beta);
						if (IsInside(x,y)) plocal[x + y * m_bmpHeadInfo.biWidth] = 255;
					}
				}
			}
		}

		RECT r2;
		if (current->x < next->x) {r2.left=current->x; r2.right=next->x; } else {r2.left=next->x ; r2.right=current->x; }
		if (current->y < next->y) {r2.bottom=current->y; r2.top=next->y; } else {r2.bottom=next->y ; r2.top=current->y; }
		if (localbox.top < r2.top) localbox.top = max(0L,min(m_bmpHeadInfo.biHeight-1,r2.top+1));
		if (localbox.left > r2.left) localbox.left = max(0L,min(m_bmpHeadInfo.biWidth-1,r2.left-1));
		if (localbox.right < r2.right) localbox.right = max(0L,min(m_bmpHeadInfo.biWidth-1,r2.right+1));
		if (localbox.bottom > r2.bottom) localbox.bottom = max(0L,min(m_bmpHeadInfo.biHeight-1,r2.bottom-1));

		i++;
	}

	//fill the outer region
	long npix=(localbox.right - localbox.left)*(localbox.top - localbox.bottom);
	POINT* pix = (POINT*)calloc(npix,sizeof(POINT));
	BYTE back=0, mark=1;
	long fx, fy, fxx, fyy, first, last;
	long xmin = 0;
	long xmax = 0;
	long ymin = 0;
	long ymax = 0;

	for (int side=0; side<4; side++){
		switch(side){
		case 0:
			xmin=localbox.left; xmax=localbox.right+1; ymin=localbox.bottom; ymax=localbox.bottom+1;
			break;
		case 1:
			xmin=localbox.right; xmax=localbox.right+1; ymin=localbox.bottom; ymax=localbox.top+1;
			break;
		case 2:
			xmin=localbox.left; xmax=localbox.right+1; ymin=localbox.top; ymax=localbox.top+1;
			break;
		case 3:
			xmin=localbox.left; xmax=localbox.left+1; ymin=localbox.bottom; ymax=localbox.top+1;
			break;
		}
		//fill from the border points
		for(y=ymin;y<ymax;y++){
			for(x=xmin;x<xmax;x++){
				if (plocal[x+y*m_bmpHeadInfo.biWidth]==0){
					// Subject: FLOOD FILL ROUTINE              Date: 12-23-97 (00:57)       
					// Author:  Petter Holmberg                 Code: QB, QBasic, PDS        
					// Origin:  petter.holmberg@usa.net         Packet: GRAPHICS.ABC
					first=0;
					last=1;
					while(first!=last){
						fx = pix[first].x;
						fy = pix[first].y;
						fxx = fx + x;
						fyy = fy + y;
						for(;;)
						{
							if ((plocal[fxx + fyy*m_bmpHeadInfo.biWidth] == back) &&
								fxx>=localbox.left && fxx<=localbox.right && fyy>=localbox.bottom && fyy<=localbox.top )
							{
								plocal[fxx + fyy*m_bmpHeadInfo.biWidth] = mark;
								if (fyy > 0 && plocal[fxx + (fyy - 1)*m_bmpHeadInfo.biWidth] == back){
									pix[last].x = fx;
									pix[last].y = fy - 1;
									last++;
									if (last == npix) last = 0;
								}
								if ((fyy + 1)<m_bmpHeadInfo.biHeight && plocal[fxx + (fyy + 1)*m_bmpHeadInfo.biWidth] == back){
									pix[last].x = fx;
									pix[last].y = fy + 1;
									last++;
									if (last == npix) last = 0;
								}
							} else {
								break;
							}
							fx++;
							fxx++;
						};

						fx = pix[first].x - 1;
						fy = pix[first].y;
						fxx = fx + x;
						fyy = fy + y;

						for( ;; )
						{
							if ((plocal[fxx + fyy*m_bmpHeadInfo.biWidth] == back) &&
								fxx>=localbox.left && fxx<=localbox.right && fyy>=localbox.bottom && fyy<=localbox.top )
							{
								plocal[fxx + (y + fy)*m_bmpHeadInfo.biWidth] = mark;
								if (fyy > 0 && plocal[fxx + (fyy - 1)*m_bmpHeadInfo.biWidth] == back){
									pix[last].x = fx;
									pix[last].y = fy - 1;
									last++;
									if (last == npix) last = 0;
								}
								if ((fyy + 1)<m_bmpHeadInfo.biHeight && plocal[fxx + (fyy + 1)*m_bmpHeadInfo.biWidth] == back){
									pix[last].x = fx;
									pix[last].y = fy + 1;
									last++;
									if (last == npix) last = 0;
								}
							} else {
								break;
							}
							fx--;
							fxx--;
						}
						
						first++;
						if (first == npix) first = 0;
					}
				}
			}
		}
	}

	//transfer the region
	long yoffset;
	for (y=localbox.bottom; y<=localbox.top; y++){
		yoffset = y * m_bmpHeadInfo.biWidth;
		for (x=localbox.left; x<=localbox.right; x++)
			if (plocal[x + yoffset]!=1) m_pSelection[x + yoffset]=level;
	}
	if (m_stImgInfo.rSelectionBox.top <= localbox.top) m_stImgInfo.rSelectionBox.top = min(m_bmpHeadInfo.biHeight,localbox.top + 1);
	if (m_stImgInfo.rSelectionBox.left > localbox.left) m_stImgInfo.rSelectionBox.left = min(m_bmpHeadInfo.biWidth,localbox.left);
	if (m_stImgInfo.rSelectionBox.right <= localbox.right) m_stImgInfo.rSelectionBox.right = min(m_bmpHeadInfo.biWidth,localbox.right + 1);
	if (m_stImgInfo.rSelectionBox.bottom > localbox.bottom) m_stImgInfo.rSelectionBox.bottom = min(m_bmpHeadInfo.biHeight,localbox.bottom);

	free(plocal);
	free(pix);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Adds to the selection all the pixels matching the specified color.
 */
bool CxImage::SelectionAddColor(RGBQUAD c, BYTE level)
{
    if (m_pSelection==NULL) SelectionCreate();
	if (m_pSelection==NULL) return false;

	RECT localbox = {m_bmpHeadInfo.biWidth,0,0,m_bmpHeadInfo.biHeight};

    for (long y = 0; y < m_bmpHeadInfo.biHeight; y++){
        for (long x = 0; x < m_bmpHeadInfo.biWidth; x++){
            RGBQUAD color = BlindGetPixelColor(x, y);
            if (color.rgbRed   == c.rgbRed &&
				color.rgbGreen == c.rgbGreen &&
                color.rgbBlue  == c.rgbBlue)
            {
                m_pSelection[x + y * m_bmpHeadInfo.biWidth] = level;

				if (localbox.top < y) localbox.top = y;
				if (localbox.left > x) localbox.left = x;
				if (localbox.right < x) localbox.right = x;
				if (localbox.bottom > y) localbox.bottom = y;
            }
        }
    }

	if (m_stImgInfo.rSelectionBox.top <= localbox.top) m_stImgInfo.rSelectionBox.top = localbox.top + 1;
	if (m_stImgInfo.rSelectionBox.left > localbox.left) m_stImgInfo.rSelectionBox.left = localbox.left;
	if (m_stImgInfo.rSelectionBox.right <= localbox.right) m_stImgInfo.rSelectionBox.right = localbox.right + 1;
	if (m_stImgInfo.rSelectionBox.bottom > localbox.bottom) m_stImgInfo.rSelectionBox.bottom = localbox.bottom;

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Adds a single pixel to the existing selection.
 */
bool CxImage::SelectionAddPixel(long x, long y, BYTE level)
{
    if (m_pSelection==NULL) SelectionCreate();
	if (m_pSelection==NULL) return false;

    if (IsInside(x,y)) {
        m_pSelection[x + y * m_bmpHeadInfo.biWidth] = level; // set the correct mask bit

		if (m_stImgInfo.rSelectionBox.top <= y) m_stImgInfo.rSelectionBox.top = y+1;
		if (m_stImgInfo.rSelectionBox.left > x) m_stImgInfo.rSelectionBox.left = x;
		if (m_stImgInfo.rSelectionBox.right <= x) m_stImgInfo.rSelectionBox.right = x+1;
		if (m_stImgInfo.rSelectionBox.bottom > y) m_stImgInfo.rSelectionBox.bottom = y;

        return true;
    }

    return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Exports the selection channel in a 8bpp grayscale image.
 */
bool CxImage::SelectionSplit(CxImage *dest)
{
	if (!m_pSelection || !dest) return false;

	CxImage tmp(m_bmpHeadInfo.biWidth,m_bmpHeadInfo.biHeight,8);
	if (!tmp.IsValid()){
		strcpy(m_stImgInfo.szLastError,tmp.GetLastError());
		return false;
	}

	for(long y=0; y<m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x<m_bmpHeadInfo.biWidth; x++){
			tmp.BlindSetPixelIndex(x,y,m_pSelection[x+y*m_bmpHeadInfo.biWidth]);
		}
	}

	tmp.SetGrayPalette();
	dest->Transfer(tmp);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Creates the selection channel from a gray scale image.
 * black = unselected
 */
bool CxImage::SelectionSet(CxImage &from)
{
	if (!from.IsGrayScale() || m_bmpHeadInfo.biWidth != from.m_bmpHeadInfo.biWidth || m_bmpHeadInfo.biHeight != from.m_bmpHeadInfo.biHeight){
		strcpy(m_stImgInfo.szLastError,"CxImage::SelectionSet: wrong width or height, or image is not gray scale");
		return false;
	}

	if (m_pSelection==NULL) m_pSelection = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);

	BYTE* src = from.m_stImgInfo.pImage;
	BYTE* dst = m_pSelection;
	if (src==NULL || dst==NULL){
		strcpy(m_stImgInfo.szLastError,"CxImage::SelectionSet: null pointer");
		return false;
	}

	for (long y=0; y<m_bmpHeadInfo.biHeight; y++){
		memcpy(dst,src,m_bmpHeadInfo.biWidth);
		dst += m_bmpHeadInfo.biWidth;
		src += from.m_stImgInfo.dwEffWidth;
	}

	SelectionRebuildBox();

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the Selection level for a single pixel
 * internal use only: doesn't set SelectionBox. Use SelectionAddPixel
 */
void CxImage::SelectionSet(const long x,const long y,const BYTE level)
{
	if (m_pSelection && IsInside(x,y)) m_pSelection[x+y*m_bmpHeadInfo.biWidth]=level;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the Selection level for a single pixel 
 */
BYTE CxImage::SelectionGet(const long x,const long y)
{
	if (m_pSelection && IsInside(x,y)) return m_pSelection[x+y*m_bmpHeadInfo.biWidth];
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Rebuilds the SelectionBox 
 */
void CxImage::SelectionRebuildBox()
{
	m_stImgInfo.rSelectionBox.left = m_bmpHeadInfo.biWidth;
	m_stImgInfo.rSelectionBox.bottom = m_bmpHeadInfo.biHeight;
	m_stImgInfo.rSelectionBox.right = m_stImgInfo.rSelectionBox.top = 0;

	if (!m_pSelection)
		return;

	long x,y;

	for (y=0; y<m_bmpHeadInfo.biHeight; y++){
		for (x=0; x<m_stImgInfo.rSelectionBox.left; x++){
			if (m_pSelection[x+y*m_bmpHeadInfo.biWidth]){
				m_stImgInfo.rSelectionBox.left = x;
				continue;
			}
		}
	}

	for (y=0; y<m_bmpHeadInfo.biHeight; y++){
		for (x=m_bmpHeadInfo.biWidth-1; x>=m_stImgInfo.rSelectionBox.right; x--){
			if (m_pSelection[x+y*m_bmpHeadInfo.biWidth]){
				m_stImgInfo.rSelectionBox.right = x+1;
				continue;
			}
		}
	}

	for (x=0; x<m_bmpHeadInfo.biWidth; x++){
		for (y=0; y<m_stImgInfo.rSelectionBox.bottom; y++){
			if (m_pSelection[x+y*m_bmpHeadInfo.biWidth]){
				m_stImgInfo.rSelectionBox.bottom = y;
				continue;
			}
		}
	}

	for (x=0; x<m_bmpHeadInfo.biWidth; x++){
		for (y=m_bmpHeadInfo.biHeight-1; y>=m_stImgInfo.rSelectionBox.top; y--){
			if (m_pSelection[x+y*m_bmpHeadInfo.biWidth]){
				m_stImgInfo.rSelectionBox.top = y+1;
				continue;
			}
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the Selection level for a single pixel 
 * "blind" version assumes that (x,y) is inside to the image.
 */
BYTE CxImage::BlindSelectionGet(const long x,const long y)
{
#ifdef _DEBUG
	if (!IsInside(x,y) || (m_pSelection==0))
  #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
		throw 0;
  #else
		return 0;
  #endif
#endif
	return m_pSelection[x+y*m_bmpHeadInfo.biWidth];
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns pointer to selection data for pixel (x,y).
 */
BYTE* CxImage::SelectionGetPointer(const long x,const long y)
{
	if (m_pSelection && IsInside(x,y)) return m_pSelection+x+y*m_bmpHeadInfo.biWidth;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::SelectionFlip()
{
	if (!m_pSelection) return false;

	BYTE *buff = (BYTE*)malloc(m_bmpHeadInfo.biWidth);
	if (!buff) return false;

	BYTE *iSrc,*iDst;
	iSrc = m_pSelection + (m_bmpHeadInfo.biHeight-1)*m_bmpHeadInfo.biWidth;
	iDst = m_pSelection;
	for (long i=0; i<(m_bmpHeadInfo.biHeight/2); ++i)
	{
		memcpy(buff, iSrc, m_bmpHeadInfo.biWidth);
		memcpy(iSrc, iDst, m_bmpHeadInfo.biWidth);
		memcpy(iDst, buff, m_bmpHeadInfo.biWidth);
		iSrc-=m_bmpHeadInfo.biWidth;
		iDst+=m_bmpHeadInfo.biWidth;
	}

	free(buff);

	long top = m_stImgInfo.rSelectionBox.top;
	m_stImgInfo.rSelectionBox.top = m_bmpHeadInfo.biHeight - m_stImgInfo.rSelectionBox.bottom;
	m_stImgInfo.rSelectionBox.bottom = m_bmpHeadInfo.biHeight - top;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::SelectionMirror()
{
	if (!m_pSelection) return false;
	BYTE* pSelection2 = (BYTE*)malloc(m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight);
	if (!pSelection2) return false;
	
	BYTE *iSrc,*iDst;
	long wdt=m_bmpHeadInfo.biWidth-1;
	iSrc=m_pSelection + wdt;
	iDst=pSelection2;
	for(long y=0; y < m_bmpHeadInfo.biHeight; y++){
		for(long x=0; x <= wdt; x++)
			*(iDst+x)=*(iSrc-x);
		iSrc+=m_bmpHeadInfo.biWidth;
		iDst+=m_bmpHeadInfo.biWidth;
	}
	free(m_pSelection);
	m_pSelection=pSelection2;
	
	long left = m_stImgInfo.rSelectionBox.left;
	m_stImgInfo.rSelectionBox.left = m_bmpHeadInfo.biWidth - m_stImgInfo.rSelectionBox.right;
	m_stImgInfo.rSelectionBox.right = m_bmpHeadInfo.biWidth - left;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_WINDOWS
/**
 * Converts the selection in a HRGN object.
 */
bool CxImage::SelectionToHRGN(HRGN& region)
{
	if (m_pSelection && region){           
        for(int y = 0; y < m_bmpHeadInfo.biHeight; y++){
            HRGN hTemp = NULL;
            int iStart = -1;
            int x = 0;
			for(; x < m_bmpHeadInfo.biWidth; x++){
                if (m_pSelection[x + y * m_bmpHeadInfo.biWidth] != 0){
					if (iStart == -1) iStart = x;
					continue;
                }else{
                    if (iStart >= 0){
                        hTemp = CreateRectRgn(iStart, y, x, y + 1);
                        CombineRgn(region, hTemp, region, RGN_OR);
                        DeleteObject(hTemp);
                        iStart = -1;
                    }
                }
            }
            if (iStart >= 0){
                hTemp = CreateRectRgn(iStart, y, x, y + 1);
                CombineRgn(region, hTemp, region, RGN_OR);
                DeleteObject(hTemp);
                iStart = -1;
            }
        }
		return true;
    }
	return false;
}
#endif //CXIMAGE_SUPPORT_WINDOWS
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_SELECTION
