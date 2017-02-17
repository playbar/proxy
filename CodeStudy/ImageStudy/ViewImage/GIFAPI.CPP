// ************************************************************************
//  文件名：GIFAPI.cpp
//
//  GIF(Graphics Interchange Format) API函数库：
//
//  DIBToGIF()          - 将指定的DIB对象（< 256色）保存为GIF文件
//  EncodeGIF()		- 对指定图像进行GIF_LZW编码
//  WriteCodeGIF() - 输出一个编码
//  ReadGIF()           - 读取GIF文件
//  DecodeGIF()     - 对GIF_LZW编码结果进行解码
//  ReadSrcDataGIF()       - 读取GIF_LZW编码
//
// ************************************************************************

#include "stdafx.h"
#include "DIBAPI.h"
#include "GIFAPI.h"

#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

/*************************************************************************
 *
 * 函数名称：
 *   DIBToGIF()
 *
 * 参数:
 *   LPSTR lpDIB        - 指向DIB对象的指针
 *   CFile& file        - 要保存的文件
 *   BOOL	bInterlace	- 是否按照交错方式保存
 *
 * 返回值:
 *   BOOL               - 成功返回True，否则返回False。
 *
 * 说明:
 *   该函数将指定的DIB对象（< 256色）保存为GIF文件。
 *
 *************************************************************************/
BOOL SaveGIF(LPCSTR lpszFileName, LPSTR lpDIB, BOOL bInterlace)
{
	// 循环变量
	WORD	i;
	WORD	j;
	
	// DIB高度
	WORD	wHeight;
	
	// DIB宽度
	WORD	wWidth;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// GIF文件头
	GIFHEADER          GIFH;
	
	// GIF逻辑屏幕描述块
	GIFSCRDESC         GIFS;
	
	// GIF图像描述块
	GIFIMAGE           GIFI;
	
	// GIF编码参数
	GIFCONTROLPARAM    GIFControlParam;
	
	// 颜色数目
	WORD               wColors;
	
	// 每行字节数
	WORD               wWidthBytes;
	
	// 调色板
	BYTE               byGIF_Pal[768];
	
	// 字节变量
	BYTE               byChar;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO	   lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO   lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL			   bWinStyleDIB;

	// open file
	CFile file;
	if (! file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite))
		return FALSE;
	
	// 获取DIB高度
	wHeight = (WORD) DIBHeight(lpDIB);
	
	// 获取DIB宽度
	wWidth  = (WORD) DIBWidth(lpDIB);
	
	// 找到DIB图像象素起始位置
	lpDIBBits = (LPSTR)FindDIBBits((LPBYTE)lpDIB);
	
	// 给GIFControlParam结构赋值
	GIFControlParam.wWidth     = wWidth;
	GIFControlParam.wDepth     = wHeight;
	GIFControlParam.wBits      = DIBBitCount(lpDIB);
	GIFControlParam.wLineBytes = (WORD)BYTE_WBYTES((DWORD)GIFControlParam.wWidth *
										   (DWORD)GIFControlParam.wBits);
	
	// 计算每行字节数
	wWidthBytes = (WORD)DWORD_WBYTES(wWidth * (DWORD)GIFControlParam.wBits);
	
	// 计算颜色数目
	wColors     = 1 << GIFControlParam.wBits;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 判断是否是WIN3.0的DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// 给调色板赋值
	if (bWinStyleDIB)
	{
		j = 0;
		for (i = 0; i < wColors; i++)
		{
			// 读取红色分量
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbRed;
			
			// 读取绿色分量
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbGreen;
			
			// 读取蓝色分量
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbBlue;
		}
	}
	else
	{
		j = 0;
		for (i = 0; i < wColors; i++)
		{
			// 读取红色分量
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtRed;
			
			// 读取绿色分量
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtGreen;
			
			// 读取红色分量
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtBlue;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////////
	// 开始写GIF文件
	
	// 写GIF文件头
	GIFH.bySignature[0] = 'G';
	GIFH.bySignature[1] = 'I';
	GIFH.bySignature[2] = 'F';
	GIFH.byVersion[0]='8';
	GIFH.byVersion[1]='9';
	GIFH.byVersion[2]='a';
	file.Write((LPSTR)&GIFH, 6);
	
	// 写GIF逻辑屏幕描述块
	GIFS.wWidth               = GIFControlParam.wWidth;
	GIFS.wDepth               = GIFControlParam.wDepth;
	GIFS.GlobalFlag.PalBits   = (BYTE)(GIFControlParam.wBits - 1);
	GIFS.GlobalFlag.SortFlag  = 0x00;
	GIFS.GlobalFlag.ColorRes  = (BYTE)(GIFControlParam.wBits - 1);
	GIFS.GlobalFlag.GlobalPal = 0x01;
	GIFS.byBackground         = 0x00;
	GIFS.byAspect             = 0x00;
	file.Write((LPSTR)&GIFS, 7);
	
	// 写GIF全局调色板
	file.Write((LPSTR)byGIF_Pal,(wColors*3));
	
	// 写GIF图像描述间隔符
	byChar      = 0x2C;
	file.Write((LPSTR)&byChar,1);
	
	// 写GIF图像描述块
	GIFI.wLeft                = 0;
	GIFI.wTop                 = 0;
	GIFI.wWidth               = GIFControlParam.wWidth;
	GIFI.wDepth               = GIFControlParam.wDepth;
	GIFI.LocalFlag.PalBits    = 0x00;
	GIFI.LocalFlag.Reserved   = 0x00;
	GIFI.LocalFlag.SortFlag   = 0x00;
	GIFI.LocalFlag.Interlace  = (BYTE)(bInterlace ? 0x01 : 0x00);
	GIFI.LocalFlag.LocalPal   = 0x00;
	file.Write((LPSTR)&GIFI, 9);
	
	// 写GIF图像压缩数据
	HANDLE hSrcBuff = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
	GIFControlParam.lpDataBuff = (LPSTR)GlobalLock(hSrcBuff);
	GIFControlParam.lpEndBuff  = GIFControlParam.lpDataBuff;
	GIFControlParam.dwTempCode = 0UL;
	GIFControlParam.wByteCnt   = 0;
	GIFControlParam.wBlockNdx  = 1;
	GIFControlParam.byLeftBits = 0x00;
	
	// 进行GIF_LZW编码
	EncodeGIF(file, lpDIBBits, &GIFControlParam,wWidthBytes, bInterlace);
	
	// 判断是否编码成功
	if (GIFControlParam.wByteCnt)
	{
		// 写入文件
		file.Write(GIFControlParam.lpDataBuff, GIFControlParam.wByteCnt);
	}
	
	// 释放内存
	GlobalUnlock(hSrcBuff);
	GlobalFree(hSrcBuff);
	
	// 写GIF Block Terminator
	byChar   = 0x00;
	file.Write((LPSTR)&byChar,1);
	
	// 写GIF文件结尾块
	byChar   = 0x3B;
	file.Write((LPSTR)&byChar,1);
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   EncodeGIF()
 *
 * 参数:
 *   LPSTR lpDIBBits		- 指向源DIB图像指针
 *   CFile& file			- 要保存的文件
 *   LPGIFCONTROLPARAM lpGIFControlParam	- 指向GIFCONTROLPARAM结构的指针
 *	 WORD wWidthBytes		- 每行图像字节数
 *	 BOOL bInterlace		- 是否按照交错方式保存
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   该函数对指定图像进行GIF_LZW编码。
 *
 *************************************************************************/
void EncodeGIF(CFile& file, LPSTR lpDIBBits, 
						  LPGIFCONTROLPARAM lpGIFControlParam,WORD wWidthBytes, BOOL bInterlace)
{
	// 内存分配句柄
	HANDLE hTableNdx;
	HANDLE hPrefix;
	HANDLE hSuffix;
	
	// 指向字串表指针
	LPWORD lpwTableNdx;
	
	// 用于字串表搜索的索引
	LPWORD lpwPrefix;
	LPBYTE lpbySuffix;
	
	// 指向当前编码像素的指针
	LPSTR  lpImage;
	
	// 计算当前数据图像的偏移量
	DWORD  dwDataNdx;
	
	// LZW_CLEAR
	WORD   wLZW_CLEAR;
	
	// LZW_EOI
	WORD   wLZW_EOI;
	
	// LZW_MinCodeLen
	BYTE   byLZW_MinCodeLen;
	
	// 字串表索引
	WORD   wPreTableNdx;
	WORD   wNowTableNdx;
	WORD   wTopTableNdx;
	
	// 哈希表索引
	WORD   wHashNdx;
	WORD   wHashGap;
	WORD   wPrefix;
	WORD   wShiftBits;
	
	// 当前图像的行数
	WORD   wRowNum;
	
	WORD   wWidthCnt;
	
	// 循环变量
	WORD   wi;
	WORD   wj;
	
	// 交错方式存储时每次增加的行数
	WORD   wIncTable[5]  = { 8,8,4,2,0 }; 
	
	// 交错方式存储时起始行数
	WORD   wBgnTable[5]  = { 0,4,2,1,0 }; 
	
	BOOL   bStart;
	BYTE   bySuffix;
	BYTE   bySubBlock[256];
	BYTE   byCurrentBits;
	BYTE   byMask;
	BYTE   byChar;
	BYTE   byPass;
	
	// 临时字节变量
	BYTE   byTemp;
	
	// 给字串表分配内存
	hTableNdx        = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
	hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
	hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_HASH_SIZE);
	
	// 锁定内存
	lpwTableNdx      = (LPWORD)GlobalLock(hTableNdx);
	lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix       = (LPBYTE)GlobalLock(hSuffix);
	
	// 计算LZW_MinCodeLen
	byLZW_MinCodeLen = (BYTE)((lpGIFControlParam->wBits>1) ? lpGIFControlParam->wBits : 0x02);
	
	// 写GIF LZW最小代码大小
	file.Write((LPSTR)&byLZW_MinCodeLen,1);
	
	wRowNum          = 0;
	bStart           = TRUE;
	byPass           = 0x00;
	
	// 计算LZW_CLEAR
	wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
	
	// 计算LZW_EOI
	wLZW_EOI         = wLZW_CLEAR + 1;
	
	// 初始化字串表
	byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
	wNowTableNdx     = wLZW_CLEAR + 2;
	wTopTableNdx     = 1 << byCurrentBits;
	for(wi=0; wi<MAX_HASH_SIZE; wi++)
	{
		// 初始化为0xFFFF
		*(lpwTableNdx+wi) = 0xFFFF;
	}
	
	// 输出LZW_CLEAR
	WriteCodeGIF(file, wLZW_CLEAR, (LPSTR)bySubBlock,
					  &byCurrentBits, lpGIFControlParam);
	
	// 逐行编码
	for(wi=0; wi<lpGIFControlParam->wDepth; wi++)
	{
		// 计算当前偏移量
		dwDataNdx  = (DWORD)(lpGIFControlParam->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
		
		// 指向当前行图像的指针
		lpImage    = (LPSTR) (((BYTE*)lpDIBBits) + dwDataNdx);
		
		wWidthCnt  = 0;
		wShiftBits = 8 - lpGIFControlParam->wBits;
		byMask     = (BYTE)((lpGIFControlParam->wBits==1) ? 0x80 : 0xF0);
		
		if (bStart)
		{
			// 判断是否是256色位图（一个像素一字节）
			if (lpGIFControlParam->wBits==8)
			{
				// 256色，直接赋值即可
				byTemp      = *lpImage++;
			}
			else
			{
				// 非256色，需要移位获取像素值
				wShiftBits  = 8 - lpGIFControlParam->wBits;
				byMask      = (BYTE)((lpGIFControlParam->wBits==1) ? 0x80 : 0xF0);
				byTemp      = (BYTE)((*lpImage & byMask) >> wShiftBits);
				byMask    >>= lpGIFControlParam->wBits;
				wShiftBits -= lpGIFControlParam->wBits;
			}
			wPrefix    = (WORD)byTemp;
			bStart     = FALSE;
			wWidthCnt ++;
		}
		
		// 每行编码
		while(wWidthCnt < lpGIFControlParam->wWidth)
		{
			// 判断是否是256色位图（一个像素一字节）
			if (lpGIFControlParam->wBits==8)
			{
				// 256色，直接赋值即可
				byTemp = *lpImage++;
			}
			else
			{
				// 非256色，需要移位获取像素值
				byChar = *lpImage;
				byTemp = (BYTE)((byChar & byMask) >> wShiftBits);
				if (wShiftBits)
				{
					byMask    >>= lpGIFControlParam->wBits;
					wShiftBits -= lpGIFControlParam->wBits;
				}
				else
				{
					wShiftBits  = 8 - lpGIFControlParam->wBits;
					byMask      = (BYTE)((lpGIFControlParam->wBits==1) ? 0x80 : 0xF0);
					lpImage    ++;
				}
			}
			bySuffix   = byTemp;
			wWidthCnt ++;
			
			// 查找当前字符串是否存在于字串表中
			wHashNdx = wPrefix ^ ((WORD)bySuffix << 4);
			wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);
			
			// 判断当前字符串是否在字串表中
			while(TRUE)
			{
				// 当前字符串不在字串表中
				if (*(lpwTableNdx + wHashNdx) == 0xFFFF)
				{
				   // 新字符串，退出循环
				   break;
				}
				
				// 判断是否找到该字符串
				if ((*(lpwPrefix+wHashNdx)  == wPrefix) &&
					(*(lpbySuffix+wHashNdx) == bySuffix))
				{
					// 找到，退出循环
					break;
				}
				
				// 第二哈希表
				if (wHashNdx < wHashGap)
				{
					wHashNdx += MAX_HASH_SIZE;
				}
				wHashNdx -= wHashGap;
			}
			
			// 判断是否是新字符串
			if (*(lpwTableNdx+wHashNdx) != 0xFFFF)
			{
				// 不是新字符串
				wPrefix = *(lpwTableNdx + wHashNdx);
			 }
			 else
			 {
				// 新字符串
				
				// 输出该编码
				WriteCodeGIF(file,wPrefix,(LPSTR)bySubBlock,
								   &byCurrentBits,lpGIFControlParam);
				
				// 将该新字符串添加到字串表中
				wPreTableNdx = wNowTableNdx;
				
				// 判断是否达到最大字串表大小
				if (wNowTableNdx < MAX_TABLE_SIZE)
				{
					*(lpwTableNdx+wHashNdx) = wNowTableNdx++;
					*(lpwPrefix+wHashNdx)   = wPrefix;
					*(lpbySuffix+wHashNdx)  = bySuffix;
				}
				
				if (wPreTableNdx == wTopTableNdx)
				{
					if (byCurrentBits<12)
					{
						byCurrentBits ++;
						wTopTableNdx <<= 1;
					}
					else
					{
						// 字串表到达最大长度
						
						// 输出LZW_CLEAR
						WriteCodeGIF(file, wLZW_CLEAR, (LPSTR)bySubBlock,
										 &byCurrentBits,lpGIFControlParam);
						
						// 重新初始化字串表
						byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
						wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
						wLZW_EOI         = wLZW_CLEAR + 1;
						wNowTableNdx     = wLZW_CLEAR + 2;
						wTopTableNdx     = 1 << byCurrentBits;
						for(wj=0;wj<MAX_HASH_SIZE;wj++)
						{
							// 初始化为0xFFFF
							*(lpwTableNdx+wj) = 0xFFFF;
						}
					}
				}
				wPrefix = (WORD)bySuffix;
			}
		}
		
		// 判断是否是交错方式
		if (bInterlace)
		{
			// 交错方式，计算下一行位置
			wRowNum += wIncTable[byPass];
			if (wRowNum>=lpGIFControlParam->wDepth)
			{
				byPass ++;
				wRowNum = wBgnTable[byPass];
			}
		}
		else
		{
			// 非交错方式，直接将行数加一即可
			wRowNum ++;
		}
	}
	
	// 输出当前编码
	WriteCodeGIF(file, wPrefix, (LPSTR)bySubBlock,
					  &byCurrentBits,lpGIFControlParam);
	
	// 输出LZW_EOI
	WriteCodeGIF(file,wLZW_EOI,(LPSTR)bySubBlock,
					  &byCurrentBits,lpGIFControlParam);
	
	if (lpGIFControlParam->byLeftBits)
	{
		// 加入该字符
		bySubBlock[lpGIFControlParam->wBlockNdx++] = (BYTE)lpGIFControlParam->dwTempCode;
		
		// 判断是否超出MAX_SUBBLOCK_SIZE
		if (lpGIFControlParam->wBlockNdx > MAX_SUBBLOCK_SIZE)
		{
			// 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
			if ((lpGIFControlParam->wByteCnt + 256) >= MAX_BUFF_SIZE)
			{
				// 输出
				file.Write(lpGIFControlParam->lpDataBuff,
							  lpGIFControlParam->wByteCnt);
				lpGIFControlParam->lpEndBuff = lpGIFControlParam->lpDataBuff;
				lpGIFControlParam->wByteCnt  = 0;
			}
			bySubBlock[0]           = (BYTE)(lpGIFControlParam->wBlockNdx - 1);
			memcpy(lpGIFControlParam->lpEndBuff,(LPSTR)bySubBlock,lpGIFControlParam->wBlockNdx);
			lpGIFControlParam->lpEndBuff   += lpGIFControlParam->wBlockNdx;
			lpGIFControlParam->wByteCnt    += lpGIFControlParam->wBlockNdx;
			lpGIFControlParam->wBlockNdx    = 1;
		}
		lpGIFControlParam->dwTempCode = 0UL;
		lpGIFControlParam->byLeftBits = 0x00;
	}
	
	if (lpGIFControlParam->wBlockNdx > 1)
	{
		// 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
		if ((lpGIFControlParam->wByteCnt + 256) >= MAX_BUFF_SIZE)
		{
			// 输出
			file.Write(lpGIFControlParam->lpDataBuff,
						   lpGIFControlParam->wByteCnt);
			lpGIFControlParam->lpEndBuff = lpGIFControlParam->lpDataBuff;
			lpGIFControlParam->wByteCnt  = 0;
		}
		bySubBlock[0]           = (BYTE)(lpGIFControlParam->wBlockNdx - 1);
		memcpy(lpGIFControlParam->lpEndBuff,(LPSTR)bySubBlock,lpGIFControlParam->wBlockNdx);
		lpGIFControlParam->lpEndBuff   += lpGIFControlParam->wBlockNdx;
		lpGIFControlParam->wByteCnt    += lpGIFControlParam->wBlockNdx;
		lpGIFControlParam->wBlockNdx    = 1;
	}
	
	// 解除锁定
	GlobalUnlock(hTableNdx);
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	
	// 释放内存
	GlobalFree(hTableNdx);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	
	// 退出
	return;
}


/*************************************************************************
 *
 * 函数名称：
 *   WriteCodeGIF()
 *
 * 参数:
 *   CFile& file			- 要保存的文件
 *	 WORD wCode				- 要添加的编码
 *   LPSTR lpSubBlock		- 子块
 *	 LPBYTE lpbyCurrentBits	- 当前位数
 *	 LPGIFCONTROLPARAM lpGIFControlParam	- 指向GIFCONTROLPARAM结构的指针
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   该函数用来输出一个编码。
 *
 *************************************************************************/
void WriteCodeGIF(CFile& file, WORD wCode, LPSTR lpSubBlock,
                              LPBYTE lpbyCurrentBits,LPGIFCONTROLPARAM lpGIFControlParam)
{
	// 输出该编码
	lpGIFControlParam->dwTempCode |= ((DWORD)wCode << lpGIFControlParam->byLeftBits);
	lpGIFControlParam->byLeftBits += (*lpbyCurrentBits);
	
	while(lpGIFControlParam->byLeftBits >= 0x08)
	{
		lpSubBlock[lpGIFControlParam->wBlockNdx++] = (BYTE)lpGIFControlParam->dwTempCode;
      
		// 判断是否超出MAX_SUBBLOCK_SIZE
		if (lpGIFControlParam->wBlockNdx > MAX_SUBBLOCK_SIZE)
		{
			// 判断wByteCnt + 256是否超过MAX_BUFF_SIZE
			if ((lpGIFControlParam->wByteCnt + 256) >= MAX_BUFF_SIZE)
			{
				// 输出
				file.Write(lpGIFControlParam->lpDataBuff,
							lpGIFControlParam->wByteCnt);
	            lpGIFControlParam->lpEndBuff = lpGIFControlParam->lpDataBuff;
		        lpGIFControlParam->wByteCnt  = 0;
			}
			lpSubBlock[0]           = (BYTE)(lpGIFControlParam->wBlockNdx - 1);
			memcpy(lpGIFControlParam->lpEndBuff,lpSubBlock,lpGIFControlParam->wBlockNdx);
			lpGIFControlParam->lpEndBuff   += lpGIFControlParam->wBlockNdx;
			lpGIFControlParam->wByteCnt    += lpGIFControlParam->wBlockNdx;
			lpGIFControlParam->wBlockNdx    = 1;
		}
		lpGIFControlParam->dwTempCode >>= 8;
		lpGIFControlParam->byLeftBits  -= 0x08;
	}
	
	// 返回
	return;
}


/*************************************************************************
 *
 * 函数名称：
 *   ReadGIF()
 *
 * 参数:
 *   CFile& file        - 要读取的文件
 *
 * 返回值:
 *   HDIB               - 成功返回DIB的句柄，否则返回NULL。
 *
 * 说明:
 *   该函数将读取指定的GIF文件。将读取的结果保存在一个未压缩
 * 编码的DIB对象中。
 *
 *************************************************************************/
HDIB ReadGIF(LPCTSTR lpszFileName)
{
	
	// DIB句柄
	HDIB			   hDIB;
	
	// DIB指针
	LPSTR			   pDIB;
	
	// 指向DIB像素的指针
	LPSTR			   lpDIBBits;
	
	// 指向BITMAPINFOHEADER的指针
	LPBITMAPINFOHEADER lpBIH;
	
	// 指向BITMAPINFO的指针
	LPBITMAPINFO       lpBI;	
	
	// GIF文件头
	GIFHEADER          GIFH;
	
	// GIF逻辑屏幕描述块
	GIFSCRDESC         GIFS;
	
	// GIF图像描述块
	GIFIMAGE           GIFI;
	
	// GIF图像控制扩充块
	GIFCONTROL         GIFC;
	
	// GIF图像说明扩充块
	GIFPLAINTEXT       GIFP;
	
	// GIF应用程序扩充块
	GIFAPPLICATION     GIFA;
	
	// GIF编码参数
	GIFDATAPARAM           GIFDataParam;
	
	// 颜色数目
	WORD               wColors;
	
	// 每行字节数
	WORD               wWidthBytes;
	
	// 调色板
	BYTE               byGIF_Pal[768];
	
	// 16色系统调色板
	BYTE               bySysPal16[48] = {	0,   0,   0,   0,   0, 128,
											0, 128,   0,   0, 128, 128,
										  128,   0,   0, 128,   0, 128,
										  128, 128,   0, 128, 128, 128,
										  192, 192, 192,   0,   0, 255,
											0, 255,   0,   0, 255, 255,
										  255,   0,   0, 255,   0, 255,
										  255, 255,   0, 255, 255, 255
									   };
	
	// DIB大小(字节数)
	DWORD              dwDIB_Size;	
	
	// 调色板大小(字节数)
	WORD               wPalSize;
	
	// 字节变量
	BYTE               byTemp;
	
	// 内存句柄
	HANDLE			   hSrcBuff;
	HANDLE             hTemp;
	
	// 内存指针
	LPSTR              lpTemp;
	
	// 字变量
	WORD               wTemp;
	
	// 循环变量
	WORD               wi;
	
	// 标签
	BYTE               byLabel;
	
	// 块大小
	BYTE               byBlockSize;

	// open file
	CFile file;
	if (! file.Open(lpszFileName, CFile::modeRead))
		return FALSE;
	
	// 读取GIF文件头
	file.Read((LPSTR)&GIFH, sizeof(GIFH));
	
	// 判断是否是GIF文件
	if (memicmp((LPSTR)GIFH.bySignature,"GIF",3) != 0)
	{
		// 非GIF文件，返回NULL
		return NULL;
	}
	
	// 判断版本号是否正确
	if ((memicmp((LPSTR)GIFH.byVersion,"87a",3) != 0) &&
		(memicmp((LPSTR)GIFH.byVersion,"89a",3) != 0))
	{
		// 不支持该版本，返回NULL
		return NULL;
	}
	
	// 读取GIF逻辑屏幕描述块
	file.Read((LPSTR)&GIFS, 7);
	
	// 获取调色板的位数
	GIFDataParam.wBits = (WORD)GIFS.GlobalFlag.PalBits + 1;
	
	// 判断是否有全局调色板
	if (GIFS.GlobalFlag.GlobalPal)
	{
		// 赋初值
		memset((LPSTR)byGIF_Pal,0,768);
		
		// 全局调色板大小
		wPalSize = 3 * (1 << GIFDataParam.wBits);
		
		// 读取全局调色板
		file.Read((LPSTR)byGIF_Pal,wPalSize);
	}
	
	// 读取下一个字节
	file.Read((LPSTR)&byTemp,1);
	
	// 对每一个描述块循环
	while(TRUE)
	{
		// 判断是否是图像描述块
		if (byTemp == 0x2C)
		{
			// 是图像描述块，退出循环
			break;
		}
		
		// 判断是否是GIF扩展块
		if (byTemp==0x21)
		{
			// 是GIF扩展块
			
			// 分配内存
			hTemp  = GlobalAlloc(GHND,(DWORD)MAX_BUFF_SIZE);
			
			// 锁定内存
			lpTemp = (LPSTR) GlobalLock(hTemp);
			
			// 读取下一个字节
			file.Read((LPSTR)&byLabel, 1);
			
			// 针对各种扩充块，进行分别处理
			switch(byLabel)
			{
				case 0xF9:
				{
					// 图像控制扩充块
					file.Read((LPSTR)&GIFC, 6);
					
					// 跳出
					break;
				}
				case 0x01:
				{
					// 图像说明扩充块
					file.Read((LPSTR)&GIFP,sizeof(GIFP));
					
					// 读取扩充块大小
					file.Read((LPSTR)&byBlockSize,1);
					
					// 当byBlockSize > 0时循环读取
					while(byBlockSize)
					{
						// 读取图像说明扩充块（这里没有进行任何处理）
						file.Read(lpTemp,byBlockSize);
						
						// 读取扩充块大小
						file.Read((LPSTR)&byBlockSize,1);
					}
					
					// 跳出
					break;
				}
				case 0xFE:
				{
					// 注释说明扩充块
					
					// 读取扩充块大小
					file.Read((LPSTR)&byBlockSize,1);
					
					// 当byBlockSize > 0时循环读取
					while(byBlockSize)
					{
						// 读取注释说明扩充块（这里没有进行任何处理）
						file.Read(lpTemp,byBlockSize);
					
						// 读取扩充块大小
						file.Read((LPSTR)&byBlockSize,1);
					}
					
					// 跳出
					break;
				}
				case 0xFF:
				{
					// 应用程序扩充块
					file.Read((LPSTR)&GIFA, sizeof(GIFA));
					
					// 读取扩充块大小
					file.Read((LPSTR)&byBlockSize,1);
					
					// 当byBlockSize > 0时循环读取
					while(byBlockSize)
					{
						// 读取应用程序扩充块（这里没有进行任何处理）
						file.Read(lpTemp,byBlockSize);
						
						// 读取扩充块大小
						file.Read((LPSTR)&byBlockSize,1);
					}
					
					// 跳出
					break;
				}
				default:
				{
					// 忽略未知的扩充块
					
					// 读取扩充块大小
					file.Read((LPSTR)&byBlockSize,1);
					
					// 当byBlockSize > 0时循环读取
					while(byBlockSize)
					{
						// 读取未知的扩充块（这里没有进行任何处理）
						file.Read(lpTemp,byBlockSize);
						
						// 读取扩充块大小
						file.Read((LPSTR)&byBlockSize,1);
					}
					
					// 跳出
					break;
				}
			
				// 释放内存
				GlobalUnlock(hTemp);
				GlobalFree(hTemp);
			}
		}
		
		// 读取下一个字节
		file.Read((LPSTR)&byTemp,1);
	}

	// 读取GIF图像描述块
	file.Read((LPSTR)&GIFI, 9);
	
	// 获取图像宽度
	GIFDataParam.wWidth         = GIFI.wWidth;
	
	// 获取图像高度
	GIFDataParam.wDepth         = GIFI.wDepth;
	
	// 判断是否有区域调色板
	if (GIFI.LocalFlag.LocalPal)
	{
		// 赋初值
		memset((LPSTR)byGIF_Pal, 0, 768);
		
		// 读取区域调色板位数
		GIFDataParam.wBits = (WORD)GIFI.LocalFlag.PalBits + 1;
		
		// 区域调色板大小
		wPalSize      = 3 * (1 << GIFDataParam.wBits);
		
		// 读取区域调色板
		file.Read((LPSTR)byGIF_Pal,wPalSize);
	}
	
	// 给GIFDataParam成员赋值
	GIFDataParam.wBits          = ((GIFDataParam.wBits==1) ? 1 :
							  (GIFDataParam.wBits<=4) ? 4 : 8);
	
	GIFDataParam.wLineBytes     = (WORD)BYTE_WBYTES((DWORD)GIFDataParam.wWidth *
											   (DWORD)GIFDataParam.wBits);
	GIFDataParam.bEOF           = FALSE;
	
	// 交错方式
	GIFDataParam.bInterlace     = (GIFI.LocalFlag.Interlace ? TRUE : FALSE);
	
	// 每行字节数
	wWidthBytes			   = (WORD)DWORD_WBYTES((DWORD)GIFDataParam.wWidth *
								  				(DWORD)GIFDataParam.wBits);
	
	// 颜色数目
	wColors				   = 1 << GIFDataParam.wBits;
	
	// 调色板大小
	wPalSize			   = wColors * sizeof(RGBQUAD);
	
	// 计算DIB长度（字节）
	dwDIB_Size			   = sizeof(BITMAPINFOHEADER) + wPalSize
							 + GIFDataParam.wDepth * wWidthBytes;
	
	// 为DIB分配内存
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIB_Size);
	
	if (hDIB == 0)
	{
		// 内存分配失败，返回NULL。
		return NULL;
	}
	
	// 锁定
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	///////////////////////////////////////////////////////////////////////////
	// 设置BITMAPINFOHEADER
	
	// 赋值
	lpBIH = (LPBITMAPINFOHEADER) pDIB;
	lpBI  = (LPBITMAPINFO) pDIB;
	
	// 给lpBIH成员赋值
	lpBIH->biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth = (DWORD)GIFDataParam.wWidth;
	lpBIH->biHeight = (DWORD)GIFDataParam.wDepth;
	lpBIH->biPlanes = 1;
	lpBIH->biBitCount = GIFDataParam.wBits;
	lpBIH->biCompression = BI_RGB;
	lpBIH->biSizeImage = (DWORD)wWidthBytes * GIFDataParam.wDepth;
	lpBIH->biXPelsPerMeter = 0;
	lpBIH->biYPelsPerMeter = 0;
	lpBIH->biClrUsed = wColors;
	lpBIH->biClrImportant = 0;
	
	///////////////////////////////////////////////////////////////////////////
	// 设置调色板
	
	// 判断是否指定全局或区域调色板
	if (GIFS.GlobalFlag.GlobalPal || GIFI.LocalFlag.LocalPal)
	{
		wTemp = 0;
		for(wi=0; wi<wColors; wi++)
		{
			lpBI->bmiColors[wi].rgbRed      = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbGreen    = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbBlue     = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbReserved = 0x00;
		}
	}
	else
	{
		// 没有指定全局和区域调色板，采用系统调色板
		
		// 按照颜色数目来分别给调色板赋值
		switch(wColors)
		{
			case   2:
			{
				// 单色位图
				
				lpBI->bmiColors[0].rgbRed          = 0x00;
				lpBI->bmiColors[0].rgbGreen        = 0x00;
				lpBI->bmiColors[0].rgbBlue         = 0x00;
				lpBI->bmiColors[0].rgbReserved     = 0x00;
				lpBI->bmiColors[1].rgbRed          = 0xFF;
				lpBI->bmiColors[1].rgbGreen        = 0xFF;
				lpBI->bmiColors[1].rgbBlue         = 0xFF;
				lpBI->bmiColors[1].rgbReserved     = 0x00;
				
				// 跳出
				break;
			}
			case  16:
			{
				// 16色位图
				
				wTemp = 0;
				for(wi=0;wi<wColors;wi++)
				{
					lpBI->bmiColors[wi].rgbRed      = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbGreen    = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbBlue     = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbReserved = 0x00;
				}
				
				// 跳出
				break;
			}
			case 256:
			{
				// 256色位图
				for(wi=0; wi<wColors; wi++)
				{
					lpBI->bmiColors[wi].rgbRed      = (BYTE)wi;
					lpBI->bmiColors[wi].rgbGreen    = (BYTE)wi;
					lpBI->bmiColors[wi].rgbBlue     = (BYTE)wi;
					lpBI->bmiColors[wi].rgbReserved = 0x00;
				}
				
				// 跳出
				break;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////
	// 解码

	// 获取编码数据长度
	GIFDataParam.dwDataLen  = (DWORD) (file.GetLength() - file.GetPosition());
	
	// 计算内存大小（最大不超过MAX_BUFF_SIZE）
	GIFDataParam.wMemLen    = ((GIFDataParam.dwDataLen > (DWORD)MAX_BUFF_SIZE) ?
						  (WORD)MAX_BUFF_SIZE : (WORD)GIFDataParam.dwDataLen);
	
	// 分配内存
	hSrcBuff = GlobalAlloc(GHND, (DWORD)GIFDataParam.wMemLen);
	
	// 锁定内存
	GIFDataParam.lpDataBuff = (LPSTR)GlobalLock(hSrcBuff);
	
	// 读取编码数据
	ReadSrcDataGIF(file,&GIFDataParam.wMemLen,&GIFDataParam.dwDataLen,
				GIFDataParam.lpDataBuff,&GIFDataParam.bEOF);
	
	// 缓冲区起始位置
	GIFDataParam.lpBgnBuff  = GIFDataParam.lpDataBuff;
	
	// 缓冲区中止位置
	GIFDataParam.lpEndBuff  = GIFDataParam.lpBgnBuff + GIFDataParam.wMemLen;
	
	// 计算DIB中像素位置
	lpDIBBits = (LPSTR) FindDIBBits((LPBYTE)pDIB);
	
	// 解码
	DecodeGIF(file, lpDIBBits, &GIFDataParam, wWidthBytes);

	// 释放内存
	GlobalUnlock(hSrcBuff);
	GlobalFree(hSrcBuff);
	
	// 返回DIB句柄
	return hDIB;
}


/*************************************************************************
 *
 * 函数名称：
 *   ReadSrcDataGIF()
 *
 * 参数:
 *   CFile& file			- 源GIF文件
 *   LPWORD lpwMemLen		- 缓冲区长度（指针）
 *   LPDWORD lpdwDataLen	- 剩余数据长度（指针）
 *   LPSTR lpSrcBuff		- 缓冲区指针
 *	 LPBOOL lpbEOF			- 结束标志
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   该函数用来读取指定GIF文件中的图像编码，每次最多读取MAX_BUFF_SIZE
 * 字节，是否读完由标志lpbEOF指定。
 *
 *************************************************************************/
void ReadSrcDataGIF(CFile& file, LPWORD lpwMemLen, LPDWORD lpdwDataLen,
                        LPSTR lpSrcBuff, LPBOOL lpbEOF)
{
	// 判断数据长度是否仍然大于内存大小
	if ((*lpdwDataLen) > (DWORD)(*lpwMemLen))
	{
		// 数据长度大于内存大小，表示没有解码完
		
		// 数据长度减内存大小
		(*lpdwDataLen) -= (DWORD)(*lpwMemLen);
	}
	else
	{
		// 数据长度不大于内存大小，表示解码将要完成
		
		// 内存大小就是剩余数据长度
		(*lpwMemLen)    = (WORD)(*lpdwDataLen);
		
		// EOF标志设置为TRUE
		(*lpbEOF)       = TRUE;
	}
	
	// 读取编码数据
	file.Read(lpSrcBuff, (*lpwMemLen));
	
	// 返回
	return;
}

/*************************************************************************
 *
 * 函数名称：
 *   DecodeGIF()
 *
 * 参数:
 *   CFile& file			- 源GIF文件
 *   LPSTR lpDIBBits		- 指向要保存的DIB图像指针
 *   LPGIFDATAPARAM lpGIFDataParam	- 指向GIFCONTROLPARAM结构的指针
 *	 WORD wWidthBytes		- 每行图像字节数
 *
 * 返回值:
 *   无
 *
 * 说明:
 *   该函数对指定GIF_LZW编码数据进行解码。
 *
 *************************************************************************/
void DecodeGIF(CFile& file, LPSTR lpDIBBits,
						  LPGIFDATAPARAM lpGIFDataParam,WORD wWidthBytes)
{
	// 指向编码后图像数据的指针
	BYTE *	lpDst;
	
	// 内存分配句柄
	HANDLE hPrefix;
	HANDLE hSuffix;
	HANDLE hStack;
	HANDLE hImage;
	
	// 用于字串表搜索的索引
	LPWORD lpwPrefix;
	LPBYTE lpbySuffix;
	LPBYTE lpbyStack;
	LPBYTE lpbyStackBgn;
	
	// 指向图像当前行解码结果的指针
	LPSTR  lpImageBgn;
	
	// 指向当前编码像素的指针
	LPSTR  lpImage;
	
	// 计算当前数据图像的偏移量
	DWORD  dwDataNdx;
	
	// LZW_CLEAR
	WORD   wLZW_CLEAR;
	
	// LZW_EOI
	WORD   wLZW_EOI;
	
	// LZW_MinCodeLen
	BYTE   byLZW_MinCodeLen;
	
	// 字串表索引
	WORD   wNowTableNdx;
	WORD   wTopTableNdx;
	
	// 当前图像的行数
	WORD   wRowNum;
	
	// 计数
	WORD   wWidthCnt;
	WORD   wBitCnt;
	WORD   wRowCnt;
	
	// 循环变量
	WORD   wi;
	
	// 交错方式存储时每次增加的行数
	WORD   wIncTable[5]  = { 8,8,4,2,0 }; 
	
	// 交错方式存储时起始行数
	WORD   wBgnTable[5]  = { 0,4,2,1,0 }; 
	
	// 块大小
	BYTE   byBlockSize;
	
	// 块索引
	BYTE   byBlockNdx;
	
	DWORD  dwData;
	
	// 当前编码
	WORD   wCode;
	
	// 上一个编码
	WORD   wOldCode;
	
	// 临时索引
	WORD   wTempNdx;
	
	WORD   wCodeMask[13] = {0x0000,
							0x0001,0x0003,0x0007,0x000F,
							0x001F,0x003F,0x007F,0x00FF,
							0x01FF,0x03FF,0x07FF,0x0FFF
							};
	
	BYTE   byLeftBits;
	BYTE   byFirstChar;
	BYTE   byCode;
	BYTE   byCurrentBits;
	BYTE   byPass;
	
	// 临时字节变量
	BYTE   byTempChar;
	
	// 给字串表分配内存
	hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE<<1));
	hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
	hStack           = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
	hImage           = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	
	// 锁定内存
	lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix       = (LPBYTE)GlobalLock(hSuffix);
	lpbyStack        = (LPBYTE)GlobalLock(hStack);
	lpbyStackBgn     = lpbyStack;
	lpImage          = (LPSTR)GlobalLock(hImage);
	lpImageBgn       = lpImage;
	
	// 读取GIF LZW最小编码大小
	byLZW_MinCodeLen = *lpGIFDataParam->lpBgnBuff++;
	
	byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
	
	// 计算LZW_CLEAR
	wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
	
	// 计算LZW_EOI
	wLZW_EOI         = wLZW_CLEAR + 1;
	
	// 计算字串表索引
	wNowTableNdx     = wLZW_CLEAR + 2;
	wTopTableNdx     = 1 << byCurrentBits;
	
	// 赋初值
	dwData           = 0UL;
	wBitCnt          = lpGIFDataParam->wBits;
	wRowNum          = 0;
	wRowCnt          = 1;
	wWidthCnt        = 0;
	wCode            = 0;
	wOldCode         = 0xFFFF;
	byBlockSize      = 0x01;
	byBlockNdx       = 0x00;
	byLeftBits       = 0x00;
	byTempChar       = 0x00;
	byPass           = 0x00;
	
	// 读取下一个编码
	while(byLeftBits < byCurrentBits)
	{
		// 读取下一个字符
		
		// 判断是否读完一个数据块
		if (++byBlockNdx == byBlockSize)
		{
			// 读取下一个数据块
			byBlockSize = *lpGIFDataParam->lpBgnBuff++;
			byBlockNdx  = 0x00;
			
			// 判断是否读完
			if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
			    !lpGIFDataParam->bEOF)
			{
				// 读取下一个数据块
				ReadSrcDataGIF(file,&lpGIFDataParam->wMemLen,
							&lpGIFDataParam->dwDataLen,
							lpGIFDataParam->lpDataBuff,&lpGIFDataParam->bEOF);
				
				// 指针重新赋值
				lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
				lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff + lpGIFDataParam->wMemLen;
			}
		}
		
		// 下一个字符
		byCode      = *lpGIFDataParam->lpBgnBuff++;
		
		// 移位
		dwData     |= ((DWORD)byCode << byLeftBits);
		byLeftBits += 0x08;
		
		// 判断是否读完
		if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
		    !lpGIFDataParam->bEOF)
		{
			// 读取下一个数据块
			ReadSrcDataGIF(file,&lpGIFDataParam->wMemLen,
						&lpGIFDataParam->dwDataLen,
						lpGIFDataParam->lpDataBuff,&lpGIFDataParam->bEOF);
			
			// 指针重新赋值
			lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
			lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff + lpGIFDataParam->wMemLen;
		}
	}
	
	wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
	dwData    >>= byCurrentBits;
	byLeftBits -= byCurrentBits;
	
	// 解码
	while(wCode != wLZW_EOI)
	{
		// 当前编码不是LZW_EOI码
		
		// 判断是否是LZW_CLEAR码
		if (wCode == wLZW_CLEAR)
		{
			// 是LZW_CLEAR，清除字串表
			
			// 重新初始化字串表
			for(wi = 0; wi < wLZW_CLEAR; wi++)
			{
				*(lpwPrefix  + wi)  = 0xFFFF;
				*(lpbySuffix + wi) = (BYTE)wi;
			}
			
			for(wi = wNowTableNdx; wi < MAX_TABLE_SIZE; wi++)
			{
				*(lpwPrefix+wi)  = 0xFFFF;
				*(lpbySuffix+wi) = 0x00;
			}
			
			byCurrentBits = byLZW_MinCodeLen + (BYTE)0x01;
			wNowTableNdx  = wLZW_CLEAR + 2;
			wTopTableNdx  = 1 << byCurrentBits;
			wOldCode      = 0xFFFF;
			
			// 获取下一个编码
			while(byLeftBits < byCurrentBits)
			{
				// 读取下一个字符
				
				// 判断是否读完一个数据块
				if (++byBlockNdx == byBlockSize)
				{
					// 读取下一个数据块
					byBlockSize = *lpGIFDataParam->lpBgnBuff++;
					byBlockNdx  = 0x00;
					
					// 判断是否读完
					if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
						!lpGIFDataParam->bEOF)
					{
						// 读取下一个数据块
						ReadSrcDataGIF(file, &lpGIFDataParam->wMemLen,
									&lpGIFDataParam->dwDataLen,
									lpGIFDataParam->lpDataBuff,
									&lpGIFDataParam->bEOF);
						
						// 指针重新赋值
						lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
						lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff +
											   lpGIFDataParam->wMemLen;
					}
				}
				byCode      = *lpGIFDataParam->lpBgnBuff++;
				dwData     |= ((DWORD)byCode << byLeftBits);
				byLeftBits += 0x08;
				
				// 判断是否读完
				if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
					 !lpGIFDataParam->bEOF)
				{
					// 读取下一个数据块
					ReadSrcDataGIF(file,&lpGIFDataParam->wMemLen,
								&lpGIFDataParam->dwDataLen,
								lpGIFDataParam->lpDataBuff,&lpGIFDataParam->bEOF);
					
					// 指针重新赋值
					lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
					lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff + lpGIFDataParam->wMemLen;
				}
			}
			wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
			dwData    >>= byCurrentBits;
			byLeftBits -= byCurrentBits;
			
			// 判断编码是否为LZW_EOI
			if (wCode!=wLZW_EOI)
			{
				// 这里没有用到lpbyStack[0]
				lpbyStack  ++;
				
				// 将数据压入堆栈
				while((*(lpwPrefix+wCode)) != 0xFFFF)
				{
					*lpbyStack++ = *(lpbySuffix+wCode);
					wCode        = *(lpwPrefix+wCode);
				}
				*lpbyStack  = *(lpbySuffix+wCode);
				byFirstChar = *lpbyStack;
				
				// 输出数据
				while(lpbyStack>lpbyStackBgn)
				{
					byTempChar |= (*lpbyStack-- << (8-wBitCnt));
					
					if (wBitCnt==8)
					{
						*lpImage++ = byTempChar;
						byTempChar = 0x00;
						wBitCnt    = lpGIFDataParam->wBits;
					}
					else
					{
						wBitCnt   += lpGIFDataParam->wBits;
					}
					
					wWidthCnt  ++;
					
					if (wWidthCnt==lpGIFDataParam->wWidth)
					{
						if (wBitCnt!=lpGIFDataParam->wBits)
						{
							*lpImage   = byTempChar;
							byTempChar = 0x00;
							wBitCnt    = lpGIFDataParam->wBits;
						}
						
						// 图像当前行偏移量
						dwDataNdx = (DWORD)(lpGIFDataParam->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
						
						// 图像当前行起始位置
						lpDst = (BYTE *)lpDIBBits + dwDataNdx;
						
						// 赋值
						memcpy(lpDst, lpImageBgn, wWidthBytes);
						
						lpImage   = lpImageBgn;
						
						// 判断是否按照交错方式保存
						if (lpGIFDataParam->bInterlace)
						{
							// 交错方式
							
							// 计算下一行的行号
							wRowNum += wIncTable[byPass];
							if (wRowNum >= lpGIFDataParam->wDepth)
							{
								byPass ++;
								wRowNum = wBgnTable[byPass];
							}
						}
						else
						{
							// 非交错方式，行号直接加1
							wRowNum ++;
						}
						wWidthCnt = 0;
					}
				}
			}
		}
		else
		{
			// 这里没有用到lpbyStack[0]
			lpbyStack  ++;
			
			// 判断字符串是否在字串表中
			if (wCode < wNowTableNdx)
			{
				// 不在字串表中
				wTempNdx   = wCode;
			}
			else
			{
				// 在字串表中
				wTempNdx     = wOldCode;
				*lpbyStack++ = byFirstChar;
			}
			
			// 将数据压入堆栈
			while((*(lpwPrefix+wTempNdx)) != 0xFFFF)
			{
				*lpbyStack++ = *(lpbySuffix+wTempNdx);
				wTempNdx     = *(lpwPrefix+wTempNdx);
			}
			*lpbyStack  = *(lpbySuffix+wTempNdx);
			byFirstChar = *lpbyStack;
			
			// 将字符串添加到字串表中
			*(lpwPrefix+wNowTableNdx)  = wOldCode;
			*(lpbySuffix+wNowTableNdx) = byFirstChar;
			if (++wNowTableNdx==wTopTableNdx && byCurrentBits<12)
			{
				byCurrentBits ++;
				wTopTableNdx   = 1 << byCurrentBits;
			}
			
			// 输出数据
			while(lpbyStack>lpbyStackBgn)
			{
				byTempChar |= (*lpbyStack-- << (8-wBitCnt));
				if (wBitCnt==8)
				{
					*lpImage++ = byTempChar;
					byTempChar = 0x00;
					wBitCnt    = lpGIFDataParam->wBits;
				}
				else
				{
					wBitCnt   += lpGIFDataParam->wBits;
				}
				
				wWidthCnt  ++;
				if (wWidthCnt==lpGIFDataParam->wWidth)
				{
					if (wBitCnt!=lpGIFDataParam->wBits)
					{
						*lpImage   = byTempChar;
						byTempChar = 0x00;
						wBitCnt    = lpGIFDataParam->wBits;
					}
					
					// 图像当前行偏移量
					dwDataNdx = (DWORD)(lpGIFDataParam->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
					
					// 图像当前行起始位置
					lpDst = (BYTE *)lpDIBBits + dwDataNdx;
					
					// 赋值
					memcpy(lpDst, lpImageBgn, wWidthBytes);
					
					lpImage   = lpImageBgn;
					
					// 判断是否按照交错方式保存
					if (lpGIFDataParam->bInterlace)
					{
						// 交错方式
						
						// 计算下一行的行号
						wRowNum += wIncTable[byPass];
						if (wRowNum >= lpGIFDataParam->wDepth)
						{
							byPass ++;
							wRowNum = wBgnTable[byPass];
						}
					}
					else
					{
						// 非交错方式，行号直接加1
						wRowNum ++;
					}
					wWidthCnt = 0;
				}
			}
		}
		wOldCode = wCode;
		
		// 读取下一个编码
		while(byLeftBits < byCurrentBits)
		{
			// 读取下一个字符
			
			// 判断是否读完一个数据块
			if (++byBlockNdx == byBlockSize)
			{
				// 读取下一个数据块
				byBlockSize = *lpGIFDataParam->lpBgnBuff++;
				byBlockNdx  = 0x00;
				
				// 判断是否读完
				if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
					 !lpGIFDataParam->bEOF)
				{
					// 读取下一个数据块
					ReadSrcDataGIF(file,&lpGIFDataParam->wMemLen,
								&lpGIFDataParam->dwDataLen,
								lpGIFDataParam->lpDataBuff,&lpGIFDataParam->bEOF);
					
					// 指针重新赋值
					lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
					lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff + lpGIFDataParam->wMemLen;
				}
			}
			byCode      = *lpGIFDataParam->lpBgnBuff++;
			dwData     |= ((DWORD)byCode << byLeftBits);
			byLeftBits += 0x08;
			
			// 判断是否读完
			if ((lpGIFDataParam->lpBgnBuff == lpGIFDataParam->lpEndBuff) &&
				  !lpGIFDataParam->bEOF)
			{
				// 读取下一个数据块
				ReadSrcDataGIF(file,&lpGIFDataParam->wMemLen,
							&lpGIFDataParam->dwDataLen,
							lpGIFDataParam->lpDataBuff,&lpGIFDataParam->bEOF);
				
				// 指针重新赋值
				lpGIFDataParam->lpBgnBuff = lpGIFDataParam->lpDataBuff;
				lpGIFDataParam->lpEndBuff = lpGIFDataParam->lpBgnBuff + lpGIFDataParam->wMemLen;
			}
		}
		wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
		dwData    >>= byCurrentBits;
		byLeftBits -= byCurrentBits;
	}
	
	// 释放内存
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	GlobalUnlock(hStack);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	GlobalFree(hStack);
	
	// 返回
	return;
}
