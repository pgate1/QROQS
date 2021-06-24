// QROQSViewEdit.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSView.h"


#include"FindObjDlg.h"

#include "other.h"

void CQROQSView::OnEditFind() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	CFindObjDlg objDlg;
	objDlg.m_bsp = bsp;
	if(objDlg.DoModal()==IDCANCEL){
		return;
	}

	bsp->ErrPos = objDlg.nSelID;

	if(bsp->ObjMap.find(bsp->ErrPos) != bsp->ObjMap.end()){
		//オブジェクトの場所に移動
		mst=0;
		SetTimer(_TID_FMOVE, 1, NULL); //作成

		//派手に
		KillTimer(_TID_OBJBEE);
		SetTimer(_TID_OBJBEE, 10, NULL); //作成
	}

}

void CQROQSView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	
}

void CQROQSView::OnEditUndo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(undo_bsp[bsp->module_name].size()==0) return;

	CPoint gpoint;
	gpoint = bsp->Globalpoint;
	*bsp = undo_bsp[bsp->module_name].front();
	bsp->Globalpoint = gpoint;

	undo_bsp[bsp->module_name].pop_front();

	bsp->intrack.clear();

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	viewReset();
	
	Invalidate(0);
}


void CQROQSView::UndoSave()
{
	if(PetMode!=_EDIT) return;
	if(UndoSize==0) return;

	if(undo_bsp[bsp->module_name].size()>=UndoSize){
	//	TRACE("pop\n");
		undo_bsp[bsp->module_name].pop_back();
	}

	undo_bsp[bsp->module_name].push_front(*bsp);

//	TRACE("undo size %d/%d\n", undo_bsp[bsp->module_name].size(), UndoSize);
}

void CQROQSView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT && undo_bsp[bsp->module_name].size()!=0);
}

void CQROQSView::OnComSelectall() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	bsp->SelectAllObj();
	bsp->TrackRectSet();

	Invalidate(0);
}

void CQROQSView::OnUpdateComSelectall(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT );
}


void CQROQSView::OnComObjlap()
{

	//オブジェクトが0,1の時は気をつけること

	multimap<long, int> xmap;

	map<int,bspObject>::iterator oit,oit_end;
	oit = bsp->ObjMap.begin();
	oit_end = bsp->ObjMap.end();
	for(;oit!=oit_end;++oit){
		xmap.insert(
			make_pair<long,int>(oit->second.gpoint.x, oit->first)
			);
	}

#define _EDIT_KASANARI_SIZE 30

	multimap<long,int>::iterator sit,osit;

	sit = xmap.begin();
	osit = sit;
	if(sit!=xmap.end()) ++sit;
	for(;sit!= xmap.end();++sit){
		if(sit->first - osit->first < _EDIT_KASANARI_SIZE){
		//	TRACE("%d - %d = %d\n",sit->first,osit->first,sit->first-osit->first);
			if(abs(bsp->ObjMap[sit->second].gpoint.y - bsp->ObjMap[osit->second].gpoint.y) < _EDIT_KASANARI_SIZE){
			//	TRACE("	%d - %d = %d\n",bsp->ObjMap[sit->second].gpoint.y , bsp->ObjMap[osit->second].gpoint.y,bsp->ObjMap[sit->second].gpoint.y - bsp->ObjMap[osit->second].gpoint.y);
				break;
			}
		}
		osit = sit;
	}

	TraceClear();

	if(sit==xmap.end()){
		Trace("重なって配置されているｵﾌﾞｼﾞｪｸﾄは存在しませんでした．\n");
		return;
	}

	bsp->ErrPos = sit->second;
#ifndef _LIGHT
	help_id=42;
#endif
	Trace("ｵﾌﾞｼﾞｪｸﾄが重なって配置されています．\n");

}


/*
コピー、ペーストは、コピー用のBSPetriを使用して、
ペトリネットのコピーコンストラクタがあるといい
*/
void CQROQSView::OnEditCopy() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		
//	TRACE("intrack %d\n",intrack.size());

	// コピーした時に、コピー元の座標の特定が出来なかったら真中を入れておく
	CRect vrect = viewRect;
	vrect.DeflateRect(40,40);
	if(!vrect.PtInRect(Oldpoint)){
		Oldpoint.x = viewRect.Width() >> 1;
		Oldpoint.y = viewRect.Height() >> 1;
	}

	//新規コピー
	copy_bsp_tree.reset();
	set<int> puttrack;
	copy_bsp_tree.Add(copy_bsp_tree.root_name.c_str(), bsp_tree, bsp->module_name.c_str(), bsp->intrack, puttrack);
	//tkOldpointにグローバル座標を突っ込んでおく
	tkOldpoint = mDiampoint(viewDiam,Oldpoint) + bsp->Globalpoint;
	pasteOldpoint = tkOldpoint;


//	TRACE("in copy %d\n",intrack.size());
	
}

void CQROQSView::OnEditPaste() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	//モジュールが貼り付けられるかチェック
	//全て貼り付け
	set<int> oset;
	map<int,bspObject>::iterator oit;
	oit = copy_bsp_tree.GetRootBSP()->ObjMap.begin();
	for(;oit != copy_bsp_tree.GetRootBSP()->ObjMap.end();++oit){
		oset.insert(oit->first);
	}
	if(bsp_tree->IsAbleAdds(bsp->module_name.c_str(),&copy_bsp_tree,copy_bsp_tree.root_name.c_str(),oset)==0){
		AfxMessageBox("ここに貼り付けることはできません．\n下位ﾓｼﾞｭｰﾙはすでに上位ﾓｼﾞｭｰﾙで使用されています．",MB_OK|MB_DEFBUTTON2);
		return;
	}

	UndoSave();

	bsp_tree->Add(bsp->module_name.c_str(), &copy_bsp_tree, copy_bsp_tree.root_name.c_str(), oset, bsp->intrack);

	//TRACE("paste %d\n",intrack.size());

	// 全く同じ座標にペーストした時はずらす
	CPoint old_point;
	old_point = mDiampoint(viewDiam,Oldpoint) + bsp->Globalpoint;
	if(old_point == pasteOldpoint){
		Oldpoint.x += 40;
		Oldpoint.y += 40;
		old_point.x += 40;
		old_point.y += 40;
	}
	pasteOldpoint = old_point;

	ObjTrackMove(bsp->intrack,pasteOldpoint);
	bsp->TrackRectSet();


	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	pos_=1;
	
	Invalidate(0);
}

void CQROQSView::OnEditPasteHorz() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	copy_bsp_tree.GetRootBSP()->SelectAllObj();
	copy_bsp_tree.GetRootBSP()->TrackRectSet();
	CPoint mp;
	mp.x=copy_bsp_tree.GetRootBSP()->trackRect.Width()+V_CHIP_SIZE;
	mp.y=0;

	PasteChain(mp);
}

void CQROQSView::OnEditPasteVert() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	copy_bsp_tree.GetRootBSP()->SelectAllObj();
	copy_bsp_tree.GetRootBSP()->TrackRectSet();
	CPoint mp;
	mp.x=0;
	mp.y=copy_bsp_tree.GetRootBSP()->trackRect.Height()+V_CHIP_SIZE;

	PasteChain(mp);
}

#include"TokenfuncDlg.h"

void CQROQSView::PasteChain(CPoint mp) 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	//モジュールが貼り付けられるかチェック
	//全て貼り付け
	set<int> oset;
	map<int,bspObject>::iterator oit;
	oit = copy_bsp_tree.GetRootBSP()->ObjMap.begin();
	for(;oit != copy_bsp_tree.GetRootBSP()->ObjMap.end();++oit){
		oset.insert(oit->first);
	}
	if(bsp_tree->IsAbleAdds(bsp->module_name.c_str(),&copy_bsp_tree,copy_bsp_tree.root_name.c_str(),oset)==0){
		AfxMessageBox("ここに貼り付けることはできません．\n下位ﾓｼﾞｭｰﾙはすでに上位ﾓｼﾞｭｰﾙで使用されています．",MB_OK|MB_DEFBUTTON2);
		return;
	}

	int n=0;

	//ダイアログ
	CTokenfuncDlg tofDlg;
	tofDlg.m_input.Format("1");
	tofDlg.m_str = "配置数";
	if(tofDlg.DoModal()==IDOK){
		n = atoi((LPCTSTR)tofDlg.m_input);
	}
	if(n<=0) return;

	if(n>1000){
		AfxMessageBox("1000以上の配置は多すぎます！\n何をしようというのか．");
		return;
	}

	UndoSave();
	int i;
	CPoint gp;
	gp=mDiampoint(viewDiam,Oldpoint)+bsp->Globalpoint;
	set<int> tkset;
	set<int>::iterator sit,sit_end;
	bsp->intrack.clear();
	for(i=0;i<n;++i){
		bsp_tree->Add(bsp->module_name.c_str(), &copy_bsp_tree, copy_bsp_tree.root_name.c_str(), oset, tkset);

		//TRACE("paste %d\n",intrack.size());
		ObjTrackMove(tkset,gp);
		gp += mp;
		sit=tkset.begin();
		sit_end = tkset.end();
		for(;sit!=sit_end;++sit){
			bsp->intrack.insert(*sit);
		}
	}

	bsp->TrackRectSet();

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());

	pos_=1;
	
	Invalidate(0);
}

void CQROQSView::OnEditDelete()
{
	UndoSave();
	set<int>::iterator tkp;
	tkp=bsp->intrack.begin();
	for(;tkp!=bsp->intrack.end();++tkp){
		if(bsp->ObjMap.find(*tkp)==bsp->ObjMap.end()){
		//	throw "ゼロオブジェクトを削除しようとしました";
		}

		if(bsp->ObjMap[*tkp].type==_PLACE){
			if(bsp->TokenMap.find(*tkp)!=bsp->TokenMap.end()){
				bsp->TokenMap.erase(*tkp);
			}
		}
		
		//接続されているアークと共に削除
		bsp->DelObject(*tkp);
	}
	
	bsp->trackRect.SetRectEmpty();
	bsp->intrack.clear();
/*
#ifdef _DEBUG
	//DEBUGチェック
	if(bsp->vObjMap.find(0)!=bsp->vObjMap.end()){
		TRACE("OnEditDelete ゼロオブジェクトが存在しています。内部エラー");
	}
	if(bsp->vArcMap.find(0)!=bsp->vArcMap.end()){
		TRACE("OnEditDelete ゼロアークが存在しています。内部エラー");
	}
#endif
*/

//	TRACE("s %d\n",bsp->ModMap.size());

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());

	pos_=1;
	Invalidate(0);
}

void CQROQSView::OnEditCut() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	/*
	//DEBUGチェック
	if(intrack.find(0)!=intrack.end()){
		TRACE("OnEditCut ゼロオブジェクトが存在しています。内部エラー");
	}
	*/

	/*
#ifdef _DEBUG
	copy_bsp_tree.GetRootBSP()->arc_check();
#endif
*/
	OnEditCopy();
	/*
	//DEBUGチェック
	if(intrack.find(0)!=intrack.end()){
		TRACE("OnEditCut ゼロオブジェクトが存在しています。内部エラー");
	}
	*/
	OnEditDelete();
}


void CQROQSView::OnComPlaceFusion() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if(bsp->intrack.size()==0) return;
	if(PutIt==0) return;
	UndoSave();
	bsp->PlaceFusion(bsp->intrack,PutIt);
	pos_=1;
	Invalidate(0);
}


void CQROQSView::OnComPlaceFission() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	if(bsp->intrack.size()==0) return;
	UndoSave();
	bsp->PlaceFission(bsp->intrack);
	bsp->TrackRectSet();
	pos_=1;
	Invalidate(0);

}


void CQROQSView::OnUpdateComPlaceFusion(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(bsp->intrack.size()!=0);
}


void CQROQSView::OnUpdateComAlign(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(bsp->intrack.size()!=0);
}

void CQROQSView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}

void CQROQSView::OnUpdateEditPasteHorz(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}

void CQROQSView::OnUpdateEditPasteVert(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}


void CQROQSView::OnComText() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		return;
	}

	//ダイアログ
	CTokenfuncDlg tofDlg;
	if(bsp->TextMap.find(PutIt)!=bsp->TextMap.end()){
		tofDlg.m_input = bsp->TextMap[PutIt].c_str();
	}
	else{
		tofDlg.m_input.Empty();
	}
	tofDlg.m_str = "ﾃｷｽﾄを記入してください．";
	if(tofDlg.DoModal()!=IDOK){
		return;
	}
	
	if(tofDlg.m_input.IsEmpty()){
		bsp->TextMap.erase(PutIt);
	}
	else{
		bsp->TextMap[PutIt] = (LPCTSTR)tofDlg.m_input;
	}

	Invalidate(0);

}


/// EOF ///
