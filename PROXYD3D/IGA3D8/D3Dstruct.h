#pragma once


#include <d3d8.h>
#include <d3dx8.h>
#include <tchar.h>
#include "ToolFunction.h"


typedef void* POS;

#ifndef SAFE_DELETE
#define SAFE_DELETE( T ) {if( T ) { delete T; T = NULL; }}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( T ) {if( T ) { delete[] T; T = NULL; }}
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( T ) if( T ) T->Release()
#endif




template< class T > struct Array
{
public:
	typedef T Type;

	// constructor
	Array()
	 :	m_iArrayNum(0)
	 ,	m_iArrayMax(0)
	 ,	m_pData(NULL)
	{}

	Array( INT iNum )
	 :	m_iArrayNum( iNum )
	 ,	m_iArrayMax( iNum )
	 ,	m_pData( NULL )
	{
		Realloc();
	}

	Array( const Array& saOther )
	 :	m_iArrayNum( saOther.m_iArrayNum )
	 ,	m_iArrayMax( saOther.m_iArrayNum )
	 ,	m_pData( NULL )
	{
		Realloc();

		if( sizeof(T) != 1 )
		{
			for( INT i = 0; i < m_iArrayNum; ++i )
				(*this)[i] = saOther[i];
		}
		else
		{
			memcpy( &(*this)[0], &saOther[0], m_iArrayNum * sizeof(T) );
		}
	}

	~Array()
	{
		if(m_iArrayNum >= 0 && m_iArrayMax >= m_iArrayNum)
		{
		    Remove( 0, m_iArrayNum );
		    if( m_pData )
			    free( m_pData );
		}
	}

	// achive member
	T*	GetData( void ) const			
	{
		return m_iArrayNum > 0 ? m_pData : NULL; 
	}

	INT Num( void ) const
	{
		return m_iArrayNum;
	}
	BOOL IsEmpty( void ) const
	{
		return 0 == m_iArrayNum;
	}
	BOOL IsNotEmpty( void ) const
	{
		return 0 != m_iArrayNum;
	}

	// operator
    T& operator[]( INT i )
	{
		return m_pData[i];
	}
	const T& operator[]( INT i ) const
	{
		return m_pData[i];
	}
    T& Item( INT i )
	{
		return m_pData[i];
	}
	const T& Item( INT i ) const
	{
		return m_pData[i];
	}
	T& Last( INT i = 0 )
	{
		return m_pData[m_iArrayNum - i - 1];
	}
	const T& Last( INT i = 0 ) const
	{
		return m_pData[m_iArrayNum - i - 1];
	}
	INT FindItem( const T& Item ) const
	{
		for( INT Index = 0; Index < m_iArrayNum; ++Index )
			if( m_pData[Index] == Item ) return Index;
		return -1;
	}

	// Add, Insert, Remove, Empty interface.
	INT Add( INT n = 1 )
	{
		INT iIndex = Add_Internal( n );

		return iIndex;
	}
	void Insert( INT iIndex, INT iCount = 1 )
	{
		Insert_Internal( iIndex, iCount );
	}
	void InsertZeroed( INT iIndex, INT iCount = 1 )
	{
		Insert( iIndex, iCount );
		memzero( m_pData+iIndex, iCount*sizeof(T) );
	}
	void InsertItem( INT iIndex, const T& Item )
	{
		Insert( iIndex );
		m_pData[iIndex] = Item;
	}
	void Remove( INT iIndex, INT iCount = 1 )
	{
		if( iCount )
		{
			memmove(m_pData + iIndex, m_pData + iIndex + iCount, (m_iArrayNum - iIndex - iCount ) * sizeof(T));
			m_iArrayNum -= iCount;
			if(    (3*m_iArrayNum < 2*m_iArrayMax  || (m_iArrayMax - m_iArrayNum) * sizeof(T) >= 16384)
			        && (m_iArrayMax - m_iArrayNum > 64 || m_iArrayNum == 0) )
			{
				m_iArrayMax = m_iArrayNum;
				Realloc();
			}
		}
	}
	void Empty( INT iNewMax = 0 )
	{
		m_iArrayNum = 0;
		m_iArrayMax = iNewMax;
		Realloc();
	}
	void SetSize( INT iNewNum )
	{
		INT iOldNum = m_iArrayNum;

		if( iNewNum == 0 )
		{
			m_iArrayNum = 0;
		}
		else
		{
			if( m_iArrayMax >= iNewNum )
				m_iArrayNum = iNewNum;
			else
			{
				m_iArrayMax = m_iArrayNum = iNewNum;
				Realloc();
			}
		}
	}

	// Functions dependent on Add, Remove.
	Array operator+( const Array& saOther )
	{
		if( this != &saOther )
		{
			Array saRet( m_iArrayNum + saOther.m_iArrayNum );
			for( INT i = 0; i < m_iArrayNum; ++i )
				new( *this )T( m_pData[i] );
			for( INT i = 0; i < saOther.m_iArrayNum; ++i )
				new( *this )T( saOther[i] );
			return saRet;
		}
		else
			return *this;
	}
	void operator+=( const Array& saOther )
	{
		if( this != &saOther )
		{
			for( INT i = 0; i < saOther.m_iArrayNum; ++i )
				new( *this )T( saOther[i] );
		}
	}
	void operator=( const Array& saOther )
	{
		if( this != &saOther )
		{
			Empty( saOther.m_iArrayNum );
			for( INT i = 0; i < saOther.m_iArrayNum; ++i )
				new( *this )T( saOther[i] );
		}
	}
	INT AddItem( const T& Item )
	{
		INT iIndex = Add();
		m_pData[iIndex] = Item;
		return iIndex;
	}
	INT AddZeroed( INT n = 1 )
	{
		INT iIndex = Add( n  );
		memzero( m_pData+iIndex, n*sizeof(T) );
		return iIndex;
	}
	INT AddUniqueItem( const T& Item )
	{
		for( INT iIndex = 0; iIndex < m_iArrayNum; ++iIndex )
			if( m_pData[iIndex] == Item )
				return iIndex;
		return AddItem( Item );
	}
	INT RemoveItem( const T& Item )
	{
		INT OriginalNum = m_iArrayNum;
		for( INT iIndex = 0; iIndex < m_iArrayNum; ++iIndex )
			if( m_pData[iIndex] == Item )
				Remove( iIndex-- );
		return OriginalNum - m_iArrayNum;
	}

	// Iterator.
	class TIterator
	{
	public:
		TIterator( Array<T>& saArray )
			: m_saArray(saArray)
			, m_iIndex(0)
		{}
		TIterator( Array<T>& saArray, const T& O )
			: m_saArray(saArray)
			, m_iIndex( ( (&O) - saArray.m_pData ) / sizeof(T) )
		{
			ASSERT( m_iIndex >= 0 );
		}

		// operator
		void operator=( const T& O )
		{
			m_iIndex = ( (&O) - m_saArray.m_pData ) / sizeof(T);
			ASSERT( m_iIndex >= 0 );
		}
		void operator++()      { if( m_iIndex < m_saArray.Num() ) ++m_iIndex; }
		void operator--()      { if( m_iIndex > 0 ) --m_iIndex; }

		BOOL operator!() const		{ return m_iIndex >= m_saArray.Num(); }

		// Reference
		T& operator*( void )  const { return m_saArray[m_iIndex]; }
		T* operator->( void ) const { return &m_saArray[m_iIndex]; }

		// method
		friend void SetFirst( TIterator &It )		{ It.m_iIndex = 0; }
		friend void SetLast( TIterator &It )		{ It.m_iIndex = m_saArray.Num(); }
		friend void RemoveCurrent( TIterator &It )
		{
			if( It.m_iIndex < It.m_saArray.Num() )
				It.m_saArray.Remove( It.m_iIndex == 0 ? It.m_iIndex : It.m_iIndex-- );
		}
		friend INT GetIndex( TIterator &It )  { return It.m_iIndex; }
		friend T& GetCurrent( TIterator &It ) { return It.m_saArray[ It.m_iIndex ]; }
		friend T& GetPrev( TIterator &It )    { return It.m_saArray[ It.m_iIndex ? It.m_iIndex-1 : It.m_saArray.Num()-1 ]; }
		friend T& GetNext( TIterator &It )    { return It.m_saArray[ It.m_iIndex < It.m_saArray.Num()-1 ? It.m_iIndex+1 : 0 ]; }

	private:
		Array<T>& m_saArray;
		INT m_iIndex;
	};

	// Pointer.
	class APointer
	{
	public:
		// constructor
		APointer( const Array<T>& A ) : m_Pointer( A.GetData() ), m_pEndPtr( A.EndPtr() ) {}
		APointer( const Array<T>& A, const T& O ) : m_Pointer( &O ), m_pEndPtr( A.EndPtr() ) {}

		// conversion
		T*& operator=(const T* const P)	{ m_Pointer = P; return m_Pointer; }
		operator T*() const				{ return m_Pointer; }
		T* operator-> () const			{ return m_Pointer; }

		// boolean
		operator BOOL() const			{ return m_Pointer < m_pEndPtr; }
		BOOL operator!() const			{ return m_Pointer >= m_pEndPtr; }

		// operator
		T* operator++()					{ return ++m_Pointer; }
		T* operator--()					{ return --m_Pointer; }
		T* operator++( INT )			{ return m_Pointer++; }
		T* operator--( INT )			{ return m_Pointer--; }

		T* operator+( INT i ) const		{ return m_Pointer+i; }
		T* operator+( DWORD i )	const	{ return m_Pointer+i; }
		T* operator+=( INT i )			{ return return m_Pointer+=i; }
		T* operator+=( DWORD i )		{ return return m_Pointer+=i; }
		friend T* operator+( INT i, const APointer& O )		{ return i+O.m_Pointer; }
		friend T* operator+( DWORD i, const APointer& O )	{ return i+O.m_Pointer; }

		T* operator-( INT i ) const		{ return m_Pointer-i; }
		T* operator-( DWORD i )	const	{ return m_Pointer-i; }
		T* operator-=( INT i )			{ return return m_Pointer-=i; }
		T* operator-=( DWORD i )		{ return return m_Pointer-=i; }

		DWORD operator-( T *P ) const	{ return m_Pointer-P; }
		DWORD operator-( const APointer& O ) const	{ return m_Pointer-O.m_Pointer; }
		friend DWORD operator-( T *P, const APointer& O ) { return P-O.m_Pointer; }
		friend DWORD operator-( const APointer& L, const APointer& R ) { return L.m_Pointer-R.m_Pointer; }

	private:
		T *m_Pointer;
		T *m_pEndPtr;
	};

protected:
	T*	  m_pData;
	INT m_iArrayNum;
	INT m_iArrayMax;

	void Realloc( void )
	{
		if( m_pData && m_iArrayMax > 0 )
			m_pData = (T*)realloc( m_pData, m_iArrayMax*sizeof(T) );
		else if( m_iArrayMax > 0 )
			m_pData = (T*)malloc( m_iArrayMax*sizeof(T) );
		else if( m_pData ) 
		{
			free( m_pData );
			m_pData = NULL;
		}
	}

	INT Add_Internal( INT n = 1 )
	{
		INT iOldNum = m_iArrayNum;
		if( (m_iArrayNum += n) > m_iArrayMax )
		{
			m_iArrayMax = m_iArrayNum + 3*m_iArrayNum/8 + 32;
			Realloc();
			//memmove( m_pData + n, m_pData, iOldNum*sizeof(T) );
		}

		return iOldNum;
	}
	
	void Insert_Internal( INT iIndex, INT iCount = 1 )
	{
		INT iOldNum = m_iArrayNum;
		if( (m_iArrayNum += iCount) > m_iArrayMax )
		{
			m_iArrayMax = m_iArrayNum + 3*m_iArrayNum/8 + 32;
			Realloc();
		}
		memmove( m_pData + iIndex + iCount, m_pData + iIndex, ( iOldNum - iIndex )*sizeof(T) );
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class CLinkedList
{
	bool	m_bUseFree;
protected:

	class node
	{
	public:
		T	 *	t;
		int		index;
		node *	prev;
		node *	next;
		node():t(NULL){}
		~node(){SAFE_DELETE(t);}
	};

	node * m_pHead,* m_pTail,* m_pFree;
	int m_iCount,m_iTableNum,m_iIncTableNum;
	node ** m_ppTable;

public:
	enum {DEFAULT_INC_TABLE_NUM = 30,DEFAULT_INDEX_TABLE_NUM = 2};
	CLinkedList(int maxtablenum,int inctablenum,bool usefree = true)
	{
		Init(maxtablenum,inctablenum,usefree);
	}
	CLinkedList(bool usefree = true)
	{
		Init(DEFAULT_INDEX_TABLE_NUM,DEFAULT_INC_TABLE_NUM,usefree);
	}
	virtual ~CLinkedList()
	{
		Clear();
		ClearFree();
		delete m_pFree;
		delete m_pHead;
		delete m_pTail;
		delete[] m_ppTable;
	}
	void Init(int maxtablenum,int inctablenum,bool usefree)
	{
		m_bUseFree = usefree;

		m_pHead = new node;
		m_pTail = new node;
		m_pFree = new node;

		m_pHead->prev = m_pHead;
		m_pHead->next = m_pTail;

		m_pTail->prev = m_pHead;
		m_pTail->next = m_pTail;

		m_pFree->prev = m_pFree;
		m_pFree->next = m_pFree;

		m_iCount = 0;
		m_iTableNum = maxtablenum;
		m_iIncTableNum = inctablenum;
		m_ppTable = new node*[m_iTableNum];
	}
	void Clear()
	{
		node * cur,* del;
		cur = m_pHead->next;

		while(cur != m_pTail)
		{
			del = cur;
			cur = cur->next;
			ReleaseNode(del);
		}

		m_pHead->prev = m_pHead;
		m_pHead->next = m_pTail;
		m_pTail->prev = m_pHead;
		m_pTail->next = m_pTail;

		m_iCount = 0;
	}
	void Insert(T t)
	{
		InsertTail(t);
	}
	void InsertTail(T t)
	{
		node * n = AllocNode();
		*(n->t) = t;

		n->next = m_pTail;
		n->prev = m_pTail->prev;
		m_pTail->prev->next = n;
		m_pTail->prev = n;

		InsertTable(n);
		m_iCount++;
	}
	void InsertHead(T t)
	{
		node * n = AllocNode();
		*(n->t) = t;

		n->next = m_pHead->next;
		n->prev = m_pHead;
		m_pHead->next->prev = n;
		m_pHead->next = n;

		InsertTable(n);
		m_iCount++;
	}
	bool Remove(T t)
	{
		node * cur,* del;
		cur = m_pHead->next;
		while(cur != m_pTail)
		{
			del = cur;
			cur = cur->next;

			if(*(del->t) != t)
				continue;

			del->next->prev = del->prev;
			del->prev->next = del->next;

			RemoveTable(del);
			ReleaseNode(del);

			m_iCount--;
			return true;
		}
		return false;
	}
	bool RemoveAt(POS pos)
	{
		node * n = (node *)pos;
		n->next->prev = n->prev;
		n->prev->next = n->next;

		RemoveTable(n);
		ReleaseNode(n);

		m_iCount--;
		return true;
	}
	bool RemoveHeader()
	{
		if(m_pHead->next == m_pTail)
			return false;
		return RemoveAt(m_pHead->next);
	}
	bool RemoveTail()
	{
		if(m_pTail->prev == m_pHead)
			return false;
		return RemoveAt(m_pTail->prev);
	}
	POS Find(T t)
	{
		node * cur;
		cur = m_pHead->next;
		while(cur != m_pTail)
		{
			if(*(cur->t) == t)
				return (POS)cur;
			cur = cur->next;
		}
		return NULL;
	}
	POS GetFirstPos()
	{
		if(m_pHead->next == m_pTail)
			return NULL;
		return (POS)m_pHead->next;
	}
	T GetNext(POS & pos)
	{
		node * n = (node *)pos;
		if(n->next == m_pTail)
			pos = NULL;
		else
			pos = (POS)n->next;
		return *(n->t);
	}
	T GetAt(POS pos)
	{
		return *(((node *)pos)->t);
	}
	int GetCount(){return m_iCount;}

	T & operator[](int index)
	{
		return *(m_ppTable[index]->t);
	}
protected:

	void InsertTable(node * tnode)
	{
		if(m_iTableNum <= m_iCount)
		{
			node ** tmp = new node*[m_iTableNum + m_iIncTableNum];
			memcpy(tmp,m_ppTable,sizeof(node*)*m_iTableNum);
			delete[] m_ppTable;
			m_ppTable = tmp;
			m_iTableNum += m_iIncTableNum;
		}
		m_ppTable[m_iCount] = tnode;
		m_ppTable[m_iCount]->index = m_iCount;
	}

	void RemoveTable(node * tnode)
	{
		if(tnode->index >= (m_iCount - 1))
			return;

		m_ppTable[m_iCount - 1]->index = tnode->index;
		m_ppTable[tnode->index] = m_ppTable[m_iCount - 1];

		if(m_iTableNum - m_iCount < m_iIncTableNum)
			return;

		node ** tmp = new node*[m_iTableNum - m_iIncTableNum];
		memcpy(tmp,m_ppTable,sizeof(node*)*m_iCount);
		m_iTableNum = m_iCount;
		delete[] m_ppTable;
		m_ppTable = tmp;
	}

	node * AllocNode()
	{
		node * p = NULL;

		if(!m_bUseFree || (m_pFree->next == m_pFree))
		{
			p = new node;
			p->t = new T;
			return p;
		}
		else
		{
			p = m_pFree->next;
			p->next->prev = p->prev;
			p->prev->next = p->next;
		}
		return p;
	}
	void ReleaseNode(node * p)
	{
		if(!m_bUseFree)
		{
			delete p;
			return;
		}

		p->next = m_pFree->next;
		p->prev = m_pFree;
		m_pFree->next->prev = p;
		m_pFree->next = p;
	}
	void ClearFree()
	{
		node * del, * cur = m_pFree->next;

		while(cur != m_pFree)
		{
			del = cur;
			cur = cur->next;
			delete del;
		}
	}
};


//////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class CQueue : public CLinkedList<T>
{
public:

	CQueue(int maxtablenum,int inctablenum,bool usefree = true):CLinkedList<T>(maxtablenum,inctablenum,usefree)
	{
	}
	CQueue(bool usefree = true):CLinkedList<T>(usefree)
	{
	}
	virtual ~CQueue()
	{
	}

public:

	void push(T t)
	{
		InsertTail(t);
	}

	bool pop(T & ret)
	{
		if(m_pHead->next == m_pTail)
			return false;

		node * n = m_pHead->next;
		n->next->prev = n->prev;
		n->prev->next = n->next;
		ret = *(n->t);
		RemoveTable(n);
		ReleaseNode(n);
		m_iCount--;
		return true;
	}
};

template<typename T>
class CStack : public CLinkedList<T>
{
public:

	CStack(int maxtablenum,int inctablenum,bool usefree = true):CLinkedList<T>(maxtablenum,inctablenum,usefree)
	{
	}
	CStack(bool usefree = true):CLinkedList<T>(usefree)
	{
	}
	virtual ~CStack()
	{
	}

public:

	void push(T t)
	{
		InsertHead(t);
	}

	bool pop(T & ret)
	{
		if(m_pHead->next == m_pTail)
			return false;

		node * n = m_pHead->next;
		n->next->prev = n->prev;
		n->prev->next = n->next;
		ret = *(n->t);
		RemoveTable(n);
		ReleaseNode(n);
		m_iCount--;
		return true;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Key,typename T>
class CHashList
{
protected:

	bool m_bUseFree;

	struct listnode;
	struct heshnode
	{
		Key		*	id;
		T		*	data;
		listnode	* list;
		heshnode	* prev,
			* next;
		heshnode():id(NULL),data(NULL){}
		~heshnode(){SAFE_DELETE(id);SAFE_DELETE(data);}
	};

	struct listnode
	{
		DWORD		index;
		heshnode	* hesh;
		listnode	* prev;
		listnode	* next;
	};

	DWORD			m_dwCount,
		m_dwMaxBucket,
		m_dwTableNum;

	heshnode * m_phhead, * m_phfree;
	listnode * m_plhead, * m_plfree;
	listnode ** m_ppTable;

public:

	CHashList(DWORD maxbucket,bool usefree = true)
	{
		m_bUseFree = usefree;

		Init(maxbucket);
	}
	virtual ~CHashList()
	{
		Release();
	}

public:

	void Init(DWORD maxbucket)
	{
		m_dwCount = 0;
		m_dwMaxBucket = maxbucket;
		m_phhead = new heshnode[maxbucket];
		for(DWORD i = 0;i < m_dwMaxBucket;i++)
		{
			m_phhead[i].next = &m_phhead[i];
			m_phhead[i].prev = &m_phhead[i];
		}
		m_plhead = new listnode;
		m_plhead->next = m_plhead;
		m_plhead->prev = m_plhead;

		m_phfree = new heshnode;
		m_plfree = new listnode;

		m_phfree->prev = m_phfree;
		m_phfree->next = m_phfree;

		m_plfree->prev = m_plfree;
		m_plfree->next = m_plfree;

		m_dwTableNum = 2;
		m_ppTable = new listnode*[m_dwTableNum];
	}

	void Release()
	{
		ClearAll();
		ClearFHNode();
		ClearFLNode();

		delete m_plhead;
		delete[] m_phhead;
		delete m_phfree;
		delete m_plfree;
		delete[] m_ppTable;
	}

	void Insert(Key id,T data)
	{
		DWORD index = DWORD(id%m_dwMaxBucket);

		heshnode * htmp = AllocHNode();
		listnode * ltmp = AllocLNode();

		ltmp->hesh = htmp;

		ltmp->prev = m_plhead->prev;
		ltmp->next = m_plhead;
		m_plhead->prev->next = ltmp;
		m_plhead->prev = ltmp;

		*(htmp->id) = id;
		*(htmp->data) = data;
		htmp->list = ltmp;

		heshnode * hh = &m_phhead[index];

		htmp->next = hh->next;
		htmp->prev = hh;
		hh->next->prev = htmp;
		hh->next = htmp;

		InsertTable(htmp->list);

		m_dwCount++;
	}

	bool Remove(Key id)
	{
		DWORD index = DWORD(id%m_dwMaxBucket);
		heshnode * del,* cur = m_phhead[index].next;

		while(cur != &m_phhead[index])
		{
			del = cur;
			cur = cur->next;

			if(*(del->id) != id)
				continue;

			del->list->next->prev = del->list->prev;
			del->list->prev->next = del->list->next;

			RemoveTable(del->list);
			ReleaseLNode(del->list);

			del->next->prev = del->prev;
			del->prev->next = del->next;

			ReleaseHNode(del);

			m_dwCount--;
			return true;
		}
		return false;
	}

	void ClearAll()
	{
		listnode * cur,* del;
		cur = m_plhead->next;

		while(cur != m_plhead)
		{
			del = cur;
			cur = cur->next;

			ReleaseHNode(del->hesh);
			ReleaseLNode(del);
		}

		m_plhead->next = m_plhead;
		m_plhead->prev = m_plhead;

		for(DWORD i = 0;i < m_dwMaxBucket;i++)
		{
			m_phhead[i].prev = &m_phhead[i];
			m_phhead[i].next = &m_phhead[i];
		}

		m_dwCount = 0;
	}

	POS GetData(Key id,T & data)
	{
		POS pos = Find(id);
		if(!pos)
			return NULL;
		data = GetAt(pos);
		return pos;
	}

	POS Find(Key id)
	{
		DWORD index = DWORD(id%m_dwMaxBucket);
		heshnode * old,* cur = m_phhead[index].next;

		while(cur != &m_phhead[index])
		{
			old = cur;
			cur = cur->next;

			if(*(old->id) != id)
				continue;

			return (POS)old->list;
		}
		return NULL;
	}

	POS GetFirstPos()
	{
		if(m_plhead->next == m_plhead)
			return NULL;
		return (POS)m_plhead->next;
	}

	T & GetNext(POS & pos)
	{
		listnode * lh = (listnode *)pos;

		if(lh->next == m_plhead)
			pos = NULL;
		else
			pos = (POS)lh->next;
		return *(lh->hesh->data);
	}

	T & GetAt(POS pos)
	{
		listnode * lh = (listnode *)pos;
		return *(lh->hesh->data);
	}

	void RemoveAt(POS pos)
	{
		listnode * list = (listnode *)pos;

		list->hesh->next->prev = list->hesh->prev;
		list->hesh->prev->next = list->hesh->next;

		ReleaseHNode(list->hesh);

		list->next->prev = list->prev;
		list->prev->next = list->next;

		RemoveTable(list);
		ReleaseLNode(list);
		m_dwCount--;
	}

	DWORD GetCount(){return m_dwCount;}

	T & operator[](DWORD index)
	{
		return *(m_ppTable[index]->hesh->data);
	}

private:
	void InsertTable(listnode * tnode)
	{
		if(m_dwTableNum <= m_dwCount)
		{
			DWORD inccount = m_dwMaxBucket/10 + 1;
			listnode ** tmp = new listnode*[m_dwTableNum + inccount];
			memcpy(tmp,m_ppTable,sizeof(listnode*)*m_dwTableNum);
			delete[] m_ppTable;
			m_ppTable = tmp;
			m_dwTableNum += inccount;
		}
		tnode->index = m_dwCount;
		m_ppTable[m_dwCount] = tnode;
	}
	void RemoveTable(listnode * tnode)
	{
		if(tnode->index >= (m_dwCount - 1))
			return;

		m_ppTable[m_dwCount - 1]->index = tnode->index;
		m_ppTable[tnode->index] = m_ppTable[m_dwCount - 1];

		DWORD inccount = m_dwMaxBucket/10 + 1;

		if(m_dwTableNum - m_dwCount < inccount)
			return;

		listnode ** tmp = new listnode*[m_dwCount];
		memcpy(tmp,m_ppTable,sizeof(listnode*)*m_dwCount);
		delete[] m_ppTable;
		m_ppTable = tmp;
		m_dwTableNum = m_dwCount;
	}
	heshnode * AllocHNode()
	{
		heshnode * p = NULL;
		if(!m_bUseFree || (m_phfree->next == m_phfree))
		{
			p = new heshnode;
			p->id = new Key;
			p->data = new T;
		}
		else
		{
			p = m_phfree->next;
			p->next->prev = p->prev;
			p->prev->next = p->next;
		}
		return p;
	}
	void ReleaseHNode(heshnode * p)
	{
		if(!m_bUseFree)
		{
			delete p;
			return;
		}

		p->next = m_phfree->next;
		p->prev = m_phfree;
		m_phfree->next->prev = p;
		m_phfree->next = p;
	}
	void ClearFHNode()
	{
		heshnode * del, * cur = m_phfree->next;

		while(cur != m_phfree)
		{
			del = cur;
			cur = cur->next;
			delete del;
		}
	}
	listnode * AllocLNode()
	{
		listnode * p = NULL;
		if(!m_bUseFree || (m_plfree->next == m_plfree))
		{
			p = new listnode;
		}
		else
		{
			p = m_plfree->next;
			p->next->prev = p->prev;
			p->prev->next = p->next;
		}
		return p;
	}
	void ReleaseLNode(listnode * p)
	{
		if(!m_bUseFree)
		{
			delete p;
			return;
		}

		p->next = m_plfree->next;
		p->prev = m_plfree;
		m_plfree->next->prev = p;
		m_plfree->next = p;
	}
	void ClearFLNode()
	{
		listnode * del, * cur = m_plfree->next;

		while(cur != m_plfree)
		{
			del = cur;
			cur = cur->next;
			delete del;
		}
	}
};


typedef struct
{
	D3DXVECTOR3	vOrig;
	D3DXVECTOR3 vVector;
}RAY,*LPRAY;