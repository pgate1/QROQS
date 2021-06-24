// OutCDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "OutCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutCDlg ダイアログ


COutCDlg::COutCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutCDlg)
	m_ModuleName = _T("");
	m_KemMax = 0;
	m_MainOut = FALSE;
	m_QROQSOut = FALSE;
	m_MakeOut = FALSE;
	//}}AFX_DATA_INIT
}


void COutCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutCDlg)
	DDX_Control(pDX, IDC_COMBO1, m_OutDir);
	DDX_Text(pDX, IDC_EDIT_MODULE_NAME, m_ModuleName);
	DDV_MaxChars(pDX, m_ModuleName, 100);
	DDX_Text(pDX, IDC_KEM_MAX, m_KemMax);
	DDV_MinMaxInt(pDX, m_KemMax, 0, 1000000);
	DDX_Check(pDX, IDC_CHECK_MAIN, m_MainOut);
	DDX_Check(pDX, IDC_CHECK_Q, m_QROQSOut);
	DDX_Check(pDX, IDC_CHECK_MAKEFILE, m_MakeOut);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(COutCDlg, CDialog)
	//{{AFX_MSG_MAP(COutCDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutCDlg メッセージ ハンドラ

#include "other.h"

BOOL COutCDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	m_OutDir.ResetContent();
	list<CString>::iterator lit;
	lit=m_pDoc->c_out_dir_list.begin();
	for(;lit!=m_pDoc->c_out_dir_list.end();++lit){
		m_OutDir.InsertString(-1, *lit);
	}

	CString ss;
	m_OutDir.GetLBText(0,ss);
	m_OutDir.SetWindowText(ss);

//	m_ModuleName = m_pDoc->top_bsp.module_name.c_str();
	m_MainOut = true;
	m_QROQSOut = true;
	m_MakeOut = true;

	UpdateData(false);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void COutCDlg::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString init_path, get_path;
	m_OutDir.GetWindowText(init_path);
//	TRACE("get %s\n",init_path);
	if(getsetPath( m_hWnd, init_path, get_path)){
		m_OutDir.SetWindowText(get_path);
	}
}

void COutCDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	UpdateData(true);

	CString ss;
	m_OutDir.GetWindowText(ss);
	if(ss!=m_pDoc->c_out_dir_list.front()){
		m_pDoc->c_out_dir_list.push_front(ss);
	}

	CDialog::OnOK();
}
