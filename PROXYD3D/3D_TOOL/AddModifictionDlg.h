#pragma once
#include "xviewctrl_add.h"
#include "afxcmn.h"

#include <io.h>
#include <iostream>
#include <map>
#include <list>
#include <math.h>

using namespace std;

//-------------------------------------------------
/*
	3D数据结构
*/
struct S3DDATA
{
	int		obj_id;
	int		mesh_id;
	char	mesh_name[MAX_PATH];
	
	float	x;
	float	y;
	float	z;

	float	scale_xyz;

	float	rot_x;
	float	rot_y;
	float	rot_z;

	char	url[MAX_PATH];

	S3DDATA()
	{
		obj_id		= -1;
		mesh_id		= -1;

		x	= 0.0f;
		y	= 0.0f;
		z	= 0.0f;

		scale_xyz	= 1.0f;

		rot_x	= 0.0f;
		rot_y	= 0.0f;
		rot_z	= 0.0f;

		url[0] = '\0';
		mesh_name[0] = '\0';
	}

	S3DDATA( const S3DDATA& data )
	{
		obj_id		= data.obj_id;
		mesh_id		= data.mesh_id;

		x	= data.x;
		y	= data.y;
		z	= data.z;

		scale_xyz	= data.scale_xyz;

		rot_x	= data.rot_x;
		rot_y	= data.rot_y;
		rot_z	= data.rot_z;

		strcpy( url, data.url );
		strcpy( mesh_name, data.mesh_name );
	}
};

//-----------------------------------------------------
// AddModifictionDlg 对话框

class AddModifictionDlg : public CDialog
{
	DECLARE_DYNAMIC(AddModifictionDlg)

public:
	AddModifictionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddModifictionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_MeshName;
	CString m_URL;
private:
	CXviewctrl_add m_XView;
	CListCtrl m_MeshList;
	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fScaling;
	float m_fPositionOffset;
	float m_fRotationOffset;
	float m_fScalingOffset;

private:
	//CStringList		m_cstrMeshList;

	/*
		first:是模型的名称；
		second:是模型的绝对路径
	*/
	typedef map<CString,CString>	MESH_NAME_MAP;
	typedef map<CString,CString>::iterator LPNODE;
	typedef pair<CString,CString>   MESH_NODE;
	MESH_NAME_MAP	m_MeshFullPathName;
	
	//...
	S3DDATA	   m_MeshInfo;


public:
	virtual BOOL OnInitDialog();
	int FindResource(CString cstrResourcePath);
	afx_msg void OnNMClickListAdd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddOk();
	afx_msg void OnBnClickedAddCancel();
	S3DDATA& Get3DData(void);
	afx_msg void OnEnChangeEditAddUrl();
	void SetCurData( S3DDATA& data );
	afx_msg void OnBnClickedAddUpdata();
	
	afx_msg void OnOK(){};
	afx_msg void OnCancel(){};
	afx_msg void OnBnClickedButtonAddGetposition();
	afx_msg void OnDeltaposSpinAddPositionX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAddPositionY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPositionZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRotationX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinrotationY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAddRotationZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinAddScaling(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddOpenUrl();
	CString GetMeshPath(CString MeshName);
	afx_msg void OnClose();
};
