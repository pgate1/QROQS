#if !defined(AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_)
#define AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfCompile.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CConfCompile ダイアログ

#include"QROQSDoc.h"


class CConfCompile : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// コンストラクション
public:
	CConfCompile(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConfCompile)
	enum { IDD = IDD_CONF_COMPILE };
	BOOL	m_CompSave;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfCompile)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_)

