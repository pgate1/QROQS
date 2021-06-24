// FindObjDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "FindObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg ダイアログ


CFindObjDlg::CFindObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindObjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindObjDlg)
	//}}AFX_DATA_INIT
}


void CFindObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindObjDlg)
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjList);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_SEARCH, m_btnSearch);
#endif
}


BEGIN_MESSAGE_MAP(CFindObjDlg, CDialog)
	//{{AFX_MSG_MAP(CFindObjDlg)
	ON_BN_CLICKED(ID_SEARCH, OnSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJ_LIST, OnItemchangedObjList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg メッセージ ハンドラ

BOOL CFindObjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	//一行選択
	ListView_SetExtendedListViewStyle(
		(HWND)m_ObjList.m_hWnd, LVS_EX_FULLROWSELECT);

//　リストコントロールのスタイル設定（グリッドラインの表示）
	DWORD dwStyle = ListView_GetExtendedListViewStyle( (HWND)m_ObjList.m_hWnd );
	dwStyle |= LVS_EX_GRIDLINES; // | LVS_EX_FULLROWSELECT;//CHECKBOXES,HEADERRAGDROP
	ListView_SetExtendedListViewStyle( (HWND)m_ObjList.m_hWnd, dwStyle );


	// カラムの追加
	m_ObjList.InsertColumn( 0, "ID", LVCFMT_LEFT, 75, -1);
	m_ObjList.InsertColumn( 1, "Type", LVCFMT_LEFT, 75, -1);
	m_ObjList.InsertColumn( 2, "Option", LVCFMT_LEFT, 75, -1);


	// アイテムの追加
	CString cs;
	int    nItem;
	map<int,bspObject>::iterator oit,oit_end;
	oit=m_bsp->ObjMap.begin();
	oit_end = m_bsp->ObjMap.end();
	for(nItem=0;oit!=oit_end;++oit,++nItem){
		cs.Format("%d",oit->first);
		bspObject &obj = oit->second;
		m_ObjList.InsertItem( nItem, cs, 0);
		switch(oit->second.type){
		case _PLACE: cs.Format("place"); break;
		case _TRANSITION: cs.Format("transition"); break;
		case _UHZI: cs.Format("uhzi"); break;
		case _KEMMAKIX: cs.Format("kemmakix"); break;
		case _MODULE: cs.Format("module"); break;
		}
		m_ObjList.SetItem( nItem, 1, LVIF_TEXT, cs, 0, 0, 0, 0);

		CString opt_str;
		if(obj.type==_MODULE){
			opt_str = m_bsp->ModMap[oit->first].c_str();
		}
		else if(obj.type==_UHZI){
			opt_str = m_bsp->UhziMap[oit->first]->input_name.c_str();
		}
		else if(obj.type==_KEMMAKIX){
			opt_str = m_bsp->KemMap[oit->first]->output_name.c_str();
		}
		if(!opt_str.IsEmpty()) opt_str += " ";
		if(m_bsp->TextMap.find(oit->first)!=m_bsp->TextMap.end()){
			opt_str += m_bsp->TextMap[oit->first].c_str();
		}
		if(!opt_str.IsEmpty()) opt_str += " ";
		if(obj.type==_TRANSITION
			&& m_bsp->simTranFireableStop.find(oit->first)!=m_bsp->simTranFireableStop.end()){
			opt_str += "SimFireStop";
		}

		if(!opt_str.IsEmpty()){
			m_ObjList.SetItem( nItem, 2, LVIF_TEXT, opt_str, 0, 0, 0, 0);
		}
	}


	nSelID=0;

	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow(false);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CFindObjDlg::OnSearch() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSel;
	nSel=m_ObjList.GetSelectionMark();
	if(nSel==-1){
		nSelID=0;
		CDialog::OnOK();
	}
//	TRACE("n %d\n",nSel);

	CString cs;
	cs=m_ObjList.GetItemText(nSel, 0);

	nSelID=atoi((LPCTSTR)cs);

//	TRACE("id %d\n",nSelID);

	CDialog::OnOK();
}


void CFindObjDlg::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	nSelID=0;

	CDialog::OnCancel();
}

void CFindObjDlg::OnItemchangedObjList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	//リストのアイテムが選択されたら表示ボタンを使用できるようにする
	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow();

	*pResult = 0;
}

/// EOF ///
