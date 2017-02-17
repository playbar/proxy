/*
* serve u
*/
#pragma once

#define REVNT_None	0	// nothing
#define REVNT_Proceed	1	// let event pass
#define REVNT_Abort	2	// stop event
#define REVNT_Suspend	3	// suspend event until decision is made

struct RFTPEventStr 
{
	// event info
	DWORD  Event;	// event code
	DWORD  SubEvent;	// sub-event code

	// user info
	DWORD SessionID;	// unique ID of the FTP session
	char User[151];	// user name
	char ClientIP[16];	// IP address of client
	char DomainIP[16];	// server IP address the client connected to

	// event attributes
	DWORD Duration;	// duration of events (in ms)
	DWORD Size;	// size of object (i.e. transferred)

	// hook info
	HWND hWindow;	// window handle to post decision to

	UINT Message;	// message to post
	char* pReplyText;	// pointer to text to send to user

	// scratch pad area
	char AuxOne[512];	// auxiliary area one
	char AuxTwo[512];	// auxiliary area two

	// domain info
	DWORD DomainPort;	// server port the client connected to
	DWORD DomainID;	// unique ID for the domain the client connected to

	// more size info
	DWORD HiSize;	// high 32 bits of size info (full size is 64 bit value)
};



#define EVNT_None 	0	// none
#define EVNT_IPName 	1	// symbolic IP name available
#define EVNT_Connect 	2	// connection was made
#define EVNT_Close 	3	// closed connection
#define EVNT_BouncedIP 	4	// bounced client because of IP address
#define EVNT_TooMany 	5	// bounced user because there are too many
#define EVNT_WrongPass 	6	// too many times wrong password

#define EVNT_TimeOut 	7	// connection timed out
#define EVNT_Login 	8	// use logged in
#define EVNT_StartUp 	9	// start upload of file
#define EVNT_EndUp 	10	// successful upload of file
#define EVNT_StartDown 	11	// start of download of file
#define EVNT_EndDown 	12	// successful download of file
#define EVNT_AbortUp  	13	// aborted upload
#define EVNT_AbortDown 	14	// aborted download

#define EVNT_Rename 	15	// renamed file/dir
#define EVNT_DelFile 	16	// deleted file
#define EVNT_DelDir 	17	// deleted dir
#define EVNT_ChgDir 	18	// changed working directory
#define EVNT_MakeDir 	19	// created directory
#define EVNT_ProgUp 	20	// progress of upload
#define EVNT_ProgDown 	21	// progress of download
#define EVNT_Maintenance 22	// user switching to maintenance mode

#define EVNT_TimerTick 	23	// one second timer tick for use by DLL

#define SEVNT_None	0	// no sub-event
#define SEVNT_ErrWrite	1	// problem writing to disk
#define SEVNT_ErrRead	2	// problem reading from disk
#define SEVNT_ErrQuota	3	// insufficient disk quota
#define SEVNT_ErrTOut	4	// packet timed out
#define SEVNT_ErrAbort	5	// user aborted transfer
#define SENVT_ErrUnknown	6	// unknown error

#define SEVNT_ErrClose	7	// data connection closed unexpectedly
#define SEVNT_System	8	// switching to SYSTEM maintenance mode
#define SEVNT_Group	9	// switching to GROUP maintenance mode
#define SEVNT_Domain	10	// switching to DOMAIN maintenance mode
#define SEVNT_ReadOnly	11	// user switching to READ-ONLY maintenance mode

#define EVNT_HookDown 	100	// hook for file downloads
#define EVNT_HookUp 	101	// hook for file uploads
#define EVNT_HookAppend 	102	// hook for append file upload
#define EVNT_HookUnique 	103	// hook for unique name upload
#define EVNT_HookRename 	104	// hook for rename file/dir
#define EVNT_HookDelFile 	105	// hook for delete file
#define EVNT_HookDelDir 	106	// hook for delete dir

#define EVNT_HookMkd 	107	// hook for make directory
#define EVNT_HookSite 	108	// hook for the SITE command
#define EVNT_HookChgDir 	109	// hook for change dir command
#define EVNT_HookCommand 	110	// hook for raw FTP command
#define EVNT_HookReply 	111	// hook for raw FTP reply


struct RClientEventStr 
{
	int Event;	// event code
	int Flag;	// flag, meaning depends on event
	char User[151];	// user name
	char Aux[512];	// auxiliary area, usage depends on event
	char HostIP[16];	// server domain IP address
	unsigned long SessionID;	// unique session ID
	int DomainID;	// unique ID for the domain the client connected to
	int DomainPort;	// server domain port number the client connected to 
};

#define SRVU_LoginMesFile 	1	// get login message file
#define SRVU_HomeDir 	2	// get home dir
#define SRVU_Password 	3	// verify password
#define SRVU_IPAccess 	4	// verify IP access
#define SRVU_WriteFile 	5	// verify write access
#define SRVU_ReadFile 	6	// verify read access
#define SRVU_ModifyFile 	7	// verify mod./del. file access
#define SRVU_ExecProg 	8	// verify execute access

#define SRVU_ListDir 	9	// verify dir listing access
#define SRVU_ChangeDir 	10	// verify dir change access
#define SRVU_DeleteDir 	11	// verify dir delete access
#define SRVU_CreateDir 	12	// verify dir create access
#define SRVU_HideHidden 	13	// get setting for 'hide hidden files'
#define SRVU_RelPaths 	14	// get setting for 'relative paths'
#define SRVU_RatioType 	15	// get setting for type of ratios

#define SRVU_RatioDown 	16	// get setting for download ratio
#define SRVU_RatioUp   	17	// get setting for upload ratio
#define SRVU_RatioCredit 	18	// get/adjust ratio credit setting
#define SRVU_RatioFree 	19	// verify if file is free for ratios
#define SRVU_QuotaEnable 	20	// verify if disk quota is enabled
#define SRVU_QuotaChange 	21	// change in disk quota

#define SRVU_QuotaMax 	22	// maximum disk quota
#define SRVU_AlwaysLogin 	23	// always allow login
#define SRVU_OneLoginPerIP 	24	// allow one login per user/IP pair
#define SRVU_LogClientIP 	25	// log client from this IP address
#define SRVU_SpeedLimit 	26	// maximum transfer speed
#define SRVU_PassChange 	27	// change user's password
#define SRVU_TimeOut 	28	// get user time-out value

#define SRVU_MaxUsers 	29	// max. no. of users for account
#define SRVU_PassChallenge 	30	// get password challenge if needed
#define SRVU_Connect 	31	// information only: client connected
#define SRVU_Close 	32	// information only: client disconnected
#define SRVU_MaxLoginPerIP 	33	// max. no. of logins from same IP for user
#define SRVU_VerifyPasswd 	34	// verify old password before changing it

#define SRVU_AppendFile 	35	// verify append file access
#define SRVU_SignOnMes 	36	// get signon message file
#define SRVU_SignOffMes 	37	// get signoff message file

#define SRVU_Maintenance 	38	// switch to maintenance mode
#define SRVU_SessionTimeOut 	39	// session time-out
#define SRVU_SecureOnly 	40	// only allow login over secure connection
#define SRVU_PassExpired 	41	// verify if password is expired


extern "C" WORD CALLBACK HandleEventHook(RFTPEventStr* pEventStruc);
extern "C" int  __cdecl HandleClientEvent(RClientEventStr* pClientEvent);





