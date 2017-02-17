#pragma  once 

class   CShareRestrictedSD       
{   
public:   
	CShareRestrictedSD();   
	virtual   ~CShareRestrictedSD();   
	SECURITY_ATTRIBUTES* GetSA();   
protected:   
	PVOID     ptr;   
	SECURITY_ATTRIBUTES   sa;   
	SECURITY_DESCRIPTOR   sd;   
};   

class ZShareMemory  
{
public:
	enum{ READONLY,READWRITE};
private:
    ZShareMemory(const ZShareMemory&) ;
	ZShareMemory& operator=(const ZShareMemory&) ;

public:
	ZShareMemory ();	
	~ZShareMemory();

	void  Attach(LPCTSTR szname);
	void  Detach();
    
	void* CreateMem(DWORD dwSize,LPCTSTR szname = NULL);
	void  DeleteMem();

	BOOL  UpdateMem(void* p,DWORD dwSize);
private:
    void _createfilemap();	
	void _deletefilemap();
protected:
	void*     m_pFirst;
	DWORD     m_dwSize;
	HANDLE    m_pMap;
    TCHAR*    m_pName;

private:
	CShareRestrictedSD       m_sd;

};


template < class TType, size_t size = 1>
class ZShareMemoryT : private ZShareMemory
{
public:
	typedef    TType    _type;
private:
    ZShareMemoryT(const ZShareMemoryT&) ;
	ZShareMemoryT& operator=(const ZShareMemoryT&) ;

public:
	ZShareMemoryT ()
	{	
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,NULL);
		}
		catch(...)
		{
			throw("template < class TType, size_t> constractor failed.\r\n");
		}
	};	

	ZShareMemoryT ( LPCTSTR szname)
	{	
		DWORD  dwSize = size * sizeof(TType);
		try
		{	
			CreateMem(dwSize,szname);
		}
		catch(...)
		{
			throw("template < class TType, size_t> constractor failed.\r\n");
		}
	};	

	~ZShareMemoryT()
	{	
		DeleteMem();
	};

	_type& operator[](int i)
	{

		static _type t;
        if ( m_pMap == NULL || i > size)  
		{
			throw("operator[] faile");
			return t;
		}

        return  *((_type*)((char*)m_pFirst + i * sizeof(_type)));
	}

	BOOL Update()
	{
        return UpdateMem(m_pFirst,m_dwSize);
	}
};

struct SIGASTATEDATA
{
	float x;
	float y;
	float z;
	bool bIsReload;
	UINT nPickObjID;
	SIGASTATEDATA()
	{
		x = 0;
		y = 0;
		z = 0;
		bIsReload = false;
		nPickObjID = 0;
	}

};

typedef ZShareMemoryT<SIGASTATEDATA,1> IGA_SHAREMEM_DATA;


