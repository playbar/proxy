#include <iostream>
#include <sstream>
#include <fstream>
#include <io.h>
#include <list>
#include "windows.h"
using namespace std;

//数据目录描述结构体
struct DataDescribe
{
	char   name[32];
	DWORD  size;
	DWORD  start_addres;
	DWORD  end_address;
};

//----------------------------------------------------------------------------
//数据包类
//创建数据包文件并管理数据包中的资源
//为使用者提供从创建的数据包中提取需要的数据文件
//更新管理数据包中的数据
//删除数据包中无效的数据
class CPack
{
public:
	CPack();	//只初始化要创建包的名称
	~CPack();

//---------------------------------------------------
//创建和加载数据目录
public:	
	HRESULT Init( char *name, char *path );

	//初始化数据目录到内存,加载目录文件到内存
	void LoadDataRootFromFile();
	
	//释放目录信息
	void ReleaseRoot();

//---------------------------------------------------
//更新数据包，完成后要创建新目录，并且重新加载新目录
public:
	//从内存目录信息创建目录文件
	void CreateFileRootFromMemory();

	//更新数据包文件（用单个文件数据）
	void UpDataPackFromMemory( char *name, char *memory, DWORD size );		//从内存创建数据更新到数据包
	void UpDataPackFromFile( char *name, char *path );						//指定路径下的文件数据包中更新

	//获取数据文件
	void GetDataFromPack( char *name, char* &outData, DWORD *size );

protected:
	list<DataDescribe*>    m_DataRoot;		//数据目录
	

protected:
	//文件数据的端口
	ifstream				data_in;
	ofstream				data_out;
	char					data_name[256];

	//文件目录信息的端口
	ifstream				root_in;
	ofstream				root_out;
	char					data_root[256];

	//更新日志
	ofstream				date_txt;	
	char					date[32];
};

class CPackTool : public CPack
{
public:
	CPackTool();
	~CPackTool();

public:
	//从指定的文件夹创建文件数据包
	void CreateDataPackFromFilePath( char *path );	

	//打印文件目录
	void OutputRootToFile();

	//输出数据到文件夹
	void OutputDataToFile( char* path );
};

