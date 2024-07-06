// QROQSView.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQROQSView

#include"other.h"


/////////////////////////////////////////////////////////////////////////////
// CQROQSView クラスの描画

#define VIEW_TEXT 0.6
void CQROQSView::ArcDraw(bspArc *arc, CPoint inp, CPoint outp, vector<CPoint> *varcp)
{

	CPoint bp;
	vector<CPoint>::iterator abp,abp_end;

	inp -= bsp->Globalpoint;
	outp -= bsp->Globalpoint;

	//TRACE("varcp %d\n",varcp);
	//途中つなぎがあるなら
	if(varcp!=NULL){
		abp = varcp->begin();
		//	TRACE("bwtuuu x %d\n",varcp->GetAt(p2).x);
		CPoint bp2 = (*abp) - bsp->Globalpoint;
		++abp;

		//bp = Betwixtpoint(inp,bp2,0,bsp->vObjMap[arc->vIn].type==_TRANSITION?10:20);
		bp = inp;
		
		//	TRACE("bwt\n");
		if(viewDiam>=0.5){
			Yajirushi(&m1DC, Diampoint(viewDiam,bp), Diampoint(viewDiam,bp2), M_PI/6, 10*viewDiam);
		}
		else{
			m1DC.MoveTo(Diampoint(viewDiam,bp));
			m1DC.LineTo(Diampoint(viewDiam,bp2));
		}
		
		//トランジションから出ているアークの名前（経由点ありの場合）
		if(viewDiam>=VIEW_TEXT && 
			(bsp->ObjMap[arc->In].type==_TRANSITION
			|| bsp->ObjMap[arc->In].type==_MODULE)
			){
			PrintArcValue(&m1DC, bp, bp2, arc->value_name.c_str());
		}
		
		bp=bp2;
		abp_end = varcp->end();
		for(;abp!=abp_end;++abp){
			bp2 = *abp - bsp->Globalpoint;
			if(viewDiam>=0.5){
				Yajirushi(&m1DC, Diampoint(viewDiam,bp), Diampoint(viewDiam,bp2), M_PI/6, 10*viewDiam);
			}
			else{
				m1DC.MoveTo(Diampoint(viewDiam,bp));
				m1DC.LineTo(Diampoint(viewDiam,bp2));
			}
			bp = bp2;
		}
		//TRACE("varcp end\n");
	}
	else{
		//bp = Betwixtpoint(inp,outp,0,(bsp->vObjMap[arc->vIn].type==_TRANSITION?10:20)>>viewDiam);
		bp = inp;
	}
	outp = Betwixtpoint(bp,outp,1,(bsp->ObjMap[arc->Out].type==_TRANSITION?8:20)*viewDiam);
	if(viewDiam>=0.5){
		Yajirushi(&m1DC, Diampoint(viewDiam,bp), Diampoint(viewDiam,outp), M_PI/6, 10*viewDiam);
	}
	else{
		m1DC.MoveTo(Diampoint(viewDiam,bp));
		m1DC.LineTo(Diampoint(viewDiam,outp));
	}
	
	if(viewDiam>=VIEW_TEXT && (
		(varcp==NULL && (bsp->ObjMap[arc->In].type==_TRANSITION || bsp->ObjMap[arc->In].type==_MODULE))
			|| bsp->ObjMap[arc->Out].type==_TRANSITION
			|| bsp->ObjMap[arc->Out].type==_MODULE
			)
		){
		//トランジションから出ているアークの名前（経由点なしの場合）
		//もしくはトランジションに入っているアークの名前（経由点あってもなくても）
		PrintArcValue(&m1DC, bp, outp, arc->value_name.c_str());
	}

}

void TransRect(LPBYTE lpBMP, int sx, int sy, int ex, int ey, int Width, int Height, int coladd)
{
	if(sx<0) sx = 0;
	if(ex>=Width) ex=Width-1;
	if(sy<0) sy=0;
	if(ey>=Height) ey=Height-1;
	int x,y,r,g,b,pos;
	for(y=sy;y<=ey;y++){
		for(x=sx;x<=ex;x++){
			pos = (y * Width + x)*3;
			b = lpBMP[pos]	+ coladd;
			g = lpBMP[pos+1]+ coladd;
			r = lpBMP[pos+2]+ coladd;
			if(r>255) r=255;
			if(g>255) g=255;
			if(b>255) b=255;
			lpBMP[pos]	= b;
			lpBMP[pos+1]	= g;
			lpBMP[pos+2]	= r;
		}
	}
}

void CQROQSView::OnDraw(CDC* pDC)
{
	CQROQSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
//	fprintf(tfp,"CQROQSView::OnDraw doc %d\n",pDoc->docst_);
//	TRACE("CQROQSView::OnDraw doc %d\n",pDoc->docst_);


//TRACE("grect,(%d,%d)(%d,%d)\n",bsp->globalRect.left,bsp->globalRect.top,bsp->globalRect.right,bsp->globalRect.bottom);
	/*		
#ifdef _DEBUG
	bsp->obj_check();TRACE("check\n");
#endif
*/

	if(bsp==NULL) return;

	int i;
	//オブジェクト
	int obj_type;
	CPoint gp,mp;
	int x,y;
	CRect rect;
	



	if(pDC->IsPrinting() && PrintColor==0/* || PetMode==_PRINT*/){
		//クリア
		m1DC.FillSolidRect(0,0,viewRect.Width(),viewRect.Height(),RGB(255,255,255));
	}
#ifdef _LIGHT
	else{
		m1DC.FillSolidRect(0,0,viewRect.Width(),viewRect.Height(),vColMap[_BACK_COL]);
#else
	else if(back_size.cx){ //背景画像
		
		int i,j;
		//TRACE("%d,%d\n",back_size.cx,back_size.cy);
		//最大縮小と同じ値にすること
		i= -(((bsp->Globalpoint.x>>6)%back_size.cx));
		j= -(((bsp->Globalpoint.y>>6)%back_size.cy));
		for(y=j-back_size.cy;y<viewRect.bottom;y+=back_size.cy){
			for(x=i-back_size.cx;x<viewRect.right;x+=back_size.cx){
				m1DC.BitBlt(x,y,back_size.cx,back_size.cy,&backDC,0,0,SRCCOPY);
			}
		}
		
#endif //non _LIGHT

		//ノーマルハッチ
		if(viewDiam>0.25 && confEditGridType!=2){
			m1DC.SelectObject(pen_hatch);

			CPoint tsp,tep;
			tsp = mDiampoint(viewDiam,viewRect.TopLeft());
			tep = mDiampoint(viewDiam,viewRect.BottomRight());

			if(confEditGridType==0){
				int st,ed;
				CPoint sp,ep;
				sp = tsp; ep = tep;
				ed = ep.x;
				st = ep.x = sp.x = sp.x - ((sp.x+bsp->Globalpoint.x) % HatchSize) - HatchSize;
				for(;st<ed;st+=HatchSize){
					sp.x += HatchSize;
					ep.x += HatchSize;
					m1DC.MoveTo(Diampoint(viewDiam,sp));
					m1DC.LineTo(Diampoint(viewDiam,ep));
				}
				sp = tsp; ep = tep;
				ed = ep.y;
				st = ep.y = sp.y = sp.y - ((sp.y+bsp->Globalpoint.y) % HatchSize) - HatchSize;
				for(;st<ed;st+=HatchSize){
					sp.y += HatchSize;
					ep.y += HatchSize;
					m1DC.MoveTo(Diampoint(viewDiam,sp));
					m1DC.LineTo(Diampoint(viewDiam,ep));
				}
			}
			else if(confEditGridType==1){

				CRect pRect;
				int sx,sy,ex,ey;
				int sxt, inf;
				inf = 2 * viewDiam;
				CPoint cp;
				sy = tsp.y - ((tsp.y+bsp->Globalpoint.y) % HatchSize) - HatchSize;
				ey = tep.y;
				sxt = tsp.x - ((tsp.x+bsp->Globalpoint.x) % HatchSize) - HatchSize;
				for(;sy<ey;sy+=HatchSize){
					sx = sxt;
					ex = tep.x;
					for(;sx<ex;sx+=HatchSize){
						cp.x = sx;
						cp.y = sy;
						cp = Diampoint(viewDiam,cp);
						pRect.SetRect(cp, cp);
						pRect.InflateRect(inf,inf);
						m1DC.FillSolidRect(pRect, vColMap[_GLID_COL]);
					}
				}
			}

		}
	}


	//アーク
	CPoint inp,outp,bp,bp2;

	vector<CPoint> *varcp;
	
	vector<CPoint>::iterator abp;
	map<int,bspArc>::iterator ait,ait_end;

	map<int,bspObject>::iterator oit,oit_end;
/*
	// オブジェクトのチェック
	oit_end = bsp->ObjMap.end();
	for(oit=bsp->ObjMap.begin(); oit!=oit_end; ++oit){
		TRACE("objID %d\n", oit->first);
		if(oit->second.type==0){
			TRACE("objs %d\n", bsp->ObjMap.size());
			throw "none obj";
		}
	}
*/

#ifndef _LIGHT
	//TRACE("sha %d\n",shadowDiam);
	//シャドウ
	if(shadowDiam<viewDiam){


		miniDC.FillSolidRect(0,0,viewRect.Width(),viewRect.Height(),RGB(0,0,0));
		
		CPen pen,*oldpen;
		pen.CreatePen(PS_SOLID,1,RGB(255,255,255));
		oldpen=miniDC.SelectObject(&pen);
		
		miniDC.SelectStockObject(NULL_BRUSH);
		bp.x =0;
		bp.y =0;
		//miniDC.SetPixel(bp,RGB(255,255,255));
		bp = sDiampoint(shadowDiam,bp);
		miniDC.Rectangle( bp.x, bp.y, bp.x+(viewRect.Width()*shadowDiam), bp.y+(viewRect.Height()*shadowDiam));
		//TRACE("m %d\n",minilpBMP[0]);
		//pDC->BitBlt(0,0,viewRect.Width(),viewRect.Height(),&miniDC,0,0,SRCCOPY);
		//return;
		CBrush brush,*oldbrush;
		brush.CreateSolidBrush(RGB(255,255,255));
		oldbrush=miniDC.SelectObject(&brush);

		//オブジェクトのシャドウ
		oit=bsp->ObjMap.begin();
		oit_end = bsp->ObjMap.end();
		for(;oit!=oit_end;++oit){
			gp = oit->second.gpoint - bsp->Globalpoint;
			mp = sDiampoint(shadowDiam,gp);
			mp = sCtoLUpoint(mp);
			PutObjectMini(&miniDC, mp.x, mp.y, oit->second.type);
		}

		vector<CPoint>::iterator abp_end;
		//アークのシャドウ
		ait=bsp->ArcMap.begin();
		ait_end = bsp->ArcMap.end();
		for(;ait!=ait_end;++ait){
			bspArc &arc = ait->second;
			inp = bsp->ObjMap[arc.In].gpoint - bsp->Globalpoint;
			outp = bsp->ObjMap[arc.Out].gpoint - bsp->Globalpoint;
			
			varcp = arc.bwtpoint;
			miniDC.MoveTo(sDiampoint(shadowDiam,inp));
			if(varcp!=NULL){
				abp = varcp->begin();
				bp = (*abp)-bsp->Globalpoint;
				++abp;
				miniDC.LineTo(sDiampoint(shadowDiam,bp));
				abp_end = varcp->end();
				for(;abp!=abp_end;++abp){
					bp = (*abp) - bsp->Globalpoint;
					miniDC.LineTo(sDiampoint(shadowDiam,bp));
				}
			}
			miniDC.LineTo(sDiampoint(shadowDiam,outp));
		}

		miniDC.SelectObject(oldpen);
		miniDC.SelectObject(oldbrush);
	
		//透過合成
		if(pressKey!=VK_SPACE){
			int r,g,b,j,i;
			for(i=0,j=0; i<viewRect.Height()*viewRect.Width(); ++i,j+=3){
				if(minilpBMP[j]!=0){
					//ノーマル用
					b = (int)m1lpBMP[j] + 100;//minilpBMP[j]/4;
					if(b>255) b=255;
					m1lpBMP[j] = b;
					g = (int)m1lpBMP[j+1] - 30;
					if(g<0) g = 0;
					m1lpBMP[j+1] = g;
					r = (int)m1lpBMP[j+2] - 30;
					if(r<0) r = 0;
					m1lpBMP[j+2] = r;
				}
			}
		}
	}
#endif // non _LIGHT
	
	CPoint mtlp,mbrp;

	// ローカル四角
	if(viewDiam<0.5){
		m1DC.SelectStockObject(WHITE_PEN);
		m1DC.SelectStockObject(NULL_BRUSH);
		mtlp = Diampoint(viewDiam, viewRect.TopLeft());
		mbrp = Diampoint(viewDiam, viewRect.BottomRight());
		m1DC.Rectangle(mtlp.x, mtlp.y, mbrp.x, mbrp.y);
	}

	m1DC.SelectObject(&vFont);
	m1DC.SetTextColor(vColMap[_TEXT_COL]);
	
	//TRACE("arc draw\n");
	//アークの配置

	mtlp.x=0;
	mtlp.y=0;
	mtlp=mDiampoint(viewDiam,mtlp) + bsp->Globalpoint;
	mbrp=mDiampoint(viewDiam,viewRect.BottomRight()) + bsp->Globalpoint;
	rect.SetRect(mtlp, mbrp); // グローバルでの表示領域

//	TRACE("ain %d\n",bsp->vArcInMap.size());
//	TRACE("arc %d\n",bsp->vArcMap.size());
//	TRACE("aot %d\n",bsp->vArcOutMap.size());
//	TRACE("ondrawarc obj %d\n",bsp->vObjMap.size());

	set<int> select_set;

	bspArc *arc;
	//新アーク
	m1DC.SelectObject(pen_arc);
	multimap<int,int>::iterator aps,ape;
	ait = bsp->ArcMap.begin();
	ait_end = bsp->ArcMap.end();
	for(;ait!=ait_end;++ait){
		arc = &( ait->second );
		inp = bsp->ObjMap[arc->In].gpoint;
		outp = bsp->ObjMap[arc->Out].gpoint;
		varcp = arc->bwtpoint;

		//はみ出し判定
		if(varcp==NULL
			&& !rect.PtInRect(inp)
			&& !rect.PtInRect(outp)
			&& abs(outp.x-inp.x)<80
			&& abs(outp.y-inp.y)<80
			){
			continue;
		}

		// 選択されているオブジェクトならアークの色を変える
		if(bsp->intrack.find(arc->In)!=bsp->intrack.end()
			|| bsp->intrack.find(arc->Out)!=bsp->intrack.end()){
			select_set.insert(ait->first);
		}
		else{
			ArcDraw(arc, inp, outp, varcp);
		}
	}
	// 選択されているオブジェクトならアークの色を変える
	m1DC.SelectObject(pen_arc_select);
	set<int>::iterator sit,sit_end;
	sit = select_set.begin();
	sit_end = select_set.end();
	for(;sit!=sit_end;++sit){
		arc = &( bsp->ArcMap[ *sit ] );
		inp = bsp->ObjMap[arc->In].gpoint;
		outp = bsp->ObjMap[arc->Out].gpoint;
		varcp = arc->bwtpoint;

		ArcDraw(arc, inp, outp, varcp);
	}


//	TRACE("ondrawbwt obj %d\n",bsp->vObjMap.size());
	//アークのつなぎ途中
	//TRACE("tunagi\n");
	if(varcBwt.size()){
		m1DC.SelectObject(pen_arc);
		inp = bsp->ObjMap[vArcBase].gpoint - bsp->Globalpoint;
		abp = varcBwt.begin();
		outp = *abp - bsp->Globalpoint;
		++abp;
		inp = Betwixtpoint(inp,outp,0,20*viewDiam);
		Yajirushi(&m1DC, Diampoint(viewDiam,inp), Diampoint(viewDiam,outp), M_PI/6, 10*viewDiam);
		if(viewDiam>=VIEW_TEXT && (
			bsp->ObjMap[vArcBase].type==_TRANSITION
			|| bsp->ObjMap[vArcBase].type==_MODULE
			)
			){
			PrintArcValue(&m1DC, inp, outp, vArcVal.c_str());
		}
		for(;abp!=varcBwt.end();++abp){
			inp = outp;
			outp = *abp - bsp->Globalpoint;
			Yajirushi(&m1DC, Diampoint(viewDiam,inp), Diampoint(viewDiam,outp), M_PI/6, 10*viewDiam);
		}
	}
	//TRACE("tunagi end\n");


	if(pos_==1){
		memset(Posface,0,sizeof(int)*viewRect.Width()*viewRect.Height());
	}
	
	//おぶじぇくとの配置(プレ、トラ、ウー、ケム）

//	TRACE("ondrawobj obj %d\n",bsp->vObjMap.size());

	CString st;

	mtlp.x=0;
	mtlp.y=0;
	mtlp=mDiampoint(viewDiam,mtlp) + bsp->Globalpoint;
	mbrp=mDiampoint(viewDiam,viewRect.BottomRight()) + bsp->Globalpoint;
	rect.SetRect( mtlp, mbrp);
	rect.InflateRect(20*viewDiam,20*viewDiam);


	// ﾄｰｸﾝを表示するか
	int e_view_token = 0;
	// ｳｰｼﾞの値を表示するか
	int e_view_uhzi = 0;
	if(PetMode==_EDIT || (PetMode==_SIMULATION && !compOk)){
		if(viewDiam>=0.125) e_view_token = 1;
		if(viewDiam>=0.5  )	e_view_uhzi  = 1;
	}


	CPoint cp;
	int opt, obj_id;

	oit=bsp->ObjMap.begin();
	bspObject *obj;
	oit_end = bsp->ObjMap.end();
	for(;oit!=oit_end;++oit){

		obj_id = oit->first;
		obj = &(oit->second);

		obj_type = obj->type;
		// 無効オブジェクト回避
	//	if(obj_type==_NONE) continue;

		gp = obj->gpoint;
		//画面からはみ出したら表示しない
		if(!rect.PtInRect(gp)) continue;

		//グローバル座標からローカル座標へ変換しろ
		mp = gp - bsp->Globalpoint;
		cp = Diampoint(viewDiam, mp);
		mp = CtoLUpoint(cp);

		//positionfaceの設定
		if(pos_==1){
			inp = mp;
			outp.x = inp.x + vChipSize;
			outp.y = inp.y + vChipSize;
			if(inp.x<0) inp.x=0;
			else if(outp.x>viewRect.Width()) outp.x=viewRect.Width();
			if(inp.y<0) inp.y=0;
			else if(outp.y>viewRect.Height()) outp.y=viewRect.Height();
			for(y=inp.y; y<outp.y; ++y){
				for(x=inp.x; x<outp.x; ++x){
					Posface[y*viewRect.Width()+x] = obj_id;
				}
			}
		}

		opt = 0;
		if(obj_type==_TRANSITION){
			if(!bsp->TranMap[ obj_id ]->pass_tra){
				opt = 1;
			}
		}

#ifdef _LIGHT
		PutObject(&m1DC, mp.x, mp.y, obj_type, opt, bsp->intrack.find(obj_id)!=bsp->intrack.end());
#else
		if(SkinUse==1){
		//skin透過配置
			m1DC.BitBlt(mp.x,mp.y,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,obj_type*V_CHIP_SIZE,V_CHIP_SIZE*2,SRCAND);
			m1DC.BitBlt(mp.x,mp.y,V_CHIP_SIZE*viewDiam,V_CHIP_SIZE*viewDiam,&objDC,obj_type*V_CHIP_SIZE,V_CHIP_SIZE,SRCINVERT);
		}
		else{
			PutObject(&m1DC, mp.x, mp.y, obj_type, opt, bsp->intrack.find(obj_id)!=bsp->intrack.end());
		}
#endif

		if(ArcPic==true && obj_id==vArcBase){//アーク元なら囲う
			m1DC.SelectObject(&pen_wred5);
			Yard(&m1DC, cp, vChipSize+4);
		}

		//ウージの値の表示
		if(e_view_uhzi && obj_type==_UHZI){
			//	TRACE("uhii %d\n",up);
			bsp->UhziMap[obj_id]->ValuePrint(&m1DC, mp, viewDiam, 1, confDataViewMax);
		}

		// ﾄｰｸﾝの表示
		if(e_view_token && obj_type==_PLACE){
			map<int, bspToken>::iterator tokit;
			if((tokit=bsp->TokenMap.find(obj_id))!=bsp->TokenMap.end()){
				m1DC.SetTextColor(RGB(255,255,255));
				ViewToken(&m1DC, cp
					,tokit->second.Color
					,tokit->second.Value);
				m1DC.SetTextColor(vColMap[_TEXT_COL]);
			}
		}

		//右下情報
		if(viewDiam>=VIEW_TEXT){
			const char *ch = NULL;
#ifndef _LIGHT
			if(obj_type==_TRANSITION){
			//	if(bsp->TranMap[op->first].non_reg==1) ch = "R省略";
			}
			else 
#endif
			if(obj_type==_UHZI){
				//	TRACE("uhii %d\n",up);
				if(!bsp->UhziMap[obj_id]->input_name.empty()){
					ch = bsp->UhziMap[obj_id]->input_name.c_str();
				}
			}
			else if(obj_type==_KEMMAKIX){
				//	TRACE("uhii %d\n",up);
				bspKemmakix *kem = bsp->KemMap[obj_id];
				i = kem->dline;
				if(i==2) ch = "破棄!";
				else if(i==3) ch = "消費しない!";
				else if(!kem->output_name.empty()){
					ch = kem->output_name.c_str();
				}
			}
			else if(obj_type==_MODULE){
				//	TRACE("uhii %d\n",up);
				if(!bsp->ModMap[obj_id].empty()){
					ch = bsp->ModMap[obj_id].c_str();
				}
			}

			if(ch!=NULL){
				m1DC.TextOut(mp.x+(10*viewDiam),mp.y+(35*viewDiam),ch);
			}
		}

		//オブジェクト番号の表示
		if(EnableObjId==1 && viewDiam>=VIEW_TEXT){
			st.Format("%d",obj_id);
		/*	
			TransRect(m1lpBMP, 
				mp.x+15*viewDiam, mp.y-5*viewDiam, 
				mp.x+(10*st.GetLength()+25)*viewDiam, mp.y+25*viewDiam, 
				viewRect.right, viewRect.bottom, 40);
		*/		
			m1DC.TextOut(mp.x+(20*viewDiam),mp.y,st);
		}
	}
	pos_=0;
	
	// テキストの表示
	if(viewDiam>=0.5){
		m1DC.SelectObject(pen_gray1d);

		map<int,string>::iterator textit;
		textit = bsp->TextMap.begin();
		for(;textit!=bsp->TextMap.end();++textit){
			gp = bsp->ObjMap[ textit->first ].gpoint;
			//グローバル座標からローカル座標へ変換しろ
			mp = gp - bsp->Globalpoint;
			cp = Diampoint(viewDiam,mp);

			mp.x = cp.x - 40*viewDiam;
			mp.y = cp.y - 60*viewDiam;
			m1DC.TextOut(mp.x,mp.y,textit->second.c_str());

			mp.y += 15*viewDiam;
			m1DC.MoveTo(mp);
			m1DC.LineTo(Betwixtpoint(mp,cp,1,20*viewDiam));
		}
	}


	//BSPを見失ったときに矢印を表示する
	if(obj_type==0 && bsp->ObjMap.size()!=0){
		CPen lost_pen;
		lost_pen.CreatePen(PS_SOLID ,18,RGB(56,250,156));
		m1DC.SelectObject(lost_pen);
		inp.x=viewRect.Width()>>1;
		inp.y=viewRect.Height()>>1;
		oit=bsp->ObjMap.end();
		oit--;
		outp=oit->second.gpoint-bsp->Globalpoint;
		outp=Betwixtpoint(inp,outp,0,100);
		outp.x -= (outp.x-inp.x)>>1;
		outp.y -= (outp.y-inp.y)>>1;
		inp -= outp-inp;
		Yajirushi(&m1DC, inp,outp, M_PI/6, 40);
	}

	//トラッキング矩形
	if(PetMode==_EDIT && bsp->intrack.size()!=0){
		mtlp = bsp->trackRect.TopLeft() - bsp->Globalpoint;
		mbrp = bsp->trackRect.BottomRight() - bsp->Globalpoint;
		mtlp = Diampoint(viewDiam,mtlp);
		mbrp = Diampoint(viewDiam,mbrp);
//	TRACE("%d %d %d %d\n",mtlp.x,mtlp.y,mbrp.x,mbrp.y);
		tracker.m_rect.SetRect(mtlp,mbrp);
		tracker.Draw(&m1DC);
#ifndef _LIGHT
		if(SkinUse){
			//トラッキング矩形内ならオブジェクトの色を変える
			set<int>::iterator trp,trp_end;
			trp=bsp->intrack.begin();
			trp_end = bsp->intrack.end();
			for(;trp!=trp_end;++trp){
				gp=bsp->ObjMap[*trp].gpoint-bsp->Globalpoint;
				mp=Diampoint(viewDiam,gp);
				mtlp.x=mp.x-(vChipSize>>1)-1;
				if(mtlp.x<0) mtlp.x=0;
				mtlp.y=mp.y-(vChipSize>>1)-1;
				if(mtlp.y<0) mtlp.y=0;
				mbrp.x=mp.x+(vChipSize>>1)+1;
				if(mbrp.x>viewRect.Width()) mbrp.x=viewRect.Width();
				mbrp.y=mp.y+(vChipSize>>1)+1;
				if(mbrp.y>viewRect.Height()) mbrp.y=viewRect.Height();
				for(y=mtlp.y;y<mbrp.y;++y){
					for(x=mtlp.x;x<mbrp.x;++x){
						//if(m1lpBMP[(y*viewRect.Width()+x)*3+2]<255){
							i=m1lpBMP[(y*viewRect.Width()+x)*3+2]+150;//赤をあげる
							if(i>0xff) i=0xff;
							m1lpBMP[(y*viewRect.Width()+x)*3+2]=i;
							/*
						}
						else{
							i=m1lpBMP[(y*viewRect.Width()+x)*3+1]+150;//緑をあげる
							if(i>0xff) i=0xff;
							m1lpBMP[(y*viewRect.Width()+x)*3+1]=i;
						}
						*/
					}
				}
			}
		} // SkinUse
#endif
	}

#ifndef _LIGHT
	//シャドウを前面に合成
	if(shadowDiam<viewDiam && pressKey==VK_SPACE){
		int b,j,i;
		for(i=0,j=0;i<viewRect.Height()*viewRect.Width();++i,j+=3){
			if(minilpBMP[j]!=0){
				m1lpBMP[j]=255;
				m1lpBMP[j+1]=100;
				m1lpBMP[j+2]=100;
			}
			else{
				b=m1lpBMP[j]+((255-m1lpBMP[j])>>1)+20;
				if(b>0xff) b=0xff;
				m1lpBMP[j]=b;
				b=m1lpBMP[j+1]+((255-m1lpBMP[j+1])>>1)+20;
				if(b>0xff) b=0xff;
				m1lpBMP[j+1]=b;
				b=m1lpBMP[j+2]+((255-m1lpBMP[j+2])>>1)+20;
				if(b>0xff) b=0xff;
				m1lpBMP[j+2]=b;
			}
		}
	}

	//あそびであり
	if(Qmode==1 && bsp->ObjMap.begin()!=bsp->ObjMap.end()){
		i=(rand()%bsp->ObjMap.size())+bsp->ObjMap.begin()->first;
		if(bsp->ObjMap.find(i)!=bsp->ObjMap.end()){
			bsp->ObjMap[i].gpoint.x += rand()%7-3;
			bsp->ObjMap[i].gpoint.y += rand()%7-3;
		}
	}


#endif



#ifdef _DEBUG
	//FPSの計測
	static int fps=0,countfps=0;
	static time_t stt=0;
	if(time(NULL)!=stt){
		stt=time(NULL);
		fps=countfps;
		countfps=0;
	}
	countfps++;
	m1DC.SetTextColor(vColMap[_TEXT_COL]);
	m1DC.SelectStockObject(SYSTEM_FONT);
	CString str;
	str.Format("FPS %d",fps);
	m1DC.TextOut(10,10,str);

//#ifdef _DEBUG

	//オブジェクトの数
	str.Format("Objects %d/%d",bsp->ObjMap.size(),bsp->ObjId);
	m1DC.TextOut(10,30,str);

	//アークの数
	str.Format("Arcs %d/%d",bsp->ArcMap.size(),bsp->ArcId);
	m1DC.TextOut(10,50,str);

	//チェック
	if(bsp->ObjMap.find(0)!=bsp->ObjMap.end()){
		TRACE("OnDraw ゼロオブジェクトが存在しています。内部エラー");
	}
	if(bsp->ArcMap.find(0)!=bsp->ArcMap.end()){
		TRACE("OnDraw ゼロアークが存在しています。内部エラー");
	}

#endif


	bufDC.BitBlt(0,0,viewRect.Width(),viewRect.Height(),&m1DC,0,0,SRCCOPY);

	if(PetMode==_SIMULATION){
		PetInvalidate();
	}

	if(pDC->IsPrinting()){
//		CRect rect;
//		pDC->GetWindow()->GetClientRect(&rect);
//		TRACE("prin %d %d\n",rect.Width(),rect.Height());
//		TRACE("****** view %d %d ********\n",viewRect.Width(),viewRect.Height());

		pDC->StretchBlt(0,0,printRect.Width(),printRect.Height(),&bufDC,0,0,viewRect.Width(),viewRect.Height(),SRCCOPY);
	}
	else{
		pDC->BitBlt(0,0,viewRect.Width(),viewRect.Height(),&bufDC,0,0,SRCCOPY);
	}



//	TRACE("bsps %d\n",bsp_tree->bsps.size());

	/*
	map<string,BSPetri*>::iterator bspit;
	bspit=bsp_tree->bsps.begin();
	for(;bspit!=bsp_tree->bsps.end();++bspit){
		TRACE("	%s\n",bspit->first.c_str());
	}
	*/

	//	TRACE("gp(%d,%d)\n",Globalpoint.x,Globalpoint.y);
//	TRACE("gp(%d,%d)(%d,%d)\n",bsp->globalRect.left,bsp->globalRect.top,bsp->globalRect.right,bsp->globalRect.bottom);
}


BOOL CQROQSView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	return true;//背景をクリアしない
	//	return CView::OnEraseBkgnd(pDC);
}

// モジュールの動作状況を色で表す
inline COLORREF life_color(float data){

	data = 1 - data;
//data: 0.0 - 1.0
	int r=255,g=255;
	if(data <= 0.5){
		if(data<0) data=0;
		g = 100 + 155*data/0.5;
	}
	else{
		if(data>1) data = 1;
		r = 100 + 155 -  155*(data-0.5)/0.5;
	}
//	TRACE("%3d %3d %3d\n",r,g,100);
	return RGB(r,g,100);
}

void CQROQSView::PetInvalidate()
{
	if(!compOk) return;

//	TRACE("petinvali %d\n",rand());

	CPoint tlp, brp;
	tlp = Diampoint(viewDiam, bsp->objRect.TopLeft() - bsp->Globalpoint);
	brp = Diampoint(viewDiam, bsp->objRect.BottomRight() - bsp->Globalpoint);
	tlp.x -= V_CHIP_SIZE;
	tlp.y -= V_CHIP_SIZE;
	brp.x += V_CHIP_SIZE + 300*viewDiam;	// ケムマキの値を全部表示させるため
	brp.y += V_CHIP_SIZE;

//	TRACE("(%d,%d)(%d,%d)\n",tlp.x,tlp.y,brp.x,brp.y);

	//クリア
	bufDC.BitBlt(tlp.x, tlp.y, brp.x-tlp.x, brp.y-tlp.y, &m1DC, tlp.x, tlp.y,SRCCOPY);
	
	bufDC.SetBkMode(TRANSPARENT );
	bufDC.SetTextColor(vColMap[_TEXT_COL]);
	bufDC.SelectObject(&vFont);

	CRect otRect;
	CRect grect;

	CPoint mtlp, mbrp;
	mtlp.x=0;
	mtlp.y=0;
	mtlp=mDiampoint(viewDiam,mtlp) + bsp->Globalpoint;
	mbrp=mDiampoint(viewDiam,viewRect.BottomRight()) + bsp->Globalpoint;
	grect.SetRect( mtlp, mbrp);
	grect.InflateRect(20*viewDiam,20*viewDiam);

	int col;

	int obj_type,obj_id;
	CPoint gp,mp,cp;
	//おぶじぇくとの配置(プレ、トラ、ウー、ケム）
	map<int,bspObject>::iterator oit,oit_end;
	oit = bsp->ObjMap.begin();
	oit_end = bsp->ObjMap.end();
	for(;oit!=oit_end;++oit){

		gp = oit->second.gpoint;

		//画面からはみ出したら表示しない
		if(!grect.PtInRect(gp)){
			continue;
		}
			
		cp=Diampoint(viewDiam,gp-bsp->Globalpoint);
		obj_type = oit->second.type;
		obj_id = oit->first;

		if(obj_type==_PLACE){
			bspPlace *place = bsp->PlaceMap[obj_id];

			//過去通過したトークンの色
			if(EnableOldToken && viewDiam>=0.25){
				bufDC.SelectObject(pen_token);
				col = place->GetOldColor();
				if(col==_BTOKEN) bufDC.SelectObject(brush_token);
				else if(col==_WTOKEN) bufDC.SelectStockObject(WHITE_BRUSH);
//				else throw "err token color";
				bufDC.Ellipse(cp.x-(20*viewDiam), cp.y+(20*viewDiam), cp.x-(8*viewDiam), cp.y+(8*viewDiam));
			}

			if(viewDiam>=0.125 && place->GetTokenFlag()){
				bufDC.SetTextColor(RGB(255,255,255));
				ViewToken(&bufDC, cp
					,place->GetTokenColor()
					,place->GetTokenValue());
			}
		}
		mp = CtoLUpoint(cp);
		//発火トラにしるしをつける
		if(obj_type==_TRANSITION){
			if(bsp->TranMap[obj_id]->Fireable()){
				if(bsp->PlaceMap[bsp->TranMap[obj_id]->InPlaceList[0]]->GetTokenColor()==_BTOKEN){
					bufDC.SelectObject(&pen_wred5);
				}
				else{
					bufDC.SelectObject(&pen_wyellow5);
				}
				Yard(&bufDC,cp,20*viewDiam+2);
			}
		}
		//ウージの値の表示
		else if(obj_type==_UHZI){
			if(viewDiam>=0.25){
				bufDC.SetTextColor(vColMap[_TEXT_COL]);
				bsp->UhziMap[obj_id]->ValuePrint(&bufDC, mp,viewDiam, 0, confDataViewMax);
			}
		}
		//ケムマキの値の表示
		else if(obj_type==_KEMMAKIX){
			if(viewDiam>=0.25){
				bufDC.SetTextColor(vColMap[_TEXT_COL]);
				bsp->KemMap[obj_id]->ValuePrint(&bufDC, mp, viewDiam, _SIMULATION, confDataViewMax);
			}
		}//obj==_KEMMAKIX
		//発火モジュールにしるしをつける
		else if(obj_type==_MODULE && bsp->ModuleMap[obj_id]->traNum){
			float fpt, afpt;
			fpt = (float)bsp->ModuleMap[obj_id]->traFireable / bsp->ModuleMap[obj_id]->traNum;
			afpt = (bsp->ModuleMap[obj_id]->ofpt + fpt) *0.5;
			if(afpt){
				CPen ftpen;
				ftpen.CreatePen(PS_SOLID, 5, life_color(afpt*3));
				bufDC.SelectObject(&ftpen);
				Yard(&bufDC,cp,48*viewDiam);
			}
			bsp->ModuleMap[obj_id]->ofpt = fpt;
		}

	}


/*
	// ﾌﾟﾚｰｽを通過した値を表示する
	POINT c_point;
	GetCursorPos(&c_point);
	mp = c_point;
	ScreenToClient(&mp);
	PutIt = PosFaceSearch(mp);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}
	obj_type=_NONE;
	if(PutIt!=0) obj_type=bsp->ObjMap[PutIt].type;
	static deque<string> place_datas_que;
	static string place_datas;
	static int Pid=0, oldColor=_WTOKEN;
	if(obj_type==_PLACE && (Pid==0 || PutIt==Pid)){
		Pid = PutIt;
		cp = Diampoint(viewDiam,bsp->ObjMap[Pid].gpoint-bsp->Globalpoint);
		bufDC.SelectObject(pen_wred5);
		Yard(&bufDC,cp,44*viewDiam);
		
		if(bsp->PlaceMap[Pid]->GetTokenFlag()){
			col = bsp->PlaceMap[Pid]->GetOldColor();
			if(col!=oldColor){
				if(col==_BTOKEN){
					CString cs;
					cs.Format("t %d",SimTurn);
					place_datas += bsp->PlaceMap[Pid]->GetTokenValue() + '0';
					place_datas += cs;
				}
				oldColor = col;
			}
		}

		if(place_datas.length()==16){
			place_datas_que.push_back(place_datas);
			place_datas.erase();
		}
		bufDC.SetTextColor(vColMap[_TEXT_COL]);
		deque<string>::iterator qit;
		qit = place_datas_que.begin();
		int y;

#define PLACE_DATA_Y_HEIGHT 60
		
		for(y=cp.y-PLACE_DATA_Y_HEIGHT-20*place_datas_que.size();qit!=place_datas_que.end();y+=20,++qit){
			bufDC.TextOut(cp.x-80,y,qit->c_str());
		}
		
		bufDC.TextOut(cp.x-80,cp.y-PLACE_DATA_Y_HEIGHT,place_datas.c_str());
	}
	else{
		place_datas_que.clear();
		place_datas.erase();
		Pid = 0;
	}
*/



	// 動作状況をステータスバーに表示
	float fpt;
	fpt = (float)bsp_tree->GetRootBSP()->traFireable / bsp_tree->GetRootBSP()->traNum;

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	int nIndex = pStatusBar->CommandToIndex(ID_INDICATOR_RUNS);
	CString cs;
	for(int i=0;i<fpt*10*2 && i<10;i++){
		cs += '*';
	}
	pStatusBar->SetPaneText(nIndex, cs);

	
	CClientDC pDC(this);
	pDC.BitBlt(tlp.x, tlp.y, brp.x-tlp.x, brp.y-tlp.y, &bufDC, tlp.x, tlp.y, SRCCOPY);

/*
//#ifdef _DEBUG
	cs.Format("turn %d",SimTurn);
	pDC.TextOut(10,70,cs);
//#endif
*/

	/*
	static int f=0;
	f++;
	static time_t st=0;
	if(time(NULL)-st){
		st=time(NULL);
		CString s;
		s.Format("tt %d",f);
		pDC.TextOut(200,0,s);
		f=0;
	}
	*/
	
}

void CQROQSView::PutObject(CDC *pDC, int x,int y,int obj,int opt,int selected)
{

	if(viewDiam<0.0625){
		//小さすぎて見えない時は点を打つ
		COLORREF col;
		switch(obj){
		case _PLACE:		col = _PLACE_COL;	break;
		case _TRANSITION:	col = opt ? _TRAN_COL : _PASSTRA_COL;	break;
		case _UHZI:			col = _UHZI_COL;		break;
		case _KEMMAKIX:		col = _KEM_COL;	break;
		case _MODULE:		col = _MOD_COL;		break;
		}
		m1DC.SetPixel(x,y,vColMap[col+(selected?_SELECTED_COL:0)]);
		return;
	}

	CPen *pen_obj;
	CBrush *brush_obj;
	switch(obj){
	case _PLACE:		pen_obj = selected ? pen_place_select : pen_place;	break;
	case _TRANSITION:	pen_obj = selected ? 
							(opt ? pen_transition_func_select : pen_transition_select) : 
							(opt ? pen_transition_func : pen_transition);	break;
	case _UHZI:			if(selected){
							pen_obj = pen_uhzi_select;
							brush_obj = brush_uhzi_select;
						}
						else{
							pen_obj = pen_uhzi;
							brush_obj = brush_uhzi;
						}
						break;
	case _KEMMAKIX:		if(selected){
							pen_obj = pen_kemmakix_select;
							brush_obj = brush_kemmakix_select;
						}
						else{
							pen_obj = pen_kemmakix;
							brush_obj = brush_kemmakix;
						}
						break;
	case _MODULE:		if(selected){
							pen_obj = pen_module_select;
							brush_obj = brush_module_select;
						}
						else{
							pen_obj = pen_module;
							brush_obj = brush_module;
						}
						break;
	}

	pDC->SelectObject(pen_obj);

	if(obj==_PLACE){
		CRect pRect;
		pRect.SetRect(vpdPlace[0], vpdPlace[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Ellipse(pRect);
	}
	else if(obj==_TRANSITION){
		CRect pRect;
		pRect.SetRect(vpdTra[0], vpdTra[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle(pRect);
	}
	else if(obj==_UHZI){
		pDC->MoveTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->LineTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->EndPath();
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->FillPath();
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->EndPath();
		pDC->SelectObject(brush_obj);
		pDC->FillPath();
		pDC->MoveTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
	}
	else if(obj==_KEMMAKIX){
		pDC->MoveTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->LineTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->EndPath();
		pDC->SelectObject(brush_obj);
		pDC->FillPath();
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->EndPath();
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->FillPath();
		pDC->MoveTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
	}
	else if(obj==_MODULE){
		CRect pRect;
		pRect.SetRect(vpdModule[0], vpdModule[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(WHITE_BRUSH);
		pDC->Rectangle(pRect);
		pRect.left = x + vpdModule[2].x;
		pRect.top = y + vpdModule[2].y;
		pDC->SelectObject(brush_obj);
		pDC->Rectangle(pRect);
	}
}

void CQROQSView::FlyObject(CDC *pDC, int x,int y,int obj)
{	
	pDC->SelectObject(&pen_gray1d);
	if(obj==_PLACE){
		CRect pRect;
		pRect.SetRect(vpdPlace[0], vpdPlace[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Ellipse(pRect);
	}
	else if(obj==_TRANSITION){
		CRect pRect;
		pRect.SetRect( vpdTra[0], vpdTra[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(pRect);
	}
	else if(obj==_UHZI){
		pDC->MoveTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->EndPath();
		pDC->SelectObject(&brush_grayh);
		pDC->FillPath();
	}
	else if(obj==_KEMMAKIX){
		pDC->MoveTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->LineTo(x+vpdUhziKem[4].x,y+vpdUhziKem[4].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpdUhziKem[0].x,y+vpdUhziKem[0].y);
		pDC->LineTo(x+vpdUhziKem[3].x,y+vpdUhziKem[3].y);
		pDC->LineTo(x+vpdUhziKem[2].x,y+vpdUhziKem[2].y);
		pDC->LineTo(x+vpdUhziKem[1].x,y+vpdUhziKem[1].y);
		pDC->EndPath();
		pDC->SelectObject(&brush_grayh);
		pDC->FillPath();
	}
	else if(obj==_MODULE){
		CRect pRect;
		pRect.SetRect(vpdModule[0], vpdModule[1]);
		pRect.OffsetRect(x,y);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(pRect);
		pRect.left = x + vpdModule[2].x;
		pRect.top = y + vpdModule[2].y;
		pDC->SelectObject(&brush_grayh);
		pDC->Rectangle(pRect);
	}
}

#ifndef _LIGHT
void CQROQSView::PutObjectMini(CDC *pDC, int x,int y,int obj)
{
	if(obj==_PLACE){
		CRect pRect;
		pRect.SetRect(vpsPlace[0], vpsPlace[1]);
		pRect.OffsetRect(x,y);
		pDC->Ellipse(pRect);
	}
	else if(obj==_TRANSITION){
		CRect pRect;
		pRect.SetRect(vpsTra[0], vpsTra[1]);
		pRect.OffsetRect(x,y);
		pDC->Rectangle(pRect);
	}
	else if(obj==_UHZI){
		pDC->MoveTo(x+vpsUhziKem[4].x,y+vpsUhziKem[4].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpsUhziKem[0].x,y+vpsUhziKem[0].y);
		pDC->LineTo(x+vpsUhziKem[3].x,y+vpsUhziKem[3].y);
		pDC->LineTo(x+vpsUhziKem[4].x,y+vpsUhziKem[4].y);
		pDC->EndPath();
		pDC->FillPath();
		pDC->LineTo(x+vpsUhziKem[1].x,y+vpsUhziKem[1].y);
		pDC->LineTo(x+vpsUhziKem[2].x,y+vpsUhziKem[2].y);
		pDC->LineTo(x+vpsUhziKem[4].x,y+vpsUhziKem[4].y);
	}
	else if(obj==_KEMMAKIX){
		pDC->MoveTo(x+vpsUhziKem[1].x,y+vpsUhziKem[1].y);
		pDC->BeginPath();
		pDC->LineTo(x+vpsUhziKem[2].x,y+vpsUhziKem[2].y);
		pDC->LineTo(x+vpsUhziKem[3].x,y+vpsUhziKem[3].y);
		pDC->LineTo(x+vpsUhziKem[0].x,y+vpsUhziKem[0].y);
		pDC->LineTo(x+vpsUhziKem[1].x,y+vpsUhziKem[1].y);
		pDC->EndPath();
		pDC->FillPath();
		pDC->LineTo(x+vpsUhziKem[2].x,y+vpsUhziKem[2].y);
		pDC->LineTo(x+vpsUhziKem[4].x,y+vpsUhziKem[4].y);
		pDC->LineTo(x+vpsUhziKem[1].x,y+vpsUhziKem[1].y);
	}
	else if(obj==_MODULE){
		CRect pRect;
		pRect.SetRect( vpsModule[0], vpsModule[1]);
		pRect.OffsetRect(x,y);
		pDC->Rectangle(pRect);
	}
}
#endif

/****************************************
	Copyright  KAWAJIRI Kensuke
****************************************/
void CQROQSView::Yajirushi(CDC *pDC, CPoint start, CPoint end, double angle, int len)
{
	
	//	TRACE("p(%d,%d) (%d,%d)\n",start.x,start.y,end.x,end.y);
	CPoint p;
	double m, n=0;
	pDC->MoveTo(start);
	pDC->LineTo(end);
	if(start.x<end.x){
		if(end.x != start.x)
			n = (double)(start.y-end.y)/(end.x-start.x);
		
		m = atan(n);
		if(angle > m){
			m = angle - m;
			p.x = (long)(end.x-len*cos(m));
			p.y = (long)(end.y-len*sin(m));
			pDC->LineTo(p);
			m = M_PI - atan(n) - angle;
			p.x = (long)(end.x+len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->MoveTo(end);
			pDC->LineTo(p);
		}
		else{
			m = m - angle;
			p.x = (long)(end.x-len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->LineTo(p);
			m = M_PI - atan(n) - angle;
			p.x = (long)(end.x+len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->MoveTo(end);
			pDC->LineTo(p);
		}
	}
	else if(start.x > end.x){
		if(end.x != start.x)
			n = (double)(start.y-end.y)/(-end.x+start.x);
		m = atan(n);
		if(angle > m){
			m = angle - m;
			p.x = (long)(end.x+len*cos(m));
			p.y = (long)(end.y-len*sin(m));
			pDC->LineTo(p);
			m = M_PI - atan(n) - angle;
			p.x = (long)(end.x-len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->MoveTo(end);
			pDC->LineTo(p);
		}
		else{
			m = m - angle;
			p.x = (long)(end.x+len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->LineTo(p);
			m = M_PI - atan(n) - angle;
			p.x = (long)(end.x-len*cos(m));
			p.y = (long)(end.y+len*sin(m));
			pDC->MoveTo(end);
			pDC->LineTo(p);
		}
	}
	else{
		if(end.y > start.y)
			p.y = (long)(end.y-cos(angle)*len);
		else 
			p.y = (long)(end.y+cos(angle)*len);
		p.x = (long)(end.x-sin(angle)*len);
		pDC->LineTo(p);
		pDC->MoveTo(end);
		p.x = (long)(end.x+sin(angle)*len);
		pDC->LineTo(p);
	}
}



void CQROQSView::PrintArcValue(CDC *pDC, CPoint inp, CPoint outp, const char *arc_val)
{
	if(arc_val[0]=='\0') return;
	
	inp=Betwixtpoint(inp,outp,0,sqrt(pow(inp.x-outp.x,2)+pow(inp.y-outp.y,2))/2);
	inp=Diampoint(viewDiam,inp);
	pDC->TextOut(inp.x-(10*viewDiam),inp.y-(10*viewDiam),arc_val);
}


//ツールチップ用
void CQROQSView::OnTipDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMTTDISPINFO lpttd;
//TRACE("%d\n",pNMHDR);
    if( pNMHDR->code == TTN_GETDISPINFO ) //意味のないif
   {
        if( !m_strTip.IsEmpty() )
        {
            //表示すべき文字列はm_strTipに入っています。これが空のときは
            //表示すべきではありません。
            lpttd = (LPNMTTDISPINFO)pNMHDR;
            lpttd->lpszText = (LPSTR)(LPCTSTR)m_strTip;
        }
    }
    *pResult = 0;
}


void CQROQSView::viewPenBrushChange()
{

	if(pen_arc!=NULL){
		delete pen_arc;
		delete pen_place;
		delete pen_transition;
		delete pen_transition_func;
		delete pen_uhzi;
		delete pen_kemmakix;
		delete pen_token;
		delete pen_module;

		delete pen_arc_select;
		delete pen_place_select;
		delete pen_transition_select;
		delete pen_transition_func_select;
		delete pen_uhzi_select;
		delete pen_kemmakix_select;
		delete pen_module_select;

		delete brush_uhzi;
		delete brush_kemmakix;
		delete brush_module;
		delete pen_hatch;

		delete brush_uhzi_select;
		delete brush_kemmakix_select;
		delete brush_module_select;

	}
	
	int i;
	if(viewDiam>=0.75) i=3;
	else if(viewDiam>=0.38) i=2;
	else i=1;
	pen_arc = new CPen(PS_SOLID, i, vColMap[_ARC_COL]);
	pen_place = new CPen(PS_SOLID, i, vColMap[_PLACE_COL]);
	pen_transition = new CPen(PS_SOLID, i, vColMap[_PASSTRA_COL]);
	pen_transition_func = new CPen(PS_SOLID, i, vColMap[_TRAN_COL]);
	pen_uhzi = new CPen(PS_SOLID, i, vColMap[_UHZI_COL]);
	pen_kemmakix = new CPen(PS_SOLID, i, vColMap[_KEM_COL]);
	pen_module = new CPen(PS_SOLID, i, vColMap[_MOD_COL]);

	pen_arc_select = new CPen(PS_SOLID, i, vColMap[_ARC_COL+_SELECTED_COL]);
	pen_place_select = new CPen(PS_SOLID, i, vColMap[_PLACE_COL+_SELECTED_COL]);
	pen_transition_select = new CPen(PS_SOLID, i, vColMap[_PASSTRA_COL+_SELECTED_COL]);
	pen_transition_func_select = new CPen(PS_SOLID, i, vColMap[_TRAN_COL+_SELECTED_COL]);
	pen_uhzi_select = new CPen(PS_SOLID, i, vColMap[_UHZI_COL+_SELECTED_COL]);
	pen_kemmakix_select = new CPen(PS_SOLID, i, vColMap[_KEM_COL+_SELECTED_COL]);
	pen_module_select = new CPen(PS_SOLID, i, vColMap[_MOD_COL+_SELECTED_COL]);

	pen_token = new CPen(PS_SOLID, i, vColBToken);

	brush_uhzi = new CBrush(vColMap[_UHZI_COL]);
	brush_kemmakix = new CBrush(vColMap[_KEM_COL]);
	brush_module = new CBrush(vColMap[_MOD_COL]);
	
	brush_uhzi_select = new CBrush(vColMap[_UHZI_COL+_SELECTED_COL]);
	brush_kemmakix_select = new CBrush(vColMap[_KEM_COL+_SELECTED_COL]);
	brush_module_select = new CBrush(vColMap[_MOD_COL+_SELECTED_COL]);

	pen_hatch=new CPen(PS_SOLID, 1,vColMap[_GLID_COL]);

	/*
	//フォントの作成
	LOGFONT lf;
	vcFont.GetLogFont(&lf);
	lf.lfHeight *= viewDiam;	// 17
	lf.lfWidth  *= viewDiam;	// 7
	vFont.DeleteObject();
	vFont.CreateFontIndirect(&lf);
*/
	int fh, fw;
	fh = 17 * viewDiam;
	fw =  7 * viewDiam;
	vFont.DeleteObject();
	vFont.CreateFont(
		fh , fw , 0 , 0 , FW_BOLD , TRUE , TRUE , FALSE ,
		SHIFTJIS_CHARSET , OUT_DEFAULT_PRECIS ,
		CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY , 
		VARIABLE_PITCH | FF_ROMAN , NULL
		);

#ifndef _LIGHT
	if(backlpBMP!=NULL && !rect_back_flag){
		backDC.FillSolidRect(0,0,back_size.cx,back_size.cy,vColMap[_BACK_COL]);
	}
#endif

}



void CQROQSView::OnComBackconf() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

	FitView();

#ifndef _LIGHT

/*
	bsp_tree->UhziRegExpand();
	bsp_tree->KemBinExpand();
	bsp_tree->KemNoConsumDelete();
	bsp_tree->KemNojointDelete();
	Invalidate(0);
	return;
*/


	if(backlpBMP!=NULL){
		rect_back_flag = 1;
		rect_back(&backDC,backlpBMP,back_size.cx,back_size.cy,vColMap[_GLID_COL]);
		viewPenBrushChange();
	}

	Invalidate(0);
#endif

}


void CQROQSView::OnComZoomup()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnMouseWheel(0, -WHEEL_DELTA, 0);
}

void CQROQSView::OnComZoomdown()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OnMouseWheel(0, WHEEL_DELTA, 0);
	
}


/// EOF ///
