; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageProcessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imageprocess.h"
LastPage=0

ClassCount=8
Class1=CChildFrame
Class2=CConvoluteDlg
Class3=CImageProcessApp
Class4=CAboutDlg
Class5=CImageProcessDoc
Class6=CImageProcessView
Class7=CMainFrame

ResourceCount=6
Resource1=IDD_EMBOSSMENT (Chinese (P.R.C.))
Resource2=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource3=IDR_MAINFRAME (Chinese (P.R.C.))
Resource4=IDR_IMAGEPTYPE (Chinese (P.R.C.))
Class8=CSpreadDlg
Resource5=IDD_CONVOLUTE (Chinese (P.R.C.))
Resource6=IDD_SPREAD (Chinese (P.R.C.))

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CConvoluteDlg]
Type=0
BaseClass=CDialog
HeaderFile=ConvoluteDlg.h
ImplementationFile=ConvoluteDlg.cpp

[CLS:CImageProcessApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImageProcess.h
ImplementationFile=ImageProcess.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageProcess.cpp
ImplementationFile=ImageProcess.cpp
LastObject=IDM_EMBOSSMENT

[CLS:CImageProcessDoc]
Type=0
BaseClass=CDocument
HeaderFile=ImageProcessDoc.h
ImplementationFile=ImageProcessDoc.cpp

[CLS:CImageProcessView]
Type=0
BaseClass=CScrollView
HeaderFile=ImageProcessView.h
ImplementationFile=ImageProcessView.cpp
Filter=C
VirtualFilter=VWC
LastObject=IDM_CANNYED

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_CONVOLUTE]
Type=1
Class=CConvoluteDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[MNU:IDR_IMAGEPTYPE (Chinese (P.R.C.))]
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
Command15=IDM_LF1
Command16=IDM_LF2
Command17=IDM_LF3
Command18=IDM_HF1
Command19=IDM_HF2
Command20=IDM_HF3
Command21=IDM_VERT
Command22=IDM_HORZ
Command23=IDM_VERTHORZ
Command24=IDM_NORTH
Command25=IDM_NORTHEAST
Command26=IDM_EAST
Command27=IDM_SOUTHEAST
Command28=IDM_SOUTH
Command29=IDM_SOUTHWEST
Command30=IDM_WEST
Command31=IDM_NORTHWEST
Command32=IDM_LAP1
Command33=IDM_LAP2
Command34=IDM_LAP3
Command35=IDM_LAP4
Command36=IDM_SOBEL
Command37=IDM_HOUGH
Command38=IDM_CANNYED
Command39=IDM_HOMOEDGEDETECT
Command40=IDM_DIFFERENTEDGEDETECT
Command41=IDM_MEDIANFILTER
Command42=IDM_EMBOSSMENT
Command43=IDM_SPREAD
Command44=IDM_FFT
Command45=IDM_DCT
Command46=IDM_WALH
Command47=IDM_EROSION_H
Command48=IDM_EROSION_V
Command49=IDM_DILATION_H
Command50=IDM_DILATION_V
Command51=IDM_OPEN_H
Command52=IDM_OPEN_V
Command53=IDM_CLOSE_H
Command54=IDM_CLOSE_V
Command55=IDM_CONTOUR_H
Command56=IDM_CONTOUR_V
Command57=IDM_THIN
Command58=ID_VIEW_TOOLBAR
Command59=ID_VIEW_STATUS_BAR
Command60=ID_WINDOW_NEW
Command61=ID_WINDOW_CASCADE
Command62=ID_WINDOW_TILE_HORZ
Command63=ID_WINDOW_ARRANGE
Command64=ID_APP_ABOUT
CommandCount=64

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

[DLG:IDD_CONVOLUTE (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SLIDER1,msctls_trackbar32,1342242836
Control4=IDC_VALUE,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_EMBOSSMENT (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SLIDER1,msctls_trackbar32,1342242836
Control4=IDC_VALUE,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_SPREAD (Chinese (P.R.C.))]
Type=1
Class=CSpreadDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SLIDER1,msctls_trackbar32,1342242836
Control4=IDC_VALUE,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CSpreadDlg]
Type=0
HeaderFile=SpreadDlg.h
ImplementationFile=SpreadDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDM_HOMOEDGEDETECT

