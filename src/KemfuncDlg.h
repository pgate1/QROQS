#if !defined(AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_)
#define AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KemfuncDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CKemfuncDlg ダイアログ

#include"BSPetriTree.h"

class CKemfuncDlg : public CDialog
{

public:
	BSPetriTree *m_bspt;
	string m_mname;
	int m_id;



// コンストラクション
public:
	CKemfuncDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CKemfuncDlg)
	enum { IDD = IDD_KEMFUNC };
	CSpinButtonCtrl	m_SpinBits;
	int		m_Getline;
	CString	m_compData;
	CString	m_kOutputData;
	CString	m_PassStatus;
	CString	m_PassStatusAll;
	CString	m_OutputName;
	BOOL	m_Common;
	int		m_Bits;
	BOOL	m_Hex;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CKemfuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKemfuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRedraw();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_)

/// EOF ///

