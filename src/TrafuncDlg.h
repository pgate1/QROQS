#if !defined(AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_)
#define AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrafuncDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTrafuncDlg ダイアログ
#include"BSPetriTree.h"

class CTrafuncDlg : public CDialog
{

public:
	BSPetriTree *m_bspt;
	string m_mname;
	int m_id;

	BSPetri *m_bsp;//Simの時のみ


// コンストラクション
public:
	CTrafuncDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ



// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
	CButtonXP m_btnTrans;
	CButtonXP m_btnCheck;
#endif
	//{{AFX_DATA(CTrafuncDlg)
	enum { IDD = IDD_TRAFUNC };
	CString	m_Trafunc;
	CString	m_AbletermAcnt;
	BOOL	m_FireableStop;
	BOOL	m_Common;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTrafuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTrafuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_)

/// EOF ///

