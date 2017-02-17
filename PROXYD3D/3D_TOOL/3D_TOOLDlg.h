// 3D_TOOLDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "AddModifictionDlg.h"
#include "SetDlg.h"
#include "afxwin.h"
#include <map>
#include <iostream>
#include <fstream>

#include <direct.h>

using namespace std;

/*
	这个结构体用来记录模型的名称、使用的纹理的名称。
*/
struct SFileCopy
{
	static int nCount;
	int		mesh_id;
    bool	bIsCopy;
	CString     MeshName;
	CStringList NameList;
	SFileCopy()
	{
		mesh_id = -1;
		bIsCopy = false;
		MeshName = "";
		NameList.RemoveAll();
	}

	SFileCopy( const SFileCopy& fileCopyObj )
	{
		mesh_id = fileCopyObj.mesh_id;
		bIsCopy = fileCopyObj.bIsCopy;
		MeshName = fileCopyObj.MeshName;
		POSITION   pos = fileCopyObj.NameList.GetHeadPosition();
		while( pos != NULL )
		{
			CString str =  fileCopyObj.NameList.GetNext(pos);
			NameList.AddHead(str);
		}
	}

	~SFileCopy()
	{
		NameList.RemoveAll();
	}
};

typedef map<CString,SFileCopy>	MESH_COPY_MAP;
typedef map<CString,SFileCopy>::iterator LPCOPYNODE;
typedef pair<CString,SFileCopy> COPY_NODE;

//----------------------------------------------------------------------------
// CD_TOOLDlg 对话框
class CD_TOOLDlg : public CDialog
{
// 构造
public:
	CD_TOOLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_D_TOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSet();
private:
	CString m_GameName;
	CString m_ResourcePath;
	CString m_ResourceOutputPath;
	CListCtrl m_3DInfoList;

	AddModifictionDlg	m_AddModifictionDlg;
	CSetDlg				m_SetDlg;

	/*
		索引号，模型的索引号从0开始，每添加一个就加1，删除模型不减少索引
	*/
	int m_nDataIndex;

	/*
		对象列表
	*/
	typedef map<int,S3DDATA>	DATA_MAP;
	typedef map<int,S3DDATA>::iterator LPNODE;
	typedef pair<int,S3DDATA>	DATA_NODE;

	DATA_MAP	m_DataMap;

	LPNODE	m_CurDataNode;
	int     m_nSelectIndex;

	/*
		更新data.ini、Adinfo.ini		
	*/
	void UpdateDataINI();
	void UpdataAdInfoXML();

	//模型文件的复制记录集合
	MESH_COPY_MAP	m_MeshCopyMap;

	/*
		被选择的对象ID，在向游戏里添加多个模型后，要修改时往往分不清楚哪一个模型（使用相同模型时）
		这里就需要每隔一段时间记录一次。
	*/
	

public:
	afx_msg void OnBnClickedButtonAdd();
private:
	void SetEnable(bool bIsEnable );
	CButton m_ButtonAdd;
	CButton m_ButtonDelete;
	CButton m_ButtonClear;
	CButton m_ButtonOutput;
	CButton m_ButtonSet;
	CButton m_ButtonQuit;
public:
	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnNMRclickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnOK(){};
	afx_msg void OnCancel(){};

	afx_msg void OnNMClickList3dinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonOutput();
	void UpdataCurList(S3DDATA& data);
	void GetMeshResNameList(CString name, CStringList* pCStringList);
	void Delete3DRes( bool bIsAll = true, CString name = "" );
	afx_msg void OnTimer(UINT nIDEvent);
private:
	int m_nCurPickID;
public:
	afx_msg void OnClose();
private:
	BOOL m_bShowPickMesh;
};
