// Gif.cpp: implementation of the CGif class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "Gif.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Global variables
//
GIF89 Gif89 = {-1,-1,-1,0};
GIFSCREEN GifScreen;
static unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
                                  0x001F, 0x003F, 0x007F, 0x00FF,
                                  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
                                  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGif::CGif()
{
	ZeroDataBlock = FALSE;
	m_strGIFError = "No Error"; // yet
	m_pDib = NULL;
	cur_accum = 0;
	cur_bits = 0;
	maxbits = BITS;
	maxmaxcode = (code_int)1 << BITS;
	free_ent = 0;
	clear_flg = 0;

	m_pDib = NULL;
}

CGif::CGif(CDib *pDib)
{
	ZeroDataBlock = FALSE;
	m_strGIFError = "No Error"; // yet
	m_pDib = NULL;
	cur_accum = 0;
	cur_bits = 0;
	maxbits = BITS;
	maxmaxcode = (code_int)1 << BITS;
	free_ent = 0;
	clear_flg = 0;

	m_pDib = NULL;

	SetDib(pDib);
}

CGif::~CGif()
{
	if (m_pDib != NULL)
		delete m_pDib;
}

// free allocate memory
void CGif::FreeBuffer(BYTE *Buffer)
{
	delete[] Buffer;
}

// get error string
CString CGif::GetErrorString()
{
	return m_strGIFError;
}

// load gif file
BOOL CGif::Load(LPCTSTR lpstrFileName)
{
	UINT uWidth, uHeight, uWidthDW;

	// read the GIF to a packed buffer of RGB bytes
	BYTE *lpTmpBuffer = ReadGIFFile(lpstrFileName, &uWidth, &uHeight);
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

// save gif file
BOOL CGif::Save(LPCTSTR lpstrFileName, CDib* pDib)
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

	//if (pDibTmp->GetBitCount() != 24)
	//	pDibTmp->ConvertFormat(24);

	UINT uWidth  = pDibTmp->GetWidth();
	UINT uHeight = pDibTmp->GetHeight();

	// convert to 8-bit image
	if (pDibTmp->GetBitCount() != 8)
		pDibTmp->ColorQuantize(8);

	// get 256 color r, g, b
	PALETTEENTRY PaletteColors[256];
	pDibTmp->GetPalette()->GetPaletteEntries(0, 256, PaletteColors);
	int red[256], blue[256], green[256];
	for (int i=0;i<256;i++) 
	{
		red[i]   = PaletteColors[i].peRed;
		green[i] = PaletteColors[i].peGreen;
		blue[i]  = PaletteColors[i].peBlue;
	}

	// convert from DIB format (DWORD aligned, vertically flipped, red and blue swapped)
	BYTE *tmp = ClearDwordAlign(pDibTmp->GetBitsPtr(), uWidth, uHeight, 8);
	if (tmp == NULL)
		return FALSE;

	// convert from DIB
	VertFlipBuf(tmp, uWidth, uHeight);

	BOOL bSuccess = WriteGIFFile(lpstrFileName,
							tmp, uWidth, uHeight,
							0,		// background color
							red, green, blue);

	delete pDibTmp;
	FreeBuffer(tmp);

	return bSuccess;
}

LPBYTE CGif::ReadGIFFile(LPCTSTR lpstrFileName, UINT *uWidth, UINT *uHeight)
{
	BYTE			buf[16];
	BYTE			c;
	BYTE			localColorMap[3][MAXCOLORMAPSIZE];
	int				useGlobalColormap;
	int				bitPixel;
	int				imageCount	=0;
	char			version[4];
	FILE 			*fd;          
	int 			w=0;
	int				h=0;	

	CString strFileName = lpstrFileName;
	if (strFileName.IsEmpty()) 
	{
		m_strGIFError="No Name Given";
		return NULL;
	}

	BYTE *bigBuf;

	fd=fopen(lpstrFileName,"rb");
	if (fd==NULL) 
	{                       
		m_strGIFError="Cant open GIF :\n" + strFileName;
		return NULL;
	}

	// read GIF file header
	if (!ReadOK(fd,buf,6)) 
	{
		m_strGIFError="Error reading GIF Magic #\n"+strFileName;
		fclose(fd);
		return NULL;
	}
	
	// need the string "GIF" in the header
	if (strncmp((char *)buf,"GIF",3)!=0) 
	{
		m_strGIFError="Error, "+strFileName+" is not a valid .GIF file";
		fclose(fd);
		return NULL;
	}	

	strncpy(version,(char *)(buf+3),3);
	version[3]='\0';

	// only handle v 87a and 89a
	if ((strcmp(version,"87a")!=0)&&(strcmp(version,"89a")!=0)) 
	{
		m_strGIFError="Error, Bad GIF Version number";
		fclose(fd);
		return NULL;
	}	

	// screen description
	if (!ReadOK(fd,buf,7)) 
	{
		m_strGIFError="Error, failed to GIF read screen descriptor.\nGiving up";
		fclose(fd);
		return NULL;
	}

	GifScreen.Width		=	LM_to_uint((BYTE)buf[0],(BYTE)buf[1]);
	GifScreen.Height	=	LM_to_uint((BYTE)buf[2],(BYTE)buf[3]);
	GifScreen.BitPixel	=	2 << ((BYTE)buf[4] & 0x07);
	GifScreen.ColorResolution = ((((BYTE)buf[4] & 0x70) >> 3) + 1);
	GifScreen.BackGround=	(BYTE)buf[5];									// background color...
	GifScreen.AspectRatio=	(BYTE)buf[6];
            

	// read colormaps
	if (BitSet((BYTE)buf[4],LOCALCOLORMAP)) 
	{
		if (!ReadColorMap(fd,GifScreen.BitPixel,GifScreen.ColorMap)) 
		{
			m_strGIFError="Error reading GIF colormap";
			fclose(fd);
			return NULL;                                             
		}
	}

	// non-square pixels, so what?	
	if ((GifScreen.AspectRatio!=0 ) && (GifScreen.AspectRatio!=49)) 
	{
		m_strGIFError="Non-square pixels in GIF image.\nIgnoring that fact...";
	}

	// there can be multiple images in a GIF file... uh?
	// what the hell do we do with multiple images?
	// so, we'll be interested in just the first image, cause we're lazy

	for(;;) 
	{	
		// read a byte;
		if (!ReadOK(fd,&c,1)) 
		{
			m_strGIFError="Unexpected EOF in GIF.\nGiving up";
			fclose(fd);
			return NULL; 
		}
	
		// image terminator
		if (c==';') 
		{
		}
	
		if (c=='!') 
		{
			if (!ReadOK(fd,&c,1)) 
			{
				m_strGIFError="Error on extension read.\nGiving up";
				fclose(fd);
				return NULL;       
			}
			DoExtension(fd,c);
			continue;
		}
	
		if (c!=',') 
		{
			// Ignoring c
			continue;
		}
	
		// read image header
		if (!ReadOK(fd,buf,9)) 
		{
			m_strGIFError="Error on dimension read\nGiving up";
			fclose(fd);
			return NULL;                     
		}
	
		useGlobalColormap=!BitSet((BYTE)buf[8],LOCALCOLORMAP);
	
		bitPixel=1<<(((BYTE)buf[8]&0x07)+1);
	
	    // let's see if we have enough mem to continue?

		long bufsize;

		if ((int)buf[5]>4) 
		{
			//AfxMessageBox("This GIF file claims to be > 2000 bytes wide!",MB_OK | MB_ICONINFORMATION);
		}
		if ((int)buf[7]>4) 
		{
			//AfxMessageBox("This GIF file claims to be > 2000 bytes high!",MB_OK | MB_ICONINFORMATION);
		}		                                                       
		
		w=LM_to_uint((BYTE)buf[4],(BYTE)buf[5]);		
		h=LM_to_uint((BYTE)buf[6],(BYTE)buf[7]);
		
		if ((w<0) || (h<0)) 
		{
			m_strGIFError="Negative image dimensions!\nGiving up";
			fclose(fd);
			return NULL;
		}
				
		bufsize=(long)w*(long)h;
		bufsize*=3;
		bigBuf= (BYTE *) new char [bufsize];
		
		if (bigBuf==NULL) 
		{
			m_strGIFError="Out of Memory in GIFRead";
			fclose(fd);
			return NULL;
		}
			
		if (!useGlobalColormap) 
		{
			if (!ReadColorMap(fd,bitPixel,localColorMap)) 
			{
				m_strGIFError="Error reading GIF colormap\nGiving up";
				delete [] bigBuf;
				fclose(fd);
				return NULL;                     
			}
	
		 	//read image
			if (!ReadImage(fd, bigBuf, w, h, localColorMap, BitSet((BYTE)buf[8],INTERLACE))) 
			{
				m_strGIFError="Error reading GIF file\nLocalColorMap\nGiving up";
				delete [] bigBuf;
				fclose(fd);
				return NULL; 				
			}
		} 
		else 
		{
			if (!ReadImage(fd, bigBuf, w, h, GifScreen.ColorMap, BitSet((BYTE)buf[8],INTERLACE))) 
			{
				m_strGIFError="Error reading GIF file\nGIFScreen Colormap\nGiving up";
				delete [] bigBuf;
				fclose(fd);
				return NULL; 			
			}
		}
		break;
	}

	*uWidth=w;
	*uHeight=h;
		
	fclose(fd);
	return bigBuf;
}

int CGif::ReadColorMap(FILE *fd, int number, BYTE buffer[3][MAXCOLORMAPSIZE])
{
	int 	i;
	BYTE rgb[3];
	
	for (i=0;i < number; ++i) 
	{
		if (!ReadOK(fd,rgb,sizeof(rgb))) 
		{
			return FALSE;
		}
		
		buffer[CM_RED][i]=rgb[0];
		buffer[CM_GREEN][i]=rgb[1];
		buffer[CM_BLUE][i]=rgb[2];
	}	
	return TRUE;
}

int CGif::DoExtension(FILE *fd, int label)
{
	static char buf[256];
	char	*str;

	switch(label) 
	{
	case 0x01  :
		str="Plain Text Ext";
		break;
	case 0xff :
		str= "Appl ext";
		break;
	case 0xfe :
		str="Comment Ext";
		while (GetDataBlock(fd,(BYTE *)buf)!=0);
		return FALSE;
	case 0XF9 :
		str="Graphic Ctrl Ext";
		(void)GetDataBlock(fd,(BYTE *)buf);
		Gif89.disposal	=(buf[0]>>2)		&0x7;
		Gif89.inputFlag	=(buf[0]>>1)		&0x1;
		Gif89.delayTime	=LM_to_uint(buf[1],buf[2]);
		if ((buf[0]&0x1)!=0)
			Gif89.transparent=buf[3];
	
		while (GetDataBlock(fd,(BYTE *)buf)!=0);
		return FALSE;
	default :
		str=buf;
		sprintf(buf,"UNKNOWN (0x%02x)",label);
		break;
	}
	
	while (GetDataBlock(fd,(BYTE *)buf)!=0);

	return FALSE;
}

int CGif::GetDataBlock(FILE *fd, BYTE *buf)
{
	BYTE count;

	if (!ReadOK(fd,&count,1)) 
	{
		//m_strGIFError="Error in GIF DataBlock Size";
		return -1;
	}

	ZeroDataBlock=count==0;

	if ((count!=0) && (!ReadOK(fd,buf,count))) 
	{
		//m_strGIFError="Error reading GIF datablock";
		return -1;
	}
	return count;
}

int CGif::GetCode(FILE *fd, int code_size, int flag)
{
	static BYTE buf[280];
	static int curbit, lastbit, done, last_byte;
	int i,j,ret;
	BYTE count;

	if (flag) 
	{
		curbit=0;
		lastbit=0;
		done=FALSE;
		return 0;
	}

	if ((curbit+code_size) >=lastbit) 
	{
		if (done) 
		{
			if (curbit >=lastbit) 
			{
				//m_strGIFError="Ran off the end of my bits";
				return 0;
			}
			return -1;
		}
		buf[0]=buf[last_byte-2];	
		buf[1]=buf[last_byte-1];

		if ((count=GetDataBlock(fd,&buf[2]))==0)
			done=TRUE;

		last_byte=2+count;

		curbit=(curbit - lastbit) + 16;

		lastbit = (2+count)*8;
	}
	ret=0;
	for (i=curbit,j=0; j<code_size;++i,++j)
		ret|=((buf[i/8]&(1<<(i% 8)))!=0)<<j;

	curbit+=code_size;

	return ret;
}

int CGif::LZWReadByte(FILE *fd, int flag, int input_code_size)
{
	static int fresh=FALSE;
	int code, incode;
	static int code_size, set_code_size;
	static int max_code, max_code_size;
	static int firstcode, oldcode;
	static int clear_code, end_code;

	static unsigned short  next[1<<MAX_LZW_BITS];
	static BYTE  vals[1<<MAX_LZW_BITS];
	static BYTE  stack [1<<(MAX_LZW_BITS+1)];
	static BYTE  *sp;
	
	register int i;

	if (flag) 
	{
		set_code_size=input_code_size;
		code_size=set_code_size+1;
		clear_code=1<<set_code_size;
		end_code = clear_code+1;
		max_code = clear_code+2;
		max_code_size=2*clear_code;

		GetCode(fd,0,TRUE);

		fresh=TRUE;
	
		for(i=0;i<clear_code;++i) 
		{
			next[i]=0;
			vals[i]=i;
		}

		for (;i<(1<<MAX_LZW_BITS);++i)
			next[i]=vals[0]=0;
	
		sp=stack;

		return 0;
	} 
	else if (fresh) 
	{
			fresh=FALSE;
			do 
			{
				firstcode=oldcode=GetCode(fd,code_size,FALSE);
			} while (firstcode==clear_code);
			return firstcode;
		}

		if (sp > stack)
			return *--sp;

		while ((code= GetCode(fd,code_size,FALSE)) >=0) 
		{
			if (code==clear_code) 
			{
				for (i=0;i<clear_code;++i) 
				{
					next[i]=0;
					vals[i]=i;
				}
				for (;i<(1<<MAX_LZW_BITS);++i)	
					next[i]=vals[i]=0;
				code_size=set_code_size+1;
				max_code_size=2*clear_code;
				max_code=clear_code+2;
				sp=stack;
				firstcode=oldcode=GetCode(fd,code_size,FALSE);
				return firstcode;
			} 
			else if (code==end_code) 
			{
				int count;
				BYTE buf[260];
		
				if (ZeroDataBlock)
					return -2;

				while ((count=GetDataBlock(fd,buf)) >0);

				if (count!=0)
					//AfxMessageBox("Missing EOD in GIF data stream (common occurrence)",MB_OK);
				return -2;	
			}

			incode = code;

			if (code >= max_code) 
			{
				*sp++=firstcode;
				code=oldcode;
			}

			while (code >=clear_code) 
			{
				*sp++=vals[code];
				if (code==(int)next[code]) 
				{
					//m_strGIFError="Circular table entry, big GIF Error!";
					return -1;
				}
				code=next[code];
			}

			*sp++ = firstcode=vals[code];

			if ((code=max_code) <(1<<MAX_LZW_BITS)) 
			{
				next[code]=oldcode;
				vals[code]=firstcode;
				++max_code;
				if ((max_code >=max_code_size) &&
					(max_code_size < (1<<MAX_LZW_BITS))) 
				{
					 max_code_size*=2;
					++code_size;
				}
			}

		oldcode=incode;

		if (sp > stack)
			return *--sp;
	}
	return code;
}   


BOOL CGif::ReadImage(FILE *fd, BYTE  * bigMemBuf, int width, int height,
					 BYTE cmap[3][MAXCOLORMAPSIZE], int interlace)
{
	BYTE c;
	int color;
	int xpos=0, ypos=0, pass=0;
	long curidx;

	if (!ReadOK(fd,& c,1)) 
	{
		return FALSE;
	}

	if (LZWReadByte(fd,TRUE,c)<0) 
	{
		return FALSE;
	}
	
	while ((color=LZWReadByte(fd,FALSE,c))>=0) 
	{
        curidx=(long)xpos+(long)ypos*(long)width;
        curidx*=3;     
        
		*(bigMemBuf+curidx)=cmap[0][color];
		*(bigMemBuf+curidx+1)=cmap[1][color];
		*(bigMemBuf+curidx+2)=cmap[2][color];				

		++xpos;
		if (xpos==width) 
		{
			xpos=0;
			if (interlace) 
			{
				switch (pass) 
				{
				case 0:
				case 1:
					ypos+=8; break;
				case 2:
					ypos+=4; break;
				case 3:
					ypos+=2; break;
				}

				if (ypos>=height) 
				{
					++pass;
					switch (pass) 
					{
					case 1: ypos=4;break;
					case 2: ypos=2;break;
					case 3: ypos=1;break;
					default : goto fini;
					}
				}
			} 
			else 
			{
				++ypos;
			}
		}
		if (ypos >=height)
			break;
	}

fini :

	ASSERT(LZWReadByte(fd,FALSE,c)>=0);

	return TRUE;
}

                                                                         
////////////
//
//	GIF writing section
//
////////////

/*
 * Bump the 'curx' and 'cury' to point to the next pixel
 */
void CGif::BumpPixel()
{
    /*
     * Bump the current X position
     */
    ++curx;

    if( curx == Width ) {
        curx = 0;
		++cury;
    }
}

/*******************************************************************************
* Return the next pixel from the image
*******************************************************************************/

int CGif::GIFNextPixel( )
{
    unsigned long index;
    int r;
    
    if( CountDown == 0 )
        return EOF;

    --CountDown;
    
    index= (unsigned long)curx + (unsigned long)cury * (unsigned long)Width;
    
	r = *(buffer+index);

    BumpPixel();

    return r;
}

/*******************************************************************************
*	here's the entry point. 
*	file ptr, screen width, height, background color, bits per pixel and
*	arrays of color values (0-255)
*******************************************************************************/
BOOL CGif::WriteGIFFile(LPCTSTR lpstrFileName,
							BYTE* buf,
							int GWidth, 
							int GHeight,
							int BackGround,
							int Red[], int Green[], int Blue[])
{                       
	FILE *fp;
	int B;
	int RWidth, RHeight;
	int LeftOfs, TopOfs;
	int Resolution;
	int ColorMapSize;
	int InitCodeSize;
	int i;
	int BitsPerPixel = 8;

	fp=fopen(lpstrFileName,"wb");
	if (fp==NULL) 
	{
		m_strGIFError="Can't open GIF for writing";
		return FALSE;
	}
	
	ColorMapSize = 1 << BitsPerPixel;

	buffer=buf;

	RWidth = Width = GWidth;
	RHeight = Height = GHeight;
	LeftOfs = TopOfs = 0;

	cur_accum = 0;
	cur_bits = 0;

	Resolution = BitsPerPixel;

	CountDown = (long)Width * (long) Height;

	if (BitsPerPixel <=1)
	InitCodeSize=2;
	else
	InitCodeSize = BitsPerPixel;

	curx = cury =0;

	fwrite("GIF87a",1,6,fp);

	Putword(RWidth,fp);
	Putword(RHeight,fp);

	B=0x80;
	
	B |=(Resolution -1) << 5;

	B |=(BitsPerPixel - 1);

	fputc(B,fp);

	fputc(BackGround,fp);
	
	fputc(0,fp);

	for(i=0; i<ColorMapSize; ++i) 
	{
		fputc(Red[i],fp);
		fputc(Green[i],fp);
		fputc(Blue[i],fp);
	}

	fputc(',',fp);

	Putword(LeftOfs,fp);
	Putword(TopOfs,fp);
	Putword(Width,fp);
	Putword(Height,fp);

	fputc(0x00,fp);

    /*
     * Write out the initial code size
     */
    fputc( InitCodeSize, fp );
    /*
     * Go and actually compress the data
     */

    compress(  InitCodeSize+1, fp);

    /*
     * Write out a Zero-length packet (to end the series)
     */
    fputc( 0, fp );

    /*
     * Write the GIF file terminator
     */
    fputc( ';', fp );

    /*
     * And close the file
     */
    fclose( fp );

	return TRUE;
}

/*******************************************************************************
 * Write out a word to the GIF file
*******************************************************************************/

void CGif::Putword(int w, FILE *fp )
{
    fputc( w & 0xff, fp );
    fputc( (w / 256) & 0xff, fp );
}


/***************************************************************************
 *
 *  GIF Image compression section
 *
 *  Lempel-Ziv compression based on 'compress'.  GIF modifications by
 *  David Rowley (mgardi@watdcsu.waterloo.edu)
 *
 ***************************************************************************/

void CGif::compress( int init_bits, FILE* outfile)
{
    register long fcode;
    register code_int i /* = 0 */;
    register int c;
    register code_int ent;
    register code_int disp;
    register int hshift;

    /*
     * Set up the globals:  g_init_bits - initial number of bits
     *                      g_outfile   - pointer to output file
     */
    g_init_bits = init_bits;
    g_outfile = outfile;

    /*
     * Set up the necessary values
     */
    clear_flg = 0;
    maxcode = MAXCODE(n_bits = g_init_bits);

    ClearCode = (1 << (init_bits - 1));
    EOFCode = ClearCode + 1;
    free_ent = ClearCode + 2;

    char_init();

    ent = GIFNextPixel( );

    hshift = 0;
    for ( fcode = (long) HSIZE;  fcode < 65536L; fcode *= 2L )
        ++hshift;
    hshift = 8 - hshift;                /* set hash code range bound */

    cl_hash( (count_int) HSIZE);            /* clear hash table */

    output( (code_int)ClearCode );

    while ( (c = GIFNextPixel( )) != EOF ) 
	{	/* } */

        fcode = (long) (((long) c << maxbits) + ent);
        i = (((code_int)c << hshift) ^ ent);    /* xor hashing */

        if ( htab[i] == fcode ) {
            ent = codetab[i];
            continue;
        } else if ( (long)htab[i] < 0 )      /* empty slot */
            goto nomatch;
        disp = HSIZE - i;           /* secondary hash (after G. Knott) */
        if ( i == 0 )
            disp = 1;
probe:
        if ( (i -= disp) < 0 )
            i += HSIZE;

        if ( htab[i] == fcode ) {
            ent = codetab[i];
            continue;
        }
        if ( (long)htab[i] > 0 )
            goto probe;
nomatch:
        output ( (code_int) ent );
        ent = c;
        if ( free_ent < maxmaxcode ) 
		{	/* } */
            codetab[i] = free_ent++; /* code -> hashtable */
            htab[i] = fcode;
        } else
                cl_block();
    }
    /*
     * Put out the final code.
     */
    output( (code_int)ent );
    output( (code_int) EOFCode );
}

/*****************************************************************
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 *      code:   A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *              that n_bits =< (long)wordsize - 1.
 * Outputs:
 *      Outputs code to the file.
 * Assumptions:
 *      Chars are 8 bits long.
 * Algorithm:
 *      Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).  Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

void CGif::output( code_int  code)
{
    cur_accum &= masks[ cur_bits ];

    if( cur_bits > 0 )
        cur_accum |= ((long)code << cur_bits);
    else
        cur_accum = code;

    cur_bits += n_bits;

    while( cur_bits >= 8 ) 
	{
        char_out( (unsigned int)(cur_accum & 0xff) );
        cur_accum >>= 8;
        cur_bits -= 8;
    }

    /*
     * If the next entry is going to be too big for the code size,
     * then increase it, if possible.
     */
   if ( free_ent > maxcode || clear_flg ) 
   {
        if( clear_flg ) 
		{
            maxcode = MAXCODE (n_bits = g_init_bits);
            clear_flg = 0;
        } 
		else 
		{
            ++n_bits;
            if ( n_bits == maxbits )
                maxcode = maxmaxcode;
            else
                maxcode = MAXCODE(n_bits);
        }
    }
	
	if( code == EOFCode ) 
	{
        /*
         * At EOF, write the rest of the buffer.
         */
        while( cur_bits > 0 ) 
		{
            char_out( (unsigned int)(cur_accum & 0xff) );
            cur_accum >>= 8;
            cur_bits -= 8;
        }
	
        flush_char();
	
        fflush( g_outfile );
	
        if( ferror( g_outfile ) ) 
		{
			AfxMessageBox("Write Error in GIF file",MB_OK);
		}
    }
}

void CGif::cl_block()
{
	cl_hash((count_int)HSIZE);
	free_ent=ClearCode+2;
	clear_flg=1;

	output((code_int)ClearCode);
}

void CGif::cl_hash(register count_int hsize)

{
	register count_int *htab_p = htab+hsize;

	register long i;
	register long m1 = -1L;

	i = hsize - 16;

	do 
	{
		*(htab_p-16)=m1;
		*(htab_p-15)=m1;
		*(htab_p-14)=m1;
		*(htab_p-13)=m1;
		*(htab_p-12)=m1;
		*(htab_p-11)=m1;
		*(htab_p-10)=m1;
		*(htab_p-9)=m1;
		*(htab_p-8)=m1;
		*(htab_p-7)=m1;
		*(htab_p-6)=m1;
		*(htab_p-5)=m1;
		*(htab_p-4)=m1;
		*(htab_p-3)=m1;
		*(htab_p-2)=m1;
		*(htab_p-1)=m1;
		
		htab_p-=16;
	} while ((i-=16) >=0);

	for (i+=16;i>0;--i)
		*--htab_p=m1;
}

/*******************************************************************************
*	GIF specific
*******************************************************************************/

void CGif::char_init()
{
	a_count=0;
}

void CGif::char_out(int c)
{
	accum[a_count++]=c;
	if (a_count >=254)
		flush_char();
}

void CGif::flush_char()
{
	if (a_count > 0) 
	{
		fputc(a_count,g_outfile);
		fwrite(accum,1,a_count,g_outfile);
		a_count=0;
	}
}

//
// copies BYTE buffer into DWORD-aligned BYTE buffer
// return addr of new buffer
//
BYTE* CGif::MakeDwordAlign(BYTE *dataBuf, UINT widthPix, UINT height, UINT *uiOutWidthBytes)		// bytes!!!
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
	if (pNew==NULL) 
	{
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
BOOL CGif::VertFlipBuf(BYTE  * inbuf, UINT widthBytes, UINT height)
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
BOOL CGif::BGRFromRGB(BYTE *buf, UINT widthPix, UINT height)
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

BYTE* CGif::ClearDwordAlign(BYTE *inBuf, UINT widthPix, UINT height, UINT nBpp)
{
	if (inBuf==NULL)
		return FALSE;


	BYTE *tmp;
	tmp=(BYTE *)new BYTE[height * widthPix * nBpp/8];
	if (tmp==NULL)
		return NULL;

	UINT row;

	for (row=0;row<height;row++) 
	{
		memcpy((tmp+row * widthPix * nBpp/8), 
				(inBuf + row * WIDTHBYTES(widthPix * nBpp)), 
				widthPix * nBpp/8);
	}

	return tmp;
}

