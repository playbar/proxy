//说明
//目前可以对DXT1、DXT3及无压缩的格式进行解析
//

#pragma once

class CddsAnalyse
{
public:
	CddsAnalyse(void);
	~CddsAnalyse(void);

public:
	DWORD* ParseData( char *pBlock, DWORD fmt );
	DWORD CountDataSize( DWORD width, DWORD high, DWORD fmt );
	bool LoadDDSFromFile( char* name, HWND hWnd );
	void ReleaseFile();
	void DrawToViewerDC( LPRECT rcRect );

	void CompresseProcessorDXT1( char* pData, DWORD size, int w, int h );
	void CompresseProcessorDXT2( char* pData, DWORD size, int w, int h );
	void CompresseProcessorDXT3( char* pData, DWORD size, int w, int h );
	void CompresseProcessorDXT4( char* pData, DWORD size, int w, int h );
	void CompresseProcessorDXT5( char* pData, DWORD size, int w, int h );

public:
	HWND    m_hWnd;
	HBITMAP m_hBitMap; 
	DWORD   m_Width;
	DWORD   m_High;
	CHAR    m_strText[256];
};
