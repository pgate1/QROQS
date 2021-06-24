// QROQSViewInst.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"



IMPLEMENT_DYNCREATE(CQROQSView, CView)

BEGIN_MESSAGE_MAP(CQROQSView, CView)
	//{{AFX_MSG_MAP(CQROQSView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_OBJ_ARC, OnObjArc)
	ON_COMMAND(ID_OBJ_BTOKEN, OnObjBtoken)
	ON_COMMAND(ID_OBJ_HAND, OnObjHand)
	ON_COMMAND(ID_OBJ_KEMMAKIX, OnObjKemmakix)
	ON_COMMAND(ID_OBJ_PLACE, OnObjPlace)
	ON_COMMAND(ID_OBJ_TRANSITION, OnObjTransition)
	ON_COMMAND(ID_OBJ_UHZI, OnObjUhzi)
	ON_COMMAND(ID_COM_STEP, OnComStep)
	ON_UPDATE_COMMAND_UI(ID_COM_STEP, OnUpdateComStep)
	ON_COMMAND(ID_COM_COMPILE, OnComCompile)
	ON_UPDATE_COMMAND_UI(ID_COM_COMPILE, OnUpdateComCompile)
	ON_COMMAND(ID_COM_GO, OnComGo)
	ON_WM_TIMER()
	ON_COMMAND(ID_COM_STOP, OnComStop)
	ON_UPDATE_COMMAND_UI(ID_OBJ_TRANSITION, OnUpdateObjTransition)
	ON_UPDATE_COMMAND_UI(ID_OBJ_UHZI, OnUpdateObjUhzi)
	ON_UPDATE_COMMAND_UI(ID_OBJ_KEMMAKIX, OnUpdateObjKemmakix)
	ON_UPDATE_COMMAND_UI(ID_OBJ_ARC, OnUpdateObjArc)
	ON_UPDATE_COMMAND_UI(ID_OBJ_BTOKEN, OnUpdateObjBtoken)
	ON_UPDATE_COMMAND_UI(ID_OBJ_HAND, OnUpdateObjHand)
	ON_COMMAND(ID_COM_CLEAR, OnComClear)
	ON_UPDATE_COMMAND_UI(ID_COM_STOP, OnUpdateComStop)
	ON_UPDATE_COMMAND_UI(ID_COM_CLEAR, OnUpdateComClear)
	ON_COMMAND(ID_COM_HALT, OnComHalt)
	ON_UPDATE_COMMAND_UI(ID_COM_HALT, OnUpdateComHalt)
	ON_WM_LBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_COMMAND(ID_COM_WRITE, OnComWrite)
	ON_COMMAND(ID_COM_OPEN, OnComOpen)
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_COM_CONFIG, OnComConfig)
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_COM_OPEN, OnUpdateComOpen)
	ON_UPDATE_COMMAND_UI(ID_COM_WRITE, OnUpdateComWrite)
	ON_UPDATE_COMMAND_UI(ID_COM_GO, OnUpdateComGo)
	ON_COMMAND(ID_OBJ_DELETE, OnObjDelete)
	ON_UPDATE_COMMAND_UI(ID_OBJ_DELETE, OnUpdateObjDelete)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI(ID_COM_ARCSORT, OnUpdateComArcsort)
	ON_UPDATE_COMMAND_UI(ID_COM_ALIGN, OnUpdateComAlign)
	ON_COMMAND(ID_TRA_FUNCTION,OnTraFunction)
	ON_COMMAND(ID_UHZI_FUNCTION,OnUhziFunction)
	ON_COMMAND(ID_KEM_FUNCTION,OnKemFunction)
	ON_COMMAND(ID_MOD_FUNCTION,OnModFunction)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_COM_ALIGN,OnComAlign)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE_HORZ, OnEditPasteHorz)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_HORZ, OnUpdateEditPasteHorz)
	ON_COMMAND(ID_EDIT_PASTE_VERT, OnEditPasteVert)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_VERT, OnUpdateEditPasteVert)
	ON_WM_CHAR()
	ON_COMMAND(ID_COM_ZOOMUP, OnComZoomup)
	ON_COMMAND(ID_COM_ZOOMDOWN, OnComZoomdown)
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_COM_CHAIN, OnComChain)
	ON_UPDATE_COMMAND_UI(ID_COM_CHAIN, OnUpdateComChain)
	ON_COMMAND(ID_FILE_OUT_VERILOG, OnFileOutVerilog)
	ON_COMMAND(ID_VIEW_OBJ_ID, OnViewObjId)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJ_ID, OnUpdateViewObjId)
	ON_COMMAND(ID_VIEW_OLD_TOKEN, OnViewOldToken)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OLD_TOKEN, OnUpdateViewOldToken)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_COM_STEP_UNDO, OnComStepUndo)
	ON_UPDATE_COMMAND_UI(ID_COM_STEP_UNDO, OnUpdateComStepUndo)
	ON_COMMAND(ID_COM_HELP, OnComHelp)
	ON_COMMAND(ID_OBJ_MODULE, OnObjModule)
	ON_COMMAND(ID_COM_DEMO, OnComDemo)
	ON_UPDATE_COMMAND_UI(ID_COM_DEMO, OnUpdateComDemo)
	ON_WM_SETCURSOR()
	ON_UPDATE_COMMAND_UI(ID_OBJ_MODULE, OnUpdateObjModule)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PLACE, OnUpdateObjPlace)
	ON_COMMAND(ID_MODULE_PROPATY, OnModulePropaty)
	ON_UPDATE_COMMAND_UI(ID_MODULE_PROPATY, OnUpdateModulePropaty)
	ON_COMMAND(ID_MODULE_READ, OnModuleRead)
	ON_UPDATE_COMMAND_UI(ID_MODULE_READ, OnUpdateModuleRead)
	ON_COMMAND(ID_MODULE_UP, OnModuleUp)
	ON_UPDATE_COMMAND_UI(ID_MODULE_UP, OnUpdateModuleUp)
	ON_COMMAND(ID_COM_SELECTALL, OnComSelectall)
	ON_COMMAND(ID_COM_BACKCONF, OnComBackconf)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_COM_OBJLAP, OnComObjlap)
	ON_COMMAND(ID_BSP_PROPATY, OnBspPropaty)
	ON_COMMAND(ID_COM_BLAST, OnComBlast)
	ON_UPDATE_COMMAND_UI(ID_COM_BLAST, OnUpdateComBlast)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_MOD_MANAGER, OnModManager)
	ON_COMMAND(ID_FILE_OUT_C, OnFileOutC)
	ON_COMMAND(ID_COM_OFFLINE_HELP, OnComOfflineHelp)
	ON_COMMAND(ID_COM_ZOOMRECT, OnComZoomrect)
	ON_UPDATE_COMMAND_UI(ID_COM_SELECTALL, OnUpdateComSelectall)
	ON_COMMAND(ID_COM_PLACE_FUSION, OnComPlaceFusion)
	ON_UPDATE_COMMAND_UI(ID_COM_PLACE_FUSION, OnUpdateComPlaceFusion)
	ON_COMMAND(ID_COM_SKIN_SAVE, OnComSkinSave)
	ON_COMMAND(ID_COM_TEXT, OnComText)
	ON_COMMAND(ID_MOD_EXPANDALL, OnModExpandall)
	ON_COMMAND(ID_COM_RESEARCH, OnComResearch)
	ON_COMMAND(ID_COM_INDATA, OnComIndata)
	ON_COMMAND(ID_COM_PLACE_FISSION, OnComPlaceFission)
	ON_COMMAND(ID_COM_MEMO, OnComMemo)
	ON_COMMAND(ID_COM_JOINTDATA, OnComJointdata)
	//}}AFX_MSG_MAP

	//ツールチップ用
	ON_NOTIFY( TTN_GETDISPINFO, 0, OnTipDispInfo )

	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	// skinメニュー
	ON_COMMAND_RANGE(IDM_SKINS,IDM_SKINS+IDM_SKINS_RANGE-1, OnSkinMenu)


END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CQROQSView クラスの構築/消滅

#include "other.h"

CQROQSView::CQROQSView()
{
	// TODO: この場所に構築用のコードを追加してください。
//	fprintf(tfp,"CQROQSView::CQROQSView\n");
//	TRACE("CQROQSView::CQROQSView\n");
	

	Posface = NULL;
	
	varcBwt.clear();
	viewReset();
	
	zoomrect_flag = 0;

	
	CWinApp* pApp=AfxGetApp();
	HatchSize = pApp->GetProfileInt("View","HatchSize",V_CHIP_SIZE);
	confCompileSave = pApp->GetProfileInt("Compile","CompileSave",false);
	confSimAnime = pApp->GetProfileInt("Simulation","SimAnime",true);
	confDataViewMax = pApp->GetProfileInt("Simulation","ViewDataMax",16);
	UndoSize = pApp->GetProfileInt("Edit","UndoSize",3);
	confEditAlwaysAlign = pApp->GetProfileInt("Edit","AlwaysAlign",FALSE);
	confEditGridType = pApp->GetProfileInt("Edit","GridType",0);

	confSimTokenMove = pApp->GetProfileInt("Simulation","TokenMove",false);
	confSimTokenTrans = pApp->GetProfileInt("Simulation","TokenTrans",false);
	confSimBinValue = pApp->GetProfileInt("Simulation","BinValue",false);
	SimundoSize = pApp->GetProfileInt("Simulation","SimundoSize",30);
	EnableObjId = pApp->GetProfileInt("View","EnableObjId",0);
	EnableOldToken= pApp->GetProfileInt("View","EnableOldToken",0);

	CString cs;
	cs = pApp->GetProfileString("View","diamDelta","0.5");
	diamDelta = atof((LPCTSTR)cs);
	cs = pApp->GetProfileString("View","diamDist","1.0");
	diamDist = atof((LPCTSTR)cs);

	/*
	// ﾌｫﾝﾄの読み込み
	LOGFONT lf;
	strcpy(lf.lfFaceName, pApp->GetProfileString("Font","FaceName",""));	// 書体名
	lf.lfHeight = pApp->GetProfileInt("Font","Size",17);	// サイズ
	lf.lfWeight = pApp->GetProfileInt("Font","Weight", 700);	// 太さ
	int opt;
	opt = pApp->WriteProfileInt("Font","Option",5);
	lf.lfItalic     = opt & 1; opt >>= 1;
	lf.lfStrikeOut  = opt & 1; opt >>= 1;
	lf.lfUnderline  = opt & 1; opt >>= 1;
	lf.lfEscapement = opt & 1;
	vcFont.CreateFontIndirect(&lf);
*/

	SkinDir = pApp->GetProfileString("View","SkinDir","Default");

	vColMap[_BACK_COL] = pApp->GetProfileInt("View","vColBack",RGB(255,235,195));
	vColMap[_ARC_COL] = pApp->GetProfileInt("View","vColArc",RGB(100,255,100));
	vColMap[_PLACE_COL] = pApp->GetProfileInt("View","vColPlace",RGB(100,255,100));
	vColMap[_PASSTRA_COL] = pApp->GetProfileInt("View","vColTransition",RGB(100,100,255));
	vColMap[_TRAN_COL] = pApp->GetProfileInt("View","vColFuncTransition",RGB(255,0,128));
	vColMap[_UHZI_COL] = pApp->GetProfileInt("View","vColUhzi",RGB(255,128,0));
	vColMap[_KEM_COL] =pApp->GetProfileInt("View","vColKemmakix",RGB(255,0,255));
	vColMap[_GLID_COL]=pApp->GetProfileInt("View","vColGlid",RGB(255,220,170));
	vColMap[_MOD_COL]=pApp->GetProfileInt("View","vColModule",RGB(255,100,100));
	vColMap[_TEXT_COL]=pApp->GetProfileInt("View","vColText",RGB(50,50,50));

	vColMap[_ARC_COL+_SELECTED_COL] = pApp->GetProfileInt("View","vColArcSel",RGB(255,100,100));
	vColMap[_PLACE_COL+_SELECTED_COL] = pApp->GetProfileInt("View","vColPlaceSel",RGB(255,255,100));
	vColMap[_PASSTRA_COL+_SELECTED_COL] = pApp->GetProfileInt("View","vColTranSel",RGB(255,255,100));
	vColMap[_TRAN_COL+_SELECTED_COL] = pApp->GetProfileInt("View","vColFTranSel",RGB(255,0,255));
	vColMap[_UHZI_COL+_SELECTED_COL] = pApp->GetProfileInt("View","vColUhziSel",RGB(255,0,0));
	vColMap[_KEM_COL+_SELECTED_COL] =pApp->GetProfileInt("View","vColKemSel",RGB(200,0,255));
	vColMap[_MOD_COL+_SELECTED_COL]=pApp->GetProfileInt("View","vColModSel",RGB(100,255,100));

#ifndef _LIGHT
	SkinUse=pApp->GetProfileInt("View","SkinUse",false);
	shadowDiam=1;
#endif


	confEditArcinMove = pApp->GetProfileInt("Edit","ArcInMove",0);
	confEditArcoutMove = pApp->GetProfileInt("Edit","ArcOutMove",0);

	PrintColor = pApp->GetProfileInt("Print","Color",1);

	//	TRACE("conf %d\n",confSimBlast);
	vChipSize=V_CHIP_SIZE;
	viewDiam=1.0;
	
	pen_gray1d.CreatePen(PS_DOT,1,RGB(100,100,100));
	pen_wred5.CreatePen(PS_SOLID,5,RGB(255,100,100));
	pen_wgreen5.CreatePen(PS_SOLID,5,RGB(100,255,100));
	pen_wblue5.CreatePen(PS_SOLID,5,RGB(100,100,255));

	pen_wyellow5.CreatePen(PS_SOLID,5,RGB(255,128,0));

	brush_grayh.CreateHatchBrush(HS_DIAGCROSS,RGB(100,100,100));

	vColBToken = RGB(100,100,100);
	brush_token.CreateSolidBrush(vColBToken);

	// 左上を原点として
	vpPlace[0].x=1;
	vpPlace[0].y=1;
	vpPlace[1].x=39;
	vpPlace[1].y=39;
	
	vpTra[0].x=14;
	vpTra[0].y=1;
	vpTra[1].x=26;
	vpTra[1].y=39;
	
	vpUhziKem[0].x=17;
	vpUhziKem[0].y=10;
	vpUhziKem[1].x=1;
	vpUhziKem[1].y=1;
	vpUhziKem[2].x=1;
	vpUhziKem[2].y=39;
	vpUhziKem[3].x=17;
	vpUhziKem[3].y=29;
	vpUhziKem[4].x=37;
	vpUhziKem[4].y=19;

	vpModule[0].x =2;
	vpModule[0].y =2;
	vpModule[1].x =38;
	vpModule[1].y =38;
	vpModule[2].x =17;
	vpModule[2].y =17;

	//この変数でペンの状態を判断している
	pen_arc=NULL;

	bsp=NULL;

#ifndef _LIGHT
	help_id=1;
	backlpBMP=NULL;
#endif

	bufBMP =NULL;

	PetMode=_EDIT;
	compOk=false;

	SimAnime=1;

	m_wndTraceDlg=NULL;

	rect_back_flag = 0;
	Oldpoint.x=0;
	Oldpoint.y=0;
	pasteOldpoint.x =0;
	pasteOldpoint.y =0;
	tkOldpoint.x=0;
	tkOldpoint.y=0;

	check_thread = NULL;


}

CQROQSView::~CQROQSView()
{
//	TRACE("view deth\n");


}

BOOL CQROQSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
//	fprintf(tfp,"CQROQSView::PreCreateWindow\n");
//	TRACE("CQROQSView::PreCreateWindow\n");

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CQROQSView クラスの診断

#ifdef _DEBUG
void CQROQSView::AssertValid() const
{
	CView::AssertValid();
}

void CQROQSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CQROQSDoc* CQROQSView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CQROQSDoc)));
	return (CQROQSDoc*)m_pDocument;
}
#endif //_DEBUG

void CQROQSView::viewReset(){

//	TRACE("reset\n");
	
	varcBwt.clear();

	ArcPic=FALSE;
	vArcBase=0;
	vArcFollow=0;
	
	HandlingTrack=false;
	pos_ = 1;
	
	SelectOBJ = _NONE;
	pressKey = NULL;
	
	Hungface=false;
	

	tra_io_var=0;

#ifndef _LIGHT
	Hungvgpoint=false;
	bee_token.clear();
#endif

	now_command=_NONE;

}



BOOL readBMP(char fn[],LPBITMAPINFO &lpInfo,LPBYTE &lpBMP,LPBYTE &lpBuf,int &xSize,int &ySize)
{
	DWORD offset,dummy;

	HANDLE fh;

	fh=CreateFile(fn,GENERIC_READ,0,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL); // ファイルオープン
	if(fh==INVALID_HANDLE_VALUE) return false;

	lpBuf= new unsigned char[GetFileSize(fh,NULL)]; // バッファ確保

	ReadFile(fh,lpBuf,GetFileSize(fh,NULL),&dummy,NULL);

	lpInfo=(LPBITMAPINFO)(lpBuf+sizeof(BITMAPFILEHEADER));

	offset=*(LPDWORD)(lpBuf+10);
	lpBMP=lpBuf+offset; // ビットマップバッファの先頭アドレス

	CloseHandle(fh);

	if (lpBuf[0]!='B' || lpInfo->bmiHeader.biBitCount<16) { // BMPでない
		delete[] lpBuf;
		return FALSE;
	}

	xSize=lpInfo->bmiHeader.biWidth;
	ySize=lpInfo->bmiHeader.biHeight;

	return true;
}

BOOL readBMPandSet(char fn[],HDC hdc,int to_x,int to_y,int to_width,int to_height) { // ビットマップ読み込み

	LPBITMAPINFO lpInfo;
	LPBYTE lpBMP,lpBuf;
	int xSize,ySize;

	int ret;
	ret = readBMP(fn,lpInfo,lpBMP,lpBuf,xSize,ySize);
	if(ret==false) return false;

//	TRACE("read %s\n",fn);
	StretchDIBits(hdc,to_x,to_y,to_width,to_height,0,0,xSize,ySize,lpBMP,
		lpInfo,DIB_RGB_COLORS,SRCCOPY);
	
	delete[] lpBuf;

     // 読み込んだDIBがフルカラーかつ縦横８ピクセル単位ならTRUE
	return true;//(lpInfo->bmiHeader.biBitCount==24);
}


void CQROQSView::ReadObjectSkin()
{
#ifndef _LIGHT
	char img_path[100];
	sprintf(img_path,"skin\\%s\\place.bmp",SkinDir);
	readBMPandSet(img_path,objDC.GetSafeHdc(),V_CHIP_SIZE,0,V_CHIP_SIZE,V_CHIP_SIZE);
	sprintf(img_path,"skin\\%s\\transition.bmp",SkinDir);
	readBMPandSet(img_path,objDC.GetSafeHdc(),V_CHIP_SIZE*2,0,V_CHIP_SIZE,V_CHIP_SIZE);
	sprintf(img_path,"skin\\%s\\uhzi.bmp",SkinDir);
	readBMPandSet(img_path,objDC.GetSafeHdc(),V_CHIP_SIZE*3,0,V_CHIP_SIZE,V_CHIP_SIZE);
	sprintf(img_path,"skin\\%s\\kemmakix.bmp",SkinDir);
	readBMPandSet(img_path,objDC.GetSafeHdc(),V_CHIP_SIZE*4,0,V_CHIP_SIZE,V_CHIP_SIZE);
	sprintf(img_path,"skin\\%s\\module.bmp",SkinDir);
	readBMPandSet(img_path,objDC.GetSafeHdc(),V_CHIP_SIZE*5,0,V_CHIP_SIZE,V_CHIP_SIZE);
#endif
}

void CQROQSView::ReadBackSkin()
{
#ifndef _LIGHT
	backBMP.DeleteObject();

	char back_path[100];
	sprintf(back_path,"skin\\%s\\back.bmp",SkinDir);
	HBITMAP hbm = (HBITMAP)LoadImage(NULL, back_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	BITMAP bmp;                            // BITMAP構造体
	GetObject(hbm, sizeof(BITMAP), &bmp);  // ヘッダ部をコピー
	if(hbm){
		back_size.cx = bmp.bmWidth;      // 画像の幅
		back_size.cy = bmp.bmHeight;
		backlpBMP=NULL;
	}
	else{
		back_size.cx =400;
		back_size.cy =400;

		BITMAPINFOHEADER backBmInfoHed;
		BITMAPINFO backBmInfo;
		ZeroMemory(&backBmInfoHed, sizeof(BITMAPINFOHEADER));
		backBmInfoHed.biSize=sizeof(BITMAPINFOHEADER);
		backBmInfoHed.biWidth=back_size.cx;
		backBmInfoHed.biHeight=-back_size.cy;
		backBmInfoHed.biPlanes=1;
		backBmInfoHed.biBitCount=24;
		backBmInfoHed.biCompression=BI_RGB;
		backBmInfo.bmiHeader=backBmInfoHed;

		hbm=CreateDIBSection(backDC,
			&backBmInfo,DIB_RGB_COLORS,(void**)&backlpBMP,NULL,NULL);
	}

	backBMP.Attach(hbm);
	backDC.SelectObject(&backBMP);

	if(backlpBMP!=NULL){
		backDC.FillSolidRect(0,0,back_size.cx,back_size.cy,vColMap[_BACK_COL]);
	}//back_size.cx=0;で背景画像なし
#endif
}

#ifndef TTS_BALLOON
#define TTS_BALLOON 0x41
#endif


void CQROQSView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	//	fprintf(tfp,"OnInitialUpdate\n");
	
	CQROQSDoc *pDoc = GetDocument();

	//初期化
//	fprintf(tfp,"CQROQSView::OnInitialUpdate doc %d\n",pDoc->docst_);
//	TRACE("CQROQSView::OnInitialUpdate doc %d\n",pDoc->docst_);

	TraceClear();


	bsp_tree = &(pDoc->bsp_tree);
	bsp = bsp_tree->GetRootBSP();
	current_item = GetModuleTree()->GetRootItem();
	viewDiam = bsp->viewDiam;


	undo_bsp.clear();

	if(pDoc->docst_==3){
		//すでにViewのセットは完了しているはず
		//新規作成、ファイルを開く
		pDoc->docst_=0;
		return;
	}
	pDoc->docst_=0;
//	TRACE("objs %d\n",bsp->vObjMap.size());
//	fprintf(tfp,"init view set\n");

	m_LeaveFlag = TRUE;     // マウスがウインドウにあるかどうか
	
	//ドラッグ＆ドロップ受け付け
	DragAcceptFiles(true);
	
	//ツールチップの準備をする
	m_toolTip.Create( this ,TTS_BALLOON | TTS_NOPREFIX);
	m_toolTip.AddTool( this, LPSTR_TEXTCALLBACK );
	
	RECT r;
	::SetRect( &r, 5, 5, 5, 5 );
	m_toolTip.SetMargin( &r );    //余白
	m_toolTip.SetDelayTime( TTDT_AUTOPOP, SHRT_MAX );  //出たら表示し続ける
	m_toolTip.SetDelayTime( TTDT_INITIAL, 500 );    //出るときは素早く
	//m_toolTip.SetDelayTime(TTDT_RESHOW ,2000);//つぎまで
	m_toolTip.SetTipBkColor(RGB(255,255,255));
	m_toolTip.SetMaxTipWidth(2000);
	m_toolTip.SetTipTextColor(RGB(0,0,0));
	// ツールチップのフォントｻｲｽﾞが小さすぎるのがどうにかならないか
	m_toolTip.SendMessage( TTM_SETMAXTIPWIDTH, 0, 300 );    //これがミソ



	//tracker
	tracker.m_nStyle = 
		CRectTracker::dottedLine 
		| CRectTracker::resizeOutside; // 外の枠線のスタイルを指定します

	srand(time(NULL));

/*
	// モジュールツリーにアイコンを設定
	HICON hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	CImageList *imgList = new CImageList;
	imgList->Create( 16, 16, ILC_MASK, 3, 3 );
	imgList->Add(hIcon);
	pModTree->SetImageList(imgList, TVSIL_NORMAL);
	delete imgList;
*/


	// ここではﾌｨｯﾄができないのでタイマでやる
	m_wndTraceDlg = new CTraceDlg;
	m_wndTraceDlg->Create(IDD_TRACEOUT,this);
	SetTimer(_TID_INIT, 1, NULL);

//////////////////////////////////////////////////////
////   View描画関連デバイスコンテキストの初期化
//////////////////////////////////////////////////////
	CClientDC cDC(this);

#ifndef _LIGHT
	objBMP.LoadBitmap(IDB_OBJECT);
	objDC.CreateCompatibleDC(&cDC);
	objDC.SelectObject(&objBMP);

	viewDiam = 1.0;
	OnMouseWheel(0,0,0);
	// オブジェクトの元
	objDC.FillSolidRect(0,0,V_CHIP_SIZE*6,V_CHIP_SIZE,RGB(0,0,0));
	for(int i=1;i<6;i++){
		PutObject(&objDC,V_CHIP_SIZE*i,0,i);
	}
	
	CQROQSApp *pApp = (CQROQSApp *)AfxGetApp();
	::SetCurrentDirectory(pApp->exe_dir);
	//ここでskinを読み込み
	if(SkinUse==1){
		ReadObjectSkin();
	}

	//背景
	backDC.CreateCompatibleDC(&cDC);
	ReadBackSkin();

	miniDC.CreateCompatibleDC(&cDC);
#endif

	
	//TRACE("ld\n");
	m1DC.CreateCompatibleDC(&cDC);
	bufDC.CreateCompatibleDC(&cDC);

	//ストリングの背景は透過
	m1DC.SetBkMode(TRANSPARENT);
	m1DC.SetTextColor(vColMap[_TEXT_COL]);
	bufDC.SetBkMode(TRANSPARENT);
	bufDC.SetTextColor(vColMap[_TEXT_COL]);


	//bspが結び付けられてからスクロールバーなどの初期化を行う
	OnSize(0,0,0);

	OnMouseWheel(0,0,0);

}

void CQROQSView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
//	TRACE("view OnDestroy\n");

	undo_bsp.clear();

	KillTimer(_TID_PETSIM);
	KillTimer(_TID_OBJBEE);

	CWinApp* pApp=AfxGetApp();
	pApp->WriteProfileInt("View","HatchSize",HatchSize);
	pApp->WriteProfileInt("Compile","CompileSave",confCompileSave);
	pApp->WriteProfileInt("Simulation","SimAnime",confSimAnime);
	pApp->WriteProfileInt("Edit","UndoSize",UndoSize);

	pApp->WriteProfileInt("Simulation","TokenMove",confSimTokenMove);
	pApp->WriteProfileInt("Simulation","TokenTrans",confSimTokenTrans);
	pApp->WriteProfileInt("Simulation","SimundoSize",SimundoSize);
	pApp->WriteProfileInt("Simulation","ViewDataMax",confDataViewMax);
	pApp->WriteProfileInt("Simulation","BinValue",confSimBinValue);

	pApp->WriteProfileInt("View","EnableObjId",EnableObjId);
	pApp->WriteProfileInt("View","EnableOldToken",EnableOldToken);
	pApp->WriteProfileInt("Edit","AlwaysAlign",confEditAlwaysAlign);
	pApp->WriteProfileInt("Edit","GridType",confEditGridType);

	CString cs;
	cs.Format("%f",diamDelta);
	pApp->WriteProfileString("View","diamDelta",cs);
	cs.Format("%f",diamDist);
	pApp->WriteProfileString("View","diamDist",cs);


/*
	// ﾌｫﾝﾄの保存
	LOGFONT lf;
	vcFont.GetLogFont(&lf);
	pApp->WriteProfileString("Font","FaceName", lf.lfFaceName);	// 書体名
//	pApp->WriteProfileString("Font","StyleName", lf.lfPitchAndFamily);	// スタイル名
	pApp->WriteProfileInt("Font","Size", lf.lfHeight);	// サイズ
	pApp->WriteProfileInt("Font","Weight", lf.lfWeight);	// 太さ
	int opt=0;
	opt = lf.lfEscapement;
	opt = (opt << 1) | lf.lfUnderline;
	opt = (opt << 1) | lf.lfStrikeOut;
	opt = (opt << 1) | lf.lfItalic;
	pApp->WriteProfileInt("Font","Option", opt);
*/


	pApp->WriteProfileInt("View","vColBack",vColMap[_BACK_COL]);
	pApp->WriteProfileInt("View","vColPlace",vColMap[_PLACE_COL]);
	pApp->WriteProfileInt("View","vColArc",vColMap[_ARC_COL]);
	pApp->WriteProfileInt("View","vColTransition",vColMap[_PASSTRA_COL]);
	pApp->WriteProfileInt("View","vColFuncTransition",vColMap[_TRAN_COL]);
	pApp->WriteProfileInt("View","vColUhzi",vColMap[_UHZI_COL]);
	pApp->WriteProfileInt("View","vColKemmakix",vColMap[_KEM_COL]);
	pApp->WriteProfileInt("View","vColGlid",vColMap[_GLID_COL]);
	pApp->WriteProfileInt("View","vColModule",vColMap[_MOD_COL]);
	pApp->WriteProfileInt("View","vColText",vColMap[_TEXT_COL]);

	pApp->WriteProfileInt("View","vColPlaceSel",vColMap[_PLACE_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColArcsel",vColMap[_ARC_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColTranSel",vColMap[_PASSTRA_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColFTranSel",vColMap[_TRAN_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColUhziSel",vColMap[_UHZI_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColKemSel",vColMap[_KEM_COL+_SELECTED_COL]);
	pApp->WriteProfileInt("View","vColModSel",vColMap[_MOD_COL+_SELECTED_COL]);

#ifndef _LIGHT
	pApp->WriteProfileInt("View","SkinUse",SkinUse);
#endif

	pApp->WriteProfileInt("Edit","ArcInMove",confEditArcinMove);
	pApp->WriteProfileInt("Edit","ArcOutMove",confEditArcoutMove);
	pApp->WriteProfileInt("Print","Color",PrintColor);
	pApp->WriteProfileString("View","SkinDir",SkinDir);


	CBitmap oldbmp;
	CPen oldpen;
	CBrush oldbrush;
	
#ifndef _LIGHT
	objDC.SelectObject(&oldbmp);
	DeleteObject(objBMP);
	DeleteDC(objDC);
	
	miniDC.SelectObject(&oldbmp);
	DeleteObject(miniBMP);
	miniDC.SelectObject(&oldpen);
	miniDC.SelectObject(&oldbrush);
	DeleteDC(miniDC);
#endif

	m1DC.SelectObject(&oldbmp);
	DeleteObject(m1BMP);
	m1DC.SelectObject(&oldpen);
	m1DC.SelectObject(&oldbrush);
	DeleteDC(m1DC);
	
	bufDC.SelectObject(&oldbmp);
	delete bufBMP;
	bufDC.SelectObject(&oldpen);
	bufDC.SelectObject(&oldbrush);
	DeleteDC(bufDC);
	
	
	//ペンの破棄
	pen_wred5.DeleteObject();
	pen_wblue5.DeleteObject();
	pen_wgreen5.DeleteObject();
	delete pen_hatch;


	delete brush_uhzi;
	delete brush_kemmakix;
	delete brush_module;

	delete brush_uhzi_select;
	delete brush_kemmakix_select;
	delete brush_module_select;

	//他のペンも消せ
	
	delete pen_arc;
	delete pen_place;
	delete pen_transition;
	delete pen_transition_func;
	delete pen_uhzi;
	delete pen_kemmakix;
	delete pen_module;
	delete pen_token;

	delete pen_arc_select;
	delete pen_place_select;
	delete pen_transition_select;
	delete pen_transition_func_select;
	delete pen_uhzi_select;
	delete pen_kemmakix_select;
	delete pen_module_select;
	
	if(Posface!=NULL) delete[] Posface;
}



/// EOF ///

