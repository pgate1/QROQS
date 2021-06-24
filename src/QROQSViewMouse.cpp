// QROQSViewMouse.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#include "other.h"

/////////////////////////////////////////////////////////////////////////////


BOOL CQROQSView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pHandlerInfo == NULL ){	//　すでに割り当てられているメッセージがない場合……

		if(tra_io_var!=0){
			int i;
		for( i = 0; i <= 99; ++i ){
			if( nID == (UINT)( ID_IOMENU_0000 + i ) ){//　もしそのメッセージなら……
				
				if( nCode == CN_COMMAND ){
					if(tra_io_var==1){
						tra_io_var=nID - ID_IOMENU_0000;
						OnTraInsort();
						tra_io_var=0;
					}
					else if(tra_io_var==2){
						tra_io_var=nID - ID_IOMENU_0000;
						OnTraOutsort();
						tra_io_var=0;
					}
				}
				else if( nCode == CN_UPDATE_COMMAND_UI ){
					//　シンボルをオンにします（これをしないとメニュー不可のままです）。
					CCmdUI* pCmdUI = (CCmdUI*)pExtra;
					pCmdUI->Enable( TRUE );
				}

				return TRUE;
			}
		}
		}
		
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


// CQROQSView クラスのメッセージ ハンドラ
BOOL CQROQSView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
 
    switch(pMsg->message){

	case WM_MBUTTONDOWN:
//		TRACE("m down\n");
		//画面をつかむ
	
		if(!HandlingTrack){
			SetCapture();
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			Hungface=true;
			Oldpoint = pMsg->lParam;
		}
	
		break;
		
	case WM_MBUTTONUP:
		//TRACE("mbutton up\n");
		if(Hungface==true){
			ReleaseCapture();
			Hungface=false;
			//positionfaceの更新
			pos_=1;
		}
		Invalidate(0);
		break;
	case WM_MOUSEHOVER:
		// ウィンドウにマウスポインタがやってきてから数秒が経過しました。");
	//	SetFocus();
	//	TRACE("mouse in\n");
	    break;
    case WM_MOUSELEAVE:
		//ウィンドウからマウスポインタが離れました
//		TRACE("mouse leave");
        m_LeaveFlag = TRUE;	//次の検出のための予約
		Invalidate(0);
        break;
    }

	m_toolTip.RelayEvent( pMsg );
	
	return CView::PreTranslateMessage(pMsg);
}

void CQROQSView::ArcChain()
{
	
	try{
		bsp->ArcChain(vArcBase,vArcFollow,&varcBwt,vArcVal);
	}
	catch(char *err){
		Trace(err);
	}
	
	varcBwt.clear();
	vArcBase=NULL;
	vArcFollow=NULL;
	ArcPic = FALSE;
	vArcVal.erase();
}

char* type_to_str(int type)
{
	switch(type){
	case _PLACE:		return "ﾌﾟﾚｰｽ";
	case _TRANSITION:	return "ﾄﾗﾝｼﾞｼｮﾝ";
	case _UHZI:			return "ｳｰｼﾞ";
	case _KEMMAKIX:		return "ｹﾑﾏｷ";
	case _MODULE:		return "ﾓｼﾞｭｰﾙ";
	}
	return "不明なｵﾌﾞｼﾞｪｸﾄ";
}

CPoint CQROQSView::mouse_to_alignPoint(CPoint point)
{
	CPoint mp;
	mp = mDiampoint(viewDiam,point) + bsp->Globalpoint;
	mp = alignPoint(mp, vChipSize, HatchSize) - bsp->Globalpoint;
	return mp;
}


float zoom_diam(
	// ビューｻｲｽﾞ
	int vWidth, int vHeight,
	// ズームする領域のｻｲｽﾞ
	int zWidth, int zHeight)
{
	/*
	int vw,vh;
	vw = ceil(log((double)vWidth/zWidth)/log(vDelta));
	vh = ceil(log((double)vHeight/zHeight)/log(vDelta));
	if(vh>vw) return vh;
	return vw;
	*/

	float vw,vh;
	vw = zWidth > vWidth ? (float)vWidth / zWidth : 1;
	vh = zHeight > vHeight ? (float)vHeight / zHeight : 1;
	if(vh<vw) return vh;
	return vw;
}

void CQROQSView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

//	TRACE("mouse L down(%d,%d)\n",point.x,point.y);
	if(Hungface) return;
	
	Oldpoint=point;
	
#ifndef _LIGHT
	if(pressKey==VK_SPACE){
		vgpoint.x = point.x/shadowDiam;
		vgpoint.y = point.y/shadowDiam;
		Hungvgpoint=true;
		return;
	}
#endif
	

	// 任意の選択領域を拡大
	if(GetAsyncKeyState( VK_RBUTTON )&0x8000 || zoomrect_flag){
		ZoomRect( point );
		Invalidate();
		return;
	}

	CPoint mp,gp;

	//そこにオブジェクトがあるかどうか
	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}
	//	TRACE("mouse selectOBJ %d\n",SelectOBJ);

	int obj_type=_NONE;
	if(PutIt!=0) obj_type=bsp->ObjMap[PutIt].type;
	

	//シミュ中にトークンを配置
	if(PetMode==_SIMULATION){
		bsp->intrack.clear();
/*
		if(PutIt && bsp->vObjMap[PutIt].type==_PLACE){
			bsp->PlaceMap[PutIt]->SetToken(bsp->PlaceMap[PutIt]->GetOldColor()==_WTOKEN?_BTOKEN:_WTOKEN,0);
		}
		*/
	}

	
//	TRACE("lbuttondown obj %d\n",bsp->vObjMap.size());


	if(now_command==_DELETE){
		if(PutIt != 0){
			
			//ていうか、プレースを選択したらとーくんをせんたくしたことにする？
			//繋がってるアーも削除
			/*
			if(AfxMessageBox("消すよ？",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
			return;
			}		
			*/

			UndoSave();
			bsp->DelObject(PutIt);
			bsp->intrack.clear();

			if(obj_type==_MODULE){
				InitModuleTree(*bsp_tree);
				ExpandAll(GetModuleTree());
			}

			pos_=1;
		}
	}
	else if(now_command == _BTOKEN){
		if(PutIt && bsp->ObjMap[PutIt].type == _PLACE){
			//トークンを置こうとしているプレースに
			//すでにトークンがあったら色を変える?

			UndoSave();
			bsp->PutToken(PutIt,_BTOKEN,0);
			//	TRACE("tset(%s,%d)\n",SelectOBJ==_BTOKEN?"BLACK":"WHITE",SelectVAL);
		}
	}
	else if(now_command == _HAND 
		
	//	|| (PetMode==_EDIT && now_command == _NONE)
		
		){
		SelectOBJ = _HAND;
		if(PutIt!=NULL){

			//Ctrlが押されていたらオブジェクトを追加選択
			if(pressKey==VK_CONTROL){
				bsp->intrack.insert(PutIt);
				bsp->TrackRectSet();
				Invalidate(0);
				return;
			}

			SetCapture();//ラバーバンドと競合するので、気をつけろ
			//	TRACE("track in\n");
			UndoSave();

			if(bsp->intrack.find(PutIt)==bsp->intrack.end()){
				bsp->intrack.clear();
				bsp->intrack.insert(PutIt);
				bsp->TrackRectSet();
			}

			HandlingTrack = true;
			tkOldpoint = mDiampoint(viewDiam,point)+bsp->Globalpoint;
		}
		else{//if(PutIt==NULL)

			if(pressKey==VK_SHIFT || pressKey==VK_CONTROL);
			else{
				bsp->trackRect.SetRectEmpty();
				bsp->intrack.clear();
				Invalidate(0);
			}

			StatusOut("ﾄﾗｯｷﾝｸﾞ中...");


			tracker.TrackRubberBand(this, point);
			tracker.m_rect.NormalizeRect();
			//	TRACE("rabber\n");
			
			//TRACE("(%d,%d)(%d,%d)\n",tracker.m_rect.left,tracker.m_rect.top,tracker.m_rect.right,tracker.m_rect.bottom);
			CPoint mtlp;
			mtlp=tracker.m_rect.TopLeft();
			CPoint mbrp;
			mbrp=tracker.m_rect.BottomRight();
			//	TRACE("go0\n");
			
			CPoint sp,ep,op;
			sp=mtlp;
			ep=mbrp;
			
			sp=mDiampoint(viewDiam, sp);
			ep=mDiampoint(viewDiam, ep);
			sp+=bsp->Globalpoint;
			ep+=bsp->Globalpoint;

//			TRACE("%d %d  %d %d\n",sp.x,sp.y,ep.x,ep.y);
		
			map<int,bspObject>::iterator oit,oit_end;
			oit=bsp->ObjMap.begin();
			oit_end = bsp->ObjMap.end();
			for(;oit!=oit_end;++oit){
				op=oit->second.gpoint;
				if(op.x>=sp.x && op.x<=ep.x && op.y>=sp.y && op.y<=ep.y){
					bsp->intrack.insert(oit->first);
				}
			}

			if(bsp->intrack.size()==0){
				Invalidate(0);
				return;
			}
			
			//	TRACE("go\n");
			//選択したオブジェクトにあわせて拡張する
			bsp->TrackRectSet();

			StatusOut("%d 個のｵﾌﾞｼﾞｪｸﾄを選択",bsp->intrack.size());

		}
	}

	//オブジェクトを置けるなら置く
	if(PutIt == NULL &&
		(now_command==_PLACE
		|| now_command==_TRANSITION
		|| now_command==_MODULE
		|| now_command==_UHZI
		|| now_command==_KEMMAKIX
		|| now_command==_CHAIN)){//何もないので置く

		bsp->intrack.clear();

		UndoSave();

		// コントロール押下で定位置配置
		if(pressKey==VK_CONTROL || confEditAlwaysAlign){
			mp = mouse_to_alignPoint(point);
		}
		else{
			mp = point;
			mp = mDiampoint(viewDiam,mp);
		}

		int obj;
		obj = bsp->dObjAdd(mp+bsp->Globalpoint, SelectOBJ);
	
		if(now_command==_UHZI){
			//連続配置プレースへ
			if(ArcPic==true){
				PutIt=obj;
				vArcFollow = obj;
			}
		}
		else if(now_command==_KEMMAKIX){
			if(ArcPic==false){
			}
			else{
				PutIt = obj;
				vArcFollow = obj;
			}
		}
		else if(now_command==_CHAIN){
			if(SelectOBJ==_PLACE){
				PutIt = obj;
				vArcFollow = obj;
				ArcChain();
				vArcBase = obj;
				SelectOBJ=_TRANSITION;
			}
			else if(SelectOBJ==_TRANSITION){
				PutIt = obj;
				vArcFollow = obj;
				ArcChain();
				vArcBase = obj;
				SelectOBJ=_PLACE;
			}
			StatusOut("次の%sを配置してください",type_to_str(SelectOBJ));
		}
		else if(now_command==_MODULE){

		//	TRACE("%s\n",bsp->ModMap[obj].c_str());

			InitModuleTree(*bsp_tree);
			ExpandAll(GetModuleTree());

		}

		pos_ = 1;
	}
	else if(PutIt==NULL && now_command==_ARC){
		if(ArcPic==true){//betwixt 何もないところ、経由点
			if(pressKey == VK_CONTROL || confEditAlwaysAlign){
				mp = mouse_to_alignPoint(point);
			}
			else{
				mp = point;
				mp = mDiampoint(viewDiam,mp);
			}

			varcBwt.push_back(mp+bsp->Globalpoint);
		}
	}

	if(PutIt!=NULL && 
		( now_command==_ARC
		|| now_command==_UHZI
		|| now_command==_KEMMAKIX
		|| now_command==_CHAIN)){

		if(ArcPic==false && vArcBase!=NULL){//接続元

			vArcFollow = NULL;
			vArcBase = PutIt;//接続元
			varcBwt.clear();
			ArcPic = TRUE;
			vArcVal.erase();

			//接続先候補
			if(now_command==_UHZI){
				SelectOBJ=_PLACE;
				StatusOut("接続先のﾌﾟﾚｰｽを選択してください");
			}
			else if(now_command==_CHAIN){
				if(obj_type==_TRANSITION || obj_type==_UHZI){
					SelectOBJ=_PLACE;
				}
				else if(obj_type==_PLACE){
					SelectOBJ=_TRANSITION;
				}
				StatusOut("次の%sを配置してください",type_to_str(SelectOBJ));
			}
		}
		else if(ArcPic==true && vArcFollow){//接続先
		//	TRACE("lbuttondown2 obj %d\n",bsp->vObjMap.size());
	//		TRACE("chain %d -> %d\n",vArcBase,vArcFollow);
			UndoSave();
			ArcChain();
		//	TRACE("chain end\n");
			if(now_command==_UHZI){
				OnObjUhzi();
			}
			if(now_command==_CHAIN){
				OnComChain();
			}
		}
		else{
			CClientDC myDC(this);
			//クリア
			BitBltRect(&myDC, &m1DC, viewRect);
		}
	}
	

//	TRACE("lbuttondown last obj %d\n",bsp->vObjMap.size());
	//TRACE("mouse down end\n");

/*
#ifdef _DEBUG
	//チェック
	if(bsp->vObjMap.find(0)!=bsp->vObjMap.end()){
		TRACE("OnLButtonDown ゼロオブジェクトが存在しています。内部エラー");
	}
	if(bsp->vArcMap.find(0)!=bsp->vArcMap.end()){
		TRACE("OnLButtonDown ゼロアークが存在しています。内部エラー");
	}

	bsp->obj_check();
#endif
*/

//	TRACE("m L up check\n");

	Invalidate(0);

	CView::OnLButtonDown(nFlags, point);
}

void CQROQSView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	OnKeyDown(VK_ESCAPE,0,0);
	

	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		return;
	}
	
	
	if(SelectOBJ==_BTOKEN && bsp->ObjMap[PutIt].type==_PLACE){//実はトークンの設定
		if(bsp->TokenMap.find(PutIt) != bsp->TokenMap.end()){//このプレースはトークンを保持している。pはトークンリストの位置
			UndoSave();
			bsp->PutToken(PutIt,
				bsp->TokenMap[PutIt].Color,
				1 - bsp->TokenMap[PutIt].Value);
			Invalidate(0);
		}
	}
	else if(bsp->ObjMap[PutIt].type==_UHZI){
		OnUhziFunction();
	}
	else if(bsp->ObjMap[PutIt].type==_TRANSITION){
		OnTraFunction();
	}
	else if(bsp->ObjMap[PutIt].type==_KEMMAKIX){
		OnKemFunction();
	}
	else if(bsp->ObjMap[PutIt].type==_MODULE){
		OnModFunction();
	}
	CView::OnLButtonDblClk(nFlags, point);
}



void CQROQSView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

//	TRACE("move p (%d, %d)\n",point.x,point.y);

#ifndef _LIGHT
	if(Qmode){
		// マウスカーソルが千鳥足
		static int yoi_flag=1;
		if(yoi_flag){
			point.x += rand()%7 - 3;
			point.y += rand()%7 - 3;
			CPoint p = point;
			ClientToScreen(&p);
			SetCursorPos(p.x, p.y);
			yoi_flag=0;
		}
		else yoi_flag=1;

		// たまにコアダンプ
		if((rand()%100)==0){
			OnLButtonDown(nFlags, point);
		}
	}

	CString cs;
	CPoint gp;
	gp=mDiampoint(viewDiam,point)+bsp->Globalpoint;
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_POSX);
	cs.Format("x:%d",gp.x);
	pStatusBar->SetPaneText(nIndex, cs);
	nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_POSY);
	cs.Format("y:%d",gp.y);
	pStatusBar->SetPaneText(nIndex, cs);
#endif

//TRACE("%d %d\n",Globalpoint.x ,Globalpoint.y);

    if(m_LeaveFlag) {
		//m_stcMessage.SetWindowText("ウィンドウにマウスポインタがやってきました。");
        m_LeaveFlag = FALSE;		
        // 次のWM_MOUSELEAVE,WM_MOUSEHOVERの検出予約
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 0;//秒数
        tme.hwndTrack = this->m_hWnd;
        TrackMouseEvent(&tme);
    }

#ifndef _LIGHT
	if(Hungvgpoint){
		vgpoint.x += (point.x - Oldpoint.x)/shadowDiam;
		vgpoint.y += (point.y - Oldpoint.y)/shadowDiam;
		Oldpoint=point;
		Invalidate(0);
		return;
	}
#endif
	
	//サブでクリア
	BitBltRect(&bufDC, &m1DC, oudRect);

	nudRect.SetRectEmpty();
	nudRect.OffsetRect(-20,-20);

	CRect nrect;

	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}

//	TRACE("putit %d\n",PutIt);


	int obj_type=_NONE;	
	if(PutIt!=0) obj_type=bsp->ObjMap[PutIt].type;
	//TraceClear();
	
	static int tip_displayed=0;
	if(PutIt!=0){
		// マウスポインタを乗せたらエラー表示を止める
		if(PutIt==bsp->ErrPos){
			bsp->ErrPos=0;
			Invalidate(0);
		}
		
		// ツールチップを表示
		if(bsp->ObjMap[PutIt].type==_TRANSITION
			&& tip_displayed==0){
			m_strTip = bsp->TranMap[PutIt]->fstring.substr(0,1024).c_str();
			if(m_strTip.GetLength() > 1020){
				m_strTip += "\r\n　　　（全て表示できませんでした）";
			}

			SetWindowStyleLayered(m_toolTip.GetSafeHwnd());
			SetLayeredWindowAttributes(m_toolTip.GetSafeHwnd(),0/*無視される*/, 200, LWA_ALPHA);

		//	m_toolTip.Update(); //ツールチップの書き換え
			tip_displayed=PutIt;
		}
		else if(tip_displayed!=PutIt){
			tip_displayed=0;
			m_strTip.Empty();
			m_toolTip.Pop();
		}
	}
	else{
		tip_displayed=0;
		m_strTip.Empty();
		m_toolTip.Pop();
	}
	
/*
	//シミュ中にトークンを配置できるかどうか
	if(PetMode==_SIMULATION){
		if(PutIt && bsp->vObjMap[PutIt].type==_PLACE){
			StatusOut("%sトークンを強制配置できます",
				bsp->PlaceMap[PutIt]->GetOldColor()==_WTOKEN?"L:0 R:1 黒":"白");
		}
		else{
			StatusOut("");
		}
	}
	//TRACE("old(%d,%d)[%d,%d]\n",oudRect.left,oudRect.top,oudRect.Width(),oudRect.Height());
*/

#ifndef _LIGHT
	if(Qmode){
		set<int> pos_enved;
		int p_obj;
		CPoint mp, gp;
		gp = mDiampoint(viewDiam,point)+bsp->Globalpoint;
		double r_pi;
		int rr;
		for(rr=20;rr<=300;rr*=2){
		for(r_pi=0.0;r_pi<M_PI*2;r_pi+=0.5){
			mp.x = rr * cos(r_pi);
			mp.y = rr * sin(r_pi);
			p_obj = PosFaceSearch(point + mp);
		//	if(p_obj==0) continue;
			if(bsp->ObjMap.find(p_obj)==bsp->ObjMap.end()){
		//		TRACE("err %d\n",p_obj);
				continue;
			}
			if(pos_enved.find(p_obj)!=pos_enved.end()) continue;
			pos_enved.insert(p_obj);
			if(rand()%100<10){
				bsp->ObjMap[p_obj].gpoint = Betwixtpoint(gp, bsp->ObjMap[p_obj].gpoint, 1, -(rand()%(100/rr+2)+2));
			}
		}
		}
		if(pos_enved.size()){
			pos_=1;
			Invalidate(0);
		}
	}
#endif


	if(Hungface==true){
		//画面の移動
		//	TRACE("fmove\n");
		FaceMove(point);
		Invalidate(0);
	}
	else if(SelectOBJ==_DELETE){
		if(PutIt!=0){
			if(obj_type==_PLACE && bsp->TokenMap.find(PutIt)!=bsp->TokenMap.end()){
				StatusOut("このﾄｰｸﾝを削除しますか？");
			}
			else StatusOut("この%sを削除しますか？",type_to_str(obj_type));

			CPoint mp;
			mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
			mp=Diampoint(viewDiam,mp);
			
			bufDC.SelectObject(&pen_wred5);
			Yard(&bufDC,mp,vChipSize+6);
			
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
		else{
			StatusOut("削除するｵﾌﾞｼﾞｪｸﾄを選択してください");
		}
	}
	else if(SelectOBJ==_BTOKEN){
		if(PutIt && bsp->ObjMap[PutIt].type==_PLACE){
			StatusOut("ここにﾄｰｸﾝを配置しますか？");
			//トークンが置ける
			CPoint mp;
			mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
			mp=Diampoint(viewDiam,mp);
			bufDC.SelectObject(&pen_wred5);
			Yard(&bufDC,mp,vChipSize+4);
			
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
		else{
			StatusOut("ﾄｰｸﾝを配置するﾌﾟﾚｰｽを選択してください");
		}
	}
	else if(SelectOBJ==_HAND && HandlingTrack==true){
		//	TRACE("handlingtrack\n");
		SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_HAND_HOLD));

		CPoint gp;
		gp=mDiampoint(viewDiam,point)+bsp->Globalpoint;

		ObjTrackMove(bsp->intrack,gp);

		tkOldpoint = gp;	//Global
		
		if(point.x > viewRect.Width())	FaceMove(point.x - viewRect.Width(),0);	// 10
		else if(point.x < 0)			FaceMove(point.x,0);	// -10
		if(point.y > viewRect.Height())	FaceMove(0,point.y - viewRect.Height());	// 10
		else if(point.y<0)				FaceMove(0,point.y);	// -10
		
		Invalidate(0);
		return;
	}


	//アークの接続元表示
	if(ArcPic==false && PutIt!=NULL && (	//接続元
		now_command==_ARC 
		|| now_command==_UHZI && obj_type==_UHZI
		|| now_command==_KEMMAKIX && obj_type==_PLACE
		|| now_command==_CHAIN
		)
		){
		
		vArcBase = PutIt;//接続元

		if(!Hungface){
			CPoint mp;
			mp = bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;//へんかんもしろ
			mp=Diampoint(viewDiam,mp);

			bufDC.SelectObject(&pen_wgreen5);
			Yard(&bufDC, mp, vChipSize+4);
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
	}
	else if(ArcPic){	//接続先
		CPoint inp,outp;

		//矢印
		if(varcBwt.size()){
			inp = varcBwt.back() - bsp->Globalpoint;
		}
		else{
			inp = bsp->ObjMap[vArcBase].gpoint - bsp->Globalpoint;
		}

		if(PutIt!=NULL && PutIt!=vArcBase &&
				(
					(bsp->ObjMap[vArcBase].type==_PLACE &&
						(
							obj_type==_TRANSITION ||
							obj_type==_KEMMAKIX ||
							obj_type==_UHZI ||
							obj_type==_MODULE
						)
					) ||
					(obj_type==_PLACE &&
						(
							bsp->ObjMap[vArcBase].type==_TRANSITION ||
							bsp->ObjMap[vArcBase].type==_KEMMAKIX ||
							bsp->ObjMap[vArcBase].type==_UHZI ||
							bsp->ObjMap[vArcBase].type==_MODULE
						)
					)
				)
			){
			//アークで接続可能
			
			vArcFollow = PutIt;

			if(!Hungface){
				CPoint mp;
				mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
				mp=Diampoint(viewDiam,mp);
				//囲う
				bufDC.SelectObject(&pen_wblue5);
				Yard(&bufDC, mp, vChipSize+4);
				
				nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
				nudRect.UnionRect(nudRect,nrect);

				outp = bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
				outp = Betwixtpoint(inp,outp,1,bsp->ObjMap[PutIt].type==_TRANSITION?8:20);
				outp=Diampoint(viewDiam, outp);
			}
		}
		else{
			vArcFollow = NULL;

			CPoint mp;
			// コントロール押下で定位置配置
			if(pressKey==VK_CONTROL || confEditAlwaysAlign){
				mp = mouse_to_alignPoint(point);
				outp = Diampoint(viewDiam,mp);
			}
			else outp = point;
		}

		if(!Hungface){
			inp = Diampoint(viewDiam,inp);
			bufDC.SelectObject(pen_arc);
			Yajirushi(&bufDC, inp, outp, M_PI/6, 15*viewDiam);

			// 逆のｱｰｸがあるかどうかﾁｪｯｸ
			{
				multimap<int,int>::iterator aioit;
				aioit=bsp->ArcInMap.lower_bound(vArcFollow);
				for(;aioit!=bsp->ArcInMap.upper_bound(vArcFollow);++aioit){
					if(bsp->ArcMap[aioit->second].Out==vArcBase){
						//TRACE("ぎゃく\n");
						CPoint mp;
						mp=bsp->ObjMap[vArcFollow].gpoint - bsp->Globalpoint;
						mp=Diampoint(viewDiam,mp);
						bufDC.SetTextColor(RGB(255,255,255));

						CRect fillrect;
						fillrect.SetRect(
							mp.x +(outp.x>inp.x?-40:40) -40,
							mp.y +(outp.y>inp.y?-24:24) -12,
							mp.x +(outp.x>inp.x?-40:40) +40,
							mp.y +(outp.y>inp.y?-24:24) +12
							);
						bufDC.FillSolidRect(fillrect,RGB(255,0,0));
						bufDC.TextOut(fillrect.left+8,fillrect.top+2,"接続解除");

						nudRect.UnionRect(nudRect,fillrect);
					}
				}
				//無かった。
			}

			nrect.SetRect(inp, outp);
			nrect.NormalizeRect();
			nrect.InflateRect(40,40);
			nudRect.UnionRect(nudRect,nrect);

			//変数の表示
			if(varcBwt.size()==0){
				bufDC.SetTextColor(vColMap[_TEXT_COL]);
				PrintArcValue(&bufDC, mDiampoint(viewDiam,inp), mDiampoint(viewDiam,outp), vArcVal.c_str());
			}
		}
	}
	else vArcBase=NULL;

	//Flyオブジェクトの表示
	if(PutIt==NULL && !Hungface &&
		(
		now_command==_PLACE
		|| now_command==_TRANSITION
		|| now_command==_UHZI
		|| now_command==_KEMMAKIX
		|| now_command==_MODULE
		|| now_command==_CHAIN
		)){//置ける

		CPoint mp;
		// コントロール押下で定位置配置
		if(pressKey==VK_CONTROL || confEditAlwaysAlign){
			mp = mouse_to_alignPoint(point);
			mp = Diampoint(viewDiam,mp);
		}
		else mp = point;
	
		mp = CtoLUpoint(mp);
		FlyObject(&bufDC, mp.x, mp.y, SelectOBJ);
		
		nrect.SetRect(mp.x,mp.y,mp.x+vChipSize,mp.y+vChipSize);
		nudRect.UnionRect(nudRect,nrect);
	}

	
	CClientDC myDC(this);
	nudRect.InflateRect(20, 20);//余分に
	oudRect.UnionRect(oudRect, nudRect);
	BitBltRect(&myDC, &bufDC, oudRect);
	oudRect=nudRect;
	
//	TRACE("%d %d %d %d\n",oudRect.left,oudRect.top,oudRect.right,oudRect.bottom);
//	TRACE("gp %d %d\n",Globalpoint.x ,Globalpoint.y);
	
	CView::OnMouseMove(nFlags, point);
}

//移動はグローバル座標で行うこと
void CQROQSView::ObjTrackMove(set<int> &track,CPoint point)
{


	bsp->ModifiedFlag = 1;

	CPoint mp;
	mp = point - tkOldpoint;	//global
	//		TRACE("mp(%d,%d)\n",mp.x,mp.y);
	
	bsp->ObjMove(track, mp.x, mp.y, confEditArcinMove, confEditArcoutMove);

	bsp->trackRect.OffsetRect(mp.x,mp.y);
}

void CQROQSView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	//	ReleaseCapture();//ラバーバンドと競合するので、気をつけろ
//	TRACE("mouse L  up\n");
#ifndef _LIGHT
	if(Hungvgpoint){
		Hungvgpoint=false;
		Invalidate(0);
		return;
	}
#endif
	
	PutIt = PosFaceSearch(point);


//	TRACE("lbuttonup1 obj %d\n",bsp->vObjMap.size());


	if(HandlingTrack){//つかんでいたら離す
		ReleaseCapture();
		HandlingTrack = false;
		pos_ = 1;
		Invalidate(0);
	}
	else if(SelectOBJ==_ARC){
		if(ArcPic && vArcFollow){//接続先（LButtonDownと同じ）
			UndoSave();
			ArcChain();
			StatusOut("");
			Invalidate(0);
		}
	}



//	TRACE("lbuttonup last obj %d\n",bsp->vObjMap.size());
	
	CView::OnLButtonUp(nFlags, point);
}



static CString GetShort(UINT id)
{
	CString str;
	str.LoadString(id);
	int nIndex = str.ReverseFind(_T('\n'));
	if(nIndex!=-1)
	{
		str=str.Mid(nIndex+1);
	}
	return str;
}

void CQROQSView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

//	TRACE("m %d %d\n",m_MouseLButtonDown,m_MouseRButtonDown);

	// 任意の選択領域を拡大
	if(GetAsyncKeyState( VK_LBUTTON )&0x8000 || zoomrect_flag){
		ZoomRect( point );
		Invalidate();
		return;
	}



	Oldpoint = point;	//View

	//コントロールキーが押されているならば、
	//画面をハングする
	if(pressKey==VK_CONTROL){
		LPARAM lParam = MAKELPARAM(point.x, point.y);
		PostMessage(WM_MBUTTONDOWN,0,lParam);
		return;
	}

	int obj_type=_NONE;
	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}
	else{
		obj_type=bsp->ObjMap[PutIt].type;
	}


	if(now_command==_CHAIN && PutIt==0){
		SendMessage(WM_KEYDOWN,VK_ESCAPE);
		return;
	}

	/*
	//シミュ中にトークンを配置
	if(PetMode==_SIMULATION){
		if(PutIt && obj_type==_PLACE){
			bsp->PlaceMap[PutIt]->SetToken(bsp->PlaceMap[PutIt]->GetOldColor()==_WTOKEN?_BTOKEN:_WTOKEN,1);
		}
	}
	*/


	if(SelectOBJ==_BTOKEN && PutIt && obj_type==_PLACE){
		//トークンを置こうとしているプレースに
		//すでにトークンがあったら色を変える
		map<int,bspToken>::iterator tokit;
		tokit = bsp->TokenMap.find(PutIt);
		if(tokit != bsp->TokenMap.end()){//トークンが配置されている
				UndoSave();

			bsp->PutToken(PutIt,
				tokit->second.Color==_BTOKEN?_WTOKEN:_BTOKEN,
				tokit->second.Value);
		}
	}

	
	CView::OnRButtonDown(nFlags, point);
}

void CQROQSView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	//画面をはなす
	if(Hungface==true){
		PostMessage(WM_MBUTTONUP);
		return;
	}


	// Rを押した時と離した時の座標が違ったらメニューは出さない
	if(point != Oldpoint) return;

	Oldpoint=point;	//View
	
	int obj_type=_NONE;
	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}
	else{
		obj_type=bsp->ObjMap[PutIt].type;
	}


	if(now_command!=_ARC && now_command!=_BTOKEN && now_command!=_CHAIN && (PetMode!=_SIMULATION || obj_type!=_PLACE)){
		if(!bsp->trackRect.PtInRect(mDiampoint(viewDiam,point)+bsp->Globalpoint)){
			bsp->intrack.clear();
			bsp->trackRect.SetRectEmpty();
			Invalidate(0);
		}
		if(bsp->intrack.size()==0 && PutIt){
			bsp->intrack.insert(PutIt);
			bsp->TrackRectSet();
			Invalidate(0);
		}
		if(bsp->trackRect.PtInRect(mDiampoint(viewDiam,point)+bsp->Globalpoint)){


/*
	menu.InsertMenu(5, MF_BYPOSITION , ID_EDIT_PASTE, GetShort(ID_EDIT_PASTE));
	menu.InsertMenu(6, MF_BYPOSITION|MF_SEPARATOR);
*/
#ifdef _LIGHT
			CMenu menu;
			menu.CreatePopupMenu();
#else
			CNewMenu menu;
			menu.CreatePopupMenu();
			menu.SetMenuTitle("Generic",MFT_GRADIENT|MFT_TOP_TITLE);
#endif
			if(PetMode==_EDIT){
				menu.AppendMenu(MF_STRING, ID_EDIT_COPY, "ｺﾋﾟｰ");
				menu.AppendMenu(MF_STRING, ID_EDIT_CUT, "切り取り");
				menu.AppendMenu(MF_STRING, ID_EDIT_DELETE, "削除");
				menu.AppendMenu(MF_STRING, ID_COM_ALIGN, "整列");
				menu.AppendMenu(MF_STRING, ID_COM_TEXT, "ﾃｷｽﾄ");

				if(obj_type){
					menu.AppendMenu(MF_SEPARATOR);
				}
			}

			switch(obj_type){
			case _PLACE:
				if(PetMode==_EDIT){
					menu.AppendMenu(MF_STRING, ID_COM_PLACE_FUSION, "ﾌﾟﾚｰｽ融合");
					menu.AppendMenu(MF_STRING, ID_COM_PLACE_FISSION, "ﾌﾟﾚｰｽ分裂");
				}
				break;
			case _TRANSITION:
				menu.AppendMenu(MF_STRING, ID_TRA_FUNCTION, "ﾄﾗﾝｼﾞｼｮﾝ編集");
				if(PetMode==_SIMULATION){
					menu.AppendMenu(MF_STRING, ID_COM_INDATA, "入出力ﾃﾞｰﾀ表示");
				}
				break;
			case _UHZI:
				menu.AppendMenu(MF_STRING, ID_UHZI_FUNCTION, "ｳｰｼﾞ編集");
				break;
			case _KEMMAKIX:
				menu.AppendMenu(MF_STRING, ID_KEM_FUNCTION, "ｹﾑﾏｷ編集");
				break;
			case _MODULE:
				menu.AppendMenu(MF_STRING, ID_MOD_FUNCTION, "ﾓｼﾞｭｰﾙへ移動");
				if(PetMode==_EDIT){
					menu.AppendMenu(MF_STRING, ID_MODULE_READ, "ﾓｼﾞｭｰﾙ読み込み");
					menu.AppendMenu(MF_STRING, ID_MODULE_PROPATY, "ﾓｼﾞｭｰﾙ変更");
				}
				break;
			}
			
#ifndef _LIGHT
			menu.LoadToolBar(ToolId,RGB(192,192,192));
#endif

//			TRACE("%d\n",PutIt);

			CPoint p=point;
			ClientToScreen(&p);
			m_LeaveFlag = TRUE;	//次の検出のための予約
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,this);
			return;

		}
	}

	if((SelectOBJ==_ARC || now_command==_CHAIN) && PutIt && (obj_type==_TRANSITION || obj_type==_MODULE)){
		if(ArcPic==true && vArcBase==PutIt) return;//同じならリターン
		if(ArcPic==true 
			&& (bsp->ObjMap[vArcBase].type==_TRANSITION || bsp->ObjMap[vArcBase].type==_MODULE)) return;

		//トランジションメニューポップアップ
#ifdef _LIGHT
		CMenu arc_menu;
#else
		CNewMenu arc_menu;
#endif
		arc_menu.CreatePopupMenu(); // トップレベルメニュー 

		struct innerfunc{
			static void arc_to_menu(
				int obj_id,
				map<string,int> &var_map,
				multimap<int,int> &aio_map,
				map<int,bspArc> &ArcMap,
#ifdef _LIGHT
				CMenu &Menu
#else
				CNewMenu &Menu
#endif
				){

				set<int> val_used;
				multimap<int,int>::iterator aioit;
				
			//	TRACE("s %d\n",var_map.size());

				//アークに使用されている変数名を登録
				aioit=aio_map.lower_bound(obj_id);
				for(;aioit!=aio_map.upper_bound(obj_id);++aioit){
					if(var_map.find( ArcMap[aioit->second].value_name ) != var_map.end()){
						val_used.insert(
							var_map[
							ArcMap[aioit->second].value_name
							]);
					}
				}
				
				map<string,int>::iterator it;
				it = var_map.begin();
				for(;it!=var_map.end();++it){
					Menu.AppendMenu(MF_STRING 
						| (val_used.find(it->second)==val_used.end()?0:MF_CHECKED) 
						,ID_IOMENU_0000 + it->second ,it->first.c_str());
				//	TRACE("%d : %s\n",it->second,it->first.c_str());
				}

				Menu.AppendMenu(MF_STRING ,ID_IOMENU_0000 ,"新規変数");
				
				return;
			}
		};

	//	TRACE("%d %d %d\n",ArcPic,vArcBase,vArcFollow);

		if(obj_type==_TRANSITION){
			bspTransition &tran = *(bsp->TranMap[PutIt]);
			if(ArcPic==true){//接続先（入力アーク)
				if(tran.in_var.size()==0){
					try{
						tran.FuncEduction();
					}
					catch(char *err){
						bsp->ErrPos = PutIt;
						Trace("ﾄﾗﾝｼﾞｼｮﾝ関数が不正です．\n%s",err);
					}
				}
#ifndef _LIGHT
		
				arc_menu.SetMenuTitle( "Input", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,tran.in_var,bsp->ArcOutMap,bsp->ArcMap,arc_menu);
			}
			else{//接続元(出力アーク)
				if(tran.out_var.size()==0){
					try{
						tran.FuncEduction();
					}
					catch(char *err){
						bsp->ErrPos = PutIt;
						Trace("ﾄﾗﾝｼﾞｼｮﾝ関数が不正です．\n%s",err);
					}
				}
#ifndef _LIGHT
				arc_menu.SetMenuTitle( "Output", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,tran.out_var,bsp->ArcInMap,bsp->ArcMap,arc_menu);
			}
		}
		else if(obj_type==_MODULE){
			
			// モジュールの入出力として選択できるのは、名前がついているもののみ

			BSPetri *sub_bsp = bsp_tree->GetBSP( bsp->ModMap[PutIt].c_str() );
			map<string,int> iovar;
			if(ArcPic==true){//接続先（入力アーク)
				if(sub_bsp!=NULL){
					map<int,bspUhzi*>::iterator uit;
					uit=sub_bsp->UhziMap.begin();
					for(int i=1;uit!=sub_bsp->UhziMap.end();++uit){
						if(!uit->second->input_name.empty()){
							iovar[uit->second->input_name] = i;
							i++;
						}
					}
				}
#ifndef _LIGHT
				arc_menu.SetMenuTitle( "Input", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,iovar,bsp->ArcOutMap,bsp->ArcMap,arc_menu);
			}
			else{//接続元(出力アーク)
				if(sub_bsp!=NULL){
					map<int,bspKemmakix*>::iterator kit;
					kit=sub_bsp->KemMap.begin();
					for(int o=1;kit!=sub_bsp->KemMap.end();++kit){
						if(!kit->second->output_name.empty()){
							iovar[kit->second->output_name] = o;
							o++;
						}
					}
				}
#ifndef _LIGHT
				arc_menu.SetMenuTitle( "Output", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,iovar,bsp->ArcInMap,bsp->ArcMap,arc_menu);
			}
		}


		// メインウィンドウへ表示しています。
		CPoint p=point;
		ClientToScreen(&p);
//		m_LeaveFlag = TRUE;	//次の検出のための予約

		//入力出力いずれのアークか示せ
		tra_io_var = (ArcPic==1 ? 1 : 2);
		arc_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,this);
	}

	if(now_command!=_CHAIN && PutIt == NULL && PetMode==_EDIT){
		//一般メニュー
		
		CMenu cput_menu;
		cput_menu.CreatePopupMenu();
		cput_menu.AppendMenu(MF_STRING, ID_EDIT_PASTE_HORZ, "水平方向");
		cput_menu.AppendMenu(MF_STRING, ID_EDIT_PASTE_VERT, "垂直方向");

#ifdef _LIGHT
		CMenu menu;
		menu.CreatePopupMenu();
#else
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.SetMenuTitle( "Layout", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
		menu.AppendMenu(MF_STRING, ID_COM_ZOOMRECT, "ｽﾞｰﾑ");
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, "貼り付け");
//		TRACE("%d\n",(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->vObjMap.size()!=0));
		menu.AppendMenu(MF_POPUP |
			((PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0) ? NULL: MF_GRAYED ),
			(UINT)cput_menu.m_hMenu, "連続配置");

#ifndef _LIGHT
		menu.LoadToolBar(IDR_TOOLBAR_SUB);
#endif

		// メインウィンドウへ表示しています。
		CPoint p=point;
		ClientToScreen(&p);
		m_LeaveFlag = TRUE;	//次の検出のための予約
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,AfxGetMainWnd());
	}
	
	Invalidate(0);


	CView::OnRButtonUp(nFlags, point);
}



BOOL CQROQSView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	int shadow_wheel=0;

	if(nFlags && MK_MBUTTON){//真中のボタン
		//	Trace("kkk");
		shadow_wheel = 1;
	}
	//	TRACE("key : %d\n",pressKey);
	int i;
	
	if(pressKey=='s' || shadow_wheel){
		if(zDelta<0){//手前ー＞拡大
			diamDist /= diamDelta;
			if(diamDist>1) diamDist=1;
		}
		else if(zDelta>0){//向こうー＞縮小
			diamDist *= diamDelta;
			if(diamDist<0.025) diamDist=(float)0.025;
		}
	}
	else{
	//		TRACE("mouse wheel m %d\n",zDelta);
		if(zDelta<0){//手前ー＞拡大
			for(i=0;i<(-zDelta / WHEEL_DELTA);i++){
				viewDiam /= diamDelta;
			}
		}
		else if(zDelta>0){//向こうー＞縮小
			for(i=0;i<(zDelta / WHEEL_DELTA);i++){
				viewDiam *= diamDelta;
			}
		}
		else{
			// viewDiamは現在設定されている値を使用する
		}

		if(viewDiam>1) viewDiam=1;
		else if(viewDiam<0.001) viewDiam=(float)0.001;

		vChipSize=V_CHIP_SIZE * viewDiam;
		if(vChipSize==0) vChipSize=1;
		
		for(i=0;i<2;++i){
			vpdPlace[i].x = vpPlace[i].x * viewDiam;
			vpdPlace[i].y = vpPlace[i].y * viewDiam;
			vpdTra[i].x = vpTra[i].x * viewDiam;
			vpdTra[i].y = vpTra[i].y * viewDiam;
		}
		for(i=0;i<5;++i){
			vpdUhziKem[i].x = vpUhziKem[i].x * viewDiam;
			vpdUhziKem[i].y = vpUhziKem[i].y * viewDiam;
		}
		for(i=0;i<3;++i){
			vpdModule[i].x = vpModule[i].x * viewDiam;
			vpdModule[i].y = vpModule[i].y * viewDiam;
		}
		
		
#ifndef _LIGHT
		if(SkinUse==1){
			objDC.StretchBlt(V_CHIP_SIZE,V_CHIP_SIZE,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,V_CHIP_SIZE,0,V_CHIP_SIZE,V_CHIP_SIZE,SRCCOPY);
			objDC.StretchBlt(V_CHIP_SIZE*2,V_CHIP_SIZE,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,V_CHIP_SIZE*2,0,V_CHIP_SIZE,V_CHIP_SIZE,SRCCOPY);
			objDC.StretchBlt(V_CHIP_SIZE*3,V_CHIP_SIZE,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,V_CHIP_SIZE*3,0,V_CHIP_SIZE,V_CHIP_SIZE,SRCCOPY);
			objDC.StretchBlt(V_CHIP_SIZE*4,V_CHIP_SIZE,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,V_CHIP_SIZE*4,0,V_CHIP_SIZE,V_CHIP_SIZE,SRCCOPY);
			objDC.StretchBlt(V_CHIP_SIZE*5,V_CHIP_SIZE,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,V_CHIP_SIZE*5,0,V_CHIP_SIZE,V_CHIP_SIZE,SRCCOPY);

			//skin
			objDC.BitBlt(0,V_CHIP_SIZE*2,V_CHIP_SIZE*6,V_CHIP_SIZE*3,&objDC,0,V_CHIP_SIZE,SRCCOPY);
			//透過のためそれぞれのオブジェクトの原点の色を透過色とする
			int o,x,y;
			COLORREF col;
			for(o=1;o<6;++o){
				col=objDC.GetPixel(V_CHIP_SIZE*o,0);
				for(y=V_CHIP_SIZE;y<V_CHIP_SIZE*2;++y){
					for(x=V_CHIP_SIZE*o;x<V_CHIP_SIZE*o+V_CHIP_SIZE;++x){
						if(objDC.GetPixel(x,y)==col){
							objDC.SetPixel(x,y,0);
							objDC.SetPixel(x,y+V_CHIP_SIZE,RGB(255,255,255));
						}
						else{
							objDC.SetPixel(x,y+V_CHIP_SIZE,0);
						}
					}
				}
			}
		}
#endif
		
		viewPenBrushChange();
		pos_=1;
	}

#ifndef _LIGHT
	shadowDiam = viewDiam * diamDist;

	//TRACE("%d %d\n",viewDiam,shadowDiam);

	for(i=0;i<2;++i){
		vpsPlace[i].x = vpPlace[i].x * shadowDiam;
		vpsPlace[i].y = vpPlace[i].y * shadowDiam;
		vpsTra[i].x = vpTra[i].x * shadowDiam;
		vpsTra[i].y = vpTra[i].y * shadowDiam;
	}
	for(i=0;i<5;++i){
		vpsUhziKem[i].x = vpUhziKem[i].x * shadowDiam;
		vpsUhziKem[i].y = vpUhziKem[i].y * shadowDiam;
	}
	for(i=0;i<3;++i){
		vpsModule[i].x = vpModule[i].x * shadowDiam;
		vpsModule[i].y = vpModule[i].y * shadowDiam;
	}
#endif

	bsp->viewDiam = viewDiam;
	
	Invalidate(0);
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CQROQSView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	/*
	SB_LEFT  //左端へスクロール  
	//SB_LINELEFT  //左へスクロール  （ボタン
	//SB_PAGELEFT  //1 ページ左へスクロール  （シャフト
	//SB_PAGERIGHT  //1 ページ右へスクロール  （シャフト
	//SB_LINERIGHT  //右へスクロール  （ボタン
	SB_RIGHT  //右端へスクロール  
	SB_ENDSCROLL  //スクロール終了  
	//SB_THUMBTRACK://スライダードラッグ中
	SB_THUMBPOSITION //絶対位置へスクロール  （バーが離された
	*/

	
	
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_HORZ,&scrollinfo);
	int sflag=false;
	
	switch(nSBCode){
	case SB_LINELEFT://左へスクロール  （ボタン
		FaceMove(-10,0);
		Invalidate(0);
		break;
	case SB_PAGELEFT:  //1 ページ左へスクロール  （シャフト
		FaceMove(-viewRect.Width(),0);
		Invalidate(0);
		break;
	case SB_THUMBTRACK://スライダードラッグ中
		bsp->Globalpoint.x = scrollinfo.nTrackPos;   //スライダードラッグ時の位置
		scrollinfo.nPos = bsp->Globalpoint.x;   
		sflag=1;
		break;
	case SB_PAGERIGHT:  //1 ページ右へスクロール  （シャフト
		FaceMove(viewRect.Width(),0);
		Invalidate(0);
		break;
	case SB_LINERIGHT://右へスクロール  （ボタン
		FaceMove(10,0);
		Invalidate(0);
		break;
	}
	
	if(sflag==1){
		scrollinfo.fMask |= SIF_POS;
		SetScrollInfo(SB_HORZ, &scrollinfo , TRUE);
		pos_=1;
		Invalidate(0);
	}
	
//	TRACE("%d %d\n",bsp->Globalpoint.x,bsp->Globalpoint.y);
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CQROQSView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	/*
	SB_TOP  //一番上までスクロール  
	//SB_LINEUP  //1 行上へスクロール  （ボタン
	//SB_PAGEUP  //1 ページ上へスクロール  （シャフト
	//SB_PAGEDOWN  //1 ページ下へスクロール  （シャフト
	//SB_LINEDOWN  //1 行下へスクロール (ボタン 
	SB_BOTTOM://一番下までスクロール
	SB_ENDSCROLL  //スクロール終了  
	//SB_THUMBTRACK://スライダードラッグ中
	SB_THUMBPOSITION //絶対位置へスクロール  （バーが離された
	*/
	
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo);
	int sflag=false;
	
	switch(nSBCode){
	case SB_LINEUP:  //1 行上へスクロール  （ボタン
		FaceMove(0,-10);
		Invalidate(0);
		break;
	case SB_PAGEUP:  //1 ページ上へスクロール  （シャフト
		FaceMove(0,-viewRect.Height());
		Invalidate(0);
		break;
	case SB_THUMBTRACK://スライダードラッグ中        
		bsp->Globalpoint.y = scrollinfo.nTrackPos;    //スライダードラッグ時の位置
		scrollinfo.nPos = bsp->Globalpoint.y;   
		sflag=1;
		break;
	case SB_PAGEDOWN:  //1 ページ下へスクロール  （シャフト
		FaceMove(0,viewRect.Height());
		Invalidate(0);
		break;
	case SB_LINEDOWN:  //1 行下へスクロール (ボタン 
		FaceMove(0,10);
		Invalidate(0);
		break;
	}
	
	if(sflag==1){
		scrollinfo.fMask |= SIF_POS;
		SetScrollInfo(SB_VERT, &scrollinfo , TRUE);
		pos_=1;
		Invalidate(0);
	}
	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CQROQSView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	fprintf(tfp,"CQROQSView::OnSize %d\n",bsp);
//	TRACE("CQROQSView::OnSize\n");


	if(bsp==NULL) return;
	

	GetClientRect(viewRect);
//	TRACE("%d,%d\n",cx,cy);
//	TRACE("viewRect %d %d %d %d\n",viewRect.left,viewRect.top,viewRect.right,viewRect.bottom);	
		
	//DIBで扱えるように幅を４の倍数とする
	if(viewRect.right%4) viewRect.right += (4 - viewRect.right%4);


	ScrollSet();

	//position faceの初期化
	if(Posface != NULL) delete[] Posface;
	Posface = new int[viewRect.Width()*viewRect.Height()];
	
	oudRect = viewRect;
	nudRect = viewRect;
	
	BITMAPINFOHEADER m1BmInfoHed;
	BITMAPINFO m1BmInfo;

	ZeroMemory(&m1BmInfoHed, sizeof(BITMAPINFOHEADER));
	
	m1BmInfoHed.biSize=sizeof(BITMAPINFOHEADER); /* 構造体の大きさ */
	m1BmInfoHed.biWidth=viewRect.Width();                     /* 横幅 */
	m1BmInfoHed.biHeight=-viewRect.Height();                    /* 高さ */
	m1BmInfoHed.biPlanes=1;                     /* プレーンの数 */
	m1BmInfoHed.biBitCount=24;                  /* プレーンの色数 */
	m1BmInfoHed.biCompression=BI_RGB;
	
	m1BmInfo.bmiHeader=m1BmInfoHed;	
	
	CBitmap oldbmp;

	m1DC.SelectObject(&oldbmp);
	DeleteObject(m1BMP);
	m1BMP=CreateDIBSection(m1DC,
		&m1BmInfo,DIB_RGB_COLORS,(void**)&m1lpBMP,NULL,NULL);
	m1DC.SelectObject(m1BMP);

#ifndef _LIGHT
	BITMAPINFO *miniBmInfo;
	miniBmInfo=new BITMAPINFO[sizeof(BITMAPINFO)+256*sizeof(RGBQUAD)];

	ZeroMemory(&miniBmInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

	miniBmInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); // 構造体の大きさ 
	miniBmInfo->bmiHeader.biWidth=viewRect.Width();              // 横幅 
	miniBmInfo->bmiHeader.biHeight=-viewRect.Height();         // 高さ 
	miniBmInfo->bmiHeader.biPlanes=1;            //プレーンの数 
	miniBmInfo->bmiHeader.biBitCount=24;          //プレーンの色数 
	miniBmInfo->bmiHeader.biCompression=BI_RGB;
		
	/*
	//カラーテーブルを使う場合
	RGBQUAD *minilpRGB;
	minilpRGB = (RGBQUAD*)(miniBmInfo+sizeof(BITMAPINFOHEADER));
	//miniBmInfo->bmiColors[0]=minilpRGB[0]; // カラーテーブルの先頭
	for(int i=0;i<256;++i){
		minilpRGB[i].rgbRed=5;
		minilpRGB[i].rgbGreen=5;
		minilpRGB[i].rgbBlue=5;
	}
	*/

	miniDC.SelectObject(&oldbmp);
	DeleteObject(miniBMP);
	miniBMP=CreateDIBSection(miniDC,
		miniBmInfo,DIB_RGB_COLORS,(void**)&minilpBMP,NULL,NULL);
	delete[] miniBmInfo;
	miniDC.SelectObject(miniBMP);
#endif

	CClientDC cDC(this);

	bufDC.SelectObject(&oldbmp);
	if(bufBMP!=NULL) delete bufBMP;
	bufBMP = new CBitmap();
	bufBMP->CreateCompatibleBitmap(&cDC, viewRect.Width(), viewRect.Height());
	bufDC.SelectObject(bufBMP);
	


	// メッセージダイアログの移動
	CRect rect;
    GetClientRect(&rect);
	ClientToScreen(&rect);
	rect.top = rect.bottom - 60;
	rect.right = rect.left + 300;
	rect.DeflateRect(5,5);
//	TRACE("(%d,%d)(%d,%d)\n",rect.left,rect.top,rect.right,rect.bottom);
	m_wndTraceDlg->MoveWindow(rect);
	CWnd *tout = m_wndTraceDlg->GetDlgItem(IDC_OUTPUTMSG);
	if(tout!=NULL){
	    m_wndTraceDlg->GetClientRect(&rect);
		rect.DeflateRect(5,5);
		tout->MoveWindow(rect);
	}


	pos_ = 1;

	Invalidate(0);
}


void CQROQSView::FitView()
{

	bsp->NormalizeGlobalRect();

//	fprintf(tfp,"viewRect (%d,%d)",viewRect.Width(),viewRect.Height());
	if(viewRect.Width()==0){
		// スクロールバーが正常に動作するように、viewRectに値を入れておく
		viewRect.SetRect(0,0,600,400);
	}

	if(bsp->ObjMap.size()!=0){
		//ファイルを開いたら中心を表示
		bsp->Globalpoint.x = (bsp->globalRect.left + bsp->globalRect.right  - viewRect.Width())  >> 1;
		bsp->Globalpoint.y = (bsp->globalRect.top  + bsp->globalRect.bottom - viewRect.Height()) >> 1;
	}
	else{
		bsp->Globalpoint.x=0;
		bsp->Globalpoint.y=0;
	}

	if(bsp->globalRect.Width()==0 || viewRect.Height()==0
		|| bsp->globalRect.Width()==0 || bsp->globalRect.Height()==0){
		return;
	}


	bsp->globalRect.InflateRect(50,50);

	viewDiam = zoom_diam(viewRect.Width(), viewRect.Height(), bsp->globalRect.Width(), bsp->globalRect.Height());
//	TRACE("vd %f\n",viewDiam);
	OnMouseWheel(0,0,0);

}


void CQROQSView::ScrollSet()
{

//	TRACE("set scroll\n");

//	TRACE("view  (%d,%d)(%d,%d)\n",viewRect.left,viewRect.top,viewRect.right,viewRect.bottom);	
	//TRACE("grect,(%d,%d)(%d,%d)\n",bsp->globalRect.left,bsp->globalRect.top,bsp->globalRect.right,bsp->globalRect.bottom);
//	TRACE("gpoint (%d,%d)\n",bsp->Globalpoint.x,bsp->Globalpoint.y);
	//Globalpointに合わせて拡張
	
	if(bsp->Globalpoint.x + viewRect.Width()  > bsp->globalRect.right){
		bsp->globalRect.right = bsp->Globalpoint.x + viewRect.Width() ;
	}
	else if(bsp->Globalpoint.x  < bsp->globalRect.left){
		bsp->globalRect.left = bsp->Globalpoint.x;
	}
	if(bsp->Globalpoint.y + viewRect.Height() > bsp->globalRect.bottom){
		bsp->globalRect.bottom = bsp->Globalpoint.y + viewRect.Height() ;
	}
	else if(bsp->Globalpoint.y  < bsp->globalRect.top){
		bsp->globalRect.top = bsp->Globalpoint.y;
	}
	

//	TRACE("global rect (%d,%d) (%d,%d)\n",bsp->globalRect.left,bsp->globalRect.top,bsp->globalRect.right,bsp->globalRect.bottom);
//	TRACE("global(%d,%d) view(%d,%d)\n",bsp->globalRect.Width(),bsp->globalRect.Height(),viewRect.Width(),viewRect.Height());	


	if(bsp->globalRect.Width() < viewRect.Width()){
		bsp->globalRect.left -= viewRect.Width()>>1;
		bsp->globalRect.right += viewRect.Width()>>1;
	}
	if(bsp->globalRect.Height() < viewRect.Height()){
		bsp->globalRect.top -= viewRect.Height()>>1;
		bsp->globalRect.bottom += viewRect.Height()>>1;
	}


	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_HORZ, &scrollinfo);
	scrollinfo.nMin = bsp->globalRect.left;
	scrollinfo.nMax = bsp->globalRect.right;
	scrollinfo.nPage = viewRect.Width();	//ページ幅
	scrollinfo.nPos = bsp->Globalpoint.x;
	scrollinfo.fMask |= SIF_ALL ;//| SIF_DISABLENOSCROLL;
	SetScrollInfo(SB_HORZ, &scrollinfo, true);


	GetScrollInfo(SB_VERT, &scrollinfo);
	scrollinfo.nMin = bsp->globalRect.top;
	scrollinfo.nMax = bsp->globalRect.bottom;
	scrollinfo.nPage = viewRect.Height();	//ページ幅
	scrollinfo.nPos = bsp->Globalpoint.y;
	scrollinfo.fMask |= SIF_ALL ;//| SIF_DISABLENOSCROLL;
	SetScrollInfo(SB_VERT, &scrollinfo, true);
	
}

void CQROQSView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	

	//最前面へ
	::SetForegroundWindow(this->GetSafeHwnd());
//	ForegroundWindow(this->GetSafeHwnd());

	CQROQSDoc *pDoc = GetDocument();
	if(pDoc->SimHalt()==false) return;
	if(pDoc->SaveModifiedBSP()==IDCANCEL) return;


	//全てクリア
	viewReset();

	undo_bsp.clear();
	
	KillTimer(_TID_OBJBEE);


	//読み込み
	//	TRACE("Drop\n");
	char buf[500];
	DragQueryFile(hDropInfo,0,buf,sizeof(buf)-1);

	try{
		pDoc->OnOpenDocument( buf );

		string fpath=buf;
		fpath.erase( fpath.rfind('\\') );
		pDoc->filepath = fpath.c_str();

	}
	catch(char *err){
		Trace(err);
	}
	DragFinish(hDropInfo);
	//	TRACE("open ok\n");
	
	TraceClear();
	pos_ = 1;

	FitView();
	
	CView::OnDropFiles(hDropInfo);
}

int CQROQSView::PosFaceSearch(CPoint point)
{
	if(!viewRect.PtInRect(point)) return NULL;
	
	int putok;
	
	putok = *(Posface 
		+ point.y * viewRect.Width()
		+ point.x);

	return putok;
}

void CQROQSView::FaceMove(int mx,int my)
{
//	TRACE("move %d %d\n",mx,my);

	//ここでOldpointは変更するな
	//スクロールバーが直接操作された場合と競合するので

	CPoint p = Oldpoint;
	p.x -= mx;
	p.y -= my;
	
	FaceMove(p);
}

void CQROQSView::FaceMove(CPoint point)
{
//	TRACE("face st %d %d\n",Oldpoint.x,Oldpoint.y);

	//画面の移動
//	TRACE("old %d %d\n",Oldpoint.x,Oldpoint.y);
//	TRACE("pnt %d %d\n",point.x,point.y);
	float vdiam=1;
#ifndef _LIGHT
	if(pressKey==VK_SPACE) vdiam=shadowDiam;
	else
#endif
		vdiam=viewDiam;

	if(Oldpoint.x != point.x){
		bsp->Globalpoint.x += (Oldpoint.x - point.x)/vdiam;
	}
	if(Oldpoint.y != point.y){
		bsp->Globalpoint.y += (Oldpoint.y - point.y)/vdiam;
	}
	if(point!=Oldpoint){
		ScrollSet();
		Oldpoint = point;
	}

	pos_=1;
}

#include"TokenfuncDlg.h"

//トランジションの入力変数へ接続する
void CQROQSView::OnTraInsort()
{
//	TRACE("in var %d\n",tra_io_var);
	if(tra_io_var==0){
		//ダイアログ
		CTokenfuncDlg tofDlg;
		tofDlg.m_input = "";
		tofDlg.m_str = "入力変数を記入してください．";
		if(tofDlg.DoModal()==IDOK){
			vArcVal = tofDlg.m_input;
		}
		else return;
	}
	else{
		int obj_type;
		obj_type = bsp->ObjMap[vArcFollow].type;
		if(obj_type==_TRANSITION){
			vArcVal = find_by_value(
				bsp->TranMap[vArcFollow]->in_var.begin()
				,bsp->TranMap[vArcFollow]->in_var.end()
				,tra_io_var)
				->first;
		}
		else if(obj_type==_MODULE){
			BSPetri *a_bsp = bsp_tree->GetBSP( bsp->ModMap[vArcFollow].c_str() );
			map<int,bspUhzi*>::iterator uit;
			uit = a_bsp->UhziMap.begin();
			for(int i=1;uit!=a_bsp->UhziMap.end();++uit){
				if(!uit->second->input_name.empty()){
					if(i==tra_io_var) break;
					i++;
				}
			}
			vArcVal = uit->second->input_name;
		//	TRACE("n %d\n",uit->first);
		}
	}

//	TRACE("%s\n",vArcVal.c_str());

	UndoSave();
	ArcChain();
	
	tra_io_var=0;

	Invalidate(0);
}

void CQROQSView::OnTraOutsort()
{
//	TRACE("out var %d\n",tra_io_var);
	
//	TRACE("PutIt %d\n",PutIt);

	vArcFollow = NULL;
	vArcBase = PutIt;//接続元
	varcBwt.clear();
	ArcPic = TRUE;

	if(tra_io_var==0){
		//ダイアログ
		CTokenfuncDlg tofDlg;
		tofDlg.m_input = "";
		tofDlg.m_str = "出力変数を記入してください．";
		if(tofDlg.DoModal()==IDOK){
			vArcVal = tofDlg.m_input;
		}
		else{
			OnKeyDown(VK_ESCAPE,0,0);
			return;
		}
	}
	else{
		int obj_type;
		obj_type = bsp->ObjMap[vArcBase].type;
		if(obj_type==_TRANSITION){
			vArcVal = 
				find_by_value(
				bsp->TranMap[vArcBase]->out_var.begin()
				,bsp->TranMap[vArcBase]->out_var.end()
				,tra_io_var)
				->first;
			if(now_command==_CHAIN){
				SelectOBJ=_PLACE;
			}
		}
		else if(obj_type==_MODULE){
			BSPetri *a_bsp = bsp_tree->GetBSP( bsp->ModMap[vArcBase].c_str() );
			map<int,bspKemmakix*>::iterator kit;
			kit = a_bsp->KemMap.begin();
			for(int o=1;kit!=a_bsp->KemMap.end();++kit){
				if(!kit->second->output_name.empty()){
					if(o==tra_io_var) break;
					o++;
				}
			}
			vArcVal = kit->second->output_name;
		}
	}

//	TRACE("%s\n",vArcVal.c_str());

	StatusOut("変数 %s の出力ﾌﾟﾚｰｽを選択してください",vArcVal.c_str());

	tra_io_var=0;
}


BOOL CQROQSView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	HCURSOR cursor=NULL;
	if(now_command==_ARC || now_command==_CHAIN){
		cursor = AfxGetApp()->LoadCursor(IDC_CUR_ARC);
	}
	else if(now_command==_BTOKEN){
		cursor = AfxGetApp()->LoadCursor(IDC_CUR_TOKEN);
	}
	else if(now_command==_HAND){
		if(HandlingTrack==true){
			cursor = AfxGetApp()->LoadCursor(IDC_CUR_HAND_HOLD);
		}
		else if(PutIt!=0){
			cursor = AfxGetApp()->LoadCursor(IDC_CUR_HAND);
		}
		else{
			cursor = LoadCursor(NULL,IDC_CROSS);
		}
	}
	else if(zoomrect_flag){
		cursor = LoadCursor(NULL,IDC_CROSS);
	}

	if(cursor!=NULL){
		SetCursor(cursor);
		return 0;
	}

	//これによりカーソルを自動的に元に戻さない
//	return 0;

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CQROQSView::OnComZoomrect() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	viewReset();
	zoomrect_flag = 1;

	StatusOut("拡大表示したい領域をﾄﾗｯｷﾝｸﾞしてください");

}

void CQROQSView::ZoomRect(CPoint point)
{
	tracker.TrackRubberBand(this, point);
	tracker.m_rect.NormalizeRect();
	//	TRACE("rabber\n");
	
	//TRACE("(%d,%d)(%d,%d)\n",tracker.m_rect.left,tracker.m_rect.top,tracker.m_rect.right,tracker.m_rect.bottom);
	CPoint mtlp;
	mtlp = mDiampoint(viewDiam, tracker.m_rect.TopLeft());
	CPoint mbrp;
	mbrp = mDiampoint(viewDiam, tracker.m_rect.BottomRight());
	//	TRACE("go0\n");

	viewDiam = 1.0;
	OnMouseWheel(0,0,0);

	CPoint cp;
	cp.x = (mtlp.x + mbrp.x) / 2 - viewRect.Width() / 2;
	cp.y = (mtlp.y + mbrp.y) / 2 - viewRect.Height() / 2;
	FaceMove(cp.x,cp.y);

	viewDiam = zoom_diam(viewRect.Width(), viewRect.Height(), mbrp.x-mtlp.x, mbrp.y-mtlp.y);
	OnMouseWheel(0,0,0);

	zoomrect_flag=0;

	Invalidate();

	StatusOut("ﾚﾃﾞｨ");

	return;
}


/// EOF ///


