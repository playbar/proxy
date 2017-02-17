/***************************************************************
* DirectXVideoTexture.cpp                                      *
*                                                              *
* This file contains the implementation of the                 *
* CDirectXVideoTexture class.  								   *
* To compile correctly, this file must be linked with:         *
* kernel32.lib                                                 *
* user32.lib                                                   *
* d3dx9dt.lib                                                  *
* d3d9.lib                                                     *
* strmbasd.lib                                                 *
* winmm.lib                                                    *
*                                                              *
***************************************************************/

#include "stdafx.h"
#include "DirectXVideoTexture.h"

CDirectXVideoTexture::CDirectXVideoTexture() 
: CBaseVideoRenderer(__uuidof(CLSID_VideoTexture), NAME("Video Texture"), NULL, NULL)
{
	m_pMediaControl  = NULL;
	m_pMediaPosition = NULL;
	m_pMediaEvent    = NULL;
	m_pGraphBuilder  = NULL;
	m_pTexture       = NULL;
	m_pTexture = NULL;
}


CDirectXVideoTexture::~CDirectXVideoTexture()
{
	m_pMediaControl  = NULL;
	m_pMediaPosition = NULL;
	m_pMediaEvent    = NULL;
	m_pGraphBuilder  = NULL;
	m_pTexture       = NULL;
}

void  CDirectXVideoTexture::Destroy()
{
	if (m_pMediaControl)
	{
		m_pMediaControl->Stop();
		m_pMediaControl.Release();
		//m_pMediaControl = NULL;
	}

	if (m_pMediaEvent)
	{
		m_pMediaEvent.Release();
		//m_pMediaEvent = NULL;	
	}

	if (m_pMediaPosition)
	{
		m_pMediaPosition.Release();
		//m_pMediaPosition = NULL ;
	}

	if (m_pGraphBuilder)
	{
		m_pGraphBuilder.Release();
		//m_pGraphBuilder = NULL; 
	}
	////Clean up the texture
	//if (m_pTexture != NULL )
	//{
	//	m_pTexture->Release();
	//	m_pTexture = NULL;
	//}
	//if ( m_pTextureBack != NULL )
	//{
	//	m_pTextureBack->Release();
	//	m_pTextureBack = NULL;
	//}
	//m_pDevice = NULL;
}

//This is called when the graph builder is trying to make connections
HRESULT CDirectXVideoTexture::CheckMediaType(const CMediaType *pMediaType)
{    
    //Only accept video stuff
	if(*pMediaType->FormatType() != FORMAT_VideoInfo) 
	{
        return E_INVALIDARG;
	}
    //Only accept RGB24
	if(IsEqualGUID(*pMediaType->Type(), MEDIATYPE_Video)  &&
		IsEqualGUID(*pMediaType->Subtype(), MEDIASUBTYPE_RGB24))
	{
		return S_OK;
	}
    return E_FAIL;
}

//Once the connection has been established, the graph builder calls SetMediaType
HRESULT CDirectXVideoTexture::SetMediaType(const CMediaType *pMediaType)
{
    //The video format is in the form of a bitmap info header.
	//Use the bitmap info to set the width and height of the texture
	VIDEOINFO *pVideoFormat = (VIDEOINFO *)pMediaType->Format();
    m_VideoWidth  = pVideoFormat->bmiHeader.biWidth;
    m_VideoHeight = abs(pVideoFormat->bmiHeader.biHeight);

	//The pitch is based on an RGB24 bitmap
    m_VideoPitch = (m_VideoWidth * 3 + 3) & ~(3);

    // Create the texture that maps to this media type
    if(FAILED(D3DXCreateTexture(m_pDevice, m_VideoWidth, m_VideoHeight, 1, 0, 
                                D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture)))
	{
        return E_FAIL;
	}

	//// Create the texture that maps to this media type
    if(FAILED(D3DXCreateTexture(m_pDevice, m_VideoWidth, m_VideoHeight, 1, 0, 
                                D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTextureBack)))
	{
        return E_FAIL;
	}

    //Make sure that we actually got the format we asked for
    D3DSURFACE_DESC SurfaceDescription;
    m_pTexture->GetLevelDesc(0, &SurfaceDescription);
    if (SurfaceDescription.Format != D3DFMT_A8R8G8B8) 
        return VFW_E_TYPE_NOT_ACCEPTED;

	m_TextureWidth  = SurfaceDescription.Width; 
	m_TextureHeight = SurfaceDescription.Height;

    return S_OK;
}

void CDirectXVideoTexture::SetUpData()
{
	//m_bBkBuf = true;
}

LPDIRECT3DTEXTURE9 CDirectXVideoTexture::GetTexture()
{
	/*if( m_bChangeSurface )
	{
		return m_pTexture;
	}*/
	return m_pTexture;
}

//This is called every time there is a new frame from the video
HRESULT CDirectXVideoTexture::DoRenderSample(IMediaSample * pSample)
{	
	BYTE  *pVideoBuffer;
	BYTE  *pTextureBuffer;
	long  TexturePitch;

	//static bool s_bfirst = true;
	//if ( s_bfirst )
	//{
	//	s_bfirst = false;
	//}
	//else
	//{
	//	return S_OK;
	//}

	//BITMAPFILEHEADER bmpheader;
	//BITMAPINFOHEADER bmpinfo;
	//BYTE* bmpdata;
	//FILE* fp;
	//fp = fopen("E:\\Whoyo\\Textrue\\test.bmp","rb");
	//if(NULL == fp)
	//{
	//	return S_OK;
	//}
	//fread(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);
	//fread(&bmpinfo,sizeof(BITMAPINFOHEADER),1,fp);
	//bmpdata= new BYTE[ bmpinfo.biSizeImage ];
	//fseek(fp,bmpheader.bfOffBits,0);
	//fread(bmpdata,bmpinfo.biSizeImage,1,fp);

	//fclose( fp );

	////D3DXCreateTextureFromFileInMemory(m_pDevice, bmpdata, bmpinfo.biSizeImage, &m_pTexture );

	//HRESULT hr = D3DXCreateTextureFromFileInMemoryEx (m_pDevice,(LPVOID*)bmpdata, bmpinfo.biSizeImage ,
	//	bmpinfo.biWidth, bmpinfo.biHeight, 1, 0, D3DFMT_A8R8G8B8, 
	//	D3DPOOL_DEFAULT, D3DX_FILTER_BOX, D3DX_DEFAULT, 0, NULL, NULL, &m_pTexture);

	//delete [] bmpdata;

	//return S_OK;

	//D3DXCreateTextureFromFile(m_pDevice, "E:\\Whoyo\\Textrue\\test.bmp", &m_pTexture );
	//return S_OK;

	//The the image buffer for the video frames
	//HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(m_pDevice, pVideoBuffer, 
	//	m_VideoHeight * m_VideoWidth * 3 , m_VideoWidth,m_VideoHeight,1, 0, 
	//	D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 255, NULL, NULL, &m_pTexture );
	//
	

	//////////////////////////////////////////////////////////////////////////
	//Lock the Texture buffer for writing

	pSample->GetPointer(&pVideoBuffer);
	D3DLOCKED_RECT LockedRect;
	if (FAILED(m_pTexture->LockRect(0, &LockedRect, 0, 0)))
		return E_FAIL;

	// Get the texture buffer & pitch
	pTextureBuffer = static_cast<byte *>(LockedRect.pBits);
	TexturePitch = LockedRect.Pitch;

	for(long y = 0; y < m_VideoHeight; y++) 
	{
		BYTE *pVideoRowStart = pVideoBuffer;
		BYTE *pTextureRowStart = pTextureBuffer;  

		//walk through each pixel of the row
		for (int x = 0; x < m_VideoWidth; x++) 
		{
			pTextureBuffer[0] = pVideoBuffer[0];
			pTextureBuffer[1] = pVideoBuffer[1];
			pTextureBuffer[2] = pVideoBuffer[2];
			pTextureBuffer[3] = 0xff;

			pTextureBuffer += 4;
			pVideoBuffer   += 3;
		}
		pVideoBuffer   = pVideoRowStart   + m_VideoPitch;
		pTextureBuffer = pTextureRowStart + TexturePitch;
	}
	if (FAILED(m_pTexture->UnlockRect(0)))
		return E_FAIL;
	return S_OK;
}

HRESULT CDirectXVideoTexture::CreateFromAVIFile(LPDIRECT3DDEVICE9 pDevice, char *pFileName)
{
	m_pDevice = pDevice;

	//Reformat the filename parameter so that it will be accepted by AddSourceFilter
	WCHAR wFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, pFileName, -1, wFileName, MAX_PATH);

	//These are some working objects used to build the filter graph
	CComPtr<IPin>           pPinIn;
	CComPtr<IBaseFilter>    pSourceFilter;
	CComPtr<IPin>           pPinOut;

	//Add a reference so that the graph builder does not delete this object.
	//That causes badness because the object is destroyed too early
	AddRef();

	//initialize the COM library
	CoInitialize(NULL) ;
	
	do 
	{
		if (FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IFilterGraph, (void **)&m_pGraphBuilder)))
		{
			break;
		}
		if (FAILED(m_pGraphBuilder->AddFilter(this, L"TEXTURERENDERER")))
		{
			break;
		}
		//Add a source filter for the AVI file
		if (FAILED(m_pGraphBuilder->AddSourceFilter (wFileName, L"SOURCE", &pSourceFilter)))
		{
			break;
		}
		//Get the input pin for this filter
		if (FAILED(FindPin(L"In", &pPinIn)))
		{
			break;
		}
		//Get the output pin of the source filter
		if (FAILED(pSourceFilter->FindPin(L"Output", &pPinOut)))
		{
			break;
		}
		//Let the graph builder figure out how to connect them
		if (FAILED(m_pGraphBuilder->Connect(pPinOut, pPinIn)))
		{
			break;
		}
		//If the graph was successful, retreive all the interfaces
		m_pGraphBuilder.QueryInterface(&m_pMediaControl);
		m_pGraphBuilder.QueryInterface(&m_pMediaEvent);
		m_pGraphBuilder.QueryInterface(&m_pMediaPosition);

		//This is for audio. Some files may not have audio and the following
		//code may not work for all media types. Check the graph builder tool
		CComPtr<IBaseFilter>    pSplitterFilter;
		CComPtr<IPin>           pAudioPin;

		//Find the a/v splitter in the graph
		if (SUCCEEDED(m_pGraphBuilder->FindFilterByName(L"AVI Splitter", &pSplitterFilter)))
		{
			//Find the pin for the first audio stream and render it. If it's not there,
			//it's OK, just move on...
			if (SUCCEEDED(pSplitterFilter->FindPin(L"Stream 01", &pAudioPin)))
				m_pGraphBuilder->Render(pAudioPin);
		}
		//If everything was successful, start retrieving frames
		//if (SUCCEEDED(m_pMediaControl->Run()))
		return S_OK;

	} while ( false );

	return E_FAIL;
}

void CDirectXVideoTexture::CheckForLoop()
{
	long lEventCode;
	long lParam1;
	long lParam2;

	//If the video is complete, loop back to the start
	m_pMediaEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
	if (EC_COMPLETE == lEventCode)
		m_pMediaPosition->put_CurrentPosition(0);


}

void CDirectXVideoTexture::StartPlay()
{
	m_pMediaControl->Run();
}

void CDirectXVideoTexture::PaustPlay()
{
	m_pMediaControl->Pause();
}

