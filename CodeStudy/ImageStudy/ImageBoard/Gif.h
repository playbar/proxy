// Gif.h: interface for the CGif class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIF_H__82031240_001C_11D3_9481_000021003EA5__INCLUDED_)
#define AFX_GIF_H__82031240_001C_11D3_9481_000021003EA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

//////////////////////////////////////////////////////////////////
// constants
//
#define MAXCOLORMAPSIZE			256
#define CM_RED					0
#define CM_GREEN				1
#define CM_BLUE					2
#define MAX_LZW_BITS			12
#define BITS				    12
#define HSIZE					5003            /* 80% occupancy */
#define INTERLACE				0x40
#define LOCALCOLORMAP			0x80

//////////////////////////////////////////////////////////////////
// macros
//
#define BitSet(byte,bit)		(((byte) & (bit))==(bit))
#define ReadOK(file,buffer,len) (fread(buffer,len,1,file)!=0)
#define LM_to_uint(a,b)			(((b)<<8)|(a))
#define MAXCODE(n_bits)        (((code_int) 1 << (n_bits)) - 1)

//////////////////////////////////////////////////////////////////
// owner type
//
typedef short int				code_int;	/* was int */
typedef long int				count_int;
typedef unsigned char			pixval;
typedef unsigned char			char_type;

//////////////////////////////////////////////////////////////////
// structures
//
typedef struct tagGIFSCREEN
{
	unsigned int Width;
	unsigned int Height;
	unsigned char ColorMap[3][MAXCOLORMAPSIZE];
	unsigned int BitPixel;
	unsigned int ColorResolution;
	unsigned int BackGround;
	unsigned int AspectRatio;
} GIFSCREEN;

typedef struct tagGIF89
{
	int transparent;
	int delayTime;
	int inputFlag;
	int disposal;
} GIF89;

////////////////////////////////////////////////////////////
// class CGif
//
class AFX_EXT_CLASS CGif  
{
// constructor
public:
	CGif();
	CGif(CDib *pDib);
	virtual ~CGif();

// public functions
public:
	// Get CDib
	CDib * GetDib()
	{
		return m_pDib;
	}
	//Set CDib
	BOOL SetDib(CDib *pDib)
	{
		if (pDib == NULL)
			return FALSE;

		if (m_pDib != NULL)
			delete m_pDib;

		m_pDib = pDib->Clone();

		return (m_pDib != NULL);
	}
	// Load GIF file
	BOOL Load(LPCTSTR lpstrFileName);
	// save GIF file
	BOOL Save(LPCTSTR lpstrFileName, CDib* pDib = NULL);
	// get error string
	CString GetErrorString();

// internal functions
private:
	// read to RGB
	LPBYTE ReadGIFFile(LPCTSTR lpstrFileName, UINT *uWidth, UINT *uHeight);
	// write 256 RGB to file
	BOOL WriteGIFFile(LPCTSTR lpstrFileName, BYTE* buf, int GWidth, int GHeight,
					  int BackGround, int Red[], int Green[], int Blue[]);
	// free allocate memory
	void FreeBuffer(BYTE *Buffer);
	int	ReadColorMap (FILE *fd, int number, BYTE buffer[3][MAXCOLORMAPSIZE]);
	int	DoExtension (FILE *fd,int label);
	int	GetDataBlock (FILE *fd, BYTE *buf);
	int	GetCode (FILE *fd, int code_size, int flag);
	int	LZWReadByte (FILE *fd,int flag, int  input_code_size);
	BOOL ReadImage(FILE *fd, BYTE  * bigMemBuf, int width, int height,
				   BYTE cmap[3][MAXCOLORMAPSIZE], int interlace);
	void BumpPixel();
	int GIFNextPixel();
	void Putword(int w, FILE* fp);
	void compress(int init_bits, FILE* outfile);
	void output(code_int code);
	void cl_block();
	void cl_hash(count_int hsize);
	void writeerr();
	void char_init();
	void char_out(int c);
	void flush_char();
	BYTE* MakeDwordAlign(BYTE *dataBuf,			// input buf
						 UINT widthPix,				// input pixels
						 UINT height,				// lines
						 UINT *uiOutWidthBytes);	// new width bytes
	BYTE* ClearDwordAlign(BYTE *inBuf,				// input buf
						UINT widthPix,				// input size
						UINT height,
						UINT nBpp);
	BOOL VertFlipBuf(BYTE * inbuf,						// input buf
				   UINT widthBytes,							// input width bytes
				   UINT height);							// height
	BOOL BGRFromRGB(BYTE *buf,							// input buf
					UINT widthPix,								// width in pixels
					UINT height);								// lines

// priovate data
private:
	// attributes
	CDib*	m_pDib;

	// data
	int		ZeroDataBlock;
	CString m_strGIFError;

	int 			Width, Height;
	int				curx, cury;
	long 			CountDown;
	unsigned long	cur_accum;
	int				cur_bits;
	unsigned char	*buffer;

	int n_bits;                         /* number of bits/code */
	int maxbits;						/* user settable max # bits/code */
	code_int maxcode;                   /* maximum code, given n_bits */
	code_int maxmaxcode;				/* should NEVER generate this code */
	count_int htab [HSIZE];
	unsigned short codetab [HSIZE];
	code_int free_ent;                  /* first unused entry */
	int clear_flg;
	int g_init_bits;
	FILE* g_outfile;
	int ClearCode;
	int EOFCode;

	int a_count;
	char accum[256];
};

#endif // !defined(AFX_GIF_H__82031240_001C_11D3_9481_000021003EA5__INCLUDED_)
