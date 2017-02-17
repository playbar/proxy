#include "stdafx.h"

//-------------------------------------------
CPack::CPack()
{
	m_DataRoot.clear();
}

CPack::~CPack()
{
	ReleaseRoot();
}

HRESULT CPack::Init( char *name, char *path )
{
	//创建数据文件
	char path_name_pd[256];
	strcpy( path_name_pd, path );
	strcat( path_name_pd, name );
	strcat( path_name_pd, ".pd");//数据包文件的扩展名：.pd
	strcpy( data_name, path_name_pd );

	//创建目录文件
	char path_name_pr[256];
	strcpy( path_name_pr, path );
	strcat( path_name_pr, name );
	strcat( path_name_pr, ".pr");//数据包文件的扩展名：.pr
	strcpy( data_root, path_name_pr );

	//日志文件名称
	strcpy( date, "pack_Error.txt" );

	return S_OK;
}

void CPack::ReleaseRoot()
{
	//释放目录描述表
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		delete ((DataDescribe*)(*ii));
	}
	m_DataRoot.clear();
}
void CPack::LoadDataRootFromFile()
{
	//寻找目录文件
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_root, &file ) )==-1l)//相对文件路径
	{
		//cout<<"data_root文件没有找到!\n";
		date_txt.open( date, ios::app, 0 );
		date_txt<<"LoadDataRootFromFile: 没有找到目录文件(*.pr)！"<<endl<<endl;
		date_txt.close();
		return;
	}
	_findclose(lf);

	//如果找到了就加载到m_DataRoot里
	root_in.open( data_root, ios::binary, 1 );
	
	//获取文件目录的数量
	int file_num = 0x0;
	root_in.seekg( 0x10 );//设置到表示文件数量的位置 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_num |=tmp1 ,tmp1 = 0,ii++);

	//释放原由目录信息
	if( m_DataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	//开始提取文件目录信息
	for( int i = 0; i < file_num; i++ )
	{
		//获取文件名称的长度
		int name_size = 0x0;
		for(int ii = 0 ; ii < 1 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , name_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件名称
		char *name = new char[name_size];
		root_in.read( name, name_size );

		//获取文件数据的大小
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的开始地址
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的结束地址
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && root_in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//创建一个文件描述
		DataDescribe *tmp_DataDescribe	= new DataDescribe();
		strcpy( tmp_DataDescribe->name, name);
		delete name;
		tmp_DataDescribe->size			= file_size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;

		//填加到m_DataRoot里
		m_DataRoot.push_back( tmp_DataDescribe );
	}
	root_in.close();
}

void CPack::CreateFileRootFromMemory()
{
	int file_num = (int)m_DataRoot.size();
	if( file_num > 0 )
	{
		//创建目录文件
		root_out.open( data_root, ios::out|ios::binary, 0 );

		//填充保留位
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		root_out<<"pr";
		root_out.write( chr, 14 );

		//写入文件数量
		root_out.write((char*)&file_num, 4 );

		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			//文件名称的有效长度
			int name_size = (int)strlen(((DataDescribe*)(*ii))->name) + 1;
			root_out.write((char*)&name_size, 1 );

			//写入文件名称
			root_out.write(((DataDescribe*)(*ii))->name, name_size );

			//写入文件长度
			DWORD file_size = ((DataDescribe*)(*ii))->size;
			root_out.write((char*)&file_size, 4 );

			//文件的起始地址
			DWORD start = ((DataDescribe*)(*ii))->start_addres;
			root_out.write((char*)&start, 4 );

			//文件的结束地址
			DWORD end = ((DataDescribe*)(*ii))->end_address;
			root_out.write((char*)&end, 4 );
		}
		root_out.close();
	}
	else
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"CreatFileRootFromMemory: "<<endl;
		date_txt<<"创建了空的目录文件！可能是目录表中没有目录信息"<<endl<<endl;
		date_txt.close();
		
		//创建目录文件
		root_out.open( data_root, ios::out|ios::binary, 0 );

		//填充保留位
		char chr[14] = {};
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		root_out<<"pr";
		root_out.write( chr, 14 );

		int tmp = 0x0;
		root_out.write( (char*)&tmp, 4 );
		root_out.close();
	}
}

void CPack::UpDataPackFromMemory( char *name, char *memory, DWORD size )
{	
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_name, &file ) )==-1l)//如果没有找到文件的话就创建文件
	{
		//写入文件头
		data_out.open( data_name, ios::binary, 0 );
		data_out<<"pd";

		//填充保留位
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		data_out.write( chr, 14 );
		data_out.close();
	}
	_findclose(lf);

	
	//-----------------------------------------------------------------

	//更新内存中数据目录
	DWORD start = 0x10;			//获得数据包中最后一个文件的结束地址
	if( m_DataRoot.size() > 0 )
	{
		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			//判断是否有重名
			if( strcmp( ((DataDescribe*)(*ii))->name, name) == 0 )
			{
				date_txt.open( date, ios::app, 0 );
				date_txt<<"UpDataPackFromMemory：有文件名称重复!"<<endl;
				date_txt<<"重复的文件名称是："<<name<<endl;
				date_txt<<"没有进行该数据文件的更新！"<<endl<<endl;
				date_txt.close();
				return;
			}
			//文件的结束地址
			DWORD tmp_end = ((DataDescribe*)(*ii))->end_address;
			if( start < tmp_end )
			{
				start = tmp_end;
			}
		}
	}
	else
	{

	}

	DWORD end	= start + size;
	
	//定义一个文件描述
	DataDescribe *tmp_DataDescribe	= new DataDescribe();
	if( tmp_DataDescribe != NULL && name != NULL )
	{
		strcpy( tmp_DataDescribe->name, name);
		tmp_DataDescribe->size			= size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;
	}

	//------------------------------------------------------------------
	//打开数据包文件，追加数据文件
	data_out.open( data_name, ios::out|ios::binary|ios::app, 0 );
	if( memory != NULL && size > 0 )
	{
		data_out.seekp( start );
		data_out.write( memory, size );
	}
	else
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromMemory：写入数据包文件失败!"<<endl;
		date_txt<<"可能是无效的内存区域或无效的大小值。"<<endl;
		date_txt<<"没有进行该数据文件的更新！"<<endl<<endl;
		date_txt.close();
		delete tmp_DataDescribe;
		return;
	}
	//添加到目录中
	m_DataRoot.push_back( tmp_DataDescribe );
	data_out.close();
}

void CPack::UpDataPackFromFile( char *name, char *path )
{
	//读取现有数据文件信息，初始化文件目录
	LoadDataRootFromFile();

	//----------------------------------------------------
	//先加载更新的包的目录
	list<DataDescribe*>    tmp_DataRoot; //临时的数据包目录
	
	//创建更新目录文件名
	char path_name_pr[256];
	strcpy( path_name_pr, path );
	strcat( path_name_pr, name );
	strcat( path_name_pr, ".pr");//数据包文件的扩展名：.pr
	
	//寻找目录文件
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( path_name_pr, &file ) )==-1l)//相对文件路径
	{
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromFile: 没有找到更新的目录文件(*.pr)！"<<endl;
		date_txt<<"    文件名称："<<name<<endl;
		date_txt<<"	   UpDataPackFromFile：没有进行任何更新！"<<endl<<endl;
		date_txt.close();
		return;
	}
	_findclose(lf);
	
	//如果找到了就加载到tmp_DataRoot里
	ifstream in;
	in.open( path_name_pr, ios::binary, 1 );

	//获取文件目录的数量
	int file_num = 0x0;
	in.seekg( 0x10 );//设置到表示文件数量的位置 
	int tmp1 = 0;
	char tmp = 0;
	for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_num |=tmp1 ,tmp1 = 0,ii++);

	//开始提取文件目录信息
	for( int i = 0; i < file_num; i++ )
	{
		//获取文件名称的长度
		int f_name_size = 0x0;
		for(int ii = 0 ; ii < 1 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , f_name_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件名称
		char *f_name = new char[f_name_size];
		in.read( f_name, f_name_size );

		//获取文件数据的大小
		int file_size = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , file_size |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的开始地址
		int start = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , start |=tmp1 ,tmp1 = 0,ii++);

		//获取文件数据的结束地址
		int end = 0x0;
		for(int ii = 0 ; ii < 4 && in.get(tmp) ; tmp1 = (int)((unsigned char)tmp) ,tmp1<<= ii*8 , end |=tmp1 ,tmp1 = 0,ii++);
	
		//创建一个文件描述
		DataDescribe *tmp_DataDescribe	= new DataDescribe();
		strcpy( tmp_DataDescribe->name, f_name);
		delete f_name;
		tmp_DataDescribe->size			= file_size;
		tmp_DataDescribe->start_addres	= start;
		tmp_DataDescribe->end_address	= end;

		//填加到m_DataRoot里
		tmp_DataRoot.push_back( tmp_DataDescribe );
	}
	in.close();
	
	//----------------------------------------------------
	//判断更新的目录是否有效
	if( tmp_DataRoot.size() > 0 )
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
			date_txt.open( date, ios::app, 0 );
			date_txt<<"UpDataPackFromFile：没有找到更新数据文件(*.pd)！"<<endl;
			date_txt<<"可能是文件不存在！"<<endl<<endl;
			date_txt.close();
			
			//释放临时的目录
			for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
			{
				delete ((DataDescribe*)(*ii));
			}
			tmp_DataRoot.clear();
			return;
		}
		_findclose(lf);

		//打开更新包数据文件
		in.open( path_name_pd, ios::binary, 1 ); //只读方式打开

		//找到更新数据包的话就开始提取数据
		for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
		{
			//获取文件信息
			DWORD	f_size	= ((DataDescribe*)(*ii))->size;
			int		start	= ((DataDescribe*)(*ii))->start_addres;
			char    *f_name = ((DataDescribe*)(*ii))->name;
			
			//在内存中申请一块数据区域
			char *tmp_data = new char[f_size];
			if( tmp_data != NULL )
			{
				in.seekg( start );
				in.read( tmp_data, f_size );
				
				//添加到原数据包中
				UpDataPackFromMemory( f_name, tmp_data, f_size );

				delete tmp_data;
				tmp_data = NULL;
			}
		}

		//关闭更新包文件
		in.close();

		//释放临时的目录
		for( list<DataDescribe*>::iterator ii = tmp_DataRoot.begin(); ii != tmp_DataRoot.end(); ii++ )
		{
			delete ((DataDescribe*)(*ii));
		}
		tmp_DataRoot.clear();

		//更新完成后，重新创建目录文件
		CreateFileRootFromMemory();
	}
	else
	{
		//不进行任何更新
		date_txt.open( date, ios::app, 0 );
		date_txt<<"UpDataPackFromFile: 更新的目录文件(*.pr)错误或没有文件数据信息！"<<endl;
		date_txt<<"	   UpDataPackFromFile：没有进行任何更新！"<<endl<<endl;
		date_txt.close();
	}

	ReleaseRoot();

}

void CPack::GetDataFromPack( char *name, char* &outDate, DWORD *size )
{
	//检查数据包文件是否存在
	_finddata_t file;
	long lf;
	if( ( lf = (long)_findfirst( data_name, &file ) )==-1l)//相对文件路径
	{
		//cout<<"GetDataFromPack: 文件没有找到!\n";
		date_txt.open( date, ios::app, 0 );
		date_txt<<"GetDataFromPack：没有找到数据文件(*.pd)！"<<endl;
		date_txt<<"可能是文件不存在！"<<endl<<endl;
		date_txt.close();
		*size	= 0x0;
		outDate = NULL;
		return;
	}
	_findclose(lf);

	//按给定的文件名称寻找数据文件
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		//按名称寻找文件
		if( strcmp( ((DataDescribe*)(*ii))->name, name) == 0 )
		{
			*size		=((DataDescribe*)(*ii))->size;
			int start	= ((DataDescribe*)(*ii))->start_addres;

			//提取数据
			data_in.open( data_name, ios::binary, 1 );
			outDate = new char[*size];
			data_in.seekg(start);
			data_in.read( outDate, *size );
			data_in.close();
			return;
		}
	}
	*size	= 0x0;
	outDate = NULL;
	//date_txt.open( date, ios::app, 0 );
	//date_txt<<"GetDataFromPack：被请求的文件名称不存在 "<<endl;
	//date_txt<<"要获取的文件名称是："<<name<<endl;
	//date_txt<<"获取文件数据失败！"<<endl<<endl;
	//date_txt.close();
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

void CPackTool::CreateDataPackFromFilePath( char *path )
{
	//清空目录表
	if( m_DataRoot.size() > 0 )
	{
		ReleaseRoot();
	}

	cout<<"创建数据文件，请稍候......"<<endl;
	//寻找文件
	char path_name[256];
	strcpy( path_name, path );
	strcat( path_name,"*.*");//好像无法判定
	_finddata_t file;
	long lf = (long)_findfirst( path_name, &file );
	if( lf == -1l )//相对文件路径
	{
		cout<<"UpDataPackFromFilePath: 文件没有找到!\n";
		return;
	}
	else
	{	
		DWORD start = 0x10;	
		data_out.open( data_name, ios::binary, 0 );	//重新建立数据文件
		data_out<<"pd";

		//填充保留位
		char chr[14] = {};
		srand( GetCurrentTime());
		for( int i = 4; i < 14; i++ )
		{
			chr[i] = rand()%0xff;
		}
		data_out.write( chr, 14 );

		//添加文件数据
		do
		{	
			if(file.attrib == _A_SUBDIR)
			{
				continue;
			}

			DWORD size = file.size;
			DWORD end	= start + size;

			//定义一个文件描述
			DataDescribe *tmp_DataDescribe	= new DataDescribe();
			strcpy( tmp_DataDescribe->name, file.name );
			tmp_DataDescribe->size			= size;
			tmp_DataDescribe->start_addres	= start;
			tmp_DataDescribe->end_address	= end;

			//添加到目录中
			m_DataRoot.push_back( tmp_DataDescribe );
			start = end;

			//把数据文件装载到内存
			char path_file[256];
			strcpy( path_file, path );
			strcat( path_file, file.name );
			ifstream in( path_file, ios::binary, 0 );
			char *tmp = new char[size];
			if( tmp != NULL )
			{
				in.read( tmp, size );

				//添加到数据包中
				data_out.write( tmp, size );
				delete tmp;
			}
			in.close();
		}
		while( _findnext( lf, &file ) == 0 );
		data_out.close();
	}
	_findclose(lf);

	//创建目录文件
	CreateFileRootFromMemory();

	cout<<"数据包创建完成！"<<endl;
}
void CPackTool::OutputRootToFile()
{
	ofstream outTxt( "DataRoot.txt" );
	if( m_DataRoot.size() > 0 )
	{
		for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
		{
			outTxt<<((DataDescribe*)(*ii))->name<<endl;
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
	for( list<DataDescribe*>::iterator ii = m_DataRoot.begin(); ii != m_DataRoot.end(); ii++ )
	{
		char path_name[256];
		strcpy( path_name, path );
		strcat( path_name,((DataDescribe*)(*ii))->name);
		char *name = ((DataDescribe*)(*ii))->name;
		char *memory = NULL;
		DWORD size = ((DataDescribe*)(*ii))->size;

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