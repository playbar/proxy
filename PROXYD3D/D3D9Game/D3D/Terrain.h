

#pragma once

#include <d3dx9.h>
#include <d3d9.h>

#include "D3D.h"
#include "StaticMesh.h"
#include "SkinMesh.h"
#include "Base/Texture.h"
#include "BillBoard.h"



#define D3DFVF_TERRAIN	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )	//坐标信息

class CTerrain : public CBase
{
protected:
	struct VERTEX_TERRAIN
	{
		float	x,y,z;			//坐标信息
		float   nx,ny,nz;
		float	tu,tv;
	};
protected:
	
	CTexture *					m_pTexture;				//材质
	LPDIRECT3DVERTEXBUFFER9		m_pVB;//顶点缓冲
	LPDIRECT3DINDEXBUFFER9		m_pIB;//索引顶点缓冲
	long						m_nCol;
	long						m_nRow;//行列
	float						m_maxHeight;//最大高度
	float						m_fCellWidth;//单元宽度
	BYTE *						m_pHeightData;//高度指针
	D3DMATERIAL9				m_matMaterial;
	VERTEX_TERRAIN *			m_pVerticesBackUp;



	Array<CStaticMeshObj *>		m_pStaticMeshObj;
	Array<CStaticMesh *>		m_pStaticMesh;

	Array<CSkinMeshObj *>		m_pSkinMeshObj;
	Array<CSkinMesh *>			m_pSkinMesh;

public:
	CTerrain( D3DVECTOR position );//构造函数
	virtual ~CTerrain();//析构函数
    bool InitialiseLight(int LightType);//初始化灯光
	void CreateTerrain(TCHAR* strTerrain,TCHAR* strTexture, float cellWidth,float height=800);//创建地形
	float GetHeight(float x,float z);//得到地形长宽
	void Render();//渲染地形
	void SetMatrix();//变换
	bool SetMaterial( D3DCOLORVALUE rgbaDiffuse, D3DCOLORVALUE rgbaAmbient, D3DCOLORVALUE rgbaSpecular, D3DCOLORVALUE rgbaEmissive, float rPower );
	bool LineCheck( D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2, D3DXVECTOR3 & vOut );
	bool MoveCheck( RAY ray );
	bool RayCheck( RAY ray, float length, D3DXVECTOR3 & vOut );
	void GetFourVertices( D3DXVECTOR3 & v0, D3DXVECTOR3 & v1, D3DXVECTOR3 & v2, D3DXVECTOR3 & v3, int iCol, int iRow );

};
