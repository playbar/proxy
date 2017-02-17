//************************************************************************************************************
// class      :  
// purpose    :  广告定义结构体
// Date       :  2008-8-4  9:45:43
// author     :  3o3afe1
//************************************************************************************************************
#if !defined _AD_H
#define _AD_H

#pragma warning(disable:4786)
#include <map>
#include <list>
#include <Set>

#pragma once

#pragma  pack(push ,1)
//关注度信息
struct RESPONSE_FOCUS
{
	enum
	{
		MAX_FOCUS	= 4,
	};
	DWORD	dwAdID;						//	广告ID号
	DWORD	dwDownloadCount;			//	下载次数
	DWORD	dwLoadCount;				//	广告资源被加载的次数
	DWORD	dwClickCount;				//	广告被点击的次数
	struct {
		union
		{
			DWORD	dwNavigateCount;	//	广告浏览或被播放的次数
			DWORD	dwPlayCount;	
		};
		DWORD	dwFullPlayCount;		//	广告被完整播放的次数
		DWORD	dwPlayTime;				//	广告被播放的时长。
	}	pi[MAX_FOCUS];	
};

struct RESPONSE_FOCUS2
{
	
	DWORD	dwAdID;						//	广告ID号
	DWORD	dwDownloadCount;			//	下载次数
	DWORD	dwLoadCount;				//	广告资源被加载的次数

	DWORD	dwClickCount;				//	广告被点击的次数   、、总曝光次数	

	DWORD	dwPlayCount;		        //	最清晰曝光次数
	DWORD	dwFullPlayCount;			//	最清晰整播次数
	DWORD	dwPlayTime;		            //  最清晰曝光时长

	DWORD	dwReserve1;		            //  清晰曝光次数
	DWORD	dwReserve2;		            //  清晰整播次数
	DWORD	dwReserve3;		            //  清晰曝光时长

	DWORD	dwReserve4;		            //  比较清晰曝光次数
	DWORD	dwReserve5;		            //  比较清晰整播次数
	DWORD	dwReserve6;		            //  比较清晰曝光时长

	DWORD	dwReserve7;		            //  模糊曝光次数
	DWORD	dwReserve8;		            //  模糊整播次数
	DWORD	dwReserve9;		            //  模糊曝光时长

};

//为了保持以前的一致

#pragma  warning(disable:4200)
struct IGA_FOCUS_RESPONSE
{
	DWORD	dwLength;		  
	BYTE	byType;			  
	BYTE	byFlags;		  // = 0
	BYTE	byCount;		  
	BYTE	byFrom;			  
	
	BYTE	byRequestType;		//	请求头类型
	BYTE	byFlags2;			//	子包包头类型
	WORD	wRequestHeader;		//	请求头部长度，包括子包头长度
	
	DWORD	dwGameID;                     //游戏id
	union
	{
		struct  {
			DWORD	dwReserved[2];
		};
		struct  {
			DWORD	dwAreaID;
			DWORD	dwAddress;
			
		};
	};
	DWORD	dwAdCount;           //广告数量
	CHAR	szMacID[ 18 ];       //mac地址
	
	RESPONSE_FOCUS   FocusData[0];
};


#pragma pack(pop)


/* 单条广告记录 */
struct  AD_XML_RECORD
{ 
public:
	DWORD      dwID;                //广告id号
	DWORD      dwClose;
	
	
	DWORD      dwAdVersion;         //广告版本号
	DWORD      dwOrigFileVersion;   //目的文件版本号
	
	DWORD      dwAdType;            //_tagADType
	DWORD      dwReserve1;          //用作是否删除
	
	TCHAR      szOrigFile [ _MAX_PATH  ] ;
	TCHAR      szAdFile   [ _MAX_PATH  ] ;
	
    //ftp下载
	TCHAR      szIP       [ 20  ] ;
	TCHAR      szftpDir   [ 256 ] ;

	inline     BOOL IsColsed ()  const { return (dwClose != 0 ) ;}
	
};


/************************************************************************/
/* 广告关注度数据                                                       */
/************************************************************************/
typedef struct tagFocus
{	
	DWORD	dwAdID;						//	广告ID号

	DWORD	dwDownloadCount;			//	下载次数
	DWORD	dwLoadCount;				//	广告资源被加载的次数
	DWORD	dwClickCount;				//	广告被点击的次数  3d
	
	DWORD   dwShowCount;                //  纹理的显示次数  设置纹理次数
	
	DWORD   dwStartPalyTime ;
	DWORD	dwFullPlayCount;		//	广告被完整播放的次数
	DWORD	dwPlayTime;				//	广告被播放的时长。

	

}FOCUS, *LPFOCUS;

/********************************************************************************************************************/
/*                 广告                                                                                             */
/********************************************************************************************************************/
typedef enum
{
    AD_TEXTURE,
	AD_FLASH,
	AD_VIDEO,
	AD_3D,
	AD_ACTIVEFLASH,
	AD_SEARCH
} _tagADType;

enum ENUM_IGA_GAME_LIST
{
		IGA_NO_GAME = 0,
		IGA_FENGYUN,			//风云
		IGA_GONGFUSHIJIE,		//功夫世界
		IGA_GUANGZHIGUODU,		//光之国度
		IGA_SHENQI,				//神泣
		IGA_TEZHONGBUDUI,		//特种部队
		IGA_HANGHAISHIJI,		//航海世纪
		IGA_JIJIASHIJI,			//机甲世纪
		IGA_WUJIEQU	,			//舞街区
		IGA_HANGHAISHIJI_EX	,	//航海世纪革新版
		IGA_JINGTIANDONGDI,		//惊天动地
		IGA_LONGHUMEN,			//龙虎门
		IGA_HAIZEIWANG	,		//海贼王
		IGA_TIANSHANGRENJIAN,	//天上人间
		IGA_JUREN,				//巨人
		IGA_KANGZHAN,			//抗战
		IGA_LUOQI,				//洛奇
		IGA_PAOPAOKADINGCHE,		//跑跑卡丁车
		IGA_REXUEJIANGHU,		//热血江湖
		
		
		IGA_SILUCHUANSHUO,		//丝路传说
		IGA_TIANJI,				//天机
		IGA_TIANLONGBABU,		//天龙八部  
		IGA_WANMEISHIJIE,		//完美世界	
		IGA_ZHUXIAN	,			//诛仙	
		IGA_WEIWUDUZUN,			//唯舞独尊	
		IGA_ZHUOYUEZHIJIAN,		//卓越之剑	
		IGA_CHUNQIU_Q_ZHUAN, 	//春秋Q传	
		IGA_FENGHUOZHILV,		//风火之旅	
		IGA_MENGJIANG,			//猛将	
		IGA_ZHENGFU	,			//征服
		IGA_MOYU,				//征服
		IGA_XINYANG,				//信仰--百年战争	
		IGA_ZHANHUO,				//战火	
		IGA_JIETOULANQIU,		//街头篮球	
		IGA_TIANTANG2,			//天堂2
		IGA_SUN,					//奇迹世界
		IGA_TIANZUN,				//天尊OnLine
		IGA_WLOL,				//武林群侠传 
		IGA_GV,					//大航海时代Online
		IGA_RELIPAIQIU,			//热力排球	
		
		IGA_WUJIEQU_MLXY	=155,	//舞街区(马来西亚)
		IGA_WUJIEQU_XJP,			//舞街区(新加坡)
		IGA_WUJIEQU_FLB		=158,	//舞街区(菲律宾)
};

struct KAd 
{
 	DWORD	       m_dwAdID               ;	 //广告id	
    _tagADType     type                   ;   //广告类型

    DWORD          m_dwAdVersion          ;   //当前广告的版本号
	DWORD          m_dwMatchVersion       ;   //当前广告的版本号

	DWORD          m_bLoad                ;   //此广告是否已经被加载
	tagFocus*      m_pFocus               ;   //广告关注度指针   
};

/*纹理广告*/
struct TextureADContent
{
		VOID*     m_pADTexture    ;               //广告纹理
		VOID*     m_pMatchTexture ;               //匹配纹理
		
		VOID*     m_pSurface      ;         
		VOID*     m_pMexture      ;               //此纹理加载的纹理地址

		BOOL      bAllReplace     ;

};
struct KTextureAD : public KAd
{

	TCHAR     m_adFileName      [ _MAX_PATH ] ;   //广告文件名称
	TCHAR     m_MatchFileName   [ _MAX_PATH ] ;   //匹配文件名称

	typedef   std::map<void*,TextureADContent>     DEV_MAP;
	typedef   DEV_MAP::iterator                    DEV_MAP_IT;
	
	
    DEV_MAP   devs;
 

};

struct KFlashAD   : public KAd
{
    TCHAR     m_FlashName       [ _MAX_PATH ] ;   //flash文件名称
	
};
struct KVideoAD   : public KAd
{
    TCHAR     m_VideoName       [ _MAX_PATH ] ;  //视频文件名称	
};

/*3d模型广告*/
struct  MeshADContent
{

};
struct K3DAD      : public KAd
{
	TCHAR     m_3DName          [ _MAX_PATH ] ;   //3D文件名称	

	typedef   std::map<void*,MeshADContent>     DEV_MAP;
	typedef   DEV_MAP::iterator                 DEV_MAP_IT;
	
	DEV_MAP   devs;

};

struct KActiveFlashAD      : public KAd
{
    TCHAR     m_ActiveFlashName [ _MAX_PATH ] ;   //活动flash文件名称	

	};

struct KSearchAD      : public KAd
{
    TCHAR     m_SearchName [ _MAX_PATH ] ;   //文件名称	

};




/*****************************************************************************************************************/
/******************配置文件中的广告********************************************************************************/




struct  AD_XML_Info
{
public:
    AD_XML_Info()     {    ::InitializeCriticalSection(&m_cr); }
	~AD_XML_Info()    {    ::DeleteCriticalSection(&m_cr)    ; }

	
	inline VOID  add (AD_XML_RECORD&   ad)
							{    m_ads.push_back(ad);	}
	inline VOID  clear()	{   m_ads.clear();	}

	inline BOOL  TryLock () {	return ::TryEnterCriticalSection(&m_cr); }
	inline VOID  Lock  ()   { ::EnterCriticalSection(&m_cr);        }	
	inline VOID  Unlock()   { ::LeaveCriticalSection(&m_cr);        }
	

	
	typedef std::list<AD_XML_RECORD>    AD_RECORD;
	typedef AD_RECORD::iterator         AD_RECORD_IT;
	
	DWORD       dwCRC;
	AD_RECORD   m_ads;

private:	
    CRITICAL_SECTION    m_cr;
	
};

#include <deque>
template < class T>
struct   D3D_DEVICE_ARRAY
{
	typedef std::map<VOID*,T>   D3D_DEVICE_ARRARY_ELEMENT;
	D3D_DEVICE_ARRARY_ELEMENT   d_array;


    D3D_DEVICE_ARRAY(){}
	~D3D_DEVICE_ARRAY(){}	
	
	//设备是否已创建
	BOOL IsCreate(VOID* pDevice ) 
	{
		D3D_DEVICE_ARRARY_ELEMENT::iterator it = d_array.find(pDevice);
		return ( it != d_array.end());
	}
};


class KADManger
{
public:
	/* 广告id和广告指针map*/
	typedef     std::map< DWORD,KAd* >       AD_MAP;
    typedef     AD_MAP::iterator             AD_MAP_IT; 


	//等待匹配的纹理广告id        
    //typedef     std::deque<DWORD>            AD_WAIT_MATCH_ID;
	//typedef     std::deque<DWORD>::iterator  AD_WAIT_MATCH_ID_IT;
    typedef     std::set<DWORD>            AD_WAIT_MATCH_ID;
	typedef     std::set<DWORD>::iterator  AD_WAIT_MATCH_ID_IT;


public:	
	KADManger();
	~KADManger();			

public: 
	inline DWORD  GetXMLCRC()   { return m_xml.dwCRC; }

public:
    inline BOOL   IsLoadingTexture()   { return m_bLoadTexture; }

public:
	//加载本类广告到列表中
	DWORD  Load(VOID*  pDevice);

	/*更新3d模型*/
	BOOL   Update3D_Mesh(AD_XML_RECORD& ad,VOID*  pDevice );

	/*更新flash*/
	BOOL   UpdateFlash(AD_XML_RECORD& ad,VOID*  pDevice );

	/*更新video*/
	BOOL   UpdateVideo(AD_XML_RECORD& ad,VOID*  pDevice );

	/*更新acitve flash*/
	BOOL   UpdateActiveFlash(AD_XML_RECORD& ad,VOID*  pDevice );

	//是否需要升级
    BOOL   IsNeedUpdate(AD_XML_RECORD& rs,BOOL bForceCmp = TRUE,int* lpChange=NULL); 
 
	VOID   Clear(VOID*);
	VOID   ClearAll();


    //DWORD  GetNextTextureID(VOID*  pDevice,BOOL  bRemove = TRUE)  ;
	//VOID   RemoveTextureID(DWORD  dwID);    


	//拷贝有效的关注度到缓冲区
	int   GetFocusDataToBuf(char* pData,DWORD&  dwLen);

	BOOL  GetInforFromAD(RESPONSE_FOCUS2* pOutFocus,tagFocus*  pFocus,_tagADType Type);

    AD_XML_Info   m_xml; 						//XML文件
	AD_MAP        m_ads;						//广告列表
	
    DWORD         m_dwReplaceADTextureCount;	//等待替换的多级纹理

    D3D_DEVICE_ARRAY<AD_WAIT_MATCH_ID>        m_dwNeedReplaceTexture ;

private:
	

	inline BOOL  TryLock () {	return ::TryEnterCriticalSection(&m_cr); }
	inline VOID  Lock  ()   { ::EnterCriticalSection(&m_cr);        }	
	inline VOID  Unlock()   { ::LeaveCriticalSection(&m_cr);        }

private:
	
	typedef std::set<VOID*>         DEV_COUNT_MAP;
	typedef DEV_COUNT_MAP::iterator  DEV_COUNT_MAP_IT;
    DEV_COUNT_MAP		m_dev_count;	//保存加载的设备数量map	    
	
	BOOL				m_bLoadTexture;	//是否正在加载纹理
	
	CRITICAL_SECTION    m_cr;			//锁
};

/*****************************************************************************************************************8
 *  模型点击的回调
 * dwID      :  模型id号
 * lpReserve : 保留
****************************************************************************************************************/
extern VOID WINAPI CallBack3DFocus(DWORD  dwID,LPVOID  lpReserve );
/*****************************************************************************************************************8
 *  更改引用计数的回调
 * pDevice   :  设备指针
 * lnCount   :  更改的引用计数
 * lpReserve :  保留
****************************************************************************************************************/
extern VOID WINAPI CallBackDeviceRefCount(VOID*  pDevice,long lnCount,LPVOID lpReserve);

/*****************************************************************************************************************8
 *  flash  ，video，activeflash播放时长的回调
 * dwID      :  广告id号
 * dwType    :  1:开始播放，2：结束播放
 * dwCurTime :  当前时间
 * lpReserve : 保留
****************************************************************************************************************/
extern VOID WINAPI CallBackPlayTimeFocus(DWORD  dwID,DWORD dwType,DWORD  dwCurTime,LPVOID  lpReserve );

#endif // !defined(AFX_AD_H__4377162B_D0C8_4F5B_B26B_A6EF2FE10401__INCLUDED_)
