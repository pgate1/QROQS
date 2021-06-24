// QROQSViewExport.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"
#include "QROQSView.h"

#include "other.h"

void CQROQSView::OnComOfflineHelp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

//	LPCTSTR pURL = "help/index.html";
//	LPCTSTR pURL = "http://oguri-195.cis.nagasaki-u.ac.jp/~sakamoto/QROQS/QROQS.html";
	LPCTSTR pURL = "http://pgate1.at-ninja.jp/QROQS/QROQS.html";
    ::ShellExecute( 0, _T("open"), pURL, 0, 0, SW_SHOWNORMAL);

}


#ifndef _LIGHT
#include "OutVerilogDlg.h"
#endif

void CQROQSView::OnFileOutVerilog() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

//	AfxMessageBox("現在この機能は改定中であるため、利用できません。開発者をせかしてください",MB_OK);
//	return;

#ifdef _LIGHT
	AfxMessageBox("Lightﾊﾞｰｼﾞｮﾝのため機能制限されています．",MB_OK);
#else


	CQROQSDoc *pDoc= GetDocument();

	COutVerilogDlg vgDlg;
	vgDlg.m_pDoc = pDoc;
	vgDlg.m_OverPlus = 0.0;
	vgDlg.m_ModuleName = bsp_tree->GetRootName();
	if(vgDlg.DoModal()==IDCANCEL){
		return;
	}

	bsp->delay_slack=vgDlg.m_OverPlus;
	bsp->out_sync = vgDlg.m_VSync;
	bsp->v_debug = vgDlg.m_VDebug;

	if(::GetFileAttributes(pDoc->verilog_out_dir_list.front()) == 0xffffffff){
		Trace("ﾌｫﾙﾀﾞが存在しません．");
		return;
	}

	vout_dir=pDoc->verilog_out_dir_list.front() + "\\" + bsp->module_name.c_str();

	if(::GetFileAttributes(vout_dir) != 0xffffffff){
		CString cs;
		cs.Format("%s : 上書きします．\nよろしいですか？",bsp_tree->GetRootName());
		if(AfxMessageBox(cs,MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
			return;
		}
	}

	CreateDirectory(vout_dir,NULL);

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	//一旦コンパイル
	OnComCompile();
	SetTimer(_TID_VOUT_CHECK, 100, NULL);

#endif

}


#ifndef _LIGHT
// 未スレッド化
UINT FileOutVerilogRun(LPVOID lpParam)
{
	CQROQSView *pView = (CQROQSView*)lpParam;

	CQROQSDoc *pDoc= pView->GetDocument();
	CString csOptModule, csOptTrafunc;
	csOptModule = pDoc->verilog_module_option;
	csOptModule.Remove('\r');
	csOptTrafunc = pDoc->verilog_trafunc_option;
	csOptTrafunc.Remove('\r');

	BSPetriTree v_bsp_tree;
	v_bsp_tree = *(pView->bsp_tree);

	try{
		v_bsp_tree.OutVerilog(pView->vout_dir,(LPCTSTR)csOptModule,
			(LPCTSTR)csOptTrafunc,Trace,ProgSet);
/*
// デバグ用　展開したものを表示する
		string s = bsp->module_name;
		pDoc->bsp_tree = v_bsp_tree;
		bsp = pDoc->bsp_tree.GetBSP(s.c_str());
		pos_ = 1;
*/
		Trace("Verilog-HDLを出力しました．\r\n");
		return 0;
	}
	catch(char * err){
		//TRACE("err\n");
		Trace("%s\n",err);
		/*
		if(bsp->vObjMap.find(bsp->vErrPos)!=bsp->vObjMap.end()){
			//エラーオブジェクトの場所に移動
			//mst=0;
			//SetTimer(_TID_FMOVE, 1, NULL); //作成
			
			StatusOut("Verilog-HDLに出力中にエラーが発生しました。もう一度エディットを行ってください");
			//エラー表示
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 10, NULL); //作成
		}
*/
	}

	return 0;
}
#endif


#ifndef _LIGHT
void CQROQSView::FileOutVerilogPost()
{
	if(compOk == false) return;
	OnComHalt();
	
	/*
	// チェックスレッド起動
	verilog_thread = AfxBeginThread(FileOutVerilogRun, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	verilog_thread->m_bAutoDelete = FALSE;
*/
	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	FileOutVerilogRun((LPVOID)this);

	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計
}
#endif



#ifndef _LIGHT
#include "OutCDlg.h"
#endif

void CQROQSView::OnFileOutC() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

#ifdef _LIGHT
	AfxMessageBox("Lightﾊﾞｰｼﾞｮﾝのため機能制限されています．",MB_OK);

#else

	CQROQSDoc *pDoc= GetDocument();
	cout_Dlg.m_pDoc = pDoc;
	cout_Dlg.m_ModuleName = bsp_tree->GetRootName();
	if(bsp_tree->output_max==0) bsp_tree->output_max = KEM_DATA_MAX;
	cout_Dlg.m_KemMax = bsp_tree->output_max;
	if(cout_Dlg.DoModal()==IDCANCEL){
		return;
	}

	bsp_tree->output_max = cout_Dlg.m_KemMax;

	if(::GetFileAttributes(pDoc->c_out_dir_list.front()) == 0xffffffff){
		Trace("ﾌｫﾙﾀﾞが存在しません．");
		return;
	}

	cout_dir = pDoc->c_out_dir_list.front() + "\\" + bsp->module_name.c_str();

	if(::GetFileAttributes(cout_dir) != 0xffffffff){
		CString cs;
		cs.Format("%s : 上書きします．\nよろしいですか？",bsp_tree->GetRootName());
		if(AfxMessageBox(cs,MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
			return;
		}
	}
	CreateDirectory(cout_dir,NULL);

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	//一旦コンパイル
	OnComCompile();

	SetTimer(_TID_COUT_CHECK, 100, NULL);

#endif
}

#ifndef _LIGHT
void CQROQSView::FileOutCPost()
{

	if(compOk == false) return;
	OnComHalt();

	time_t out_st;
	out_st = time(NULL);

	BSPetriTree c_bsp_tree;

	c_bsp_tree = *bsp_tree;

//	TRACE("%s\n",dir);

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計
	try{
		c_bsp_tree.OutC(cout_dir, cout_Dlg.m_MainOut, cout_Dlg.m_QROQSOut, cout_Dlg.m_MakeOut, Trace);

/*
// デバグ用　展開したものを表示する
		string s = bsp->module_name;
		pDoc->bsp_tree = c_bsp_tree;
		bsp = pDoc->bsp_tree.GetBSP(s.c_str());
		pos_ = 1;
*/

		Trace("C言語記述を出力しました．\r\n");
	}
	catch(char * err){
		//TRACE("err\n");
		Trace("%s\n",err);
		/*
		if(bsp->vObjMap.find(bsp->vErrPos)!=bsp->vObjMap.end()){
			//エラーオブジェクトの場所に移動
			//mst=0;
			//SetTimer(_TID_FMOVE, 1, NULL); //作成
			
			StatusOut("Verilog-HDLに出力中にエラーが発生しました。もう一度エディットを行ってください");
			//エラー表示
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 10, NULL); //作成
		}
*/
	}
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除

	Trace("Time : %d\n",time(NULL)-out_st);

}
#endif


/// EOF ///
