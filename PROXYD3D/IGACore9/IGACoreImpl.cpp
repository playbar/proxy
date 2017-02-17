#include "stdafx.h"
#include "IGACore.h"
#include "IGACoreImpl.h"

#include <tinyxml.h>
#include <process.h>

#include <string>
#include <cctype>
#include <list>
#include <map>
#include <algorithm>

#include "formats.h"

class iga2d_ads
{
public:
	std::string name;
	std::string type;
	std::string match_file;
	std::string adsfile;
	int level;
	int adslevel;

	LPDIRECT3DTEXTURE9 match_tex;
	LPDIRECT3DTEXTURE9 ads_tex;

	iga2d_ads() : level(-1), adslevel(-1), match_tex(NULL), ads_tex(NULL) {

	}
};

class texdist
{
public:
	int id;
	std::string name;
	std::string match_file;

	LPDIRECT3DTEXTURE9 match_tex;
	texdist() : id(-1), match_tex(NULL) {

	}
};
typedef std::list<texdist>                  CTexDistList;
typedef std::list<iga2d_ads>                CIGA2DAdsList;
typedef std::map<IDirect3DTexture9*, BOOL>  CDXMapTextureExport;
typedef std::map<IDirect3DTexture9*, BOOL>  CDXMapTextureScan;

CTexDistList               g_listTexDist; // 匹配贴图分辨率
CIGA2DAdsList              g_listIGA2DAds;
CDXMapTextureExport        g_mapDXTextureExport;
CDXMapTextureScan          g_mapDXTextureScan;

// 获得表面单行所占字节数
UINT GetSurfaceLineBytes(D3DSURFACE_DESC* pDesc)
{
	UINT nLineBytes = 0;

	for( int i = 0; i < fmtInfoArraySize; i++ )
	{
		if( fmtInfoArray[i].fmt == pDesc->Format )
		{
			nLineBytes = (pDesc->Width * (fmtInfoArray[i].nBitsPerTexel / sizeof(BYTE))) / 8;
			break;
		}
	}
	return nLineBytes;
}
/*
// 比较图像
BOOL CompareTexture(DWORD dwLevel, IDirect3DTexture9* pTarget, DWORD dwMatchLevel, IDirect3DTexture9* pMatchTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC match_desc;

	TCHAR chDebugString[128];

	if (pTarget->GetType() != pMatchTexture->GetType())
		goto l_failed;
	if (dwLevel >= pTarget->GetLevelCount())
		goto l_failed;
	if (dwMatchLevel >= pMatchTexture->GetLevelCount())
		goto l_failed;

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pMatchTexture->GetLevelDesc(dwMatchLevel, &match_desc);
	if (FAILED(hr))
		goto l_failed;

	if (target_desc.Width != match_desc.Width)
		goto l_failed;
	if (target_desc.Height != match_desc.Height)
		goto l_failed;
	if (target_desc.Format != match_desc.Format)
		goto l_failed;
//	if (target_desc.Type != match_desc.Type)
//		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT match_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr))
		goto l_failed;

	hr = pMatchTexture->LockRect(dwMatchLevel, &match_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr)) {
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	UINT i;
	UINT Height = target_desc.Height;
//	INT LineBytes = target_lockedrect.Pitch / 4;
//	INT Pitch = target_lockedrect.Pitch;
	INT resPitch = target_lockedrect.Pitch / 4;
	UINT LineBytes = GetSurfaceLineBytes(&target_desc);

	_stprintf(chDebugString, _T("LineBytes = %d\r\n"), LineBytes);
	OutputDebugString(chDebugString);

	_stprintf(chDebugString, _T("Pitch = %d\r\n"), resPitch);
	OutputDebugString(chDebugString);

	// 比较单行
	BOOL bMatch = (memcmp(target_lockedrect.pBits, match_lockedrect.pBits, LineBytes) == 0);
	if (!bMatch)
		goto l_return;
	
	// 比较所有行
	//bMatch = (memcmp(target_lockedrect.pBits, match_lockedrect.pBits, LineBytes * Height) == 0);

	LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
	LPBYTE lpMatchBits  = (LPBYTE)match_lockedrect.pBits;
	for (i = 0; i < Height; i++)
	{
		if (memcmp(lpTargetBits, lpMatchBits, LineBytes) != 0) {
			bMatch = FALSE;
			break;
		}
		lpTargetBits += resPitch;
		lpMatchBits  += resPitch;
	}
	
l_return:
	pTarget->UnlockRect(dwLevel);
	pMatchTexture->UnlockRect(dwMatchLevel);
	return bMatch;

l_failed:
	return FALSE;
}

// 替换材质
BOOL ReplaceTexture(DWORD dwLevel, IDirect3DTexture9* pTarget, DWORD dwAdsLevel, IDirect3DTexture9* pAdsTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC adstex_desc;

	if (pTarget->GetType() != pAdsTexture->GetType())
		goto l_failed;
	if (dwLevel >= pTarget->GetLevelCount())
		goto l_failed;
	if (dwAdsLevel >= pAdsTexture->GetLevelCount())
		goto l_failed;

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pAdsTexture->GetLevelDesc(dwAdsLevel, &adstex_desc);
	if (FAILED(hr))
		goto l_failed;

	if (target_desc.Width != adstex_desc.Width)
		goto l_failed;
	if (target_desc.Height != adstex_desc.Height)
		goto l_failed;
	if (target_desc.Format != adstex_desc.Format)
		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT adstex_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
	if (FAILED(hr))
		goto l_failed;

	hr = pAdsTexture->LockRect(dwAdsLevel, &adstex_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr)) {
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	UINT Height = target_desc.Height;
	INT LineBytes = target_lockedrect.Pitch / 4;
	INT Pitch = target_lockedrect.Pitch;
	CopyMemory(target_lockedrect.pBits, adstex_lockedrect.pBits, LineBytes * Height);

	pTarget->UnlockRect(dwLevel);
	pAdsTexture->UnlockRect(dwAdsLevel);
	
	return TRUE;

l_failed:
	return FALSE;
}

BOOL ReplaceTextureAllLevel(IDirect3DTexture9* pTarget, IDirect3DTexture9* pAdsTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC adstex_desc;

//	TCHAR chDebugString[128];

	if (pTarget->GetType() != pAdsTexture->GetType())
		goto l_failed;
	if (pTarget->GetLevelCount() <= 0)
		goto l_failed;
	if (pTarget->GetLevelCount() != pAdsTexture->GetLevelCount())
		goto l_failed;

	hr = pTarget->GetLevelDesc(0, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pAdsTexture->GetLevelDesc(0, &adstex_desc);
	if (FAILED(hr))
		goto l_failed;

	if (target_desc.Width != adstex_desc.Width)
		goto l_failed;
	if (target_desc.Height != adstex_desc.Height)
		goto l_failed;
	if (target_desc.Format != adstex_desc.Format)
		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT adstex_lockedrect;

	DWORD i;
	UINT Width, Height;
	INT LineBytes;
	INT Pitch;
	DWORD dwLevelCount = pTarget->GetLevelCount();
	for (i = 0; i < dwLevelCount; i++)
	{
		hr = pTarget->GetLevelDesc(i, &target_desc);
		if (FAILED(hr))
			continue;

		hr = pTarget->LockRect(i, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		if (FAILED(hr))
			continue;

		hr = pAdsTexture->LockRect(i, &adstex_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		if (FAILED(hr)) {
			pTarget->UnlockRect(i);
			continue;
		}

		Width  =  target_desc.Width;
		Height = target_desc.Height;
		LineBytes = target_lockedrect.Pitch / 4;
		Pitch = target_lockedrect.Pitch;

//		_stprintf(chDebugString, _T("target_desc Width: %d, Height: %d\r\ntarget_desc LineBytes: %d\r\n"), Width, Height, LineBytes);
//		OutputDebugString(chDebugString);

		CopyMemory(target_lockedrect.pBits, adstex_lockedrect.pBits, LineBytes * Height);

		pTarget->UnlockRect(i);
		pAdsTexture->UnlockRect(i);
	}
	return TRUE;

l_failed:
	return FALSE;
}
*/


// 比较图像
BOOL CompareTexture(DWORD dwLevel, IDirect3DTexture9* pTarget, DWORD dwMatchLevel, IDirect3DTexture9* pMatchTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("CompareTexture");
#endif
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC match_desc;

	DWORD i;
	INT  Pitch;
	UINT Width, Height;
	UINT LineBytes;
	D3DFORMAT Format;
	
	if (pTarget->GetType() != pMatchTexture->GetType())
	{
		OutputDebugString("类型比较失败！");
		goto l_failed;
	}
	if (dwLevel >= pTarget->GetLevelCount())
	{
		OutputDebugString("dwLevel >= pTarget->GetLevelCount()");
		goto l_failed;
	}
	if (dwMatchLevel >= pMatchTexture->GetLevelCount())
	{
		OutputDebugString("dwMatchLevel >= pMatchTexture->GetLevelCount()");
		goto l_failed;
	}

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
	{
		OutputDebugString("获取pTarget层级描述失败");
		goto l_failed;
	}
	hr = pMatchTexture->GetLevelDesc(dwMatchLevel, &match_desc);
	if (FAILED(hr))
	{
		OutputDebugString("获取dwMatchLevel层级描述失败");
		goto l_failed;
	}

	LineBytes = GetSurfaceLineBytes(&target_desc);
	if (LineBytes == 0)
	{
		OutputDebugString("GetSurfaceLineBytes(&target_desc) == 0");
		goto l_failed;
	}

	if (target_desc.Width != match_desc.Width)
	{
		OutputDebugString("target_desc.Width != match_desc.Width");
		//CHAR  text[256];
		//sprintf( text, "target_desc: %d %d, match_desc: %d %d", target_desc.Width, target_desc.Height, match_desc.Width, match_desc.Height);
		//OutputDebugString(text);
		goto l_failed;
	}
	if (target_desc.Height != match_desc.Height)
	{
		OutputDebugString("target_desc.Height != match_desc.Height");
		goto l_failed;
	}
	if (target_desc.Format != match_desc.Format)
	{
		OutputDebugString("target_desc.Format != match_desc.Format");
		goto l_failed;
	}

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT match_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr))
	{
		OutputDebugString("pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_READONLY)");
		goto l_failed;
	}

	hr = pMatchTexture->LockRect(dwMatchLevel, &match_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr))
	{
		OutputDebugString("pMatchTexture->LockRect(dwMatchLevel, &match_lockedrect, NULL, D3DLOCK_READONLY)");
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	Width  =  target_desc.Width;
	Height = target_desc.Height;
	Format = target_desc.Format;

	switch (Format)
	{
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		Pitch = target_lockedrect.Pitch / 4;
		break;
	default:
		Pitch = target_lockedrect.Pitch;
		break;
	}

	// 比较单行
	BOOL bMatch = (memcmp(target_lockedrect.pBits, match_lockedrect.pBits, LineBytes) == 0);
	if (!bMatch)
	{
		OutputDebugString("bMatch == false");
		//goto l_return;
	}

	LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
	LPBYTE lpMatchBits  = (LPBYTE)match_lockedrect.pBits;
	for (i = 0; i < Height; i++)
	{
		if (memcmp(lpTargetBits, lpMatchBits, LineBytes) != 0) 
		{
			OutputDebugString("memcmp(lpTargetBits, lpMatchBits, LineBytes) != 0");
			bMatch = FALSE;
			break;
		}
		lpTargetBits += Pitch;
		lpMatchBits  += Pitch;
	}

l_return:
	pTarget->UnlockRect(dwLevel);
	pMatchTexture->UnlockRect(dwMatchLevel);
	return bMatch;

l_failed:
	return FALSE;
}

bool    b = false;
// 替换材质
BOOL ReplaceTexture(IDirect3DDevice9* pDevice, DWORD dwLevel, IDirect3DTexture9* pTarget, DWORD dwAdsLevel, IDirect3DTexture9* pAdsTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("ReplaceTexture");
#endif

	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC match_desc;

	DWORD i;
	INT  Pitch;
	UINT Width, Height;
	UINT LineBytes;
	D3DFORMAT Format;

	if (pTarget->GetType() != pAdsTexture->GetType())
	{
		OutputDebugString("pTarget->GetType() != pAdsTexture->GetType()");
		goto l_failed;
	}
	if (dwLevel >= pTarget->GetLevelCount())
	{
		OutputDebugString("dwLevel >= pTarget->GetLevelCount()");
		goto l_failed;
	}
	if (dwAdsLevel >= pAdsTexture->GetLevelCount())
	{
		OutputDebugString("dwAdsLevel >= pAdsTexture->GetLevelCount()");
		goto l_failed;
	}

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
	{
		OutputDebugString("E_FAIL == pTarget->GetLevelDesc(dwLevel, &target_desc)");
		goto l_failed;
	}
	hr = pAdsTexture->GetLevelDesc(dwAdsLevel, &match_desc);
	if (FAILED(hr))
	{
		OutputDebugString("E_FAIL == pAdsTexture->GetLevelDesc(dwAdsLevel, &match_desc)");
		goto l_failed;
	}

	LineBytes = GetSurfaceLineBytes(&target_desc);
	if (LineBytes == 0)
	{
		OutputDebugString("LineBytes == 0");
		goto l_failed;
	}

	if (target_desc.Width != match_desc.Width)
	{
		OutputDebugString("target_desc.Width != match_desc.Width");
		goto l_failed;
	}
	if (target_desc.Height != match_desc.Height)
	{
		OutputDebugString("target_desc.Height != match_desc.Height");
		goto l_failed;
	}
	if (target_desc.Format != match_desc.Format)
	{
		OutputDebugString("target_desc.Format != match_desc.Format");
		goto l_failed;
	}

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT match_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
	if (FAILED(hr))
	{
		OutputDebugString("E_FAIL == pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE)");
		goto l_failed;
	}

	hr = pAdsTexture->LockRect(dwAdsLevel, &match_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr)) 
	{
		OutputDebugString("E_FAIL == pAdsTexture->LockRect(dwAdsLevel, &match_lockedrect, NULL, D3DLOCK_READONLY)");
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	Width  =  target_desc.Width;
	Height = target_desc.Height;
	Format = target_desc.Format;

	switch (Format)
	{
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		Pitch = target_lockedrect.Pitch / 4;
		break;
	default:
		Pitch = target_lockedrect.Pitch;
		break;
	}

	LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
	LPBYTE lpMatchBits  = (LPBYTE)match_lockedrect.pBits;
	for (i = 0; i < Height; i++)
	{
		::CopyMemory(lpTargetBits, lpMatchBits, LineBytes);
 
		lpTargetBits += Pitch;
		lpMatchBits  += Pitch;
	}

	pTarget->UnlockRect(dwLevel);
	pAdsTexture->UnlockRect(dwAdsLevel);

	b = true;
	//hr = D3DXSaveTextureToFile(
	//		"Replace.dds",
	//		D3DXIFF_DDS,
	//		pAdsTexture,
	//		NULL);
	//
	//if (SUCCEEDED(hr))
	//{
	//	OutputDebugString("导出替换后的图片成功！");
	//}
	//else
	//{
	//	OutputDebugString("导出替换后的图片失败！");
	//}

	return TRUE;

l_failed:
	return FALSE;
}

BOOL ReplaceTextureAllLevel(IDirect3DDevice9* pDevice, IDirect3DTexture9* pTarget, IDirect3DTexture9* pAdsTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("ReplaceTextureAllLevel");
#endif

	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC adstex_desc;

//	TCHAR chDebugString[128];

	if (pTarget->GetType() != pAdsTexture->GetType())
		goto l_failed;
	if (pTarget->GetLevelCount() <= 0)
		goto l_failed;
	if (pTarget->GetLevelCount() > pAdsTexture->GetLevelCount())
		goto l_failed;
	hr = pTarget->GetLevelDesc(0, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pAdsTexture->GetLevelDesc(0, &adstex_desc);
	if (FAILED(hr))
		goto l_failed;

	if (target_desc.Width != adstex_desc.Width)
		goto l_failed;
	if (target_desc.Height != adstex_desc.Height)
		goto l_failed;
	if (target_desc.Format != adstex_desc.Format)
		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT adstex_lockedrect;

	DWORD i, j;
	UINT Width, Height;
	INT Pitch;
	UINT LineBytes;
	D3DFORMAT Format;

	DWORD dwLevelCount = pTarget->GetLevelCount();
	for (i = 0; i < dwLevelCount; i++)
	{
		hr = pTarget->GetLevelDesc(i, &target_desc);
		if (FAILED(hr))
			goto l_failed;

		LineBytes = GetSurfaceLineBytes(&target_desc);
		if (LineBytes == 0)
			goto l_failed;

		hr = pTarget->LockRect(i, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		if (FAILED(hr))
			goto l_failed;

		hr = pAdsTexture->LockRect(i, &adstex_lockedrect, NULL, D3DLOCK_READONLY);
		if (FAILED(hr)) {
			pTarget->UnlockRect(i);
			goto l_failed;
		}

		Width  = target_desc.Width;
		Height = target_desc.Height;
		Format = target_desc.Format;

		switch (Format)
		{
		case D3DFMT_DXT1:
		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_DXT5:
			Pitch = target_lockedrect.Pitch / 4;
			break;
		default:
			Pitch = target_lockedrect.Pitch;
			break;
		}

		LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
		LPBYTE lpAdstexBits  = (LPBYTE)adstex_lockedrect.pBits;
		for (j = 0; j < Height; j++)
		{
			::CopyMemory(lpTargetBits, lpAdstexBits, LineBytes);

			lpTargetBits  += Pitch;
			lpAdstexBits  += Pitch;
		}

		pAdsTexture->UnlockRect(i);
		pTarget->UnlockRect(i);
	}
	return TRUE;

l_failed:
	return FALSE;
}

IIGACore* GetIGACore() 
{
	static CIGACoreImpl impl;
	return &impl;
}

CIGACoreImpl::CIGACoreImpl()
{

#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::CIGACoreImpl");
#endif
	m_bEnableExport = FALSE;
	m_bEnableScan = TRUE;
	m_bEnablePlayVideo = TRUE;
	m_pIDirect3DDevice9 = NULL;
	strcpy(m_chExportPath, "E:\\exports\\image\\"); 
}

CIGACoreImpl::~CIGACoreImpl()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::~CIGACoreImpl");
#endif
	
}

void CIGACoreImpl::SetExportPath(LPCTSTR lpszExportPath)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::SetExportPath");
#endif
	if (lpszExportPath == NULL)
		return;
	_tcsncpy(m_chExportPath, lpszExportPath, MAX_PATH);
	m_chExportPath[MAX_PATH - 1] = '\0';
}

void CIGACoreImpl::LoadConfigDocument(LPCTSTR lpszFileName)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::LoadConfigDocument");
#endif

	std::string name;
	std::string exports;
	std::string scans;
	std::string export_path;

	TCHAR chDebugString[128];

	TiXmlDocument doc(lpszFileName);
	if (!doc.LoadFile()) return;
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	// block: name
	{
		pElem = hDoc.FirstChildElement().Element();
		if (!pElem)
			return;

		//把XML文件里元素对应的标志字符串所代表的的值传递给相应的字符串
		name = pElem->Attribute("name");
		exports = pElem->Attribute("exports");
		scans = pElem->Attribute("scans");
		export_path = pElem->Attribute("export_path");

		std::transform(exports.begin(), exports.end(), exports.begin(), std::toupper);
		if (exports == "TRUE")
			m_bEnableExport = TRUE;
		else if (exports == "FALSE")
			m_bEnableExport = FALSE;

		std::transform(scans.begin(), scans.end(), scans.begin(), std::toupper);
		if (scans == "TRUE")
			m_bEnableScan = TRUE;
		else if (scans == "FALSE")
			m_bEnableScan = FALSE;

		SetExportPath(export_path.c_str());

		// save this for later
		hRoot = TiXmlHandle(pElem);	//得到当前元素的目录
	}

	//通过这个元素的目录，开始解吸这个元素的子元素
	TiXmlElement* pAdsTextureNode = hRoot.FirstChild( "adstexture" ).FirstChild().Element();
	for (pAdsTextureNode; pAdsTextureNode != NULL; pAdsTextureNode = pAdsTextureNode->NextSiblingElement("iga2d_ads"))
	{
		iga2d_ads ads;

		//获取子元素的属性（字符串）
		const char* pAdsName = pAdsTextureNode->Attribute("name");
		if (pAdsName != NULL)
			ads.name = pAdsName;

		const char* pAdsType = pAdsTextureNode->Attribute("type");
		if (pAdsType != NULL)
			ads.type = pAdsType;

		//获取子元素的属性（整数类型，还可以访问float、double）
		pAdsTextureNode->QueryIntAttribute("level", &ads.level); // If this fails, original value is left as-is
		
		const char* pmatch_file = pAdsTextureNode->Attribute("match_file");
		if (pmatch_file != NULL)
			ads.match_file = pmatch_file;

		pAdsTextureNode->QueryIntAttribute("adslevel", &ads.adslevel);

		const char* padsfile = pAdsTextureNode->Attribute("adsfile");
		if (padsfile != NULL)
			ads.adsfile = padsfile;

		// 加入广告投放列表
		g_listIGA2DAds.push_back(ads);

		// <iga2d_ads name="tex001" type="DDS" level="0" match_file="" adslevel="0" adsfile="" />
		_stprintf(chDebugString, _T("iga2d_ads(%s, %s, %d, %s, %d, %s)\r\n"), ads.name.c_str(), ads.type.c_str(), ads.level, ads.match_file.c_str(), ads.adslevel, ads.adsfile.c_str());
		OutputDebugString(chDebugString);
	}

	TiXmlElement* pTexDistNode = hRoot.FirstChild( "setting" ).FirstChild().Element();
	for (pTexDistNode; pTexDistNode != NULL; pTexDistNode = pTexDistNode->NextSiblingElement("texdist"))
	{
		texdist td;

		pTexDistNode->QueryIntAttribute("id", &td.id); // If this fails, original value is left as-is

		const char* pName = pTexDistNode->Attribute("name");
		if (pName != NULL)
			td.name = pName;

		const char* pmatch_file = pTexDistNode->Attribute("match_file");
		if (pmatch_file != NULL)
			td.match_file = pmatch_file;

		if (td.id != -1) {
			// 加入广告投放列表
			g_listTexDist.push_back(td);
		}
		// <texdist id="1" name="low"  match_file=".\IGAImages\rsyz.dds">
		_stprintf(chDebugString, _T("texdist(%d, %s, %s)\r\n"), td.id, td.name.c_str(), td.match_file.c_str());
		OutputDebugString(chDebugString);
	}

	m_bEnableTexDist = TRUE;

	m_bRegistered = TRUE;
	// 绑定
//	m_bRegistered = DSValidate(".\\IGACore.ini");
}

void CIGACoreImpl::LoadTexDistResource()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::LoadTexDistResource");
#endif
	HRESULT hr;
	LPDIRECT3DTEXTURE9 pMatchTexture;

	CTexDistList::iterator p = g_listTexDist.begin();
	while (p != g_listTexDist.end())
	{
		pMatchTexture = NULL;

		texdist& td = *p;

		hr = D3DXCreateTextureFromFileA(m_pIDirect3DDevice9, td.match_file.c_str(), &pMatchTexture);
		if (FAILED(hr)) {
			p++;
			continue;
		}

		td.match_tex = pMatchTexture;
		p++;
	}
}

void CIGACoreImpl::LoadAdsResource()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::LoadAdsResource");
#endif
	Load2DAdsResource();
//	LoadVideoAdsResource();
}

void CIGACoreImpl::Load2DAdsResource()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::Load2DAdsResource");
#endif
	HRESULT hr;
	LPDIRECT3DTEXTURE9 pMatchTexture;
	LPDIRECT3DTEXTURE9 pAdsTexture;

	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *tmp;
	tmp = strrchr(TmpPath,'\\');
	if(!tmp)
		return ;
	*(tmp+1) = 0;


	CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
	while (p != g_listIGA2DAds.end())
	{
		pMatchTexture = NULL;
		pAdsTexture = NULL;

		iga2d_ads& ads = *p;

		char szLogFile[MAX_PATH];
		sprintf(szLogFile,"%s%s",TmpPath,ads.match_file.c_str());
		OutputDebugString( szLogFile );

		hr = D3DXCreateTextureFromFileA(m_pIDirect3DDevice9, szLogFile, &pMatchTexture);
		if (FAILED(hr)) 
		{
			OutputDebugString("匹配图加载失败！");
			OutputDebugString(szLogFile);
			p++;
			continue;
		}

		char szLogFile1[MAX_PATH];

		//计算匹配图片的信息
		DWORD LevelCount = pMatchTexture->GetLevelCount();
		//sprintf(szLogFile1,"LevelCount: %d", LevelCount );
		OutputDebugString( szLogFile1 );

		int w = 0;
		int h = 0;
		D3DFORMAT fmt;
		D3DPOOL pool;
		DWORD Usage = 0;
        if( LevelCount > 0 )
		{
			D3DSURFACE_DESC desc;
			hr = pMatchTexture->GetLevelDesc( 0, &desc );
			if( SUCCEEDED(hr) )
			{
				w = desc.Width;
				h = desc.Height;
				fmt = desc.Format;
				pool = desc.Pool;
				Usage = desc.Usage;
			}
		}

		sprintf(szLogFile1,"%s%s",TmpPath,ads.adsfile.c_str());
		OutputDebugString( szLogFile1 );
		//hr = D3DXCreateTextureFromFileA(m_pIDirect3DDevice9, szLogFile1, &pAdsTexture);
		hr = D3DXCreateTextureFromFileEx(	m_pIDirect3DDevice9, szLogFile1, 
											w, h, LevelCount,
											Usage, fmt, pool, 
											D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, 
											&pAdsTexture );
		if (FAILED(hr)) 
		{
			OutputDebugString("匹配图加载失败！");
			OutputDebugString(szLogFile1);

			p++;
			continue;
		}

		if( ads.match_tex == NULL )
		{
			ads.match_tex = pMatchTexture;
		}
		else
		{
			pMatchTexture->Release();
		}
		ads.ads_tex = pAdsTexture;

		p++;
	}
}

void CIGACoreImpl::LoadVideoAdsResource()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::LoadVideoAdsResource");
#endif

}

void CIGACoreImpl::ReleaseAdsResource()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::ReleaseAdsResource");
#endif


	CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
	while (p != g_listIGA2DAds.end())
	{
		iga2d_ads& ads = *p;

		if (ads.match_tex)
			ads.match_tex->Release();
		if (ads.ads_tex)
			ads.ads_tex->Release();

		p++;
	}
}

void CIGACoreImpl::PrintTextureDesc(IDirect3DTexture9* pTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::PrintTextureDesc");
#endif

	HRESULT hr;
	D3DSURFACE_DESC desc;
	DWORD dwLevelCount;

	TCHAR chDebugString[128];

	if (pTexture == NULL) 
		return;

	pTexture->AddRef();

	dwLevelCount = pTexture->GetLevelCount();

	_stprintf(chDebugString, _T("PrintTextureDesc(0x%p, Level Count: %d)\r\n"), pTexture, dwLevelCount);
	OutputDebugString(chDebugString);

	hr = pTexture->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr))
	{
		_stprintf(chDebugString, _T("\tWidth: %d\r\n\tHeight: %d\r\n\tType: %d\r\n\tFormat: 0x%08X\r\n\tPool: 0x%08X\r\n"), 
			desc.Width, 
			desc.Height, 
			desc.Type,
			desc.Format,
			desc.Pool
			);
		OutputDebugString(chDebugString);
	}
	pTexture->Release();
}

void CIGACoreImpl::OnDirect3DCreate(IDirect3D9 *pD3D)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnDirect3DCreate");
#endif
	
}

void CIGACoreImpl::OnInitD3DDevice(IDirect3DDevice9* pDevice)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnInitD3DDevice");
#endif

	m_pIDirect3DDevice9 = pDevice;
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;
    
	char szLogFile[MAX_PATH];
	sprintf(szLogFile,"%sIGAConfig.xml",TmpPath);

	LoadConfigDocument(szLogFile);
	// 加载“匹配贴图分辨率”所需资源
	LoadTexDistResource();
	// 加载广告资源
	OutputDebugString("加载替换的图片");
	LoadAdsResource();
	OutputDebugString("完成IGA资源的加载");
}

void CIGACoreImpl::OnDestroyD3DDevice(IDirect3DDevice9* pDevice)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnDestroyD3DDevice");
#endif
	ReleaseAdsResource();
}

void CIGACoreImpl::OnCreateTexture(UINT Width,
								   UINT Height,
								   UINT Levels,
								   DWORD Usage,
								   D3DFORMAT Format,
								   D3DPOOL Pool,
								   IDirect3DTexture9* pTexture,
								   HANDLE* pSharedHandle)
{

#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnCreateTexture");
#endif
}

void CIGACoreImpl::OnReleaseTexture(IDirect3DTexture9* pTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnReleaseTexture");
#endif
	if (m_bEnableExport) 
	{
		// 删除元素从导出列表
		g_mapDXTextureExport.erase(pTexture);
	}

	if (m_bEnableScan) 
	{
		// 删除元素从扫描列表
		g_mapDXTextureScan.erase(pTexture);
	}
}
//int		num = 0;
//int		numSet = 0;


IDirect3DTexture9* CIGACoreImpl::OnSetTexture(DWORD Stage,IDirect3DTexture9* pTexture)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnSetTexture");
#endif
	int len;
	static DWORD sCounter;
	TCHAR chDebugString[128];

	HRESULT hr;
	INT nLevelCount;
	TCHAR chFileName[MAX_PATH];
	TCHAR chExportFile[MAX_PATH];
	
	IDirect3DTexture9* tex = pTexture;
	IDirect3DTexture9* res = tex;

	sCounter++;

	OutputDebugString("开始准备导出图片....\r\n");
	if (m_bEnableExport && m_bRegistered)
	{
		// 插入未导出列表
		if (g_mapDXTextureExport.find(tex) == g_mapDXTextureExport.end()) 
		{
			g_mapDXTextureExport.insert(std::pair<IDirect3DTexture9*, BOOL>(tex, FALSE));
		}

		// 判断是否已经导出
		OutputDebugString("判断是否已经导出....");
		if (!g_mapDXTextureExport[tex])
		{
			OutputDebugString("开始导出图片....");
			nLevelCount = pTexture->GetLevelCount();
			_tcsncpy(chExportFile, m_chExportPath, MAX_PATH);
			len = (int)_tcslen(chExportFile);
			if ((len > 0) && (len < MAX_PATH) && chExportFile[len - 1] != '\\') {
				_tcscat(chExportFile, _T("\\"));
			}

			_stprintf(chFileName, _T("0x%p_%08X_%d_mipmap%02d.dds"), pTexture, GetCurrentThreadId(), sCounter, nLevelCount);
			//_stprintf(chFileName, _T("%d.dds"), num );
			_tcscat(chExportFile, chFileName);
			chExportFile[MAX_PATH - 1] = '\0';
			
			OutputDebugString("导出图片....");
			hr = D3DXSaveTextureToFile(
					chExportFile,
					D3DXIFF_DDS,
					pTexture,
					NULL);
			
			if (SUCCEEDED(hr))
			{
				g_mapDXTextureExport[tex] = TRUE;
				OutputDebugString("D3DXSaveTextureToFile导出图片成功！");
				//num++;
			}
			else
			{
				g_mapDXTextureExport[tex] = FALSE;
				OutputDebugString("D3DXSaveTextureToFile导出图片失败！");
			}
		}
	}

	OutputDebugString("开始准备替换图片....");

	BOOL bReplaced;
	if (m_bEnableScan && m_bRegistered) // 图像扫描开关
	{
		OutputDebugString("判断插入未扫描列表....");
		// 插入未扫描列表
		if (g_mapDXTextureScan.find(tex) == g_mapDXTextureScan.end()) {
			g_mapDXTextureScan.insert(std::pair<IDirect3DTexture9*, BOOL>(tex, FALSE));
		}

		// 判断是否已经扫描过
		OutputDebugString("判断是否已经扫描过....");
		if (!g_mapDXTextureScan[tex])
		{
			OutputDebugString("判断是否匹配贴图分辨率....");
			if (m_bEnableTexDist)
			{
				// 匹配贴图分辨率
				OutputDebugString("寻找匹配贴图分辨率....");
				CTexDistList::iterator p = g_listTexDist.begin();
				while (p != g_listTexDist.end())
				{
					texdist& td = *p;
					if (td.id != -1)
					{
						// 完整替换
						if (CompareTexture(0, tex, 0, td.match_tex)) // 比较level 0，最大的贴图
						{
							// 发送下载广告的请求
							// td.id
							OutputDebugString(" 寻找匹配：发送下载广告的请求....");
							_stprintf(chDebugString, _T("Download Texture Patch Version(%d)\r\n"), td.id);
							OutputDebugString(chDebugString);
							m_bEnableTexDist = FALSE;
						}
					}
					p++;
				}
			}

			// 2D 广告投放操作
			//OutputDebugString("开始寻找要替换的图片....");
			CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
			while (p != g_listIGA2DAds.end())
			{
				OutputDebugString("逐个寻找....");
				bReplaced = FALSE;
				iga2d_ads& ads = *p;
				if (ads.match_tex != NULL)
				{
					OutputDebugString("存在要替换的图片....");
					if (ads.level == -1 && ads.adslevel == -1)
					{
						// 完整替换
						OutputDebugString("进行完全替换....");
						OutputDebugString("开始匹配计算....");
						if (CompareTexture(0, tex, 0, ads.match_tex)) // 比较level 0，最大的贴图
						{
							OutputDebugString("替换图片所有层....");
							if (ReplaceTextureAllLevel(m_pIDirect3DDevice9, tex, ads.ads_tex)) 
							{
							}
							bReplaced = TRUE;
						}
						else
						{
							OutputDebugString("CompareTexture比较失败！");
						}
					}
					else
					{
						// 单层替换
						OutputDebugString("进行单层替换....");
						OutputDebugString("开始匹配计算....");
						if (CompareTexture(0, tex, 0, ads.match_tex)) // 比较level 0，最大的贴图
						{
							OutputDebugString("替换图片第一层....");
							if (ReplaceTexture(m_pIDirect3DDevice9, ads.level, tex, ads.adslevel, ads.ads_tex)) 
							{
								OutputDebugString("替换图片第一层成功！！");
								bReplaced = TRUE;
							}
							else
							{
								OutputDebugString("替换图片失败.");
							}
						}
						else
						{
							OutputDebugString("CompareTexture比较失败！");
						}
					}
				}
				else
				{
					OutputDebugString("ads.match_tex == NULL");
				}
				// 替换成功，移除广告。
				OutputDebugString("判断图片替换...");
				if (bReplaced) 
				{
					OutputDebugString("图片替换成功！");
					break;
				}
				p++;
			}
			g_mapDXTextureScan[tex] = TRUE;
		}
	}
	//numSet++;
	OutputDebugString("返回....");
	//CHAR count[256];
	//sprintf( count, "%d  张, 调用次数：%d ", num, numSet );
	//OutputDebugString( count );
	OutputDebugString("-----------------------------------");
	return res;
}

void CIGACoreImpl::OnGetSurfaceLevel(IDirect3DTexture9* pTexture, UINT Level, IDirect3DSurface9* pSurfaceLevel)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnGetSurfaceLevel");
#endif
}

void CIGACoreImpl::OnBeginScene()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnBeginScene");
#endif
}

void CIGACoreImpl::OnEndScene()
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnEndScene");
#endif
}

void CIGACoreImpl::OnSurfaceLockRect(IDirect3DSurface9* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnSurfaceLockRect");
#endif
}

void CIGACoreImpl::OnSurfaceUnlockRect(IDirect3DSurface9* pSurface)
{
#if OUTPUT_DEBUG
	OutputDebugString("CIGACoreImpl::OnSurfaceUnlockRect");
#endif
}



