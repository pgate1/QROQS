// QROQSViewSim.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSView.h"

#include"other.h"



UINT CheckThreadRun(LPVOID lpParam)
{
	CQROQSView *pView = (CQROQSView*)lpParam;
	try{
		pView->bsp_tree->Compile();
	}
	catch(char *err){
		pView->err_str = err;
		pView->compOk = false;
		return 0;
	}

	pView->compOk = true;

	return 0;
}

void CQROQSView::OnComCompile()
//ペトリネットのコンパイル
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	

	if(PetMode==_SIMULATION){
		// ｼﾐｭﾚｰｼｮﾝのリセット

		sim_uk.clear();
		bsp_tree->PetSimReset();
		bsp_tree->PetSimUKReset();

		sim_token.clear();
		sim_token.push_back( sim_init_token );
		OnComStepUndo();

		Invalidate(0);
		PetInvalidate();
		st2 = 0;
		sim_pm = 1;
		sim_end = 0;
		KillTimer(_TID_PETSIM);
		SetTimer(_TID_PETSIM, 100, NULL); //作成

		Trace("ﾘｾｯﾄしました．\n");
	
		simGo = 0;
		SimAnime = 1;

		return;
	}

	AfxGetApp()->DoWaitCursor(1); //マウスカーソル砂時計

	TraceClear();
	viewReset();
	
	if(confCompileSave){
		StatusOut("ﾌｧｲﾙへ保存しています...");
		SendMessage(WM_COMMAND,ID_FILE_SAVE);
	}
	
	StatusOut("ﾁｪｯｸ中です");
	Trace("ﾁｪｯｸ中...\r\n");
	
	PetMode = _SIMULATION;
	compOk = false;
	
	simGo = false;
	bsp->ErrPos = 0;
	SimAnime = 1;
	SimTurn = 0;
	
	bsp_tree->NormalizeGlobalRect();

	KillTimer(_TID_OBJBEE);

	bsp_tree->confSimBinValue = confSimBinValue;
	bsp_tree->AttachProg(ProgSet);

	err_str.erase();

	// チェックスレッド起動
	bsp_tree->f_check_halt = 0;
	check_thread = AfxBeginThread(CheckThreadRun, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	check_thread->m_bAutoDelete = FALSE;

	SetTimer(_TID_CHECK, 100, NULL);
}

void CQROQSView::CheckEnd()
{
	if(compOk){
		// 初期状態を保存
		SimSave(1);
		sim_init_token = sim_token.front();
		
		sim_token.clear();
		sim_uk.clear();
	}
	else{
		Trace(err_str.c_str());

		PetMode = _EDIT;

		bsp_tree->HaltSimulation();

		if(!bsp_tree->err_mname.empty()){
		//	TRACE("err mname %s\n",bsp_tree->err_mname.c_str());
			CModuleTreeDlgBar* pDlgBar = GetModTreeDlg();
			pDlgBar->SelectModule(bsp_tree->err_mname.c_str());
		}

		//コンパイルでエラーが起きたオブジェクトを示す
//		TRACE("err %d\n",bsp->vErrPos);
		if(bsp->ObjMap.find(bsp->ErrPos)!=bsp->ObjMap.end()){
#ifndef _LIGHT
			switch(bsp->ObjMap[bsp->ErrPos].type){
			case _TRANSITION: help_id=2;
			}
#endif
			/*
			//エラーオブジェクトの場所に移動
			mst=0;
			SetTimer(_TID_FMOVE, 1, NULL); //作成
*/
		
			//エラー表示
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 20, NULL); //作成
		}

		// verilogout中断
		KillTimer(_TID_VOUT_CHECK);
		// C out中断
		KillTimer(_TID_COUT_CHECK);
		// Mod Expand All 中断
		KillTimer(_TID_MEA_CHECK);
		// Research 中断
		KillTimer(_TID_RES_CHECK);
	}
//		TRACE("compOk : %d\n",compOk);
	
	//再描画
	Invalidate(0);
	if(compOk){

		//現在のツリーを選択するために、一旦rootを選択している
		//ルートのアドレスは同じだからこれでいい
		HTREEITEM cur_item = current_item;
		GetModTreeDlg()->SelectRootModule();
		GetModuleTree()->SelectItem(cur_item);
		
		PetInvalidate();
		st2 = 0;
		sim_pm = 1;
		sim_end = 0;
		SetTimer(_TID_PETSIM, 100, NULL); //作成
		
		Trace("Total Transition Size : %d\r\n", bsp->traNum);

		Trace("QROQS - ｴﾗｰ 0、警告 0\r\n");
		StatusOut("ｼﾐｭﾚｰｼｮﾝを実行してください");
	}
	else if(bsp_tree->f_check_halt){
		Trace("ﾁｪｯｸを中断しました．\n");
		StatusOut("ﾘｸｴｽﾄによってﾁｪｯｸを中断しました");
	}
	else{
		StatusOut("ｴﾗｰを発見しました．もう一度ｴﾃﾞｨｯﾄを行ってください");
	}


	MessageBeep(MB_ICONASTERISK);

	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
}

void CQROQSView::OnComGo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	TraceClear();

	if(Qmode) Trace("ｼｭﾐﾚｰｼｮﾝ中...");
	else Trace("ｼﾐｭﾚｰｼｮﾝを実行しています…");
	
	simGo = true;
	CQROQSDoc *pDoc = GetDocument();

	sim_st_time = time(NULL);

	if(confSimAnime && SimAnime){


#ifndef _LIGHT
		KillTimer(_TID_BEETOKEN);
#endif
		SetTimer(_TID_PETSIM, pDoc->SimSpeed, NULL); //作成
		
		st2 = 1;
	}
	else{
		AfxGetApp()->DoWaitCursor(1); //マウスカーソル砂時計
		try{
			sim_token.clear();
			sim_uk.clear();
			bsp_tree->PetSimBlast(2);
		}
		catch(char *ret){//シミュレーション停止
			PetInvalidate();
			simGo = false;
			Trace(ret);

			Trace("time %d sec",time(NULL)-sim_st_time);

			if(bsp->ObjMap.find(bsp->ErrPos)!=bsp->ObjMap.end()){
				//エラーオブジェクトの場所に移動
				//mst=0;
				//SetTimer(_TID_FMOVE, 1, NULL); //作成
				
				StatusOut("ｼﾐｭﾚｰｼｮﾝ中にｴﾗｰが発生しました．もう一度ｴﾃﾞｨｯﾄを行ってください．");
				//エラー表示
				KillTimer(_TID_OBJBEE);
				SetTimer(_TID_OBJBEE, 10, NULL); //作成
			}
		}

		MessageBeep(MB_ICONASTERISK);
		Sleep(120);
		MessageBeep(MB_ICONASTERISK/*MB_OK*/);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
	}
	
	SimAnime=1;
}


void CQROQSView::OnComBlast() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	SimAnime = 0;

	OnComGo();

}

void CQROQSView::OnComStep() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	KillTimer(_TID_OBJBEE);

	sim_st_time = time(NULL);

	TraceClear();
	st2=1;
	PetSim();
	st2=0;

}

void CQROQSView::OnComStop() 
{
	simGo = false;
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	KillTimer(_TID_PETSIM);
	SetTimer(_TID_PETSIM, 100, NULL); //作成
	//	KillTimer(_TID_BEETOKEN);

#ifndef _LIGHT
	bee_token.clear();
#endif

	PetInvalidate();
	st2 = 0;
	SimAnime=1;

}

void CQROQSView::PetSim()
{

	int ret;
	if(st2){
		st2 = 0;//タイマーが重なった場合の排他処理

		try{

			if(sim_pm==1){
				bsp_tree->PetSimTraCheck();
				PetInvalidate();
				sim_pm=2;
			}
			else if(sim_pm == 2){
				SimTurn++;
			//	TRACE("pm=2\n");

				SimSave(SimundoSize);
				bsp_tree->PetSimKemConsum();
				ret =0;
				ret += bsp_tree->PetSimUhziIndustry();

			//	TRACE("u %d\n",ret);
				SimSaveUK(SimundoSize);
			//	TRACE("k %d\n",ret);
				bsp_tree->PetSimTraFire();

				// ﾄｰｸﾝ移動エフェクト（いまいち）
				if(viewDiam>0.5) TokenMoveEffect();

			/*	
			// 発火履歴をファイルに保存
				{
					static int tt=0;
					FILE *fp;
					fp=fopen("fired_q.txt","a");
				//	fprintf(fp,"%5d:",tt);
					tt++;
					map<int,Transition>::iterator tit;
					tit = bsp->TranMap.begin();
					tit_end = bsp->TranMap.end();
					for(;tit!=tit_end;++tit){
						if(tit->second->fired_flag) fprintf(fp,"%d,",tit->first);
					}
				//	fprintf(fp,"\n");
					fclose(fp);
				}
*/

				int ftra=0;
//TRACE("st\n");
				ftra = bsp_tree->PetSimTraCheck();
		//		TRACE("%5d/%5d %3.1f %%\n",ftra,bsp_tree->GetRootBSP()->traNum,(double)ftra/bsp_tree->GetRootBSP()->traNum*100);
				ret += ftra;
				ret += bsp_tree->PetSimUhziCheck();
				ret += bsp_tree->PetSimKemCheck();
			


//TRACE("end\n");	
				PetInvalidate();

				if(ret==0){
					sim_end = 1;
					throw "ｼﾐｭﾚｰｼｮﾝを終了します．";
				}
			}
	
			st2 = 1;
		}
		catch(char *ret){//シミュレーション停止
			Trace("time %d sec",time(NULL)-sim_st_time);

			KillTimer(_TID_PETSIM);
			SimAnime=1;
			simGo = 0;
			st2 = 0;
			Trace(ret);
			SetTimer(_TID_PETSIM, 100, NULL); //シミュ稼動中作成
			if(bsp->ErrPos){
				//エラーオブジェクトの場所に移動
				//mst=0;
				//SetTimer(_TID_FMOVE, 1, NULL); //作成
				
				//エラー表示
				KillTimer(_TID_OBJBEE);
				SetTimer(_TID_OBJBEE, 50, NULL); //作成
			}
		}
	}
	else{
		PetInvalidate();
	}
	
}

void CQROQSView::TokenMoveEffect()
{
#ifndef _LIGHT
	if(simGo) return;
	if(confSimAnime==false) return;
	if(confSimTokenTrans==false) return;

	int firenum=0;
	map<int,bspTransition*>::iterator tit,tit_end;
	tit=bsp->TranMap.begin();
	tit_end = bsp->TranMap.end();
	for(;tit!=tit_end;++tit){
		if(tit->second->fired_flag==1){
			firenum += tit->second->in_place_num;
			firenum += tit->second->out_place_num;
		}
	}

	if(firenum==0) return;
	bee_token.resize(firenum);
	
	multimap<int,int>::iterator aioit;
	int i=0;
	int col;
	tit=bsp->TranMap.begin();
	tit_end = bsp->TranMap.end();
	for(;tit!=tit_end;++tit){
		if(tit->second->fired_flag==1){
			//入力アークを探す
			aioit = bsp->ArcOutMap.lower_bound(tit->first);
			
			col=bsp->PlaceMap[bsp->ArcMap[aioit->second].In]->GetOldColor();
			
			for(;aioit!=bsp->ArcOutMap.upper_bound(tit->first);++aioit){
				bee_token[i++]=BeeToken(
					bsp->ObjMap[bsp->ArcMap[aioit->second].In].gpoint-bsp->Globalpoint
					,bsp->ObjMap[tit->first].gpoint-bsp->Globalpoint
					,col);
			}
			//出力アークを探す
			aioit=bsp->ArcInMap.lower_bound(tit->first);
			for(;aioit!=bsp->ArcInMap.upper_bound(tit->first);++aioit){
				bee_token[i++]=BeeToken(
					bsp->ObjMap[tit->first].gpoint-bsp->Globalpoint
					,bsp->ObjMap[bsp->ArcMap[aioit->second].Out].gpoint-bsp->Globalpoint
					,col);
			}
		}
	}
	//				TRACE("gg %d\n",i);
	SetTimer(_TID_BEETOKEN, 10, NULL); //作成
#endif
}


void CQROQSView::SimSave(int undo_size)
{

	if(undo_size==0) return;

	struct func{
		static void sim_save(
			BSPetri* cur_bsp,
			map<BSPetri*, map<int,bspPlace> > &prev_token)
		{
			map<int,bspPlace*>::iterator pit,pit_end;
			pit = cur_bsp->PlaceMap.begin();
			pit_end = cur_bsp->PlaceMap.end();
			for(;pit!=pit_end;++pit){
				prev_token[ cur_bsp ][pit->first] = *(pit->second);
			}

			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sim_save(bspit->second, prev_token);
			}
		}
	};

	map<BSPetri*, map<int,bspPlace> > prev_token;
	func::sim_save(bsp_tree->GetRootBSP(), prev_token);
	sim_token.push_back( prev_token );
	if(sim_token.size() > undo_size){
		sim_token.pop_front();
	}

}

void CQROQSView::SimSaveUK(int undo_size)
{

	if(undo_size==0) return;

	struct func{
		static void sim_save(
			BSPetri* cur_bsp,
			map<BSPetri*, set<int> > &prev_uk)
		{
			map<int,bspUhzi*>::iterator uit,uit_end;
			uit = cur_bsp->UhziMap.begin();
			uit_end = cur_bsp->UhziMap.end();
			for(;uit!=uit_end;++uit){
				if(uit->second->fired==1){
			//		TRACE("save %d\n",uit->first);
					prev_uk[cur_bsp].insert(uit->first);
				}
			}
			map<int,bspKemmakix*>::iterator kit,kit_end;
			kit = cur_bsp->KemMap.begin();
			kit_end = cur_bsp->KemMap.end();
			for(;kit!=kit_end;++kit){
				if(kit->second->fired==1){
					prev_uk[cur_bsp].insert(kit->first);
				}
			}
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sim_save(bspit->second, prev_uk);
			}
		}
	};

	map<BSPetri*, set<int> > prev_uk;
	func::sim_save(bsp_tree->GetRootBSP(), prev_uk);
	sim_uk.push_back( prev_uk );
	if(sim_uk.size() > undo_size){
		sim_uk.pop_front();
	}

}


void CQROQSView::OnComStepUndo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	struct func{
		static void sim_undo(
			BSPetri* cur_bsp,
			map<BSPetri*, map<int,bspPlace> > &prev_token)
		{
			//	mapを丸ごとコピーしようとすると、コピーコンストラクタを
			//	作っていてもアドレスが変わってしまうので
			//	いちいちコピーしないといけない？
			map<int,bspPlace>::iterator pit,pit_end;
			pit = prev_token[cur_bsp].begin();
			pit_end = prev_token[cur_bsp].end();
			for(;pit!=pit_end;++pit){
				//マスタープレースのみコピー
				if(cur_bsp->PlaceMap[pit->first]==cur_bsp->PlaceMap[pit->first]->GetRealPlace()){
					*(cur_bsp->PlaceMap[ pit->first ]) = pit->second;
				}
			}

			// ﾄﾗﾝｼﾞｼｮﾝの発火状況をリセット
			map<int,bspTransition*>::iterator tit,tit_end;
			tit = cur_bsp->TranMap.begin();
			tit_end = cur_bsp->TranMap.end();
			for(;tit!=tit_end;++tit){
				tit->second->FireableFlag = false;
			}

			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sim_undo(bspit->second,prev_token);
			}
		}
		static void sim_undo(
			BSPetri* cur_bsp,
			map<BSPetri*, set<int> > &prev_uk)
		{

			set<int>::iterator sit,sit_end;
			sit = prev_uk[cur_bsp].begin();
			sit_end = prev_uk[cur_bsp].end();
			for(;sit!=sit_end;++sit){
				switch(cur_bsp->ObjMap[*sit].type){
				case _UHZI:
			//		TRACE("undo %d\n",*sit);
					cur_bsp->UhziMap[*sit]->UndoIndustry();
					break;
				case _KEMMAKIX:
					cur_bsp->KemMap[*sit]->UndoConsum();
					break;
				default:
					break;
				}
			}

			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sim_undo(bspit->second,prev_uk);
			}
		}
	};

	if(sim_token.size()){
		func::sim_undo(bsp_tree->GetRootBSP(), sim_token.back());
		sim_token.pop_back();
	}

	if(sim_uk.size()){
		func::sim_undo(bsp_tree->GetRootBSP(), sim_uk.back());
		sim_uk.pop_back();
	}

	sim_end = 0;
	sim_pm=1;
	OnComStep();

}



void CQROQSView::OnComHalt() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	if(check_thread){
		bsp_tree->f_check_halt = 1;
		return;
	}


	KillTimer(_TID_PETSIM);

#ifndef _LIGHT
	KillTimer(_TID_BEETOKEN);
	bee_token.clear();
#endif

	CRect grect;
	CPoint gpoint;
	grect = bsp->globalRect;
	gpoint = bsp->Globalpoint;
	viewDiam = bsp->viewDiam;
	bsp = bsp_tree->GetBSP(bsp->module_name.c_str());
	bsp->globalRect=grect;
	bsp->Globalpoint = gpoint;
	OnMouseWheel(0,0,0);
	bsp_tree->HaltSimulation();


	PetMode = _EDIT;
	simGo = false;
	compOk = false;

	SimAnime = 1;

	sim_token.clear();
	sim_uk.clear();


	StatusOut("ﾚﾃﾞｨ");
	TraceClear();

	viewReset();

	Invalidate(0);
}



void CQROQSView::OnUpdateComCompile(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(!check_thread);
}

void CQROQSView::OnUpdateComGo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(compOk && !simGo && sim_end==0);
}

void CQROQSView::OnUpdateComStep(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(compOk && !simGo && sim_end==0);
}

void CQROQSView::OnUpdateComStop(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(compOk && simGo);
}

void CQROQSView::OnUpdateComBlast(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(compOk && !simGo && sim_end==0);
}

void CQROQSView::OnUpdateComStepUndo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(sim_token.size() && !simGo);
}

void CQROQSView::OnUpdateComHalt(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable(PetMode==_SIMULATION);
}

/// EOF ///
