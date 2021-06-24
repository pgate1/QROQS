// ConfCompile.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "ConfCompile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfCompile ダイアログ

#include"QROQSView.h"


CConfCompile::CConfCompile(CWnd* pParent /*=NULL*/)
	: CDialog(CConfCompile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfCompile)
	m_CompSave = FALSE;
	//}}AFX_DATA_INIT
}

void CConfCompile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfCompile)
	DDX_Check(pDX, IDC_CONF_EDIT_COMPSAVE, m_CompSave);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfCompile, CDialog)
	//{{AFX_MSG_MAP(CConfCompile)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfCompile メッセージ ハンドラ

BOOL CConfCompile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	m_CompSave = pView->confCompileSave;

	UpdateData(false);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CConfCompile::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	pView->confCompileSave = m_CompSave;

}


//  EOF //

