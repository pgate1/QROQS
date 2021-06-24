// OutVerilogDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "OutVerilogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutVerilogDlg ダイアログ


COutVerilogDlg::COutVerilogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutVerilogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutVerilogDlg)
	m_ModuleName = _T("");
	m_OverPlus = 0.0;
	m_VSync = FALSE;
	m_VDebug = FALSE;
	m_OptModule = _T("");
	m_OptTrafunc = _T("");
	//}}AFX_DATA_INIT
	
}


void COutVerilogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutVerilogDlg)
	DDX_Control(pDX, IDC_COMBO1, m_OutDir);
	DDX_Text(pDX, IDC_EDIT_MODULE_NAME, m_ModuleName);
	DDV_MaxChars(pDX, m_ModuleName, 32);
	DDX_Text(pDX, IDC_EDIT_OVER_PLUS, m_OverPlus);
	DDV_MinMaxDouble(pDX, m_OverPlus, -0.5, 2.);
	DDX_Check(pDX, IDC_VSYNC, m_VSync);
	DDX_Check(pDX, IDC_VDEBUG, m_VDebug);
	DDX_Text(pDX, IDC_OPTION_MODULE, m_OptModule);
	DDX_Text(pDX, IDC_OPTION_TRAFUNC, m_OptTrafunc);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(COutVerilogDlg, CDialog)
	//{{AFX_MSG_MAP(COutVerilogDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutVerilogDlg メッセージ ハンドラ

#include"other.h"

int COutVerilogDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	return 0;
}


BOOL COutVerilogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	

	m_OutDir.ResetContent();
	list<CString>::iterator lit;
	lit=m_pDoc->verilog_out_dir_list.begin();
	for(;lit!=m_pDoc->verilog_out_dir_list.end();++lit){
		m_OutDir.InsertString(-1, *lit);
	}

	CString ss;
	m_OutDir.GetLBText(0,ss);
	m_OutDir.SetWindowText(ss);

//	m_ModuleName = m_pDoc->top_bsp.module_name.c_str();
	m_VDebug = m_pDoc->verilog_debug;
	m_VSync = m_pDoc->verilog_out_sync;
	m_OptModule = m_pDoc->verilog_module_option;
	m_OptTrafunc = m_pDoc->verilog_trafunc_option;


	UpdateData(false);



	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void COutVerilogDlg::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
		

	CString init_path, get_path;
	m_OutDir.GetWindowText(init_path);
//	TRACE("get %s\n",init_path);
	if(getsetPath( m_hWnd, init_path, get_path)){
		m_OutDir.SetWindowText(get_path);
	}

}


void COutVerilogDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	UpdateData(true);


	CString ss;
	m_OutDir.GetWindowText(ss);
	if(ss!=m_pDoc->verilog_out_dir_list.front()){
		m_pDoc->verilog_out_dir_list.push_front(ss);
	}

	m_pDoc->verilog_debug = m_VDebug;
	m_pDoc->verilog_out_sync = m_VSync;

	m_pDoc->verilog_module_option = m_OptModule;
	m_pDoc->verilog_trafunc_option = m_OptTrafunc;

	CDialog::OnOK();
}

/// EOF ///



