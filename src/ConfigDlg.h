#if !defined(AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_)
#define AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : ヘッダー ファイル
//

#include"QROQSDoc.h"

#include"ConfGene.h"
#include"ConfEdit.h"
#include"ConfCompile.h"
#include"ConfSim.h"


/////////////////////////////////////////////////////////////////////////////
// CConfigDlg ダイアログ

class CConfigDlg : public CDialog
{

	CConfGene m_ConfGene;
	CConfEdit m_ConfEdit;
	CConfCompile m_ConfCompile;
	CConfSim m_ConfSim;

public:
	//追加
	CQROQSDoc *m_pDoc;

// コンストラクション
public:
	CConfigDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIGDLG };
	CTabCtrl	m_Tab;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	//追加
	int CheckDlgData();
	void showTab(int);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_)


/// EOF ///

