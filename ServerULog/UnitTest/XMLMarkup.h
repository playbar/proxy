// Markup.h: interface for the CMarkup class.
//
// Markup Release 8.2
// Copyright (C) 1999-2006 First Objective Software, Inc. All rights reserved
// Go to www.firstobject.com for the latest CMarkup and EDOM documentation
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.

#if !defined(XMLMARKUP_H)
#define XMLMARKUP_H

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
#endif // _MSC_VER > 1000

#ifdef _DEBUG
#define _DS(i) (i?&((LPCTSTR)m_strDoc)[m_aPos[i].nStart]:0)
#define MARKUP_SETDEBUGSTATE m_pMainDS=_DS(m_iPos); m_pChildDS=_DS(m_iPosChild)
#else
#define MARKUP_SETDEBUGSTATE
#endif

class CXMLBase  
{
public:
	CXMLBase() { SetDoc( NULL ); InitDocFlags(); };
	CXMLBase( LPCTSTR szDoc ) { SetDoc( szDoc ); InitDocFlags(); };
	CXMLBase( int nFlags ) { SetDoc( NULL ); m_nFlags = nFlags; };
	CXMLBase( const CXMLBase& markup ) { *this = markup; };
	void operator=( const CXMLBase& markup );
	~CXMLBase() 
	{};

	// Navigate
	virtual bool Load( LPCTSTR szFileName );
	
	bool SetDoc( LPCTSTR szDoc );
	bool IsWellFormed();
	bool FindElem( LPCTSTR szName=NULL );
	bool FindChildElem( LPCTSTR szName=NULL );
	bool IntoElem();
	bool OutOfElem();
	void ResetChildPos() { x_SetPos(m_iPosParent,m_iPos,0); };
	void ResetMainPos() { x_SetPos(m_iPosParent,0,0); };
	void ResetPos() { x_SetPos(0,0,0); };
	CString GetTagName() const;
	CString GetChildTagName() const { return x_GetTagName(m_iPosChild); };
	CString GetData() const { return x_GetData(m_iPos); };
	CString GetChildData() const { return x_GetData(m_iPosChild); };
	CString GetElemContent() const { return x_GetElemContent(m_iPos); };
	CString GetAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib(m_iPos,szAttrib); };
	CString GetChildAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib(m_iPosChild,szAttrib); };
	CString GetAttribName( int n ) const;
	int FindNode( int nType=0 );
	int GetNodeType() { return m_nNodeType; };
	bool SavePos( LPCTSTR szPosName=_T("") );
	bool RestorePos( LPCTSTR szPosName=_T("") );
	const CString& GetError() const { return m_strError; };
	int GetDocFlags() const { return m_nFlags; };
	void SetDocFlags( int nFlags ) { m_nFlags = nFlags; };
	enum MarkupDocFlags
	{
		MDF_IGNORECASE = 8,
	};
	enum MarkupNodeFlags
	{
		MNF_WITHCDATA      = 0x01,
		MNF_WITHNOLINES    = 0x02,
		MNF_WITHXHTMLSPACE = 0x04,
		MNF_WITHREFS       = 0x08,
		MNF_WITHNOEND      = 0x10,
		MNF_ESCAPEQUOTES  = 0x100,
		MNF_NONENDED   = 0x100000,
		MNF_ILLDATA    = 0x200000,
	};
	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
		MNT_LONE_END_TAG			= 128,// 0x80
		MNT_NODE_ERROR              = 32768 // 0x8000
	};
	bool GetOffsets( int& nStart, int& nEnd ) const;

	// Create
	bool Save( LPCTSTR szFileName );
	const CString& GetDoc() const { return m_strDoc; };
	bool AddElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags); };
	bool InsertElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT); };
	bool AddChildElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_CHILD); };
	bool InsertChildElem( LPCTSTR szName, LPCTSTR szData=NULL, int nFlags=0 ) { return x_AddElem(szName,szData,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags); };
	bool InsertElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT); };
	bool AddChildElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_CHILD); };
	bool InsertChildElem( LPCTSTR szName, int nValue, int nFlags=0 ) { return x_AddElem(szName,nValue,nFlags|MNF_INSERT|MNF_CHILD); };
	bool AddAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool AddChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool AddAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool AddChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool AddSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,0); };
	bool InsertSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_INSERT); };
	CString GetSubDoc() const { return x_GetSubDoc(m_iPos); };
	bool AddChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD); };
	bool InsertChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,MNF_CHILD|MNF_INSERT); };
	CString GetChildSubDoc() const { return x_GetSubDoc(m_iPosChild); };
	bool AddNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,0); };
	bool InsertNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,MNF_INSERT); };

	// Modify
	bool RemoveElem();
	bool RemoveChildElem();
	bool RemoveNode();
	bool SetAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPos,szAttrib,szValue); };
	bool SetChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_iPosChild,szAttrib,szValue); };
	bool SetAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPos,szAttrib,nValue); };
	bool SetChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_iPosChild,szAttrib,nValue); };
	bool SetData( LPCTSTR szData, int nFlags=0 ) { return x_SetData(m_iPos,szData,nFlags); };
	bool SetChildData( LPCTSTR szData, int nFlags=0 ) { return x_SetData(m_iPosChild,szData,nFlags); };
	bool SetData( int nValue ) { return x_SetData(m_iPos,nValue); };
	bool SetChildData( int nValue ) { return x_SetData(m_iPosChild,nValue); };
	bool SetElemContent( LPCTSTR szContent ) { return x_SetElemContent(szContent); };

	// Utility
	static bool ReadTextFile( LPCTSTR szFileName, CString& strDoc, CString* pstrError=NULL, int* pnFlags=NULL );
	
	static bool WriteTextFile( LPCTSTR szFileName, CString& strDoc, CString* pstrError=NULL, int* pnFlags=NULL );
	static CString EscapeText( LPCTSTR szText, int nFlags = 0 );
	static CString UnescapeText( LPCTSTR szText, int nTextLength = -1 );

protected:

#ifdef _DEBUG
	LPCTSTR m_pMainDS;
	LPCTSTR m_pChildDS;
#endif

	
	CString m_strDoc;
	CString m_strError;

	int m_iPosParent;
	int m_iPos;
	int m_iPosChild;
	int m_iPosFree;
	int m_iPosDeleted;
	int m_nNodeType;
	int m_nNodeOffset;
	int m_nNodeLength;
	int m_nFlags;

	struct ElemPos
	{
		ElemPos() {};
		ElemPos( const ElemPos& pos ) { *this = pos; };
		enum { EP_STBITS=22, EP_STMASK=0x2fffff, EP_LEVMASK=0xffff };
		int StartTagLen() const { return (nTagLengths & EP_STMASK); };
		void SetStartTagLen( int n ) { nTagLengths = (nTagLengths & ~EP_STMASK) + n; };
		void AdjustStartTagLen( int n ) { nTagLengths += n; };
		int EndTagLen() const { return (nTagLengths >> EP_STBITS); };
		void SetEndTagLen( int n ) { nTagLengths = (nTagLengths & EP_STMASK) + (n << EP_STBITS); };
		bool IsEmptyElement() { return (StartTagLen()==nLength)?true:false; };
		int StartContent() const { return nStart + StartTagLen(); };
		int ContentLen() const { return nLength - StartTagLen() - EndTagLen(); };
		int StartAfter() const { return nStart + nLength; };
		int Level() const { return nFlags & EP_LEVMASK; };
		void SetLevel( int nLev ) { nFlags = (nFlags & ~EP_LEVMASK) | nLev; };
		void ClearVirtualParent() { memset(this,0,sizeof(ElemPos)); };

		// Memory size: 8 32-bit integers == 32 bytes
		int nStart;
		int nLength;
		int nTagLengths; // 22 bits 4MB limit for start tag, 10 bits 1K limit for end tag
		int nFlags; // 16 bits flags, 16 bits level 65536 depth limit
		int iElemParent;
		int iElemChild; // first child
		int iElemNext;
		int iElemPrev; // if this is first child, iElemPrev points to last
	};

	enum MarkupNodeFlagsInternal
	{
		MNF_REPLACE    = 0x001000,
		MNF_INSERT     = 0x002000,
		MNF_CHILD      = 0x004000,
		MNF_QUOTED     = 0x008000,
		MNF_EMPTY      = 0x010000,
		MNF_DELETED    = 0x020000,
		MNF_FIRST      = 0x080000,
		MNF_PUBLIC     = 0x300000,
		MNF_ILLFORMED  = 0x800000,
		MNF_USER      = 0xf000000,
	};

	struct NodePos
	{
		NodePos() {};
		NodePos( int n ) { nFlags=n; nNodeType=0; nStart=0; nLength=0; };
		int nNodeType;
		int nStart;
		int nLength;
		int nFlags;
		CString strMeta;
	};

	struct TokenPos
	{
		TokenPos( LPCTSTR sz, int n ) { Clear(); szDoc=sz; nTokenFlags=n; };
		void Clear() { nL=0; nR=-1; nNext=0; };
		int Length() const { return nR - nL + 1; };
		bool Match( LPCTSTR szName )
		{
			int nLen = nR - nL + 1;
			if ( nTokenFlags & MDF_IGNORECASE )
				return ( (_tcsncicmp( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || _tcschr(_T(" =/[]"),szName[nLen]) ) );
			else
				return ( (_tcsnccmp( &szDoc[nL], szName, nLen ) == 0)
					&& ( szName[nLen] == _T('\0') || _tcschr(_T(" =/[]"),szName[nLen]) ) );
		};
		int nL;
		int nR;
		int nNext;
		LPCTSTR szDoc;
		int nTokenFlags;
		int nPreSpaceStart;
		int nPreSpaceLength;
	};

	struct SavedPos
	{
		SavedPos() { nSavedPosFlags=0; iPos=0; };
		CString strName;
		int iPos;
		int nSavedPosFlags;
	};

	struct SavedPosMap
	{
		SavedPosMap() { pTable = NULL; };
		~SavedPosMap() { RemoveAll(); };
		void RemoveAll() { if (pTable) Release(); pTable=NULL; };
		enum { SPM_SIZE = 7, SPM_MAIN = 1, SPM_CHILD = 2, SPM_USED = 4, SPM_LAST = 8 };
		void Release() { for (int n=0;n<SPM_SIZE;++n) if (pTable[n]) delete[] pTable[n]; delete[] pTable; };
		void AllocMapTable() { pTable = new SavedPos*[SPM_SIZE]; for (int n=0; n<SPM_SIZE; ++n) pTable[n]=NULL; };
		int Hash( LPCTSTR szName ) { int n=0; while (*szName) n += *szName++; return n % SPM_SIZE; };
		SavedPos** pTable;
	};
	SavedPosMap m_mapSavedPos;

	struct PosArray
	{
		PosArray() { Clear(); };
		~PosArray() { Release(); };
		enum { PA_SEGBITS = 16, PA_SEGMASK = 0xffff };
		void RemoveAll() { Release(); Clear(); };
		void Release() { for (int n=0;n<SegsUsed();++n) delete[] (char*)pSegs[n]; if (pSegs) delete[] (char*)pSegs; };
		void Clear() { nSegs=0; nSize=0; pSegs=NULL; };
		int GetSize() const { return nSize; };
		int SegsUsed() const { return ((nSize-1)>>PA_SEGBITS) + 1; };
		ElemPos& operator[](int n) const { return pSegs[n>>PA_SEGBITS][n&PA_SEGMASK]; };
		ElemPos** pSegs;
		int nSize;
		int nSegs;
	};
	PosArray m_aPos;

	struct NodeStack
	{
		NodeStack() { nTop=-1; nSize=0; pN=NULL; };
		~NodeStack() { if (pN) delete [] pN; };
		NodePos& Top() { return pN[nTop]; };
		NodePos& At( int n ) { return pN[n]; };
		void Add() { ++nTop; if (nTop==nSize) Alloc(nSize*2+6); };
		void Remove() { --nTop; };
		int TopIndex() { return nTop; };
	protected:
		void Alloc( int nNewSize ) { NodePos* pNNew = new NodePos[nNewSize]; Copy(pNNew); nSize=nNewSize; };
		void Copy( NodePos* pNNew ) { for(int n=0;n<nSize;++n) pNNew[n]=pN[n]; if (pN) delete [] pN; pN=pNNew; };
		NodePos* pN;
		int nSize;
		int nTop;
	};

	void x_SetPos( int iPosParent, int iPos, int iPosChild )
	{
		m_iPosParent = iPosParent;
		m_iPos = iPos;
		m_iPosChild = iPosChild;
		m_nNodeOffset = 0;
		m_nNodeLength = 0;
		m_nNodeType = iPos?MNT_ELEMENT:0;
		MARKUP_SETDEBUGSTATE;
	};
	int x_GetFreePos()
	{
		if ( m_iPosFree == m_aPos.GetSize() )
			x_AllocPosArray();
		return m_iPosFree++;
	};
	bool x_AllocPosArray( int nNewSize = 0 );

	void InitDocFlags()
	{
		// To always ignore case, define MARKUP_IGNORECASE
	#ifdef MARKUP_IGNORECASE
		m_nFlags = MDF_IGNORECASE;
	#else
		m_nFlags = 0;
	#endif
	};

	bool x_ParseDoc();
	int x_ParseElem( int iPos, TokenPos& token );
	static bool x_FindAny( LPCTSTR szDoc, int& nChar );
	static bool x_FindName( TokenPos& token );
	static CString x_GetToken( const TokenPos& token );
	int x_FindElem( int iPosParent, int iPos, LPCTSTR szPath ) const;
	CString x_GetPath( int iPos ) const;
	CString x_GetTagName( int iPos ) const;
	CString x_GetData( int iPos ) const;
	CString x_GetAttrib( int iPos, LPCTSTR szAttrib ) const;
	static CString x_EncodeCDATASection( LPCTSTR szData );
	bool x_AddElem( LPCTSTR szName, LPCTSTR szValue, int nFlags );
	bool x_AddElem( LPCTSTR szName, int nValue, int nFlags );
	CString x_GetSubDoc( int iPos ) const;
	bool x_AddSubDoc( LPCTSTR szSubDoc, int nFlags );
	static bool x_FindAttrib( TokenPos& token, LPCTSTR szAttrib, int n=0 );
	bool x_SetAttrib( int iPos, LPCTSTR szAttrib, LPCTSTR szValue );
	bool x_SetAttrib( int iPos, LPCTSTR szAttrib, int nValue );
	bool x_AddNode( int nNodeType, LPCTSTR szText, int nFlags );
	void x_RemoveNode( int iPosParent, int& iPos, int& nNodeType, int& nNodeOffset, int& nNodeLength );
	void x_AdjustForNode( int iPosParent, int iPos, int nShift );
	static bool x_CreateNode( CString& strNode, int nNodeType, LPCTSTR szText );
	int x_InsertNew( int iPosParent, int& iPosRel, NodePos& node );
	void x_LinkElem( int iPosParent, int iPosBefore, int iPos );
	int x_UnlinkElem( int iPos );
	int x_ReleaseSubDoc( int iPos );
	int x_ReleasePos( int iPos );
	void x_CheckSavedPos();
	static int x_ParseNode( TokenPos& token, NodePos& node );
	bool x_SetData( int iPos, LPCTSTR szData, int nFlags );
	bool x_SetData( int iPos, int nValue );
	int x_RemoveElem( int iPos );
	CString x_GetElemContent( int iPos ) const;
	bool x_SetElemContent( LPCTSTR szContent );
	void x_DocChange( int nLeft, int nReplace, const CString& strInsert );
	void x_Adjust( int iPos, int nShift, bool bAfterPos = false );
};

class CXMLMakeUp : public CXMLBase
{

public:
	CXMLMakeUp(): m_byDoc(NULL),m_ilength(0) {}
	~CXMLMakeUp()
	{
		if ( NULL != m_byDoc )
		{
			delete [] m_byDoc;
			m_byDoc = NULL;
		}
	}

	virtual bool Load( LPCTSTR szFileName );
	bool         Load(byte *byData, int ilength);

	bool ReadTextFile( LPCTSTR szFileName, CString* pstrError=NULL, int* pnFlags=NULL );
	bool DecryptionString(); //对m_byDoc进行加密
	bool EncryptionString(); //对m_byDoc进行解密


private:
	byte*   m_byDoc;
	int		m_ilength;

};


#endif // !defined(XMLMARKUP_H)


/*
CMarkup概述
 
创建一个新的XML文档，分析修改现存的XML文档，所有的这些都中可以从一个利用MFC或STL字符串所实现
的一个简单类的方法中得到。关于CMarkup，你可以运行Zip文件右边的例子，如果你愿意，你可以通过看
源码，几分钟把它加入到你自己的应用中。
特征
Cmarkup是依据EDOM的，这是这个简单XML分析的关键。EDOM是操作XML元素、属性和数据的简明方法集。
当你不需要复杂的XML特性时，它避免你读大量的DOM和SAX文档。
如果你希望使用微软的MSXML，CMarkup工程中CmarkupMSXML类全面的演示了通过EDOM来使用MSXML，并且
它是以C++ COM语法开始。
下载包（看上面zip文件的链接）中包含了测试对话框工程的源码、Cmarkup类的所有测试和示例、以及编
译选项。关于用在商业用途中的详细说明请看文章底部的在Licensing中的注意事项。        
下面列出了CMarkup的特征：
独立：不需要任何外部的XML组件。
小：编译到你的工程只是很小的一个类，并且对于文档只维持了一个字符串，以及一个总计小于字符串内
存大小的索引数组。
快：分析器在相当快的建立索引数组。
简单：EDOM方法使创建、导航和修改ＸＭＬ无比的简单。
MSXML：CMarkupMSXML类用EDOM方法包装了微软的XML服务。MSXML编译选项用来演示这个类。
UNICODE（统一字符编码标准）：对于WindowsNT和WindowsCE平台能够被编译成UNICODE，在WindowsNT 
UNICODE下，XML文档是流到UTF-8文件中，但是内部用宽字符。
UTF-8：对于UTF-8文件，这接受和返回UTF-8字符串（确保_MBCS没有被定义）
MBCS：对于双字节数据（定义_MBCS），它不与UTF-8兼容.
STL：CMarkupSTL类完全没有使用MFC，它的演示要使用STL编译选项。
 
每天数据的XML
 我们经常在一个文件里需要保存或传递信息，或从计算机Ａ 发送一个信息块到计算机Ｂ。总是有这样一
个问题出现：我要怎样安排这些数据的格式？出现XML之前，你可能要考虑“env”类型，例如
PATH=C:\WIN95；　“ini”类型（一些部分的组合），逗号限制或其它一些限制、或者是固定了字符串的
长度。现在XML就是被确定的这些问题的答案，但是某些时候，程序员很气馁，他们需要一些便于帮助分
析和格式尖括号所有这些的解决XML的复杂性。对于最低限度读XML标签匹配规则，我建议阅读在
CcodeProject站点的“Beginning XML - Chapter 2: Well-Formed XML”
XML更好是因为它的灵活和自然的等级，加上它广泛的应用。虽然XML比限定格式用了更多的字符，如果需
要的话它可以良好的向下压缩。当你需要扩展你文档信息类型时，XML的灵活性得到了体现，不需要要求
信息的每个消费者重写逻辑过程。你能够保持旧的信息标识，并且制定用相同的方式增加新属性和元素。 
 
使用CMarkup
Cmarkup是基于“压缩”文档对象模型(EDOM ： "Encapsulated" Document Object Model,)，这是这个简
单XML处理器的关键。它的一套XML处理方法和产生的效果与DOM(Document Object Model)是一样的。但是
整个DOM有无数的对象类型，而EDOM只定义了一个对象：XML文档，EDOM回到了XML原始的吸引力—简单。
关于方法的完整文档，请看EDOM Specification.
CmarkUp封装了XML文档文本、结构和当前位置。它提供了增加元素、导航和得到元素属性和数据的所有方
法以。在文档中操作被执行的地方取决于当前位置和当前孩子位置。当前位置允许你增加一个元素到文档
中的这个位置，而不需要明确指出增加对象到文档的这点上，在CMarkup的有效生命周期里，对象一直保
持着一个用来描述文档的字符串，当你调用GetDoc.可以得到它。  
查看free firstobject XML editor的源码，这是用CMarkup创建、导航你自己的XML文档。
创建一个XML文档
对于创建一个XML文档，需要实例化一个CMarkup对象，并调用AddElem创建根元素。.在这个位置，如果你
调用 AddElem("ORDER") ，你的文档会简单的装一个空ORDER元素<ORDER/>. 然后调用AddChildElem 在根
元素的下面创建元素 (例如：“进入”根元素内部，层次表示).下面的示例代码创建一个XML文档并返回
它（的内容）到一个字符串中。
CMarkup xml;
xml.AddElem( "ORDER" );
xml.AddChildElem( "ITEM" );
xml.IntoElem();
xml.AddChildElem( "SN", "132487A-J" );
xml.AddChildElem( "NAME", "crank casing" );
xml.AddChildElem( "QTY", "1" );
CString csXML = xml.GetDoc();
这些代码产生了下面的XML，这个根结点是ORDER元素；注意它的开始标签<ORDER> 在开头，结束标签
</ORDER>在结尾。当一个元素是在一个父下面（深入或被包含），这个父元素的开始标签要在它之前，结
束标签要在它之后。ORDER元素包含一个ITEM元素，而ITEM元素包含了三个字子元素：SN、NAME和QTY；
<ORDER>
<ITEM>
<SN>132487A-J</SN>
<NAME>crank casing</NAME>
<QTY>1</QTY>
</ITEM>
</ORDER>
如例子中所显示的，你也能够在一个子元素下创建新元素，这需要调用IntoElem 移动你的当前主位置到
当前子元素位置，然后你就可以在这下面增加一个子元素了。CMarkup在索引中保持了一个当前位置指针
，以保证你的源码更加短和更简单，当导航文件时，相同的逻辑位置也会被使用。
导航XML文档
上面的例子所创建的XML字符串，用SetDoc方法加入到CMarkup对象中能够被解析，你也可以引导它正确的
进入被创建的同一个CMarkup对象中，如果你要设置当前位置到文档的开始时，需要调用ResetPos.
在下面的例子中，从csXML字符串生成CMarkup对象后，我们循环ORDER元素下的所有ITEM元素，并得到每
个项目的序号和数量。
CMarkup xml;
xml.SetDoc( csXML );
while ( xml.FindChildElem("ITEM") )
{
    xml.IntoElem();
    xml.FindChildElem( "SN" );
    CString csSN = xml.GetChildData();
    xml.FindChildElem( "QTY" );
    int nQty = atoi( xml.GetChildData() );
    xml.OutOfElem();
}
对于我们发现的每个元素，在查询它了子元素之前要调用IntoElem，查询完之后再调用OutOfElem ，当你
习惯于这种导航类型时，你将知道，检查你的循环时，要确定每个IntoElem 调用都有一个与之对应的
OutOfElem 调用 。
增加元素和属性
上面创建文档的例子中仅创建了一个ITEM元素，现在这个例子是创建多个项目，从前一个内容加裁后，再
增加数据源，加上SHIPMENT信息元素中有一个属性，这段代码也演示了你能调用调用IntoElem和AddElem
来代替AddChildElem,函数调用。虽然这意味着更多的调用，但许多人认为这样更直观。
CMarkup xml;
xml.AddElem( "ORDER" );
xml.IntoElem(); // inside ORDER
for ( int nItem=0; nItem<aItems.GetSize(); ++nItem )
{
    xml.AddElem( "ITEM" );
    xml.IntoElem(); // inside ITEM
    xml.AddElem( "SN", aItems[nItem].csSN );
    xml.AddElem( "NAME", aItems[nItem].csName );
    xml.AddElem( "QTY", aItems[nItem].nQty );
    xml.OutOfElem(); // back out to ITEM level
}
xml.AddElem( "SHIPMENT" );
xml.IntoElem(); // inside SHIPMENT
xml.AddElem( "POC" );
xml.SetAttrib( "type", csPOCType );
xml.IntoElem(); // inside POC
xml.AddElem( "NAME", csPOCName );
xml.AddElem( "TEL", csPOCTel );
这段代码产生了下面的XML，根元素ORDER包含两个ITEM元素和一个SHIPMENT元素，ITEM元素全都包含SN、
NAME、和QTY元素，SHIPMENT元素包含一个带有属性类型的POC元素，和NAME及TEL子元素。
<ORDER>

<ITEM>

<SN>132487A-J</SN>

<NAME>crank casing</NAME>

<QTY>1</QTY>

</ITEM>

<ITEM>

<SN>4238764-A</SN>

<NAME>bearing</NAME>

<QTY>15</QTY>

</ITEM>

<SHIPMENT>

<POC type="non-emergency">

<NAME>John Smith</NAME>

<TEL>555-1234</TEL>

</POC>

</SHIPMENT>

</ORDER>

查找元素
FindElem 和 FindChildElem方法用于到下一个兄弟元素。如果可选的标签名被指定，那么它们将到下一
个与标签名相匹配的元素，被发现的元素是当前元素，并且下次调用Find将会到当前位置后的下一个兄弟
或下一个匹配兄弟。
当你无法判断元素的索引时，在调用两个Find方法之间，一定要复位当前位置。看上面的例子中ITEM元素
，如果是别的人创建的XML文件，你不能确定SN元素在QTY元素之前，那么在查找QTY元素之前就要调用
ResetChildPos();
对于用一个特定的序号去查找元素，你需要完全循环ITEM元素，并比较SN元素的数据和你正在搜索的序号
。这个例子不同于先前导航的例子，它调用IntoElem 进入到ORDER元素，并且用FindElem("ITEM")替换
FindChildElem("ITEM");其实两种方式都挺好。需要注意的是，在Find方法中指定ITEM元素的标签名，我
们会忽略所有其它的兄弟元素，例如SHIPMENT元素。
CMarkup xml;
xml.SetDoc( csXML );
xml.FindElem(); // ORDER element is root
xml.IntoElem(); // inside ORDER
while ( xml.FindElem("ITEM") )
{
    xml.FindChildElem( "SN" );
    if ( xml.GetChildData() == csFindSN )
        break; // found
}
编码
ASCII编码引用了我们所依靠的字符码128以下的字符，如用英语编程。如果你只使用ASCII码，很方便，
UTF-8编程与你拉公共ASCII集相同。
如果你所使用的字符集不在Unicode编码集(UTF-8，UTF-16，UCS-2)中，那么出于交互性以及在IE中很好
的显示，你真的需要在XML声明中进行描述。像ISO-8859-1(西欧)字符集指定字符值在一个比特且在128到
255之间。以便每个字符仍然使用一个比特。Windows双字节字符集像GB2312，Shift_JIS和EUC-KR，每个
字符都是用一个或两个字节，对于这些Windows字符集，在你的预处理中需要定义_MBCS ，并要确定用户
的操作系统设置到合适的编码页。
关于用一个XML描述的XML文档前缀，像<?xml version="1.0" encoding="ISO-8859-1"?>，需要通过用
SetDoc或Cmarkup的构造函数来传递。在结尾要包括回车符，这样根结点会显示在下一行。
xml.SetDoc( "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\r\n" );
xml.AddElem( "island", "Cura?ao" );
测试对框
Markup.exe是一Cmarkup的测试程序，这是一个VC6下的MFC工程。当它开始时，工程在OnTest函数里运行
诊断程序，在选择了特定的编译选项下测试Cmarkup.,你可以一步一步的调试OnTest函数，会看到怎样用
Cmarkup的一些例子，用Open和Parse函数测试一个文件。
在下面的插图里，显示的编译版本是“CMarkup 7.0 Unicode”，这意味着Cmarkup类是用在_UNICODE定义
下编译的。成功的运行了RunTest，分析错误在Charsets_err.xml文件中被遇到，文件被加载，文件原是
1500比特转换到1033Unicode宽字符(例如2066字节)，但是它遇到了一个结束标签与起始标签不对应的错
误 。

这个测试对话框保存着最后一个被分析文件和对话框的屏幕位置信息，这些都被注册表中的
HKEY_CURRENT_USER/ Software/ First Objective Software/ Markup/ Settings.键下。

*/

//////////////////////////
/**************这样也可以**********************
 vc中操作Xml--使用CMarkup类 
DotNet中使用xml相对比较轻松，在vc中操作xml，有些繁琐。最近在网络上得知CMarkup比较好用，于是
试了下，还好，就把部分例子写出来。以后有时间的话，再写清楚点
//----------UserInfo.xml--------------
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
1。生成UserInfo.xml
CMarkup xml;
xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
xml.AddElem("UserInfo");
xml.IntoElem();
xml.AddElem("UserID","luo");
xml.AddElem("UserID","lin");
xml.OutOfElem();
xml.Save("UserInfo.xml");
2。浏览特定元素
//----查看所有UserID----
CMarkup xml;
xml.Load("UserInfo.xml");
BOOL bFind = true;
xml.ResetMainPos();
while (xml.FindChildElem("UserID");)
{
       //此时接点还是父接点
         CString strTagName = _T("");
         CString strData = _T("");
         strTagName = xml.GetChildTagName();
         strData = xml.GetChildData();
         TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
}
       /********************这样也可以******************
       CMarkup xml;
       xml.Load("UserInfo.xml");
       BOOL bFind = true;
       xml.ResetMainPos();
       while (xml.FindChildElem("UserID");)
       {
                    xml.IntoElem();
                     CString strTagName = _T("");
                     CString strData = _T("");
                     strTagName = xml.GetTagName();
                     strData = xml.GetData();
                     TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
                     xml.OutOfElem();

       }
       
       CMarkup xml;
       xml.Load("UserInfo.xml");
       BOOL bFind = true;
       xml.ResetMainPos();
       xml.FindElem();  //UserInfo
       xml.IntoElem();
       while (xml.FindElem("UserID");)
       {
                     CString strTagName = _T("");
                     CString strData = _T("");
                     strTagName = xml.GetTagName();
                     strData = xml.GetData();
                     TRACE("\n---tagName:%s,Data:%s--\n",strTagName,strData);
       }
       
//-------------结果----------
---tagName:UserID,Data:luo--
---tagName:UserID,Data:lin—
3。修改
//--------把UserID为”luo”改为”flypigluo”-----------
BOOL bLoadXml = false;
CMarkup xml;
bLoadXml = xml.Load("UserInfo.xml");
       if (bLoadXml)
       {
             CString strUserID = _T("");
              xml.ResetMainPos();
              xml.FindElem();
              xml.IntoElem();
              while (xml.FindElem("UserID"))
              {
                     strUserID = xml.GetData();
                    if (strUserID=="luo")
                     {
                           xml.SetData(CString("flypig")+strUserID);
                           xml.Save("UserInfo.xml");
                            break;
                     }
              }
       }
4。添加
4。1   添在最后面（使用的是AddElem）
//加在最后面
       BOOL bLoadXml = false;
       CMarkup xml;
       bLoadXml = xml.Load("UserInfo.xml");
       if (bLoadXml)
       {
              xml.ResetMainPos();
              xml.FindElem();
              xml.IntoElem();
              xml.AddElem("UserID","luoluo");
              xml.OutOfElem();
              xml.Save("UserInfo.xml");
       }
//--------原来为------
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
//---------添加后为-------------
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  <UserID>luoluo</UserID> 
  </UserInfo>
4。2  加在最前面（使用InsertElem）
BOOL bLoadXml = false;
CMarkup xml;
bLoadXml = xml.Load("UserInfo.xml");
if (bLoadXml)
{
       xml.ResetMainPos();
       xml.FindElem();
       xml.IntoElem();
       xml.InsertElem("UserID","AddUserIDHead");
       xml.OutOfElem();
       xml.Save("UserInfo.xml");
}
//----原来为—
<?xml version="1.0" encoding="UTF-8" ?> 
 <UserInfo>
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
//------修改后-----
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
  <UserID>AddUserIDHead</UserID> 
  <UserID>luo</UserID> 
  <UserID>lin</UserID> 
  </UserInfo>
 5。删除
CMarkup xml;
xml.Load("UserInfo.xml");
BOOL bFind = true;
xml.ResetMainPos();       
while (bFind)
{
       bFind = xml.FindChildElem("UserID");
       if (bFind)
       {
              //此时接点还是父接点
              CString strData = _T("");
              strData = xml.GetChildData();
              if (strData=="luo")
              {
                     xml.RemoveChildElem();
                     xml.Save("UserInfo.xml");
                     break;
              }
       }
}
6。查找
见第二点浏览。类似
上面的例子里面没有属性这些东西，下面的一个例子是有属性的
//UserInfo1.xml
<?xml version="1.0" encoding="UTF-8" ?> 
<UserInfo>
 <User UserID="UserID00" UserPwd="UserPwd00" UserLevel="158" /> 
  <User UserID="UserID01" UserPwd="UserPwd01" UserLevel="162" /> 
  <User UserID="UserID02" UserPwd="UserPwd02" UserLevel="165" /> 
  <User UserID="UserID03" UserPwd="UserPwd03" UserLevel="168" /> 
  <User UserID="UserID04" UserPwd="UserPwd04" UserLevel="171" /> 
  <User UserID="UserID05" UserPwd="UserPwd05" UserLevel="175" /> 
  <User UserID="UserID06" UserPwd="UserPwd06" UserLevel="178" /> 
  <User UserID="UserID07" UserPwd="UserPwd07" UserLevel="181" /> 
  <User UserID="UserID08" UserPwd="UserPwd08" UserLevel="184" /> 
  <User UserID="UserID09" UserPwd="UserPwd09" UserLevel="188" /> 
  </UserInfo>
7。产生
CString strID = _T("");
       CString strPwd = _T("");
       CMarkup xml;
       xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
       xml.AddElem("UserInfo");
       xml.IntoElem();
       for (int i=0; i<10; i++)
       {
              //产生一个随机数，作为权限
              srand(UINT(time(NULL)+i));
              int nLevel = rand()%256;
              strID.Format("UserID%02d",i);
              strPwd.Format("UserPwd%02d",i);
              xml.AddElem("User");
              xml.AddAttrib("UserID",strID);
              xml.AddAttrib("UserPwd",strPwd);
             xml.AddAttrib("UserLevel",nLevel);      
	}
       xml.OutOfElem();
       xml.Save("UserInfo1.xml");
8。查找
CMarkup xml;
BOOL bLoad = false;
BOOL bFind = false;
bLoad = xml.Load("UserInfo1.xml");
       if (bLoad)
       {
              CString strID;
              CString strPwd;
              xml.ResetMainPos();
              xml.FindElem();    //UserInfo
              while (xml.FindChildElem("User"))
              {
                     strID = xml.GetChildAttrib("UserID");
                     strPwd = xml.GetChildAttrib("UserPwd");
                     TRACE("\n----id:%s,pwd:%s-------\n",strID,strPwd);
                     if (strID=="UserID00"&&strPwd=="UserPwd00")
                     {
                           bFind = true;
                            break;
                     }
              }
       }
       if (bFind)
       {
              TRACE("\n---- find------\n");
       }
      else
      {
             TRACE("\n----no find------\n");
       }
9。修改
CMarkup xml;
       BOOL bLoad = false;
       BOOL bFind = false;
       bLoad = xml.Load("UserInfo1.xml");
       if (bLoad)
       {
             CString strID;
              CString strPwd;
             xml.ResetMainPos();
              xml.FindElem();    //UserInfo
             while (xml.FindChildElem("User"))
              {
                     strID = xml.GetChildAttrib("UserID");
                     strPwd = xml.GetChildAttrib("UserPwd");
                     TRACE("\n----id:%s,pwd:%s-------\n",strID,strPwd);
                     if (strID=="UserID00"&&strPwd=="UserPwd00")
                     {
                            bFind = true;
                            xml.SetChildAttrib("UserID",strID+CString("Modify"));
                            xml.Save("UserInfo1.xml");
                            break;
                   }
              }
       }
       if (bFind)
       {
              TRACE("\n---- find------\n");
       }
       else
       {
              TRACE("\n----no find------\n");
       }
*/
