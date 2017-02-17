#include "d3d8.h"
#include "d3dx8.h"
#include <atlbase.h>
#include <streams.h>
#include <iostream>

//生成一个ID
struct _declspec( uuid( "{71771540-2017-11cf-ae26-0020afd79769}") ) CLSID_VideoTexture;

class CVideoTexture : public CBaseVideoRenderer
{
public:
	CVideoTexture();
	virtual ~CVideoTexture();

	HRESULT CreateFromAVIFile(LPDIRECT3DDEVICE8 pDevice, TCHAR *pFileName);

	void CheckForLoop();
	HRESULT CheckMediaType(const CMediaType *pmt);
	HRESULT SetMediaType(const CMediaType *pmt);
	HRESULT DoRenderSample(IMediaSample *pMediaSample);

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

	LPDIRECT3DTEXTURE8 m_pTexture, pTex3;
	LPDIRECT3DDEVICE8  m_pDevice;
};

