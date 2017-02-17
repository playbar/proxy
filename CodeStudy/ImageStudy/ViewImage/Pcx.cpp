// Pcx.cpp: implementation of the CPcx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <malloc.h>

#include "Pcx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPcx::CPcx()
{
	m_pDib = NULL;
}

CPcx::CPcx(CDib *pDib)
{
	m_pDib = NULL;
	SetDib(pDib);
}

CPcx::~CPcx()
{
	if (m_pDib != NULL)
		delete m_pDib;
}

BOOL CPcx::Load(LPCTSTR lpstrFileName)
{
    PCXPALETTE palette[256];
	PCXHEADER header;
	int width, height, bytes, widthbytes;
	BYTE *lpBuffer;
	FILE *inFile;

	/*** Open the PCX file ***/
	if ((inFile=fopen(lpstrFileName,"rb")) == NULL)
		return FALSE;

	/*** Read in the header ***/
	if (!fread((char *)&header, sizeof(PCXHEADER), 1, inFile))
	{
		fclose(inFile);
		return FALSE;
	}

	/*** Check to make sure it's a PCX ***/
	if (header.manufacturer!=0x0A || header.version!=5)
	{
		fclose(inFile);
		return FALSE;
	}

	/*** Find the palette ***/
	if (fseek(inFile, -1*(long)(sizeof(palette)+1), SEEK_END))
	{
		fclose(inFile);
		return FALSE;
	}

	/*** Read in the palette data ***/
	if (fgetc(inFile)!=0x0C || fread((char *)palette, 1, sizeof(palette), inFile) != sizeof(palette))
	{
		fclose(inFile);
		return FALSE;
	}

	/*** Seek to start of image data ***/
	fseek(inFile, (long)sizeof(PCXHEADER), SEEK_SET);

	/*** PCX dimension ***/
	width = (header.xmax - header.xmin) + 1;
	height = (header.ymax - header.ymin) + 1;
	bytes = header.bytesPerLine;
	widthbytes = WIDTHBYTES(bytes*8);

	/*** Allocate  buffer ***/
	DWORD dwBitsSize = widthbytes * height;
	if ((lpBuffer = (BYTE*)malloc(dwBitsSize)) == NULL)
	{
		fclose(inFile);
		return FALSE;
	}

	/*** Translate the image data ***/
	int i =0;
	for ( i=0; i<height; ++i)
	{
		if (! ReadPCXLine(bytes, lpBuffer+i*widthbytes, inFile))
		{
			free(lpBuffer);
			fclose(inFile);
			return FALSE;
		}
	}
	VertFlipBuf(lpBuffer, widthbytes, height);

	// file bitmap header
	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = width;
	bmiHeader.biHeight = height;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 8;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = 0;
	bmiHeader.biXPelsPerMeter = 0;
	bmiHeader.biYPelsPerMeter = 0;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;
	// color table
	RGBQUAD ColorTable[256];
	for (i = 0; i < 256; ++i)
	{
		ColorTable[i].rgbRed = palette[i].rgbRed;
		ColorTable[i].rgbGreen = palette[i].rgbGreen;
		ColorTable[i].rgbBlue = palette[i].rgbBlue;
		ColorTable[i].rgbReserved = 0;
	}
    // Allocate enough memory for the new CF_DIB, and copy bits 
	DWORD dwHeaderSize = sizeof(BITMAPINFOHEADER);
    HDIB hDIB = GlobalAlloc(GHND, dwHeaderSize + dwBitsSize + 256*sizeof(RGBQUAD)); 
	if (hDIB == NULL)
	{
		free(lpBuffer);
		fclose(inFile);
		return FALSE;
	}
    LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB); 
    memcpy(lpDIB, (LPBYTE)&bmiHeader, dwHeaderSize); 
    memcpy(lpDIB+dwHeaderSize, (LPBYTE)&ColorTable, 256*sizeof(RGBQUAD)); 
    memcpy(FindDIBBits((LPBYTE)lpDIB), lpBuffer, dwBitsSize); 

	/*** Clean up ***/
	free(lpBuffer);
	fclose(inFile);

	// create DIB
	if (m_pDib != NULL)
		delete m_pDib;

	m_pDib = new CDib();
	m_pDib->Attach(hDIB);

	return TRUE;
}

BOOL CPcx::Save(LPCTSTR lpstrFileName, CDib* pDib)
{
	if (pDib == NULL)
		pDib = m_pDib;
	if (pDib == NULL)
		return FALSE;

	HDIB hDib = CopyHandle(pDib->GetHandle());
	if (hDib == NULL)
		return FALSE;

	CDib* pDibTmp = new CDib;
	pDibTmp->Attach(hDib);

	UINT uWidth  = pDibTmp->GetWidth();
	UINT uHeight = pDibTmp->GetHeight();

	// convert to 8-bit image
	if (pDibTmp->GetBitCount() != 8)
		pDibTmp->ConvertFormat(8);

	// make PCX header
	PCXHEADER header;
	memset((LPBYTE)&header, 0, sizeof(PCXHEADER));
	header.manufacturer = 0x0A;
	header.version = 5;
	header.encoding = 1;
	header.bitsPerPixel = 8;
	header.xmin = 0;
	header.ymin = 0;
	header.xmax = uWidth-1;
	header.ymax = uHeight-1;
	//header.hres
	//header.vres;
	header.palette[0] = (BYTE)0x00;  // for correct process for mono 
	header.palette[1] = (BYTE)0x00;
	header.palette[2] = (BYTE)0x00;
	header.palette[3] = (BYTE)0xff;
	header.palette[4] = (BYTE)0xff;
	header.palette[5] = (BYTE)0xff;
	//header.palette[48];
	header.reserved = 0;
	header.colourPlanes = 1;
	header.bytesPerLine = uWidth;
	header.paletteType = 2;
	//filler[58];

	// construct PCX palette from DIB color table
    PCXPALETTE palette[256];
	PALETTEENTRY PaletteColors[256];
	pDibTmp->GetPalette()->GetPaletteEntries(0, 256, PaletteColors);
	int i = 0;
	for ( i=0;i<256;i++) 
	{
		palette[i].rgbRed   = PaletteColors[i].peRed;
		palette[i].rgbGreen = PaletteColors[i].peGreen;
		palette[i].rgbBlue  = PaletteColors[i].peBlue;
	}

	// get bits ptr
	HDIB hDIB = CopyHandle(pDibTmp->GetHandle());
	delete pDibTmp;
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	BYTE* lpBuffer = (BYTE *)FindDIBBits(lpDIB);
	WORD wWidthBytes = (WORD)BytesPerLine(lpDIB);

	/*** Open the PCX file ***/
	FILE *outFile;
	if ((outFile=fopen(lpstrFileName,"wb")) == NULL)
	{
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);
		return FALSE;
	}

	/*** Write the header ***/
	fwrite((char *)&header, sizeof(PCXHEADER), 1, outFile);

	/*** Write image data ***/
	for (i=(int)uHeight-1; i>=0; --i)
	{
		if (! WritePCXLine(header.bytesPerLine, lpBuffer+i*wWidthBytes, outFile))
		{
			fclose(outFile);
			GlobalUnlock(hDIB);
			GlobalFree(hDIB);
			return FALSE;
		}
	}

	/*** Write the palette data ***/
	fputc(0x0c, outFile);
	fwrite((char *)palette, 1, sizeof(palette), outFile);

	// clear & close
	fclose(outFile);
	GlobalUnlock(hDIB);
	GlobalFree(hDIB);

	return TRUE;
}

/****
***** Read a single line of a PCX file.
****/
BOOL CPcx::ReadPCXLine(int bytes, BYTE *p, FILE *fp)
{
	int n=0;
	register int c, i;

	/*** Null the buffer ***/
	do
	{
		c = fgetc(fp) & 0xFF;                   /* get a key byte */
		/*** If it's a run of bytes field ***/
		if ((c&0xC0) == 0xC0)
		{
			i = c & 0x3F;                         /* AND off high bits */
			c = fgetc(fp);                        /* get the run byte */
			while (i--)                           /* run the byte */
				p[n++] = c;
		}
		/*** Else just store it ***/
		else
			p[n++] = c;
	}	while (n < bytes);

	return (ferror(fp) ? FALSE : TRUE);
}

/****
***** Write a single line of a PCX file.
****/
BOOL CPcx::WritePCXLine(int bytes, BYTE *p, FILE *fp)
{
	unsigned int i=0, j=0, t=0;

	do
	{
		i = 0;
		while ((p[t+i]==p[t+i+1]) && ((t+i)<(WORD)bytes) && (i<63))
			++i;
		if (i > 0) /* there are i equal pixels from current position */
		{
			fputc(i|0xc0, fp);          /* write duplicated number */
			fputc(p[t], fp);           /* write pixel value */
			t += i;						/* bytes have been processed */
			j += 2;						/* bytes have been written to file */
		}
		else	/* no duplicated pixel */
		{
			if (((p[t]) & 0xc0) == 0xc0) /* pixel value > 192 ? */
			{
				fputc(0xc1, fp);        /* write identify sign 0xc1 */
				++j;
			}
			fputc(p[t++], fp);
			++j;
		}
	} while (t < (WORD)bytes);

	return (ferror(fp) ? FALSE : TRUE);
}

//
//	vertically flip a buffer 
//	note, this operates on a buffer of widthBytes bytes, not pixels!!!
//
BOOL CPcx::VertFlipBuf(BYTE  * inbuf, UINT widthBytes, UINT height)
{   
	BYTE  *tb1;
	BYTE  *tb2;

	if (inbuf==NULL)
		return FALSE;

	UINT bufsize;

	bufsize=widthBytes;

	tb1= (BYTE *)new BYTE[bufsize];
	if (tb1==NULL) {
		return FALSE;
	}

	tb2= (BYTE *)new BYTE [bufsize];
	if (tb1==NULL) {
		return FALSE;
	}
	
	UINT row_cnt;     
	ULONG off1=0;
	ULONG off2=0;

	for (row_cnt=0;row_cnt<(height+1)/2;row_cnt++) {
		off1=row_cnt*bufsize;
		off2=((height-1)-row_cnt)*bufsize;   
		
		memcpy(tb1,inbuf+off1,bufsize);
		memcpy(tb2,inbuf+off2,bufsize);	
		memcpy(inbuf+off1,tb2,bufsize);
		memcpy(inbuf+off2,tb1,bufsize);
	}	

	delete [] tb1;
	delete [] tb2;

	return TRUE;
}        

