// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_)
#define AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。


//#include <afxctl.h> //追加?
#define _WIN32_WINNT 0x0400	//TrackMouseEvent()用
//#define _WIN32_WINNT 0x0500	//SetLayeredWindowAttributes()用

#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#include <afxtempl.h>

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
//#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
//#include<afxpriv.h>


#include <algorithm>
#include<time.h>
#include<math.h>

#include<string>
#include<vector>
#include<map>
#include<list>
#include<stack>
#include<set>
#include<deque>

#include <process.h>    /* _beginthread, _endthread */

using namespace std;

#ifndef _LIGHT
#include"Tools/Tools.h"
#include"Tools/StatusBarXP.h"
#include"Tools/ToolBarXP.h"
#include"Tools/Popup.h"
#include"Tools/ButtonXP.h"

//#define USE_NEW_DOCK_BAR
#include"NewMenu/NewMenu.h"
//#include"NewMenu/NewToolBar.h"
#endif

#endif // _AFX_NO_AFXCMN_SUPPORT



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_)

/// EOF ///

