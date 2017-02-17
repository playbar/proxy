// Tga.cpp: implementation of the CTga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTga::CTga()
{
	m_pDib = NULL;
}

CTga::CTga(CDib *pDib)
{
	m_pDib = NULL;
	SetDib(pDib);
}

CTga::~CTga()
{
	if (m_pDib != NULL)
		delete m_pDib;
}

BOOL CTga::Load(LPCTSTR lpstrFileName)
{
	CFile file;
	if (! file.Open(lpstrFileName, CFile::modeRead))
		return FALSE;

	HDIB hDIB = ReadTGAFile(file);

	file.Close();

	// create DIB
	if (m_pDib != NULL)
		delete m_pDib;

	m_pDib = new CDib();
	m_pDib->Attach(hDIB);

	return TRUE;
}

BOOL CTga::Save(LPCTSTR lpstrFileName, CDib* pDib)
{
	if (pDib == NULL)
		pDib = m_pDib;
	if (pDib == NULL)
		return FALSE;

	CDib *pTmpDib = pDib->Clone();

	if (pTmpDib->GetBitCount() != 24)
	{
		pTmpDib->ConvertFormat(24);
	}

	HDIB hDIB = pTmpDib->GetHandle();
	if (hDIB == NULL)
	{
		delete pTmpDib;
		return FALSE;
	}

	CFile file;
	if (! file.Open(lpstrFileName, CFile::modeCreate | CFile::modeWrite))
	{
		delete pTmpDib;
		return FALSE;
	}

	if (! SaveTGAFile(hDIB, file))
	{
		delete pTmpDib;
		return FALSE;
	}

	file.Close();
	delete pTmpDib;

	return TRUE;
}

/*************************************************************************
*
*  Function:  ReadTGAFile (CFile&)
*  
*  Purpose:  Reads in the specified targe file into a global chunk of
* 	  	     memory.
*          
*            Returns:  A handle to a dib (hDIB) if successful.
*            NULL if an error occurs.
*              
**************************************************************************/
          
HDIB CTga::ReadTGAFile(CFile& file)
{
  //Determine if the file is in the original TGA format
  //or the new TGA format i.e. post September 1989
  DWORD dwFileLength = file.GetLength();
  if (dwFileLength == 0)
    return NULL;
  file.Seek(dwFileLength - sizeof(TGA_FILE_FOOTER), CFile::begin);
  TGA_FILE_FOOTER fileFooter;
  file.Read(&fileFooter, sizeof(TGA_FILE_FOOTER));
  BOOL bNewTGAFormat = FALSE;
  if (strcmp(fileFooter.cSignature, "TRUEVISION-XFILE.") == 0)
    bNewTGAFormat = TRUE;

  //Read in the TGA file header
  TGA_FILE_HEADER fileHeader;                  
  file.SeekToBegin();
  file.Read(&fileHeader, sizeof(TGA_FILE_HEADER));

  //We only support 32 and 24 bit color depths
  if ((fileHeader.byPixelDepth != 32) &&
      (fileHeader.byPixelDepth != 24))
  {
    TRACE(_T("Bit depth is %d, which cannot be handled\n"), fileHeader.byPixelDepth);
    return NULL;
  }

  //now read in the image data
  if (fileHeader.byImageType != 2)
  {
    TRACE(_T("Image type is not an uncompressed true-color image, instead it is %d\n"), fileHeader.byImageType);
    return NULL;
  }

  //check that the Color map type is 0
  if (fileHeader.byColorMapType != 0)
  {
    TRACE(_T("Color map type is non zero in the TGA file header for this uncompressed true-color tga file, instead it is %d\n"), fileHeader.byColorMapType);
    return NULL;
  }      

  //Determine the end offset of the "Image Data"
  DWORD dwEndImageData = 0;
  if (bNewTGAFormat)
    dwEndImageData = fileFooter.lExtensionAreaOffset - 1;
  else
    dwEndImageData = dwFileLength;

  //Determine the start offset of the "Image Data" and the pointer to 
  //the color map
  DWORD dwStartImageData = sizeof(TGA_FILE_HEADER) + fileHeader.byIdLength;

  //Read in the image data
  DWORD dwImageSize = dwEndImageData - dwStartImageData;
  BYTE* pImageData = new BYTE[dwImageSize];
  file.Seek(dwStartImageData, CFile::begin);
  file.Read(pImageData, dwImageSize);

  // initializing incapsulated image with correct values
  BITMAPINFO bmi;
  BITMAPINFOHEADER& bih = bmi.bmiHeader;
  ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = fileHeader.nImageWidth;
  bih.biHeight = fileHeader.nImageHeight;
  bih.biCompression = BI_RGB;
  bih.biPlanes = 1;
  bih.biBitCount = 24;

  //Allocate a buffer to use for the transfer to DIB format
  DWORD imageSize = fileHeader.nImageHeight*(fileHeader.nImageWidth*3+((4-((fileHeader.nImageWidth*3)&3))&3));
  BYTE* pBmp = new BYTE[imageSize];

  //Determine the up/down and left/right ordering of the pixels
  BOOL bTopToBottom = (((fileHeader.byImageDescriptor >> 4) & 1) != 0);
  BOOL bLeftToRight = (((fileHeader.byImageDescriptor >> 5) & 1) != 0);

  //Copy the DIB bits from the user buffer into the DIB
  int nInBytesPerPel = fileHeader.byPixelDepth / 8;

  for (int j=0; j<fileHeader.nImageHeight; j++)
  {
    int nDepthInOffset = 0;
    if (bTopToBottom)
      nDepthInOffset = (fileHeader.nImageHeight - j - 1)*fileHeader.nImageWidth*nInBytesPerPel;
    else
      nDepthInOffset = j*fileHeader.nImageWidth*nInBytesPerPel;

    int nDepthOutOffset = j*WIDTHBYTES(fileHeader.nImageWidth*24);

    for (int i=0; i<fileHeader.nImageWidth; i++)
    {
      int nInOffset = 0;
      if (bTopToBottom)
      {
        if (bLeftToRight)
          nInOffset = nDepthInOffset + (fileHeader.nImageWidth - i - 1)*nInBytesPerPel;
        else
          nInOffset = nDepthInOffset + i*nInBytesPerPel;
      }
      else
      {
        if (bLeftToRight)
          nInOffset = nDepthInOffset + (fileHeader.nImageWidth - i - 1)*nInBytesPerPel;
        else
          nInOffset = nDepthInOffset + i*nInBytesPerPel;
      }

      int nOutOffset = nDepthOutOffset + i*3;

      if (nInBytesPerPel == 3 || nInBytesPerPel == 4)
      {
        pBmp[nOutOffset]   = pImageData[nInOffset];
        pBmp[nOutOffset+1] = pImageData[nInOffset+1];
        pBmp[nOutOffset+2]  = pImageData[nInOffset+2];
      }
      else
      {
        ASSERT(FALSE);
        break;
      }
    }
  }

  //Delete the memory used to load the targe prior to transfering to the DIB
	delete [] pImageData;

  // Allocate memory for DIB
  HDIB hDIB = (HDIB) ::GlobalAlloc(GHND, bih.biSize + imageSize);
  if (hDIB == 0)
  {
	  delete [] pBmp;
    return NULL;
  }
  LPSTR pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
  if (pDIB == 0)
  {
	  delete [] pBmp;
    return NULL;
  }

  CopyMemory(pDIB, &bmi.bmiHeader, bih.biSize);
  CopyMemory(pDIB + bih.biSize, pBmp, imageSize);
  delete [] pBmp;

  ::GlobalUnlock((HGLOBAL) hDIB);
  return hDIB;
}

/*************************************************************************
*
* SaveTGAFile()
*
* Saves the specified DIB as a Targa into the specified CFile.  The CFile
* is opened and closed by the caller.
*
* Parameters:
*
* HDIB hDib - Handle to the dib to save
*
* CFile& file - open CFile used to save as Targa
*
* Return value: TRUE if successful, else FALSE or CFileException
*
*************************************************************************/

BOOL CTga::SaveTGAFile(HDIB hDib, CFile& file)
{
  BOOL bSuccess = TRUE;

  //No bitmap, then cannot save
  if (hDib == NULL)
    return FALSE;

  // Get a pointer to the DIB memory, the first of which contains
  // a BITMAPINFO structure
  LPBITMAPINFOHEADER lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
  if (lpBI == NULL)
    return FALSE;
   
  if (!IS_WIN30_DIB(lpBI))
  {
    TRACE(_T("Only Windows 3.0 DIB images can be saved as Targa!\n"));
    ::GlobalUnlock((HGLOBAL) hDib);
    return FALSE;  // It's an other-style DIB (save not supported)
  }

  if (lpBI->biBitCount != 24)
  {
    TRACE(_T("Only 16 million colors (24 bit) images can be saved as Targa!\n"));
    ::GlobalUnlock((HGLOBAL) hDib);
    return FALSE;
  }

  if (lpBI->biCompression != BI_RGB)
  {
    TRACE(_T("RLE-compressed images can't be saved as Targa!\n"));
    ::GlobalUnlock((HGLOBAL) hDib);
    return FALSE;
  }

  DWORD dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);  // Partial Calculation

  // Now calculate the size of the image

  DWORD dwBmBitsSize;  // Size of Bitmap Bits only
  // It's not RLE, so size is Width (DWORD aligned) * Height
  dwBmBitsSize = WIDTHBYTES(lpBI->biWidth*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
  dwDIBSize += dwBmBitsSize;

  // Now, since we have calculated the correct size, why don't we
  // fill in the biSizeImage field (this will fix any .BMP files which
  // have this field incorrect).
  lpBI->biSizeImage = dwBmBitsSize;

  TRY
  {
    //First write out the header
    TGA_FILE_HEADER fileHeader;                  
    ZeroMemory(&fileHeader, sizeof(TGA_FILE_HEADER));
    fileHeader.byImageType = 2;
    fileHeader.nImageWidth = (short) lpBI->biWidth;
    fileHeader.nImageHeight = (short) lpBI->biHeight;
    fileHeader.byPixelDepth = 24;
    file.SeekToBegin();
    file.Write(&fileHeader, sizeof(TGA_FILE_HEADER));

    //Allocate some memory to save the Dib bits into 
    BYTE* pImageData = (BYTE*)lpBI + lpBI->biSize;
    DWORD dwImageDataSize = lpBI->biWidth*lpBI->biHeight*3;
    BYTE* pBmp = new BYTE[dwImageDataSize];

    //Copy the Dib bits into the buffer we just allocated
    for (int j=0; j<lpBI->biHeight; j++)
    {
      int nDepthOffset = j*WIDTHBYTES(lpBI->biWidth*24);
      for (int i=0; i<lpBI->biWidth; i++)
      {
        int nOutputOffset = j*lpBI->biWidth*3 + i*3;
        int nInputOffset = nDepthOffset + i*3;
	      pBmp[nOutputOffset]	    = pImageData[nInputOffset];
	      pBmp[nOutputOffset+1]	= pImageData[nInputOffset+1];
	      pBmp[nOutputOffset+2]	= pImageData[nInputOffset+2];
      }
    }

    ::GlobalUnlock((HGLOBAL)hDib);

    //Write the image data to disk
    file.Write(pBmp, dwImageDataSize);

    //Free up the memory we used
    delete [] pBmp;
  }
  CATCH_ALL(e)
  {
    bSuccess = FALSE;
    TRACE(_T("Error storing TGA file!\n"));
  }
  END_CATCH_ALL

  return bSuccess;
}



