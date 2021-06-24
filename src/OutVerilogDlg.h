#if !defined(AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_)
#define AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutVerilogDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COutVerilogDlg ダイアログ


#include"QROQSDoc.h"

class COutVerilogDlg : public CDialog
{
// コンストラクション
public:
	COutVerilogDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CQROQSDoc *m_pDoc;

// ダイアログ データ
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
	//{{AFX_DATA(COutVerilogDlg)
	enum { IDD = IDD_OUT_VERILOG };
	CComboBox	m_OutDir;
	CString	m_ModuleName;
	double	m_OverPlus;
	BOOL	m_VSync;
	BOOL	m_VDebug;
	CString	m_OptModule;
	CString	m_OptTrafunc;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COutVerilogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COutVerilogDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_)
