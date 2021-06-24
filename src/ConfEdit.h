#if !defined(AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_)
#define AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfEdit.h : ヘッダー ファイル
//

#include"QROQSDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CConfEdit ダイアログ

class CConfEdit : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// コンストラクション
public:
	CConfEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConfEdit)
	enum { IDD = IDD_CONF_EDIT };
	UINT	m_HatchSize;
	int		m_UndoSize;
	BOOL	m_ArcInMove;
	BOOL	m_ArcOutMove;
	BOOL	m_AlwaysAlign;
	int		m_GridType;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_)

