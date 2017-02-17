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
// InfoZip.h: interface for the CInfoZip class.
//
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/*
	http://blog.csdn.net/qianjintianguo/archive/2006/04/21/671895.aspx

   一个 ZIP 文件由三个部分组成：
         

        压缩源文件数据区+压缩源文件目录区+压缩源文件目录结束标志

    1、压缩源文件数据区

    在这个数据区中每一个压缩的源文件/目录都是一条记录，记录的格式如下：
       

       [文件头+ 文件数据 + 数据描述符]
 
       a、文件头结构

         组成    　                长度
      文件头标记                  4 bytes  (0x04034b50)
      解压文件所需 pkware 版本    2 bytes
      全局方式位标记              2 bytes
  　　压缩方式                    2 bytes
  　　最后修改文件时间             2 bytes
 　　 最后修改文件日期             2 bytes
 　　 CRC-32校验                  4 bytes
 　 　压缩后尺寸                  4 bytes
 　 　未压缩尺寸                  4 bytes
 　　 文件名长度                  2 bytes

      扩展记录长度                2 bytes
 　　 文件名                     （不定长度）
 　　 扩展字段                   （不定长度）

		typedef struct tagZippedFileHeader
		{
			DWORD	dwMarker;
			WORD	wVersion;
			WORD	wGlobalFlags;
			WORD	wCompressedMode;
			WORD	wTimeLastModified;
			WORD	wDateLastModified;
			DWORD	dwCRC32;
			DWORD	dwSizeCompressed;
			DWORD	dwSizeUncompressed;
			WORD	wFileNameLength;
			CHAR	FileName[0];
		}ZIPPEDFILEHEADER, *LPZIPPEDFILEHEADER;


        b、文件数据

 

        

        c、数据描述符

 　　　组成    　长度
  　　CRC-32校验                  4 bytes
  　　压缩后尺寸                   4 bytes
 　 　未压缩尺寸                   4 bytes

      这个数据描述符只在全局方式位标记的第３位设为１时才存在（见后详解），紧接在压缩数据的最后一个字节后。这个数据描述符只用在不能对输出的 ZIP 文件进行检索时使用。例如：在一个不能检索的驱动器（如：磁带机上）上的 ZIP 文件中。如果是磁盘上的ZIP文件一般没有这个数据描述符。

 

 

     2、压缩源文件目录区

     在这个数据区中每一条纪录对应在压缩源文件数据区中的一条数据

 

   　　　组成               　            长度

    　　目录中文件文件头标记             4 bytes  (0x02014b50)

    　　压缩使用的　pkware 版本          2 bytes

    　　解压文件所需 pkware 版本         2 bytes

    　　全局方式位标记                   2 bytes

    　　压缩方式                        2 bytes

    　　最后修改文件时间                 2 bytes

    　　最后修改文件日期                 2 bytes

    　　ＣＲＣ－３２校验                 4 bytes

    　　压缩后尺寸                      4 bytes

    　　未压缩尺寸                      4 bytes

    　　文件名长度                      2 bytes

    　　扩展字段长度                    2 bytes

    　　文件注释长度                    2 bytes

    　　磁盘开始号                      2 bytes

    　　内部文件属性                    2 bytes

    　　外部文件属性                    4 bytes

        局部头部偏移量                  4 bytes

    　　文件名                       （不定长度）

    　　扩展字段                     （不定长度）

        文件注释                     （不定长度）

 

 

    3、压缩源文件目录结束标志

 

    　　　组成               　          长度

        目录结束标记                    4 bytes  (0x02014b50)

        当前磁盘编号                    2 bytes

        目录区开始磁盘编号              2 bytes

    　　本磁盘上纪录总数                 2 bytes

    　　目录区中纪录总数                 2 bytes

    　　目录区尺寸大小                   4 bytes

    　　目录区对第一张磁盘的偏移量        4 bytes

    　　ZIP 文件注释长度                 2 bytes

    　　ZIP 文件注释                   （不定长度）

 


                                                                     */
/************************************************************************/

#if !defined(AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_)
#define AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ZipDll.h"
#include "UnzipDll.h"
#include "ZCallBck.h"
BEGIN_ZIP


#define ZERROR_NONE                    0
#define ZERROR_DLL_NOT_FOUND           1
#define ZERROR_DLL_FOUNCTION_NOT_FOUND 2
#define ZERROR_NOT_INITIALIZED         3

class InfoZip  
{
public:
	BOOL ExtractFiles(const char* pszArchive, const char* pszTargetFolder);
	void SetDefaultValues(UnzipParams *pParams);
	void ReleaseParams(UnzipParams *pParams);
	BOOL Execute(UnzipParams *pParams);
	BOOL FinalizeUnzip();
	BOOL FinalizeZip();
	BOOL InitializeUnzip();
	BOOL InitializeZip();
	BOOL GetInitializedUnzip();
	BOOL GetInitializedZip();
	void ReleaseParams(ZipParams *pParams);
	BOOL AddFiles(const char *pszArchive, char **paFiles, int iFileCount, BOOL bIncludeDirectoryName = TRUE);
	BOOL Execute(ZipParams *pParams);
	void SetDefaultValues(ZipParams *pZipParms);
	BOOL Finalize();
	BOOL GetInitialized();
	BOOL Initialize();
	UINT GetLastError();
	int GetZipDllVersion();
	int GetUnzipDllVersion();
	InfoZip();
	virtual ~InfoZip();

private:
	void SetLastError(UINT uiError);
	UINT				m_uiLastError;
	HINSTANCE			m_ZipDllHandle;
	HINSTANCE			m_UnzipDllHandle;
	CZipDllExec			m_ZipDllExec;
	CUnzipDllExec		m_UnzipDllExec;
	CGetZipDllVersion   m_GetZipDllVersion;
	CGetUnzipDllVersion m_GetUnzipDllVersion;
private:
	static BOOL __stdcall DefaultZipCallback(ZipCallbackData *pData);
};


END_ZIP

#endif // !defined(AFX_INFOZIP_H__A5C325B4_180B_11D3_ABD5_00A0C9B4D8AB__INCLUDED_)
