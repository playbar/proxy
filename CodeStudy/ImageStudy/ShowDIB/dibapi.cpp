//********************************************************************** 
// 
//  AIBAPI.CPP
// 
//  Source file for Device-Independent Bitmap (DIB) API. 
// 
//********************************************************************** 
 
#include "stdafx.h"
#include <math.h> 
#include <windows.h> 
#include "dibapi.h" 

 
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
HANDLE LoadDIB(LPCTSTR lpFileName) 
{ 
    HANDLE        hDIB; 
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
 * SaveDIB() 
 * 
 * Saves the specified DIB into the specified file name on disk.  No 
 * error checking is done, so if the file already exists, it will be 
 * written over. 
 * 
 * Parameters: 
 * 
 * HANDLE hDib - Handle to the dib to save 
 * 
 * LPCTSTR lpFileName - pointer to full pathname to save DIB under 
 * 
 * Return value: TRUE if successful
 *				 FALSE if failure
 * 
 *************************************************************************/ 
BOOL SaveDIB(HANDLE hDib, LPCTSTR lpFileName) 
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
 
    dwDIBSize = *(LPDWORD)lpBI + DIBNumColors((LPBYTE)lpBI) * sizeof(RGBQUAD);   
 
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
            DIBNumColors((LPBYTE)lpBI) * sizeof(RGBQUAD); 
 
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
 * PaintDIB() 
 * 
 * Parameters: 
 * 
 * HDC hDC          - DC to do output to 
 * 
 * LPRECT lpDCRect  - rectangle on DC to do output to 
 * 
 * HANDLE hDIB        - handle to global memory with a DIB spec 
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
BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HANDLE hDIB, LPRECT lpDIBRect, 
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
    lpDIBBits = lpDIBHdr + sizeof(BITMAPINFOHEADER) + 
                DIBNumColors(lpDIBHdr)*sizeof(RGBQUAD);
	
 
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
				             ((LPBITMAPINFOHEADER)lpDIBHdr)->biHeight - lpDIBRect->top - RECTHEIGHT(lpDIBRect), 
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
 
	GlobalUnlock(hDIB);

    // Return with success/fail flag 
    return bSuccess; 
} 

