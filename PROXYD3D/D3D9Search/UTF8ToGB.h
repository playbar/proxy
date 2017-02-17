
#pragma  once 

char* HexToBin(const char * string);

char* BinToHex(const char * BinString);

int BinToInt(const char * string);

int UTF2Uni(const char* src, wchar_t *des );


void ReadINIUtf8ToWideChar( WCHAR* INIfileName, 
						   WCHAR* ElementName, 
						   WCHAR* KeyName, 
						   WCHAR* OutBuf, 
						   WCHAR* DefaultText );


