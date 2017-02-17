#include <d3d8.h>
#include <d3dx8.h>

class CIGACamera
{
public:
	CIGACamera();
	~CIGACamera();

	void ComputeCamera( D3DXMATRIX* pmatView, D3DXMATRIX* pmatProj );
	bool InViewCentrum( D3DXVECTOR3* pv );
	bool GetCameraPt( D3DXVECTOR3* pvCameraPt );

	void SetClipDistance( float fDist );

protected:
	void UpdataViewCentrum();

private:
	D3DXMATRIX		m_matView;	//视图矩阵
	D3DXMATRIX		m_matProj;	//投影矩阵

	//得到相关参数
	D3DXVECTOR3		m_vLookAtDir;	//摄影机的观察方向
	D3DXVECTOR3		m_vUpDir;		//摄影机的上方向
	D3DXVECTOR3		m_vRightDir;    //摄影机的有方向
	D3DXVECTOR3     m_vCameraPoint;    //摄影机的位置

	//视椎体
	D3DXVECTOR3		m_vtx[8];
	D3DXVECTOR3		m_vViewCentrum[8];

	//裁剪有效距离及裁剪角度
	double			m_dbClipDistance;
	float			m_fHorizontalClipAngle;	//水平裁剪角
	float			m_fVerticalClipAngle;	//垂直裁剪角
};

extern CIGACamera g_CIGACamera;

