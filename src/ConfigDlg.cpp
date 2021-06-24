// ConfigDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "QROQS.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg ダイアログ

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg メッセージ ハンドラ

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
//	TRACE("conf init\n");

	m_Tab.InsertItem(0,"一般");
	m_Tab.InsertItem(1,"Edit");
	m_Tab.InsertItem(2,"Compile");
	m_Tab.InsertItem(3,"Simulation");

	//一般設定
	m_ConfGene.m_pDoc = m_pDoc;
	m_ConfGene.Create(IDD_CONF_GENE, this);

	//エディットフェーズ設定
	m_ConfEdit.m_pDoc = m_pDoc;
    m_ConfEdit.Create(IDD_CONF_EDIT, this);

	m_ConfCompile.m_pDoc = m_pDoc;
    m_ConfCompile.Create(IDD_CONF_COMPILE, this);
	//シミュレーションフェーズ設定
	m_ConfSim.m_pDoc = m_pDoc;
    m_ConfSim.Create(IDD_CONF_SIM, this);

    // タブコントロールの表示領域の取得
    CRect rect;
    m_Tab.GetWindowRect(&rect);     // タブコントロールのウィンドウ領域（スクリーン座標系）
    ScreenToClient(&rect);          // クライアント座標系に修正
    m_Tab.AdjustRect(FALSE, &rect); // ウィンドウ領域に基づき、表示部の領域に調整

	// ダイアログボックスの位置あわせ
    m_ConfGene.MoveWindow(&rect);
    m_ConfEdit.MoveWindow(&rect);
    m_ConfCompile.MoveWindow(&rect);
    m_ConfSim.MoveWindow(&rect);

	// 最初のページを表示
	showTab(0);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CConfigDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	showTab(m_Tab.GetCurSel());
	*pResult = 0;
}

int CConfigDlg::CheckDlgData()
{
	if(m_ConfGene.UpdateData(TRUE)==0) return 0;
    if(m_ConfEdit.UpdateData(TRUE)==0) return 1;
	if(m_ConfCompile.UpdateData(TRUE)==0) return 2;
    if(m_ConfSim.UpdateData(TRUE)==0) return 3;
    return 4;
}

void CConfigDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	int nCurSel=CheckDlgData();
    if(nCurSel<4){	//DDV問題があった
		showTab(nCurSel);
		return;
	}

	m_ConfGene.update=1;

	GetParent()->Invalidate();

    CDialog::OnOK();

}

void CConfigDlg::showTab(int nCurSel){
	m_Tab.SetCurSel(nCurSel);

	// 全てのページを非表示にする
	m_ConfGene.ShowWindow(SW_HIDE);
	m_ConfEdit.ShowWindow(SW_HIDE);
	m_ConfCompile.ShowWindow(SW_HIDE);
	m_ConfSim.ShowWindow(SW_HIDE);
	
	// 引数に該当するページのみを表示する
	switch(nCurSel){
	case 0: m_ConfGene.ShowWindow(SW_SHOW); break;
	case 1: m_ConfEdit.ShowWindow(SW_SHOW); break;
	case 2: m_ConfCompile.ShowWindow(SW_SHOW); break;
	case 3: m_ConfSim.ShowWindow(SW_SHOW); break;
//	default: throw "規定されていないプロパティページ:内部エラー";
	}
}

void CConfigDlg::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	m_ConfGene.update = 0;

	CDialog::OnCancel();
}


/// EOF ////

