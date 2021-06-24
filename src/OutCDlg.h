#if !defined(AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_)
#define AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutCDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COutCDlg ダイアログ


#include"QROQSDoc.h"

class COutCDlg : public CDialog
{
// コンストラクション
public:
	COutCDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CQROQSDoc *m_pDoc;

// ダイアログ データ
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;

	//{{AFX_DATA(COutCDlg)
	enum { IDD = IDD_OUT_C };
	CComboBox	m_OutDir;
	CString	m_ModuleName;
	int		m_KemMax;
	BOOL	m_MainOut;
	BOOL	m_QROQSOut;
	BOOL	m_MakeOut;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COutCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COutCDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_)
