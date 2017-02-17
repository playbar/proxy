#ifndef  _AD_3DFILTER_H
#define  _AD_3DFILTER_H
#include "D3DHelpFun.h"
#ifdef  USING_D3D8
   #include "D3d8Filter.h"
   #define Filter_IDirect3D          Filter_IDirect3D8 
   #define Filter_IDirect3DDevice    Filter_IDirect3DDevice8 
   #define Filter_IDirect3DTexture   Filter_IDirect3DTexture8 
   #define Filter_IDirect3DSurface   Filter_IDirect3DSurface8
#else ifdef  USING_D3D9
	#include "D3d9Filter.h"
	#define Filter_IDirect3D          Filter_IDirect3D9 
	#define Filter_IDirect3DDevice    Filter_IDirect3DDevice9
	#define Filter_IDirect3DTexture   Filter_IDirect3DTexture9 
	#define Filter_IDirect3DSurface   Filter_IDirect3DSurface9

#endif

class IGA_Filter_IDirect3D : public Filter_IDirect3D
{
public:
	IGA_Filter_IDirect3D();
	~IGA_Filter_IDirect3D();

	virtual   VOID    OnRelease_After      (ULONG  ulRefCount       );
	virtual   VOID    OnCreateDevice_After (IDirect3DDevice*  pDevice,HWND hFocusWindow,IDirect3DDevice* pDeviceOrg);
public:
	DWORD   m_dwRefCount;
};


/*设备过滤器*/
class IGA_Filter_IDirect3DDevice : public Filter_IDirect3DDevice
{
public:
	IGA_Filter_IDirect3DDevice();
	virtual ~IGA_Filter_IDirect3DDevice();

    virtual VOID OnRelease_After                 (IDirect3DDevice* pDevice,ULONG&  ulRefCount );	
	
	virtual VOID OnPresent_Before                (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual VOID OnPresent_After                 (IDirect3DDevice* pDevice, CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);

	virtual VOID OnReset_Before                  (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	virtual VOID OnReset_After                   (IDirect3DDevice* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice* pOrgDevice);

	virtual VOID OnSetTransform_Before           (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix); 
	virtual VOID OnSetTransform_After          (IDirect3DDevice* pDevice, D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);    


	virtual VOID OnSetTexture_Before             (IDirect3DDevice* pDevice, DWORD Stage, IDirect3DBaseTexture* pTexture);
public:
	
};

/*纹理*/
class IGA_Filter_IDirect3DTexture : public Filter_IDirect3DTexture
{
public:
	IGA_Filter_IDirect3DTexture();
	virtual   ~IGA_Filter_IDirect3DTexture();
	
    virtual   VOID    OnRelease_After      (IDirect3DDevice*pDevice,IDirect3DTexture*  pTexture,ULONG  ulRefCount       );	
    
};


/*表面过滤*/
#pragma warning(disable:4786)
#include <map>
class IGA_Filter_IDirect3DSurface : public  Filter_IDirect3DSurface
{
public:
	IGA_Filter_IDirect3DSurface();
	virtual   ~IGA_Filter_IDirect3DSurface();
	
 	virtual   VOID    OnLockRect_After (IDirect3DSurface* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags) ;	
    virtual   VOID    OnUnlockRect_Before(IDirect3DDevice* pDevice,IDirect3DSurface* pSurface) ;		
	
private:
	//zzhenfei当前锁定的surface
	struct SURFACE_PARAM
	{		
		INT             Pitch;
		void*           pBits;
		
		const RECT*     pRect;
		DWORD           Flags;
	};
	typedef std::map<VOID*, SURFACE_PARAM>              CUR_SURFACE_MAP; 
	typedef std::map<VOID*, SURFACE_PARAM>::iterator    CUR_SURFACE_MAP_IT; 
	
	CUR_SURFACE_MAP         m_lockSurface;

    IDirect3DTexture*   m_pTexture; 
	IDirect3DTexture*   m_pAdTexture; 
	
};


#endif
