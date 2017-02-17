#if !defined( MACRO_H )
#define MACRO_H

//########################################################################
#define STRNCPY(to, head, len)	strncpy(to, head, len); *(to+len) = 0;
#define VALID_TCF(tcf)			(tcf && tcf->m_pView)

//########################################################################
//c:\\temp\abc.txt
#define GET_FILE(str)	str.Mid(str.ReverseFind('\\')+1)	//abc.txt 
#define GET_ROOT(str)		str.Left(str.ReverseFind('\\')+1)	//c:\\temp\ 
#define GET_TITLE(str)		str.Mid(str.ReverseFind('\\')+1, str.ReverseFind('.')-str.ReverseFind('\\')-1)	//abc

//########################################################################
#define countof(x)			(sizeof(x)/sizeof(x[0]))
#define FREENULL(x);		if (x) { free(x); x=NULL; }
#define SYSFREESTRING(x);	if (x) { SysFreeString(x); x=NULL; }
#define RELEASE(x);			if (x) { x->Release(); x=NULL; }

//########################################################################
#define STR_ERR_PATH_CHAR		"\\/:*?\"<>|\r\n"
#define REPLACE_ERR_PATH_CHAR(title)	\
		if (title.FindOneOf(STR_ERR_PATH_CHAR)>=0)	\
		{	\
			title.Replace('\\', '_'); title.Replace('/', '_'); \
			title.Replace(':', '_');  title.Replace('*', '_'); \
			title.Replace('?', '_');  title.Replace('\"', '_'); \
			title.Replace('<', '_');  title.Replace('>', '_'); \
			title.Replace('|', '_');  title.Replace('\r', '_'); \
			title.Replace('\n', '_');	\
		}

// http://qwe/123.jpg => 123.jpg
#define GET_URL_NAME(str)	\
		if (IS_URL(str))	\
		{	\
			int pos;	\
			if ( (pos=str.ReverseFind('/')) >0 || (pos=str.ReverseFind('\\')) >0 )	\
			{	\
				if (pos < str.GetLength()-1)	\
					str = str.Mid(pos+1);	\
			}	\
		}

// the flag\r\n the content => the flag
#define GET_FILE_NAME(strFileName,str)	\
		int pos = str.FindOneOf(STR_ERR_PATH_CHAR);	\
		if (pos>0)	\
			strFileName = str.Left(pos);	\
		else	\
		{	\
			strFileName = str;	\
			if (pos==0)	\
			{ REPLACE_ERR_PATH_CHAR(strFileName);}	\
		}

#define GET_TAB_TITLE(tcf,strName);	\
		if(tcf->m_strStickName.GetLength()>0)	\
			strName = tcf->m_strStickName;	\
		else	\
			strName = tcf->m_strTitle;

//get insert pos
#define LIST_GET_INSERT_POS(list, p);	\
		POSITION pos = list.GetFirstSelectedItemPosition();	\
		if(pos!=NULL)	\
			*p = list.GetNextSelectedItem(pos)+1;	\
		else	\
			*p= list.GetItemCount();

//focus next after del
#define LIST_FOCUS_POS(list, i);	\
		list.EnsureVisible(i, FALSE);	\
		list.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);	\
		list.SetFocus();

#define GET_LIST_SEL(list,nCurSel)	\
		POSITION pos = list.GetFirstSelectedItemPosition();	\
		if(pos!=NULL)	\
			nCurSel = m_url.GetNextSelectedItem(pos);	\
		else	\
			nCurSel = -1;

//########################################################################
//sub menu pos
#define SMP_NEW			pmf->GetMenu()->GetSubMenu(0)->GetSubMenu(0)
#define SMP_SAVE		pmf->GetMenu()->GetSubMenu(0)->GetSubMenu(7)
#define SMP_CLOSE		pmf->GetMenu()->GetSubMenu(0)->GetSubMenu(11)

#define SMP_FONTS		pmf->GetMenu()->GetSubMenu(2)->GetSubMenu(4)
#define SMP_ENCODE		pmf->GetMenu()->GetSubMenu(2)->GetSubMenu(5)
#define SMP_UNDO		pmf->GetMenu()->GetSubMenu(2)->GetSubMenu(9)

#define SMP_FAV			pmf->GetMenu()->GetSubMenu(3)

#define SMP_GROUP		pmf->GetMenu()->GetSubMenu(4)

#define SMP_AUTOSCROLL	pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(0)
#define SMP_AUTOFILL	pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(2)
#define SMP_BGCOLOR		pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(3)
#define SMP_DOWNLOAD	pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(4)

#define SMP_SKIN		pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(6)
#define SMP_CLEAN		pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(7)
#define SMP_EXTOOLS		pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(8)

#define SMP_SEARCH		pmf->GetMenu()->GetSubMenu(5)->GetSubMenu(15)

//sub menu handle
#define SMH_FAV			SMP_FAV->m_hMenu

//########################################################################
#define IS_FTP(str)			(str.Left(6)=="ftp://" || str.Left(4)=="ftp.")
#define IS_DISK_STR(str)	( str.GetAt(1)==':' && (str.GetAt(2)=='\\' || str.GetAt(2)=='/' || str.GetAt(2)==NULL) ) //c: 

#define IS_URL(str)			(	str.Left(7)=="http://" || \
								str.Left(7)=="file://" || \
								str.Left(8)=="https://" || \
								str.Left(7)=="file://" || \
								str.Left(4)=="www." || \
								IS_FTP(str) || \
								IS_RES_URL(str) || \
								str=="about:blank" || \
								IS_DISK_STR(str) \
							)

#define IS_SCRIPT(str)				(str.Left(11)=="javascript:" || str.Left(9)=="vbscript:")
#define IS_MAILTO(str)				(str.Left(7)=="mailto:") // || (str.Find('@')>0 && str.Find('.')>2 && !IS_URL(str)))	//mailto:a@b.com a@b.com
#define IS_SCRIPT_MAILTO(str)		( IS_SCRIPT(str) || IS_MAILTO(str) )
#define IS_SCRIPT_STR(str)			(strstr(str,"javascript:") || strstr(str,"vbscript:"))
#define IS_MAILTO_STR(str)			(strstr(str,"mailto:"))
#define IS_SCRIPT_MAILTO_STR(str)	(IS_SCRIPT_STR(str) || IS_MAILTO_STR(str))
#define IS_MAIL_STR(str)			(str.Find('@')>0 && !IS_URL(str) )

//the string should not be search, c:, \\server ror.cn
// not navigate a@b.com
// http://china.qianlong.com/4352/2004/08/18/45@2225131.htm
#define NOT_SEARCH_STR(str)	\
		(	\
			(str.Find('.')>0 || str.Find('/')>0 || str.Find('\\')>=0 || str.Left(2)=="\\\\" || IS_DISK_STR(str) )	\
			&&	\
			( !IS_MAIL_STR(str) )	\
		)

// ex: \\app\app1 
//str.Right(1)=='.' means c:\qwe\.  
//shell:ControlPanelFolder
#define IS_LOCAL_FOLDER(str)	\
		(	\
			( str.ReverseFind('.')<=str.ReverseFind('\\') || str.ReverseFind('.')<=str.ReverseFind('/') || str.Right(1)=='.') &&	\
			( str.Left(5)=="file:" || str.Left(2)=="\\\\" || str.Left(6)=="shell:" || IS_DISK_STR(str))		\
		)

//if have if before, must use { }
#define CLOSE_PAGE(tcf);	\
		if(tcf && !tcf->m_bLock && !tcf->m_pView->m_bToClose)	\
		{	\
			tcf->m_pView->m_bToClose = TRUE;	\
			tcf->PostMessage(WM_CLOSE);	\
		}

//########################################################################
#define LOADSTR(str,id);	\
	theApp.m_bUseLngFile ? _LoadString(str,id) : str.LoadString(id)

#define LOADDLG(strDomain);	\
	if (theApp.m_bUseLngFile)	\
		_LoadDlgString(this,strDomain);

#define LOADMENU(pMenu,strDomain);	\
	if (theApp.m_bUseLngFile)	\
		_LoadMenuString(pMenu,strDomain);

//########################################################################
#define IS_RELATIVE_URL(str)		(str.Left(2)==".\\")

#define DO_RELATIVE_URL(str);	\
	if ( IS_RELATIVE_URL(str) )	\
	{	\
		str = str.Mid(2);	\
		str = theApp.m_strRoot + str;	\
	}

//########################################################################
#define IS_RES_URL(str)			(str.Left(6)=="res://")

#define DO_RES_URL(str)	\
	int pos = str.Find('#');	\
	if (pos>0)	\
	{	\
		str = str.Right(str.GetLength()-pos-1); \
		if (str.Left(5)=="file:")	\
		{	\
			str.Replace('\\', '/');	/*#file://H:\My.htm*/ \
			if (str.Find("file:///") == -1)	\
				str.Replace("file://", "file:///");	\
		}	\
	}

#define IS_FAVURL(strUrl)	(strUrl.Right(4).CompareNoCase(".url") == 0)

//press the key
#define PRESS(k)			(GetKeyState(k)<0)
#define PRESS_CTRL			(GetKeyState(VK_CONTROL)<0)
#define PRESS_SHIFT			(GetKeyState(VK_SHIFT)<0)

//####################################################
#define SAVEINT(from,to,section,entry);	\
	if (to != from )	\
	{	\
		to = from;	\
		theApp.WriteProfileInt(section, entry, to);	\
	}

#define SAVESTR(from,to,section,entry);	\
	if (to != from )	\
	{	\
		to = from;	\
		theApp.WriteProfileString(section, entry, to);	\
	}

//########################################################################
#endif