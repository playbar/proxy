#include "Surface.h"

Surface::Surface()
{
	m_pkSurface = NULL;
	m_ulColorKey = -1;
}

Surface::Surface(LPDIRECTDRAW7 pkDD, HINSTANCE hInst, const char *pcFileName, int iWidth, int iHeight, unsigned long ulColorKey)
{
	m_pkSurface = NULL;
	m_ulColorKey = -1;
	
	Create(pkDD, iWidth, iHeight, ulColorKey);
	LoadBitmap(hInst, pcFileName);
}

Surface::~Surface()
{
	if(m_pkSurface)
	{
		m_pkSurface->Release();
		m_pkSurface = NULL;
	}
}

void Surface::Destroy()
{
	if(m_pkSurface != NULL)
	{
		m_pkSurface->Release();
		m_pkSurface = NULL;
	}
}

bool Surface::Create(LPDIRECTDRAW7 pkDD, int iWidth, int iHeight, unsigned long ulColorKey)
{
    DDSURFACEDESC2 kDDSurfDesc;
    DDCOLORKEY kDDCK;

    ZeroMemory(&kDDSurfDesc, sizeof(kDDSurfDesc));
    kDDSurfDesc.dwSize = sizeof(kDDSurfDesc);
	kDDSurfDesc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    kDDSurfDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	kDDSurfDesc.dwWidth = iWidth;
    kDDSurfDesc.dwHeight = iHeight;

    HRESULT hRet = pkDD->CreateSurface(&kDDSurfDesc, &m_pkSurface, NULL);
    if(hRet != DD_OK)
	{
		if(hRet == DDERR_OUTOFVIDEOMEMORY)
		{
			kDDSurfDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			hRet = pkDD->CreateSurface(&kDDSurfDesc, &m_pkSurface, NULL );
		}

		if(hRet != DD_OK)
			return false;
	}

	if((int)ulColorKey != -1)
	{
		kDDCK.dwColorSpaceLowValue = ulColorKey;
		kDDCK.dwColorSpaceHighValue = 0;
		m_pkSurface->SetColorKey(DDCKEY_SRCBLT, &kDDCK);
	}

	m_ulColorKey = ulColorKey;
	m_uiWidth  = iWidth;
	m_uiHeight = iHeight;

	return true;
}

bool Surface::LoadBitmap(HINSTANCE hInst, const char *pcFileName, int iX, int iY, int iWidth, int iHeight)
{
	HBITMAP	kHBitmap = (HBITMAP)LoadImage(hInst, pcFileName, IMAGE_BITMAP, iWidth, iHeight, LR_LOADFROMFILE);
	if(kHBitmap == NULL || m_pkSurface == NULL)
		return false;

	HDC hImage = CreateCompatibleDC(NULL);
	if (!hImage)
		return false;

	BITMAP kBitmap;
	SelectObject(hImage, kHBitmap);
	GetObject(kHBitmap, sizeof(kBitmap), &kBitmap);

	if(iWidth == 0)
		iWidth = kBitmap.bmWidth;

	if(iHeight == 0)
		iHeight = kBitmap.bmHeight;

	DDSURFACEDESC2 kDDSurfDesc;
	kDDSurfDesc.dwSize = sizeof(kDDSurfDesc);
	kDDSurfDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	m_pkSurface->Restore();
	m_pkSurface->GetSurfaceDesc(&kDDSurfDesc);

	HDC hDC;
	if(m_pkSurface->GetDC(&hDC) == DD_OK)
	{
		StretchBlt(hDC, 0, 0, kDDSurfDesc.dwWidth, kDDSurfDesc.dwHeight, hImage, iX, iY, iWidth, iHeight, SRCCOPY);
		m_pkSurface->ReleaseDC(hDC);
	}
	DeleteDC(hImage);

	return true;
}

bool Surface::Draw(LPDIRECTDRAWSURFACE7 pkDest, int iDestX, int iDestY, int iSrcX, int iSrcY, int iWidth, int iHeight)
{
	RECT kRect;
	HRESULT	hRet;

	if(iWidth == 0)
		iWidth = m_uiWidth;

	if(iHeight == 0)
		iHeight = m_uiHeight;

	kRect.left = iSrcX;
	kRect.top = iSrcY;
	kRect.right = iWidth  + iSrcX;
	kRect.bottom = iHeight + iSrcY;

	while(1)
	{
		if((int)m_ulColorKey < 0)
			hRet = pkDest->BltFast(iDestX, iDestY, m_pkSurface, &kRect,  DDBLTFAST_NOCOLORKEY);
		else
			hRet = pkDest->BltFast(iDestX, iDestY, m_pkSurface, &kRect,  DDBLTFAST_SRCCOLORKEY);

		if(hRet == DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
			Restore();
		else
		{
			if(hRet != DDERR_WASSTILLDRAWING)
				return false;
		}
	}

	return true;
}


/*
int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
	// this function is used to flip bottom-up .BMP images

	UCHAR *buffer; // used to perform the image processing
	int index;     // looping index

	// allocate the temporary buffer
	if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
		return(0);

	// copy image to work area
	memcpy(buffer,image,bytes_per_line*height);

	// flip vertically
	for (index=0; index < height; index++)
		memcpy(&image[((height-1) - index)*bytes_per_line],
		&buffer[index*bytes_per_line], bytes_per_line);

	// release the memory
	free(buffer);

	// return success
	return(1);

} // end Flip_Bitmap


int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename)
{
	// this function opens a bitmap file and loads the data into bitmap

	int file_handle,  // the file handle
		index;        // looping index

	UCHAR   *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
	OFSTRUCT file_data;          // the file data information

	// open the file if it exists
	if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
		return(0);

	// now load the bitmap file header
	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

	// test if this is a bitmap file
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
	{
		// close the file
		_lclose(file_handle);

		// return error
		return(0);
	} // end if

	// now we know this is a bitmap, so read in all the sections

	// first the bitmap infoheader

	// now load the bitmap file header
	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

	// now load the color palette if there is one
	if (bitmap->bitmapinfoheader.biBitCount == 8)
	{
		_lread(file_handle, &bitmap->palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));

		// now set all the flags in the palette correctly and fix the reversed 
		// BGR RGBQUAD data format
		for (index=0; index < MAX_COLORS_PALETTE; index++)
		{
			// reverse the red and green fields
			int temp_color                = bitmap->palette[index].peRed;
			bitmap->palette[index].peRed  = bitmap->palette[index].peBlue;
			bitmap->palette[index].peBlue = temp_color;

			// always set the flags word to this
			bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		} // end for index

	} // end if

	// finally the image data itself
	//_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);

	// now read in the image, if the image is 8 or 16 bit then simply read it
	// but if its 24 bit then read it into a temporary area and then convert
	// it to a 16 bit image

	if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16 || 
		bitmap->bitmapinfoheader.biBitCount==24)
	{
		// delete the last image if there was one
		if (bitmap->buffer)
			free(bitmap->buffer);

		// allocate the memory for the image
		if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		{
			// close the file
			_lclose(file_handle);

			// return error
			return(0);
		} // end if

		// now read it in
		_lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);

	} // end if
	else
	{
		// serious problem
		return(0);

	} // end else

#if 0
	// write the file info out 
	printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
		filename,
		bitmap->bitmapinfoheader.biSizeImage,
		bitmap->bitmapinfoheader.biWidth,
		bitmap->bitmapinfoheader.biHeight,
		bitmap->bitmapinfoheader.biBitCount,
		bitmap->bitmapinfoheader.biClrUsed,
		bitmap->bitmapinfoheader.biClrImportant);
#endif

	// close the file
	_lclose(file_handle);

	// flip the bitmap
	Flip_Bitmap(bitmap->buffer, 
		bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
		bitmap->bitmapinfoheader.biHeight);

	// return success
	return(1);

} // end Load_Bitmap_File


int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
	// this function releases all memory associated with "bitmap"
	if (bitmap->buffer)
	{
		// release memory
		free(bitmap->buffer);

		// reset pointer
		bitmap->buffer = NULL;

	} // end if

	// return success
	return(1);

} // end Unload_Bitmap_File

*/