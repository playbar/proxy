#include "stdafx.h"

#include "tchar.h"
#include "z_FileIni.h"
#include "pack.h"

//-------------------------------------------
CPack::CPack()
{
	m_listDataRoot.clear();
}

CPack::~CPack()
{
	ReleaseRoot();
}

HRESULT CPack::Init( char *name, char *path )
{
	//创建数据文件
	char szPathNamePd[256];
	strcpy( szPathNamePd, path );
	strcat( szPathNamePd, name );
	strcat( szPathNamePd, ".pd");//数据包文件的扩展名：.pd
	strcpy( m_szDataName, szPathNamePd );

	//创建目录文件
	char szPathNamePr[256];
	strcpy( szPathNamePr, path );
	strcat( szPathNamePr, name );
	strcat( szPathNamePr, ".pr");//数据包文件的扩展名：.pr
	strcpy( m_szDataRoot, szPathNamePr );

	//日志文件名称
	strcpy( m_szDate, "pack_Error.txt" );

	return S_OK;
}

void CPack::ReleaseRoot()
{
	//释放目录描述表
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		delete ((STDataDescribe*)(*ii));
	}
	m_listDataRoot.clear();
}
void CPack::LoadDataRootFromFile()
{
	//寻找目录文件
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataRoot, &file ) )==-1l)//相对文件路径
	{
		//cout<<"data_root文件没有找到!\n";
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"LoadDataRootFromFile: 没有找到目录文件(*.pr)！"<<endl<<endl;
		m_ofstreamDateTxt.close();
		return;
	}
	_findclose(lf);

	//如果找到了就加载到m_DataRoot里
	m_ifstreamRootIn.open( m_szDataRoot, ios::binary, 1 );
	
	//获取文件目录的数量
	int ifileNum = 0x0;
	m_ifstreamRootIn.seekg( 0x10 );//设置到表示文件数量的位置 
	int iTemp = 0;
	char chTmp = 0;
	for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; ii++)
	{
		iTemp = (int)((unsigned char)chTmp);
		iTemp<<= ii*8 ;
		ifileNum |=iTemp;
		iTemp = 0;
	}

	if( m_listDataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	for( int i = 0; i < ifileNum; i++ )
	{
		//获取文件名称的长度
		int iNameSize = 0x0;
		for(int ii = 0 ; ii < 1 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iNameSize |=iTemp ,iTemp = 0,ii++);

		//获取文件名称
		char *pszName = new char[iNameSize];
		m_ifstreamRootIn.read( pszName, iNameSize );

		//获取文件数据的大小
		int iFileSize = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iFileSize |=iTemp ,iTemp = 0,ii++);

		//获取文件数据的开始地址
		int iStart = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iStart |=iTemp ,iTemp = 0,ii++);

		//获取文件数据的结束地址
		int iEnd = 0x0;
		for(int ii = 0 ; ii < 4 && m_ifstreamRootIn.get(chTmp) ; iTemp = (int)((unsigned char)chTmp) ,iTemp<<= ii*8 , iEnd |=iTemp ,iTemp = 0,ii++);
	
		//创建一个文件描述
		STDataDescribe *pDataDescribe	= new STDataDescribe();
		strcpy( pDataDescribe->szName, pszName);
		delete pszName;
		pDataDescribe->dwSize			= iFileSize;
		pDataDescribe->dwStartAddress	= iStart;
		pDataDescribe->dwEndAddress	= iEnd;

		//填加到m_DataRoot里
		m_listDataRoot.push_back( pDataDescribe );
	}
	m_ifstreamRootIn.close();
}

void CPack::CreateFileRootFromMemory()
{
	int iFileNum = (int)m_listDataRoot.size();
	if( iFileNum > 0 )
	{
		//创建目录文件
		m_ifstreamRootOut.open( m_szDataRoot, ios::out|ios::binary, 0 );

		//填充保留位
		char szChr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			szChr[i] = rand()%0xff;
		}
		m_ifstreamRootOut<<"pr";
		m_ifstreamRootOut.write( szChr, 14 );

		//写入文件数量
		m_ifstreamRootOut.write((char*)&iFileNum, 4 );

		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			//文件名称的有效长度
			int name_size = (int)strlen(((STDataDescribe*)(*ii))->szName) + 1;
			m_ifstreamRootOut.write((char*)&name_size, 1 );

			//写入文件名称
			m_ifstreamRootOut.write(((STDataDescribe*)(*ii))->szName, name_size );

			//写入文件长度
			DWORD file_size = ((STDataDescribe*)(*ii))->dwSize;
			m_ifstreamRootOut.write((char*)&file_size, 4 );

			//文件的起始地址
			DWORD start = ((STDataDescribe*)(*ii))->dwStartAddress;
			m_ifstreamRootOut.write((char*)&start, 4 );

			//文件的结束地址
			DWORD end = ((STDataDescribe*)(*ii))->dwEndAddress;
			m_ifstreamRootOut.write((char*)&end, 4 );
		}
		m_ifstreamRootOut.close();
	}
	else
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"CreatFileRootFromMemory: "<<endl;
		m_ofstreamDateTxt<<"创建了空的目录文件！可能是目录表中没有目录信息"<<endl<<endl;
		m_ofstreamDateTxt.close();
		
		//创建目录文件
		m_ifstreamRootOut.open( m_szDataRoot, ios::out|ios::binary, 0 );

		//填充保留位
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamRootOut<<"pr";
		m_ifstreamRootOut.write( chr, 14 );

		int tmp = 0x0;
		m_ifstreamRootOut.write( (char*)&tmp, 4 );
		m_ifstreamRootOut.close();
	}
}

void CPack::UpDataPackFromMemory( char *name, char *memory, DWORD size )
{	
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataName, &file ) )==-1l)//如果没有找到文件的话就创建文件
	{
		//写入文件头
		m_ifstreamDataOut.open( m_szDataName, ios::binary, 0 );
		m_ifstreamDataOut<<"pd";

		//填充保留位
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamDataOut.write( chr, 14 );
		m_ifstreamDataOut.close();
	}
	_findclose(lf);

	
	//-----------------------------------------------------------------

	//更新内存中数据目录
	DWORD dwstart = 0x10;			//获得数据包中最后一个文件的结束地址
	if( m_listDataRoot.size() > 0 )
	{
		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			//判断是否有重名
			if( strcmp( ((STDataDescribe*)(*ii))->szName, name) == 0 )
			{
				m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
				m_ofstreamDateTxt<<"UpDataPackFromMemory：有文件名称重复!"<<endl;
				m_ofstreamDateTxt<<"重复的文件名称是："<<name<<endl;
				m_ofstreamDateTxt<<"没有进行该数据文件的更新！"<<endl<<endl;
				m_ofstreamDateTxt.close();
				return;
			}
			//文件的结束地址
			DWORD tmp_end = ((STDataDescribe*)(*ii))->dwEndAddress;
			if( dwstart < tmp_end )
			{
				dwstart = tmp_end;
			}
		}
	}

	DWORD dwEnd	= dwstart + size;
	
	//定义一个文件描述
	STDataDescribe *pDataDescribe	= new STDataDescribe();
	if( pDataDescribe != NULL && name != NULL )
	{
		strcpy( pDataDescribe->szName, name);
		pDataDescribe->dwSize			= size;
		pDataDescribe->dwStartAddress	= dwstart;
		pDataDescribe->dwEndAddress	= dwEnd;
	}

	//------------------------------------------------------------------
	//打开数据包文件，追加数据文件
	m_ifstreamDataOut.open( m_szDataName, ios::out|ios::binary|ios::app, 0 );
	if( memory != NULL && size > 0 )
	{
		m_ifstreamDataOut.seekp( dwstart );
		m_ifstreamDataOut.write( memory, size );
	}
	else
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromMemory：写入数据包文件失败!"<<endl;
		m_ofstreamDateTxt<<"可能是无效的内存区域或无效的大小值。"<<endl;
		m_ofstreamDateTxt<<"没有进行该数据文件的更新！"<<endl<<endl;
		m_ofstreamDateTxt.close();
		delete pDataDescribe;
		return;
	}
	//添加到目录中
	m_listDataRoot.push_back( pDataDescribe );
	m_ifstreamDataOut.close();
}

void CPack::UpDataPackFromFile( char *name, char *path )
{
	LoadDataRootFromFile();
	list<STDataDescribe*>    listDataRoot; 
	
	char szPathNamePr[256];
	strcpy( szPathNamePr, path );
	strcat( szPathNamePr, name );
	strcat( szPathNamePr, ".pr");//数据包文件的扩展名：.pr
	
	//寻找目录文件
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( szPathNamePr, &file ) )==-1l)//相对文件路径
	{
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromFile: 没有找到更新的目录文件(*.pr)！"<<endl;
		m_ofstreamDateTxt<<"    文件名称："<<name<<endl;
		m_ofstreamDateTxt<<"	   UpDataPackFromFile：没有进行任何更新！"<<endl<<endl;
		m_ofstreamDateTxt.close();
		return;
	}
	_findclose(lf);
	
	//如果找到了就加载到tmp_DataRoot里
	ifstream ifstreamIn;
	ifstreamIn.open( szPathNamePr, ios::binary, 1 );

	//获取文件目录的数量
	int iFileNum = 0x0;
	ifstreamIn.seekg( 0x10 );//设置到表示文件数量的位置 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , iFileNum |=tmp1 ,tmp1 = 0,ii++);

	//开始提取文件目录信息
	for( int i = 0; i < iFileNum; i++ )
	{
		//获取文件名称的长度
		int f_name_size = 0x0;
		for(int ii = 0 ; ii < 1 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , f_name_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件名称
		char *f_name = new char[f_name_size];
		ifstreamIn.read( f_name, f_name_size );

		//获取文件数据的大小
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的开始地址
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的结束地址
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && ifstreamIn.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//创建一个文件描述
		STDataDescribe *tmp_DataDescribe	= new STDataDescribe();
		strcpy( tmp_DataDescribe->szName, f_name);
		delete f_name;
		tmp_DataDescribe->dwSize			= file_size;
		tmp_DataDescribe->dwStartAddress	= start;
		tmp_DataDescribe->dwEndAddress	= end;

		//填加到m_DataRoot里
		listDataRoot.push_back( tmp_DataDescribe );
	}
	ifstreamIn.close();
	
	//----------------------------------------------------
	//判断更新的目录是否有效
	if( listDataRoot.size() > 0 )
	{
		//寻找数据更新包
		char path_name_pd[256];
		strcpy( path_name_pd, path );
		strcat( path_name_pd, name );
		strcat( path_name_pd, ".pd");//数据包文件的扩展名：.pr
		_finddata_t file;
		long lf;
		if( ( lf = (long)_findfirst( path_name_pd, &file ) )==-1l)//相对文件路径
		{
			m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
			m_ofstreamDateTxt<<"UpDataPackFromFile：没有找到更新数据文件(*.pd)！"<<endl;
			m_ofstreamDateTxt<<"可能是文件不存在！"<<endl<<endl;
			m_ofstreamDateTxt.close();
			
			//释放临时的目录
			for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
			{
				delete ((STDataDescribe*)(*ii));
			}
			listDataRoot.clear();
			return;
		}
		_findclose(lf);

		//打开更新包数据文件
		ifstreamIn.open( path_name_pd, ios::binary, 1 ); //只读方式打开

		//找到更新数据包的话就开始提取数据
		for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
		{
			//获取文件信息
			DWORD	f_size	= ((STDataDescribe*)(*ii))->dwSize;
			int		start	= ((STDataDescribe*)(*ii))->dwStartAddress;
			char    *f_name = ((STDataDescribe*)(*ii))->szName;
			
			//在内存中申请一块数据区域
			char *tmp_data = new char[f_size];
			if( tmp_data != NULL )
			{
				ifstreamIn.seekg( start );
				ifstreamIn.read( tmp_data, f_size );
				
				//添加到原数据包中
				UpDataPackFromMemory( f_name, tmp_data, f_size );

				delete tmp_data;
				tmp_data = NULL;
			}
		}

		//关闭更新包文件
		ifstreamIn.close();

		//释放临时的目录
		for( list<STDataDescribe*>::iterator ii = listDataRoot.begin(); ii != listDataRoot.end(); ii++ )
		{
			delete ((STDataDescribe*)(*ii));
		}
		listDataRoot.clear();

		//更新完成后，重新创建目录文件
		CreateFileRootFromMemory();
	}
	else
	{
		//不进行任何更新
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"UpDataPackFromFile: 更新的目录文件(*.pr)错误或没有文件数据信息！"<<endl;
		m_ofstreamDateTxt<<"	   UpDataPackFromFile：没有进行任何更新！"<<endl<<endl;
		m_ofstreamDateTxt.close();
	}

	ReleaseRoot();

}

void CPack::GetDataFromPack( char *name, char* &outDate, DWORD *pdwSize )
{
	//检查数据包文件是否存在
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( m_szDataName, &file ) )==-1l)//相对文件路径
	{
		//cout<<"GetDataFromPack: 文件没有找到!\n";
		m_ofstreamDateTxt.open( m_szDate, ios::app, 0 );
		m_ofstreamDateTxt<<"GetDataFromPack：没有找到数据文件(*.pd)！"<<endl;
		m_ofstreamDateTxt<<"可能是文件不存在！"<<endl<<endl;
		m_ofstreamDateTxt.close();
		*pdwSize	= 0x0;
		outDate = NULL;
		return;
	}
	_findclose(lf);

	//按给定的文件名称寻找数据文件
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		//按名称寻找文件
		if( strcmp( ((STDataDescribe*)(*ii))->szName, name) == 0 )
		{
			*pdwSize		=((STDataDescribe*)(*ii))->dwSize;
			int start	= ((STDataDescribe*)(*ii))->dwStartAddress;

			//提取数据
			m_ifstreamDataIn.open( m_szDataName, ios::binary, 1 );
			outDate = new char[*pdwSize];
			m_ifstreamDataIn.seekg(start);
			m_ifstreamDataIn.read( outDate, *pdwSize );
			m_ifstreamDataIn.close();
			return;
		}
	}
	*pdwSize	= 0x0;
	outDate = NULL;

	return;
}

//------------------------------------------------------------------------
//数据包工具
CPackTool::CPackTool():CPack()
{

}

CPackTool::~CPackTool()
{

}

void CPackTool::CreateDataPackFromFilePath( char *pszPath )
{
	if( m_listDataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	cout<<"创建数据文件，请稍候......"<<endl;
	//寻找文件
	char szPathName[256];
	strcpy( szPathName, pszPath );
	strcat( szPathName,"*.*");//好像无法判定
	_finddata_t file;
	long lf = (long)_findfirst( szPathName, &file );
	if( lf == -1l )//相对文件路径
	{
		cout<<"UpDataPackFromFilePath: 文件没有找到!\n";
		return;
	}
	else
	{	
		DWORD dwStart = 0x10;	
		m_ifstreamDataOut.open( m_szDataName, ios::binary, 0 );	//重新建立数据文件
		m_ifstreamDataOut<<"pd";

		//填充保留位
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		m_ifstreamDataOut.write( chr, 14 );

		//添加文件数据
		do
		{	
			if(file.attrib == _A_SUBDIR)
			{
				continue;
			}

			DWORD dwSize = file.size;
			DWORD end	= dwStart + dwSize;

			//定义一个文件描述
			STDataDescribe *tmp_DataDescribe	= new STDataDescribe();
			strcpy( tmp_DataDescribe->szName, file.name );
			tmp_DataDescribe->dwSize			= dwSize;
			tmp_DataDescribe->dwStartAddress	= dwStart;
			tmp_DataDescribe->dwEndAddress	= end;

			//添加到目录中
			m_listDataRoot.push_back( tmp_DataDescribe );
			dwStart = end;

			//把数据文件装载到内存
			char path_file[256];
			strcpy( path_file, pszPath );
			strcat( path_file, file.name );
			ifstream in( path_file, ios::binary, 0 );
			char *tmp = new char[dwSize];
			if( tmp != NULL )
			{
				in.read( tmp, dwSize );

				//添加到数据包中
				m_ifstreamDataOut.write( tmp, dwSize );
				delete tmp;
			}
			in.close();
		}
		while( _findnext( lf, &file ) == 0 );
		m_ifstreamDataOut.close();
	}
	_findclose(lf);

	//创建目录文件
	CreateFileRootFromMemory();

	cout<<"数据包创建完成！"<<endl;
}
void CPackTool::OutputRootToFile()
{
	ofstream outTxt( "DataRoot.txt" );
	if( m_listDataRoot.size() > 0 )
	{
		for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
		{
			outTxt<<((STDataDescribe*)(*ii))->szName<<endl;
		}
	}
	else
	{
		outTxt<<"目录中没有任何数据信息" <<endl;
	}
	outTxt.close();
}

void CPackTool::OutputDataToFile( char *path )
{
	for( list<STDataDescribe*>::iterator ii = m_listDataRoot.begin(); ii != m_listDataRoot.end(); ii++ )
	{
		char path_name[256];
		strcpy( path_name, path );
		strcat( path_name,((STDataDescribe*)(*ii))->szName);
		char *name = ((STDataDescribe*)(*ii))->szName;
		char *memory = NULL;
		DWORD size = ((STDataDescribe*)(*ii))->dwSize;

		GetDataFromPack( name, memory, &size );

		if( memory != NULL )
		{
			ofstream out( path_name, ios::binary, 0 );
			out.write( memory, size );
			out.close();
			delete memory;
		}
	}
}