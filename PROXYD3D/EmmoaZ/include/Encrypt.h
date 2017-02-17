/**
 * Copyright 2007 by smallfool,
 * Room 303, Building 41, No 51, Boshan Road, Pudong District, Shanghai, China
 * All rights reserved.
 *
 * This software is the confidential and proprietary information of smallfool. 
 * Use is subject to license terms.
 *
 * Thanks to Dandan Huang.
 */
// Compression.h: interface for the KCompression class.
//
//////////////////////////////////////////////////////////////////////
//************************************************************************************************************
// purpose    :  整理修改
// Date       :  2008-6-4  10:01:04
// author     :  3o3afe1
//************************************************************************************************************
#if !defined(_ENCRYPT_H)
#define _ENCRYPT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ENCRYPT_DLL
    #pragma  message (" export encrypt dll. 3o3afe1")
    #define _ENCRYPT_API   __declspec(dllexport)
#else
    #pragma  message (" import encrypt dll. 3o3afe1")
    #define _ENCRYPT_API   __declspec(dllimport)
    #ifdef _DEBUG
          #pragma comment(lib,"..\\lib\\igaencryptd.lib") 
    #else
          #pragma comment(lib,"..\\lib\\igaencrypt.lib") 
    #endif
#endif



//////////////////////////////////////////////////////////////////////////
// Class wrapped for encryption 
//////////////////////////////////////////////////////////////////////////
class _ENCRYPT_API KEncrypt 
{
public:
	KEncrypt();
	virtual ~KEncrypt();
	
public:
	INT Encrypt(CHAR *pBuffer, DWORD dwLength);
	INT Deencrypt(CHAR *pBuffer, DWORD dwLength);
};

/************************************************************************/
/*  Base64编码加密                                                      */
/************************************************************************/

#define BASE64_DEFAULT_FILL_CODE	'-' // '='
const static unsigned int MAX_LINE_LENGTH = 76;

const static char BASE64_ALPHABET [64] = 
	{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', //   0 -   9
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', //  10 -  19
	'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', //  20 -  29
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', //  30 -  39
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', //  40 -  49
	'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', //  50 -  59
	'8', '9', '~', '/'								  //  60 -  63
	};

const static char BASE64_DEALPHABET [128] = 
	{
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //   0 -   9
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  10 -  19
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  20 -  29
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  30 -  39
	 0,  0,  0,  0,  0,  0,  0, 63, 52, 53, //  40 -  49
	54, 55, 56, 57, 58, 59, 60, 61,  0,  0, //  50 -  59
	 0, 0,  0,  0,  0,  0,  1,  2,  3,  4, //  60 -  69
	 5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //  70 -  79
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, //  80 -  89
	25,  0,  0,  0,  0,  0,  0, 26, 27, 28, //  90 -  99
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, // 100 - 109
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, // 110 - 119
	49, 50, 51,  0,  0,  0, 62,  0			// 120 - 127
	};

enum
	{
	UNABLE_TO_OPEN_INPUT_FILE,
	UNABLE_TO_OPEN_OUTPUT_FILE,
	UNABLE_TO_CREATE_OUTPUTBUFFER
	};

class _ENCRYPT_API KBase64Coding
{
public:
	KBase64Coding ();

	unsigned int CalculateRecquiredEncodeOutputBufferSize (unsigned int p_InputByteCount);
	unsigned int CalculateRecquiredDecodeOutputBufferSize (char* p_pInputBufferString);

	void			EncodeByteTriple  (char* p_pInputBuffer, unsigned int InputCharacters, char* p_pOutputBuffer);
	unsigned int	DecodeByteQuartet (char* p_pInputBuffer, char* p_pOutputBuffer);

	void			EncodeBuffer (char* p_pInputBuffer, unsigned int p_InputBufferLength, char*p_pOutputBufferString);
	unsigned int	DecodeBuffer (char* p_pInputBufferString, char* p_pOutputBuffer);

	unsigned int CreateMatchingEncodingBuffer (unsigned int p_InputByteCount, char** p_ppEncodingBuffer);
	unsigned int CreateMatchingDecodingBuffer (char* p_pInputBufferString, char** p_ppDecodingBuffer);

	unsigned int EncodeFile (char* p_pSourceFileName, char* p_pEncodedFileName);
	unsigned int DecodeFile (char* p_pSourceFileName, char* p_pDecodedFileName);

};


/************************************************************************/
/*                                                                      */
/************************************************************************/
#define	MD5_CONTEXT_STATE	4
#define	MD5_CONTEXT_COUNT	2
#define	MD5_CONTEXT_BUFFER	64

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef struct tagMD5_CONTEXT
{
	
	UINT	state[MD5_CONTEXT_STATE];		// state (ABCD)
	UINT	count[MD5_CONTEXT_COUNT];		// number of bits, modulo 2^64 (lsb first)
	UCHAR	buffer[MD5_CONTEXT_BUFFER];     // input buffer
} MD5_CONTEXT, *LPMD5_CONTEXT;


class _ENCRYPT_API KMD5 
{
public:
	enum
	{
		DEFAULT_DIGEST_SIZE	= 16,
	};
    KMD5();
    ~KMD5();
	
    BOOL GetMD5_128(CHAR *pszResult,HANDLE hFile, DWORD dwFileOffset, DWORD dwFileLen);
    BOOL GetMD5_128(CHAR *pszResult,const CHAR *lpszSourceBuff,DWORD dwSourceBuffLen);
    bool VerifyMD5_128(const CHAR *pszVerifyBuff,INT nFileID,INT nFileOff,INT iFileLen);
    bool VerifyMD5_128(const CHAR *pszVerifyBuff,const CHAR *pszSourceBuff,INT iSourceBuffLen);
	
private:
    UCHAR m_szDigest[DEFAULT_DIGEST_SIZE];
	
private:
    void MD5Init (MD5_CONTEXT *context);
    void MD5Update(MD5_CONTEXT *context, UCHAR *input,UINT inputLen);
    void MD5Transform  (UINT [4], UCHAR [64]) ;
    void Encode(UCHAR *, UINT *, UINT);
    void Decode (UINT *, UCHAR *, UINT);
    void MD5_memcpy(UCHAR*, UCHAR*, UINT);
    void MD5_memset(UCHAR*, INT, UINT);
    void MD5Final (UCHAR digest[16], MD5_CONTEXT *context);
private:
	static UCHAR	s_szPADDING[64];
};

#endif // !defined(_ENCRYPT_H)
