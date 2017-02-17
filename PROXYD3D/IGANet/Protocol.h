// Protocol.h: interface for the Protocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_)
#define AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/************************************************************************/
/* 通信报类型                                                           */
/************************************************************************/
typedef enum ePACKETTYPE
{
	PACKETTYPE_NONE		= 0x0,	//
	PACKETTYPE_CONNECT,			// Connect packet
	PACKETTYPE_RECONNECT,		// Reconnect packet
	
	PACKETTYPE_STREAM,			// Stream packet

	PACKETTYPE_MESSAGE,			//	消息包
	PACKETTYPE_BULLETIN,		//	公告信息包
	PACKETTYPE_TALKS,			//	聊天信息
	PACKETTYPE_MAX		= 0xFF,
}PACKETTYPE;

/************************************************************************/
/* 子包类型                                                             */
/************************************************************************/
typedef enum eSUBPACKETTYPE
{
	SUBPACKETTYPE_FILE,		
	SUBPACKETTYPE_DATA,
}SUBPACKETTYPE;

/************************************************************************/
/* 通信包压缩或加密类型                                                 */
/************************************************************************/
typedef enum ePACKETFLAGS
{
	PACKETFLAGS_NONE,				// 既不压缩，也不加密
	PACKETFLAGS_COMPRESSEDONLY,		// 仅仅压缩
	PACKETFLAGS_ENCRYPTEDONLY,		// 仅仅加密
	PACKETFLAGS_COMPRESSENCRYPTED,	// 先压缩后加密
	PACKETFLAGS_ENCRYPTCOMPRESSED,	// 先加密后压缩
	PACKETFLAGS_MD5,				// MD5算法加密
}PACKETFLAGS;

/************************************************************************/
/* 请求或应答类型                                                       */
/************************************************************************/
typedef enum eREQUESTTYPE
{
	REQUESTTYPE_HARDWAREINFO,		//	硬件信息 
	REQUESTTYPE_GAMEINFO,			//	游戏媒体信息
	REQUESTTYPE_ADINFO,				//	广告位信息
	REQUESTTYPE_SDKUPDATEINFO,		//	SDK升级信息
	REQUESTTYPE_FILEINFO,			//	文件升级信息
	REQUESTTYPE_FOCUS,				//	广告关注度数据
	REQUESTTYPE_BULLETIN,			//	公告信息
	REQUESTTYPE_MESSAGE,			//	通信消息
	REQUESTTYPE_CHAT,				//	聊天消息
	REQUESTTYPE_AD,					//	
	REQUESTTYPE_PLAYINFO,			//
}REQUESTTYPE;

//////////////////////////////////////////////////////////////////////////
//	通信包来源
typedef enum ePacketFrom
{
	PACKETFROM_IIS,	//	来自于IIS服务
	PACKETFROM_PLAYER,	//	来自于玩家的请求
	PACKETFROM_ROUTER,	//	来自于Router的请求
	PACKETFROM_FRONTER,	//	来自于Fronter的请求
	PACKETFROM_PROXY,	//	来自于内部网吧代理服务器的请求或应答
}PACKETFROM;


#pragma pack(push, 1)

#define SERVER_TIME


/// General packet header item
typedef struct tagPacketHeader 
{
	DWORD	dwLength;		// General packet length including this packet header
#ifdef SERVER_TIME
	DWORD	dwSrvTime;		//	the tick of the services
#endif
	BYTE	byType;			// Packet type, PACKETTYPE_XXXX
	BYTE	byFlags;		// Packet flags. such as crypt or compressed
	BYTE	byCount;		// Sub packet count
	BYTE	byFrom;			// Reserved;
}PACKETHEADER, *LPPACKETHEADER;

/************************************************************************/
/* 子报头                                                               */
/************************************************************************/
typedef struct tagSubPacketHeader 
{
	BYTE	bySubPacketType;		//	子包类型
	union
	{
		BYTE	byReserved;
		BYTE	byResponseState;
	};
	WORD	wHeaderLength;			//	子包包头长度
}SUBPACKETHEADER, *LPSUBPACKETHEADER;

/************************************************************************/
/*	请求头或应答包头                                                    */
/************************************************************************/
typedef struct tagRequestHeader 
{
	BYTE	byRequestType;		//	请求头类型
	BYTE	byFlags;			//	子包包头类型
	WORD	wRequestHeader;		//	请求头部长度，包括子包头长度
}REQUESTHEADER, *LPREQUESTHEADER;

#endif // !defined(AFX_PROTOCOL_H__ABA4BE9F_3FB2_42DD_A492_4ADCC4AD3828__INCLUDED_)
