//说明
//目前可以对DXT1、DXT3及无压缩的格式进行解析
//

#include "StdAfx.h"
#include "CddsAnalyse.h"

#include <fstream>
#include <ddraw.h>
#include <d3d8types.h>
using namespace std;

#include "stdafx.h"
#include <stdio.h>
#include <list>


////解析压缩块（DTX3）
CddsAnalyse::CddsAnalyse(void)
:m_hWnd(NULL),
 m_hBitMap(NULL), 
 m_Width(0),
 m_High(0)
{
}

//////////////////////////////////////////////////////////////////////////
CddsAnalyse::~CddsAnalyse(void)
{
}

//////////////////////////////////////////////////////////////////////////

DWORD* CddsAnalyse::ParseData( char *pBlock, DWORD fmt )
{
	if( pBlock == NULL )
	{
		return NULL;
	}
	//定义输出像素块
	DWORD *black = new DWORD[16];
	if( black == NULL )
	{
		return NULL;
	}
	//定义调色板
	DWORD color[4] = {0};
	//颜色索引缓冲区
	BYTE Index[16] = {0};

	//ALPHA缓冲区
	BYTE alpha[16] = {0};
	//----------------------------------------------------------------------------------------------
	//块的压缩信息
	if( D3DFMT_DXT3 == fmt )
	{
		//-------------------------------------------------
		//取出alpha
		for( int i = 0; i < 8; i++ )
		{
			char tmp1 = pBlock[i];

			BYTE tmp = tmp1&0xf0;
			tmp>>=8;
			alpha[i*2] = tmp*0xff/0xf;

			tmp = tmp1&0xf;
			alpha[i*2+1] = tmp*0xff/0xf;
		}

		//--------------------------------------------------
		// 解析调色板
		//获取第一个颜色
		WORD tmp_color = *((WORD*)&pBlock[8]);

		BYTE r = ( tmp_color & 0xF800 ) >> 8;
		BYTE g = ( tmp_color & 0x7E0 ) >> 3;
		BYTE b = ( tmp_color & 0x1f) << 3;
		BYTE a = 0;

		//保存一份
		BYTE r0 = r;
		BYTE g0 = g;
		BYTE b0 = b;
		BYTE a0 = a;

		color[0] = (DWORD)a;
		color[0] <<= 8; color[0] |= r; color[0] <<= 8; color[0] |= g; color[0] <<= 8; color[0] |= b;

		//获取第二个颜色
		tmp_color = *((WORD*)&pBlock[10]);

		r = ( tmp_color & 0xF800 ) >> 8;
		g = ( tmp_color & 0x7E0 ) >> 3;
		b = ( tmp_color & 0x1f) << 3;
		a = 0;

		//保存一份
		BYTE r1 = r;
		BYTE g1 = g;
		BYTE b1 = b;
		BYTE a1 = a;

		color[1] = (DWORD)a;
		color[1] <<= 8; color[1] |= r; color[1] <<= 8; color[1] |= g; color[1] <<= 8; color[1] |= b;

		//计算第3、4个颜色
		BYTE r2_3 = 0;
		BYTE g2_3 = 0;
		BYTE b2_3 = 0;
		BYTE a2_3 = 0;
		if( color[0] > color[1] )
		{
			r2_3 = ( r0 * 2 + r1 + 1 ) / 3;
			g2_3 = ( g0 * 2 + g1 + 1 ) / 3;
			b2_3 = ( b0 * 2 + b1 + 1 ) / 3;
			a2_3 = 0;
			color[2] = (DWORD)a2_3;
			color[2] <<= 8; color[2] |= r2_3; color[2] <<= 8; color[2] |= g2_3; color[2] <<= 8; color[2] |= b2_3;

			r2_3 = ( r0 + r1 * 2 + 1 ) / 3;
			g2_3 = ( g0 + g1 * 2 + 1 ) / 3;
			b2_3 = ( b0 + b1 * 2 + 1 ) / 3;
			a2_3 = 0;
			color[3] = (DWORD)a2_3;
			color[3] <<= 8; color[3] |= r2_3; color[3] <<= 8; color[3] |= g2_3; color[3] <<= 8; color[3] |= b2_3;

		}
		else
		{
			r2_3 = ( r0 + r1 ) / 2;
			g2_3 = ( g0 + g1 ) / 2;
			b2_3 = ( b0 + b1 ) / 2;
			a2_3 = 0;
			color[2] = (DWORD)a2_3;
			color[2] <<= 8; color[2] |= r2_3; color[2]<<= 8; color[2] |= g2_3; color[2] <<= 8; color[2] |= b2_3;

			r2_3 = ( r0 + r1 * 2 + 1 ) / 3;
			g2_3 = ( g0 + g1 * 2 + 1 ) / 3;
			b2_3 = ( b0 + b1 * 2 + 1 ) / 3;
			a2_3 = 0;
			color[3] = (DWORD)a2_3;
			color[3] <<= 8; color[3] |= r2_3; color[3] <<= 8; color[3] |= g2_3; color[3] <<= 8; color[3] |= b2_3;
		}

		//----------------------------------------------------------------
		//解析颜色索引
		DWORD  *index = (DWORD*)(&pBlock[12]);
		DWORD  tmp_index = *index;
		for( int i = 0; i < 16; i++ )
		{
			Index[i] = (BYTE)tmp_index & 0x3;
			tmp_index>>=2;
		}

		//-----------------------------------------------------------------
		//构造数据
		for( int i = 0; i < 16; i++ )
		{
			//获得颜色值
			DWORD tmp_color = 0;
			switch( Index[i] )
			{
			case 0:
				tmp_color = color[0];
				break;
			case 1:
				tmp_color = color[1];
				break;
			case 2:
				tmp_color = color[2];
				break;
			case 3:
				tmp_color = color[3];
				break;
			}

			//获取alpha
			DWORD tmp_alpha = (DWORD)alpha[i];
			tmp_color |= (tmp_alpha<<=24);

			//完成
			black[i] = tmp_color;
		}
	}
	//-----------------------------------------------------------------------------------------------
	else if( D3DFMT_DXT1 == fmt )
	{
		//--------------------------------------------------
		// 解析调色板,无alpha
		//获取第一个颜色
		WORD tmp_color = *((WORD*)&pBlock[0]);

		BYTE r = ( tmp_color & 0xF800 ) >> 8;
		BYTE g = ( tmp_color & 0x7E0 ) >> 3;
		BYTE b = ( tmp_color & 0x1f) << 3;
		BYTE a = 0;

		//保存一份
		BYTE r0 = r;
		BYTE g0 = g;
		BYTE b0 = b;
		BYTE a0 = a;

		color[0] = (DWORD)a;
		color[0] <<= 8; color[0] |= r; color[0] <<= 8; color[0] |= g; color[0] <<= 8; color[0] |= b;

		//获取第二个颜色
		tmp_color = *((WORD*)&pBlock[2]);

		r = ( tmp_color & 0xF800 ) >> 8;
		g = ( tmp_color & 0x7E0 ) >> 3;
		b = ( tmp_color & 0x1f) << 3;
		a = 0;

		//保存一份
		BYTE r1 = r;
		BYTE g1 = g;
		BYTE b1 = b;
		BYTE a1 = a;

		color[1] = (DWORD)a;
		color[1] <<= 8; color[1] |= r; color[1] <<= 8; color[1] |= g; color[1] <<= 8; color[1] |= b;

		//计算第3、4个颜色
		BYTE r2_3 = 0;
		BYTE g2_3 = 0;
		BYTE b2_3 = 0;
		BYTE a2_3 = 0;
		if( color[0] > color[1] )
		{
			r2_3 = ( r0 * 2 + r1 + 1 ) / 3;
			g2_3 = ( g0 * 2 + g1 + 1 ) / 3;
			b2_3 = ( b0 * 2 + b1 + 1 ) / 3;
			a2_3 = 0;
			color[2] = (DWORD)a2_3;
			color[2] <<= 8; color[2] |= r2_3; color[2] <<= 8; color[2] |= g2_3; color[2] <<= 8; color[2] |= b2_3;

			r2_3 = ( r0 + r1 * 2 + 1 ) / 3;
			g2_3 = ( g0 + g1 * 2 + 1 ) / 3;
			b2_3 = ( b0 + b1 * 2 + 1 ) / 3;
			a2_3 = 0;
			color[3] = (DWORD)a2_3;
			color[3] <<= 8; color[3] |= r2_3; color[3] <<= 8; color[3] |= g2_3; color[3] <<= 8; color[3] |= b2_3;

		}
		else
		{
			r2_3 = ( r0 + r1 ) / 2;
			g2_3 = ( g0 + g1 ) / 2;
			b2_3 = ( b0 + b1 ) / 2;
			a2_3 = 0;
			color[2] = (DWORD)a2_3;
			color[2] <<= 8; color[2] |= r2_3; color[2]<<= 8; color[2] |= g2_3; color[2] <<= 8; color[2] |= b2_3;

			r2_3 = ( r0 + r1 * 2 + 1 ) / 3;
			g2_3 = ( g0 + g1 * 2 + 1 ) / 3;
			b2_3 = ( b0 + b1 * 2 + 1 ) / 3;
			a2_3 = 0;
			color[3] = (DWORD)a2_3;
			color[3] <<= 8; color[3] |= r2_3; color[3] <<= 8; color[3] |= g2_3; color[3] <<= 8; color[3] |= b2_3;
		}

		//----------------------------------------------------------------
		//解析颜色索引
		DWORD  *index = (DWORD*)(&pBlock[4]);
		DWORD  tmp_index = *index;
		for( int i = 0; i < 16; i++ )
		{
			Index[i] = (BYTE)tmp_index & 0x3;
			tmp_index>>=2;
		}

		//-----------------------------------------------------------------
		//构造数据
		for( int i = 0; i < 16; i++ )
		{
			//获得颜色值
			DWORD tmp_color = 0;
			switch( Index[i] )
			{
			case 0:
				tmp_color = color[0];
				break;
			case 1:
				tmp_color = color[1];
				break;
			case 2:
				tmp_color = color[2];
				break;
			case 3:
				tmp_color = color[3];
				break;
			}

			//获取alpha
			DWORD tmp_alpha = 0xff000000;
			tmp_color |= tmp_alpha;

			//完成
			black[i] = tmp_color;
		}
	}
	return black;
}

//////////////////////////////////////////////////////////////////////////

void CddsAnalyse::CompresseProcessorDXT1( char* pData, DWORD size, int w, int h )
{
	if( pData == NULL || size <= 0 )
	{
		return;
	}

	//-----------------------------------------------------------
	//开始分析数据
	//建立位图块缓冲区
	list<DWORD*> blockBuf;

	//根据DXT1的数据块格式，以8个字节为单位
	DWORD num_block = size>>3;

	int k = 0;
	for( DWORD i = 0; i < num_block; i++ )
	{
		char p[16];
		for( int j = 0; j < 8; j++ )
		{
			p[j] = pData[i*8+j];
		}
		blockBuf.push_front(  ParseData( p, D3DFMT_DXT1 ) ); 
		k++;
	}

	//-----------------------------------------------------------
	//建立位图,所有块组成的一张大图，可能比原图大；
	//横向的块数
	DWORD num_w = w/4;
	if( w%4 > 0 )
	{
		num_w ++;
	}

	//纵向的块数
	DWORD num_h = h/4;
	if( h%4 > 0 )
	{
		num_h++;
	}

	DWORD *maxBmp = new DWORD[num_w*4*num_h*4];
	if( maxBmp == NULL )
	{
		return;
	}

	//逐块复制像素到这个大图上
	DWORD *buf = new DWORD[16];
	list<DWORD*>::iterator ii = blockBuf.begin();
	DWORD *tmp_block = NULL;

	for( int i = num_h - 1; i >= 0; i-- )
	{
		for( int j = num_w - 1; j >= 0; j-- )
		{
			if( ii == blockBuf.end() )
			{			 
				break;
			}

			tmp_block = ((DWORD*)(*ii));
			memcpy( (char*)buf, (char*)tmp_block, 64);

			for( int l = 0; l < 4; l++ )
			{
				for( int k = 0; k < 4; k++ )
				{
					maxBmp[i*(num_w<<4) + (j<<2) + l*(num_w<<2) + k] = buf[(l<<2) + k];
				}
			}
			if( tmp_block != NULL )
			{
				delete tmp_block;
				tmp_block = NULL;
			}
			ii++;
		}
	}
	delete buf;

	//释放块索引数据
	blockBuf.clear();

	//还原的图片
	DWORD *bmp = new DWORD[w*h];
	if( bmp == NULL )
	{
		return;
	}

	//从大图上截取需要的部分
	DWORD w = num_w * 4;
	DWORD h = num_h * 4;
	for( DWORD i = 0; i < h; i++ )
	{
		for( DWORD j = 0; j < w; j++ )
		{
			bmp[i*w + j] = maxBmp[i*w + j];
		}
	}
	m_hBitMap = CreateBitmap( w, h, 1, 32, bmp );

	delete bmp;
	delete maxBmp;
}

void CddsAnalyse::CompresseProcessorDXT2( char* pData, DWORD size, int w, int h )
{
	OutputDebugString( "DXT2" );
}

void CddsAnalyse::CompresseProcessorDXT3( char* pData, DWORD size, int w, int h )
{
	if( pData == NULL || size <= 0 )
	{
		return;
	}

	//-----------------------------------------------------------
	//开始分析数据
	//建立位图块缓冲区
	list<DWORD*> blockBuf;

	//根据DXT3的数据块格式，以16个字节为单位
	DWORD num_block = size>>4;

	int k = 0;
	for( DWORD i = 0; i < num_block; i++ )
	{
		char p[16];
		for( int j = 0; j < 16; j++ )
		{
			p[j] = pData[i*16+j];
		}
		blockBuf.push_front(  ParseData( p, D3DFMT_DXT3 ) ); 
		k++;
	}

	//-----------------------------------------------------------
	//建立位图,所有块组成的一张大图，可能比原图大；
	//横向的块数
	DWORD num_w = w/4;
	if( w%4 > 0 )
	{
		num_w ++;
	}

	//纵向的块数
	DWORD num_h = h/4;
	if( h%4 > 0 )
	{
		num_h++;
	}

	DWORD *maxBmp = new DWORD[num_w*4*num_h*4];
	if( maxBmp == NULL )
	{
		return;
	}

	//逐块复制像素到这个大图上
	DWORD *buf = new DWORD[16];
	list<DWORD*>::iterator ii = blockBuf.begin();
	DWORD *tmp_block = NULL;

	for( int i = num_h - 1; i >= 0; i-- )
	{
		for( int j = num_w - 1; j >= 0; j-- )
		{
			if( ii == blockBuf.end() )
			{			 
				break;
			}

			tmp_block = ((DWORD*)(*ii));
			memcpy( (char*)buf, (char*)tmp_block, 64);

			for( int l = 0; l < 4; l++ )
			{
				for( int k = 0; k < 4; k++ )
				{
					maxBmp[i*(num_w<<4) + (j<<2) + l*(num_w<<2) + k] = buf[(l<<2) + k];
				}
			}
			if( tmp_block != NULL )
			{
				delete tmp_block;
				tmp_block = NULL;
			}
			ii++;
		}
	}
	delete buf;

	//释放块索引数据
	blockBuf.clear();

	//还原的图片
	DWORD *bmp = new DWORD[w*h];
	if( bmp == NULL )
	{
		return;
	}

	//从大图上截取需要的部分
	DWORD w = num_w * 4;
	DWORD h = num_h * 4;
	for( DWORD i = 0; i < h; i++ )
	{
		for( DWORD j = 0; j < w; j++ )
		{
			bmp[i*w + j] = maxBmp[i*w + j];
		}
	}
	m_hBitMap = CreateBitmap( w, h, 1, 32, bmp );

	delete bmp;
	delete maxBmp;
}



void CddsAnalyse::CompresseProcessorDXT4( char* pData, DWORD size, int w, int h )
{
	OutputDebugString( "DXT4" );
}

void CddsAnalyse::CompresseProcessorDXT5( char* pData, DWORD size, int w, int h )
{
	OutputDebugString( "DXT5" );
}

DWORD CddsAnalyse::CountDataSize( DWORD width, DWORD high, DWORD fmt )
{
	//横向的块数
	DWORD num_w = width/4;
	if( width%4 > 0 )
	{
		num_w ++;
	}

	//纵向的块数
	DWORD num_h = high/4;
	if( high%4 > 0 )
	{
		num_h++;
	}

	//数据区的字节数
	DWORD num_size = 0;
	switch( fmt )
	{
	case D3DFMT_DXT1:
		num_size = num_w * num_h * 8;	//每块占8个字节，代表16个像素
		break;
	case D3DFMT_DXT2:

		break;
	case D3DFMT_DXT3:
		num_size = num_w * num_h * 16;	//每块占16个字节，代表16个像素		
		break;
	case D3DFMT_DXT4:

		break;
	case D3DFMT_DXT5:

		break;
	}
	return num_size;
}
//////////////////////////////////////////////////////////////////////////

bool CddsAnalyse::LoadDDSFromFile( char* name, HWND hWnd )
{
	//OutputDebugString("begin LoadDDSFromFile");
	m_hWnd = hWnd;
	//---------------
	//文件流的形式读取数据
	DWORD CompressionFlg    = 0L;
	DWORD CompressionFormat = 0L;
	DWORD RGBBitCount		= 0L;

	ifstream in;
	in.open( name, ios::binary );

	char tmp[4];
	in.read( tmp, 4 );
	if( tmp[0] == 'D' && tmp[1] == 'D' && tmp[2] == 'S' )
	{
		DDSURFACEDESC2 desc;
		in.read( (char*)&desc, sizeof(desc) );

		CompressionFlg    = desc.ddpfPixelFormat.dwFlags;
		CompressionFormat = desc.ddpfPixelFormat.dwFourCC;
		RGBBitCount		  = desc.ddpfPixelFormat.dwBumpBitCount;

		//取得图片大小
		m_Width = desc.dwWidth;
		m_High  = desc.dwHeight;

		//无压缩格式的处理
		if( RGBBitCount == 32 && CompressionFlg == DDPF_RGB )
		{
			DWORD data_size = desc.dwHeight * desc.dwWidth * 4;
			BYTE *data	= new BYTE[data_size];
			if( data == NULL )
			{
				in.close();
				return false;
			}
			in.read( (char*)data, data_size );
			m_hBitMap = CreateBitmap( desc.dwWidth, desc.dwHeight, 1, 32, data );
			delete data;
		}
		else if( CompressionFlg == DDPF_FOURCC )
		{
			//获得数据的大小
			DWORD size = CountDataSize( desc.dwWidth, desc.dwHeight, CompressionFormat );

			//读取数据
			char *tmp_data = new char[size]; 
			if( tmp_data == NULL )
			{
				in.close();
				return false;
			}
			in.read( tmp_data, size );

			switch( CompressionFormat )
			{
			case D3DFMT_DXT1:
				CompresseProcessorDXT1( tmp_data, size, desc.dwWidth, desc.dwHeight );
				break;
			case D3DFMT_DXT2:
				CompresseProcessorDXT2( tmp_data, size, desc.dwWidth, desc.dwHeight );
				break;
			case D3DFMT_DXT3:
				CompresseProcessorDXT3( tmp_data, size, desc.dwWidth, desc.dwHeight );
				break;
			case D3DFMT_DXT4:
				CompresseProcessorDXT4( tmp_data, size, desc.dwWidth, desc.dwHeight );
				break;
			case D3DFMT_DXT5:
				CompresseProcessorDXT5( tmp_data, size, desc.dwWidth, desc.dwHeight );
				break;
			}

			delete tmp_data;
			tmp_data = NULL;
		}
	}
	else
	{
		in.close();
		return false;
	}

	in.close();

	//OutputDebugString("end LoadDDSFromFile");

	if( m_hBitMap != NULL )
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void CddsAnalyse::ReleaseFile()
{
	DeleteObject( m_hBitMap );
}	

void CddsAnalyse::DrawToViewerDC( LPRECT rcRect )
{
	HDC   dc = GetDC( m_hWnd );
	HDC   MemDC	= ::CreateCompatibleDC( dc ); //创建内存dc
	SelectObject( MemDC,m_hBitMap);
	POINT pt;
	pt.x = rcRect->left;
	pt.y = rcRect->top;

	BitBlt( dc, pt.x, pt.y, m_Width, m_High, MemDC, 0, 0, SRCCOPY );
	DeleteObject( MemDC );
}



