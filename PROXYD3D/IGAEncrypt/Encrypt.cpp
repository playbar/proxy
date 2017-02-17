
#include "stdafx.h"
#include "Encrypt.h"
#include "Des\ndes.h"

#ifdef _DEBUG
   #pragma comment(lib,".\\Des\\Debug\\Des.lib")
#else
   #pragma comment(lib,".\\Des\\Release\\Des.lib")
#endif

/************************************************************************/
/*  Base64编码加密                                                      */
/************************************************************************/

KBase64Coding::KBase64Coding ()
{
}

unsigned int KBase64Coding::CalculateRecquiredEncodeOutputBufferSize (unsigned int p_InputByteCount)
{
	div_t result = div (p_InputByteCount, 3);

	unsigned int RecquiredBytes = 0;
	if (result.rem == 0)
		{
		// Number of encoded characters
		RecquiredBytes = result.quot * 4;

		// CRLF -> "\r\n" each 76 characters
		result = div (RecquiredBytes, 76);
		RecquiredBytes += result.quot * 2;

		// Terminating null for the Encoded String
		RecquiredBytes += 1;

		return RecquiredBytes;
		}
	else
		{
		// Number of encoded characters
		RecquiredBytes = result.quot * 4 + 4;

		// CRLF -> "\r\n" each 76 characters
		result = div (RecquiredBytes, 76);
		RecquiredBytes += result.quot * 2;

		// Terminating null for the Encoded String
		RecquiredBytes += 1;

		return RecquiredBytes;
		}
}

unsigned int KBase64Coding::CalculateRecquiredDecodeOutputBufferSize (char* p_pInputBufferString)
{
	unsigned int BufferLength = strlen (p_pInputBufferString);

	div_t result = div (BufferLength, 4);

	if (p_pInputBufferString [BufferLength - 1] != BASE64_DEFAULT_FILL_CODE)
		{
		return result.quot * 3;
		}
	else
		{
		if (p_pInputBufferString [BufferLength - 2] == BASE64_DEFAULT_FILL_CODE)
			{
			return result.quot * 3 - 2;
			}
		else
			{
			return result.quot * 3 - 1;
			}
		}
}

void KBase64Coding::EncodeByteTriple  (char* p_pInputBuffer, unsigned int InputCharacters, char* p_pOutputBuffer)
{
	unsigned int mask = 0xfc000000;
	unsigned int buffer = 0;


	char* temp = (char*) &buffer;
	temp [3] = p_pInputBuffer [0];
	if (InputCharacters > 1)
		temp [2] = p_pInputBuffer [1];
	if (InputCharacters > 2)
		temp [1] = p_pInputBuffer [2];

	switch (InputCharacters)
		{
		case 3:
			{
			p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [2] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [3] = BASE64_ALPHABET [(buffer & mask) >> 26];
			break;
			}
		case 2:
			{
			p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [2] = BASE64_ALPHABET [(buffer & mask) >> 26];
			p_pOutputBuffer [3] = BASE64_DEFAULT_FILL_CODE;
			break;
			}
		case 1:
			{
			p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
			buffer = buffer << 6;
			p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
			p_pOutputBuffer [2] = BASE64_DEFAULT_FILL_CODE;
			p_pOutputBuffer [3] = BASE64_DEFAULT_FILL_CODE;
			break;
			}
		}
}

unsigned int KBase64Coding::DecodeByteQuartet (char* p_pInputBuffer, char* p_pOutputBuffer)
{
	unsigned int buffer = 0;

	if (p_pInputBuffer[3] == BASE64_DEFAULT_FILL_CODE)
		{
		if (p_pInputBuffer[2] == BASE64_DEFAULT_FILL_CODE)
			{
			buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
			buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
			buffer = buffer << 14;

			char* temp = (char*) &buffer;
			p_pOutputBuffer [0] = temp [3];
			
			return 1;
			}
		else
			{
			buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
			buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
			buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[2]]) << 6;
			buffer = buffer << 8;

			char* temp = (char*) &buffer;
			p_pOutputBuffer [0] = temp [3];
			p_pOutputBuffer [1] = temp [2];
			
			return 2;
			}
		}
	else
		{
		buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
		buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
		buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[2]]) << 6;
		buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[3]]) << 6; 
		buffer = buffer << 2;

		char* temp = (char*) &buffer;
		p_pOutputBuffer [0] = temp [3];
		p_pOutputBuffer [1] = temp [2];
		p_pOutputBuffer [2] = temp [1];

		return 3;
		}

	return -1;
}

void KBase64Coding::EncodeBuffer (char* p_pInputBuffer, unsigned int p_InputBufferLength, char* p_pOutputBufferString)
{
	unsigned int FinishedByteQuartetsPerLine = 0;
	unsigned int InputBufferIndex  = 0;
	unsigned int OutputBufferIndex = 0;

	memset (p_pOutputBufferString, 0, CalculateRecquiredEncodeOutputBufferSize (p_InputBufferLength));

	while (InputBufferIndex < p_InputBufferLength)
		{
		if (p_InputBufferLength - InputBufferIndex <= 2)
			{
			FinishedByteQuartetsPerLine ++;
			EncodeByteTriple (p_pInputBuffer + InputBufferIndex, p_InputBufferLength - InputBufferIndex, p_pOutputBufferString + OutputBufferIndex);
			break;
			}
		else
			{
			FinishedByteQuartetsPerLine++;
			EncodeByteTriple (p_pInputBuffer + InputBufferIndex, 3, p_pOutputBufferString + OutputBufferIndex);
			InputBufferIndex  += 3;
			OutputBufferIndex += 4;
			}

		if (FinishedByteQuartetsPerLine == 19)
			{
			p_pOutputBufferString [OutputBufferIndex  ] = '\r';
			p_pOutputBufferString [OutputBufferIndex+1] = '\n';
			p_pOutputBufferString += 2;
			FinishedByteQuartetsPerLine = 0;
			}
		}
}

unsigned int KBase64Coding::DecodeBuffer (char* p_pInputBufferString, char* p_pOutputBuffer)
{
	unsigned int InputBufferIndex  = 0;
	unsigned int OutputBufferIndex = 0;
	unsigned int InputBufferLength = strlen (p_pInputBufferString);

	char ByteQuartet [4];

	while (InputBufferIndex < InputBufferLength)
		{
		for (int i = 0; i < 4; i++)
			{
			ByteQuartet [i] = p_pInputBufferString [InputBufferIndex];

			// Ignore all characters except the ones in BASE64_ALPHABET
			if ((ByteQuartet [i] >= 48 && ByteQuartet [i] <=  57) ||
				(ByteQuartet [i] >= 65 && ByteQuartet [i] <=  90) ||
				(ByteQuartet [i] >= 97 && ByteQuartet [i] <= 122) ||
				 ByteQuartet [i] == '~' || ByteQuartet [i] == '/' || ByteQuartet [i] == BASE64_DEFAULT_FILL_CODE)
				{
				}
			else
				{
				// Invalid character
				i--;
				}

			InputBufferIndex++;
			}

		OutputBufferIndex += DecodeByteQuartet (ByteQuartet, p_pOutputBuffer + OutputBufferIndex);
		}

	// OutputBufferIndex gives us the next position of the next decoded character
	// inside our output buffer and thus represents the number of decoded characters
	// in our buffer.
	return OutputBufferIndex;
}

unsigned int KBase64Coding::CreateMatchingEncodingBuffer (unsigned int p_InputByteCount, char** p_ppEncodingBuffer)
{
	unsigned int Size = CalculateRecquiredEncodeOutputBufferSize (p_InputByteCount);
	(*p_ppEncodingBuffer) = (char*) malloc (Size);
	memset (*p_ppEncodingBuffer, 0, Size);
	return Size;
}

unsigned int KBase64Coding::CreateMatchingDecodingBuffer (char* p_pInputBufferString, char** p_ppDecodingBuffer)
{
	unsigned int Size = CalculateRecquiredDecodeOutputBufferSize (p_pInputBufferString);
	(*p_ppDecodingBuffer) = (char*) malloc (Size);
	memset (*p_ppDecodingBuffer, 0, Size);
	return Size;
}

unsigned int KBase64Coding::EncodeFile (char* p_pSourceFileName, char* p_pEncodedFileName)
{
	/*
	CFile InputFile;
	CFile OutputFile;

	if (!InputFile.Open (p_pSourceFileName, CFile::modeRead))
		return UNABLE_TO_OPEN_INPUT_FILE;

	if (!OutputFile.Open (p_pEncodedFileName, CFile::modeCreate|CFile::modeWrite))
		return UNABLE_TO_OPEN_OUTPUT_FILE;

	char InputBuffer [19 * 3];
	char* pOutputBuffer;
	CreateMatchingEncodingBuffer (sizeof (InputBuffer), &pOutputBuffer);

	if (pOutputBuffer == 0)
		return UNABLE_TO_CREATE_OUTPUTBUFFER;

	unsigned int ReadBytes = 0;
	while ((ReadBytes = InputFile.Read (InputBuffer, sizeof (InputBuffer))) != 0)
		{
		EncodeBuffer (InputBuffer, ReadBytes, pOutputBuffer);
		OutputFile.Write (pOutputBuffer, strlen (pOutputBuffer));
		}

	OutputFile.Flush ();
	OutputFile.Close ();
	InputFile.Close  ();
	*/
	return 0;
}

unsigned int KBase64Coding::DecodeFile (char* p_pSourceFileName, char* p_pDecodedFileName)
{
	/*
	CStdioFile	InputFile;
	CFile		OutputFile;

	if (!InputFile.Open (p_pSourceFileName, CFile::modeRead))
		return UNABLE_TO_OPEN_INPUT_FILE;

	if (!OutputFile.Open (p_pDecodedFileName, CFile::modeCreate|CFile::modeWrite))
		return UNABLE_TO_OPEN_OUTPUT_FILE;

	CString InputBuffer;
	char	OutputBuffer[64];

	unsigned int ReadBytes = 0;
	while ((ReadBytes = InputFile.ReadString (InputBuffer)) != 0)
		{
		InputBuffer.Remove ('\r');
		InputBuffer.Remove ('\n');
		unsigned int DecodedBytes = DecodeBuffer ((LPTSTR) (LPCTSTR) InputBuffer, OutputBuffer);
		OutputFile.Write (&OutputBuffer [0], DecodedBytes);
		}


	OutputFile.Flush ();
	OutputFile.Close ();
	InputFile.Close  ();
	*/
	return 0;
}




#define	DEFAULT_DES_KEY		(DEFAULT_COMMPANY_NAME PROJECT AUTHOR)


//////////////////////////////////////////////////////////////////////////
KEncrypt::KEncrypt()
{
	DEFINE_NAME(KEncrypt);
#if	defined(DES) || defined(USING_DES)
	::desinit( 0 );
	::setkey( DEFAULT_DES_KEY );
#endif

}

KEncrypt::~KEncrypt()
{
#if	defined(DES) || defined(USING_DES)
	::desdone();
#endif
}

INT KEncrypt::Encrypt(CHAR *pBuffer, DWORD dwLength)
{
	ASSERT(!::IsBadReadPtr(pBuffer, dwLength));
#if	defined(DES) || defined(USING_DES)
	::endes( pBuffer );
#else
#endif
	return	dwLength;
}

INT KEncrypt::Deencrypt(CHAR *pBuffer, DWORD dwLength)
{
#if	defined(DES) || defined(USING_DES)
	::dedes( pBuffer );
#else
#endif
	return	dwLength;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
UCHAR KMD5::s_szPADDING[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define MD5_S11 7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

#if PROTOTYPES
#	define PROTO_LIST(list) list
#else
#	define PROTO_LIST(list) ()
#endif

CHAR* MDString PROTO_LIST	((CHAR *));
CHAR* MDFile PROTO_LIST		((CHAR *));
CHAR* hmac_md5(CHAR* text, CHAR* key);

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) \
  { \
  (a) += F ((b), (c), (d)) + (x) + (UINT)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

#define GG(a, b, c, d, x, s, ac) \
  { \
  (a) += G ((b), (c), (d)) + (x) + (UINT)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

#define HH(a, b, c, d, x, s, ac) \
  { \
  (a) += H ((b), (c), (d)) + (x) + (UINT)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

#define II(a, b, c, d, x, s, ac) \
  { \
  (a) += I ((b), (c), (d)) + (x) + (UINT)(ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
  }

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL KMD5::GetMD5_128(CHAR *pszResult,/*INT nFileID*/HANDLE hFile,DWORD dwFileOff,DWORD dwFileLen)
{
	ASSERT(hFile != INVALID_HANDLE_VALUE);
	
	if(dwFileLen == 0 )
	{
		appZeroMemory(pszResult, 16);
		return	TRUE;
	}

	MD5_CONTEXT	context	= { 0 };
	this->MD5Init(&context);
	
	UCHAR	szDigest[16]			= { 0 };
	const	DWORD	BUFFERSIZE		= 1024;
	
	UCHAR	szBuffer[BUFFERSIZE]	= { 0 };	
	DWORD	dwCount					= 0L;
	DWORD	dwBytesRead				= 0L;
	
	while(dwFileLen > BUFFERSIZE)
	{
		::SetFilePointer(hFile, BUFFERSIZE * dwCount + dwFileOff, NULL, FILE_BEGIN);
		if(::ReadFile(hFile, szBuffer, BUFFERSIZE, &dwBytesRead, NULL))
		{	
			ASSERT(dwBytesRead == BUFFERSIZE);
			this->MD5Update(&context, szBuffer, dwBytesRead);
			dwFileLen -= dwBytesRead;
			++dwCount;
		}
		else
		{
			break;
		}
	}
	::SetFilePointer(hFile, BUFFERSIZE * dwCount + dwFileOff, NULL, FILE_BEGIN);
	if(::ReadFile(hFile, szBuffer, dwFileLen, &dwBytesRead, NULL))
	{
		this->MD5Update(&context, szBuffer, dwBytesRead);
		this->MD5Final(szDigest, &context);
		appCopyMemory(pszResult, (CHAR*)szDigest, sizeof(szDigest));
		return	TRUE;
	}
	return	FALSE;
}

//------------------
BOOL KMD5::GetMD5_128(CHAR *pszResult,const CHAR *lpszSourceBuff,DWORD dwSourceBuffLen)
{
	if(dwSourceBuffLen == 0)
	{
		appZeroMemory(pszResult, 16);
		return	TRUE;
	}
	MD5_CONTEXT	context		= { 0 };
	UCHAR	szDigest[16]	= { 0 };
	const	DWORD	BUFFERSIZE		= 1024;
	this->MD5Init(&context);
	this->MD5Update(&context, (UCHAR*)lpszSourceBuff, dwSourceBuffLen);
	this->MD5Final(szDigest, &context);
	appCopyMemory(pszResult, (CHAR*)szDigest, sizeof(szDigest));

	return	TRUE;
}

//------------------
bool KMD5::VerifyMD5_128(const CHAR *pcVerifyBuff,INT iFileID,INT iFileOff,INT iFileLen)
{
//	if (iFileLen==0) 
//		return(true);
//	CHAR acBuff[16];
//	GetMD5_128(acBuff,iFileID,iFileOff,iFileLen);
//	if (memcmp(pcVerifyBuff,acBuff,16))
//		return(false);
	return(true);
}

//------------------
bool KMD5::VerifyMD5_128(const CHAR *pcVerifyBuff,const CHAR *pcSourceBuff,INT iSourceBuffLen)
{
	if (iSourceBuffLen==0) 
		return(true);
	CHAR acBuff[16];
	GetMD5_128(acBuff,pcSourceBuff,iSourceBuffLen);
	if (memcmp(pcVerifyBuff,acBuff,16))
		return(false);
	return(true);
}

//------------------
//MD5内部实现函数
void KMD5::MD5Init (MD5_CONTEXT *context)
{
	context->count[0] = context->count[1] = 0;
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

//------------------
void KMD5::MD5Update (MD5_CONTEXT *context, UCHAR *input,UINT inputLen)
{
	UINT i, index, partLen;
	
	index = (UINT)((context->count[0] >> 3) & 0x3F);
	
	if ((context->count[0] += ((UINT)inputLen << 3)) < ((UINT)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((UINT)inputLen >> 29);
	
	partLen = 64 - index;
	
	if (inputLen >= partLen)
    {
		MD5_memcpy((UCHAR*)&context->buffer[index], (UCHAR*)input, partLen);
		MD5Transform (context->state, context->buffer);
		
		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (context->state, &input[i]);
		
		index = 0;
    }
	else
		i = 0;
	
	MD5_memcpy((UCHAR*)&context->buffer[index], (UCHAR*)&input[i],inputLen-i);
}

//------------------
void KMD5::MD5Final (UCHAR digest[16], MD5_CONTEXT *context)
{
	UCHAR bits[8];
	UINT index, padLen;
	
	Encode (bits, context->count, 8);
	
	index	= (UINT)((context->count[0] >> 3) & 0x3f);
	padLen	= (index < 56) ? (56 - index) : (120 - index);
	MD5Update (context,(UCHAR*) s_szPADDING, padLen);
	
	MD5Update (context, bits, 8);
	Encode (digest, context->state, 16);
	
	MD5_memset ((UCHAR*)context, 0, sizeof (*context));
}

//------------------
void KMD5::MD5Transform (UINT state[4],UCHAR block[64])
{
	INT i=0;
	
	UINT a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	
	Decode (x, block, 64);
	
	//Round 1
	FF (a, b, c, d, x[ 0], MD5_S11, 0xd76aa478); // 1
	FF (d, a, b, c, x[ 1], MD5_S12, 0xe8c7b756); // 2
	FF (c, d, a, b, x[ 2], MD5_S13, 0x242070db); // 3
	FF (b, c, d, a, x[ 3], MD5_S14, 0xc1bdceee); // 4
	FF (a, b, c, d, x[ 4], MD5_S11, 0xf57c0faf); // 5
	FF (d, a, b, c, x[ 5], MD5_S12, 0x4787c62a); // 6
	FF (c, d, a, b, x[ 6], MD5_S13, 0xa8304613); // 7
	FF (b, c, d, a, x[ 7], MD5_S14, 0xfd469501); // 8
	FF (a, b, c, d, x[ 8], MD5_S11, 0x698098d8); // 9
	FF (d, a, b, c, x[ 9], MD5_S12, 0x8b44f7af); // 10
	FF (c, d, a, b, x[10], MD5_S13, 0xffff5bb1); // 11
	FF (b, c, d, a, x[11], MD5_S14, 0x895cd7be); // 12
	FF (a, b, c, d, x[12], MD5_S11, 0x6b901122); // 13
	FF (d, a, b, c, x[13], MD5_S12, 0xfd987193); // 14
	FF (c, d, a, b, x[14], MD5_S13, 0xa679438e); // 15
	FF (b, c, d, a, x[15], MD5_S14, 0x49b40821); // 16
	
	//Round 2
	GG (a, b, c, d, x[ 1], MD5_S21, 0xf61e2562); // 17
	GG (d, a, b, c, x[ 6], MD5_S22, 0xc040b340); // 18
	GG (c, d, a, b, x[11], MD5_S23, 0x265e5a51); // 19
	GG (b, c, d, a, x[ 0], MD5_S24, 0xe9b6c7aa); // 20
	GG (a, b, c, d, x[ 5], MD5_S21, 0xd62f105d); // 21
	GG (d, a, b, c, x[10], MD5_S22,  0x2441453); // 22
	GG (c, d, a, b, x[15], MD5_S23, 0xd8a1e681); // 23
	GG (b, c, d, a, x[ 4], MD5_S24, 0xe7d3fbc8); // 24
	GG (a, b, c, d, x[ 9], MD5_S21, 0x21e1cde6); // 25
	GG (d, a, b, c, x[14], MD5_S22, 0xc33707d6); // 26
	GG (c, d, a, b, x[ 3], MD5_S23, 0xf4d50d87); // 27
	GG (b, c, d, a, x[ 8], MD5_S24, 0x455a14ed); // 28
	GG (a, b, c, d, x[13], MD5_S21, 0xa9e3e905); // 29
	GG (d, a, b, c, x[ 2], MD5_S22, 0xfcefa3f8); // 30
	GG (c, d, a, b, x[ 7], MD5_S23, 0x676f02d9); // 31
	GG (b, c, d, a, x[12], MD5_S24, 0x8d2a4c8a); // 32
	
	//Round 3
	HH (a, b, c, d, x[ 5], MD5_S31, 0xfffa3942); // 33
	HH (d, a, b, c, x[ 8], MD5_S32, 0x8771f681); // 34
	HH (c, d, a, b, x[11], MD5_S33, 0x6d9d6122); // 35
	HH (b, c, d, a, x[14], MD5_S34, 0xfde5380c); // 36
	HH (a, b, c, d, x[ 1], MD5_S31, 0xa4beea44); // 37
	HH (d, a, b, c, x[ 4], MD5_S32, 0x4bdecfa9); // 38
	HH (c, d, a, b, x[ 7], MD5_S33, 0xf6bb4b60); // 39
	HH (b, c, d, a, x[10], MD5_S34, 0xbebfbc70); // 40
	HH (a, b, c, d, x[13], MD5_S31, 0x289b7ec6); // 41
	HH (d, a, b, c, x[ 0], MD5_S32, 0xeaa127fa); // 42
	HH (c, d, a, b, x[ 3], MD5_S33, 0xd4ef3085); // 43
	HH (b, c, d, a, x[ 6], MD5_S34,  0x4881d05); // 44
	HH (a, b, c, d, x[ 9], MD5_S31, 0xd9d4d039); // 45
	HH (d, a, b, c, x[12], MD5_S32, 0xe6db99e5); // 46
	HH (c, d, a, b, x[15], MD5_S33, 0x1fa27cf8); // 47
	HH (b, c, d, a, x[ 2], MD5_S34, 0xc4ac5665); // 48
	
	//Round 4
	II (a, b, c, d, x[ 0], MD5_S41, 0xf4292244); // 49
	II (d, a, b, c, x[ 7], MD5_S42, 0x432aff97); // 50
	II (c, d, a, b, x[14], MD5_S43, 0xab9423a7); // 51
	II (b, c, d, a, x[ 5], MD5_S44, 0xfc93a039); // 52
	II (a, b, c, d, x[12], MD5_S41, 0x655b59c3); // 53
	II (d, a, b, c, x[ 3], MD5_S42, 0x8f0ccc92); // 54
	II (c, d, a, b, x[10], MD5_S43, 0xffeff47d); // 55
	II (b, c, d, a, x[ 1], MD5_S44, 0x85845dd1); // 56
	II (a, b, c, d, x[ 8], MD5_S41, 0x6fa87e4f); // 57
	II (d, a, b, c, x[15], MD5_S42, 0xfe2ce6e0); // 58
	II (c, d, a, b, x[ 6], MD5_S43, 0xa3014314); // 59
	II (b, c, d, a, x[13], MD5_S44, 0x4e0811a1); // 60
	II (a, b, c, d, x[ 4], MD5_S41, 0xf7537e82); // 61
	II (d, a, b, c, x[11], MD5_S42, 0xbd3af235); // 62
	II (c, d, a, b, x[ 2], MD5_S43, 0x2ad7d2bb); // 63
	II (b, c, d, a, x[ 9], MD5_S44, 0xeb86d391); // 64
	
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	
	MD5_memset ((UCHAR*)x, 0, sizeof (x));
}

//------------------
void KMD5::Encode (UCHAR *output,UINT *input,UINT len)
{
	UINT i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4)
    {
		output[j]	= (UCHAR)(input[i] & 0xff);
		output[j+1] = (UCHAR)((input[i] >> 8) & 0xff);
		output[j+2] = (UCHAR)((input[i] >> 16) & 0xff);
		output[j+3] = (UCHAR)((input[i] >> 24) & 0xff);
    }
}

//------------------
void KMD5::Decode (UINT *output,UCHAR *input,UINT len)
{
	UINT i, j;
	
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((UINT)input[j]) | (((UINT)input[j+1]) << 8) |
		(((UINT)input[j+2]) << 16) | (((UINT)input[j+3]) << 24);
}

//------------------
void KMD5::MD5_memcpy (UCHAR* output,UCHAR* input,UINT len)
{
	UINT i;
	
	for (i = 0; i < len; i++)
		output[i] = input[i];
}

//------------------
void KMD5::MD5_memset (UCHAR* output,INT value,UINT len)
{
	UINT i;
	
	for (i = 0; i < len; i++)
		((CHAR *)output)[i] = (CHAR)value;
}

