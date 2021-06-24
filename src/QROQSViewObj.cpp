// QROQSViewObj.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#include "other.h"

/////////////////////////////////////////////////////////////////////////////
// CQROQSViewObj

void CQROQSView::OnObjPlace() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _PLACE;
	now_command=_PLACE;
	StatusOut("ﾌﾟﾚｰｽを配置してください");
	Invalidate(0);
}

void CQROQSView::OnObjTransition() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _TRANSITION;
	now_command=_TRANSITION;
	StatusOut("ﾄﾗﾝｼﾞｼｮﾝを配置してください");
	Invalidate(0);
}

void CQROQSView::OnObjUhzi() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _UHZI;
	now_command=_UHZI;
	StatusOut("ｳｰｼﾞ(ｿｰｽﾄﾗﾝｼﾞｼｮﾝ)を配置してください");
	Invalidate(0);
}

void CQROQSView::OnObjKemmakix() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _KEMMAKIX;
	now_command=_KEMMAKIX;
	StatusOut("ｹﾑﾏｷ(ｼﾝｸﾄﾗﾝｼﾞｼｮﾝ)を配置してください");
	Invalidate(0);
}

void CQROQSView::OnObjBtoken() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _BTOKEN;
	now_command=_BTOKEN;
	StatusOut("ﾄｰｸﾝを配置してください");
	Invalidate(0);
}


void CQROQSView::OnObjModule() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	viewReset();
	SelectOBJ = _MODULE;
	now_command=_MODULE;
	StatusOut("ﾓｼﾞｭｰﾙを配置してください");
	Invalidate(0);

}

void CQROQSView::OnObjArc() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _ARC;
	now_command=_ARC;
	StatusOut("接続元を選択してください");
	Invalidate(0);
}

void CQROQSView::OnComChain() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _PLACE;
	now_command=_CHAIN;
	StatusOut("連続配置を行います");
	Invalidate(0);
}



void CQROQSView::OnObjHand() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _HAND;
	now_command=_HAND;
	StatusOut("ﾚﾃﾞｨ");
	Invalidate(0);
}

void CQROQSView::OnObjDelete() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	SelectOBJ = _DELETE;
	now_command=_DELETE;
	StatusOut("削除するｵﾌﾞｼﾞｪｸﾄを選択してください");
	Invalidate(0);
}


void CQROQSView::OnTimer(UINT nIDEvent)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	switch(nIDEvent){
	case _TID_PETSIM:
		//	TRACE("st2 %d\n",st2);
		PetSim();
		break;
#ifndef _LIGHT
	case _TID_BEETOKEN:{
		if(bee_token.size()==0) return;
		
		errnudRect.SetRectEmpty();
			
		CClientDC pDC(this);
		//サブでクリア
		BitBltRect(&pDC, &bufDC, erroudRect);
		
		int i;
		//トークンを飛ばす
		CRect bee_rect;
		for(i=0;i<bee_token.size();++i){
			bee_rect = bee_token[i].view(this);
			errnudRect.UnionRect(errnudRect, bee_rect);
		}
		if(bee_token[0].dd > 8){
			KillTimer(_TID_BEETOKEN);
			bee_token.clear();
			return;
		}
		
		errnudRect.InflateRect(1,1);
		erroudRect = errnudRect;
		}
		break;
#endif
	case _TID_OBJBEE:{//エラー表示
		
		int i;
		static int beetu=1;
		
		if(bsp->ObjMap.find(bsp->ErrPos)==bsp->ObjMap.end()){
			bsp->ErrPos=0;
		}
		if(bsp->ErrPos==0){
			KillTimer(_TID_OBJBEE);
			bee_err.clear();
			beetu=1;
			return;
		}
		
		errnudRect.SetRectEmpty();

		
		//サブでクリア
		BitBltRect(&bufDC, &m1DC, erroudRect);
		
		CPoint mp;

		//●を飛ばす
		int num=Qmode==1?200:2;
		if(beetu){
			beetu=0;
			bee_err.resize(num);
			for(i=0;i<num;++i) bee_err[i]=Beecir();
		}
		mp = bsp->ObjMap[bsp->ErrPos].gpoint-bsp->Globalpoint;
		int obj_type = bsp->ObjMap[bsp->ErrPos].type;
		CRect bee_rect;
		for(i=0;i<num;++i){
			bee_rect=bee_err[i].view(this,mp,obj_type,obj_type==_TRANSITION&&bsp->TranMap[bsp->ErrPos]->pass_tra?0:1);
			bee_rect.InflateRect(2,2);
			errnudRect.UnionRect(errnudRect, bee_rect);
		}
	
		CClientDC pDC(this);
		errnudRect.InflateRect(3,3);
		erroudRect.UnionRect(erroudRect,errnudRect);
		BitBltRect(&pDC, &bufDC, erroudRect);
		erroudRect = errnudRect;
		   }
		break;
	case _TID_FMOVE:{//エラー表示
		if(bsp->ObjMap.find(bsp->ErrPos)==bsp->ObjMap.end())
			break;
		static CPoint gmp;
		static int nnu;
//		TRACE("s %d\n",mst);
		if(mst==0){
			nnu=10*viewDiam;
			if(nnu==0){
				gmp.x=0;
				gmp.y=0;
			}
			else{
				gmp=bsp->ObjMap[bsp->ErrPos].gpoint-bsp->Globalpoint;
				gmp.x -= (viewRect.Width()>>1);
				gmp.y -= (viewRect.Height()>>1);
				gmp.x/=nnu;
				gmp.y/=nnu;
			}
			mst=1;
		}
		else if(mst<=nnu){
			CPoint ep;
			ep = bsp->Globalpoint+gmp;
			FaceMove((ep.x-bsp->Globalpoint.x)*viewDiam,(ep.y-bsp->Globalpoint.y)*viewDiam);
			mst++;
		}
		else{
			gmp=bsp->ObjMap[bsp->ErrPos].gpoint;
			gmp.x -= (viewRect.Width()>>1);
			gmp.y -= (viewRect.Height()>>1);
			FaceMove((gmp.x-bsp->Globalpoint.x)*viewDiam,(gmp.y-bsp->Globalpoint.y)*viewDiam);
			KillTimer(_TID_FMOVE);
		}
		Invalidate(0);

		}
		break;
#ifndef _LIGHT
	case _TID_DEMO:	//デモの実行
		KillTimer(_TID_DEMO);
//		if(m_LeaveFlag) break;
		OnComDemo();
		break;
#endif
	case _TID_INIT:	//初期化
		KillTimer(_TID_INIT);
		SetWindowLong(m_wndTraceDlg->GetSafeHwnd(), GWL_EXSTYLE,
			GetWindowLong(m_wndTraceDlg->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_wndTraceDlg->GetSafeHwnd(), 0, 120, LWA_ALPHA);
		TraceClear();
		break;
	case _TID_CHECK:	// BSPチェックスレッド監視
		DWORD dwParam;
		GetExitCodeThread(check_thread->m_hThread, &dwParam);
		if(dwParam == STILL_ACTIVE) break;
		// CWinThreadは終了すると自分でハンドル解放するようになっている
		delete check_thread;
		check_thread = NULL;
		KillTimer(_TID_CHECK);
		CheckEnd();
		break;

#ifndef _LIGHT
	case _TID_VOUT_CHECK:	// verilog out
		if(compOk && !check_thread){
			KillTimer(_TID_VOUT_CHECK);
			FileOutVerilogPost();
		}
		break;
	case _TID_COUT_CHECK:	// C out
		if(compOk && !check_thread){
			KillTimer(_TID_COUT_CHECK);
			FileOutCPost();
		}
		break;
	case _TID_MEA_CHECK:	// Mod Expand All
		if(compOk && !check_thread){
			KillTimer(_TID_MEA_CHECK);
			ModExpandallPost();
		}
		break;
	case _TID_RES_CHECK:	// Research
		if(compOk && !check_thread){
			KillTimer(_TID_RES_CHECK);
			ResearchPost();
		}
		break;
#endif
	}


	CView::OnTimer(nIDEvent);
}


void CQROQSView::OnComClear() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	if(AfxMessageBox("全て削除します．\nよろしいですか？",
		MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
		return;
	}
	
	viewReset();
	KillTimer(_TID_OBJBEE);
	
	OnComSelectall();
	OnEditDelete();
	
	TraceClear();
	Invalidate(0);
}

void CQROQSView::OnComWrite() //クイックセーブ
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	AfxGetApp()->DoWaitCursor(1); //マウスカーソル砂時計

	CQROQSApp *pApp = (CQROQSApp*)AfxGetApp();
	::SetCurrentDirectory(pApp->exe_dir);

	char *ftemp=new char[1024];
	try{
		sprintf(ftemp,"%s\\temp\\temp.txt",(LPCTSTR)pApp->exe_dir);
		FILE *fp = fopen(ftemp,"w");
		if(fp==NULL){
			throw "temp/temp.txt ﾌｧｲﾙを開けませんでした．";
		}
		delete[] ftemp;
		fprintf(fp,"RootBSP=%s;\n",bsp_tree->GetRootName());
		fclose(fp);
		bsp_tree->WriteAll("temp");
	}
	catch(char *err){
		delete[] ftemp;
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return;
	}
	StatusOut("ｸｲｯｸﾌｧｲﾙへ保存しました");
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
}

void CQROQSView::OnComOpen() //クイックロード
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	AfxGetApp()->DoWaitCursor(1); //マウスカーソル砂時計
	
	viewReset();
	undo_bsp.clear();

	CQROQSApp *pApp = (CQROQSApp*)AfxGetApp();
	::SetCurrentDirectory(pApp->exe_dir);

	char *ftemp = new char[1024];
	char *temp_root = new char[100];
	CQROQSDoc *pDoc=GetDocument();
	try{
		sprintf(ftemp,"%s\\temp\\temp.txt",(LPCTSTR)pApp->exe_dir);
		FILE *fp = fopen(ftemp,"r");
		if(fp==NULL){
			throw "temp/temp.txt ﾌｧｲﾙを開けませんでした．";
		}
		fscanf(fp,"RootBSP=%[^;];\n",temp_root);
		fclose(fp);
		sprintf(ftemp,"%s\\temp\\%s.bsp",(LPCTSTR)pApp->exe_dir,temp_root);

		pDoc->OnOpenDocument( ftemp );
		delete[] temp_root;
		delete[] ftemp;
	}
	catch(char * err){
		delete[] temp_root;
		delete[] ftemp;
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return;
	}
	pDoc->docst_=0;
	TraceClear();
	StatusOut("ｸｲｯｸﾌｧｲﾙを読み込みました");
	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	pos_=1;
	
	Invalidate(0);
	
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
}


#include"ConfigDlg.h"

void CQROQSView::OnComConfig() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CConfigDlg cfDlg;
	CQROQSDoc *pDoc = GetDocument();
	cfDlg.m_pDoc = pDoc;
	
	cfDlg.DoModal();
}


/*
各オブジェクト設定ダイアログは、PutIt変数にオブジェクトの位置情報が入っていること
*/
#include"TrafuncDlg.h"

void CQROQSView::OnTraFunction()
{
	CTrafuncDlg *tfDlg = new CTrafuncDlg;
	
	tfDlg->m_id = PutIt;
	tfDlg->m_mname = bsp->module_name;
	tfDlg->m_bspt = bsp_tree;
	tfDlg->m_bsp = bsp;
	tfDlg->Create(IDD_TRAFUNC,this);
	Invalidate(0);
}


#include"UhzifuncDlg.h"

void CQROQSView::OnUhziFunction()
{
	CUhzifuncDlg *ufDlg = new CUhzifuncDlg;
	
	ufDlg->m_id = PutIt;
	ufDlg->m_mname = bsp->module_name;
	ufDlg->m_bspt = bsp_tree;
	ufDlg->Create(IDD_UHZIFUNC,this);
	Invalidate(0);
}


#include"KemfuncDlg.h"

void CQROQSView::OnKemFunction()
{
	CQROQSDoc *pDoc = GetDocument();
	CKemfuncDlg *kfDlg = new CKemfuncDlg;
	
	kfDlg->m_id = PutIt;
	kfDlg->m_mname = bsp->module_name;
	kfDlg->m_bspt = bsp_tree;
	kfDlg->Create(IDD_KEMFUNC,this);
	Invalidate(0);
}


void CQROQSView::OnComAlign()
{
	if(bsp->intrack.size()==0) return;

	UndoSave();
	bsp->ModifiedFlag = 1;

	//オブジェクトを整列
	set<int>::iterator sit,sit_end;
	sit=bsp->intrack.begin();
	sit_end = bsp->intrack.end();
	for(;sit!=sit_end;++sit){
		bsp->ObjMap[*sit].gpoint =
			alignPoint(bsp->ObjMap[*sit].gpoint,vChipSize,HatchSize);
	}

	//アークの中継点を整列
	vector<CPoint>::iterator bit;
	map<int,bspArc>::iterator ait,ait_end;
	ait=bsp->ArcMap.begin();
	ait_end = bsp->ArcMap.end();
	for(;ait!=ait_end;++ait){
		if(bsp->intrack.find(ait->second.In)!=bsp->intrack.end()
			&& bsp->intrack.find(ait->second.Out)!=bsp->intrack.end()
			&& ait->second.bwtpoint!=NULL){
			bit=ait->second.bwtpoint->begin();
			for(;bit!=ait->second.bwtpoint->end();++bit){
				*bit = alignPoint(*bit,vChipSize,HatchSize);
			}
		}
	}

	
	bsp->TrackRectSet();
	pos_=1;

	Invalidate(0);
}


void CQROQSView::OnViewObjId() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	EnableObjId = EnableObjId ? false : true;
	Invalidate(0);
}


void CQROQSView::OnViewOldToken() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	EnableOldToken = EnableOldToken ? false : true;
	Invalidate(0);
}



void CQROQSView::OnComIndata() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	/*
	CTokenfuncDlg getDlg;
	getDlg.m_input.Empty();
	getDlg.m_str = "ﾄﾗﾝｼﾞｼｮﾝIDを指定";
	if(getDlg.DoModal()!=IDOK) return;
	int tid = atoi((LPCTSTR)getDlg.m_input);
	*/

//	TRACE("t %d\n",PutIt);
	int tid = PutIt;
	if(bsp->TranMap.find(tid)==bsp->TranMap.end()) return;

	bspTransition *tran = bsp->TranMap[tid];

	multimap<int,int>::iterator aioit;
	
	if(bsp->PlaceMap[bsp->ArcMap[bsp->ArcOutMap.lower_bound(tid)->second].In]->GetTokenFlag()){
		Trace("入力ﾃﾞｰﾀ\n");
		aioit = bsp->ArcOutMap.lower_bound(tid);
		for(;aioit!=bsp->ArcOutMap.upper_bound(tid);++aioit){
			Trace("%s	: %d\n",bsp->ArcMap[aioit->second].value_name.c_str(), bsp->PlaceMap[bsp->ArcMap[aioit->second].In]->GetTokenValue());
		}
	}
	else{
		Trace("出力ﾃﾞｰﾀ\n");
		aioit = bsp->ArcInMap.lower_bound(tid);
		for(;aioit!=bsp->ArcInMap.upper_bound(tid);++aioit){
			Trace("%s	: %d\n",bsp->ArcMap[aioit->second].value_name.c_str(), bsp->PlaceMap[bsp->ArcMap[aioit->second].Out]->GetTokenValue());
		}
	}

}


//////////////アップデートメッセージ ///////////////////////



void CQROQSView::OnUpdateObjPlace(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_PLACE);
}

void CQROQSView::OnUpdateObjTransition(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_TRANSITION);
}

void CQROQSView::OnUpdateObjUhzi(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_UHZI);
}

void CQROQSView::OnUpdateObjKemmakix(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_KEMMAKIX);
}

void CQROQSView::OnUpdateObjBtoken(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_BTOKEN);
}

void CQROQSView::OnUpdateObjArc(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_ARC);
}

void CQROQSView::OnUpdateComChain(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_CHAIN);
}


void CQROQSView::OnUpdateObjModule(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_MODULE);
}



void CQROQSView::OnUpdateObjHand(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_HAND);
}

void CQROQSView::OnUpdateObjDelete(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_DELETE);
}


void CQROQSView::OnUpdateComClear(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateComWrite(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}
void CQROQSView::OnUpdateComOpen(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateComArcsort(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(false);
}

void CQROQSView::OnUpdateViewObjId(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck((EnableObjId==1) ? true:false);
}

void CQROQSView::OnUpdateViewOldToken(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->SetCheck((EnableOldToken==1) ? true:false);
}
/// EOF ///

