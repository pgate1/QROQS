// QROQSDocFile.cpp : CQROQSDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CQROQSDocFile

#include"QROQSView.h"
#include"other.h"
BOOL CQROQSDoc::SimHalt()
{
	if(PetMode!=_EDIT){
		if(AfxMessageBox("ｼﾐｭﾚｰｼｮﾝ中です．\n中断しますか？",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)
			==IDCANCEL){
			return false;
		}
		POSITION p= GetFirstViewPosition();
		CQROQSView *pView = (CQROQSView *)GetNextView(p);
		pView->SendMessage(WM_COMMAND,ID_COM_HALT);
	}
	return true;
}

void CQROQSDoc::InitTreeView()
{

	CTreeCtrl *pModTree = GetModuleTree();
	pModTree->DeleteAllItems();

	POSITION p = GetFirstViewPosition();
	CQROQSView *pView = (CQROQSView *)GetNextView(p);
	pView->bsp_tree = &bsp_tree;
	pView->bsp = bsp_tree.GetRootBSP();
	pView->viewDiam = pView->bsp->viewDiam;
	pView->SendMessage(WM_MOUSEWHEEL);
	pView->FitView();
	pView->InitModuleTree(bsp_tree);
	pView->ExpandAll(pModTree);
	GetModTreeDlg()->SelectRootModule();


	SetTitle(bsp_tree.GetRootName());
}

static int _hinst = 1;
//オーバーライド
BOOL CQROQSDoc::OnNewDocument()
{
	if(SimHalt()==false) return 0;

	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

//	fprintf(tfp,"CQROQSDoc::OnNewDocument %d\n",docst_);
//	TRACE("CQROQSDoc::OnNewDocument %d\n",docst_);

	//すでにウインドウが開いている状態ならばdocst_=3
	if(_hinst==1){
		//通常起動
		docst_ = 1;
	}
	else{
		//コマンド新規作成
		docst_ = 3;
	}
	_hinst=0;

	//この時点で変更保存確認はすでに行われている

	//新規分をクリア
	bsp_tree.reset();

	InitTreeView();

	return TRUE;
}

BOOL CQROQSDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	//	TRACE("on save:%s\n",lpszPathName);

	POSITION p = GetFirstViewPosition();
	CQROQSView *pView = (CQROQSView *)GetNextView(p);


	pView->bsp->Write(lpszPathName );

	pView->bsp->ModifiedFlag = 0;
	
	//成功したら1を返す
	return 1;

//	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CQROQSDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;

	//アイコンにファイルをドロップした場合、lpszPathNameには
	//	すでにファイル名が入っている。

	//TRACE("on open:%s\n",lpszPathName);
//	fprintf(tfp,"CQROQSDoc::OnOpenDocument %d\n",docst_);
//	TRACE("CQROQSDoc::OnOpenDocument %d\n",docst_);

	//すでにウインドウが開いている状態ならばdocst_=3
	if(__argc>=2 && _hinst){
	//	fprintf(tfp,"%s\n",__argv[1]);

		string fpath=lpszPathName;
		fpath.erase( fpath.rfind('\\') );
		filepath = fpath.c_str();

		docst_=2;
	}
	else{
		docst_ = 3;
	}
	_hinst=0;

	try{
		BSPetriTree bspt;
		bspt.Open(lpszPathName);
		bsp_tree = bspt;
	//	TRACE("set %d\n",bsp_tree.bsps.size());
	}
	catch(char *err){
//		fprintf(tfp,"%s\n",err);
//		TRACE("error %s\n",err);
		if(docst_==2){
			//アイコンにドラッグして開けなかった
			filepath.Empty();
		}
		else{
			throw err;
		}
	}


	InitTreeView();


	return 1;
}

void CQROQSDoc::OnFileOpen()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	//	TRACE("file open doc\n");
	if(SimHalt()==false) return;

	if(SaveModifiedBSP()==IDCANCEL) return;

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
	
	//最後に\\があってもなくてもいい
	fd.m_ofn.lpstrInitialDir = filepath;	//初期ディレクトリ
	
	if(fd.DoModal() != IDOK) return;
		
	CString filename = fd.GetPathName();
	//	TRACE("file name:%s %d\n",filepath,filepath.GetLength());
	string fpath=(LPCTSTR)filename;
	fpath.erase( fpath.rfind('\\') );
	filepath = fpath.c_str();
//	TRACE("file path:%s\n",filepath);

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	TraceClear();
	try{
		OnOpenDocument((LPCTSTR)filename);
	}
	catch(char * err){
//		TRACE("err\n");
//		top_bsp.PetClear();
		Trace("%s %s\n",(LPCTSTR)filename,err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return;
	}

	Trace("%s : 開きました．\r\n",(LPCTSTR)filename);

	SetTitle(bsp_tree.GetRootName());
	
	UpdateAllViews(NULL);
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
}

int CQROQSDoc::OnFileSave() //上書き保存
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
//	Trace("CQROQSDoc::OnFileSave %s\n",(LPCTSTR)filepath);

	if(filepath.IsEmpty() || GetTitle()=="untitle"){
//		TRACE("no name\n");
		return OnFileSaveAs();
	}

	// 既にファイルが存在するか確認
	CString filename;
	filename.Format("%s\\%s.bsp",filepath,GetTitle());
//	TRACE("%s\n",(LPCTSTR)filename);
	if(GetFileAttributes(filename)==0xFFFFFFFF){//ファイルがない
//		TRACE("no file %s\n",filename);
		return OnFileSaveAs();
	}

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	try{
		OnSaveDocument(filename);
	}
	catch(char * err){
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return 1;
	}
	Trace(filename + " : 上書き保存しました．\r\n");
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除

	return 0;
}

int CQROQSDoc::OnFileSaveAs() //名前を付けて保存
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	//	TRACE("file save as doc\n");

	if(GetTitle()=="untitle"){
		if(AfxMessageBox("ﾙｰﾄBSP名を設定して保存してください．",MB_OKCANCEL|MB_ICONWARNING|MB_DEFBUTTON1)
			==IDCANCEL){
			return 1;
		}
		POSITION p = GetFirstViewPosition();
		CQROQSView *pView = (CQROQSView *)GetNextView(p);
		pView->SendMessage(WM_COMMAND,ID_BSP_PROPATY);
		return 1;
	}

	
	CFileDialog fd(
		false,	//保存
		"bsp",	//デフォルトの拡張子
		GetTitle()+".bsp",	//デフォルトのファイル名
		OFN_OVERWRITEPROMPT|
		OFN_HIDEREADONLY|
		OFN_LONGNAMES,	//フラグ
		"ﾋﾞｯﾄｼﾘｱﾙﾍﾟﾄﾘﾈｯﾄ (*.bsp)|*.bsp|"
		"全てのﾌｧｲﾙ (*.*)|*.*||",	//フィルタ
		NULL	//親ウィンドウ
		);
	
	fd.m_ofn.lpstrInitialDir = filepath;	//初期ディレクトリ
	
	if(fd.DoModal() != IDOK) return 1;


	CString filename = fd.GetPathName();
	//	TRACE("file name:%s %d\n",filepath,filepath.GetLength());
	string fpath=(LPCTSTR)filename;
	fpath.erase( fpath.rfind('\\') );
	filepath = fpath.c_str();

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	try{
		OnSaveDocument(filename);
	}
	catch(char * err){
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
		return 1;
	}

	Trace(filename + " : 保存しました．\r\n");

	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
	return 0;
}



void CQROQSDoc::OnFileSaveAll() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	TRACE("save all\n");

	CString get_path;
	if(getsetPath( AfxGetApp()->GetMainWnd()->m_hWnd, filepath, get_path)){
//		TRACE("getpath %s\n",get_path);
	}
	else return;

	bsp_tree.WriteAll((LPCTSTR)get_path);
	
	map<string,BSPetri*>::iterator bspit;
	bspit = bsp_tree.bsps.begin();
	for(;bspit!=bsp_tree.bsps.end();++bspit){
		Trace("%s ",bspit->first.c_str());
		bspit->second->ModifiedFlag = 0;
	}

	Trace("\n%s に一括保存しました．\n",(LPCTSTR)get_path);
	filepath = get_path;
}

void CQROQSDoc::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	
}

/// EOF///

