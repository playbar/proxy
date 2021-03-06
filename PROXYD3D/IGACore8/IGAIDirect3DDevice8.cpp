#include "StdAfx.h"
#include "Common.h"
#include "IGAIDirect3DTexture8.h"
#include "IGAIDirect3DDevice8.h"

#include <stdio.h>
#include <tchar.h>

#include "IGA8Core.h"

//-----------------------------------------------------------------
//3D模块
extern FUN_IGA3DINIT          g_pFunIGA3DInit          ;
extern FUN_IGA3DDestory       g_pIGA3DDestoryt         ;
extern FUN_IGA3DGetMatrixData g_pIGA3DGetMatrixData    ;
extern FUN_IGA3DRender        g_pIGA3DRender           ;
extern FUN_ISetFristDraw      g_pSetFristDraw          ;
extern HINSTANCE              g_h3DCore;
extern BOOL				      g_bInit3D;	
extern DWORD				  g_RenderPos;
//--------------------------------------------------------------------

ID3DXMesh* g_Teapot = NULL;  //测试

bool	g_bOutputTexture = false;
extern  DWORD	g_dwOutputTexture ;

IGAIDirect3DDevice8::IGAIDirect3DDevice8(IDirect3DDevice8* pOriginal)
{
    m_pIDirect3DDevice8 = pOriginal; // store the pointer to original object

	//-----------------------------------------------------------
	if( g_bInit3D )
	{
		g_RenderPos = g_pFunIGA3DInit( m_pIDirect3DDevice8, NULL, NULL );
	}
	//-----------------------------------------------------------

	D3DXCreateTeapot(this, &g_Teapot, 0);

	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	SetTransform(D3DTS_VIEW, &V);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)640 / (float)480,
		1.0f,
		1000.0f);
	SetTransform(D3DTS_PROJECTION, &proj);

	//SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

IGAIDirect3DDevice8::~IGAIDirect3DDevice8(void)
{
	IGAWIN::DestoryIGAWin(this);
}

HRESULT IGAIDirect3DDevice8::QueryInterface (REFIID riid, void** ppvObj) {
	*ppvObj = NULL;
	HRESULT hRes = m_pIDirect3DDevice8->QueryInterface(riid, ppvObj); 
	if (hRes == NOERROR) {
		*ppvObj = this;
	}
	return hRes;
}

ULONG IGAIDirect3DDevice8::AddRef(void)
{
	ULONG lCount = m_pIDirect3DDevice8->AddRef();
    return lCount;
}

ULONG IGAIDirect3DDevice8::Release(void)
{
	extern IGAIDirect3DDevice8* gl_pIGAIDirect3DDevice8;
	// Calling original function now
	ULONG lCount = m_pIDirect3DDevice8->Release();	
	/*if (lCount == m_dwRefCount) 
	{
		OutputDebugString("IGAIDirect3DDevice8:Release(void)......");
		if( g_bInit3D )
		{
			g_pIGA3DDestoryt();
		}
		GetIGACore()->OnDestroyD3DDevice(gl_pIGAIDirect3DDevice8);
		gl_pIGAIDirect3DDevice8 = NULL;
  	    delete(this);
	}*/
	return lCount;
}

HRESULT __stdcall IGAIDirect3DDevice8::TestCooperativeLevel(void)
{  return (m_pIDirect3DDevice8->TestCooperativeLevel());}

UINT    __stdcall IGAIDirect3DDevice8::GetAvailableTextureMem(void)
{  return (m_pIDirect3DDevice8->GetAvailableTextureMem());}

HRESULT __stdcall IGAIDirect3DDevice8::ResourceManagerDiscardBytes(DWORD Bytes)
{  return (m_pIDirect3DDevice8->ResourceManagerDiscardBytes( Bytes));}

HRESULT __stdcall IGAIDirect3DDevice8::GetDirect3D(IDirect3D8** ppD3D8)
{  return (m_pIDirect3DDevice8->GetDirect3D( ppD3D8));}

HRESULT __stdcall IGAIDirect3DDevice8::GetDeviceCaps(D3DCAPS8* pCaps)
{  return (m_pIDirect3DDevice8->GetDeviceCaps( pCaps));}

HRESULT __stdcall IGAIDirect3DDevice8::GetDisplayMode(D3DDISPLAYMODE* pMode)
{  return (m_pIDirect3DDevice8->GetDisplayMode( pMode));}

HRESULT __stdcall IGAIDirect3DDevice8::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)  
{  return (m_pIDirect3DDevice8->GetCreationParameters(pParameters) );}

HRESULT __stdcall IGAIDirect3DDevice8::SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface8* pCursorBitmap)  
{  return (m_pIDirect3DDevice8->SetCursorProperties( XHotSpot, YHotSpot, pCursorBitmap)  );}

void __stdcall IGAIDirect3DDevice8::SetCursorPosition(int X,int Y,DWORD Flags) {
	m_pIDirect3DDevice8->SetCursorPosition( X, Y, Flags);
}

BOOL __stdcall IGAIDirect3DDevice8::ShowCursor(BOOL bShow)  
{  return (m_pIDirect3DDevice8->ShowCursor( bShow) );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain8** pSwapChain)  
{  return (m_pIDirect3DDevice8->CreateAdditionalSwapChain( pPresentationParameters, pSwapChain)  );}

HRESULT __stdcall IGAIDirect3DDevice8::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)  
{ 
	//-----------------------------------------------------
	if( g_bInit3D )
	{
		g_pIGA3DDestoryt();
	}

	HRESULT hr = m_pIDirect3DDevice8->Reset( pPresentationParameters);  

	if( SUCCEEDED(hr) && g_bInit3D )
	{
		g_RenderPos = g_pFunIGA3DInit( m_pIDirect3DDevice8, NULL, NULL );
	}

	return hr;
	//-------------------------------------------------------
}

HRESULT __stdcall IGAIDirect3DDevice8::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion) 
{
	
	SetRenderState(D3DRS_LIGHTING, true);
	D3DXMATRIX Ry;
	static float y = 0.0f;
	D3DXMatrixRotationY(&Ry, y);

	y += 0.01;
	if(y >= 6.28f)
		y = 0.0f;
	SetTransform(D3DTS_WORLD, &Ry);
	g_Teapot->DrawSubset(0);

	//----------------------------------------------------
	if( g_RenderPos == Per_Present && g_bInit3D )
	{
		g_pIGA3DRender();
	}

	//开关设置
	if( g_bOutputTexture )
	{
		g_bOutputTexture = false;
	}

	HRESULT hr = m_pIDirect3DDevice8->Present( pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	
	if( g_bInit3D )
	{
		g_pSetFristDraw();
	}

	if( g_dwOutputTexture != 0 )
	{
		g_bOutputTexture = true;
		::InterlockedExchange((volatile long*)(&g_dwOutputTexture), 0);
	}

	//---------------------------------
	//《特种部队》
	static bool s_b = true;
	if( GetAsyncKeyState('Y') )
	{
		if( s_b )
		{
			s_b = false;
			g_bOutputTexture = true;
		}
	}
	else
	{
		s_b = true;
	}

	return hr;

	//---------------------------------------------------
}

HRESULT __stdcall IGAIDirect3DDevice8::GetBackBuffer(UINT BackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface8** ppBackBuffer)  {
	return m_pIDirect3DDevice8->GetBackBuffer( BackBuffer, Type, ppBackBuffer);
}

HRESULT __stdcall IGAIDirect3DDevice8::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus) {  
	return m_pIDirect3DDevice8->GetRasterStatus( pRasterStatus);
}

void __stdcall IGAIDirect3DDevice8::SetGammaRamp(DWORD Flags,CONST D3DGAMMARAMP* pRamp) {
	m_pIDirect3DDevice8->SetGammaRamp( Flags, pRamp);
}

void __stdcall IGAIDirect3DDevice8::GetGammaRamp(D3DGAMMARAMP* pRamp) { 
	m_pIDirect3DDevice8->GetGammaRamp( pRamp);
}

HRESULT IGAIDirect3DDevice8::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8** ppTexture)
{
	static BOOL s_Replaced = FALSE;
	HRESULT hr;
	hr = m_pIDirect3DDevice8->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture);
	if (SUCCEEDED(hr))
	{
		// 初始化 IDirect3DTexture8 Proxy
		if (!s_Replaced) {
			IDirect3DTexture8* pTexture = (*ppTexture);
			InitIDirect3DTexture8Proxy(pTexture);
			s_Replaced = TRUE;
		}
		GetIGACore()->OnCreateTexture(Width, Height, Levels, Usage, Format, Pool, *ppTexture);
	}
    return hr;
}

HRESULT __stdcall IGAIDirect3DDevice8::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture8** ppVolumeTexture)  
{  return (m_pIDirect3DDevice8->CreateVolumeTexture( Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateCubeTexture(  UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture8** ppCubeTexture)  
{  return (m_pIDirect3DDevice8->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateVertexBuffer(  UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer8** ppVertexBuffer)  
{  return (m_pIDirect3DDevice8->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateIndexBuffer(  UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer8** ppIndexBuffer)  
{  return (m_pIDirect3DDevice8->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateRenderTarget(  UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,BOOL Lockable,IDirect3DSurface8** ppSurface)  
{  return (m_pIDirect3DDevice8->CreateRenderTarget(Width, Height, Format, MultiSample, Lockable, ppSurface)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateDepthStencilSurface(  UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,IDirect3DSurface8** ppSurface)  
{  return (m_pIDirect3DDevice8->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface)  );}

HRESULT __stdcall IGAIDirect3DDevice8::CreateImageSurface(  UINT Width,UINT Height,D3DFORMAT Format,IDirect3DSurface8** ppSurface)  
{  return (m_pIDirect3DDevice8->CreateImageSurface(Width, Height, Format, ppSurface));}

HRESULT __stdcall IGAIDirect3DDevice8::CopyRects(  IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,IDirect3DSurface8* pDestinationSurface,CONST POINT* pDestPointsArray)  
{  return (m_pIDirect3DDevice8->CopyRects(pSourceSurface,  pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray)  );}

HRESULT __stdcall IGAIDirect3DDevice8::UpdateTexture(  IDirect3DBaseTexture8* pSourceTexture,IDirect3DBaseTexture8* pDestinationTexture)  
{  return (m_pIDirect3DDevice8->UpdateTexture(pSourceTexture, pDestinationTexture));}

HRESULT __stdcall IGAIDirect3DDevice8::GetFrontBuffer(  IDirect3DSurface8* pDestSurface)  
{  return (m_pIDirect3DDevice8->GetFrontBuffer(pDestSurface));}

HRESULT __stdcall IGAIDirect3DDevice8::SetRenderTarget(  IDirect3DSurface8* pRenderTarget,IDirect3DSurface8* pNewZStencil)  
{  return (m_pIDirect3DDevice8->SetRenderTarget(pRenderTarget, pNewZStencil));}

HRESULT __stdcall IGAIDirect3DDevice8::GetRenderTarget(  IDirect3DSurface8** ppRenderTarget)  
{  return (m_pIDirect3DDevice8->GetRenderTarget(ppRenderTarget));}

HRESULT __stdcall IGAIDirect3DDevice8::GetDepthStencilSurface(  IDirect3DSurface8** ppZStencilSurface)  
{  return (m_pIDirect3DDevice8->GetDepthStencilSurface(ppZStencilSurface));}

HRESULT IGAIDirect3DDevice8::BeginScene(void)
{
	//OutputDebugString("IGAIDirect3DDevice8::BeginScene");
	HRESULT hr = m_pIDirect3DDevice8->BeginScene();
	GetIGACore()->OnBeginScene();
    return hr;
}

HRESULT IGAIDirect3DDevice8::EndScene(void)
{
	HRESULT hr = m_pIDirect3DDevice8->EndScene();
	GetIGACore()->OnEndScene();
    return hr;
}

HRESULT __stdcall IGAIDirect3DDevice8::Clear(  DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)  
{ 
	return (m_pIDirect3DDevice8->Clear(Count,  pRects, Flags, Color, Z, Stencil)  );
}

HRESULT __stdcall IGAIDirect3DDevice8::SetTransform(  D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)  
{  
	//-------------------------------------------------------
	if( g_bInit3D )
	{
		g_pIGA3DGetMatrixData( State, pMatrix );

		if(  g_RenderPos == Per_SetTransform && State == D3DTS_WORLD )
		{
			g_pIGA3DRender();
		}
	}

	//D3DXMATRIX cubWorld;
	//D3DXMatrixTranslation( &cubWorld, 5,5,5);

	//--------------------------------------------------------
	/*if ( D3DTS_WORLD == State)
	{
		char szDebugInfo[512];
		sprintf(szDebugInfo, "_11 = %d, _12 = %d, _13 = %d, _14 = %d,_21 = %d, _22 = %d, _23 = %d, _24 = %d,_31 = %d, _32 = %d, _33 = %d, _34 = %d,_41 = %d, _42 = %d, _43 = %d, _44 = %d ", pMatrix->_11, pMatrix->_12, pMatrix->_13, pMatrix->_14, pMatrix->_21, pMatrix->_22, pMatrix->_23, pMatrix->_24, pMatrix->_31, pMatrix->_32, pMatrix->_33, pMatrix->_34, pMatrix->_41, pMatrix->_42, pMatrix->_43, pMatrix->_44);
		OutputDebugString(szDebugInfo);
	}*/
	
	return (m_pIDirect3DDevice8->SetTransform(State, pMatrix) );
}

HRESULT __stdcall IGAIDirect3DDevice8::GetTransform(  D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)  
{  return (m_pIDirect3DDevice8->GetTransform(State, pMatrix)  );}

HRESULT __stdcall IGAIDirect3DDevice8::MultiplyTransform(  D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix)  
{  return (m_pIDirect3DDevice8->MultiplyTransform(  State,pMatrix) );}

HRESULT __stdcall IGAIDirect3DDevice8::SetViewport(  CONST D3DVIEWPORT8* pViewport)  
{  return (m_pIDirect3DDevice8->SetViewport( pViewport)  );}

HRESULT __stdcall IGAIDirect3DDevice8::GetViewport(  D3DVIEWPORT8* pViewport)  
{  return (m_pIDirect3DDevice8->GetViewport(pViewport)  );}

HRESULT __stdcall IGAIDirect3DDevice8::SetMaterial(  CONST D3DMATERIAL8* pMaterial)  
{  return (m_pIDirect3DDevice8->SetMaterial( pMaterial) );}

HRESULT __stdcall IGAIDirect3DDevice8::GetMaterial(  D3DMATERIAL8* pMaterial)  
{  return (m_pIDirect3DDevice8->GetMaterial(pMaterial) );}

HRESULT __stdcall IGAIDirect3DDevice8::SetLight(  DWORD Index,CONST D3DLIGHT8* pLight)  
{  return (m_pIDirect3DDevice8->SetLight(Index, pLight) );}

HRESULT __stdcall IGAIDirect3DDevice8::GetLight(  DWORD Index,D3DLIGHT8* pLight)  
{  return (m_pIDirect3DDevice8->GetLight(Index, pLight) );}

HRESULT __stdcall IGAIDirect3DDevice8::LightEnable(  DWORD Index,BOOL Enable)  
{  return (m_pIDirect3DDevice8->LightEnable(Index, Enable)  );}

HRESULT __stdcall IGAIDirect3DDevice8::GetLightEnable(  DWORD Index,BOOL* pEnable)  
{  return (m_pIDirect3DDevice8->GetLightEnable(Index, pEnable) );}

HRESULT __stdcall IGAIDirect3DDevice8::SetClipPlane(  DWORD Index,CONST float* pPlane)  
{  return (m_pIDirect3DDevice8->SetClipPlane(Index,  pPlane) );}

HRESULT __stdcall IGAIDirect3DDevice8::GetClipPlane(  DWORD Index,float* pPlane)
{  return (m_pIDirect3DDevice8->GetClipPlane(Index, pPlane));}

HRESULT __stdcall IGAIDirect3DDevice8::SetRenderState(  D3DRENDERSTATETYPE State,DWORD Value)  
{  return (m_pIDirect3DDevice8->SetRenderState(State, Value)  );}

HRESULT __stdcall IGAIDirect3DDevice8::GetRenderState(  D3DRENDERSTATETYPE State,DWORD* pValue)  
{  return (m_pIDirect3DDevice8->GetRenderState(State, pValue) );}

HRESULT __stdcall IGAIDirect3DDevice8::BeginStateBlock(void)  
{  return (m_pIDirect3DDevice8->BeginStateBlock() );}

HRESULT __stdcall IGAIDirect3DDevice8::EndStateBlock(  DWORD* pToken)  
{  return (m_pIDirect3DDevice8->EndStateBlock(pToken));}

HRESULT __stdcall IGAIDirect3DDevice8::ApplyStateBlock(  DWORD Token)  
{  return (m_pIDirect3DDevice8->ApplyStateBlock(Token) );}

HRESULT __stdcall IGAIDirect3DDevice8::CaptureStateBlock(  DWORD Token)  
{  return (m_pIDirect3DDevice8->CaptureStateBlock(Token) );  }

HRESULT __stdcall IGAIDirect3DDevice8::DeleteStateBlock(  DWORD Token)  
{  return (m_pIDirect3DDevice8->DeleteStateBlock(Token) );  }

HRESULT __stdcall IGAIDirect3DDevice8::CreateStateBlock(  D3DSTATEBLOCKTYPE Type,DWORD* pToken)  
{  return (m_pIDirect3DDevice8->CreateStateBlock(Type, pToken)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::SetClipStatus(  CONST D3DCLIPSTATUS8* pClipStatus)  
{  return (m_pIDirect3DDevice8->SetClipStatus( pClipStatus)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetClipStatus(  D3DCLIPSTATUS8* pClipStatus)  
{  return (m_pIDirect3DDevice8->GetClipStatus(pClipStatus)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetTexture(  DWORD Stage,IDirect3DBaseTexture8** ppTexture)  
{  return (m_pIDirect3DDevice8->GetTexture(Stage, ppTexture)  );  }

HRESULT IGAIDirect3DDevice8::SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture)  
{
	//OutputDebugString("SetTexture.....");
	HRESULT hr;
	hr = m_pIDirect3DDevice8->SetTexture(Stage, pTexture);
	if (pTexture == NULL)
		return S_FALSE;

	IDirect3DTexture8* tex;
	hr = pTexture->QueryInterface(IID_IDirect3DTexture8, (void **) &tex);
	if (SUCCEEDED(hr))
	{
		//OutputDebugString("开始纹理计算！跳转到OnSetTexture");
		pTexture = GetIGACore()->OnSetTexture(Stage, tex);
		tex->Release();
	}
	else
	{
		// TODO: Failed!!!
	}
	return hr;
}

HRESULT __stdcall IGAIDirect3DDevice8::GetTextureStageState(  DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)  
{  return (m_pIDirect3DDevice8->GetTextureStageState(Stage, Type, pValue)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::SetTextureStageState(  DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)  
{  return (m_pIDirect3DDevice8->SetTextureStageState(Stage, Type, Value));  }

HRESULT __stdcall IGAIDirect3DDevice8::ValidateDevice(  DWORD* pNumPasses)  
{  return (m_pIDirect3DDevice8->ValidateDevice(pNumPasses)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetInfo(  DWORD DevInfoID,void* pDevInfoStruct,DWORD DevInfoStructSize)  
{  return (m_pIDirect3DDevice8->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::SetPaletteEntries(  UINT PaletteNumber,CONST PALETTEENTRY* pEntries)  
{  return (m_pIDirect3DDevice8->SetPaletteEntries(PaletteNumber,  pEntries) );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetPaletteEntries(  UINT PaletteNumber,PALETTEENTRY* pEntries)  
{  return (m_pIDirect3DDevice8->GetPaletteEntries(PaletteNumber, pEntries)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::SetCurrentTexturePalette(  UINT PaletteNumber)  
{  return (m_pIDirect3DDevice8->SetCurrentTexturePalette(PaletteNumber)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetCurrentTexturePalette(  UINT *PaletteNumber)  
{  return (m_pIDirect3DDevice8->GetCurrentTexturePalette(  PaletteNumber) );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawPrimitive(  D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)  
{  return (m_pIDirect3DDevice8->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawIndexedPrimitive(  D3DPRIMITIVETYPE Type,UINT minIndex,UINT NumVertices,UINT startIndex,UINT primCount)  
{  return (m_pIDirect3DDevice8->DrawIndexedPrimitive(  Type, minIndex, NumVertices, startIndex, primCount)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawPrimitiveUP(  D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)  
{  return (m_pIDirect3DDevice8->DrawPrimitiveUP(PrimitiveType, PrimitiveCount,  pVertexStreamZeroData, VertexStreamZeroStride)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawIndexedPrimitiveUP(  D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertexIndices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)  
{  return (m_pIDirect3DDevice8->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount,  pIndexData, IndexDataFormat,  pVertexStreamZeroData, VertexStreamZeroStride));  }

HRESULT __stdcall IGAIDirect3DDevice8::ProcessVertices(  UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer8* pDestBuffer,DWORD Flags)  
{  return (m_pIDirect3DDevice8->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::CreateVertexShader(  CONST DWORD* pDeclaration,CONST DWORD* pFunction,DWORD* pHandle,DWORD Usage)  
{  return (m_pIDirect3DDevice8->CreateVertexShader( pDeclaration, pFunction, pHandle, Usage));  }

HRESULT __stdcall IGAIDirect3DDevice8::SetVertexShader(  DWORD Handle)  
{  return (m_pIDirect3DDevice8->SetVertexShader(Handle) );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetVertexShader(  DWORD* pHandle)  
{  return (m_pIDirect3DDevice8->GetVertexShader(pHandle) );  }

HRESULT __stdcall IGAIDirect3DDevice8::DeleteVertexShader(  DWORD Handle)  
{  return (m_pIDirect3DDevice8->DeleteVertexShader(Handle));  }

HRESULT __stdcall IGAIDirect3DDevice8::SetVertexShaderConstant(  DWORD Register,CONST void* pConstantData,DWORD ConstantCount)  
{  return (m_pIDirect3DDevice8->SetVertexShaderConstant(Register,  pConstantData, ConstantCount)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetVertexShaderConstant(  DWORD Register,void* pConstantData,DWORD ConstantCount)  
{  return (m_pIDirect3DDevice8->GetVertexShaderConstant(Register, pConstantData, ConstantCount));  }

HRESULT __stdcall IGAIDirect3DDevice8::GetVertexShaderDeclaration(  DWORD Handle,void* pData,DWORD* pSizeOfData)  
{  return (m_pIDirect3DDevice8->GetVertexShaderDeclaration(Handle, pData, pSizeOfData));  }

HRESULT __stdcall IGAIDirect3DDevice8::GetVertexShaderFunction(  DWORD Handle,void* pData,DWORD* pSizeOfData)  
{  return (m_pIDirect3DDevice8->GetVertexShaderFunction(Handle, pData, pSizeOfData));  }

HRESULT __stdcall IGAIDirect3DDevice8::SetStreamSource(  UINT StreamNumber,IDirect3DVertexBuffer8* pStreamData,UINT Stride)  
{  return (m_pIDirect3DDevice8->SetStreamSource(StreamNumber, pStreamData, Stride));  }

HRESULT __stdcall IGAIDirect3DDevice8::GetStreamSource(  UINT StreamNumber,IDirect3DVertexBuffer8** ppStreamData,UINT* pStride)  
{  return (m_pIDirect3DDevice8->GetStreamSource(StreamNumber, ppStreamData, pStride));  }

HRESULT __stdcall IGAIDirect3DDevice8::SetIndices(  IDirect3DIndexBuffer8* pIndexData,UINT BaseVertexIndex)  
{  return (m_pIDirect3DDevice8->SetIndices(pIndexData, BaseVertexIndex));  }

HRESULT __stdcall IGAIDirect3DDevice8::GetIndices(  IDirect3DIndexBuffer8** ppIndexData,UINT* pBaseVertexIndex)  
{  return (m_pIDirect3DDevice8->GetIndices(ppIndexData, pBaseVertexIndex) );  }

HRESULT __stdcall IGAIDirect3DDevice8::CreatePixelShader(  CONST DWORD* pFunction,DWORD* pHandle)  
{  return (m_pIDirect3DDevice8->CreatePixelShader( pFunction, pHandle));  }

HRESULT __stdcall IGAIDirect3DDevice8::SetPixelShader(  DWORD Handle)  
{  return (m_pIDirect3DDevice8->SetPixelShader(Handle) );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetPixelShader(  DWORD* pHandle)  
{  return (m_pIDirect3DDevice8->GetPixelShader(pHandle));  }

HRESULT __stdcall IGAIDirect3DDevice8::DeletePixelShader(  DWORD Handle)  
{  return (m_pIDirect3DDevice8->DeletePixelShader(Handle)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::SetPixelShaderConstant(  DWORD Register,CONST void* pConstantData,DWORD ConstantCount)  
{  return (m_pIDirect3DDevice8->SetPixelShaderConstant(Register,  pConstantData, ConstantCount)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::GetPixelShaderConstant(  DWORD Register,void* pConstantData,DWORD ConstantCount)  
{  return (m_pIDirect3DDevice8->GetPixelShaderConstant(Register, pConstantData, ConstantCount));  }

HRESULT __stdcall IGAIDirect3DDevice8::GetPixelShaderFunction(  DWORD Handle,void* pData,DWORD* pSizeOfData)  
{  return (m_pIDirect3DDevice8->GetPixelShaderFunction(Handle, pData, pSizeOfData)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawRectPatch(  UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo)  
{  return (m_pIDirect3DDevice8->DrawRectPatch(Handle,  pNumSegs,  pRectPatchInfo)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DrawTriPatch(  UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo)  
{  return (m_pIDirect3DDevice8->DrawTriPatch(Handle,  pNumSegs,  pTriPatchInfo)  );  }

HRESULT __stdcall IGAIDirect3DDevice8::DeletePatch(  UINT Handle)  
{  return (m_pIDirect3DDevice8->DeletePatch(Handle)  );  }
