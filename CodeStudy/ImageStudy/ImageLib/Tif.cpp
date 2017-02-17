// Tif.cpp: implementation of the CTif class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tif.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTif::CTif()
{
  m_lPageCount     = - 1;
  m_lPage          = 0; 
  m_tif            = NULL;
  height           = 0;
  width            = 0;
  bitspersample    = 0;
  samplesperpixel  = 0;
  rowsperstrip     = 0;
  photometric      = 0;
  compression      = 0;
  orientation      = 0;
  x_resolution     = 0;
  y_resolution     = 0;

  x        = 0;
  y        = 0;
  x_offset = 0;
  y_offset = 0;
  m_bIsRGB = FALSE;

  memset( &m_info, 0 , sizeof( m_info ) );
  memset( &m_head, 0 , sizeof( m_head ) );
  m_pbSelection = NULL;
  m_pbAlpha     = NULL;
  m_pDib        = NULL;

  m_pDibObject = NULL;
}

CTif::CTif(CDib *pDib)
{
	m_pDibObject = NULL;
	SetDib(pDib);
}

CTif::~CTif()
{
	if (m_pDibObject != NULL)
		delete m_pDibObject;
}

BOOL CTif::Load(LPCTSTR szFileName)
{
	if (! open((char *)szFileName, "rb"))
		return FALSE;

	if (! DecompressImage())
		return FALSE;

    // Allocate enough memory for the new CF_DIB, and copy bits 
	DWORD dwHeaderSize = sizeof(BITMAPINFOHEADER);
	DWORD dwBitsSize = m_head.biSizeImage + (m_head.biClrUsed * sizeof(RGBQUAD));
    HDIB hDIB = GlobalAlloc(GHND, dwHeaderSize + dwBitsSize); 
	if (hDIB == NULL)
		return FALSE;

    LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB); 
	LPBITMAPINFOHEADER  lpbi;
	lpbi  = (LPBITMAPINFOHEADER)(lpDIB);
	*lpbi = m_head;
	lpbi->biSize = sizeof(BITMAPINFOHEADER);
	LPBYTE lpBits = lpDIB + dwHeaderSize;
	memcpy(lpBits, m_info.pImage, dwBitsSize); 

	if (m_pDibObject != NULL)
		delete m_pDibObject;

	m_pDibObject = new CDib();
	m_pDibObject->Attach(hDIB);

	close();

	return TRUE;
}

// Save TIFF file
BOOL CTif::Save(LPCTSTR lpstrFileName, CDib* pDib)
{
	if (pDib == NULL)
		pDib = m_pDibObject;
	if (pDib == NULL)
		return FALSE;

	HDIB hDib = CopyHandle(pDib->GetHandle());
	if (hDib == NULL)
		return FALSE;

    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
    if (!lpBI) 
	{
		GlobalUnlock(hDib);
        return FALSE; 
	}
	UINT32 total_width = lpBI->biWidth;
	GlobalUnlock(hDib);

	CDib* pDibTmp = new CDib;
	pDibTmp->Attach(hDib);
	// convert to 24-bit image
	if (pDibTmp->GetBitCount() != 24)
		pDibTmp->ConvertFormat(24);

	UINT w  = pDibTmp->GetWidth();
	UINT h = pDibTmp->GetHeight();
	UINT32 bitcount = pDibTmp->GetBitCount();
	UINT32 bytecount = bitcount / 8;

	TIFF * tif;
	if ((tif = TIFFOpen(lpstrFileName, "w")) == NULL)
		return FALSE;

    // setup the image tags
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_PACKBITS);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, RESUNIT_NONE);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

	unsigned char * psrc = (unsigned char *)pDib->GetBitsPtr();
	unsigned char * pdst = new unsigned char[(w * 3)];

	UINT32 src_index;
	UINT32 dst_index;

	  // now go line by line to write out the image data
	for (UINT row = 0; row < h; row++ )
	{
		  // initialize the scan line to zero
		memset(pdst,0,(size_t)(w * 3));

		  // moving the data from the dib to a row structure that can
		  // be used by the tiff library
		for (UINT col = 0; col < w; col++)
		{
			src_index = (h - row - 1) * total_width * bytecount + col * bytecount;
			dst_index = col * 3;
			pdst[dst_index++] = psrc[src_index+2];
			pdst[dst_index++] = psrc[src_index+1];
			pdst[dst_index] = psrc[src_index];
		}

		  // now actually write the row data
		TIFFWriteScanline(tif, pdst, row, 0);
	}

	TIFFClose(tif);

	return TRUE;
}

/****************************************************************************/
/*                                                                          */
/*   CTif::getdata                                                         */
/*                                                                          */
/****************************************************************************/
void CTif::close()
{   
  try {
    TIFFClose(m_tif); 

    if (m_pbSelection)
      delete[] m_pbSelection;

    if (m_pbAlpha)
      delete[] m_pbAlpha;

    if (m_pDib)
      delete[] m_pDib;
  } catch(...)
  {
  }
};


/****************************************************************************/
/*                                                                          */
/*   CTif::getdata                                                         */
/*                                                                          */
/****************************************************************************/
void CTif::getdata()
{

  rowsperstrip = 0;

  height = 0;
  width = 0;
  bitspersample = 0;
  samplesperpixel = 0;
  photometric = compression  = orientation = 0;
  x = y = 0;
  x_resolution = y_resolution = x_offset = y_offset = 0;
  m_bIsRGB = FALSE;

  TIFFGetField(m_tif, TIFFTAG_COMPRESSION, &compression);
  TIFFGetField(m_tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(m_tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(m_tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
  TIFFGetField(m_tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  TIFFGetField(m_tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);   
  TIFFGetField(m_tif, TIFFTAG_PHOTOMETRIC, &photometric);
  TIFFGetField(m_tif, TIFFTAG_ORIENTATION, &orientation);
  TIFFGetField(m_tif, TIFFTAG_XRESOLUTION, &x_resolution);
  TIFFGetField(m_tif, TIFFTAG_YRESOLUTION, &y_resolution);
  TIFFGetField(m_tif, TIFFTAG_XPOSITION, &x_offset);
  TIFFGetField(m_tif, TIFFTAG_XPOSITION, &y_offset);

  m_bIsRGB = (bitspersample >= 8) 
                      &&
             (photometric == PHOTOMETRIC_RGB) 
                      ||
             (photometric == PHOTOMETRIC_YCBCR) 
                      ||
             (photometric == PHOTOMETRIC_SEPARATED) 
                      ||
             (photometric == PHOTOMETRIC_LOGLUV);

  //
  //  default to 200 dpi if no res
  //
  if (!x_resolution)
    x_resolution = 200;

  if (!y_resolution)
    y_resolution = 200;


}

/****************************************************************************/
/*                                                                          */
/*   CTif::pagecount                                                       */
/*                                                                          */
/****************************************************************************/
long CTif::pagecount()
{
  return m_lPageCount;
}


/****************************************************************************/
/*                                                                          */
/*  CTif::isFax                                                            */
/*                                                                          */
/****************************************************************************/
int CTif::isFax()
{
  return (compression == COMPRESSION_CCITTFAX3 ||
      compression == COMPRESSION_CCITTFAX4 );
}

/****************************************************************************/
/*                                                                          */
/*  CTif::open                                                             */
/*                                                                          */
/****************************************************************************/
BOOL CTif::open( char* szTiff, char * szMode ) 
{
  char szM[ 10 ];

  if (szMode) {
    memset( szM, 0 , sizeof(char) * 10 );
    memcpy( szM, szMode, lstrlen( szMode ) );
  } else {
    szM[ 0 ] = 'r';
    szM[ 1 ] = 'b';
  }

  m_tif = TIFFOpen(szTiff, szM);

  if (m_lPageCount == -1) {
    m_lPageCount = 0;
    while(TIFFSetDirectory(m_tif,(uint16)m_lPageCount))
      m_lPageCount++;
  }

  if (!TIFFSetDirectory(m_tif, 0))
    return false;

  getdata();

  return true;
}


BOOL CTif::DecompressImage()
{
  try {

    BYTE * bits;   //pointer to source data
    BYTE * bits2;  //pointer to destination data

    //
    //check if it's a tiff file
    //
    if (!m_tif)
      return FALSE;

    //
    //set image info
    //
    m_info.xDPI    = (long)x_resolution;
    m_info.yDPI    = (long)y_resolution;
    m_info.xOffset = (long)x_offset;
    m_info.yOffset = (long)y_offset;

    m_head.biWidth   = width;
    m_head.biHeight  = height;
    m_head.biClrUsed = 0;

    if (m_bIsRGB){
      m_head.biBitCount = 24;
    } else {
      if ( (photometric == PHOTOMETRIC_MINISBLACK)
                        ||
           (photometric==PHOTOMETRIC_MINISWHITE) )
      {
        if (bitspersample == 1) {
          //
          //  Black & White Image
          //
          m_head.biBitCount = 1;    
          m_head.biClrUsed  = 2;
        } else if (bitspersample == 4) {
          //
          // 16 colors gray scale
          //
          m_head.biBitCount = 4;    
          m_head.biClrUsed  = 16;
        } else {
          //
          // gray scale
          //
          m_head.biBitCount = 8;    
          m_head.biClrUsed  = 256;
        }
      } else if (bitspersample == 4) {
        //
        // 16 colors
        //
        m_head.biBitCount = 4;
        m_head.biClrUsed  = 16;
      } else {
        //
        // 256 colors
        //
        m_head.biBitCount = 8;      
        m_head.biClrUsed  = 256;
      }
    }

    //
    //  Create  the dib and bitmap.
    //
    Create( m_head.biWidth, m_head.biHeight, m_head.biBitCount );

    if (m_bIsRGB) {
      //
      // Read the whole image into one big RGBA buffer using
      // the traditional TIFFReadRGBAImage() API that we trust.
      //
      uint32 * raster;    // retrieve RGBA image
      uint32 * row;

      raster = (uint32*)_TIFFmalloc(width * height * sizeof (uint32));
      if (raster == NULL)
        return FALSE;

      //
      // Read the image in one chunk into an RGBA array
      //
      if(!TIFFReadRGBAImage( m_tif, width, height, raster, 1)) {
        _TIFFfree( raster );
        return FALSE;
      }

      //
      // read the raster lines and save them in the DIB
      // with RGB mode, we have to change the order of the 3 samples RGB
      //
      row = &raster[0];
      bits2 = m_info.pImage;
      for (y = 0; y < height; y++) {
        bits = bits2;
        for (x = 0; x < width; x++) {
          *bits++ = (BYTE)TIFFGetB( row[x] );
          *bits++ = (BYTE)TIFFGetG( row[x] );
          *bits++ = (BYTE)TIFFGetR( row[x] );
        }
        row   += width;
        bits2 += m_info.dwEffWidth;
      }
      _TIFFfree( raster );
    } else {
      RGBQUAD * pal;
      pal = (RGBQUAD*)calloc(256,sizeof(RGBQUAD));
      if (pal==NULL) 
        return FALSE;

      //
      // set up the colormap based on photometric  
      //
      switch (photometric) {
        //
        // bitmap and greyscale image types
        //
        case PHOTOMETRIC_MINISBLACK:  
        case PHOTOMETRIC_MINISWHITE:
          //
          // Monochrome image
          //
          if (bitspersample == 1) {  
            if (photometric == PHOTOMETRIC_MINISBLACK) {
              pal[ 1 ].rgbRed = pal[ 1 ].rgbGreen = pal[ 1 ].rgbBlue = 255;
            } else {
              pal[ 0 ].rgbRed = pal[ 0 ].rgbGreen = pal[ 0 ].rgbBlue = 255;
            }
          } else {    
            //
            // need to build the scale for greyscale images
            //
            if (photometric == PHOTOMETRIC_MINISBLACK) {
              for (UINT i = 0; i < m_head.biClrUsed; i++) {
                pal[ i ].rgbRed = pal[ i ].rgbGreen = pal[ i ].rgbBlue = (BYTE)(i * (255 / (m_head.biClrUsed - 1 )));
              }
            } else {
              for (UINT i = 0; i < m_head.biClrUsed; i++) {
                pal[ i ].rgbRed = pal[ i ].rgbGreen = pal[ i ].rgbBlue = (BYTE)(255 - i * (255 / (m_head.biClrUsed - 1)));
              }
            }
          }
          break;
        case PHOTOMETRIC_PALETTE:  // color map indexed
          uint16 * red;
          uint16 * green;
          uint16 * blue;
          TIFFGetField( m_tif, TIFFTAG_COLORMAP, &red, &green, &blue ); 

          //
          // Is the palette 16 or 8 bits ?
          //
          BOOL Palette16Bits = FALSE;
          int n = 1 << bitspersample;
          while (n-- > 0) {
            if (red[ n ] >= 256 || green[ n ] >= 256 || blue[n] >= 256) {
              Palette16Bits=TRUE;
              break;
            }
          }

          //
          // load the palette in the DIB
          //
          for (int i = (1 << bitspersample) - 1; i >= 0; i--) {
            if (Palette16Bits) {
              pal[ i ].rgbRed   = (BYTE)CVT(red[ i ]);
              pal[ i ].rgbGreen = (BYTE)CVT(green[ i ]);
              pal[ i ].rgbBlue  = (BYTE)CVT(blue[ i ]);           
            } else {
              pal[ i ].rgbRed   = (BYTE)red[ i ];
              pal[ i ].rgbGreen = (BYTE)green[ i ];
              pal[ i ].rgbBlue  = (BYTE)blue[ i ];        
            }
          }
          break;
      }

      //
      // Set the pallet up.
      //
	    if ( (pal != NULL)
                 &&
           (m_pDib != NULL)
                 &&
           (m_head.biClrUsed != 0))
      {
	      memcpy( (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER)),pal,min( (m_head.biClrUsed * sizeof(RGBQUAD)), m_head.biClrUsed * sizeof( RGBQUAD ) ) );
      } 
      free( pal );


      //
      // read the tiff lines and save them in the DIB
      //
      uint32 nrow;
      uint32 ys;
      int  line    = CalculateLine(width, bitspersample * samplesperpixel);
      long bitsize = TIFFStripSize( m_tif );

      //
      // verify bitsize: could be wrong if StripByteCounts is missing.
      //
      if ( bitsize > (long)(m_info.dwEffWidth * height) )
        bitsize = m_info.dwEffWidth * height;

      bits = (BYTE*)malloc( bitsize );

      for (ys = 0; ys < height; ys += rowsperstrip) {
        nrow = (ys + rowsperstrip > height ? height - ys : rowsperstrip);
        if (TIFFReadEncodedStrip( m_tif, TIFFComputeStrip( m_tif, ys, 0 ), bits, nrow * line ) == -1) {
          free( bits );
          return 0;
        }

        for (y = 0; y < nrow; y++)  {
          long offset = (nrow-y-1) * line;
          if (bitspersample == 16) {
            for (ULONG xi = 0; xi < width; xi++)
              bits[ xi + offset ] = bits[ xi * 2 + offset + 1 ];
          }
          memcpy( m_info.pImage + m_info.dwEffWidth * (height - ys - nrow + y),bits + offset,m_info.dwEffWidth );
        }
      }

      free( bits );
    }
  } catch(...) {
    return FALSE;
  }

  return TRUE;
}


void * CTif::Create( DWORD a_dwWidth, DWORD a_dwHeight, long a_wBpp )
{
  //
  // destroy the existing image (if any)
  //
  if (m_pDib)
    free( m_pDib );

  m_pDib = NULL;

  //
  // Make sure bits per pixel is valid
  //
  if (a_wBpp <= 1)
    a_wBpp = 1;
  else if (a_wBpp <= 4)
    a_wBpp = 4;
  else if (a_wBpp <= 8)
    a_wBpp = 8;
  else
    a_wBpp = 24;

  //
  // set the correct bpp value
  //
  switch (a_wBpp) {
    case 1:
        m_head.biClrUsed = 2;
        break;
    case 4:
        m_head.biClrUsed = 16;
        break;
    case 8:
        m_head.biClrUsed = 256;
        break;
    default:
        m_head.biClrUsed = 0;
  }

  //
  //set the common image informations
  //
  m_info.bColorType = (BYTE)((a_wBpp >8) ? COLORTYPE_COLOR : COLORTYPE_PALETTE);
  m_info.dwEffWidth = ( ( ( (a_wBpp * a_dwWidth) + 31) / 32) * 4);

  //
  // initialize BITMAPINFOHEADER
  //
  m_head.biWidth         = a_dwWidth;      // fill in width from parameter
  m_head.biHeight        = a_dwHeight;     // fill in height from parameter
  m_head.biPlanes        = 1;              // must be 1
  m_head.biBitCount      = (WORD)a_wBpp;   // from parameter
  m_head.biCompression   = BI_RGB;    
  m_head.biSizeImage     = m_info.dwEffWidth * a_dwHeight;
  m_head.biXPelsPerMeter = (long) floor(m_info.xDPI * 10000.0 / 254.0 + 0.5);
  m_head.biYPelsPerMeter = (long) floor(m_info.yDPI * 10000.0 / 254.0 + 0.5);
  m_head.biClrImportant  = 0;

  //
  // alloc memory block to store our bitmap
  //
  long lDibSize = m_head.biSize + m_head.biSizeImage + (m_head.biClrUsed * sizeof(RGBQUAD));
  m_pDib = malloc( lDibSize ); 
  if (!m_pDib)
    return NULL;
  
  //
  // clear the palette
  //
  RGBQUAD * pal = NULL;
  if ((m_pDib) && (m_head.biClrUsed))
    pal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));

  if (pal) 
    memset( pal,0,(m_head.biClrUsed * sizeof(RGBQUAD)) );

  //
  // use our bitmap info structure to fill in first part of
  // our DIB with the BITMAPINFOHEADER
  //
  LPBITMAPINFOHEADER  lpbi;
  lpbi  = (LPBITMAPINFOHEADER)(m_pDib);
  *lpbi = m_head;
  m_info.pImage = ((BYTE*)m_pDib + *(LPDWORD)m_pDib + (m_head.biClrUsed * sizeof(RGBQUAD)));


  return m_pDib; //return handle to the DIB
}

void CTif::SetPaletteIndex(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha)
{
	if ((m_pDib) && (m_head.biClrUsed)) {
		BYTE * iDst = (BYTE*)(m_pDib) + sizeof(BITMAPINFOHEADER);
		if (idx < m_head.biClrUsed) {
			long ldx = idx * sizeof( RGBQUAD );
			iDst[ ldx++ ] = (BYTE)b;
			iDst[ ldx++ ] = (BYTE)g;
			iDst[ ldx++ ] = (BYTE)r;
			iDst[ ldx ]   = (BYTE)alpha;
		}
	}
}

int CTif::FillColorMap( QT_COLOR_MAP * colors )
{
  
  long count_colors = 1 << bitspersample;
  int i;
  switch (photometric) {
    case PHOTOMETRIC_MINISBLACK:
    {
      for ( i = 0; i < (long) count_colors; i++) {
        colors[ i ].red   = (((double) 255 * i)/max(count_colors - 1,1)+0.5);
        colors[ i ].green = colors[ i ].red;
        colors[ i ].blue  = colors[ i ].red;
      }
      break;
    }
    case PHOTOMETRIC_MINISWHITE:
    default:
    {
      for (i=0; i < (long) count_colors; i++) {
        colors[ i ].red   =(255- (((double) 255 * i) / max( count_colors - 1,1))+0.5);
        colors[ i ].green = colors[ i ].red;
        colors[ i ].blue  = colors[ i ].red;
      }
      break;
    }
    case PHOTOMETRIC_PALETTE:
    {
      long range;

      uint16 * blue_colormap, *green_colormap, *red_colormap;

      TIFFGetField(m_tif,TIFFTAG_COLORMAP,&red_colormap, &green_colormap,&blue_colormap); 
      range=256L;  /* might be old style 8-bit colormap */
      for (i=0; i < (long) count_colors; i++)
        if ((red_colormap[i] >= 256) || (green_colormap[i] >= 256) || (blue_colormap[i] >= 256)) 
        {
            range=65535L;
            break;
        }

      for (i=0; i < (long) count_colors; i++) {
        colors[ i ].red   = (((double) 255 * red_colormap[ i ])   / range+0.5);
        colors[ i ].green = (((double) 255 * green_colormap[ i ]) / range+0.5);
        colors[ i ].blue  = (((double) 255 * blue_colormap[ i ])  / range+0.5);
      }
      break;
    }
  }

  return 1;
}
