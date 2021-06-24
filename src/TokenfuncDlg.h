#if !defined(AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_)
#define AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TokenfuncDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg ダイアログ

class CTokenfuncDlg : public CDialog
{
// コンストラクション
public:
	CTokenfuncDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CTokenfuncDlg)
	enum { IDD = IDD_TOKENFUNC };
	CString	m_str;
	CString	m_input;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTokenfuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTokenfuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_)

/// EOF ///

