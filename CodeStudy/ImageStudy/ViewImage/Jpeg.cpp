// Jpeg.cpp: implementation of the CJpeg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>

#include "Jpeg.h"

#ifdef __cplusplus
	extern "C" {
#endif // __cplusplus

#include "jpeglib.h"

#ifdef __cplusplus
	}
#endif // __cplusplus

//
//
//

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */

#include <setjmp.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// error handler, to avoid those pesky exit(0)'s

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

//
//
//

METHODDEF(void) my_error_exit (j_common_ptr cinfo);

//
//	to handle fatal errors.
//	the original JPEG code will just exit(0). can't really
//	do that in Windows....
//

METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	char buffer[JMSG_LENGTH_MAX];

	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);

	/* Always display the message. */
	MessageBox(GetActiveWindow(),buffer,"JPEG Fatal Error",MB_ICONSTOP);


	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}

// store a scanline to our data buffer
void j_putRGBScanline(BYTE *jpegline, 
						 int widthPix,
						 BYTE *outBuf,
						 int row);

void j_putGrayScanlineToRGB(BYTE *jpegline, 
						 int widthPix,
						 BYTE *outBuf,
						 int row);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJpeg::CJpeg()
{
	m_strJPEGError = "No Error"; // yet
	m_pDib = NULL;
}

CJpeg::CJpeg(CDib *pDib)
{
	m_strJPEGError = "No Error"; // yet
	m_pDib = NULL;
	SetDib(pDib);
}

CJpeg::~CJpeg()
{
	if (m_pDib != NULL)
		delete m_pDib;
}

// free allocate memory
void CJpeg::FreeBuffer(BYTE *Buffer)
{
	delete[] Buffer;
}

// get error string
CString CJpeg::GetErrorString()
{
	return m_strJPEGError;
}

// load gif file
BOOL CJpeg::Load(LPCSTR lpstrFileName)
{
	UINT uWidth, uHeight, uWidthDW;

	// read the GIF to a packed buffer of RGB bytes
	BYTE *lpTmpBuffer = ReadJPEGFile(lpstrFileName, &uWidth, &uHeight);
	if (lpTmpBuffer == NULL)
		return FALSE;

	// do this before DWORD-alignment!!!
	// swap red and blue for display
	BGRFromRGB(lpTmpBuffer, uWidth, uHeight);

	// now DWORD-align for display
	BYTE *lpBuffer = MakeDwordAlign(lpTmpBuffer, uWidth, uHeight, &uWidthDW);
	FreeBuffer(lpTmpBuffer);

	// flip for display
	VertFlipBuf(lpBuffer, uWidthDW, uHeight);

	BITMAPINFOHEADER bmiHeader;
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = uWidth;
	bmiHeader.biHeight = uHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biCompression = BI_RGB;
	bmiHeader.biSizeImage = 0;
	bmiHeader.biXPelsPerMeter = 0;
	bmiHeader.biYPelsPerMeter = 0;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;

    // Allocate enough memory for the new CF_DIB, and copy bits 
	DWORD dwHeaderSize = sizeof(BITMAPINFOHEADER);
	DWORD dwBitsSize = WIDTHBYTES(uWidth*24) * uHeight;
    HDIB hDIB = GlobalAlloc(GHND, dwHeaderSize + dwBitsSize); 
	if (hDIB == NULL)
		return FALSE;

    LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB); 
    memcpy(lpDIB, (LPBYTE)&bmiHeader, dwHeaderSize); 
    memcpy(FindDIBBits((LPBYTE)lpDIB), lpBuffer, dwBitsSize); 
	FreeBuffer(lpBuffer);

	if (m_pDib != NULL)
		delete m_pDib;

	m_pDib = new CDib();
	m_pDib->Attach(hDIB);

	return TRUE;
}

// save Jpeg file
BOOL CJpeg::Save(LPCSTR lpstrFileName, CDib* pDib, BOOL bColor, int nQuality)
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

	if (pDibTmp->GetBitCount() != 24)
		pDibTmp->ConvertFormat(24);

	UINT uWidth  = pDibTmp->GetWidth();
	UINT uHeight = pDibTmp->GetHeight();

	// convert from DIB format (DWORD aligned, vertically flipped, red and blue swapped)
	BYTE* tmp = ClearDwordAlign(pDibTmp->GetBitsPtr(),
									uWidth,
									WIDTHBYTES(uWidth * 24),
									uHeight);
	if (tmp == NULL)
		return FALSE;

	// convert from DIB
	VertFlipBuf(tmp, uWidth*3, uHeight);

	BGRFromRGB(tmp, uWidth, uHeight);

	BOOL bSuccess = WriteJPEGFile(lpstrFileName,
							tmp,
							uWidth, 
							uHeight,
							bColor,
							nQuality);

	delete pDibTmp;
	FreeBuffer(tmp);

	return bSuccess;
}

//
// copies BYTE buffer into DWORD-aligned BYTE buffer
// return addr of new buffer
//
BYTE * CJpeg::MakeDwordAlign(BYTE *dataBuf,
								 UINT widthPix,				// pixels!!
								 UINT height,
								 UINT *uiOutWidthBytes)		// bytes!!!
{
	////////////////////////////////////////////////////////////
	// what's going on here? this certainly means trouble 
	if (dataBuf==NULL)
		return NULL;

	////////////////////////////////////////////////////////////
	// how big is the smallest DWORD-aligned buffer that we can use?
	UINT uiWidthBytes;
	uiWidthBytes = WIDTHBYTES(widthPix * 24);

	DWORD dwNewsize=(DWORD)((DWORD)uiWidthBytes * 
							(DWORD)height);
	BYTE *pNew;

	////////////////////////////////////////////////////////////
	// alloc and open our new buffer
	pNew=(BYTE *)new BYTE[dwNewsize];
	if (pNew==NULL) {
		return NULL;
	}
	
	////////////////////////////////////////////////////////////
	// copy row-by-row
	UINT uiInWidthBytes = widthPix * 3;
	UINT uiCount;
	for (uiCount=0;uiCount < height;uiCount++) 
	{
		BYTE * bpInAdd;
		BYTE * bpOutAdd;
		ULONG lInOff;
		ULONG lOutOff;

		lInOff=uiInWidthBytes * uiCount;
		lOutOff=uiWidthBytes * uiCount;

		bpInAdd= dataBuf + lInOff;
		bpOutAdd= pNew + lOutOff;

		memcpy(bpOutAdd,bpInAdd,uiInWidthBytes);
	}

	*uiOutWidthBytes=uiWidthBytes;
	return pNew;
}

//
//	vertically flip a buffer 
//	note, this operates on a buffer of widthBytes bytes, not pixels!!!
//
BOOL CJpeg::VertFlipBuf(BYTE* inbuf, UINT widthBytes, UINT height)
{   
	BYTE  *tb1;
	BYTE  *tb2;

	if (inbuf==NULL)
		return FALSE;

	UINT bufsize;

	bufsize=widthBytes;

	tb1= (BYTE *)new BYTE[bufsize];
	if (tb1==NULL) 
	{
		return FALSE;
	}

	tb2= (BYTE *)new BYTE [bufsize];
	if (tb1==NULL) 
	{
		return FALSE;
	}
	
	UINT row_cnt;     
	ULONG off1=0;
	ULONG off2=0;

	for (row_cnt=0;row_cnt<(height+1)/2;row_cnt++) 
	{
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

//
//	swap Rs and Bs
//
//	Note! this does its stuff on buffers with a whole number of pixels
//	per data row!!
//
BOOL CJpeg::BGRFromRGB(BYTE *buf, UINT widthPix, UINT height)
{
	if (buf==NULL)
		return FALSE;

	UINT col, row;
	for (row=0;row<height;row++) 
	{
		for (col=0;col<widthPix;col++) 
		{
			LPBYTE pRed, pGrn, pBlu;
			pRed = buf + row * widthPix * 3 + col * 3;
			pGrn = buf + row * widthPix * 3 + col * 3 + 1;
			pBlu = buf + row * widthPix * 3 + col * 3 + 2;

			// swap red and blue
			BYTE tmp;
			tmp = *pRed;
			*pRed = *pBlu;
			*pBlu = tmp;
		}
	}
	return TRUE;
}

BYTE* CJpeg::ClearDwordAlign(BYTE *inBuf,
							UINT widthPix, 
							UINT widthBytes,
							UINT height)
{
	if (inBuf==NULL)
		return FALSE;


	BYTE *tmp;
	tmp=(BYTE *)new BYTE[height * widthPix * 3];
	if (tmp==NULL)
		return NULL;

	UINT row;

	for (row=0;row<height;row++) 
	{
		memcpy((tmp+row * widthPix * 3), 
				(inBuf + row * widthBytes), 
				widthPix * 3);
	}

	return tmp;
}


//
//	read a JPEG file
//
BYTE* CJpeg::ReadJPEGFile(LPCSTR lpstrFileName, UINT *uWidth, UINT *uHeight)
{
	// basic code from IJG Jpeg Code v6 example.c

	*uWidth=0;
	*uHeight=0;

	/* This struct contains the JPEG decompression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	*/
	struct jpeg_decompress_struct cinfo;
	/* We use our private extension JPEG error handler.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct my_error_mgr jerr;
	/* More stuff */
	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	char buf[250];

	/* In this example we want to open the input file before doing anything else,
	* so that the setjmp() error recovery below can assume the file is open.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to read binary files.
	*/

	if ((infile = fopen(lpstrFileName, "rb")) == NULL) 
	{
		sprintf(buf, "JPEG :\nCan't open %s\n", lpstrFileName);
		m_strJPEGError = buf;
		return NULL;
	}

	/* Step 1: allocate and initialize JPEG decompression object */

	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;


	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) 
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */

		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return NULL;
	}

	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);

	/* Step 2: specify data source (eg, a file) */

	jpeg_stdio_src(&cinfo, infile);

	/* Step 3: read file parameters with jpeg_read_header() */

	(void) jpeg_read_header(&cinfo, TRUE);
	/* We can ignore the return value from jpeg_read_header since
	*   (a) suspension is not possible with the stdio data source, and
	*   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	* See libjpeg.doc for more info.
	*/

	/* Step 4: set parameters for decompression */

	/* In this example, we don't need to change any of the defaults set by
	* jpeg_read_header(), so we do nothing here.
	*/

	/* Step 5: Start decompressor */

	(void) jpeg_start_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* We may need to do some setup of our own at this point before reading
	* the data.  After jpeg_start_decompress() we have the correct scaled
	* output image dimensions available, as well as the output colormap
	* if we asked for color quantization.
	* In this example, we need to make an output work buffer of the right size.
	*/ 

	// get our buffer set to hold data
	BYTE *dataBuf;

	////////////////////////////////////////////////////////////
	// alloc and open our new buffer
	dataBuf=(BYTE *)new BYTE[cinfo.output_width * 3 * cinfo.output_height];
	if (dataBuf==NULL) 
	{

		m_strJPEGError = "JpegFile :\nOut of memory";

		jpeg_destroy_decompress(&cinfo);
		
		fclose(infile);

		return NULL;
	}

	// how big is this thing gonna be?
	*uWidth = cinfo.output_width;
	*uHeight = cinfo.output_height;
	
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;

	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	/* Here we use the library's state variable cinfo.output_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	*/
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */

		// asuumer all 3-components are RGBs
		if (cinfo.out_color_components==3) 
		{
			
			j_putRGBScanline(buffer[0], 
							*uWidth,
							dataBuf,
							cinfo.output_scanline-1);

		} 
		else if (cinfo.out_color_components==1) 
		{

			// assume all single component images are grayscale
			j_putGrayScanlineToRGB(buffer[0], 
								*uWidth,
								dataBuf,
								cinfo.output_scanline-1);

		}

	}

	/* Step 7: Finish decompression */

	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

	/* Step 8: Release JPEG decompression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);

	/* After finish_decompress, we can close the input file.
	* Here we postpone it until after no more JPEG errors are possible,
	* so as to simplify the setjmp error logic above.  (Actually, I don't
	* think that jpeg_destroy can do an error exit, but why assume anything...)
	*/
	fclose(infile);

	/* At this point you may want to check to see whether any corrupt-data
	* warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	*/

	return dataBuf;
}

//
//	write a JPEG file
//
BOOL CJpeg::WriteJPEGFile(LPCSTR lpstrFileName, 
						BYTE *dataBuf,
						UINT widthPix,
						UINT height,
						BOOL color, 
						int quality)
{
	if (dataBuf==NULL)
		return FALSE;
	if (widthPix==0)
		return FALSE;
	if (height==0)
		return FALSE;

	LPBYTE tmp;
	if (!color) 
	{
		tmp = (BYTE*)new BYTE[widthPix*height];
		if (tmp==NULL) 
		{
			m_strJPEGError = "Memory error";
			return FALSE;
		}

		UINT row,col;
		for (row=0;row<height;row++) 
		{
			for (col=0;col<widthPix;col++) 
			{
				LPBYTE pRed, pGrn, pBlu;
				pRed = dataBuf + row * widthPix * 3 + col * 3;
				pGrn = dataBuf + row * widthPix * 3 + col * 3 + 1;
				pBlu = dataBuf + row * widthPix * 3 + col * 3 + 2;

				// luminance
				int lum = (int)(.299 * (double)(*pRed) + .587 * (double)(*pGrn) + .114 * (double)(*pBlu));
				LPBYTE pGray;
				pGray = tmp + row * widthPix + col;
				*pGray = (BYTE)lum;
			}
		}
	}

	struct jpeg_compress_struct cinfo;
	/* More stuff */
	FILE * outfile;			/* target file */
	int row_stride;			/* physical row widthPix in image buffer */

	struct my_error_mgr jerr;

	/* Step 1: allocate and initialize JPEG compression object */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) 
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */

		jpeg_destroy_compress(&cinfo);
		fclose(outfile);

		if (!color) 
		{
			delete [] tmp;
		}
		return FALSE;
	}

	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	/* Step 2: specify data destination (eg, a file) */
	/* Note: steps 2 and 3 can be done in either order. */

	if ((outfile = fopen(lpstrFileName, "wb")) == NULL) 
	{
		char buf[250];
		sprintf(buf, "JpegFile :\nCan't open %s\n", lpstrFileName);
		m_strJPEGError = buf;
		return FALSE;
	}

	jpeg_stdio_dest(&cinfo, outfile);

	/* Step 3: set parameters for compression */
												    
	/* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
	cinfo.image_width = widthPix; 	/* image widthPix and height, in pixels */
	cinfo.image_height = height;
	if (color) 
	{
		cinfo.input_components = 3;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	} 
	else 
	{
		cinfo.input_components = 1;		/* # of color components per pixel */
		cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	}

 
/* Now use the library's routine to set default compression parameters.
   * (You must set at least cinfo.in_color_space before calling this,
   * since the defaults depend on the source color space.)
   */

  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  row_stride = widthPix * 3;	/* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) 
  {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
	LPBYTE outRow;
	if (color) 
	{
		outRow = dataBuf + (cinfo.next_scanline * widthPix * 3);
	} 
	else 
	{
		outRow = tmp + (cinfo.next_scanline * widthPix);
	}

    (void) jpeg_write_scanlines(&cinfo, &outRow, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);

  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

  if (!color)
	  delete [] tmp;
  /* And we're done! */

  return TRUE;
}

//
//	stash a scanline
//

void j_putRGBScanline(BYTE *jpegline, 
					 int widthPix,
					 BYTE *outBuf,
					 int row)
{
	int offset = row * widthPix * 3;
	int count;
	for (count=0;count<widthPix;count++) {
		BYTE iRed, iBlu, iGrn;
		LPBYTE oRed, oBlu, oGrn;

		iRed = *(jpegline + count * 3 + 0);
		iGrn = *(jpegline + count * 3 + 1);
		iBlu = *(jpegline + count * 3 + 2);

		oRed = outBuf + offset + count * 3 + 0;
		oGrn = outBuf + offset + count * 3 + 1;
		oBlu = outBuf + offset + count * 3 + 2;

		*oRed = iRed;
		*oGrn = iGrn;
		*oBlu = iBlu;
	}
}

//
//	stash a gray scanline
//

void j_putGrayScanlineToRGB(BYTE *jpegline, 
							 int widthPix,
							 BYTE *outBuf,
							 int row)
{
	int offset = row * widthPix * 3;
	int count;
	for (count=0;count<widthPix;count++) 
	{
		BYTE iGray;
		LPBYTE oRed, oBlu, oGrn;

		// get our grayscale value
		iGray = *(jpegline + count);

		oRed = outBuf + offset + count * 3;
		oGrn = outBuf + offset + count * 3 + 1;
		oBlu = outBuf + offset + count * 3 + 2;

		*oRed = iGray;
		*oGrn = iGray;
		*oBlu = iGray;
	}
}
