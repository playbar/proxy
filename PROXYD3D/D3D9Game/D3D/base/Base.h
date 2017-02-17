

#pragma once


#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9core.h>



enum COLOR_STATE
{
	COLOR_NONE,
	COLOR_SELECT_DIFFUSE,
	COLOR_SELECT_TEXTURE,
	COLOR_MODULATE_TEXTURE_CURRENT,
	COLOR_MODULATE_TEXTURE_DIFFUSE,
	COLOR_MODULATE_DIFFUSE_CURRENT,
};


enum ALPHA_STATE
{
	ALPHA_NONE,
	ALPHA_SELECT_DIFFUSE,
	ALPHA_SELECT_TEXTURE,
	ALPHA_MODULATE_TEXTURE_DIFFUSE,
	ALPHA_SPECULAR,
};

enum ALPHABLEND_STATE
{
	ALPHABLEND_NONE,
	ALPHABLEND_SRCALPHA_INVSRCALPHA,		  //src*alpha+des*(1-alpha)
	ALPHABLEND_ONE_ONE,						  //加亮混合
	ALPHABLEND_SRCCOLOR_ONE,				  //通过texture控制亮度
	ALPHABLEND_SRCALPHA_ONE,				  //同上
	ALPHABLEND_DEST_ZERO,					  //高光，color*color
	ALPHABLEND_DEST_SRC,					  //高光，color*color*2
};

enum FILTER_STATE
{
	FILTER_NONE,
	FILTER_POINT,
	FILTER_LINEAR,
	FILTER_ANISOTROPIC,
};

enum CULL_STATE
{
	CULL_NONE,
	CULL_CCW,
	CULL_CW,
};


struct RENDER_STATE
{
	COLOR_STATE color;
	ALPHA_STATE alpha;
	ALPHABLEND_STATE blend;
	FILTER_STATE filter;
	CULL_STATE cull;
	bool		bZWrite;
	bool		bAlphaTest;
	DWORD		dwCustomColor;
};




//世界物体基类
class CBase
{
public:
	CBase(){}
	~CBase(){}
	virtual void Render() = 0;
	DWORD GetId()		{ return m_dwBaseId; }
	DWORD GetType()		{ return m_dwBaseType; }
	DWORD GetFlag()		{ return m_dwBaseFlag; }
	float GetRotationY(){return m_fAngle;}

	void SetPos( D3DXVECTOR3 vPos )	{m_vPos = vPos;}
	void SetPos( float x, float y, float z )
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void GetPos( D3DXVECTOR3 & vPos )	{vPos = m_vPos;}
	D3DXVECTOR3 * GetPos()				{return &m_vPos;}

	void SetRotationY( float fAngleY )	{m_fAngle = fAngleY;}
	void RotationY( float fAngleY )		{m_fAngle += fAngleY;}
	void SetScaling( float fScaling )	{m_fScaling = fScaling;}

	inline void SetCustomRenderState( COLOR_STATE color, ALPHA_STATE alpha, ALPHABLEND_STATE blend, FILTER_STATE filter, CULL_STATE cull, bool bZWrite, bool bAlphaTest, DWORD dwCustomColor )
	{
		m_RenderState.color = color;
		m_RenderState.alpha = alpha;
		m_RenderState.blend = blend;
		m_RenderState.filter = filter;
		m_RenderState.cull = cull;
		m_RenderState.bZWrite = bZWrite;
		m_RenderState.bAlphaTest = bAlphaTest;
		m_RenderState.dwCustomColor =dwCustomColor;
	}
	inline void SetCustomRenderState( COLOR_STATE color )		{m_RenderState.color = color;}
	inline void SetCustomRenderState( ALPHA_STATE alpha )		{m_RenderState.alpha = alpha;}
	inline void SetCustomRenderState( ALPHABLEND_STATE blend )	{m_RenderState.blend = blend;}
	inline void SetCustomRenderState( FILTER_STATE filter )		{m_RenderState.filter = filter;}
	inline void SetCustomRenderState( bool bZWrite, bool bAlphaTest )
	{
		m_RenderState.bZWrite = bZWrite;
		m_RenderState.bAlphaTest = bAlphaTest;
	}
	inline void SetCustomRenderState( DWORD dwCustomColor )		{m_RenderState.dwCustomColor = dwCustomColor;}
	inline void SetCustomRenderState( CULL_STATE cull )			{m_RenderState.cull = cull;}


	void UseRenderState()
	{
		if( m_RenderState.bZWrite )
		{
			m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		}
		else
		{
			m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		}

		switch( m_RenderState.color )
		{
		case COLOR_NONE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
			}
			break;
		case COLOR_SELECT_DIFFUSE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			}
			break;
		case COLOR_SELECT_TEXTURE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
			}
			break;
		case COLOR_MODULATE_TEXTURE_CURRENT:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			}
			break;
		case COLOR_MODULATE_TEXTURE_DIFFUSE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			}
			break;
		case COLOR_MODULATE_DIFFUSE_CURRENT:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
			}
		default:
			break;

		}

		switch( m_RenderState.alpha )
		{
		case ALPHA_NONE:
			{
				m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			}
			break;
		case ALPHA_SELECT_DIFFUSE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			}
			break;
		case ALPHA_SELECT_TEXTURE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE );
			}
			break;
		case ALPHA_MODULATE_TEXTURE_DIFFUSE:
			{
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			}
			break;
		case ALPHA_SPECULAR:
			{
				m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_RenderState.dwCustomColor );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR );
				m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
			}
			break;
		default:
			break;
		}

		if( m_RenderState.blend != ALPHABLEND_NONE )
			m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

		switch( m_RenderState.blend )
		{
		case ALPHABLEND_NONE:
			{
				m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			}
			break;
		case ALPHABLEND_SRCALPHA_INVSRCALPHA:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			}
			break;
		case ALPHABLEND_ONE_ONE:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			}
			break;
		case ALPHABLEND_SRCCOLOR_ONE:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			}
			break;
		case ALPHABLEND_SRCALPHA_ONE:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
			}
			break;
		case ALPHABLEND_DEST_ZERO:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTALPHA );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
			}
			break;
		case ALPHABLEND_DEST_SRC:
			{
				m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DTOP_DISABLE );
				m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTALPHA );
				m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
			}
			break;
		default:
			break;
		}

		switch( m_RenderState.cull )
		{
		case CULL_NONE:
			{
				m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			}
			break;
		case CULL_CCW:
			{
				m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			}
			break;
		case CULL_CW:
			{
				m_pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
			}
			break;
		default:
			break;
		}

		switch( m_RenderState.filter )
		{
		case FILTER_NONE:
			{
				m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
				m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
			}
			break;
		case FILTER_POINT:
			{
				m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
				m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			}
			break;
		case FILTER_LINEAR:
			{
				m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
				m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			}
			break;
		case FILTER_ANISOTROPIC:
			{
				m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC );
				m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
			}
			break;
		default:
			break;
		}

		if( m_RenderState.bAlphaTest )
		{
			m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
			m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00000088 );
			m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		}
		else
		{
			m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
		}
	}

private:

protected:

	DWORD			m_dwBaseId;
	DWORD			m_dwBaseType;
	DWORD			m_dwBaseFlag;
	RENDER_STATE	m_RenderState;
	LPDIRECT3DDEVICE9 m_pDevice;
	D3DXVECTOR3				m_vPos;
	float					m_fAngle;
	float					m_fScaling;

};