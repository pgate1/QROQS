// QROQSViewModule.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"
#include "QROQSView.h"

#include "other.h"

void CQROQSView::InitModuleTree(BSPetriTree &bsp_tree)
{

//	TRACE("init tree\n");

	struct func{
		static void add_tree(
			BSPetriTree &bspt,
			CTreeCtrl *pModTree,
			HTREEITEM htree,
			BSPetri *cur_bsp)
		{
	//		TRACE("root %s\n",cur_bsp->module_name.c_str());

			HTREEITEM chtree, nextitem;
			map<int,HTREEITEM> tmap;
			int idata;

			//データに存在しなかったらツリーから削除
			chtree = pModTree->GetChildItem(htree);
			for(;chtree!=NULL;){
				// 登録
				idata = pModTree->GetItemData(chtree);
			//	TRACE("%d\n", idata);
				tmap[idata] = chtree;
				
				nextitem = pModTree->GetNextSiblingItem(chtree);
				if(cur_bsp->ModMap.find(idata) == cur_bsp->ModMap.end()){
				//	TRACE("del %s\n",pModTree->GetItemText(chtree));
					pModTree->DeleteItem(chtree);
					tmap.erase(idata);
				}
				chtree = nextitem;
			}

		//	TRACE("idata %d\n", tmap.size());

			map<int, string>::iterator mit,mit_end;
			mit = cur_bsp->ModMap.begin();
			mit_end = cur_bsp->ModMap.end();
			for(;mit!=mit_end;++mit){
				idata = mit->first;
				if(tmap.find(idata)==tmap.end()){
					// ツリーに存在しないので新規追加する
					chtree = pModTree->InsertItem(
						mit->second.empty() ? "no_name" : mit->second.c_str(),
						htree, TVI_LAST);
					pModTree->SetItemData( chtree, idata);
				//	TRACE("add tree\n");
				}
				else{
					chtree = tmap[idata];
					if(!mit->second.empty()
						&& pModTree->GetItemText(chtree) != mit->second.c_str()){
						// 名前が違うので変更
				//		TRACE("change name : %s -> %s\n", pModTree->GetItemText(chtree), mit->second.c_str());
						pModTree->SetItemText(chtree, mit->second.c_str());
					}
				}

				if(!mit->second.empty()){
				//	TRACE("%s %d\n",mit->second.c_str(),bspt.GetBSP(mit->second.c_str()));
					add_tree(bspt, pModTree, chtree, bspt.GetBSP(mit->second.c_str()));
				}
			}

			return;
		}
	};


	CTreeCtrl *pModTree = GetModuleTree();

	HTREEITEM htree;
	htree = pModTree->GetRootItem();
	if(htree==NULL){
		// ないなら追加
	//	TRACE("add root %s\n",bsp_tree.root_name.c_str());
		htree = pModTree->InsertItem(bsp_tree.root_name.c_str(),TVI_ROOT, TVI_LAST);
		pModTree->SetItemData( htree, 0);
	}
	else if(pModTree->GetItemText(htree) != bsp_tree.root_name.c_str()){
		// 名前が違うなら変更
		pModTree->SetItemText(htree, bsp_tree.root_name.c_str());
	}

	// ロックをかけて再描画させない
	CWnd::LockWindowUpdate();
	func::add_tree(bsp_tree, pModTree, htree, bsp_tree.GetRootBSP());
	CWnd::UnlockWindowUpdate();

}


void CQROQSView::OnModuleUp()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	TRACE("module up\n");
	CTreeCtrl *pModTree = GetModuleTree();
	if(current_item == pModTree->GetRootItem()){
		return;
	}

	current_item = pModTree->GetParentItem(current_item);
	pModTree->SelectItem(current_item);

	pos_=1;
	Invalidate(0);
}

void CQROQSView::OnUpdateModuleUp(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください

//	pCmdUI->Enable(current_item != GetModuleTree()->GetRootItem());
}


void CQROQSView::OnModFunction()
{
	if(bsp->ModMap[PutIt].empty()){
		//module_nameが設定されていないので
		//設定ダイアログを表示する
		if(PetMode==_EDIT){
			OnModulePropaty();
		}
	}
	else{
		//module nameが設定されているなら下位モジュールに移動する
		CTreeCtrl *pModTree = GetModuleTree();

		HTREEITEM hitem;
		hitem = pModTree->GetChildItem(current_item);
		for(;hitem!=NULL;){
			if(pModTree->GetItemData(hitem) == PutIt) break;
			hitem = pModTree->GetNextSiblingItem(hitem);
		}
		if(hitem==NULL){
			TRACE("下位モジュールがない：内部エラー");
			return;
		}
		
		pModTree->SelectItem(hitem);

		if(bsp->ErrPos){		//エラー表示
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 20, NULL); //作成
		}
	}

	Invalidate(0);
}

#include"ModuleManagerDlg.h"

void CQROQSView::OnModManager() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	CModuleManagerDlg mmDlg;
	mmDlg.m_bsp_tree = bsp_tree;
	mmDlg.m_SelMod = bsp->module_name.c_str();
	if(mmDlg.DoModal()==IDCANCEL){
		return;
	}
/*
	if(!mmDlg.m_SelMod.IsEmpty()){
		GetModTreeDlg()->SelectModule((LPCTSTR)mmDlg.m_SelMod);
	}
*/
}


#include"TokenfuncDlg.h"

void CQROQSView::OnModulePropaty() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	TRACE("propaty %s %d\n",bsp->module_name.c_str(),PutIt);
//TRACE("p %d\n",PutIt);
	//設定するモジュールに移動しておくこと

	CTokenfuncDlg mnameDlg;
	if(bsp->ModMap.find(PutIt)!=bsp->ModMap.end()){
		mnameDlg.m_input = bsp->ModMap[PutIt].c_str();
	}
	else if(PutIt==0){
		mnameDlg.m_input = bsp_tree->GetRootName();
	}
	else{
		mnameDlg.m_input.Empty();
	}
	mnameDlg.m_str = "ﾓｼﾞｭｰﾙ名を設定してください．";

	if(mnameDlg.DoModal()==IDOK){
//		TRACE("set %s\n",mnameDlg.m_input);
		if(mnameDlg.m_input.IsEmpty()) return;
		UndoSave();
		int ret;
		ret = bsp_tree->SetModuleName(bsp->module_name.c_str(), PutIt, (LPCTSTR)mnameDlg.m_input);
		if(ret==0){
			CString cs;
			cs = mnameDlg.m_input + " : 変更できません．\n";
			cs += bsp_tree->err_msg.c_str();
			AfxMessageBox(cs,MB_OK|MB_DEFBUTTON2);
#ifndef _LIGHT
			help_id = 4;
#endif
			return;
		}

		if(PutIt==0){
			bsp = bsp_tree->GetRootBSP();
			CQROQSDoc *pDoc = GetDocument();
			pDoc->SetTitle( bsp->module_name.c_str() );
		}

		InitModuleTree(*bsp_tree);
		ExpandAll(GetModuleTree());
		Invalidate(0);
	}

}

// ﾓｼﾞｭｰﾙﾂﾘｰ制御
void CQROQSView::ExpandAll(CTreeCtrl *pTree)
{
	return;	// 展開しない

	struct func{
		static void expand(CTreeCtrl *pTree, HTREEITEM cur)
		{
			for(;cur!=NULL;){
				pTree->Expand(cur, TVE_EXPAND);
				expand(pTree, pTree->GetChildItem(cur));
				cur = pTree->GetNextSiblingItem( cur );
			}
		}
	};
	// ロックをかけて再描画させない
	CWnd::LockWindowUpdate();
	func::expand(pTree, pTree->GetRootItem());
	CWnd::UnlockWindowUpdate();

}

void CQROQSView::OnBspPropaty()
{
	PutIt = 0;

	CTreeCtrl *pModTree = GetModuleTree();
	pModTree->SelectItem( pModTree->GetRootItem() );
	OnModulePropaty();
}


void CQROQSView::OnModuleRead()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	CFileDialog fd(
		TRUE,	//開く
		"bsp",	//デフォルトの拡張子
		"",	//デフォルトのファイル名
		OFN_FILEMUSTEXIST|
		OFN_HIDEREADONLY|
		OFN_LONGNAMES,	//フラグ
		"ﾋﾞｯﾄｼﾘｱﾙﾍﾟﾄﾘﾈｯﾄ (*.bsp)|*.bsp|"
		"全てのﾌｧｲﾙ (*.*)|*.*||",	//フィルタ
		NULL	//親ウィンドウ
		);
	
	CQROQSDoc *pDoc = GetDocument();
	fd.m_ofn.lpstrInitialDir = pDoc->filepath;	//初期ディレクトリ
	
	if(fd.DoModal() != IDOK) return;
	
	CString filename = fd.GetPathName();

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	try{
		UndoSave();

		BSPetriTree read_bsp_tree;
		read_bsp_tree.Open((LPCTSTR)filename);

		string mname;
		mname = read_bsp_tree.GetRootName();

		int ret;
		ret = bsp_tree->SetModuleName(bsp->module_name.c_str(), PutIt, mname.c_str());
		if(ret==0){
			string err;
			err = mname + " : 変更できません．\n" + bsp_tree->err_msg;
			AfxMessageBox(err.c_str(),MB_OK|MB_DEFBUTTON2);
#ifndef _LIGHT
			help_id = 4;
#endif
			return;
		}	
		
		InitModuleTree(*bsp_tree);


		//モジュールが貼り付けられるかチェック
		//全て貼り付け
		set<int> oset;
		map<int,bspObject>::iterator oit,oit_end;
		oit = read_bsp_tree.GetRootBSP()->ObjMap.begin();
		oit_end = read_bsp_tree.GetRootBSP()->ObjMap.end();
		for(;oit!=oit_end;++oit){
			oset.insert(oit->first);
		}
		if(bsp_tree->IsAbleAdds(mname.c_str(),&read_bsp_tree,read_bsp_tree.GetRootName(),oset)==0){
			AfxMessageBox("ﾓｼﾞｭｰﾙを読み込むことはできません．\nすでに上位ﾓｼﾞｭｰﾙで下位ﾓｼﾞｭｰﾙが使用されています．",MB_OK|MB_DEFBUTTON2);
			OnEditUndo();
			return;
		}



		//読み込んだモジュールは上書き
		//ダストモジュールは削除する
		//参照されているモジュールがあるなら上書きしない

		delete bsp_tree->GetBSP(mname.c_str());
		bsp_tree->bsps.erase( mname );
//		bsp_tree->DeleteNonRefer();

		map<string,BSPetri*>::iterator bspit;
		bspit = read_bsp_tree.bsps.begin();
		for(;bspit!=read_bsp_tree.bsps.end();++bspit){
			if(bsp_tree->GetBSP( bspit->first.c_str() ) == NULL){
				bsp_tree->bsps[ bspit->first ] = new BSPetri;
				*(bsp_tree->bsps[ bspit->first ]) = *(bspit->second);
			}
		}

		Trace("%s : 読み込みました．\r\n",(LPCTSTR)filename);
	}
	catch(char * err){
//		TRACE("err\n");
//		top_bsp.PetClear();
		Trace("%s %s\n",(LPCTSTR)filename,err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return;
	}

	
	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	Invalidate(0);

	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除

}


void CQROQSView::OnModExpandall() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
#ifdef _LIGHT
	AfxMessageBox("Lightﾊﾞｰｼﾞｮﾝでは機能制限されています．",MB_OK);
#else
	if(AfxMessageBox(
		"全てのﾓｼﾞｭｰﾙを展開します．\nよろしいですか？",
		MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
		return;
	}

	//一旦コンパイル
	OnComCompile();
	SetTimer(_TID_MEA_CHECK, 100, NULL);
#endif

}

#ifndef _LIGHT
void CQROQSView::ModExpandallPost()
{

	if(compOk == false) return;
	OnComHalt();

	Trace("展開中...\n");

	BSPetriTree exp_bsp_tree;

	exp_bsp_tree = *bsp_tree;
	exp_bsp_tree.NormalizeGlobalRect();

time_t st;
st = time(NULL);
	exp_bsp_tree.ExpandBSPAll( exp_bsp_tree.GetBSP(bsp->module_name.c_str()) , true);
Trace("expand %d sec\n",time(NULL)-st);

	*bsp = *( exp_bsp_tree.GetBSP(bsp->module_name.c_str()) );

	bsp_tree->DeleteNonRefer();

	InitModuleTree(*bsp_tree);
	pos_ = 1;
	Invalidate(0);

	Trace("ﾓｼﾞｭｰﾙを展開しました．\n");
}
#endif



void CQROQSView::OnUpdateModulePropaty(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(true);
}

void CQROQSView::OnUpdateModuleRead(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(true);
}


/// EOF ///
