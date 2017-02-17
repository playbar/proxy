
#include "DIBAPI.H"

typedef struct tagGIFHEADER
{
	BYTE bySignature[3];
	BYTE byVersion[3];
}  GIFHEADER;

typedef struct tagGIFSCRDESC
{
	WORD wWidth;
	WORD wDepth;
	struct tagGlobalFlag
	{
		BYTE PalBits   : 3;
		BYTE SortFlag  : 1;
		BYTE ColorRes  : 3;
		BYTE GlobalPal : 1;
	}  GlobalFlag;
	BYTE byBackground;
	BYTE byAspect;
}  GIFSCRDESC;

typedef struct tagGIFIMAGE
{
	WORD wLeft;
	WORD wTop;
	WORD wWidth;
	WORD wDepth;
	struct tagLocalFlag
	{
		BYTE PalBits   : 3;
		BYTE Reserved  : 2;
		BYTE SortFlag  : 1;
		BYTE Interlace : 1;
		BYTE LocalPal  : 1;
	}  LocalFlag;
}  GIFIMAGE;

typedef struct tagGIFCONTROL
{
	BYTE byBlockSize;
	struct tagFlag
	{
		BYTE Transparency   : 1;
		BYTE UserInput      : 1;
		BYTE DisposalMethod : 3;
		BYTE Reserved       : 3;
	}  Flag;
	WORD wDelayTime;
	BYTE byTransparencyIndex;
	BYTE byTerminator;
}  GIFCONTROL;

typedef struct tagGIFPLAINTEXT
{
	BYTE byBlockSize;
	WORD wTextGridLeft;
	WORD wTextGridTop;
	WORD wTextGridWidth;
	WORD wTextGridDepth;
	BYTE byCharCellWidth;
	BYTE byCharCellDepth;
	BYTE byForeColorIndex;
	BYTE byBackColorIndex;
}  GIFPLAINTEXT;

typedef struct tagGIFAPPLICATION
{
	BYTE byBlockSize;
	BYTE byIdentifier[8];
	BYTE byAuthentication[3];
}  GIFAPPLICATION;

typedef struct tagGIFDATAPARAM
{
	LPSTR lpDataBuff;
	LPSTR lpBgnBuff;
	LPSTR lpEndBuff;
	DWORD dwDataLen;
	WORD  wMemLen;
	WORD  wWidth;
	WORD  wDepth;
	WORD  wLineBytes;
	WORD  wBits;
	BOOL  bEOF;
	BOOL  bInterlace;
}  GIFDATAPARAM;
typedef GIFDATAPARAM FAR *LPGIFDATAPARAM;

typedef struct tagGIFCONTROLPARAM
   {
      LPSTR lpDataBuff;
      LPSTR lpEndBuff;
      DWORD dwTempCode;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      WORD  wByteCnt;
      WORD  wBlockNdx;
      BYTE  byLeftBits;
   }  GIFCONTROLPARAM;
typedef GIFCONTROLPARAM FAR *LPGIFCONTROLPARAM;


// 宏运算
#define DWORD_WBYTES(x)         ( (((x) + 31UL) >> 5) << 2 )
#define WORD_WBYTES(x)          ( (((x) + 15UL) >> 4) << 1 )
#define BYTE_WBYTES(x)          (  ((x) +  7UL) >> 3       )

//常量
#define MAX_BUFF_SIZE			32768 /* 32K */
#define MAX_HASH_SIZE           5051
#define MAX_TABLE_SIZE          4096 /* 12-bit */
#define MAX_SUBBLOCK_SIZE       255

// 函数原型
BOOL SaveGIF(LPCSTR lpszFileName, LPSTR lpDIB, BOOL bInterlace);
void EncodeGIF(CFile& file, LPSTR lpDIBBits, 
			  LPGIFCONTROLPARAM lpGIFControlParam,
			  WORD wWidthBytes, BOOL bInterlace);
void WriteCodeGIF(CFile& file, WORD wCode, LPSTR lpSubBlock,
				  LPBYTE lpbyCurrentBits, LPGIFCONTROLPARAM lpGIFControlParam);

HDIB ReadGIF(LPCSTR lpszFileName);
void ReadSrcDataGIF(CFile& file, LPWORD lpwMemLen, LPDWORD lpdwDataLen,
                    LPSTR lpSrcBuff, LPBOOL lpbEOF);
void DecodeGIF(CFile& file, LPSTR lpDIBBits,
			  LPGIFDATAPARAM lpGIFDataParam,WORD wWidthBytes);

