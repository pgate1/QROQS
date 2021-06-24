// QROQS.h : QROQS アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_QROQS_H__45D5814E_71C7_4589_8238_33A1463AD463__INCLUDED_)
#define AFX_QROQS_H__45D5814E_71C7_4589_8238_33A1463AD463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル



#ifdef  GLOBAL_QROQS_VALUE_DEFINE
#define GLOBAL
#else
#define GLOBAL extern
#endif
GLOBAL BOOL Qmode;
GLOBAL int PetMode;

//GLOBAL FILE *tfp;

#undef GLOBAL



static WORD ToolId[] = { IDR_TOOLBAR_SUB,
                        16, 16,

                        ID_FILE_PRINT,
                        ID_EDIT_CUT,
                        ID_EDIT_COPY,
                        ID_EDIT_PASTE,
						ID_FILE_PRINT_PREVIEW,
						ID_EDIT_FIND,
						ID_MOD_FUNCTION,
						ID_COM_OBJLAP,
						ID_BSP_PROPATY,
						ID_APP_ABOUT,
						ID_TRA_FUNCTION,
						ID_UHZI_FUNCTION,
						ID_KEM_FUNCTION,
						ID_EDIT_DELETE,
						ID_COM_ALIGN,
						ID_EDIT_UNDO,
						ID_FILE_OUT_VERILOG,
						ID_FILE_OUT_C,
						ID_COM_CONFIG,
						ID_COM_CLEAR,
						ID_COM_ZOOMRECT,
						ID_FILE_SAVE_AS,
						ID_COM_PLACE_FUSION,
						ID_COM_TEXT,
						ID_COM_PLACE_FISSION,
						ID_COM_MEMO,
                        NULL};

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp:
// このクラスの動作の定義に関しては QROQS.cpp ファイルを参照してください。
//

class CQROQSApp : public CWinApp
{
public:
	CQROQSApp();

	CString exe_dir;

	UINT style_mode;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CQROQSApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CQROQSApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_QROQS_H__45D5814E_71C7_4589_8238_33A1463AD463__INCLUDED_)

/// EOF ///

