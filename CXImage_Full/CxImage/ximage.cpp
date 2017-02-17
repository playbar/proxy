// ximage.cpp : main implementation file
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

////////////////////////////////////////////////////////////////////////////////
// CxImage 
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize the internal structures
 */
void CxImage::Startup(DWORD imagetype)
{
	//init pointers
	m_pDib = m_pSelection = m_pAlpha = NULL;
	m_ppLayers = m_ppFrames = NULL;
	//init structures
	memset(&m_bmpHeadInfo,0,sizeof(BITMAPINFOHEADER));
	memset(&m_stImgInfo,0,sizeof(CXIMAGEINFO));
	//init default attributes
    m_stImgInfo.dwType = imagetype;
	m_stImgInfo.fQuality = 90.0f;
	m_stImgInfo.nAlphaMax = 255;
	m_stImgInfo.nBkgndIndex = -1;
	m_stImgInfo.bEnabled = true;
	SetXDPI(CXIMAGE_DEFAULT_DPI);
	SetYDPI(CXIMAGE_DEFAULT_DPI);

	short test = 1;
	m_stImgInfo.bLittleEndianHost = (*((char *) &test) == 1);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Empty image constructor
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(DWORD imagetype)
{
	Startup(imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Call this function to destroy image pixels, alpha channel, selection and sub layers.
 * - Attributes are not erased, but IsValid returns false.
 *
 * \return true if everything is freed, false if the image is a Ghost
 */
bool CxImage::Destroy()
{
	//free this only if it's valid and it's not a ghost
	if (m_stImgInfo.pGhost==NULL){
		if (m_ppLayers) { 
			for(long n=0; n<m_stImgInfo.nNumLayers;n++){ delete m_ppLayers[n]; }
			delete [] m_ppLayers; m_ppLayers=0; m_stImgInfo.nNumLayers = 0;
		}
		if (m_pSelection) {free(m_pSelection); m_pSelection=0;}
		if (m_pAlpha) {free(m_pAlpha); m_pAlpha=0;}
		if (m_pDib) {free(m_pDib); m_pDib=0;}
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::DestroyFrames()
{
	if (m_stImgInfo.pGhost==NULL) {
		if (m_ppFrames) {
			for (long n=0; n<m_stImgInfo.nNumFrames; n++) { delete m_ppFrames[n]; }
			delete [] m_ppFrames; m_ppFrames = NULL; m_stImgInfo.nNumFrames = 0;
		}
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sized image constructor
 * \param dwWidth: width
 * \param dwHeight: height
 * \param wBpp: bit per pixel, can be 1, 4, 8, 24
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(DWORD dwWidth, DWORD dwHeight, DWORD wBpp, DWORD imagetype)
{
	Startup(imagetype);
	Create(dwWidth,dwHeight,wBpp,imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * image constructor from existing source
 * \param src: source image.
 * \param copypixels: copy the pixels from the source image into the new image.
 * \param copyselection: copy the selection from source
 * \param copyalpha: copy the alpha channel from source
 * \sa Copy
 */
CxImage::CxImage(const CxImage &src, bool copypixels, bool copyselection, bool copyalpha)
{
	Startup(src.GetType());
	Copy(src,copypixels,copyselection,copyalpha);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Copies the image from an exsisting source
 * \param src: source image.
 * \param copypixels: copy the pixels from the source image into the new image.
 * \param copyselection: copy the selection from source
 * \param copyalpha: copy the alpha channel from source
 */
void CxImage::Copy(const CxImage &src, bool copypixels, bool copyselection, bool copyalpha)
{
	// if the source is a ghost, the copy is still a ghost
	if (src.m_stImgInfo.pGhost){
		Ghost(&src);
		return;
	}
	//copy the attributes
	memcpy(&m_stImgInfo,&src.m_stImgInfo,sizeof(CXIMAGEINFO));
	memcpy(&m_bmpHeadInfo,&src.m_bmpHeadInfo,sizeof(BITMAPINFOHEADER)); // [andy] - fix for bitmap header DPI
	//rebuild the image
	Create(src.GetWidth(),src.GetHeight(),src.GetBpp(),src.GetType());
	//copy the pixels and the palette, or at least copy the palette only.
	if (copypixels && m_pDib && src.m_pDib) memcpy(m_pDib,src.m_pDib,GetSize());
	else SetPalette(src.GetPalette());
	long nSize = m_bmpHeadInfo.biWidth * m_bmpHeadInfo.biHeight;
	//copy the selection
	if (copyselection && src.m_pSelection){
		if (m_pSelection) free(m_pSelection);
		m_pSelection = (BYTE*)malloc(nSize);
		memcpy(m_pSelection,src.m_pSelection,nSize);
	}
	//copy the alpha channel
	if (copyalpha && src.m_pAlpha){
		if (m_pAlpha) free(m_pAlpha);
		m_pAlpha = (BYTE*)malloc(nSize);
		memcpy(m_pAlpha,src.m_pAlpha,nSize);
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Copies the image attributes from an existing image.
 * - Works only on an empty image, and the image will be still empty.
 * - <b> Use it before Create() </b>
 */
void CxImage::CopyInfo(const CxImage &src)
{
	if (m_pDib==NULL) memcpy(&m_stImgInfo,&src.m_stImgInfo,sizeof(CXIMAGEINFO));
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \sa Copy
 */
CxImage& CxImage::operator = (const CxImage& isrc)
{
	if (this != &isrc) Copy(isrc);
	return *this;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Initializes or rebuilds the image.
 * \param dwWidth: width
 * \param dwHeight: height
 * \param wBpp: bit per pixel, can be 1, 4, 8, 24
 * \param imagetype: (optional) set the image format, see ENUM_CXIMAGE_FORMATS
 * \return pointer to the internal pDib object; NULL if an error occurs.
 */
void* CxImage::Create(DWORD dwWidth, DWORD dwHeight, DWORD wBpp, DWORD imagetype)
{
	// destroy the existing image (if any)
	if (!Destroy())
		return NULL;

	// prevent further actions if width or height are not vaild <Balabasnia>
	if ((dwWidth == 0) || (dwHeight == 0)){
		strcpy(m_stImgInfo.szLastError,"CxImage::Create : width and height must be greater than zero");
		return NULL;
	}

    // Make sure bits per pixel is valid
    if		(wBpp <= 1)	wBpp = 1;
    else if (wBpp <= 4)	wBpp = 4;
    else if (wBpp <= 8)	wBpp = 8;
    else				wBpp = 24;

	// limit memory requirements (and also a check for bad parameters)
	if (((dwWidth*dwHeight*wBpp)>>3) > CXIMAGE_MAX_MEMORY ||
		((dwWidth*dwHeight*wBpp)/wBpp) != (dwWidth*dwHeight))
	{
		strcpy(m_stImgInfo.szLastError,"CXIMAGE_MAX_MEMORY exceeded");
		return NULL;
	}

	// set the correct bpp value
    switch (wBpp){
        case 1:
            m_bmpHeadInfo.biClrUsed = 2;	break;
        case 4:
            m_bmpHeadInfo.biClrUsed = 16; break;
        case 8:
            m_bmpHeadInfo.biClrUsed = 256; break;
        default:
            m_bmpHeadInfo.biClrUsed = 0;
    }

	//set the common image informations
    m_stImgInfo.dwEffWidth = ((((wBpp * dwWidth) + 31) / 32) * 4);
    m_stImgInfo.dwType = imagetype;

    // initialize BITMAPINFOHEADER
	m_bmpHeadInfo.biSize = sizeof(BITMAPINFOHEADER); //<ralphw>
    m_bmpHeadInfo.biWidth = dwWidth;		// fill in width from parameter
    m_bmpHeadInfo.biHeight = dwHeight;	// fill in height from parameter
    m_bmpHeadInfo.biPlanes = 1;			// must be 1
    m_bmpHeadInfo.biBitCount = (WORD)wBpp;		// from parameter
    m_bmpHeadInfo.biCompression = BI_RGB;    
    m_bmpHeadInfo.biSizeImage = m_stImgInfo.dwEffWidth * dwHeight;
//    head.biXPelsPerMeter = 0; See SetXDPI
//    head.biYPelsPerMeter = 0; See SetYDPI
//    head.biClrImportant = 0;  See SetClrImportant

	m_pDib = malloc(GetSize()); // alloc memory block to store our bitmap
    if (!m_pDib){
		strcpy(m_stImgInfo.szLastError,"CxImage::Create can't allocate memory");
		return NULL;
	}

	//clear the palette
	RGBQUAD* pal=GetPalette();
	if (pal) memset(pal,0,GetPaletteSize());
	//Destroy the existing selection
#if CXIMAGE_SUPPORT_SELECTION
	if (m_pSelection) SelectionDelete();
#endif //CXIMAGE_SUPPORT_SELECTION
	//Destroy the existing alpha channel
#if CXIMAGE_SUPPORT_ALPHA
	if (m_pAlpha) AlphaDelete();
#endif //CXIMAGE_SUPPORT_ALPHA

    // use our bitmap info structure to fill in first part of
    // our DIB with the BITMAPINFOHEADER
    BITMAPINFOHEADER*  lpbi;
	lpbi = (BITMAPINFOHEADER*)(m_pDib);
    *lpbi = m_bmpHeadInfo;

	m_stImgInfo.pImage=GetBits();

    return m_pDib; //return handle to the DIB
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return pointer to the image pixels. <b> USE CAREFULLY </b>
 */
BYTE* CxImage::GetBits(DWORD row)
{ 
	if (m_pDib){
		if (row) {
			if (row<(DWORD)m_bmpHeadInfo.biHeight){
				return ((BYTE*)m_pDib + *(DWORD*)m_pDib + GetPaletteSize() + (m_stImgInfo.dwEffWidth * row));
			} else {
				return NULL;
			}
		} else {
			return ((BYTE*)m_pDib + *(DWORD*)m_pDib + GetPaletteSize());
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return the size in bytes of the internal pDib object
 */
long CxImage::GetSize()
{
	return m_bmpHeadInfo.biSize + m_bmpHeadInfo.biSizeImage + GetPaletteSize();
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the coordinates are inside the image
 * \return true if x and y are both inside the image
 */
bool CxImage::IsInside(long x, long y)
{
  return (0<=y && y<m_bmpHeadInfo.biHeight && 0<=x && x<m_bmpHeadInfo.biWidth);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the image bits to the specified value
 * - for indexed images, the output color is set by the palette entries.
 * - for RGB images, the output color is a shade of gray.
 */
void CxImage::Clear(BYTE bval)
{
	if (m_pDib == 0) return;

	if (GetBpp() == 1){
		if (bval > 0) bval = 255;
	}
	if (GetBpp() == 4){
		bval = (BYTE)(17*(0x0F & bval));
	}

	memset(m_stImgInfo.pImage,bval,m_bmpHeadInfo.biSizeImage);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Transfers the image from an existing source image. The source becomes empty.
 * \return true if everything is ok
 */
bool CxImage::Transfer(CxImage &from, bool bTransferFrames /*=true*/)
{
	if (!Destroy())
		return false;

	memcpy(&m_bmpHeadInfo,&from.m_bmpHeadInfo,sizeof(BITMAPINFOHEADER));
	memcpy(&m_stImgInfo,&from.m_stImgInfo,sizeof(CXIMAGEINFO));

	m_pDib = from.m_pDib;
	m_pSelection = from.m_pSelection;
	m_pAlpha = from.m_pAlpha;
	m_ppLayers = from.m_ppLayers;

	memset(&from.m_bmpHeadInfo,0,sizeof(BITMAPINFOHEADER));
	memset(&from.m_stImgInfo,0,sizeof(CXIMAGEINFO));
	from.m_pDib = from.m_pSelection = from.m_pAlpha = NULL;
	from.m_ppLayers = NULL;

	if (bTransferFrames){
		DestroyFrames();
		m_ppFrames = from.m_ppFrames;
		from.m_ppFrames = NULL;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * (this) points to the same pDib owned by (*from), the image remains in (*from)
 * but (this) has the access to the pixels. <b>Use carefully !!!</b>
 */
void CxImage::Ghost(const CxImage *from)
{
	if (from)
	{
		memcpy(&m_bmpHeadInfo,&from->m_bmpHeadInfo,sizeof(BITMAPINFOHEADER));
		memcpy(&m_stImgInfo,&from->m_stImgInfo,sizeof(CXIMAGEINFO));
		m_pDib = from->m_pDib;
		m_pSelection = from->m_pSelection;
		m_pAlpha = from->m_pAlpha;
		m_ppLayers = from->m_ppLayers;
		m_ppFrames = from->m_ppFrames;
		m_stImgInfo.pGhost=(CxImage *)from;
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * turns a 16 or 32 bit bitfield image into a RGB image
 */
void CxImage::Bitfield2RGB(BYTE *src, DWORD redmask, DWORD greenmask, DWORD bluemask, BYTE bpp)
{
	switch (bpp){
	case 16:
	{
		DWORD ns[3]={0,0,0};
		// compute the number of shift for each mask
		for (int i=0;i<16;i++){
			if ((redmask>>i)&0x01) ns[0]++;
			if ((greenmask>>i)&0x01) ns[1]++;
			if ((bluemask>>i)&0x01) ns[2]++;
		}
		ns[1]+=ns[0]; ns[2]+=ns[1];	ns[0]=8-ns[0]; ns[1]-=8; ns[2]-=8;
		// dword aligned width for 16 bit image
		long effwidth2=(((m_bmpHeadInfo.biWidth + 1) / 2) * 4);
		WORD w;
		long y2,y3,x2,x3;
		BYTE *p=m_stImgInfo.pImage;
		// scan the buffer in reverse direction to avoid reallocations
		for (long y=m_bmpHeadInfo.biHeight-1; y>=0; y--){
			y2=effwidth2*y;
			y3=m_stImgInfo.dwEffWidth*y;
			for (long x=m_bmpHeadInfo.biWidth-1; x>=0; x--){
				x2 = 2*x+y2;
				x3 = 3*x+y3;
				w = (WORD)(src[x2]+256*src[1+x2]);
				p[  x3]=(BYTE)((w & bluemask)<<ns[0]);
				p[1+x3]=(BYTE)((w & greenmask)>>ns[1]);
				p[2+x3]=(BYTE)((w & redmask)>>ns[2]);
			}
		}
		break;
	}
	case 32:
	{
		DWORD ns[3]={0,0,0};
		// compute the number of shift for each mask
		for (int i=8;i<32;i+=8){
			if (redmask>>i) ns[0]++;
			if (greenmask>>i) ns[1]++;
			if (bluemask>>i) ns[2]++;
		}
		// dword aligned width for 32 bit image
		long effwidth4 = m_bmpHeadInfo.biWidth * 4;
		long y4,y3,x4,x3;
		BYTE *p=m_stImgInfo.pImage;
		// scan the buffer in reverse direction to avoid reallocations
		for (long y=m_bmpHeadInfo.biHeight-1; y>=0; y--){
			y4=effwidth4*y;
			y3=m_stImgInfo.dwEffWidth*y;
			for (long x=m_bmpHeadInfo.biWidth-1; x>=0; x--){
				x4 = 4*x+y4;
				x3 = 3*x+y3;
				p[  x3]=src[ns[2]+x4];
				p[1+x3]=src[ns[1]+x4];
				p[2+x3]=src[ns[0]+x4];
			}
		}
	}

	}
	return;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Creates an image from a generic buffer
 * \param pArray: source memory buffer
 * \param dwWidth: image width
 * \param dwHeight: image height
 * \param dwBitsperpixel: can be 1,4,8,24,32
 * \param dwBytesperline: line alignment, in bytes, for a single row stored in pArray
 * \param bFlipImage: tune this parameter if the image is upsidedown
 * \return true if everything is ok
 */
bool CxImage::CreateFromArray(BYTE* pArray,DWORD dwWidth,DWORD dwHeight,DWORD dwBitsperpixel, DWORD dwBytesperline, bool bFlipImage)
{
	if (pArray==NULL) return false;
	if (!((dwBitsperpixel==1)||(dwBitsperpixel==4)||(dwBitsperpixel==8)||
		(dwBitsperpixel==24)||(dwBitsperpixel==32))) return false;

	if (!Create(dwWidth,dwHeight,dwBitsperpixel)) return false;

	if (dwBitsperpixel<24) SetGrayPalette();

#if CXIMAGE_SUPPORT_ALPHA
	if (dwBitsperpixel==32) AlphaCreate();
#endif //CXIMAGE_SUPPORT_ALPHA

	BYTE *dst,*src;

	for (DWORD y = 0; y<dwHeight; y++) {
		dst = m_stImgInfo.pImage + (bFlipImage?(dwHeight-1-y):y) * m_stImgInfo.dwEffWidth;
		src = pArray + y * dwBytesperline;
		if (dwBitsperpixel==32){
			for(DWORD x=0;x<dwWidth;x++){
				*dst++=src[0];
				*dst++=src[1];
				*dst++=src[2];
#if CXIMAGE_SUPPORT_ALPHA
				AlphaSet(x,(bFlipImage?(dwHeight-1-y):y),src[3]);
#endif //CXIMAGE_SUPPORT_ALPHA
				src+=4;
			}
		} else {
			memcpy(dst,src,min(m_stImgInfo.dwEffWidth,dwBytesperline));
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \sa CreateFromArray
 */
bool CxImage::CreateFromMatrix(BYTE** ppMatrix,DWORD dwWidth,DWORD dwHeight,DWORD dwBitsperpixel, DWORD dwBytesperline, bool bFlipImage)
{
	if (ppMatrix==NULL) return false;
	if (!((dwBitsperpixel==1)||(dwBitsperpixel==4)||(dwBitsperpixel==8)||
		(dwBitsperpixel==24)||(dwBitsperpixel==32))) return false;

	if (!Create(dwWidth,dwHeight,dwBitsperpixel)) return false;

	if (dwBitsperpixel<24) SetGrayPalette();

#if CXIMAGE_SUPPORT_ALPHA
	if (dwBitsperpixel==32) AlphaCreate();
#endif //CXIMAGE_SUPPORT_ALPHA

	BYTE *dst,*src;

	for (DWORD y = 0; y<dwHeight; y++) {
		dst = m_stImgInfo.pImage + (bFlipImage?(dwHeight-1-y):y) * m_stImgInfo.dwEffWidth;
		src = ppMatrix[y];
		if (src){
			if (dwBitsperpixel==32){
				for(DWORD x=0;x<dwWidth;x++){
					*dst++=src[0];
					*dst++=src[1];
					*dst++=src[2];
#if CXIMAGE_SUPPORT_ALPHA
					AlphaSet(x,(bFlipImage?(dwHeight-1-y):y),src[3]);
#endif //CXIMAGE_SUPPORT_ALPHA
					src+=4;
				}
			} else {
				memcpy(dst,src,min(m_stImgInfo.dwEffWidth,dwBytesperline));
			}
		}
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return lightness difference between elem1 and elem2
 */
int CxImage::CompareColors(const void *elem1, const void *elem2)
{
	RGBQUAD* c1 = (RGBQUAD*)elem1;
	RGBQUAD* c2 = (RGBQUAD*)elem2;

	int g1 = (int)RGB2GRAY(c1->rgbRed,c1->rgbGreen,c1->rgbBlue);
	int g2 = (int)RGB2GRAY(c2->rgbRed,c2->rgbGreen,c2->rgbBlue);
	
	return (g1-g2);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * simply calls "if (memblock) free(memblock);".
 * Useful when calling Encode for a memory buffer,
 * from a DLL compiled with different memory management options.
 * CxImage::FreeMemory will use the same memory environment used by Encode. 
 * \author [livecn]
 */
void CxImage::FreeMemory(void* memblock)
{
	if (memblock)
		free(memblock);
}
////////////////////////////////////////////////////////////////////////////////
//EOF
