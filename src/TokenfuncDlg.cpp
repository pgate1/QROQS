// TokenfuncDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "QROQS.h"
#include "TokenfuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg ダイアログ

CTokenfuncDlg::CTokenfuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTokenfuncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTokenfuncDlg)
	m_str = _T("");
	m_input = _T("");
	//}}AFX_DATA_INIT
}

void CTokenfuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTokenfuncDlg)
	DDX_Text(pDX, IDC_EDIT_STR, m_str);
	DDV_MaxChars(pDX, m_str, 100);
	DDX_Text(pDX, IDC_TOKENVAL, m_input);
	DDV_MaxChars(pDX, m_input, 100);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CTokenfuncDlg, CDialog)
	//{{AFX_MSG_MAP(CTokenfuncDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg メッセージ ハンドラ


BOOL CTokenfuncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	UpdateData(false);

	CEdit *myED=(CEdit *)GetDlgItem(IDC_TOKENVAL);
	myED->SetFocus();
    myED->SetSel(0, -1);
	// フォーカスを移動させ、すぐに入力できるようにする
	return false;
  

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CTokenfuncDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(true);

	CDialog::OnOK();
}


/// EOF ///
