#if !defined(AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_)
#define AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfSim.h : ヘッダー ファイル
//

#include"QROQSDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CConfSim ダイアログ

class CConfSim : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// コンストラクション
public:
	CConfSim(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConfSim)
	enum { IDD = IDD_CONF_SIM };
	UINT	m_SimSpeed;
	BOOL	m_TokenMove;
	BOOL	m_SimAnime;
	int		m_SimundoSize;
	int		m_ViewDataSize;
	BOOL	m_BinValue;
	BOOL	m_TokenTrans;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfSim)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfSim)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnFastsim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_)

