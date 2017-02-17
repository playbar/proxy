#ifndef  _COMPRESS_CONFIG_H
#define  _COMPRESS_CONFIG_H


#define DEFINE_NAME(s)     TRACE(#s)

# if defined(DEBUG) || defined(_DEBUG) || defined(DBG)
   #	define	PROTECT(s)		{	static	TCHAR *__FUNC_NAME__	= TEXT(#s);
   #	define	UNPROTECT		}
# else
   #	define	PROTECT(s)	{			   \
								try			\
								{			\
									static	TCHAR *__FUNC_NAME__= TEXT(#s);	
								
   #	define	UNPROTECT       }			\
								catch(...)	\
								{			\
								}			\
							}
# endif



# ifndef SAFE_DELETE
#		define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
# endif    
# ifndef SAFE_DELETE_ARRAY
#		define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
# endif    
# ifndef SAFE_RELEASE
#		define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
# endif

#	define	appZeroMemory		::ZeroMemory
#	define	appCopyMemory		::CopyMemory
#	define	appStrStr			::_tcsstr
#	define	appStrChr			::_tcschr
#	define	appStrrChr			::_tcsrchr
#	define	appStrlen			::_tcslen
#	define	appStrCmp			::_tcscmp
#	define	appStrICmp			::_tcsicmp
#	define	appStrNCmp			::_tcsncmp
#	define	appStrNICmp			::_tcsnicmp
#	define	appStrCat			::_tcscat


#	define	appStrCopy			::_tcscpy
#	define	appPrintF			::_stprintf
#	define	appPrintFN			::_sntprintf
#	define	appFullPath			::_tfullpath
#	define	appSplitPath		::_tsplitpath
#	define	appAccess			::_taccess
#	define	appMakeDir			::_tmkdir

#	define	appMoveMemory		::memmove
#	define	appCompMemory		::memcmp
#	define	appCmpIMemory		::memicmp
#	define	appSetMemory		::memset

#	define	BRK						_T('\\')	//	Ð±¸Ü×Ö·û
#	define	ABRK					_T('/')		//	·´Ð±¸Ü×Ö·û
#	define	ALLFILES				_T("*.*")



#endif   //_COMPRESS_CONFIG_H