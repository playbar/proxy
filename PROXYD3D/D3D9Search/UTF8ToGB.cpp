
#include "dxstdafx.h"
#include "string.h"
using namespace std;

#include "UTF8ToGB.h"

char* HexToBin(const char * string)//将16进制数转换成2进制
{
	if( stricmp(string, "0")) 
		return "0000";
	if( stricmp(string,"1")) 
		return "0001";
	if( stricmp(string,"2") ) 
		return "0010";
	if( stricmp(string, "3") ) 
		return "0011";
	if( stricmp(string,"4") ) 
		return "0100";
	if( stricmp(string,"5") ) 
		return "0101";
	if( stricmp(string,"6") ) 
		return "0110";
	if( stricmp(string,"7") ) 
		return "0111";
	if( stricmp(string,"8") ) 
		return "1000";
	if( stricmp(string,"9") )  
		return "1001";
	if( stricmp(string,"a") ) 
		return "1010";
	if( stricmp(string,"b") ) 
		return "1011";
	if( stricmp(string,"c") ) 
		return "1100";
	if( stricmp(string,"d") ) 
		return "1101";
	if( stricmp(string,"e") )  
		return "1110";
	if( stricmp(string,"f") ) 
		return "1111";
	return "";
}


char* BinToHex(const char * BinString)//将2进制数转换成16进制
{
	if( stricmp(BinString, "0000") )
		return "0";
	if( stricmp(BinString, "0001"))
		return "1";
	if( stricmp(BinString, "0010"))
		return "2";
	if( stricmp(BinString, "0011"))
		return "3";
	if( stricmp(BinString, "0100"))
		return "4";
	if( stricmp(BinString, "0101"))
		return "5";
	if( stricmp(BinString, "0110"))
		return "6";
	if( stricmp(BinString, "0111"))
		return "7";
	if( stricmp(BinString, "1000"))
		return "8";
	if( stricmp(BinString, "1001"))
		return "9";
	if( stricmp(BinString, "1010"))
		return "a";
	if( stricmp(BinString, "1011"))
		return "b";
	if( stricmp(BinString, "1100"))
		return "c";
	if( stricmp(BinString, "1101"))
		return "d";
	if( stricmp(BinString, "1110"))
		return "e";
	if( stricmp(BinString, "1111"))
		return "f";
	return "";
}

int BinToInt(const char * string)//2进制字符数据转换成10进制整型
{
	int len =0;
	int tempInt = 0;
	int strInt = 0;
	for(int i =0 ;i < strlen(string) ;i ++)
	{
		tempInt = 1;
		strInt = (int)string[i]-48;
		for(int k =0 ;k < 7-i ; k++)
		{
			tempInt = 2*tempInt;
		}
		len += tempInt*strInt;
	}
	return len;
} 

//
//WCHAR* UTF_8ToUnicode(char *ustart)  //把UTF-8转换成Unicode
//{
//	char char_one;
//	char char_two;
//	char char_three;
//	int Hchar;
//	int Lchar;
//	char uchar[2];
//	WCHAR *unicode;
//	CString string_one;
//	CString string_two;
//	CString string_three;
//	CString combiString;
//	char_one = *ustart;
//	char_two = *(ustart+1);
//	char_three = *(ustart+2);
//	string_one.Format("%x",char_one);
//	string_two.Format("%x",char_two);
//	string_three.Format("%x",char_three);
//	string_three = string_three.Right(2);
//	string_two = string_two.Right(2);
//	string_one = string_one.Right(2);
//	string_three = HexToBin(string_three.Left(1))+HexToBin(string_three.Right(1));
//	string_two = HexToBin(string_two.Left(1))+HexToBin(string_two.Right(1));
//	string_one = HexToBin(string_one.Left(1))+HexToBin(string_one.Right(1));
//	combiString = string_one +string_two +string_three;
//	combiString = combiString.Right(20);
//	combiString.Delete(4,2);
//	combiString.Delete(10,2);
//	Hchar = BinToInt(combiString.Left(8));
//	Lchar = BinToInt(combiString.Right(8));
//	uchar[1] = (char)Hchar;
//	uchar[0] = (char)Lchar;
//	unicode = (WCHAR *)uchar;
//	return unicode;
//}



int UTF2Uni(const char* src, wchar_t *des )
{
	if (src == NULL  || des == NULL ) 
	{
		return -1;
	}

	int size_s = strlen(src);
	int size_d = size_s + 20;          //?

	//memset(des, 0, size_d * sizeof(wchar_t));

	int s = 0, d = 0;
	bool toomuchbyte = true; //set true to skip error prefix.

	while (s < size_s && d < size_d)
	{
		unsigned char c = src[s];
		if ((c & 0x80) == 0) 
		{
			des[d++] += src[s++];
		} 
		else if((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			WCHAR &wideChar = des[d++];
			wideChar  = (src[s + 0] & 0x3F) << 6;
			wideChar |= (src[s + 1] & 0x3F);

			s += 2;
		}
		else if((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			WCHAR &wideChar = des[d++];

			wideChar  = (src[s + 0] & 0x1F) << 12;
			wideChar |= (src[s + 1] & 0x3F) << 6;
			wideChar |= (src[s + 2] & 0x3F);

			s += 3;
		} 
		else if((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			WCHAR &wideChar = des[d++];

			wideChar  = (src[s + 0] & 0x0F) << 18;
			wideChar  = (src[s + 1] & 0x3F) << 12;
			wideChar |= (src[s + 2] & 0x3F) << 6;
			wideChar |= (src[s + 3] & 0x3F);

			s += 4;
		} 
		else 
		{
			WCHAR &wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 

			wideChar  = (src[s + 0] & 0x07) << 24;
			wideChar  = (src[s + 1] & 0x3F) << 18;
			wideChar  = (src[s + 2] & 0x3F) << 12;
			wideChar |= (src[s + 3] & 0x3F) << 6;
			wideChar |= (src[s + 4] & 0x3F);

			s += 5;
		}
	}

	return 0;
}



int Uni2UTF(wchar_t wchar, char *utf8)
{
	if (utf8 == NULL) {
		return -1;
	}
	int len = 0;
	int size_d = 8;

	if (wchar < 0x80)
	{  //
		//length = 1;
		utf8[len++] = (char)wchar;
	}
	else if(wchar < 0x800)
	{
		//length = 2;

		if (len + 1 >= size_d)
			return -1;

		utf8[len++] = 0xc0 | ( wchar >> 6 );
		utf8[len++] = 0x80 | ( wchar & 0x3f );
	}
	else if(wchar < 0x10000 )
	{
		//length = 3;
		if (len + 2 >= size_d)
			return -1;

		utf8[len++] = 0xe0 | ( wchar >> 12 );
		utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
		utf8[len++] = 0x80 | ( wchar & 0x3f );
	}
	else if( wchar < 0x200000 ) 
	{
		//length = 4;
		if (len + 3 >= size_d)
			return -1;

		utf8[len++] = 0xf0 | ( (int)wchar >> 18 );
		utf8[len++] = 0x80 | ( (wchar >> 12) & 0x3f );
		utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
		utf8[len++] = 0x80 | ( wchar & 0x3f );
	}
	return len;
}


void ReadINIUtf8ToWideChar( WCHAR* INIfileName, 
						   WCHAR* ElementName, 
						   WCHAR* KeyName, 
						   WCHAR* OutBuf, 
						   WCHAR* DefaultText )
{
	char _buf[MAX_PATH] = {0};

	//将默认字符串转换成多字节
	char _DefaultText[MAX_PATH] = {0};
	int Size = WideCharToMultiByte(CP_ACP,0,DefaultText,-1, NULL, 0, NULL,0);
	WideCharToMultiByte(CP_ACP, 0, DefaultText, -1, _DefaultText, Size, NULL, 0);	

	//将元素名称转换成多字节
	char _ElementName[MAX_PATH] = {0};
	Size = WideCharToMultiByte(CP_ACP,0,ElementName,-1, NULL, 0, NULL,0);
	WideCharToMultiByte(CP_ACP,0,ElementName,-1,_ElementName,Size, NULL, 0);	

	//将键值名称转换成多字节
	char _KeyName[MAX_PATH] = {0};
	Size = WideCharToMultiByte(CP_ACP,0,KeyName,-1, NULL, 0, NULL,0);
	WideCharToMultiByte(CP_ACP,0,KeyName,-1,_KeyName,Size, NULL, 0);	

	//将文件名称转换成多字节
	char _ini_file_name[MAX_PATH] = {0};
	Size = WideCharToMultiByte(CP_ACP,0,INIfileName,-1, NULL, 0, NULL,0);
	WideCharToMultiByte(CP_ACP,0,INIfileName,-1,_ini_file_name,Size, NULL, 0);

	//读取键值
	::GetPrivateProfileStringA( _ElementName, _KeyName, _DefaultText, _buf, MAX_PATH, _ini_file_name );

	//将键值转换成宽字节
	MultiByteToWideChar( CP_UTF8,0, _buf, strlen(_buf)+1, OutBuf,	MAX_PATH );
	return ;
}

