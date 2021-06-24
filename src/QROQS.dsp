# Microsoft Developer Studio Project File - Name="QROQS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=QROQS - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "QROQS.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "QROQS.mak" CFG="QROQS - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "QROQS - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "QROQS - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "QROQS - Win32 Light" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QROQS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /WX /Gi /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fr /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386
# SUBTRACT LINK32 /verbose /map

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "QROQS___Win32_Light"
# PROP BASE Intermediate_Dir "QROQS___Win32_Light"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Light"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /WX
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_LIGHT" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "QROQS - Win32 Release"
# Name "QROQS - Win32 Debug"
# Name "QROQS - Win32 Light"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "BSPetri"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BSPetri.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriFile.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriObj.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriOutC.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BSPetriSim.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriTree.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriTreeSim.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPetriVerilog.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Compile.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

# ADD CPP /W3
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\v2c.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Tools\ButtonXP.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Draw.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Popup.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\StatusBarXP.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\ToolBarXP.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Tools.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "NewMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewMenu\NewMenu.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfCompile.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfGene.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfSim.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FindObjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KemfuncDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleTreeDlgbar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeCtrl.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutCDlg.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutVerilogDlg.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlacefuncDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenfuncDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TrafuncDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UhzifuncDlg.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "QROQS - Win32 Release"

# ADD CPP /W3
# SUBTRACT CPP /WX

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# ADD BASE CPP /W3
# SUBTRACT BASE CPP /WX
# ADD CPP /W3
# SUBTRACT CPP /WX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\other.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQS.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQS.rc
# End Source File
# Begin Source File

SOURCE=.\QROQSDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSDocFile.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSView.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewExport.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewInst.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewKey.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewModule.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewObj.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewOther.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\QROQSViewSim.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "BSPetri Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BSPetri.h
# End Source File
# Begin Source File

SOURCE=.\BSPetriObj.h
# End Source File
# Begin Source File

SOURCE=.\BSPetriTree.h
# End Source File
# Begin Source File

SOURCE=.\Compile.h
# End Source File
# Begin Source File

SOURCE=.\v2c.h
# End Source File
# End Group
# Begin Group "Tools Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Tools\ButtonXP.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Draw.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Popup.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\StatusBarXP.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\ToolbarXP.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools\Tools.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "NewMenu Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewMenu\NewMenu.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Config Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfCompile.h
# End Source File
# Begin Source File

SOURCE=.\ConfEdit.h
# End Source File
# Begin Source File

SOURCE=.\ConfGene.h
# End Source File
# Begin Source File

SOURCE=.\ConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConfSim.h
# End Source File
# End Group
# Begin Group "Dialogs Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FindObjDlg.h
# End Source File
# Begin Source File

SOURCE=.\KemfuncDlg.h
# End Source File
# Begin Source File

SOURCE=.\MemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModuleManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModuleTreeDlgbar.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeCtrl.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutCDlg.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OutVerilogDlg.h

!IF  "$(CFG)" == "QROQS - Win32 Release"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Debug"

!ELSEIF  "$(CFG)" == "QROQS - Win32 Light"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PlaceFuncDlg.h
# End Source File
# Begin Source File

SOURCE=.\TokenfuncDlg.h
# End Source File
# Begin Source File

SOURCE=.\TraceDlg.h
# End Source File
# Begin Source File

SOURCE=.\TraceEdit.h
# End Source File
# Begin Source File

SOURCE=.\TrafuncDlg.h
# End Source File
# Begin Source File

SOURCE=.\UhzifuncDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\other.h
# End Source File
# Begin Source File

SOURCE=.\QROQS.h
# End Source File
# Begin Source File

SOURCE=.\QROQSDoc.h
# End Source File
# Begin Source File

SOURCE=.\QROQSView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\RES\cur_hand.cur
# End Source File
# Begin Source File

SOURCE=.\Res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\object.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\object1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\pointer.cur
# End Source File
# Begin Source File

SOURCE=.\res\QROQS.ico
# End Source File
# Begin Source File

SOURCE=.\res\QROQS.rc2
# End Source File
# Begin Source File

SOURCE=.\res\QROQSDoc.ico
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
