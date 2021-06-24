// ModuleTreeDlgbar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "ModuleTreeDlgbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModuleTreeDlgbar ダイアログ



CModuleTreeDlgBar::CModuleTreeDlgBar(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CModuleTreeDlgbar::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CModuleTreeDlgBar)
	//}}AFX_DATA_INIT
}


void CModuleTreeDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModuleTreeDlgBar)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModuleTreeDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CModuleTreeDlgBar)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MODTREE, OnSelchangedModtree)
	ON_NOTIFY(NM_RCLICK, IDC_MODTREE, OnRclickModtree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModuleTreeDlgBar メッセージ ハンドラ


#include"QROQSView.h"
#include"other.h"

void CModuleTreeDlgBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	TRACE("size\n");

	CMainFrame *pWnd=(CMainFrame *)AfxGetMainWnd();
	if(pWnd==NULL) return;

	CRect rect;
    pWnd->GetClientRect(&rect);     // タブコントロールのウィンドウ領域（スクリーン座標系）

	if(rect.right<=0 || rect.bottom<=0) return;
//	TRACE("(%d,%d)(%d,%d)\n",rect.left,rect.top,rect.right,rect.bottom);

	rect.top = 0;
	rect.left = 0;
	rect.right = 100;
	rect.bottom -= 50;

//	TRACE("(%d,%d)(%d,%d)\n",rect.left,rect.top,rect.right,rect.bottom);

	MoveWindow(rect);

	CWnd *tout = GetDlgItem(IDC_MODTREE);
	if(tout!=NULL){
//		TRACE("set\n");
		rect.top	= 10;
		rect.left	= 0;
		rect.right	-= 4;
		rect.bottom -= 6;
		tout->MoveWindow(rect);
	}
	
}

BOOL CModuleTreeDlgBar::IsNamed(HTREEITEM hitem)
{
	CTreeCtrl *pModTree= GetModuleTree();
	int id = pModTree->GetItemData( hitem );
	if(id==0) return 1;
	CQROQSView* pView =  (CQROQSView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveView());
	if(pView==NULL) return 0;
	BSPetri *bsp = pView->bsp_tree->GetBSP( (LPCTSTR)pModTree->GetItemText(pModTree->GetParentItem(hitem)) );
	if(bsp->ModMap[id].empty()) return 0;
	return 1;
}

void CModuleTreeDlgBar::OnSelchangedModtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

//	TRACE("module selected\n");

	CTreeCtrl *pModTree = (CTreeCtrl *)GetDlgItem(IDC_MODTREE);

	HTREEITEM hitem, parent_item;
	hitem = pModTree->GetSelectedItem();

	CQROQSView* pView =  (CQROQSView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveView());
	if(pView==NULL) return;


	parent_item = pModTree->GetParentItem(hitem);

	int id = pModTree->GetItemData(hitem);
//	TRACE("id %d\n",id);

	if(parent_item==NULL) parent_item = hitem;

	if(PetMode == _EDIT){
		pView->bsp = pView->bsp_tree->GetBSP(pModTree->GetItemText(parent_item));
	//fprintf(tfp,"item %d %d %d\n",parent_item,hitem,id);
	//	TRACE("名前を設定しなさい\n");

		if(parent_item!=hitem && id!=0
			&& pView->bsp->ModMap[id].empty()){
/*
//			AfxMessageBox("モジュール名が設定されていません");
			pModTree->SelectItem(pView->current_item);
			return;
*/
			//名前の設定
			pView->PutIt = id;
			pView->SendMessage(WM_COMMAND,ID_MODULE_PROPATY);
			if(pView->bsp->ModMap[pView->PutIt].empty()){
				pModTree->SelectItem(pView->current_item);
				return;
			}

		}
		//子に移動
		pView->bsp = pView->bsp_tree->GetBSP(pModTree->GetItemText(hitem));
	}
	else{	//PetMode == _SIMULATION

		//上まで移動しながらidを取得
		HTREEITEM pitem;
		stack<int> tree_id;
		pitem = hitem;
		for(;pitem!=pModTree->GetRootItem();){
			tree_id.push( pModTree->GetItemData(pitem) );
			pitem = pModTree->GetParentItem( pitem );
		}
		//idを見ながら下に移動
		BSPetri *cur_bsp;
		cur_bsp = pView->bsp_tree->GetRootBSP();
		for(;!tree_id.empty();){
			cur_bsp = cur_bsp->ModuleMap[ tree_id.top() ];
//名前が設定されていないなら移動しない
			if(cur_bsp==NULL){
				TRACE("このモジュールに移動することはできません。設定されていません:内部エラー");
				pModTree->SelectItem(pView->current_item);
				return;
			}
			tree_id.pop();
		}

		//名前が設定されていないなら移動しない
		if(cur_bsp->module_name.empty()){
			return;
		}
		pView->bsp = cur_bsp;
	}

	pView->GetDocument()->SetTitle(pView->bsp->module_name.c_str());

	pView->current_item = hitem;
	pView->viewDiam = pView->bsp->viewDiam;
	pView->SendMessage(WM_MOUSEWHEEL);

	pView->ScrollSet();
	pView->SendMessage(WM_KEYDOWN,VK_ESCAPE);

	if(pView->bsp->Globalpoint.x==0 && pView->bsp->Globalpoint.y==0){
		pView->FitView();
	}

//TRACE("selected end\n");
	*pResult = 0;
}



void CModuleTreeDlgBar::OnRclickModtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	CTreeCtrl *pModTree=(CTreeCtrl *)GetDlgItem(IDC_MODTREE);


	CPoint point;
 
	const MSG* pmsg = GetCurrentMessage();
	point= pmsg->pt;
	//::GetCursorPos(&point);
	ScreenToClient(&point);
//	TRACE("%d %d\n",point.x,point.y); 
	point.y-=15;
    UINT flags;
    HTREEITEM hitem = pModTree->HitTest(point, &flags);
    if (hitem == NULL) return;

//	TRACE("%s\n",pModTree->GetItemText(hitem));


	if(hitem != pModTree->GetRootItem()) return;

	CQROQSView* pView =  (CQROQSView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveView());

	pModTree->SelectItem(hitem);

#ifdef _LIGHT
	CMenu menu;
	menu.CreatePopupMenu();
#else
	CNewMenu menu;
	menu.CreatePopupMenu();
	menu.SetMenuTitle( pModTree->GetItemText(hitem), MFT_GRADIENT|MFT_TOP_TITLE);
#endif
	menu.AppendMenu(MF_STRING, ID_BSP_PROPATY, "ﾌﾟﾛﾊﾟﾃｨ");

	::GetCursorPos(&point);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x+10,point.y, this);

	*pResult = 0;
}

//mnameであるモジュール（どれでもいい）に移動する
void CModuleTreeDlgBar::SelectModule(const char *mname)
{
	if(mname==NULL) return;
	if(mname[0]=='\0') return;

	// mnameであるモジュールを見つける（どれでもいい）
	struct func{
		static HTREEITEM mfind(
			CTreeCtrl *pModTree,
			const char *mname,
			HTREEITEM cur_item)
		{
	//		TRACE("find %s\n",pModTree->GetItemText(cur_item));
			if(pModTree->GetItemText(cur_item) == mname){
				return cur_item;
			}
			HTREEITEM citem,retitem=NULL;
			citem = pModTree->GetChildItem(cur_item);
			for(;citem!=NULL;){
				retitem = mfind(pModTree, mname, citem);
				if(retitem != NULL) break;
				citem = pModTree->GetNextSiblingItem(citem);
			}
			return retitem;
		}
	};

	CTreeCtrl *pModTree = GetModuleTree();
	HTREEITEM retitem;
	retitem = func::mfind(pModTree, mname, pModTree->GetRootItem());
	if(retitem==NULL) return;
	pModTree->SelectItem( retitem );
}

void CModuleTreeDlgBar::SelectRootModule()
{
//	TRACE("set root\n");
	CTreeCtrl *pModTree = GetModuleTree();
	pModTree->SelectItem(pModTree->GetRootItem());
	LRESULT pResult;
	OnSelchangedModtree(NULL, &pResult);
}

/*
void CModuleTreeDlgBar::OnEndlabeleditModtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if(pTVDispInfo->item.pszText==NULL
		|| pTVDispInfo->item.pszText[0]=='\0'){
		//NULLなら変更しない
		*pResult = 0;
		return;
	}

//	CQROQSView* pView =  (CQROQSView*)(((CMainFrame *)AfxGetMainWnd())->GetActiveView());

//	TRACE("sel %s\n",pModTree->GetItemText(htree));
	TRACE("new %s\n",pTVDispInfo->item.pszText);

	//trueで変更される
	*pResult = 1;
}
*/

/// EOF ///
