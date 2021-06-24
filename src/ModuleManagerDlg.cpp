// ModuleManagerDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "ModuleManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleManagerDlg ダイアログ


CModuleManagerDlg::CModuleManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModuleManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModuleManagerDlg)
	m_Root = FALSE;
	m_SimTime = 0;
	m_SelMod = _T("");
	//}}AFX_DATA_INIT
}


void CModuleManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModuleManagerDlg)
	DDX_Control(pDX, IDC_MOD_LIST, m_ModList);
	DDX_Check(pDX, IDC_ROOT, m_Root);
	DDX_Text(pDX, IDC_SIMTIME, m_SimTime);
	DDV_MinMaxInt(pDX, m_SimTime, 1, 10000000);
	DDX_Text(pDX, IDC_SELECTED_MODULE, m_SelMod);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_SEARCH, m_btnSearch);
#endif
}


BEGIN_MESSAGE_MAP(CModuleManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CModuleManagerDlg)
	ON_BN_CLICKED(ID_SEARCH, OnSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MOD_LIST, OnItemchangedModList)
	ON_NOTIFY(NM_RCLICK, IDC_MOD_LIST, OnRclickModList)
	ON_BN_CLICKED(IDC_ROOT, OnRoot)
	ON_EN_CHANGE(IDC_SIMTIME, OnChangeSimtime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleManagerDlg メッセージ ハンドラ

BOOL CModuleManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	//一行選択
	ListView_SetExtendedListViewStyle(
		(HWND)m_ModList.m_hWnd, LVS_EX_FULLROWSELECT);

//　リストコントロールのスタイル設定（グリッドラインの表示）
	DWORD dwStyle = ListView_GetExtendedListViewStyle( (HWND)m_ModList.m_hWnd );
	dwStyle |= LVS_EX_GRIDLINES; // | LVS_EX_FULLROWSELECT;//CHECKBOXES,HEADERRAGDROP
	ListView_SetExtendedListViewStyle( (HWND)m_ModList.m_hWnd, dwStyle );

	m_RootName = m_bsp_tree->root_name.c_str();

	// カラムの追加
	m_ModList.InsertColumn( 0, "Module", LVCFMT_LEFT, 75, -1);
	m_ModList.InsertColumn( 1, "Objects", LVCFMT_LEFT, 75, -1);
	m_ModList.InsertColumn( 2, "Option", LVCFMT_LEFT, 75, -1);

	InitItems();

	// 現在のモジュールを選択
	if(m_SelMod == m_RootName){
		m_Root = true;
	}

	map<string,BSPetri*>::iterator bspit;
	bspit=m_bsp_tree->bsps.begin();
	for(;bspit!=m_bsp_tree->bsps.end();++bspit){
		m_Maxtime[bspit->first.c_str()] = bspit->second->confSimMaxTurn;
	}
	m_SimTime = m_Maxtime[m_SelMod];

	CEdit *ed;
	ed=(CEdit *)GetDlgItem(IDC_SIMTIME);
	if(m_Root){
		ed->EnableWindow();
	}
	else{
		ed->EnableWindow(false);
	}

	UpdateData(false);


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CModuleManagerDlg::InitItems()
{

	m_ModList.DeleteAllItems();
	// アイテムの追加
	CString cs;
	int nItem;
	map<string,BSPetri*>::iterator bspit;
	bspit=m_bsp_tree->bsps.begin();
	for(nItem=0;bspit!=m_bsp_tree->bsps.end();++bspit,++nItem){

		m_ModList.InsertItem( nItem, bspit->first.c_str(), 0);

		cs.Format("%d",bspit->second->ObjMap.size());
		m_ModList.SetItem( nItem, 1, LVIF_TEXT, cs, 0, 0, 0, 0);

		if(bspit->first==(LPCTSTR)m_RootName){
			m_ModList.SetItem( nItem, 2, LVIF_TEXT, "ROOT", 0, 0, 0, 0);
		}
	}

	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow(false);
}


#include "MainFrm.h"
#include "QROQSDoc.h"
#include "QROQSView.h"
#include "other.h"
void CModuleManagerDlg::OnSearch() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	GetModTreeDlg()->SelectModule((LPCTSTR)m_SelMod);
	GetParent()->UpdateData();
	CQROQSView *pView = (CQROQSView*)((CMainFrame*)::AfxGetMainWnd())->GetActiveView();
	pView->FitView();

//	CDialog::OnOK();
}

void CModuleManagerDlg::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	m_SelMod.Empty();

	CDialog::OnCancel();
}

void CModuleManagerDlg::OnItemchangedModList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
		
//	UpdateData(true);

	//リストのアイテムが選択されたら表示ボタンを使用できるようにする
	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow();

//	TRACE("item changed\n");

	int nSel;
	nSel=m_ModList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nSel==-1){
		return;
	}
//	TRACE("n %d\n",nSel);

	m_SelMod = m_ModList.GetItemText(nSel, 0);
//	TRACE("m %s\n",SelmName.c_str());

	if(m_SelMod == m_RootName){
		m_Root = true;
	}
	else m_Root = false;

	m_SimTime = m_Maxtime[ m_SelMod ];

	CEdit *ed;
	ed=(CEdit *)GetDlgItem(IDC_SIMTIME);
	if(m_Root){
		ed->EnableWindow();
	}
	else{
		ed->EnableWindow(false);
	}

	UpdateData(false);

	*pResult = 0;
}

void CModuleManagerDlg::OnRoot() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if(m_Root){
		UpdateData(false);
		return;
	}

	m_RootName = m_SelMod;

	InitItems();

	CEdit *ed;
	ed=(CEdit *)GetDlgItem(IDC_SIMTIME);
	ed->EnableWindow();

	UpdateData(true);

}

void CModuleManagerDlg::OnChangeSimtime() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	UpdateData(true);
	m_Maxtime[m_SelMod] = m_SimTime;

}

void CModuleManagerDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	if(m_RootName != m_bsp_tree->root_name.c_str()){
		// ROOTを更新した時に移動する
		CQROQSDoc *pDoc = (CQROQSDoc*)((CMainFrame*)::AfxGetMainWnd())->GetActiveDocument();
		pDoc->bsp_tree.SetRootBSP( (LPCTSTR)m_RootName );
		pDoc->InitTreeView();
	}
	map<CString,int>::iterator mtit;
	mtit = m_Maxtime.begin();
	for(;mtit!=m_Maxtime.end();++mtit){
		m_bsp_tree->GetBSP( (LPCTSTR)mtit->first )->confSimMaxTurn = mtit->second;
	}

	CDialog::OnOK();
}


void CModuleManagerDlg::OnRclickModList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	return;
	/*
	// リストコントロールで右クリックサンプル

	int nSel;
	nSel=m_ModList.GetSelectionMark();
	if(nSel==-1){
		return;
	}
//	TRACE("n %d\n",nSel);
	SelmName = m_ModList.GetItemText(nSel, 0);
//	TRACE("%s\n",SelmName.c_str());

#ifdef _LIGHT
	CMenu menu;
	menu.CreatePopupMenu();
#else
	CNewMenu menu;
	menu.CreatePopupMenu();
	menu.SetMenuTitle( SelmName.c_str(), MFT_GRADIENT|MFT_TOP_TITLE);
#endif
	menu.AppendMenu(MF_STRING, ID_MOD_SET_ROOT, "ROOTに設定");
	menu.AppendMenu(MF_STRING, ID_MOD_MAX_TURN, "シミュ時間");

	CPoint point;
	::GetCursorPos(&point);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x+10,point.y, this);
*/
	*pResult = 0;
}

/// EOF ///
