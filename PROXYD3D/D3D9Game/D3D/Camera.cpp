

#include "../Game.h"


CCamera * g_pCamera;

void CCamera::Init( float fMinDis, float fMaxDis, float fAngleXZMin, float fAngleXZMax )
{
	m_pDevice = g_pMyD3D->GetDevice();

	m_vFirstPos = m_vPos = D3DXVECTOR3( 1000, 50, 1000 );
	m_vFirstLookAt = m_vLookAt = m_vDesPos = D3DXVECTOR3( 1000, 50, 1020 );
	m_vTop = D3DXVECTOR3( 0, 1, 0 );

	m_fFirstAngleY = m_fAngleY = 0;

	m_fFirstAngleXZ = 0;

	m_fDistance = sqrt( pow( m_vPos.x - m_vDesPos.x, 2 ) + pow( m_vPos.y - m_vDesPos.y, 2 ) + pow( m_vPos.z - m_vDesPos.z, 2 ) );

	m_fMinDis = fMinDis;
	m_fMaxDis = fMaxDis;

	m_fAngleXZMin = fAngleXZMin;
	m_fAngleXZMax = fAngleXZMax;
}

void CCamera::AllRotate( const D3DXMATRIX & matTrans )
{
	APP_Tools::appVec3Transform( m_vPos, matTrans );
	APP_Tools::appVec3Transform( m_vLookAt, matTrans );
}

void CCamera::AllTranslate( const D3DXMATRIX & matTrans )
{
	APP_Tools::appVec3Transform( m_vPos, matTrans );
	APP_Tools::appVec3Transform( m_vLookAt, matTrans );
}

void CCamera::RotateX( float fAngle )
{
	D3DXMATRIX matTrans;
	D3DXMatrixRotationX( &matTrans, fAngle );

	AllRotate( matTrans );
}
void CCamera::RotateY( float fAngle )
{
	D3DXMATRIX matTrans;
	D3DXMatrixRotationY( &matTrans, fAngle );

	AllRotate( matTrans );
}
void CCamera::RotateZ( float fAngle )
{
	D3DXMATRIX matTrans;
	D3DXMatrixRotationZ( &matTrans, fAngle );

	AllRotate( matTrans );
}

void CCamera::Rotate( float fAngleY, float fAngleXZ )
{
	m_fAngleY += fAngleY;
	if( m_fAngleY > D3DX_PI )
		m_fAngleY -= D3DX_PI * 2;
	else if( m_fAngleY < -D3DX_PI )
		m_fAngleY += D3DX_PI * 2;

	m_fAngleXZ += fAngleXZ;
}


void CCamera::Translate( float x, float y, float z )
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, x, y, z );

	AllTranslate( matTrans );
}
void CCamera::Translate( const D3DXVECTOR3 & vTrans )
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation( &matTrans, vTrans.x, vTrans.y, vTrans.z );

	AllTranslate( matTrans );
}

void CCamera::Translate( float fDistance )
{
	if( m_fDistance + fDistance > m_fMaxDis )
	{
		fDistance = m_fMaxDis - m_fDistance;
		m_fDistance = m_fMaxDis;
	}
	else if( m_fDistance + fDistance < m_fMinDis )
	{
		fDistance = m_fMinDis - m_fDistance;
		m_fDistance = m_fMinDis;
	}
	else
		m_fDistance += fDistance;

}

void CCamera::ToWorld()
{
	m_vPos = m_vFirstPos;

	//根据距离计算pos
	float fDis = D3DXVec3Length( &(m_vFirstPos - m_vFirstLookAt) ) / m_fDistance;

	m_vPos.x = ( m_vFirstPos.x - m_vDesPos.x ) / fDis + m_vDesPos.x;
	m_vPos.y = ( m_vFirstPos.y - m_vDesPos.y ) / fDis + m_vDesPos.y;
	m_vPos.z = ( m_vFirstPos.z - m_vDesPos.z ) / fDis + m_vDesPos.z;

	//将目标点平移到原点
	D3DXVECTOR3 vTrans = m_vDesPos;
	Translate( -vTrans );

	//延y轴旋转
	RotateY( m_fAngleY );


	//仰角旋转
	float fAngleY = 0.0f;
	if( m_vPos.x != 0 )
		fAngleY = atan( m_vPos.z / m_vPos.x );
	else
	{
		if( m_vPos.z > 0 )
			fAngleY = D3DX_PI / 2;
		else
			fAngleY = -D3DX_PI / 2;
	}

	if( m_vPos.x < 0 )
		fAngleY = D3DX_PI + fAngleY;

	RotateY( fAngleY );


	if( m_fAngleXZ + m_fFirstAngleXZ > m_fAngleXZMax )
	{
		m_fAngleXZ = m_fAngleXZMax - m_fFirstAngleXZ;
	}
	else if( m_fAngleXZ + m_fFirstAngleXZ < m_fAngleXZMin )
	{
		m_fAngleXZ = m_fAngleXZMin - m_fFirstAngleXZ;
	}

	RotateZ( m_fAngleXZ );

	RotateY( -fAngleY );

	//将目标点平移回原位
	Translate( vTrans );

	m_vLookAt = m_vDesPos;
}

void CCamera::UpdateCamera()
{
	ToWorld();

	if( g_pGame->m_pTerrain )
	{
		if( m_vPos.y <= m_vDesPos.y )
		{
			m_vPos.y -= 0.35f;
			RAY ray;
			ray.vOrig = m_vDesPos;
			D3DXVec3Normalize( &ray.vVector, &(m_vPos - m_vDesPos) );
			if( g_pGame->m_pTerrain->RayCheck( ray, D3DXVec3Length( &(m_vPos - m_vDesPos) ), m_vPos ) )
				m_vPos.y += 0.35f;
			else
				m_vPos.y += 0.35f;
		}
	}
	D3DXMatrixLookAtLH( &m_matView, &m_vPos, &m_vLookAt, &m_vTop );
	m_pDevice->SetTransform( D3DTS_VIEW, &m_matView );
	UpdateViewport();
}


void CCamera::GetRay( int x, int y, LPRAY ray )
{
	ray->vOrig = m_vPos;
	RECT rect;
	GetClientRect( g_pMyD3D->GetHWND(), &rect );

	D3DXMATRIXA16 matProj;
	m_pDevice->GetTransform( D3DTS_PROJECTION , &matProj );

	//计算屏幕空间的坐标(-1,-1)～(1,1)
	D3DXVECTOR3 v;
	v.x =  ( 2 * x  / (float)rect.right - 1 ) / matProj._11;
	v.y =  -( 2 * y  / (float)rect.bottom - 1 ) / matProj._22;
	v.z =  1.0f;

	//获得视图变换矩阵的逆矩阵
	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &m_matView );

	//把上述坐标转换成世界坐标
	D3DXVECTOR3 vTmp = v;
	v.x = vTmp.x * m.m[0][0] + vTmp.y * m.m[1][0] + vTmp.z * m.m[2][0] ;
	v.y = vTmp.x * m.m[0][1] + vTmp.y * m.m[1][1] + vTmp.z * m.m[2][1] ;
	v.z = vTmp.x * m.m[0][2] + vTmp.y * m.m[1][2] + vTmp.z * m.m[2][2] ;

	ray->vVector = v;
	
}

void CCamera::GetViewRay( LPRAY ray )
{
	ray->vOrig = m_vPos;

	D3DXMATRIXA16 m;
	D3DXMatrixInverse( &m, NULL, &m_matView );

	ray->vVector.x = m.m[2][0];
	ray->vVector.y = m.m[2][1];
	ray->vVector.z = 1.f;
}
