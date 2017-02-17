; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CColorProcessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "colorprocess.h"
LastPage=0

ClassCount=13
Class1=CAdjustColorDlg
Class2=CBCDlg
Class3=CChildFrame
Class4=CColorBitsDlg
Class5=CColorProcessApp
Class6=CAboutDlg
Class7=CColorProcessDoc
Class8=CColorProcessView
Class9=CGrayDlg
Class10=CMainFrame

ResourceCount=9
Resource1=IDR_COLORPTYPE (Chinese (P.R.C.))
Resource2=IDD_COLORBITS (Chinese (P.R.C.))
Resource3=IDD_GRAY (Chinese (P.R.C.))
Resource4=IDR_MAINFRAME (Chinese (P.R.C.))
Resource5=IDD_COLOR (Chinese (P.R.C.))
Resource6=IDD_GAMMA (Chinese (P.R.C.))
Resource7=IDD_ABOUTBOX (Chinese (P.R.C.))
Class11=CGammaCorrection
Resource8=IDD_BC (Chinese (P.R.C.))
Class12=CExposure
Class13=CExposureDlg
Resource9=IDD_EXPOSURE (Chinese (P.R.C.))

[CLS:CAdjustColorDlg]
Type=0
BaseClass=CDialog
HeaderFile=AdjustColorDlg.h
ImplementationFile=AdjustColorDlg.cpp

[CLS:CBCDlg]
Type=0
BaseClass=CDialog
HeaderFile=BCDlg.h
ImplementationFile=BCDlg.cpp

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CColorBitsDlg]
Type=0
BaseClass=CDialog
HeaderFile=ColorBitsDlg.h
ImplementationFile=ColorBitsDlg.cpp

[CLS:CColorProcessApp]
Type=0
BaseClass=CWinApp
HeaderFile=ColorProcess.h
ImplementationFile=ColorProcess.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ColorProcess.cpp
ImplementationFile=ColorProcess.cpp
LastObject=CAboutDlg

[CLS:CColorProcessDoc]
Type=0
BaseClass=CDocument
HeaderFile=ColorProcessDoc.h
ImplementationFile=ColorProcessDoc.cpp

[CLS:CColorProcessView]
Type=0
BaseClass=CScrollView
HeaderFile=ColorProcessView.h
ImplementationFile=ColorProcessView.cpp
Filter=C
VirtualFilter=VWC
LastObject=IDM_EXPOSURE

[CLS:CGrayDlg]
Type=0
BaseClass=CDialog
HeaderFile=GrayDlg.h
ImplementationFile=GrayDlg.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_COLOR]
Type=1
Class=CAdjustColorDlg

[DLG:IDD_BC]
Type=1
Class=CBCDlg

[DLG:IDD_COLORBITS]
Type=1
Class=CColorBitsDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_GRAY]
Type=1
Class=CGrayDlg

[MNU:IDR_COLORPTYPE (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=IDM_COLORBITS
Command16=IDM_GRAY
Command17=IDM_ADJUSTCOLOR
Command18=IDM_BC
Command19=IDM_GETRED
Command20=IDM_GETGREEN
Command21=IDM_GETBLUE
Command22=IDM_RED
Command23=IDM_GREEN
Command24=IDM_BLUE
Command25=IDM_CORRECTGAMMA
Command26=IDM_NEGATIVE
Command27=IDM_EXPOSURE
Command28=ID_VIEW_TOOLBAR
Command29=ID_VIEW_STATUS_BAR
Command30=ID_WINDOW_NEW
Command31=ID_WINDOW_CASCADE
Command32=ID_WINDOW_TILE_HORZ
Command33=ID_WINDOW_ARRANGE
Command34=ID_APP_ABOUT
CommandCount=34

[DLG:IDD_COLOR (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=18
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO1,button,1342177289
Control3=IDC_RADIO4,button,1342177289
Control4=IDC_RADIO2,button,1342177289
Control5=IDC_RADIO3,button,1342177289
Control6=IDC_SCROLLBAR1,scrollbar,1342177284
Control7=IDC_SCROLLBAR2,scrollbar,1342177284
Control8=IDC_SCROLLBAR3,scrollbar,1342177284
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816
Control11=IDC_NAME1,static,1342308352
Control12=IDC_NAME2,static,1342308352
Control13=IDC_NAME3,static,1342308352
Control14=IDC_VALUE1,static,1342308352
Control15=IDC_VALUE2,static,1342308352
Control16=IDC_VALUE3,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308354

[DLG:IDD_GAMMA (Chinese (P.R.C.))]
Type=1
Class=CGammaCorrection
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BRIGHT,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308354
Control7=IDC_SLIDER1,msctls_trackbar32,1342242840

[DLG:IDD_GRAY (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_RADIO1,button,1342177289
Control5=IDC_RADIO2,button,1342177289
Control6=IDC_RADIO3,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT1,edit,1350631552
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT2,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT3,edit,1350631552
Control13=IDC_STATIC,static,1342308352

[DLG:IDD_BC (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_SCROLLBAR1,scrollbar,1342177280
Control5=IDC_BRIGHT,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_SCROLLBAR2,scrollbar,1342177280
Control8=IDC_CONTRAST,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308354

[TB:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COLORBITS (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_COLORBITS,button,1342308359
Control2=IDC_RADIO24,button,1342177289
Control3=IDC_RADIO8,button,1342177289
Control4=IDC_RADIO4,button,1342177289
Control5=IDC_RADIO1,button,1342177289
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[CLS:CGammaCorrection]
Type=0
HeaderFile=GammaCorrection.h
ImplementationFile=GammaCorrection.cpp
BaseClass=CDialog
Filter=D
LastObject=CGammaCorrection
VirtualFilter=dWC

[CLS:CExposure]
Type=0
HeaderFile=Exposure.h
ImplementationFile=Exposure.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CExposure

[CLS:CExposureDlg]
Type=0
HeaderFile=ExposureDlg.h
ImplementationFile=ExposureDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_EXPOSURE (Chinese (P.R.C.))]
Type=1
Class=CExposureDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

