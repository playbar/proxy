//********************************************************************** 
// 
//  AIBAPI.CPP
// 
//  Source file for Device-Independent Bitmap (DIB) API. 
// 
//********************************************************************** 
 
#define     STRICT      // enable strict type checking 
 
#include "stdafx.h"
#include <string.h> 
#include <stdio.h> 
#include <math.h> 
#include <io.h> 
#include <direct.h> 
#include <stdlib.h> 
#include <assert.h> 
#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h> 
#include "dibapi.h" 

 
/************************************************************************* 
 * 
 * CreateDIB() 
 * 
 * Parameters: 
 * 
 * DWORD dwWidth    - Width for new bitmap, in pixels 
 * DWORD dwHeight   - Height for new bitmap  
 * WORD  wBitCount  - Bit Count for new DIB (1, 4, 8, or 24) 
 * 
 * Return Value: 
 * 
 * HDIB             - Handle to new DIB 
 * 
 * Description: 
 * 
 * This function allocates memory for and initializes a new DIB by 
 * filling in the BITMAPINFOHEADER, allocating memory for the color 
 * table, and allocating memory for the bitmap bits.  As with all 
 * HDIBs, the header, colortable and bits are all in one contiguous 
 * memory block.  This function is similar to the CreateBitmap()  
 * Windows API. 
 * 
 * The colortable and bitmap bits are left uninitialized (zeroed) in the 
 * returned HDIB. 
 * 
 * 
 ************************************************************************/ 
HDIB CreateDIB(DWORD dwWidth, DWORD dwHeight, WORD wBitCount) 
{ 
    BITMAPINFOHEADER    bi;             // bitmap header 
    LPBITMAPINFOHEADER  lpbi;           // pointer to BITMAPINFOHEADER 
    DWORD               dwLen;          // size of memory block 
    HDIB                hDIB; 
    DWORD               dwBytesPerLine; // Number of bytes per scanline 
 
 
    // Make sure bits per pixel is valid 
 
    if (wBitCount <= 1) 
        wBitCount = 1; 
    else if (wBitCount <= 4) 
        wBitCount = 4; 
    else if (wBitCount <= 8) 
        wBitCount = 8; 
    else if (wBitCount <= 24) 
        wBitCount = 24; 
    else 
        wBitCount = 4;  // set default value to 4 if parameter is bogus 
 
    // initialize BITMAPINFOHEADER 
 
    bi.biSize = sizeof(BITMAPINFOHEADER); 
    bi.biWidth = dwWidth;         // fill in width from parameter 
    bi.biHeight = dwHeight;       // fill in height from parameter 
    bi.biPlanes = 1;              // must be 1 
    bi.biBitCount = wBitCount;    // from parameter 
    bi.biCompression = BI_RGB;     
    bi.biSizeImage = 0;           // 0's here mean "default" 
    bi.biXPelsPerMeter = 0; 
    bi.biYPelsPerMeter = 0; 
    bi.biClrUsed = 0; 
    bi.biClrImportant = 0; 
 
    // calculate size of memory block required to store the DIB.  This 
    // block should be big enough to hold the BITMAPINFOHEADER, the color 
    // table, and the bits 
 
    dwBytesPerLine = WIDTHBYTES(wBitCount * dwWidth); 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + (dwBytesPerLine * dwHeight); 
 
    // alloc memory block to store our bitmap 
 
    hDIB = GlobalAlloc(GHND, dwLen); 
 
    // major bummer if we couldn't get memory block 
 
    if (!hDIB) 
        return NULL; 
 
    // lock memory and get pointer to it 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    // use our bitmap info structure to fill in first part of 
    // our DIB with the BITMAPINFOHEADER 
 
    *lpbi = bi; 
 
    // Since we don't know what the colortable and bits should contain, 
    // just leave these blank.  Unlock the DIB and return the HDIB. 
 
    GlobalUnlock(hDIB); 
 
    //return handle to the DIB 
 
    return hDIB; 
} 
 
/************************************************************************* 
 * 
 * CreateDefaultDIB() 
 * 
 * Parameters: 
 * 
 * DWORD dwWidth    - Width for new bitmap, in pixels 
 * DWORD dwHeight   - Height for new bitmap  
  * 
 * Return Value: 
 * 
 * HDIB             - Handle to new DIB 
 * 
 * Description: 
 * 
 * This function allocates memory for and initializes a new DIB by 
 * filling in the BITMAPINFOHEADER, allocating memory for the color 
 * table, and allocating memory for the bitmap bits.  As with all 
 * HDIBs, the header, colortable and bits are all in one contiguous 
 * memory block.  This function is similar to the CreateBitmap()  
 * Windows API. 
 * 
 * The colortable is initialized with system palette, but bitmap bits
 * are left uninitialized (zeroed) in the returned HDIB. 
 * 
 * 
 ************************************************************************/ 
HDIB CreateDefaultDIB(DWORD dwWidth, DWORD dwHeight) 
{ 
	// Get DC
    HDC hDC = GetDC(NULL); 
    if (!hDC) 
        return NULL; 

	// DC bts/pixel
	int nDeviceBitsPixel = GetDeviceCaps(hDC, BITSPIXEL);

	// create DIB according to DC
	HDIB hDIB = CreateDIB(dwWidth, dwHeight, nDeviceBitsPixel);

	// DIB buffer
	LPBITMAPINFO lpbmi = (LPBITMAPINFO)GlobalLock(hDIB);
    LPBYTE lpDIBBits = FindDIBBits((LPBYTE)lpbmi); 
    DWORD dwBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 

	// set DIB color to White
	for (DWORD l=0; l<dwBitsSize; ++l)
		lpDIBBits[l] = 0xff;

	// if no palette, return DIB handle
	if (nDeviceBitsPixel > 8)
	{
		GlobalUnlock(hDIB);
	    ReleaseDC(NULL, hDC); 
		return hDIB;
	}

	// if there is palette, set system palette to DIB

	// colors in system palette
    int nColors = PalEntriesOnDevice(hDC);   // Number of palette entries 
  
    // Copy the current system palette into our logical palette 
	PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hDC, 0, nColors, pe); 

	// set color table
	for (int i=0; i<nColors; ++i)
	{
		lpbmi->bmiColors[i].rgbRed = pe[i].peRed;
		lpbmi->bmiColors[i].rgbGreen = pe[i].peGreen;
		lpbmi->bmiColors[i].rgbBlue = pe[i].peBlue;
		lpbmi->bmiColors[i].rgbReserved = 0;
	}

    // clean up 
	GlobalUnlock(hDIB);
    ReleaseDC(NULL, hDC); 

	return hDIB;
}
/************************************************************************* 
 * 
 * DestroyDIB () 
 * 
 * Purpose:  Frees memory associated with a DIB 
 * 
 * Returns:  none 
 * 
 *************************************************************************/ 
void DestroyDIB(HDIB hDib) 
{ 
    GlobalFree(hDib); 
} 
 
/************************************************************************* 
 * 
 * Function:  ReadDIBFile (int) 
 * 
 *  Purpose:  Reads in the specified DIB file into a global chunk of 
 *            memory. 
 * 
 *  Returns:  A handle to a dib (hDIB) if successful. 
 *            NULL if an error occurs. 
 * 
 * Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything 
 *            from the end of the BITMAPFILEHEADER structure on is 
 *            returned in the global memory handle. 
 * 
 * 
 * NOTE: The DIB API were not written to handle OS/2 DIBs, so this 
 * function will reject any file that is not a Windows DIB. 
 * 
 *************************************************************************/ 
HANDLE ReadDIBFile(HANDLE hFile) 
{ 
    BITMAPFILEHEADER    bmfHeader; 
    DWORD               dwBitsSize; 
    UINT                nNumColors;   // Number of colors in table 
    HANDLE              hDIB;         
    HANDLE              hDIBtmp;      // Used for GlobalRealloc() //MPB 
    LPBITMAPINFOHEADER  lpbi; 
    DWORD               offBits; 
    DWORD               dwRead; 
 
    // get length of DIB in bytes for use when reading 
 
    dwBitsSize = GetFileSize(hFile, NULL); 
 
    // Allocate memory for header & color table. We'll enlarge this 
    // memory as needed. 
 
    hDIB = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 
            256 * sizeof(RGBQUAD))); 
 
    if (!hDIB) 
        return NULL; 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    if (!lpbi)  
    { 
        GlobalFree(hDIB); 
        return NULL; 
    } 
 
    // read the BITMAPFILEHEADER from our file 
 
    if (!ReadFile(hFile, (LPBYTE)&bmfHeader, sizeof (BITMAPFILEHEADER), 
            &dwRead, NULL)) 
        goto ErrExit; 
 
    if (sizeof (BITMAPFILEHEADER) != dwRead) 
        goto ErrExit; 
 
    if (bmfHeader.bfType != 0x4d42)  // 'BM' 
        goto ErrExit; 
 
    // read the BITMAPINFOHEADER 
 
    if (!ReadFile(hFile, (LPBYTE)lpbi, sizeof(BITMAPINFOHEADER), &dwRead, 
            NULL)) 
        goto ErrExit; 
 
    if (sizeof(BITMAPINFOHEADER) != dwRead) 
        goto ErrExit; 
 
    // Check to see that it's a Windows DIB -- an OS/2 DIB would cause 
    // strange problems with the rest of the DIB API since the fields 
    // in the header are different and the color table entries are 
    // smaller. 
    // 
    // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL. 
 
    if (lpbi->biSize == sizeof(BITMAPCOREHEADER)) 
        goto ErrExit; 
 
    // Now determine the size of the color table and read it.  Since the 
    // bitmap bits are offset in the file by bfOffBits, we need to do some 
    // special processing here to make sure the bits directly follow 
    // the color table (because that's the format we are susposed to pass 
    // back) 
 
    if (!(nNumColors = (UINT)lpbi->biClrUsed)) 
    { 
        // no color table for 24-bit, default size otherwise 
 
        if (lpbi->biBitCount != 24) 
            nNumColors = 1 << lpbi->biBitCount; // standard size table 
    } 
 
    // fill in some default values if they are zero 
 
    if (lpbi->biClrUsed == 0) 
        lpbi->biClrUsed = nNumColors; 
 
    if (lpbi->biSizeImage == 0) 
    { 
        lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 
                31) & ~31) >> 3) * lpbi->biHeight; 
    } 
 
    // get a proper-sized buffer for header, color table and bits 
 
    GlobalUnlock(hDIB); 
    hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * 
            sizeof(RGBQUAD) + lpbi->biSizeImage, 0); 
 
    if (!hDIBtmp) // can't resize buffer for loading 
        goto ErrExitNoUnlock; //MPB 
    else 
        hDIB = hDIBtmp; 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    // read the color table 
 
    ReadFile (hFile, (LPBYTE)(lpbi) + lpbi->biSize, 
            nNumColors * sizeof(RGBQUAD), &dwRead, NULL); 
 
    // offset to the bits from start of DIB header 
 
    offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD); 
 
    // If the bfOffBits field is non-zero, then the bits might *not* be 
    // directly following the color table in the file.  Use the value in 
    // bfOffBits to seek the bits. 
 
    if (bmfHeader.bfOffBits != 0L) 
        SetFilePointer(hFile, bmfHeader.bfOffBits, NULL, FILE_BEGIN); 
 
    if (ReadFile(hFile, (LPBYTE)lpbi + offBits, lpbi->biSizeImage, &dwRead, 
            NULL)) 
        goto OKExit; 
 
ErrExit: 
    GlobalUnlock(hDIB);     
 
ErrExitNoUnlock:     
    GlobalFree(hDIB); 
    return NULL; 
 
OKExit: 
    GlobalUnlock(hDIB); 
    return hDIB; 
} 

/************************************************************************* 
 * 
 * LoadDIB() 
 * 
 * Loads the specified DIB from a file, allocates memory for it, 
 * and reads the disk file into the memory. 
 * 
 * 
 * Parameters: 
 * 
 * LPCTSTR lpFileName - specifies the file to load a DIB from 
 * 
 * Returns: A handle to a DIB, or NULL if unsuccessful. 
 * 
 * NOTE: The DIB API were not written to handle OS/2 DIBs; This 
 * function will reject any file that is not a Windows DIB. 
 * 
 *************************************************************************/ 
HDIB LoadDIB(LPCTSTR lpFileName) 
{ 
    HDIB        hDIB; 
    HANDLE      hFile; 
 
    // Set the cursor to a hourglass, in case the loading operation 
    // takes more than a sec, the user will know what's going on. 
 
    SetCursor(LoadCursor(NULL, IDC_WAIT)); 
 
    if ((hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
            NULL)) != INVALID_HANDLE_VALUE) 
    { 
        hDIB = ReadDIBFile(hFile); 
        CloseHandle(hFile); 
        SetCursor(LoadCursor(NULL, IDC_ARROW)); 
        return hDIB; 
    } 
    else 
    { 
        SetCursor(LoadCursor(NULL, IDC_ARROW)); 
        return NULL; 
    } 
} 
 
 
/************************************************************************* 
 * 
 * SaveDIB() 
 * 
 * Saves the specified DIB into the specified file name on disk.  No 
 * error checking is done, so if the file already exists, it will be 
 * written over. 
 * 
 * Parameters: 
 * 
 * HDIB hDib - Handle to the dib to save 
 * 
 * LPCTSTR lpFileName - pointer to full pathname to save DIB under 
 * 
 * Return value: TRUE if successful
 *				 FALSE if failure
 * 
 *************************************************************************/ 
BOOL SaveDIB(HDIB hDib, LPCTSTR lpFileName) 
{ 
    BITMAPFILEHEADER    bmfHdr;     // Header for Bitmap file 
    LPBITMAPINFOHEADER  lpBI;       // Pointer to DIB info structure 
    HANDLE              fh;         // file handle for opened file 
    DWORD               dwDIBSize; 
    DWORD               dwWritten; 
 
    if (!hDib) 
        return FALSE; 
 
    fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
 
    if (fh == INVALID_HANDLE_VALUE) 
        return FALSE; 
 
    // Get a pointer to the DIB memory, the first of which contains 
    // a BITMAPINFO structure 
 
    lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
    if (!lpBI) 
    { 
        CloseHandle(fh); 
        return FALSE; 
    } 
 
    // Check to see if we're dealing with an OS/2 DIB.  If so, don't 
    // save it because our functions aren't written to deal with these 
    // DIBs. 
 
    if (lpBI->biSize != sizeof(BITMAPINFOHEADER)) 
    { 
        GlobalUnlock(hDib); 
        CloseHandle(fh); 
        return FALSE; 
    } 
 
    // Fill in the fields of the file header 
 
    // Fill in file type (first 2 bytes must be "BM" for a bitmap) 
 
    bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM" 
 
    // Calculating the size of the DIB is a bit tricky (if we want to 
    // do it right).  The easiest way to do this is to call GlobalSize() 
    // on our global handle, but since the size of our global memory may have 
    // been padded a few bytes, we may end up writing out a few too 
    // many bytes to the file (which may cause problems with some apps, 
    // like HC 3.0). 
    // 
    // So, instead let's calculate the size manually. 
    // 
    // To do this, find size of header plus size of color table.  Since the 
    // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains 
    // the size of the structure, let's use this. 
 
    // Partial Calculation 
 
    dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);   
 
    // Now calculate the size of the image 
 
    // It's an RLE bitmap, we can't calculate size, so trust the biSizeImage 
    // field 
 
    if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
        dwDIBSize += lpBI->biSizeImage; 
    else 
    { 
        DWORD dwBmBitsSize;  // Size of Bitmap Bits only 
 
        // It's not RLE, so size is Width (DWORD aligned) * Height 
 
        dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * 
                lpBI->biHeight; 
 
        dwDIBSize += dwBmBitsSize; 
 
        // Now, since we have calculated the correct size, why don't we 
        // fill in the biSizeImage field (this will fix any .BMP files which  
        // have this field incorrect). 
 
        lpBI->biSizeImage = dwBmBitsSize; 
    } 
 
 
    // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER) 
                    
    bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER); 
    bmfHdr.bfReserved1 = 0; 
    bmfHdr.bfReserved2 = 0; 
 
    // Now, calculate the offset the actual bitmap bits will be in 
    // the file -- It's the Bitmap file header plus the DIB header, 
    // plus the size of the color table. 
     
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + 
            PaletteSize((LPBYTE)lpBI); 
 
    // Write the file header 
 
    WriteFile(fh, (LPBYTE)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
 
    // Write the DIB header and the bits -- use local version of 
    // MyWrite, so we can write more than 32767 bytes of data 
     
    WriteFile(fh, (LPBYTE)lpBI, dwDIBSize, &dwWritten, NULL); 
 
    GlobalUnlock(hDib); 
    CloseHandle(fh); 
 
    if (dwWritten == 0) 
        return FALSE; // oops, something happened in the write 
    else 
        return TRUE; // Success code 
} 
 
/************************************************************************* 
 * 
 * FindDIBBits() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpDIB      - pointer to packed-DIB memory block 
 * 
 * Return Value: 
 * 
 * LPBYTE            - pointer to the DIB bits 
 * 
 * Description: 
 * 
 * This function calculates the address of the DIB's bits and returns a 
 * pointer to the DIB bits. 
 * 
 ************************************************************************/ 
LPBYTE FindDIBBits(LPBYTE lpDIB) 
{ 
   return (lpDIB + *(LPDWORD)lpDIB + PaletteSize(lpDIB)); 
} 
 
/************************************************************************* 
 * 
 * DIBWidth() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpDIB      - pointer to packed-DIB memory block 
 * 
 * Return Value: 
 * 
 * DWORD            - width of the DIB 
 * 
 * Description: 
 * 
 * This function gets the width of the DIB from the BITMAPINFOHEADER 
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER 
 * width field if it is an OS/2-style DIB. 
 * 
 ************************************************************************/ 
DWORD DIBWidth(LPBYTE lpDIB) 
{ 
    LPBITMAPINFOHEADER   lpbmi;  // pointer to a Win 3.0-style DIB 
    LPBITMAPCOREHEADER   lpbmc;  // pointer to an OS/2-style DIB 
 
    // point to the header (whether Win 3.0 and OS/2) 
 
    lpbmi = (LPBITMAPINFOHEADER)lpDIB; 
    lpbmc = (LPBITMAPCOREHEADER)lpDIB; 
 
    // return the DIB width if it is a Win 3.0 DIB 
 
    if (lpbmi->biSize == sizeof(BITMAPINFOHEADER)) 
        return lpbmi->biWidth; 
    else  // it is an OS/2 DIB, so return its width 
        return (DWORD)lpbmc->bcWidth; 
} 
 
DWORD DIBWidth(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBWidth(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 
 
/************************************************************************* 
 * 
 * DIBHeight() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpDIB      - pointer to packed-DIB memory block 
 * 
 * Return Value: 
 * 
 * DWORD            - height of the DIB 
 * 
 * Description: 
 * 
 * This function gets the height of the DIB from the BITMAPINFOHEADER 
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER 
 * height field if it is an OS/2-style DIB. 
 * 
 ************************************************************************/ 
DWORD DIBHeight(LPBYTE lpDIB) 
{ 
   LPBITMAPINFOHEADER   lpbmi;  // pointer to a Win 3.0-style DIB 
   LPBITMAPCOREHEADER   lpbmc;  // pointer to an OS/2-style DIB 
 
   // point to the header (whether OS/2 or Win 3.0 
 
   lpbmi = (LPBITMAPINFOHEADER)lpDIB; 
   lpbmc = (LPBITMAPCOREHEADER)lpDIB; 
 
    // return the DIB height if it is a Win 3.0 DIB 
    if (lpbmi->biSize == sizeof(BITMAPINFOHEADER)) 
        return lpbmi->biHeight; 
    else  // it is an OS/2 DIB, so return its height 
        return (DWORD)lpbmc->bcHeight; 
} 
 
DWORD DIBHeight(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBHeight(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 
 
/************************************************************************* 
 * 
 * PaletteSize() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpDIB      - pointer to packed-DIB memory block 
 * 
 * Return Value: 
 * 
 * WORD             - size of the color palette of the DIB 
 * 
 * Description: 
 * 
 * This function gets the size required to store the DIB's palette by 
 * multiplying the number of colors by the size of an RGBQUAD (for a 
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an OS/2- 
 * style DIB). 
 * 
 ************************************************************************/ 
WORD PaletteSize(LPBYTE lpDIB) 
{ 
    // calculate the size required by the palette 
    if (IS_WIN30_DIB (lpDIB)) 
        return (DIBNumColors(lpDIB) * sizeof(RGBQUAD)); 
    else 
        return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE)); 
} 
 
WORD PaletteSize(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = PaletteSize(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 
 
/************************************************************************* 
 * 
 * DIBNumColors() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpDIB      - pointer to packed-DIB memory block 
 * 
 * Return Value: 
 * 
 * WORD             - number of colors in the color table 
 * 
 * Description: 
 * 
 * This function calculates the number of colors in the DIB's color table 
 * by finding the bits per pixel for the DIB (whether Win3.0 or OS/2-style 
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256, 
 * if 24, no colors in color table. 
 * 
 ************************************************************************/ 
WORD DIBNumColors(LPBYTE lpDIB) 
{ 
    WORD wBitCount;  // DIB bit count 
 
    // If this is a Windows-style DIB, the number of colors in the 
    // color table can be less than the number of bits per pixel 
    // allows for (i.e. lpbi->biClrUsed can be set to some value). 
    // If this is the case, return the appropriate value. 
     
 
    if (IS_WIN30_DIB(lpDIB)) 
    { 
        DWORD dwClrUsed; 
 
        dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed; 
        if (dwClrUsed) 
 
        return (WORD)dwClrUsed; 
    } 
 
    // Calculate the number of colors in the color table based on 
    // the number of bits per pixel for the DIB. 
     
    if (IS_WIN30_DIB(lpDIB)) 
        wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
    else 
        wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
 
    // return number of colors based on bits per pixel 
 
    switch (wBitCount) 
    { 
        case 1: 
            return 2; 
 
        case 4: 
            return 16; 
 
        case 8: 
            return 256; 
 
        default: 
            return 0; 
    } 
} 

WORD DIBNumColors(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = DIBNumColors(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 

WORD DIBBitCount(LPBYTE lpDIB) 
{ 
    if (IS_WIN30_DIB(lpDIB)) 
        return ((LPBITMAPINFOHEADER)lpDIB)->biBitCount; 
    else 
        return ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount; 
} 

WORD DIBBitCount(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	WORD wSize = DIBBitCount(lpDIB);
	GlobalUnlock(hDIB);
	return wSize;
} 
 
/**************************************************************************** 
* 
*     FUNCTION: BytesPerLine 
* 
*     PURPOSE:  Calculates the number of bytes in one scan line. 
* 
*     PARAMS:   LPBYTE lpDIB - pointer to the BITMAPINFOHEADER 
                              that begins the CF_DIB block 
* 
*     RETURNS:  DWORD - number of bytes in one scan line (DWORD aligned) 
* 
\****************************************************************************/ 
DWORD BytesPerLine(LPBYTE lpDIB) 
{ 
    return WIDTHBYTES(((LPBITMAPINFOHEADER)lpDIB)->biWidth * ((LPBITMAPINFOHEADER)lpDIB)->biPlanes * ((LPBITMAPINFOHEADER)lpDIB)->biBitCount); 
} 

DWORD BytesPerLine(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = BytesPerLine(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 

/**************************************************************************** 
* 
*     FUNCTION: DIBlockSize 
* 
*     PURPOSE:  Calculates the number of bytes in one scan line. 
* 
*     PARAMS:   LPBYTE lpDIB - pointer to the BITMAPINFOHEADER 
                              that begins the CF_DIB block 
* 
*     RETURNS:  DWORD - DIB block size
* 
\****************************************************************************/ 
DWORD DIBlockSize(LPBYTE lpDIB)
{
	if (((LPBITMAPINFOHEADER)lpDIB)->biSizeImage == 0)
		((LPBITMAPINFOHEADER)lpDIB)->biSizeImage = BytesPerLine(lpDIB) * ((LPBITMAPINFOHEADER)lpDIB)->biHeight;
	return ((LPBITMAPINFOHEADER)lpDIB)->biSize + PaletteSize(lpDIB) + ((LPBITMAPINFOHEADER)lpDIB)->biSizeImage;
}

DWORD DIBlockSize(HDIB hDIB) 
{ 
	LPBYTE lpDIB = (LPBYTE)GlobalLock(hDIB);
	DWORD dw = DIBlockSize(lpDIB);
	GlobalUnlock(hDIB);
	return dw;
} 
 
/************************************************************************* 
 * 
 * CreateDIBPalette() 
 * 
 * Parameter: 
 * 
 * LPBYTE lpbi       - specifies the DIB 
 * 
 * Return Value: 
 * 
 * HPALETTE         - specifies the palette 
 * 
 * Description: 
 * 
 * This function creates a palette from a DIB by allocating memory for the 
 * logical palette, reading and storing the colors from the DIB's color table 
 * into the logical palette, creating a palette from this logical palette, 
 * and then returning the palette's handle. This allows the DIB to be 
 * displayed using the best possible colors (important for DIBs with 256 or 
 * more colors). 
 * 
 ************************************************************************/ 
HPALETTE CreateDIBPalette(LPBYTE lpbi) 
{ 
    LPLOGPALETTE        lpPal;          // pointer to a logical palette 
    HANDLE              hLogPal;        // handle to a logical palette 
    HPALETTE            hPal = NULL;    // handle to a palette 
    int                 i, wNumColors;  // loop index, number of colors in color table 
    LPBITMAPINFO        lpbmi;          // pointer to BITMAPINFO structure (Win3.0) 
    LPBITMAPCOREINFO    lpbmc;          // pointer to BITMAPCOREINFO structure (OS/2) 
    BOOL                bWinStyleDIB;   // Win3.0 DIB? 
 
    // if handle to DIB is invalid, return NULL 
 
    if (! lpbi) 
        return NULL; 
 
    // get pointer to BITMAPINFO (Win 3.0) 
 
    lpbmi = (LPBITMAPINFO)lpbi; 
 
    // get pointer to BITMAPCOREINFO (OS/2 1.x) 
 
    lpbmc = (LPBITMAPCOREINFO)lpbi; 
 
    // get the number of colors in the DIB  
 
    wNumColors = DIBNumColors(lpbi); 
 
    // is this a Win 3.0 DIB? 
 
    bWinStyleDIB = IS_WIN30_DIB(lpbi); 
    if (wNumColors) 
    { 
        // allocate memory block for logical palette 
 
        hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + 
                sizeof(PALETTEENTRY) * wNumColors); 
 
        // if not enough memory, clean up and return NULL 
 
        if (!hLogPal) 
            return NULL; 
 
        // lock memory block and get pointer to it 
 
        lpPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
        // set version and number of palette entries 
 
        lpPal->palVersion = PALVERSION; 
        lpPal->palNumEntries = wNumColors; 
 
        // store RGB triples (if Win 3.0 DIB) or RGB quads (if OS/2 DIB) 
        // into palette 
         
        for (i = 0; i < wNumColors; i++) 
        { 
            if (bWinStyleDIB) 
            { 
                lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed; 
                lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen; 
                lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue; 
                lpPal->palPalEntry[i].peFlags = 0; 
            } 
            else 
            { 
                lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed; 
                lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen; 
                lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue; 
                lpPal->palPalEntry[i].peFlags = 0; 
            } 
        } 
 
        // create the palette and get handle to it 
 
        hPal = CreatePalette(lpPal); 
 
        // if error getting handle to palette, clean up and return NULL 
 
        if (!hPal) 
        { 
            GlobalUnlock(hLogPal); 
            GlobalFree(hLogPal); 
            return NULL; 
        } 
    } 
 
    // return handle to DIB's palette 
    return hPal; 
} 
 
/************************************************************************* 
 * 
 * CreateDIBPalette() 
 * 
 * Parameter: 
 * 
 * HDIB hDIB        - specifies the DIB 
 * 
 * Return Value: 
 * 
 * HPALETTE         - specifies the palette 
 * 
 * Description: 
 * 
 * This function creates a palette from a DIB by allocating memory for the 
 * logical palette, reading and storing the colors from the DIB's color table 
 * into the logical palette, creating a palette from this logical palette, 
 * and then returning the palette's handle. This allows the DIB to be 
 * displayed using the best possible colors (important for DIBs with 256 or 
 * more colors). 
 * 
 ************************************************************************/ 
HPALETTE CreateDIBPalette(HDIB hDIB) 
{ 
    HPALETTE            hPal = NULL;    // handle to a palette 
    LPBYTE               lpbi;           // pointer to packed-DIB 
 
    // if handle to DIB is invalid, return NULL 
 
    if (!hDIB) 
        return NULL; 
 
    // lock DIB memory block and get a pointer to it 
 
    lpbi = (LPBYTE)GlobalLock(hDIB); 
 
	hPal = CreateDIBPalette(lpbi);

	// Unlock hDIB
    GlobalUnlock(hDIB); 
 
    // return handle to DIB's palette 
    return hPal; 
} 
 
/************************************************************************* 
 * 
 * DIBToBitmap() 
 * 
 * Parameters: 
 * 
 * HDIB hDIB        - specifies the DIB to convert 
 * 
 * HPALETTE hPal    - specifies the palette to use with the bitmap 
 * 
 * Return Value: 
 * 
 * HBITMAP          - identifies the device-dependent bitmap 
 * 
 * Description: 
 * 
 * This function creates a bitmap from a DIB using the specified palette. 
 * If no palette is specified, default is used. 
 * 
 * NOTE: 
 * 
 * The bitmap returned from this funciton is always a bitmap compatible 
 * with the screen (e.g. same bits/pixel and color planes) rather than 
 * a bitmap with the same attributes as the DIB.  This behavior is by 
 * design, and occurs because this function calls CreateDIBitmap to 
 * do its work, and CreateDIBitmap always creates a bitmap compatible 
 * with the hDC parameter passed in (because it in turn calls 
 * CreateCompatibleBitmap). 
 * 
 * So for instance, if your DIB is a monochrome DIB and you call this 
 * function, you will not get back a monochrome HBITMAP -- you will 
 * get an HBITMAP compatible with the screen DC, but with only 2 
 * colors used in the bitmap. 
 * 
 * If your application requires a monochrome HBITMAP returned for a 
 * monochrome DIB, use the function SetDIBits(). 
 * 
 * Also, the DIBpassed in to the function is not destroyed on exit. This 
 * must be done later, once it is no longer needed. 
 * 
 ************************************************************************/ 
HBITMAP DIBToBitmap(HDIB hDIB, HPALETTE hPal) 
{ 
    LPBYTE       lpDIBHdr, lpDIBBits;  // pointer to DIB header, pointer to DIB bits 
    HBITMAP     hBitmap;            // handle to device-dependent bitmap 
    HDC         hDC;                    // handle to DC 
    HPALETTE    hOldPal = NULL;    // handle to a palette 
 
    // if invalid handle, return NULL  
 
    if (!hDIB) 
        return NULL; 
 
    // lock memory block and get a pointer to it 
 
    lpDIBHdr = (LPBYTE)GlobalLock(hDIB); 
 
    // get a pointer to the DIB bits 
 
    lpDIBBits = FindDIBBits(lpDIBHdr); 
 
    // get a DC  
 
    hDC = GetDC(NULL); 
    if (!hDC) 
    { 
        // clean up and return NULL 
        GlobalUnlock(hDIB); 
        return NULL; 
    } 
 
    // select and realize palette 
 
    if (hPal) 
	{
        hOldPal = SelectPalette(hDC, hPal, FALSE); 
	    RealizePalette(hDC); 
	}
 
    // create bitmap from DIB info and bits 
    hBitmap = CreateDIBitmap(hDC, 
							(LPBITMAPINFOHEADER)lpDIBHdr, 
							CBM_INIT, 
							lpDIBBits, 
							(LPBITMAPINFO)lpDIBHdr, 
							DIB_RGB_COLORS);
 
    // restore previous palette 
    if (hOldPal) 
        SelectPalette(hDC, hOldPal, FALSE); 
 
    // clean up 
    ReleaseDC(NULL, hDC); 
    GlobalUnlock(hDIB); 
 
    // return handle to the bitmap 
    return hBitmap; 
} 
 
 
/************************************************************************* 
 * 
 * BitmapToDIB() 
 * 
 * Parameters: 
 * 
 * HBITMAP hBitmap  - specifies the bitmap to convert 
 * 
 * HPALETTE hPal    - specifies the palette to use with the bitmap 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-dependent bitmap 
 * 
 * Description: 
 * 
 * This function creates a DIB from a bitmap using the specified palette. 
 * 
 ************************************************************************/ 
HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal) 
{ 
    BITMAP              bm;         // bitmap structure 
    BITMAPINFOHEADER    bi;         // bitmap header 
    LPBITMAPINFOHEADER  lpbi;       // pointer to BITMAPINFOHEADER 
    DWORD               dwLen;      // size of memory block 
    HANDLE              hDIB, h;    // handle to DIB, temp handle 
    HDC                 hDC;        // handle to DC 
    WORD                biBits;     // bits per pixel 
 
    // check if bitmap handle is valid 
 
    if (!hBitmap) 
        return NULL; 
 
    // fill in BITMAP structure, return NULL if it didn't work 
 
    if (!GetObject(hBitmap, sizeof(bm), (LPBYTE)&bm)) 
        return NULL; 
 
    // if no palette is specified, use default palette 
 
    if (hPal == NULL) 
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE); 
 
    // calculate bits per pixel 
 
    biBits = bm.bmPlanes * bm.bmBitsPixel; 
 
    // make sure bits per pixel is valid 
 
    if (biBits <= 1) 
        biBits = 1; 
    else if (biBits <= 4) 
        biBits = 4; 
    else if (biBits <= 8) 
        biBits = 8; 
    else // if greater than 8-bit, force to 24-bit 
        biBits = 24; 
 
    // initialize BITMAPINFOHEADER 
 
    bi.biSize = sizeof(BITMAPINFOHEADER); 
    bi.biWidth = bm.bmWidth; 
    bi.biHeight = bm.bmHeight; 
    bi.biPlanes = 1; 
    bi.biBitCount = biBits; 
    bi.biCompression = BI_RGB; 
    bi.biSizeImage = 0; 
    bi.biXPelsPerMeter = 0; 
    bi.biYPelsPerMeter = 0; 
    bi.biClrUsed = 0; 
    bi.biClrImportant = 0; 
 
    // calculate size of memory block required to store BITMAPINFO 
 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi); 
 
    // get a DC 
 
    hDC = GetDC(NULL); 
 
    // select and realize our palette 
 
    hPal = SelectPalette(hDC, hPal, FALSE); 
    RealizePalette(hDC); 
 
    // alloc memory block to store our bitmap 
 
    hDIB = GlobalAlloc(GHND, dwLen); 
 
    // if we couldn't get memory block 
 
    if (!hDIB) 
    { 
      // clean up and return NULL 
 
      SelectPalette(hDC, hPal, TRUE); 
      RealizePalette(hDC); 
      ReleaseDC(NULL, hDC); 
      return NULL; 
    } 
 
    // lock memory and get pointer to it 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    /// use our bitmap info. to fill BITMAPINFOHEADER 
 
    *lpbi = bi; 
 
    // call GetDIBits with a NULL lpBits param, so it will calculate the 
    // biSizeImage field for us     
 
    GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, NULL, (LPBITMAPINFO)lpbi, 
        DIB_RGB_COLORS); 
 
    // get the info. returned by GetDIBits and unlock memory block 
 
    bi = *lpbi; 
    GlobalUnlock(hDIB); 
 
    // if the driver did not fill in the biSizeImage field, make one up  
    if (bi.biSizeImage == 0) 
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight; 
 
    // realloc the buffer big enough to hold all the bits 
 
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + bi.biSizeImage; 
 
    if (h = GlobalReAlloc(hDIB, dwLen, 0)) 
        hDIB = h; 
    else 
    { 
        // clean up and return NULL 
 
        GlobalFree(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    } 
 
    // lock memory block and get pointer to it */ 
 
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB); 
 
    // call GetDIBits with a NON-NULL lpBits param, and actualy get the 
    // bits this time 
 
    if (GetDIBits(hDC, hBitmap, 0, (UINT)bi.biHeight, (LPBYTE)lpbi + 
            (WORD)lpbi->biSize + PaletteSize((LPBYTE)lpbi), (LPBITMAPINFO)lpbi, 
            DIB_RGB_COLORS) == 0) 
    { 
        // clean up and return NULL 
 
        GlobalUnlock(hDIB); 
        hDIB = NULL; 
        SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
        ReleaseDC(NULL, hDC); 
        return NULL; 
    } 
 
    bi = *lpbi; 
 
    // clean up  
    GlobalUnlock(hDIB); 
    SelectPalette(hDC, hPal, TRUE); 
    RealizePalette(hDC); 
    ReleaseDC(NULL, hDC); 
 
    // return handle to the DIB 
    return hDIB; 
} 
 
/************************************************************************* 
 * 
 * BitmapToDIB() 
 * 
 * Parameters: 
 * 
 * HBITMAP hBitmap  - specifies the bitmap to convert 
 * 
 * HPALETTE hPal    - specifies the palette to use with the bitmap 
 * 
 * WORD wBitCount   - specifies the DIB colorbits 
 *
 * Return Value: 
 * 
 * HDIB             - identifies the device-dependent bitmap 
 * 
 * Description: 
 * 
 * This function creates a DIB from a bitmap using the specified palette. 
 * 
 ************************************************************************/ 
HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPalette, WORD wBitCount)
{ 
	HDIB hNewDib;

	if (! hBitmap)
        return NULL; 

	// get bitmap info
	BITMAP  bm;         // bitmap structure 
	GetObject(hBitmap, sizeof(bm), (LPBYTE)&bm); 
	int biBits = bm.bmPlanes * bm.bmBitsPixel;
	// make sure bits per pixel is valid 
	if (biBits <= 1) 
		biBits = 1; 
	else if (biBits <= 4) 
		biBits = 4; 
	else if (biBits <= 8) 
		biBits = 8; 
	else // if greater than 8-bit, force to 24-bit 
		biBits = 24; 

	// get DIB handle from current bitmap
	HDIB hDib = BitmapToDIB(hBitmap, hPalette);
	if (! hDib)
        return NULL; 

	if (wBitCount == biBits)
		hNewDib = hDib;
	else
	{
	    // DIB bits/pixel is not the same as device
		// convert dib format to accordingly
		hNewDib = ConvertDIBFormat(hDib, wBitCount, hPalette);
		// cleanup hDib
		GlobalFree(hDib);
	}

	return hNewDib;
} 


/************************************************************************* 
 * 
 * ChangeBitmapFormat() 
 * 
 * Parameter: 
 * 
 * HBITMAP          - handle to a bitmap 
 * 
 * WORD             - desired bits per pixel 
 * 
 * DWORD            - desired compression format 
 * 
 * HPALETTE         - handle to palette 
 * 
 * Return Value: 
 * 
 * HDIB             - handle to the new DIB if successful, else NULL 
 * 
 * Description: 
 * 
 * This function will convert a bitmap to the specified bits per pixel 
 * and compression format. The bitmap and it's palette will remain 
 * after calling this function. 
 * 
 ************************************************************************/ 
HDIB ChangeBitmapFormat (HBITMAP hbm, WORD biBits, DWORD biCompression, HPALETTE hpal)
{
    BITMAP               bm;
    BITMAPINFOHEADER     bi;
    LPBITMAPINFOHEADER   lpbi;
    DWORD                dwLen;
    HANDLE               hdib;
    HANDLE               h;
    HDC                  hdc;

    if (!hbm)
        return NULL;

    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    GetObject(hbm,sizeof(bm),(LPBYTE)&bm);

    if (biBits == 0)
	{
        biBits =  bm.bmPlanes * bm.bmBitsPixel;

		// make sure bits per pixel is valid 
	    if (biBits <= 1) 
		    biBits = 1; 
	    else if (biBits <= 4) 
		    biBits = 4; 
	    else if (biBits <= 8) 
		    biBits = 8; 
		else // if greater than 8-bit, force to 24-bit 
			biBits = 24; 
	}

    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = biBits;
    bi.biCompression        = biCompression;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + PaletteSize((LPBYTE)&bi);

    hdc = GetDC(NULL);
    HPALETTE hpalT = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);

    hdib = GlobalAlloc(GHND,dwLen);

    if (!hdib)
	{
        SelectPalette(hdc,hpalT,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    *lpbi = bi;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
    GetDIBits(hdc, hbm, 0L, (DWORD)bi.biHeight,
        (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bi.biSizeImage == 0)
	{
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

        if (biCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bi.biSize + PaletteSize((LPBYTE)&bi) + bi.biSizeImage;
    if (h = GlobalReAlloc(hdib,dwLen,0))
        hdib = h;
    else
	{
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc,hpalT,FALSE);
        ReleaseDC(NULL,hdc);
        return hdib;
    }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    if (! GetDIBits(hdc,
                    hbm,
                    0L,
                    (DWORD)bi.biHeight,
                    (LPBYTE)lpbi + (WORD)lpbi->biSize + PaletteSize((LPBYTE)lpbi),
                    (LPBITMAPINFO)lpbi, 
					(DWORD)DIB_RGB_COLORS))
	{
         GlobalUnlock(hdib);
         hdib = NULL;
         SelectPalette(hdc,hpalT,FALSE);
         ReleaseDC(NULL,hdc);
         return NULL;
    }
    bi = *lpbi;
   
	GlobalUnlock(hdib);
    SelectPalette(hdc,hpalT,FALSE);
    ReleaseDC(NULL,hdc);
    return hdib;
}


/************************************************************************* 
 * 
 * ChangeDIBFormat() 
 * 
 * Parameter: 
 * 
 * HDIB             - handle to packed-DIB in memory 
 * 
 * WORD             - desired bits per pixel 
 * 
 * DWORD            - desired compression format 
 * 
 * Return Value: 
 * 
 * HDIB             - handle to the new DIB if successful, else NULL 
 * 
 * Description: 
 * 
 * This function will convert the bits per pixel and/or the compression 
 * format of the specified DIB. Note: If the conversion was unsuccessful, 
 * we return NULL. The original DIB is left alone. Don't use code like the 
 * following: 
 * 
 *    hMyDIB = ChangeDIBFormat(hMyDIB, 8, BI_RLE4); 
 * 
 * The conversion will fail, but hMyDIB will now be NULL and the original 
 * DIB will now hang around in memory. We could have returned the old 
 * DIB, but we wanted to allow the programmer to check whether this 
 * conversion succeeded or failed. 
 * 
 ************************************************************************/ 
HDIB ChangeDIBFormat(HDIB hDIB, WORD wBitCount, DWORD dwCompression) 
{ 
    HBITMAP            hBitmap;         // Handle to bitmap 
    HDIB               hNewDIB = NULL;  // Handle to new DIB 
    HPALETTE           hPal;			// Handle to palette

    // Check for a valid DIB handle 
 
    if (!hDIB) 
        return NULL; 
 
    // Save the old DIB's palette 
 
    hPal = CreateDIBPalette(hDIB); 
    if (hPal == NULL)
        hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
 
    // Convert old DIB to a bitmap 
 
    hBitmap = DIBToBitmap(hDIB, hPal); 
    if (!hBitmap) 
    { 
        DeleteObject(hPal); 
        return NULL; 
    } 

	// Change bitmap format

	hNewDIB = ChangeBitmapFormat(hBitmap, wBitCount, dwCompression, hPal);
 
    // Clean up and return 
 
    DeleteObject(hBitmap); 
    DeleteObject(hPal); 
 
    return hNewDIB; 
} 
 

/**************************************************************************** 
* 
*     FUNCTION: ConvertDIBFormat 
* 
*     PURPOSE:  Creates a new DIB of the requested format, copies the source 
*               image to the new DIB. 
* 
*     PARAMS:   LPBITMAPINFO lpSrcDIB - the source CF_DIB 
*               UINT         nWidth   - width for new DIB 
*               UINT         nHeight  - height for new DIB 
*               UINT         nbpp     - bpp for new DIB 
*               BOOL         bStretch - TRUE to stretch source to dest 
*                                       FALSE to take upper left of image 
*				HPALETTE	 hPalSrc  - Palette used to set new DIB
*				DWORD		 dwSize   - return the size of new DIB pointer
* 
*     RETURNS:  HDIB - Handle of CF_DIB memory block with new image 
*               NULL on failure 
* 
\****************************************************************************/ 
HDIB ConvertDIBFormat(LPBYTE lpDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc) 
{ 
	LPBITMAPINFO lpSrcDIB =  (LPBITMAPINFO)lpDIB;
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hDib;
 
    // Allocate and fill out a BITMAPINFO struct for the new DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    // Fill in the color table 
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // Set the color tables for the DIBSections 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // If we are asking for a straight copy, do it 
    if( (lpSrcDIB->bmiHeader.biWidth==lpbmi->bmiHeader.biWidth) && (lpSrcDIB->bmiHeader.biHeight==lpbmi->bmiHeader.biHeight) ) 
    { 
        BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
    } 
    else 
    { 
        // else, should we stretch it? 
        if( bStretch ) 
        { 
            SetStretchBltMode( hTargetDC, COLORONCOLOR ); 
            StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY ); 
        } 
        else 
        { 
            // or just take the upper left corner of the source 
            BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
        } 
    } 
 
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    // Allocate enough memory for the new CF_DIB, and copy bits 
    hDib = GlobalAlloc(GHND, dwTargetHeaderSize + dwTargetBitsSize ); 
    lpResult = (LPBYTE)GlobalLock(hDib); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    // final cleanup 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
	free( lpbmi ); 
    GlobalUnlock(hDib);
 
    return hDib; 
} 
/* End ConvertDIBFormat() 1***************************************************/ 

/**************************************************************************** 
* 
*     FUNCTION: ConvertDIBFormat 
* 
*     PURPOSE:  Creates a new DIB of the requested format, copies the source 
*               image to the new DIB. 
* 
*     PARAMS:   HDIB		 hDIB     - the source CF_DIB 
*               UINT         nWidth   - width for new DIB 
*               UINT         nHeight  - height for new DIB 
*               UINT         nbpp     - bpp for new DIB 
*               BOOL         bStretch - TRUE to stretch source to dest 
*                                       FALSE to take upper left of image 
*				HPALETTE	 hPalSrc  - Palette used to set new DIB
* 
*     RETURNS:  HDIB - new CF_DIB handle
* 
\****************************************************************************/ 
HDIB ConvertDIBFormat(HDIB hDIB, UINT nWidth, UINT nHeight, UINT nbpp, BOOL bStretch, HPALETTE hPalSrc)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;

	// Get DIB pointer
	if (! hDIB)
		return NULL;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;

    // Allocate and fill out a BITMAPINFO struct for the new DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    // Fill in the color table 
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // Set the color tables for the DIBSections 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // If we are asking for a straight copy, do it 
    if( (lpSrcDIB->bmiHeader.biWidth==lpbmi->bmiHeader.biWidth) && (lpSrcDIB->bmiHeader.biHeight==lpbmi->bmiHeader.biHeight) ) 
    { 
        BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
    } 
    else 
    { 
        // else, should we stretch it? 
        if( bStretch ) 
        { 
            SetStretchBltMode( hTargetDC, COLORONCOLOR ); 
            StretchBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, lpSrcDIB->bmiHeader.biWidth, lpSrcDIB->bmiHeader.biHeight, SRCCOPY ); 
        } 
        else 
        { 
            // or just take the upper left corner of the source 
            BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
        } 
    } 
 
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    // Allocate enough memory for the new CF_DIB, and copy bits 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);//malloc( dwTargetHeaderSize + dwTargetBitsSize ); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    // final cleanup 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}
/* End ConvertDIBFormat() 2***************************************************/ 

/**************************************************************************** 
* 
*     FUNCTION: ConvertDIBFormat 
* 
*     PURPOSE:  Creates a new DIB of the requested format, copies the source 
*               image to the new DIB. 
* 
*     PARAMS:   LPBYTE		 lpDIB    - the source CF_DIB 
*               UINT         nbpp     - bpp for new DIB 
*				HPALETTE	 hPalSrc  - Palette used to set new DIB
* 
*     RETURNS:  HDIB - new CF_DIB handle
* 
\****************************************************************************/ 
HDIB ConvertDIBFormat(LPBYTE lpDIB, UINT nbpp, HPALETTE hPalSrc)
{
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)lpDIB;
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HDIB		 hNewDIB;
	DWORD		 dwSize;
	int			 nWidth, nHeight;

	nWidth = lpSrcDIB->bmiHeader.biWidth;
	nHeight = lpSrcDIB->bmiHeader.biHeight;

    // Allocate and fill out a BITMAPINFO struct for the new DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    // Fill in the color table 
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // Set the color tables for the DIBSections 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // We are asking for a straight copy, do it 
    BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
 
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    // Allocate enough memory for the new CF_DIB, and copy bits 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);//malloc( dwTargetHeaderSize + dwTargetBitsSize ); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    // final cleanup 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}
/* End ConvertDIBFormat() 3***************************************************/ 

/**************************************************************************** 
* 
*     FUNCTION: ConvertDIBFormat 
* 
*     PURPOSE:  Creates a new DIB of the requested format, copies the source 
*               image to the new DIB. 
* 
*     PARAMS:   HDIB		 hDIB     - the source CF_DIB 
*               UINT         nbpp     - bpp for new DIB 
*				HPALETTE	 hPalSrc  - Palette used to set new DIB
* 
*     RETURNS:  HDIB - new CF_DIB handle
* 
\****************************************************************************/ 
HDIB ConvertDIBFormat(HDIB hDIB, UINT nbpp, HPALETTE hPalSrc)
{
    LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize, dwColorNum; 
	HANDLE		 hNewDIB;
	DWORD		 dwSize;
	int			 nWidth, nHeight;

	// Get DIB pointer
	if (! hDIB)
		return NULL;
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;
	nWidth = lpSrcDIB->bmiHeader.biWidth;
	nHeight = lpSrcDIB->bmiHeader.biHeight;

    // Allocate and fill out a BITMAPINFO struct for the new DIB 
    if (nbpp <= 8) 
		dwColorNum = 1 << nbpp;
	else
		dwColorNum = 0;
    dwTargetHeaderSize = sizeof( BITMAPINFO ) + ( dwColorNum * sizeof( RGBQUAD ) ); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = nbpp; 
    lpbmi->bmiHeader.biCompression = BI_RGB; 
    lpbmi->bmiHeader.biSizeImage = 0; 
    lpbmi->bmiHeader.biXPelsPerMeter = 0; 
    lpbmi->bmiHeader.biYPelsPerMeter = 0; 
    lpbmi->bmiHeader.biClrUsed = 0; 
    lpbmi->bmiHeader.biClrImportant = 0; 
    // Fill in the color table 
    if( ! CopyColorTable( lpbmi, (LPBITMAPINFO)lpSrcDIB, hPalSrc ) ) 
    { 
        free( lpbmi ); 
        return NULL; 
    } 
 
    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
 
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
 
    // Set the color tables for the DIBSections 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hTargetDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // We are asking for a straight copy, do it 
    BitBlt( hTargetDC, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, hSourceDC, 0, 0, SRCCOPY ); 
 
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    // Allocate enough memory for the new CF_DIB, and copy bits 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB = GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);//malloc( dwTargetHeaderSize + dwTargetBitsSize ); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPBYTE)lpResult ), lpTargetBits, dwTargetBitsSize ); 
 
    // final cleanup 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
 
    return hNewDIB;
}
/* End ConvertDIBFormat() 4***************************************************/ 
 
/**************************************************************************** 
* 
*     FUNCTION: CopyColorTable 
* 
*     PURPOSE:  Copies the color table from one CF_DIB to another. 
* 
*     PARAMS:   LPBITMAPINFO lpTarget - pointer to target DIB 
*               LPBITMAPINFO lpSource - pointer to source DIB 
* 
*     RETURNS:  BOOL - TRUE for success, FALSE for failure 
* 
\****************************************************************************/ 
BOOL CopyColorTable( LPBITMAPINFO lpTarget, LPBITMAPINFO lpSource, HPALETTE hPalSrc ) 
{ 
    // What we do depends on the target's color depth 
    switch( lpTarget->bmiHeader.biBitCount ) 
    { 
        // 8bpp - need 256 entry color table 
        case 8: 
			if (hPalSrc)
			{ // Palette is provided, use it
				PALETTEENTRY    pe[256]; 
				UINT            i; 

				GetPaletteEntries( hPalSrc, 0, 256, pe ); 
				for(i=0;i<256;i++) 
				{ 
					lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
					lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
					lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
					lpTarget->bmiColors[i].rgbReserved = 0; 
				} 
			}
			else
			{ // no palette povided
				if( lpSource->bmiHeader.biBitCount == 8 ) 
				{ // Source is 8bpp too, copy color table 
					memcpy( lpTarget->bmiColors, lpSource->bmiColors, 256*sizeof(RGBQUAD) ); 
				} 
				else 
				{ // Source is != 8bpp, use Octree algorithm to create palette
					HPALETTE        hPal; 
					HDC            hDC = GetDC( NULL ); 
					PALETTEENTRY    pe[256]; 
					UINT            i; 
 
					hPal = CreateOctreePalette((LPBYTE)lpSource, 236, 8);
					if (! hPal)	 // use halftone palette                 
						hPal = CreateHalftonePalette( hDC ); 
					ReleaseDC( NULL, hDC ); 

					GetPaletteEntries( hPal, 0, 256, pe ); 
					DeleteObject( hPal ); 
					for(i=0;i<256;i++) 
					{ 
						lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
						lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
						lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
						lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags; 
					} 
				}
			}
			break; // end 8bpp 
 
        // 4bpp - need 16 entry color table 
        case 4: 
			if (hPalSrc)
			{ // Palette is provided, use it
				PALETTEENTRY    pe[16]; 
				UINT            i; 

				GetPaletteEntries( hPalSrc, 0, 16, pe ); 
				for(i=0;i<16;i++) 
				{ 
					lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
					lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
					lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
					lpTarget->bmiColors[i].rgbReserved = 0; 
				} 
			}
			else
			{ // No palette is provided
				if( lpSource->bmiHeader.biBitCount == 4 ) 
				{ // Source is 4bpp too, copy color table 
					memcpy( lpTarget->bmiColors, lpSource->bmiColors, 16*sizeof(RGBQUAD) ); 
				} 
				else 
				{ // Source is != 4bpp, use system palette 
					HPALETTE        hPal; 
					PALETTEENTRY    pe[256]; 
					UINT            i; 
 
					hPal = (HPALETTE)GetStockObject( DEFAULT_PALETTE ); 
					GetPaletteEntries( hPal, 0, 16, pe ); 
					for(i=0;i<16;i++) 
					{ 
						lpTarget->bmiColors[i].rgbRed = pe[i].peRed; 
						lpTarget->bmiColors[i].rgbGreen = pe[i].peGreen; 
						lpTarget->bmiColors[i].rgbBlue = pe[i].peBlue; 
						lpTarget->bmiColors[i].rgbReserved = pe[i].peFlags; 
					}
                } 
			}
			break; // end 4bpp 
 
        // 1bpp - need 2 entry mono color table 
        case 1: 
            lpTarget->bmiColors[0].rgbRed = 0; 
            lpTarget->bmiColors[0].rgbGreen = 0; 
            lpTarget->bmiColors[0].rgbBlue = 0; 
            lpTarget->bmiColors[0].rgbReserved = 0; 
            lpTarget->bmiColors[1].rgbRed = 255; 
            lpTarget->bmiColors[1].rgbGreen = 255; 
            lpTarget->bmiColors[1].rgbBlue = 255; 
            lpTarget->bmiColors[1].rgbReserved = 0; 
			break; // end 1bpp 
 
        // no color table for the > 8bpp modes 
        case 32: 
        case 24: 
        case 16: 
        default: 
	        break; 
    } 
    return TRUE; 
} 
/* End CopyColorTable() *****************************************************/ 

/**************************************************************************** 
* 
*     FUNCTION: DIBToDIBSection 
* 
*     PURPOSE:  Create DIBSECTION from DIB
* 
*     PARAMS:   LPBYTE lpDIB - pointer to DIB data buffer
* 
*     RETURNS:  HBITMAP - handle of DIBSECTION, or NULL for failure 
* 
\****************************************************************************/ 
HBITMAP DIBToDIBSection(LPBYTE lpDIB) 
{
    LPBYTE       lpSourceBits; 
    HDC			 hDC = NULL, hSourceDC; 
    HBITMAP      hSourceBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize; 

	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)lpDIB;
	if (! lpSrcDIB)
		return NULL;

    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = GetDC( NULL ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
 
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPBYTE)lpSrcDIB), dwSourceBitsSize ); 
 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
 
    // Set the color tables for the DIBSections 
    if( lpSrcDIB->bmiHeader.biBitCount <= 8 ) 
        SetDIBColorTable( hSourceDC, 0, 1 << lpSrcDIB->bmiHeader.biBitCount, lpSrcDIB->bmiColors ); 

    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    DeleteDC( hSourceDC ); 
    ReleaseDC( NULL, hDC ); 
 
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
 
    return hSourceBitmap;
} 

// Create DIBSECTION from DIB
HBITMAP DIBToDIBSection(HDIB hDIB) 
{
    HBITMAP      hSourceBitmap; 

	// Get DIB pointer
	if (! hDIB)
		return NULL;
	LPBYTE lpSrcDIB = (LPBYTE)GlobalLock(hDIB);
	if (! lpSrcDIB)
		return NULL;

	hSourceBitmap = DIBToDIBSection(lpSrcDIB);

    // final cleanup 
	GlobalUnlock(hDIB);
 
    return hSourceBitmap;
} 

/**************************************************************************** 
* 
*     FUNCTION: DIBSectionToDIB 
* 
*     PURPOSE:  Create DIB from DIBSECTION
* 
*     PARAMS:   HBITMAP - handle of DIBSECTION, or NULL for failure 
* 
*     RETURNS:  HBITMAP hBitmap - DIB handle
* 
\****************************************************************************/ 
HDIB DIBSectionToDIB(HBITMAP hBitmap)
{
    HDC			 hDC = NULL, hSourceDC; 
	HBITMAP		 hOldSourceBitmap;
	HANDLE		 hNewDIB;
    LPBITMAPINFO lpbmi = NULL; 
	DWORD		 dwSize;
	DIBSECTION   ds;
    DWORD		 dwColorNum; 

	// get DIB info
	GetObject(hBitmap, sizeof(DIBSECTION), &ds);
    dwColorNum = ds.dsBmih.biClrUsed; 
    if (dwColorNum == 0 && ds.dsBmih.biBitCount <= 8) 
		dwColorNum = 1 << ds.dsBmih.biBitCount;

    // Allocate and fill out a BITMAPINFO struct for the new DIB 
	dwSize = sizeof(BITMAPINFOHEADER) + (dwColorNum * sizeof(RGBQUAD)) + ds.dsBmih.biSizeImage;
	hNewDIB = GlobalAlloc(GHND, dwSize);
	if (! hNewDIB)
		return NULL;
	lpbmi = (LPBITMAPINFO)GlobalLock(hNewDIB);
	if (! lpbmi)
		return NULL;
    lpbmi->bmiHeader.biSize = sizeof( BITMAPINFOHEADER ); 
    lpbmi->bmiHeader.biWidth = ds.dsBm.bmWidth; 
    lpbmi->bmiHeader.biHeight = ds.dsBm.bmHeight; 
    lpbmi->bmiHeader.biPlanes = 1; 
    lpbmi->bmiHeader.biBitCount = ds.dsBmih.biBitCount; 
    lpbmi->bmiHeader.biCompression = ds.dsBmih.biCompression; 
    lpbmi->bmiHeader.biSizeImage = ds.dsBmih.biSizeImage; 
    lpbmi->bmiHeader.biXPelsPerMeter = ds.dsBmih.biXPelsPerMeter; 
    lpbmi->bmiHeader.biYPelsPerMeter = ds.dsBmih.biYPelsPerMeter; 
    lpbmi->bmiHeader.biClrUsed = ds.dsBmih.biClrUsed; 
    lpbmi->bmiHeader.biClrImportant = ds.dsBmih.biClrImportant; 

	// get DC for operation
	hDC = GetDC( NULL ); 
 
	// get DIB bits
    if (! GetDIBits(hDC,
                    hBitmap,
                    0L,
                    (DWORD)ds.dsBm.bmHeight,
                    (LPBYTE)lpbmi + (WORD)lpbmi->bmiHeader.biSize + (dwColorNum*sizeof(RGBQUAD)),
                    (LPBITMAPINFO)lpbmi, 
					(DWORD)DIB_RGB_COLORS))
	{
         GlobalUnlock(hNewDIB);
         ReleaseDC(NULL,hDC);
         return NULL;
    }

	// memory DC
    hSourceDC = CreateCompatibleDC( hDC ); 
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hBitmap ); 
 
    // Fill in the color table from DIBSection
    if( lpbmi->bmiHeader.biBitCount <= 8 ) 
        GetDIBColorTable( hSourceDC, 0, 1 << lpbmi->bmiHeader.biBitCount, lpbmi->bmiColors ); 
 
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    DeleteDC( hSourceDC ); 
    ReleaseDC( NULL, hDC ); 
	GlobalUnlock(hNewDIB);
 
    return hNewDIB; 
}


/************************************************************************* 
 * 
 * CopyScreenToBitmap() 
 * 
 * Parameter: 
 * 
 * LPRECT lpRect    - specifies the window 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-dependent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part of the screen to a device- 
 * dependent bitmap. 
 * 
 * 
 ************************************************************************/ 
HBITMAP CopyScreenToBitmap(LPRECT lpRect) 
{ 
    HDC         hScrDC, hMemDC;         // screen DC and memory DC 
    HBITMAP     hBitmap, hOldBitmap;    // handles to deice-dependent bitmaps 
    int         nX, nY, nX2, nY2;       // coordinates of rectangle to grab 
    int         nWidth, nHeight;        // DIB width and height 
    int         xScrn, yScrn;           // screen resolution 
 
    // check for an empty rectangle 
 
    if (IsRectEmpty(lpRect)) 
      return NULL; 
 
    // create a DC for the screen and create 
    // a memory DC compatible to screen DC 
     
    hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL); 
    hMemDC = CreateCompatibleDC(hScrDC); 
 
    // get points of rectangle to grab 
 
    nX = lpRect->left; 
    nY = lpRect->top; 
    nX2 = lpRect->right; 
    nY2 = lpRect->bottom; 
 
    // get screen resolution 
 
    xScrn = GetDeviceCaps(hScrDC, HORZRES); 
    yScrn = GetDeviceCaps(hScrDC, VERTRES); 
 
    //make sure bitmap rectangle is visible 
 
    if (nX < 0) 
        nX = 0; 
    if (nY < 0) 
        nY = 0; 
    if (nX2 > xScrn) 
        nX2 = xScrn; 
    if (nY2 > yScrn) 
        nY2 = yScrn; 
 
    nWidth = nX2 - nX; 
    nHeight = nY2 - nY; 
 
    // create a bitmap compatible with the screen DC 
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
 
    // select new bitmap into memory DC 
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
 
    // bitblt screen DC to memory DC 
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); 
 
    // select old bitmap back into memory DC and get handle to 
    // bitmap of the screen 
     
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap); 
 
    // clean up 
 
    DeleteDC(hScrDC); 
    DeleteDC(hMemDC); 
 
    // return handle to the bitmap 
 
    return hBitmap; 
} 

/************************************************************************* 
 * 
 * CopyWindowToBitmap() 
 * 
 * Parameters: 
 * 
 * HWND hWnd        - specifies the window 
 * 
 * WORD fPrintArea  - specifies the window area to copy into the device- 
 *                    dependent bitmap 
 * 
 * Return Value: 
 * 
 * HDIB         - identifies the device-dependent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part(s) of the window to a device- 
 * dependent bitmap. 
 * 
 * 
 ************************************************************************/ 
HBITMAP CopyWindowToBitmap(HWND hWnd, WORD fPrintArea) 
{ 
    HBITMAP     hBitmap = NULL;  // handle to device-dependent bitmap 
 
    // check for a valid window handle 
 
    if (!hWnd) 
        return NULL; 
 
    switch (fPrintArea) 
    { 
        case PW_WINDOW: // copy entire window 
        { 
            RECT    rectWnd; 
 
            // get the window rectangle 
 
            GetWindowRect(hWnd, &rectWnd); 
 
            // get the bitmap of that window by calling 
            // CopyScreenToBitmap and passing it the window rect 
        
            hBitmap = CopyScreenToBitmap(&rectWnd); 
            break; 
        } 
         
 
        case PW_CLIENT: // copy client area 
        { 
            RECT rectClient; 
            POINT pt1, pt2; 
 
            // get client dimensions 
 
            GetClientRect(hWnd, &rectClient); 
 
            // convert client coords to screen coords 
 
            pt1.x = rectClient.left; 
            pt1.y = rectClient.top; 
            pt2.x = rectClient.right; 
            pt2.y = rectClient.bottom; 
            ClientToScreen(hWnd, &pt1); 
            ClientToScreen(hWnd, &pt2); 
            rectClient.left = pt1.x; 
            rectClient.top = pt1.y; 
            rectClient.right = pt2.x; 
            rectClient.bottom = pt2.y; 
 
            // get the bitmap of the client area by calling 
            // CopyScreenToBitmap and passing it the client rect 
             
            hBitmap = CopyScreenToBitmap(&rectClient); 
            break; 
        } 
     
        default:    // invalid print area 
            return NULL; 
    } 
 
    // return handle to the bitmap 
 
   return hBitmap; 
} 
 
 
/************************************************************************* 
 * 
 * CopyClientRectToBitmap() 
 * 
 * Parameters: 
 * 
 * HWND hWnd        - specifies the window 
 * 
 * LPRECT lpRect    - specifies the client area rect to copy into the device- 
 *                    independent bitmap 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-independent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part(s) of the window client to a 
 * device-independent bitmap. 
 * 
 ************************************************************************/ 
HBITMAP CopyClientRectToBitmap(HWND hWnd, LPRECT lpRect) 
{ 
   HBITMAP     hBitmap = NULL;  // handle to DIB 
 
   // check for a valid window handle 
 
    if (!hWnd) 
        return NULL; 
 
    POINT   pt1, pt2; 

    // convert client coords to screen coords 

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ClientToScreen(hWnd, &pt1); 
    ClientToScreen(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

    // get the DIB of the client area by calling 
    // CopyScreenToDIB and passing it the client rect 

    hBitmap = CopyScreenToBitmap(lpRect); 
 
	// restore lpRect

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ScreenToClient(hWnd, &pt1); 
    ScreenToClient(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

	// return the handle to the DIB 
 
   return hBitmap; 
} 
 
/************************************************************************* 
 * 
 * CopyScreenToDIB() 
 * 
 * Parameter: 
 * 
 * LPRECT lpRect    - specifies the window 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-independent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part of the screen to a device- 
 * independent bitmap. 
 * 
 ************************************************************************/ 
HDIB CopyScreenToDIB(LPRECT lpRect) 
{ 
    HBITMAP     hBitmap;        // handle to device-dependent bitmap 
    HPALETTE    hPalette;       // handle to palette 
    HDIB        hDIB = NULL;    // handle to DIB 
 
    // get the device-dependent bitmap in lpRect by calling 
    //  CopyScreenToBitmap and passing it the rectangle to grab 
 
    hBitmap = CopyScreenToBitmap(lpRect); 
 
    // check for a valid bitmap handle 
 
    if (!hBitmap) 
      return NULL; 
 
    // get the current palette 
 
    hPalette = GetSystemPalette(); 
 
    // convert the bitmap to a DIB 
 
    hDIB = BitmapToDIB(hBitmap, hPalette); 
 
    // clean up  
 
    DeleteObject(hPalette); 
    DeleteObject(hBitmap); 
 
    // return handle to the packed-DIB 
    return hDIB; 
} 
 
/************************************************************************* 
 * 
 * CopyWindowToDIB() 
 * 
 * Parameters: 
 * 
 * HWND hWnd        - specifies the window 
 * 
 * WORD fPrintArea  - specifies the window area to copy into the device- 
 *                    independent bitmap 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-independent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part(s) of the window to a device- 
 * independent bitmap. 
 * 
 ************************************************************************/ 
HDIB CopyWindowToDIB(HWND hWnd, WORD fPrintArea) 
{ 
   HDIB     hDIB = NULL;  // handle to DIB 
 
   // check for a valid window handle 
 
    if (!hWnd) 
        return NULL; 
 
    switch (fPrintArea) 
    { 
        case PW_WINDOW: // copy entire window 
        { 
            RECT    rectWnd; 
 
            // get the window rectangle 
 
            GetWindowRect(hWnd, &rectWnd); 
 
            // get the DIB of the window by calling 
            // CopyScreenToDIB and passing it the window rect 
             
            hDIB = CopyScreenToDIB(&rectWnd); 
            break; 
        } 
       
        case PW_CLIENT: // copy client area 
        { 
            RECT    rectClient; 
            POINT   pt1, pt2; 
 
            // get the client area dimensions 
 
            GetClientRect(hWnd, &rectClient); 
 
            // convert client coords to screen coords 
 
            pt1.x = rectClient.left; 
            pt1.y = rectClient.top; 
            pt2.x = rectClient.right; 
            pt2.y = rectClient.bottom; 
            ClientToScreen(hWnd, &pt1); 
            ClientToScreen(hWnd, &pt2); 
            rectClient.left = pt1.x; 
            rectClient.top = pt1.y; 
            rectClient.right = pt2.x; 
            rectClient.bottom = pt2.y; 
 
            // get the DIB of the client area by calling 
            // CopyScreenToDIB and passing it the client rect 
 
            hDIB = CopyScreenToDIB(&rectClient); 
            break; 
        } 
       
        default:    // invalid print area 
            return NULL; 
    } 
 
   // return the handle to the DIB 
 
   return hDIB; 
} 
 
/************************************************************************* 
 * 
 * CopyClientRectToDIB() 
 * 
 * Parameters: 
 * 
 * HWND hWnd        - specifies the window 
 * 
 * LPRECT lpRect    - specifies the client area rect to copy into the device- 
 *                    independent bitmap 
 * 
 * Return Value: 
 * 
 * HDIB             - identifies the device-independent bitmap 
 * 
 * Description: 
 * 
 * This function copies the specified part(s) of the window client to a 
 * device-independent bitmap. 
 * 
 ************************************************************************/ 
HDIB CopyClientRectToDIB(HWND hWnd, LPRECT lpRect) 
{ 
   HDIB     hDIB = NULL;  // handle to DIB 
 
   // check for a valid window handle 
 
    if (!hWnd) 
        return NULL; 
 
    POINT   pt1, pt2; 

    // convert client coords to screen coords 

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ClientToScreen(hWnd, &pt1); 
    ClientToScreen(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

    // get the DIB of the client area by calling 
    // CopyScreenToDIB and passing it the client rect 

    hDIB = CopyScreenToDIB(lpRect); 
 
	// restore lpRect

    pt1.x = lpRect->left; 
    pt1.y = lpRect->top; 
    pt2.x = lpRect->right; 
    pt2.y = lpRect->bottom; 
    ScreenToClient(hWnd, &pt1); 
    ScreenToClient(hWnd, &pt2); 
    lpRect->left = pt1.x; 
    lpRect->top = pt1.y; 
    lpRect->right = pt2.x; 
    lpRect->bottom = pt2.y; 

	// return the handle to the DIB 
 
   return hDIB; 
} 

 
/************************************************************************* 
 * 
 * PaintDIB() 
 * 
 * Parameters: 
 * 
 * HDC hDC          - DC to do output to 
 * 
 * LPRECT lpDCRect  - rectangle on DC to do output to 
 * 
 * HDIB hDIB        - handle to global memory with a DIB spec 
 *                    in it followed by the DIB bits 
 * 
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect 
 *
 * HPALETTE hPal    - Palette used to diaplsy DIB, if is NULL, 
 *					  use DIB palette to display
 *
 * DWORD dwRop      - ROP mode to display DIB
 * 
 * Return Value: 
 * 
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise 
 * 
 * Description: 
 *   Painting routine for a DIB.  Calls StretchDIBits() or 
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is 
 *   output to the specified DC, at the coordinates given 
 *   in lpDCRect.  The area of the DIB to be output is 
 *   given by lpDIBRect. 
 * 
 * NOTE: This function always selects the palette as background. Before 
 * calling this function, be sure your palette is selected to desired 
 * priority (foreground or background). 
 * 
 * 
 ************************************************************************/ 
BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect, 
        HPALETTE hPal, DWORD dwRop)
{ 
    LPBYTE        lpDIBHdr;           // Pointer to BITMAPINFOHEADER 
    LPBYTE        lpDIBBits;          // Pointer to DIB bits 
    BOOL         bSuccess=FALSE;     // Success/fail flag 
    HPALETTE     hOldPal=NULL;       // Previous palette 
 
    // Check for valid DIB handle 
 
    if (!hDIB) 
        return FALSE; 

    // Lock down the DIB, and get a pointer to the beginning of the bit 
    // buffer 
     
    lpDIBHdr  = (LPBYTE)GlobalLock(hDIB); 
    lpDIBBits = FindDIBBits(lpDIBHdr); 
 
	// if no palette provided, create one from DIB

    if (! hPal) 
		hPal = CreateDIBPalette(lpDIBHdr);

    // Select and realize our palette as background 
 
    if (hPal) 
    { 
        hOldPal = SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
    } 
 
    // Make sure to use the stretching mode best for color pictures 
 
    SetStretchBltMode(hDC, COLORONCOLOR); 
 
    // Call StretchDIBits() with dwRop
 
	bSuccess = StretchDIBits(hDC, 
							 lpDCRect->left, 
							 lpDCRect->top, 
							 RECTWIDTH(lpDCRect), 
							 RECTHEIGHT(lpDCRect), 
							 lpDIBRect->left, 
				             (int)DIBHeight(lpDIBHdr) - lpDIBRect->top - RECTHEIGHT(lpDIBRect), 
							 RECTWIDTH(lpDIBRect), 
							 RECTHEIGHT(lpDIBRect), 
							 lpDIBBits, 
							 (LPBITMAPINFO)lpDIBHdr, 
							 DIB_RGB_COLORS, 
							 SRCCOPY); 
 
    // Unlock the memory block 
 
    GlobalUnlock(hDIB); 
 
    // Reselect old palette 
 
    if (hOldPal) 
		SelectPalette(hDC, hOldPal, FALSE); 
 
    // Return with success/fail flag 
    return bSuccess; 
} 

/************************************************************************* 
 * 
 * PaintBitmap() 
 * 
 * Parameters: 
 * 
 * HDC hDC          - DC to do output to 
 * 
 * LPRECT lpDCRect  - rectangle on DC to do output to 
 * 
 * HBITMAP hDDB     - handle to device-dependent bitmap (DDB) 
 * 
 * LPRECT lpDDBRect - rectangle of DDB to output into lpDCRect 
 * 
 * HPALETTE hPalette - handle to the palette to use with hDDB 
 * 
 * Return Value: 
 * 
 * BOOL             - TRUE if bitmap was drawn, FLASE otherwise 
 * 
 * Description: 
 * 
 * Painting routine for a DDB.  Calls BitBlt() or 
 * StretchBlt() to paint the DDB.  The DDB is 
 * output to the specified DC, at the coordinates given 
 * in lpDCRect.  The area of the DDB to be output is 
 * given by lpDDBRect.  The specified palette is used. 
 * 
 * NOTE: This function always selects the palette as background. Before 
 * calling this function, be sure your palette is selected to desired 
 * priority (foreground or background). 
 * 
 ************************************************************************/ 
BOOL PaintBitmap(HDC hDC, LPRECT lpDCRect, HBITMAP hDDB,  LPRECT lpDDBRect,  
        HPALETTE hPal, DWORD dwRop)
{ 
    HDC         hMemDC;            // Handle to memory DC 
    HBITMAP     hOldBitmap;        // Handle to previous bitmap 
    HPALETTE    hOldPal1 = NULL;   // Handle to previous palette 
    HPALETTE    hOldPal2 = NULL;   // Handle to previous palette 
    BOOL        bSuccess = FALSE;  // Success/fail flag 
 
    // Create a memory DC 
 
    hMemDC = CreateCompatibleDC (hDC); 
 
    // If this failed, return FALSE 
 
    if (!hMemDC) 
        return FALSE; 
 
    // If we have a palette, select and realize it 
 
    if (hPal) 
    { 
        hOldPal1 = SelectPalette(hMemDC, hPal, TRUE); 
        hOldPal2 = SelectPalette(hDC, hPal, TRUE); 
        RealizePalette(hDC); 
    } 
 
    // Select bitmap into the memory DC 
 
    hOldBitmap = (HBITMAP)SelectObject (hMemDC, hDDB); 
 
    // Make sure to use the stretching mode best for color pictures 
 
    SetStretchBltMode (hDC, COLORONCOLOR); 
 
    // Determine whether to call StretchBlt() or BitBlt() 
    if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDDBRect)) && 
            (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect))) 
        bSuccess = BitBlt(hDC, lpDCRect->left, lpDCRect->top, 
                lpDCRect->right - lpDCRect->left, 
                lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left, 
                lpDDBRect->top, dwRop); 
    else 
        bSuccess = StretchBlt(hDC, lpDCRect->left,  lpDCRect->top,  
                lpDCRect->right - lpDCRect->left, 
                lpDCRect->bottom - lpDCRect->top, hMemDC, lpDDBRect->left,  
                lpDDBRect->top,  lpDDBRect->right - lpDDBRect->left, 
                lpDDBRect->bottom - lpDDBRect->top, dwRop); 
 
    // Clean up 
 
    SelectObject(hMemDC, hOldBitmap); 
 
    if (hOldPal1) 
        SelectPalette (hMemDC, hOldPal1, FALSE); 
 
    if (hOldPal2) 
        SelectPalette (hDC, hOldPal2, FALSE); 
 
    DeleteDC (hMemDC); 
 
    // Return with success/fail flag 
 
    return bSuccess; 
} 

/************************************************************************* 
 * 
 * DisplayPalette () 
 * 
 * Parameter: 
 * 
 * HDC hDC          - handle of device context to display palette
 * LPRECT lpRect      - rect range to show palette
 * HPALETTE hPal     - handle of palette to display
 * 
 * Return Value: 
 * 
 * BOOL            - TRUE, if success, else FALSE
 * 
 * Description: 
 * 
 * This function displays palette on the specified rectangle in a device context 
 * 
 ************************************************************************/ 
BOOL DisplayPalette(HDC hDC, LPRECT lpRect, HPALETTE hPal)
{
	if (! hPal)
		return FALSE;

	int nEntries;
	PALETTEENTRY pe[256];
	nEntries = GetPaletteEntries(hPal, 0, 256, pe);
	int nSqr = (int)sqrt((double)nEntries);

	int nWidth = (lpRect->right-lpRect->left)/nSqr;
	int nHeight = (lpRect->bottom-lpRect->top)/nSqr;
	lpRect->right = lpRect->left + nWidth*nSqr;
	lpRect->bottom = lpRect->top + nHeight*nSqr;

	HPALETTE hOldPal = (HPALETTE)SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	HBRUSH hBrush, hOldBrush;
	int x, y;
	for (int i=0; i<nEntries; ++i)
	{
		x = i%nSqr;
		y = i/nSqr;
		hBrush = CreateSolidBrush(RGB(pe[i].peRed, pe[i].peGreen, pe[i].peBlue));
		hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, 
			      lpRect->left + x*nWidth, 
				  lpRect->top + y*nHeight, 
				  lpRect->left + (x+1)*nWidth, 
				  lpRect->top + (y+1) *nHeight);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
	}
	
	SelectPalette(hDC, hOldPal, FALSE);

	return TRUE;
}

/************************************************************************* 
 * 
 * CopyPalette () 
 * 
 * Parameter: 
 * 
 * HPALETTE hPalSrc    - source palette handle
 * 
 * Return Value: 
 * 
 * HPALETTE          - destination palette handle
 * 
 * Description: 
 * 
 * This function copys the source palette to a new palette handle 
 * 
 ************************************************************************/ 
HPALETTE CopyPalette(HPALETTE hPalSrc)
{
    PLOGPALETTE     plogPal;
    int             iNumEntries=0;
    HPALETTE        hPal;
	HANDLE			h;

	iNumEntries = GetPaletteEntries(hPalSrc, 0, iNumEntries, NULL);
    if (iNumEntries == 0) 
        return (HPALETTE) NULL;

	h = GlobalAlloc(GHND, sizeof(DWORD) + sizeof(PALETTEENTRY)*iNumEntries);
	if (! h)
        return (HPALETTE) NULL;
	plogPal = (PLOGPALETTE)GlobalLock(h);
    if (! plogPal) 
        return (HPALETTE) NULL;

    plogPal->palVersion = 0x300;
    plogPal->palNumEntries = (WORD) iNumEntries;
    GetPaletteEntries(hPalSrc, 0, iNumEntries, plogPal->palPalEntry);
    hPal = CreatePalette(plogPal);

    GlobalUnlock(h);
    GlobalFree(h);

    return hPal;
}
 
/************************************************************************* 
 * 
 * PalEntriesOnDevice() 
 * 
 * Parameter: 
 * 
 * HDC hDC          - device context 
 * 
 * Return Value: 
 * 
 * int              - number of palette entries on device 
 * 
 * Description: 
 * 
 * This function gets the number of palette entries on the specified device 
 * 
 ************************************************************************/ 
int PalEntriesOnDevice(HDC hDC) 
{ 
    int nColors;  // number of colors 
 
    // Find out the number of colors on this device. 
     
    nColors = (1 << (GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES))); 
 
    assert(nColors); 
    return nColors; 
} 
 
 
/************************************************************************* 
 * 
 * GetSystemPalette() 
 * 
 * Parameters: 
 * 
 * None 
 * 
 * Return Value: 
 * 
 * HPALETTE         - handle to a copy of the current system palette 
 * 
 * Description: 
 * 
 * This function returns a handle to a palette which represents the system 
 * palette.  The system RGB values are copied into our logical palette using 
 * the GetSystemPaletteEntries function.   
 * 
 ************************************************************************/ 
HPALETTE GetSystemPalette(void) 
{ 
    HDC hDC;                // handle to a DC 
    static HPALETTE hPal = NULL;   // handle to a palette 
    HANDLE hLogPal;         // handle to a logical palette 
    LPLOGPALETTE lpLogPal;  // pointer to a logical palette 
    int nColors;            // number of colors 
 
    // Find out how many palette entries we want. 
 
    hDC = GetDC(NULL); 
 
    if (!hDC) 
        return NULL; 
 
    nColors = PalEntriesOnDevice(hDC);   // Number of palette entries 
 
    // Allocate room for the palette and lock it. 
 
    hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * 
            sizeof(PALETTEENTRY)); 
 
    // if we didn't get a logical palette, return NULL 
 
    if (!hLogPal) 
        return NULL; 
 
    // get a pointer to the logical palette 
 
    lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal); 
 
    // set some important fields 
 
    lpLogPal->palVersion = PALVERSION; 
    lpLogPal->palNumEntries = nColors; 
 
    // Copy the current system palette into our logical palette 
 
    GetSystemPaletteEntries(hDC, 0, nColors, 
            (LPPALETTEENTRY)(lpLogPal->palPalEntry)); 
 
    // Go ahead and create the palette.  Once it's created, 
    // we no longer need the LOGPALETTE, so free it.     
 
    hPal = CreatePalette(lpLogPal); 
 
    // clean up 
 
    GlobalUnlock(hLogPal); 
    GlobalFree(hLogPal); 
    ReleaseDC(NULL, hDC); 
 
    return hPal; 
} 
 
/************************************************************************* 
 * 
 * CreateIdentifyPalette () 
 * 
 * Parameter: 
 * 
 * HPALETTE hPalSrc    - source palette handle
 * 
 * Return Value: 
 * 
 * HPALETTE          - destination identify palette handle
 * 
 * Description: 
 * 
 * This function creates an identify palette from the source palette handle 
 * 
 ************************************************************************/ 
HPALETTE CreateIdentifyPalette(HPALETTE hPalSrc)
{
    BOOL bResult = FALSE;
    int i, iSysColors, iPalEntries;
    HPALETTE hPalette, hpalOld;

	if (! hPalSrc)
		return NULL;

	// create a new palette equal to input
	hPalette = CopyPalette(hPalSrc);

    // Get a screen DC to work with
    HDC hdcScreen = GetDC(NULL);
    ASSERT(hdcScreen);

    // Make sure we are on a palettized device
    if (!(GetDeviceCaps(hdcScreen, RASTERCAPS) & RC_PALETTE)) 
	{
        TRACE("Not a palettized device");
        goto abort;
    }

    // Get the number of system colors and the number of palette entries
    // Note that on a palletized device the number of colors is the
    // number of guaranteed colors.  I.e. the number of reserved system colors
    iSysColors = GetDeviceCaps(hdcScreen, NUMCOLORS);
    iPalEntries = GetDeviceCaps(hdcScreen, SIZEPALETTE);

    // if there are more than 256 colors we are wasting our time
    if (iSysColors > 256) goto abort;

    // Now we force the palette manager to reset its tables so that
    // the next palette to be realized will get its colors in the order they are 
    // in the logical palette. This is done by changing the number of
    // reserved colors.
    SetSystemPaletteUse(hdcScreen, SYSPAL_NOSTATIC);
    SetSystemPaletteUse(hdcScreen, SYSPAL_STATIC);

    // Select our palette into the screen DC and realize it so that
    // its colors will be entered into the free slots in the physical palette
    hpalOld = ::SelectPalette(hdcScreen,
                              hPalette, // our hpal
                              FALSE);
    ::RealizePalette(hdcScreen);
    // Now replace the old palette (but don't realize it)
    ::SelectPalette(hdcScreen, hpalOld, FALSE);

    // The physical palette now has our colors set in place and its own
    // reserved colors at either end.  We can grab the lot now
    PALETTEENTRY pe[256];
    GetSystemPaletteEntries(hdcScreen, 
                            0,
                            iPalEntries,
                            pe);

    // Set the PC_NOCOLLAPSE flag for each of our colors so that GDI
    // won't merge them together.  Be careful not to set PC_NOCOLLAPSE for the 
    // sys color entries or we'll get multpile copies of these colors in
    // the palette when we realize it.
    for (i = 0; i < iSysColors/2; i++) {
        pe[i].peFlags = 0;
    }
    for (; i < iPalEntries-iSysColors/2; i++) {
        pe[i].peFlags = PC_NOCOLLAPSE;
    }
    for (; i < iPalEntries; i++) {
        pe[i].peFlags = 0;
    }

    // Resize the palette in case it was smaller
    ResizePalette(hPalette, iPalEntries);

    // Update the palette entries with what is now in the physical palette
    SetPaletteEntries(hPalette, 0, iPalEntries, pe);
    bResult = TRUE;

abort:
    ::ReleaseDC(NULL, hdcScreen);
    return bResult ? hPalette : NULL;
}

/************************************************************************* 
 * 
 * MapDIBColorsToPalette () 
 * 
 * Parameter: 
 * 
 * HDIB hDIB		    - DIB handle
 * HPALETTE hPalette    - given palette handle
 * 
 * Return Value: 
 * 
 * HPALETTE          - destination identify palette handle
 * 
 * Description: 
 * 
 * This function Map the colors in a DIB to a given palette.
 * 
 ************************************************************************/ 
BOOL MapDIBColorsToPalette(HDIB hDIB, HPALETTE hPalette)
{
    if (! hDIB)
        return FALSE;
    if (! hPalette)
        return FALSE;

    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    if (! lpbi)
        return FALSE;
    LPRGBQUAD pctThis = (LPRGBQUAD)((LPBYTE)lpbi + lpbi->biSize);

    // build an index translation table to map this DIBs colors
    // to those of the reference DIB
    BYTE imap[256];
	int i;
    for ( i = 0; i < 256; i++) 
	{
        imap[i] = (BYTE)GetNearestPaletteIndex(hPalette, 
									RGB(pctThis->rgbRed,
										pctThis->rgbGreen,
										pctThis->rgbBlue));
        pctThis++;
    }
    // now map the DIB bits
    LPBYTE pBits = (LPBYTE)lpbi + lpbi->biSize + PaletteSize((LPBYTE)lpbi);
    int iSize = WIDTHBYTES(lpbi->biBitCount*lpbi->biWidth) * lpbi->biHeight;
    while (iSize--) 
	{
        *pBits = imap[*pBits];
        pBits++;
    }
    // Now reset the DIB color table so that its RGB values match
    // those in the palette
    PALETTEENTRY pe[256];
    GetPaletteEntries(hPalette, 0, 256, pe);
    pctThis = (LPRGBQUAD)((LPBYTE)lpbi + lpbi->biSize);
    for (i = 0; i < 256; i++) 
	{
        pctThis->rgbRed = pe[i].peRed;    
        pctThis->rgbGreen = pe[i].peGreen;    
        pctThis->rgbBlue = pe[i].peBlue;
        pctThis++;    
    }
	
	GlobalUnlock(hDIB);
    return TRUE;
}

/************************************************************************* 
 * 
 * DrawTransparentBitmap () 
 * 
 * Parameters: 
 * 
 * HDC hDC          - DC to do output to 
 * 
 * HBITMAP hBitmap    - handle of bitmap to draw transparently
 * 
 * LONG xStart, yStart   - start position to draw *
 *
 * COLORREF cTransparentColor - transparent color * 
 * Return Value: 
 * 
 * Description: none
 *		 Draw bitmap transparently
 * 
 ************************************************************************/ 
void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, 
						   LONG xStart, LONG yStart, 
						   COLORREF cTransparentColor)
{
   BITMAP     bm;
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   POINT      ptSize;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   GetObject(hBitmap, sizeof(BITMAP), (LPBYTE)&bm);
   ptSize.x = bm.bmWidth;            // Get width of bitmap
   ptSize.y = bm.bmHeight;           // Get height of bitmap
   DPtoLP(hdcTemp, &ptSize, 1);      // Convert from device
                                     // to logical points

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
   bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = (HBITMAP)SelectObject(hdcBack, bmAndBack);
   bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
   bmMemOld    = (HBITMAP)SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = (HBITMAP)SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
          SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
          NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
          SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
          SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
}

/************************************************************************* 
 * 
 * Fade() 
 * 
 * Parameters: 
 * 
 * int nDeltaR   -  change value of Red
 * int nDeltaG   -  change value of Green
 * int nDeltaB   -  change value of Blue
 * BYTE rm		 -  maxinum value of Red
 * BYTE gm		 -  maxinum value of Green
 * BYTE bm		 -  maxinum value of Blue
 * BYTE *r		 -  pointer of value of Red after change
 * BYTE *g		 -  pointer of value of Green after change
 * BYTE *b		 -  pointer of value of Blue after change
 * 
 * Return Value: 
 *
 * BOOL          -  TRUE if success, FALSE is failue
 * 
 * Description: 
 * 
 * This function change the value of R, G, b
 * 
 ************************************************************************/
BOOL Fade(int nDeltaR, int nDeltaG, int nDeltaB, 
		  BYTE rm, BYTE gm, BYTE bm,
		  BYTE *r, BYTE *g, BYTE *b)
{
	int R = *r + nDeltaR;
	int G = *g + nDeltaG;
	int B = *b + nDeltaB;

	if (nDeltaR < 0)
		*r = (BYTE)BOUND(R, rm, 255);
	else
		*r = (BYTE)BOUND(R, 0, rm);
	if (nDeltaG < 0)
		*g = (BYTE)BOUND(G, gm, 255);
	else
		*g = (BYTE)BOUND(G, 0, gm);
	if (nDeltaB < 0)
		*b = (BYTE)BOUND(B, bm, 255);
	else
		*b = (BYTE)BOUND(B, 0, bm);

	if (*r==rm && *g==gm && *b==bm)
		return TRUE;
	else
		return FALSE;
}


/************************************************************************* 
 * 
 * Delay() 
 * 
 * Parameters: 
 * 
 * DWORD dwDelayTime   - ms to delay
 * 
 * Return Value: none
 * 
 * Description: 
 * 
 * This function Delay specified ms before perform next instruction 
 * 
 ************************************************************************/ 
void Delay(DWORD dwDelayTime)
{
    DWORD dwTimeBegin, dwTimeEnd;
    dwTimeBegin = timeGetTime();
    do
	{
        dwTimeEnd = timeGetTime();
	} while (dwTimeEnd - dwTimeBegin < dwDelayTime);
}

/************************************************************************* 
 * 
 * CopyHandle() 
 * 
 * Parameters: 
 * 
 * HANDLE h         - source handle
 * 
 * Return Value: 
 * 
 * HANDLE           - duplicated handle
 * 
 * Description: 
 * 
 * Copy memory handle to another
 ************************************************************************/ 
HANDLE CopyHandle(HANDLE h)
{
	if (h == NULL)
		return NULL;

	DWORD  dwLen = ::GlobalSize((HGLOBAL)h);
	HANDLE hCopy = ::GlobalAlloc(GHND, dwLen);
	if (hCopy == NULL)
		return NULL;

	void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
	void* lp     = ::GlobalLock((HGLOBAL) h);
	::CopyMemory(lpCopy, lp, dwLen);
	::GlobalUnlock(hCopy);
	::GlobalUnlock(h);

	return hCopy;
}
