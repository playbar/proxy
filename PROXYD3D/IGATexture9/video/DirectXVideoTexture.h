/***************************************************************
* DirectXVideoTexture.h                                        *
*                                                              *
* This file contains the header information for the class      *
* CDirectXVideoTexture.  For this file to compile correctly,   *
* the path for the DX include files must be added to your path.*
*                                                              *
***************************************************************/
#pragma  once 

#include <d3d9.h>
//#include <d3dx9.h>

//include COM and stream classes needed for the DirectShow stuff
#include <atlbase.h>
#include <streams.h>

// Define GUID for Video Texture
struct __declspec(uuid("{71771540-2017-11cf-ae26-0020afd79769}")) CLSID_VideoTexture;

class CDirectXVideoTexture : public CBaseVideoRenderer
{
public:
	CDirectXVideoTexture();
	virtual ~CDirectXVideoTexture();

	HRESULT CreateFromAVIFile(LPDIRECT3DDEVICE9 pDevice, char *pFileName);
	void	CheckForLoop();
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT SetMediaType(const CMediaType *pmt);
    HRESULT DoRenderSample(IMediaSample *pMediaSample);

	void				SetUpData();
	LPDIRECT3DTEXTURE9	GetTexture();

	void	StartPlay();
	void	PaustPlay();
	void    Destroy();
    
	long      m_VideoWidth;
	long      m_VideoHeight;
	long      m_TextureWidth;
	long      m_TextureHeight;
	long      m_VideoPitch;
	D3DFORMAT m_VideoFormat;

	CComPtr<IGraphBuilder>          m_pGraphBuilder; 
	CComPtr<IMediaControl>		    m_pMediaControl;  
	CComPtr<IMediaEvent>		    m_pMediaEvent;  
	CComPtr<IMediaPosition>		    m_pMediaPosition;      

	CComPtr<IDirect3DDevice9>  m_pDevice;
	CComPtr<IDirect3DTexture9> m_pTexture;
	CComPtr<IDirect3DTexture9> m_pTextureBack;

};