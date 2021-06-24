// QROQSView.h : CQROQSView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QROQSVIEW_H__00ACB2DF_AE5B_4447_A351_01FB7113D5CB__INCLUDED_)
#define AFX_QROQSVIEW_H__00ACB2DF_AE5B_4447_A351_01FB7113D5CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////  追加　　・・・・


//#define GLOBAL_VIEW_VALUE_DEFINE
#include"QROQSDoc.h"

#include"BSPetriTree.h"

#include "TraceDlg.h"

#ifndef _LIGHT
#include "OutCDlg.h"
#endif

#define V_CHIP_SIZE 40

#define IDM_SKINS_RANGE	100

#ifndef _LIGHT
//トークンの移動
class BeeToken{
public:
	CPoint inp,outp;
	int rad,wid;
	int dd,pd,sd;
	int col,cd;
	double arg;

	BeeToken(CPoint in=0,CPoint out=0,int col=0);
	CRect view(CView *vp);
	~BeeToken(){};
};
#endif

//エラー表示
class Beecir{
	int rad,wid;
	int dd,pd,sd;
	int r,g,b,col,cd;
	double arg;
public:
	Beecir();
	CRect view(CView *vp,CPoint gp,int type,int opt=0);
	~Beecir(){};
};


enum{
	_NONE_COL,
	_PLACE_COL,
	_TRAN_COL,
	_PASSTRA_COL,
	_UHZI_COL,
	_KEM_COL,
	_MOD_COL,
	_ARC_COL,
	_BACK_COL,
	_GLID_COL,
	_TEXT_COL
};

enum{
	_TID_NULL,
	_TID_PETSIM,
	_TID_BEETOKEN,
	_TID_OBJBEE,
	_TID_FMOVE,
	_TID_DEMO,
	_TID_INIT,
	_TID_CHECK,
	_TID_VOUT_CHECK,
	_TID_COUT_CHECK,
	_TID_MEA_CHECK,
	_TID_RES_CHECK
};

#define _SELECTED_COL 100

class CQROQSView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CQROQSView();
	DECLARE_DYNCREATE(CQROQSView)


	CDC m1DC,bufDC;
	CBitmap *bufBMP;
	HBITMAP m1BMP;
	LPBYTE m1lpBMP;

#ifndef _LIGHT
	CDC objDC,miniDC,backDC;
	CBitmap objBMP,backBMP;
	HBITMAP miniBMP;
	LPBYTE minilpBMP,backlpBMP;
	CSize back_size;
#endif

	int st2;

	int *Posface;
	int vArcBase,vArcFollow,PutIt;
	
	int compOk,simGo;

	UINT pressKey;

	int SelectOBJ;
	bool HandlingTrack;
	bool ArcPic;
	string vArcVal;

#ifndef _LIGHT
	bool Hungvgpoint;
#endif

	CRect
		viewRect,//ビューのやつ
		printRect,//印刷用
		oudRect,//前回更新した領域
		nudRect//今回更新する領域
		;
	CRect
		erroudRect,//前回更新した領域
		errnudRect//今回更新する領域
		;
	
	bool Hungface;
	CPoint Oldpoint,tkOldpoint,pasteOldpoint;

	int now_command;

	CPen 
		pen_gray1d,
		pen_wred5,
		pen_wgreen5,
		pen_wblue5,
		pen_wyellow5;

	CPen
		*pen_hatch,
		*pen_token,
		*pen_arc,
		*pen_place,
		*pen_transition,
		*pen_transition_func,
		*pen_uhzi,
		*pen_kemmakix,
		*pen_module,
		
		*pen_arc_select,
		*pen_place_select,
		*pen_transition_select,
		*pen_transition_func_select,
		*pen_uhzi_select,
		*pen_kemmakix_select,
		*pen_module_select;

	CBrush 
		brush_grayh,
		brush_token,
		*brush_uhzi,
		*brush_kemmakix,
		*brush_module,
		
		*brush_uhzi_select,
		*brush_kemmakix_select,
		*brush_module_select;


	int pos_;

	CFont vcFont;	// ベースフォント
	CFont vFont;	// 描画時に使用されるもの

	CString SkinDir;

	vector<CPoint> varcBwt;

	// マウスがウィンドウから離れているかどうかのフラグ
	BOOL m_LeaveFlag;       
                            // FALSE … マウスはウィンドウの中
                            // TRUE … マウスはウィンドウの外

#ifndef _LIGHT
	float shadowDiam;
	CPoint vpsPlace[2], vpsTra[2], vpsUhziKem[5], vpsModule[3];
#endif
	float viewDiam;
	float diamDelta;
	int vChipSize;

	//アーク接続時に選択するトランジション関数の変数
	int tra_io_var;

	// 任意の領域をズームイン
	int zoomrect_flag;

	int sim_pm;//シミュレーションの状態

	CPoint vpPlace[2], vpTra[2], vpUhziKem[5], vpModule[3];
	CPoint vpdPlace[2], vpdTra[2], vpdUhziKem[5], vpdModule[3];


	//トラッキング
	CRectTracker tracker;
	
#ifndef _LIGHT
	vector<BeeToken> bee_token;
	//スペース移動時
	CPoint vgpoint;
#endif
	vector<Beecir> bee_err;

	int SimAnime;

	//ツールチップ用
	CString m_strTip;
	CToolTipCtrl m_toolTip;
	
	//iniに保存するもの
	int HatchSize;
	int confDataViewMax;
	BOOL confCompileSave;
	BOOL confSimAnime;
	BOOL confSimTokenMove;
	BOOL confSimTokenTrans;
	BOOL confSimBinValue;
	int UndoSize;
	int SimundoSize;
	BOOL confEditArcinMove,confEditArcoutMove;
	BOOL confEditAlwaysAlign;
	int confEditGridType;
	float diamDist;

	int SimTurn;

	time_t sim_st_time;

	map<int,COLORREF> vColMap;

	COLORREF	vColBToken;

	//フォントの種類など

	//オブジェクトIDを表示するかどうか
	BOOL EnableObjId;
	//過去通過トークンを表示する
	BOOL EnableOldToken;

	BSPetri *bsp;	//カレントBSP

	BSPetriTree *bsp_tree;	//カレントツリー
	BSPetriTree copy_bsp_tree;	//コピーツリー

	map<string, deque<BSPetri> > undo_bsp;//undo用
	// 頭から入れて穴から削除。
	// stlバグのため、デストラクタでなく明示的にclear()を使用すること

	//simulation undo
	deque<map<BSPetri*,map<int,bspPlace> > > sim_token;
	map<BSPetri*,map<int,bspPlace> > sim_init_token;
	deque<map<BSPetri*,set<int> > > sim_uk;

	int mst;
	int sim_end;

#ifndef _LIGHT
	int help_id;
	int SkinUse;
#endif

	HTREEITEM current_item;

	int print_now_page;
	int PrintColor;

	BOOL rect_back_flag;

	CTraceDlg *m_wndTraceDlg;


	// チェックスレッド
	CWinThread *check_thread;
	void CheckEnd();

	string err_str;
	CString trace_buf;

#ifndef _LIGHT
	// Verilogスレッド
//	CWinThread *verilog_thread;
	void FileOutVerilogPost();
	CString vout_dir;

	// C code
	void FileOutCPost();
	CString cout_dir;
	COutCDlg cout_Dlg;

	// Mod Expand All
	void ModExpandallPost();

	// Research
	void ResearchPost();
#endif



// アトリビュート
public:
	CQROQSDoc* GetDocument();


// オペレーション
public:

	///////追加・・・・
	int PetCom(CQROQSDoc*);
	
	//グローバル座標からビューハッチ座標へ変換 L左上
	CPoint CtoLUpoint(CPoint lp)
	{
		lp.x -= (vChipSize>>1);
		lp.y -= (vChipSize>>1);
		return lp;
	}
#ifndef _LIGHT
	CPoint sCtoLUpoint(CPoint lp)
	{
		lp.x -= ((V_CHIP_SIZE>>1)*shadowDiam);
		lp.y -= ((V_CHIP_SIZE>>1)*shadowDiam);
		return lp;
	}
#endif

	int PosFaceSearch(CPoint);
	void FaceMove(CPoint);
	void FaceMove(int ,int );
	void PetInvalidate();
	void PutObject(CDC *, int x,int y,int obj_type, int opt=0, int selected=0);

	void ViewToken(CDC *pDC,CPoint mp,int col,int val)
	{
	//	TRACE("%d\n",val);
		pDC->SelectObject(pen_token);
	//	TRACE("put token\n");
		if(col==_BTOKEN) pDC->SelectObject(brush_token);
		else if(col==_WTOKEN) pDC->SelectStockObject(WHITE_BRUSH);
	//	else TRACE("token color err\n");//throw "err token color";

		//トークンを振動させる
		if(PetMode==_SIMULATION && 
			(Qmode || (confSimAnime && confSimTokenMove) )
			&& viewDiam>=0.25){
			mp.x += (rand()%5-2)*viewDiam;
			mp.y += (rand()%5-2)*viewDiam;
		}
		
		if(Qmode && PetMode==_SIMULATION){
			pDC->Ellipse(mp.x-((rand()%10+5)*viewDiam), mp.y-((rand()%10+5)*viewDiam), 
				mp.x+((rand()%10+5)*viewDiam), mp.y+((rand()%10+5)*viewDiam));
		}
		else{
			pDC->Ellipse(mp.x-(10*viewDiam), mp.y-(10*viewDiam), mp.x+(10*viewDiam), mp.y+(10*viewDiam));
		}
	
		CString cs;
		//値を表示
		if(
	//#ifndef _DEBUG
			col==_BTOKEN && 
	//#endif
			viewDiam>=0.5){
			if(val==0 || val==1) cs.Format("%d",val);
			else cs="X";
			pDC->TextOut(mp.x-(6*viewDiam), mp.y-(10*viewDiam), cs);
		}
	}

	//囲う
	void Yard(CDC *pDC, CPoint mp, int size)
	{
		int half = size >> 1;
		pDC->SelectStockObject( NULL_BRUSH );
		pDC->Rectangle(mp.x-half, mp.y-half, mp.x+half, mp.y+half);
	}
	void FlyObject(CDC *, int ,int,int);
	void PetSim();
	void viewReset();
	void Yajirushi(CDC *, CPoint, CPoint, double , int);
	void viewPenBrushChange();
	
	//ローカル座標（縮小なしの画面）からビュー座標（縮小画面）への変換
	CPoint Diampoint(float diam, CPoint p)
	{
		int vhalfw=viewRect.Width()>>1;
		int vhalfh=viewRect.Height()>>1;
		p.x = vhalfw-((vhalfw-p.x)*diam);
		p.y = vhalfh-((vhalfh-p.y)*diam);
		return p;
	}

	//ビュー座標からローカル座標への変換
	CPoint mDiampoint(float diam, CPoint p)
	{
		int vhalfw=viewRect.Width()>>1;
		int vhalfh=viewRect.Height()>>1;
		p.x = vhalfw-((vhalfw-p.x)/diam);
		p.y = vhalfh-((vhalfh-p.y)/diam);
		return p;
	}

	void ScrollSet();
	CPoint mouse_to_alignPoint(CPoint point);
	void ZoomRect(CPoint point);
	void ArcDraw(bspArc *arc, CPoint inp, CPoint outp, vector<CPoint> *varcp);
	void ReadObjectSkin();
	void ReadBackSkin();

	void OnTraInsort();
	void OnTraOutsort();

#ifndef _LIGHT
	//シャドウ用
	CPoint sDiampoint(float diam, CPoint p)
	{
		int vhalfw=viewRect.Width()>>1;
		int vhalfh=viewRect.Height()>>1;
		p.x = vhalfw-((vhalfw-p.x)*diam);
		p.y = vhalfh-((vhalfh-p.y)*diam)-(vhalfh>>1);
		return p;
	}
	void PutObjectMini(CDC *, int ,int,int);//シャドウ用
#endif
	void TokenMoveEffect();

	void ArcChain();
	void PasteChain(CPoint mp);

	void ObjTrackMove(set<int> &track,CPoint point);
	void FitView();


	//undo
	void UndoSave();

	//simulation undo
	void SimSave(int undo_size);
	void SimSaveUK(int undo_size);

	void InitModuleTree(BSPetriTree &bsp_tree);
	void ExpandAll(CTreeCtrl *pTree);
	void PrintArcValue(CDC *pDC, CPoint inp, CPoint outp, const char *arc_val);



// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CQROQSView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CQROQSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CQROQSView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnObjArc();
	afx_msg void OnObjBtoken();
	afx_msg void OnObjHand();
	afx_msg void OnObjKemmakix();
	afx_msg void OnObjPlace();
	afx_msg void OnObjTransition();
	afx_msg void OnObjUhzi();
	afx_msg void OnComStep();
	afx_msg void OnUpdateComStep(CCmdUI* pCmdUI);
	afx_msg void OnComCompile();
	afx_msg void OnUpdateComCompile(CCmdUI* pCmdUI);
	afx_msg void OnComGo();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnComStop();
	afx_msg void OnUpdateObjTransition(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjUhzi(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjKemmakix(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjArc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjBtoken(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjHand(CCmdUI* pCmdUI);
	afx_msg void OnComClear();
	afx_msg void OnUpdateComStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComClear(CCmdUI* pCmdUI);
	afx_msg void OnComHalt();
	afx_msg void OnUpdateComHalt(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnComWrite();
	afx_msg void OnComOpen();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnComConfig();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateComOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComWrite(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComGo(CCmdUI* pCmdUI);
	afx_msg void OnObjDelete();
	afx_msg void OnUpdateObjDelete(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnUpdateComArcsort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateComAlign(CCmdUI* pCmdUI);
	afx_msg void OnTraFunction();
	afx_msg void OnUhziFunction();
	afx_msg void OnKemFunction();
	afx_msg void OnModFunction();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnComAlign();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteHorz();
	afx_msg void OnUpdateEditPasteHorz(CCmdUI* pCmdUI);
	afx_msg void OnEditPasteVert();
	afx_msg void OnUpdateEditPasteVert(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnComZoomup();
	afx_msg void OnComZoomdown();
	afx_msg void OnDestroy();
	afx_msg void OnEditCopy();
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnComChain();
	afx_msg void OnUpdateComChain(CCmdUI* pCmdUI);
	afx_msg void OnFileOutVerilog();
	afx_msg void OnViewObjId();
	afx_msg void OnUpdateViewObjId(CCmdUI* pCmdUI);
	afx_msg void OnViewOldToken();
	afx_msg void OnUpdateViewOldToken(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnComStepUndo();
	afx_msg void OnUpdateComStepUndo(CCmdUI* pCmdUI);
	afx_msg void OnComHelp();
	afx_msg void OnObjModule();
	afx_msg void OnComDemo();
	afx_msg void OnUpdateComDemo(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdateObjModule(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjPlace(CCmdUI* pCmdUI);
	afx_msg void OnModulePropaty();
	afx_msg void OnUpdateModulePropaty(CCmdUI* pCmdUI);
	afx_msg void OnModuleRead();
	afx_msg void OnUpdateModuleRead(CCmdUI* pCmdUI);
	afx_msg void OnModuleUp();
	afx_msg void OnUpdateModuleUp(CCmdUI* pCmdUI);
	afx_msg void OnComSelectall();
	afx_msg void OnComBackconf();
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnComObjlap();
	afx_msg void OnBspPropaty();
	afx_msg void OnComBlast();
	afx_msg void OnUpdateComBlast(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnModManager();
	afx_msg void OnFileOutC();
	afx_msg void OnComOfflineHelp();
	afx_msg void OnComZoomrect();
	afx_msg void OnUpdateComSelectall(CCmdUI* pCmdUI);
	afx_msg void OnComPlaceFusion();
	afx_msg void OnUpdateComPlaceFusion(CCmdUI* pCmdUI);
	afx_msg void OnComSkinSave();
	afx_msg void OnComText();
	afx_msg void OnModExpandall();
	afx_msg void OnComResearch();
	afx_msg void OnComIndata();
	afx_msg void OnComPlaceFission();
	afx_msg void OnComMemo();
	afx_msg void OnComJointdata();
	//}}AFX_MSG
	//ツールチップ用
    afx_msg void OnTipDispInfo(NMHDR* pNMHDR, LRESULT* pResult);

	//skinメニュー
	afx_msg void OnSkinMenu(UINT uID);

	DECLARE_MESSAGE_MAP()
};




#ifndef _DEBUG  // QROQSView.cpp ファイルがデバッグ環境の時使用されます。
inline CQROQSDoc* CQROQSView::GetDocument()
   { return (CQROQSDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_QROQSVIEW_H__00ACB2DF_AE5B_4447_A351_01FB7113D5CB__INCLUDED_)

/// EOF ///

