# Microsoft Developer Studio Project File - Name="ImageBoard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ImageBoard - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImageBoard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageBoard.mak" CFG="ImageBoard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageBoard - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ImageBoard - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageBoard - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib version.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\ImageLib\Release\ImageLib.dll Release\ImageLib.dll
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ImageBoard - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy ..\ImageLib\Debug\ImageLib.dll Debug\ImageLib.dll
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ImageBoard - Win32 Release"
# Name "ImageBoard - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdjustColorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CanvasSize.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBitsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvoluteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EmbossmentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExposureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatDibWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GammaCorrection.cpp
# End Source File
# Begin Source File

SOURCE=.\GrayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageBoard.rc
# End Source File
# Begin Source File

SOURCE=.\ImageBoardDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageBoardView.cpp
# End Source File
# Begin Source File

SOURCE=.\JpegFormatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintParambar.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\PenParamsList.cpp
# End Source File
# Begin Source File

SOURCE=.\PenWidth.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\Scale.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectPasteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\SpreadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdjustColorDlg.h
# End Source File
# Begin Source File

SOURCE=.\BCDlg.h
# End Source File
# Begin Source File

SOURCE=.\CanvasSize.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColorBitsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorGrid.h
# End Source File
# Begin Source File

SOURCE=.\ConvoluteDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dib.h
# End Source File
# Begin Source File

SOURCE=.\dibapi.h
# End Source File
# Begin Source File

SOURCE=.\EmbossmentDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExposureDlg.h
# End Source File
# Begin Source File

SOURCE=.\FloatDibWnd.h
# End Source File
# Begin Source File

SOURCE=.\GammaCorrection.h
# End Source File
# Begin Source File

SOURCE=.\Gif.h
# End Source File
# Begin Source File

SOURCE=.\GIFAPI.H
# End Source File
# Begin Source File

SOURCE=.\GrayDlg.h
# End Source File
# Begin Source File

SOURCE=.\Huffman.h
# End Source File
# Begin Source File

SOURCE=.\ImageBoard.h
# End Source File
# Begin Source File

SOURCE=.\ImageBoardDoc.h
# End Source File
# Begin Source File

SOURCE=.\ImageBoardView.h
# End Source File
# Begin Source File

SOURCE=.\Jpeg.h
# End Source File
# Begin Source File

SOURCE=.\JpegFormatDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PaintParambar.h
# End Source File
# Begin Source File

SOURCE=.\PaintToolbar.h
# End Source File
# Begin Source File

SOURCE=.\Pcx.h
# End Source File
# Begin Source File

SOURCE=.\PenParamsList.h
# End Source File
# Begin Source File

SOURCE=.\PenWidth.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateAngle.h
# End Source File
# Begin Source File

SOURCE=.\Scale.h
# End Source File
# Begin Source File

SOURCE=.\SelectPasteDlg.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\SpreadDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Tga.h
# End Source File
# Begin Source File

SOURCE=.\Tif.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\4way01.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\blank.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Cross02.cur
# End Source File
# Begin Source File

SOURCE=.\res\Cross03.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursorfr.cur
# End Source File
# Begin Source File

SOURCE=.\res\font.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageBoard.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImageBoard.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ImageBoardDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pencil.cur
# End Source File
# Begin Source File

SOURCE=.\res\pick.cur
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ImageBoard : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:108
# 	2:21:SplashScreenInsertKey:4.0
# End Section
