// QROQSViewPrint.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSView.h"

#include "other.h"

Beecir::Beecir()
{
	wid=/*(rand()%2?-1:1)**/(rand()%10+2);
	rad=rand()%5+(Qmode==1?rand()%100:20);
	dd=1; pd=0; sd=0;
	col=0;//rand()%6;
	cd=20;
	switch(col){
	case 0: r=255; g=rand()%256; b=0; break;
	case 1: r=rand()%256; g=255; b=0; break;
	case 2: r=0; g=255; b=rand()%256; break;
	case 3: r=0; g=rand()%256; b=255; break;
	case 4: r=rand()%256; g=0; b=255; break;
	case 5: r=255; g=0; b=rand()%256; break;
	}
	arg=((rand()*rand())%360)*3.14/180;
}

CRect Beecir::view(CView *cvp,CPoint gp,int type,int opt)
{

	CQROQSView *vp=(CQROQSView *)cvp;

	CPoint mmp;
	double nr;
	arg += wid*3.14/180;
	nr = rad + 5 + 10 * sin(arg/5);
	mmp.x = (int)(gp.x + nr * cos(arg) /*+10*sin(arg/2.3)*/);
	mmp.y  = (int)(gp.y + nr * sin(arg)/*+10*cos(arg/1.4)*/);
	mmp=vp->Diampoint(vp->viewDiam,mmp);
	
	switch(col){
	case 0: g+=cd; if(g>255){ g=255; col=1; } break;
	case 1: r-=cd; if(r<0){ r=0; col=2; } break;
	case 2: b+=cd; if(b>255){ b=255; col=3; } break;
	case 3: g-=cd; if(g<0){ g=0; col=4; } break;
	case 4: r+=cd; if(r>255){ r=255; col=5; } break;
	case 5: b-=cd; if(b<0){ b=0; col=0; } break;
	}
	
	if(mmp.x>gp.x-20 && mmp.x<gp.x+20 && mmp.y>gp.y-20 && mmp.y<gp.y+20){
		if(pd==0){
			if(sd==0){ dd=0; sd=1; }
			else sd=0;
		}
		pd=1;
	}
	else{ dd=1; pd=0; }

	CPoint objp;
	objp=gp;
	objp.x -= 20*vp->viewDiam;
	objp.y -= 20*vp->viewDiam;
	objp.x+=rand()%5-2;
	objp.y+=rand()%5-2;
	if(dd==1 && vp->viewDiam==1){
		if(type!=_PLACE)
			vp->PutObject(&(vp->bufDC), objp.x, objp.y, type, opt);
	}

	vp->bufDC.SelectStockObject(NULL_BRUSH);
	CPen pen4,*oldpen;
	pen4.CreatePen(PS_SOLID, 10, RGB(r,g,b));
	oldpen = vp->bufDC.SelectObject(&pen4);
	vp->bufDC.Ellipse(mmp.x-3,mmp.y-3,mmp.x+3,mmp.y+3);
	vp->bufDC.SelectObject(oldpen);
	pen4.DeleteObject();
	
	if(dd==0 && vp->viewDiam==1){
		if(type!=_PLACE)
			vp->PutObject(&(vp->bufDC), objp.x, objp.y, type, opt);
	}

	CRect pRect;
	pRect.SetRect(mmp,mmp);
	pRect.InflateRect(V_CHIP_SIZE,V_CHIP_SIZE);
	
	return pRect;
}


#ifndef _LIGHT
BeeToken::BeeToken(CPoint in,CPoint out,int c)
{
	inp=in;
	outp=out;
	
	dd=2;	// 開始
	pd=10;	// 分割数
	
	rad=sqrt((inp.x-outp.x)*(inp.x-outp.x)+(inp.y-outp.y)*(inp.y-outp.y));
	col=c;
}
CRect BeeToken::view(CView *cvp)
{
	CQROQSView *vp=(CQROQSView *)cvp;
	CPoint mmp;
	
	mmp=Betwixtpoint(inp,outp,0,rad*dd/pd);
	dd++;
	mmp=vp->Diampoint(vp->viewDiam,mmp);

	CClientDC pDC(vp);
	pDC.SelectObject(vp->pen_token);
	CRect pRect;
	if(col==_BTOKEN){
		pDC.SelectStockObject(BLACK_BRUSH);
	}
	else if(col==_WTOKEN){
		pDC.SelectStockObject(WHITE_BRUSH);
	}
	int s=(5*vp->viewDiam);
	pRect.SetRect(mmp.x-s, mmp.y-s, mmp.x+s, mmp.y+s);
	
	pDC.Ellipse(pRect);
	
	return pRect;
}
#endif


void CQROQSView::OnSkinMenu(UINT uID) 
{

//	TRACE("sel men %d\n",uID-IDM_SKINS);
	uID -= IDM_SKINS;

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	CFileFind FileFind;
    // skinフォルダのすべてのファイルを検索
    CString strSearchFile = pApp->exe_dir + _T("skin\\*");
//	TRACE("%s\n",strSearchFile);
    
    if(!FileFind.FindFile(strSearchFile)) 
        return; 
   
	int i=0;
    BOOL bContinue = TRUE;
    while(bContinue){
        bContinue = FileFind.FindNextFile();    
        // ドット("." , "..")の場合 無視
        if(FileFind.IsDots()) 
            continue;
        // ディレクトリの場合、
        if(FileFind.IsDirectory()){
			if(i==uID) break;
			i++;
		}
    }
	if(i!=uID) return;

//	TRACE("%s\n",FileFind.GetFilePath() );


/////////////////////////////////////////////////////
//	読み込み

	char skin_str[200+1];
	sprintf(skin_str,"%s\\color.txt",FileFind.GetFilePath());
	FILE *fp;
	fp=fopen(skin_str,"r");
	if(fp==NULL){
		AfxMessageBox("選択されたskinを読み込めませんでした．");
		return;
	}
	else{
		struct func{
			static COLORREF get_rgb(const char *str)
			{
				int r,g,b;
				sscanf(str,"%2x%2x%2x",&r,&g,&b);
				return RGB(r,g,b);
			}
			static COLORREF get_rgb_str(const char *str)
			{
				int i=0;
				while(i<100 && str[i]!='#') i++;
				i++;
				return get_rgb(str+i);
			}
		};

		while(fgets(skin_str,100,fp)){
			if(!strncmp(skin_str,"place",5)){
				vColMap[_PLACE_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"arc",3)){
				vColMap[_ARC_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"transition",10)){
				vColMap[_PASSTRA_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"functransition",14)){
				vColMap[_TRAN_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"uhzi",4)){
				vColMap[_UHZI_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"kemmakix",8)){
				vColMap[_KEM_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"module",6)){
				vColMap[_MOD_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"back",4)){
				vColMap[_BACK_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"glid",4)){
				vColMap[_GLID_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"text",4)){
				vColMap[_TEXT_COL] = func::get_rgb_str(skin_str);
			}
			// 選択時の色も追加しろ
			else if(!strncmp(skin_str,"select_place",5+7)){
				vColMap[_PLACE_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_arc",3+7)){
				vColMap[_ARC_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_transition",10+7)){
				vColMap[_PASSTRA_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_functransition",14+7)){
				vColMap[_TRAN_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_uhzi",4+7)){
				vColMap[_UHZI_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_kemmakix",8+7)){
				vColMap[_KEM_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
			else if(!strncmp(skin_str,"select_module",6+7)){
				vColMap[_MOD_COL+_SELECTED_COL] = func::get_rgb_str(skin_str);
			}
#ifndef _LIGHT
			else if(!strncmp(skin_str,"skin",4)){
				sscanf(skin_str,"skin:%d",&SkinUse);
			}
#endif
		}
		fclose(fp);
	}

	SkinDir = (LPCTSTR)FileFind.GetFilePath() + FileFind.GetFilePath().ReverseFind('\\')+1;

//	TRACE("%s\n",SkinDir);

	rect_back_flag=0;
//	viewPenBrushChange();

#ifndef _LIGHT
	if(SkinUse==1){
		ReadObjectSkin();
	}

	ReadBackSkin();
#endif

	OnMouseWheel(0,0,0);

	Invalidate(0);

}

#include "TokenFuncDlg.h"
void CQROQSView::OnComSkinSave() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	if(::GetFileAttributes("skin") == 0xffffffff){
		Trace("skinﾌｫﾙﾀﾞが存在しません．");
		return;
	}

	CTokenfuncDlg mnameDlg;
	mnameDlg.m_input = SkinDir;
	mnameDlg.m_str = "skinﾃｰﾏを入力してください．";
	if(mnameDlg.DoModal()!=IDOK){
		return;
	}
	if(mnameDlg.m_input.IsEmpty()){
		return;
	}

	CString skin_dir;
	skin_dir = "skin\\" + mnameDlg.m_input;
	if(::GetFileAttributes(skin_dir) != 0xffffffff){
		if(AfxMessageBox(mnameDlg.m_input+" : skinﾃｰﾏを上書きします．\nよろしいですか？",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)
			==IDCANCEL){
			return;
		}
	}


//	TRACE("%s\n",SkinDir);

	CreateDirectory(skin_dir,NULL);

	FILE *fp;
	fp=fopen((LPCTSTR)(skin_dir+"\\color.txt"),"w");
	if(fp==NULL){
		Trace("skinﾃｰﾏﾌｧｲﾙを作成できませんでした．\n");
		return;
	}

	struct func{
		static int rgb( COLORREF ref )
		{
			int ret = 0;
			ret += ref & 0x0000ff;
			ret <<= 8;
			ref >>= 8;
			ret += ref & 0x0000ff;
			ret <<= 8;
			ref >>= 8;
			ret += ref & 0x0000ff;
			return ret;
		}
	};

	fprintf(fp,"place:#%06x\n",func::rgb(vColMap[_PLACE_COL]));
	fprintf(fp,"transition:#%06x\n",func::rgb(vColMap[_PASSTRA_COL]));
	fprintf(fp,"functransition:#%06x\n",func::rgb(vColMap[_TRAN_COL]));
	fprintf(fp,"uhzi:#%06x\n",func::rgb(vColMap[_UHZI_COL]));
	fprintf(fp,"kemmakix:#%06x\n",func::rgb(vColMap[_KEM_COL]));
	fprintf(fp,"module:#%06x\n",func::rgb(vColMap[_MOD_COL]));
	fprintf(fp,"arc:#%06x\n",func::rgb(vColMap[_ARC_COL]));

	fprintf(fp,"select_place:#%06x\n",func::rgb(vColMap[_PLACE_COL+_SELECTED_COL]));
	fprintf(fp,"select_transition:#%06x\n",func::rgb(vColMap[_PASSTRA_COL+_SELECTED_COL]));
	fprintf(fp,"select_functransition:#%06x\n",func::rgb(vColMap[_TRAN_COL+_SELECTED_COL]));
	fprintf(fp,"select_uhzi:#%06x\n",func::rgb(vColMap[_UHZI_COL+_SELECTED_COL]));
	fprintf(fp,"select_kemmakix:#%06x\n",func::rgb(vColMap[_KEM_COL+_SELECTED_COL]));
	fprintf(fp,"select_module:#%06x\n",func::rgb(vColMap[_MOD_COL+_SELECTED_COL]));
	fprintf(fp,"select_arc:#%06x\n",func::rgb(vColMap[_ARC_COL+_SELECTED_COL]));

	fprintf(fp,"glid:#%06x\n",func::rgb(vColMap[_GLID_COL]));
	fprintf(fp,"back:#%06x\n",func::rgb(vColMap[_BACK_COL]));
	fprintf(fp,"text:#%06x\n",func::rgb(vColMap[_TEXT_COL]));

#ifndef _LIGHT
	fprintf(fp,"skin:%d\n",SkinUse);
#endif
	fclose(fp);

	SkinDir = mnameDlg.m_input;

}


void CQROQSView::OnComResearch() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
#ifdef _LIGHT
	AfxMessageBox("Lightﾊﾞｰｼﾞｮﾝでは機能制限されています．",MB_OK);
#else
	//一旦コンパイル
	OnComCompile();

	SetTimer(_TID_RES_CHECK, 100, NULL);
#endif
}

#ifndef _LIGHT
void CQROQSView::ResearchPost()
{
	if(compOk == false) return;
	OnComHalt();


	BSPetriTree s_bsp_tree;
	s_bsp_tree = *bsp_tree;

//		TRACE("s %d\n",MAX_PATH);
	// root以下のモジュール名を取得
	set<string> mset;
	s_bsp_tree.UnderMnameGet(&s_bsp_tree, s_bsp_tree.GetRootBSP(), mset);
	mset.insert(s_bsp_tree.GetRootName());

	Trace("解析中...\n");

	// 各BSPでﾌｨｰﾄﾞﾊﾞｯｸ接続の解析と不要トラプレの削除
	s_bsp_tree.C_Analysis(mset);
//	TRACE("expand\n");

	struct func{
		static void res_io(
			BSPetriTree *bspt,
			BSPetri *cur_bsp,
			map<string,int> &res_map
			)
		{
			map<int,string>::iterator mit;
			mit = cur_bsp->ModMap.begin();
			for(;mit!=cur_bsp->ModMap.end();++mit){
				res_io(bspt,bspt->GetBSP(mit->second.c_str()),res_map);
			}

			char buf[20];
			map<int,bspTransition*>::iterator tit;
			tit = cur_bsp->TranMap.begin();
			for(;tit!=cur_bsp->TranMap.end();++tit){
				sprintf(buf,"%d_%d_%d",tit->second->in_place_num,tit->second->loop_num,tit->second->out_place_num);
				if(res_map.find(buf)==res_map.end()){
					res_map[buf] = 1;
				}
				else{
					res_map[buf] ++;
				}
			}

		}
	};

	map<string,int> res_map;
	func::res_io(&s_bsp_tree,s_bsp_tree.GetRootBSP(),res_map);


	Trace("入ル出	頻度\n");
	map<string,int>::iterator rmit;
	rmit=res_map.begin();
	for(;rmit!=res_map.end();++rmit){
		Trace("%s	%d\n",rmit->first.c_str(),rmit->second);
	}

	// 上位からの参照されているBSP
	map<int,string> remap;
	{
		map<string,int> rfmap;
		rfmap[bsp_tree->GetRootName()] = 1;
		bsp_tree->UnderMnameGetRefers(bsp_tree, bsp_tree->GetRootBSP(), rfmap, 2);
		map<string,int>::iterator mit;
		mit = rfmap.begin();
		for(;mit!=rfmap.end();++mit){
			remap[mit->second] = mit->first;
		}
		rfmap.clear();
	}
	// 保持するﾓｼﾞｭｰﾙを列挙
	map<int,string>::iterator bspit;
	bspit = remap.begin();
	for(;bspit!=remap.end();++bspit){
		Trace("\nBSP : %s\n",bspit->second.c_str());
		BSPetri *q_bsp = bsp_tree->GetBSP(bspit->second.c_str());
		set<string> mnset;
		map<int,string>::iterator mit;
		mit = q_bsp->ModMap.begin();
		for(;mit!=q_bsp->ModMap.end();++mit){
			if(mnset.find(mit->second)==mnset.end()){
				Trace("	%s\n",mit->second.c_str());
				mnset.insert(mit->second);
			}
		}
	}
}
#endif

#include "Memodlg.h"
void CQROQSView::OnComMemo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CMemoDlg mDlg;
	mDlg.m_Memo = bsp->Memo.c_str();
	if(mDlg.DoModal()!=IDOK){
		return;
	}

	bsp->SetMemo( (LPCTSTR)mDlg.m_Memo );

}


void CQROQSView::OnComJointdata() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
	CFileDialog fd(
		false,	//保存
		"txt",	//デフォルトの拡張子
		(bsp->module_name+"_joint.txt").c_str(),	//デフォルトのファイル名
		OFN_OVERWRITEPROMPT|
		OFN_HIDEREADONLY|
		OFN_LONGNAMES,	//フラグ
		"BSP接続ﾃﾞｰﾀ (*.txt)|*.txt|"
		"全てのﾌｧｲﾙ (*.*)|*.*||",	//フィルタ
		NULL	//親ウィンドウ
		);
	
	CQROQSDoc *pDoc = GetDocument();

	//最後に\\があってもなくてもいい
	fd.m_ofn.lpstrInitialDir = pDoc->filepath;	//初期ディレクトリ
	
	if(fd.DoModal() != IDOK) return;


	FILE *fp;
	fp = fopen((LPCTSTR)fd.GetPathName(),"w");
	if(fp==NULL){
		Trace("ﾌｧｲﾙを作成できませんでした．\n");
		return;
	}

	AfxGetApp()->DoWaitCursor(1);//マウスカーソル砂時計

	map<int,bspObject>::const_iterator oit,oit2,oit_end;
	multimap<int,int>::const_iterator aioit;
	oit = bsp->ObjMap.begin();
	oit_end = bsp->ObjMap.end();
	fprintf(fp,"       ");
	for(;oit!=oit_end;++oit){
		fprintf(fp,"%6d,",oit->first);
	}
	fprintf(fp,"\n");
	oit = bsp->ObjMap.begin();
	for(;oit!=oit_end;++oit){
		fprintf(fp,"%6d,", oit->first);
		oit2 = bsp->ObjMap.begin();
		for(;oit2!=oit_end;++oit2){
			aioit = bsp->ArcInMap.lower_bound(oit->first);
			if(aioit!=bsp->ArcInMap.upper_bound(oit->first)
				&& bsp->ArcMap[aioit->second].Out==oit2->first){
				fprintf(fp,"%6d,",1);
			}
			else fprintf(fp,"%6d,",0);
		}
		fprintf(fp,"\n");
	}


	fclose(fp);

	Trace(fd.GetPathName() + " : 保存しました．\r\n");
	AfxGetApp()->DoWaitCursor(-1);//マウスカーソル砂時計解除
}


void CQROQSView::OnComHelp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
//	TRACE("help\n");
#ifdef _LIGHT
		TraceClear();
#else
	switch(help_id){
	case 0:
		TraceClear();
		bsp->ErrPos = 0;
		break;
	case 1://helpです
	//	Trace("何かあったら、ヘルプボタンを押してください。お助け情報があればここに表示されます。");
		break;
	case 2://トランジションコンパイルエラー
		Trace("ﾄﾗﾝｼﾞｼｮﾝに記述されている関数で使用される変数に対して，ｱｰｸを接続してください．");
		break;
	case 3://発火可能ストップオプション
		Trace("ﾄﾗﾝｼﾞｼｮﾝのｵﾌﾟｼｮﾝ「発火可能時にｼﾐｭﾚｰｼｮﾝを停止する」が設定されているﾄﾗﾝｼﾞｼｮﾝが発火可能となったため，ｼﾐｭﾚｰｼｮﾝを停止しました．");
		break;
	case 4://モジュール名変更
		Trace("数字から始まるﾓｼﾞｭｰﾙ名は使用できません．ROOTﾓｼﾞｭｰﾙ名を変更しようとしていた場合，下位ﾓｼﾞｭｰﾙにすでにそのﾓｼﾞｭｰﾙ名が使用されています．それ以外の場合は，上位ﾓｼﾞｭｰﾙでそのﾓｼﾞｭｰﾙ名が使用されています．再帰構造を構築することはできません．");
	//	Trace(bsp_tree->err_msg.c_str());
		break;
	}
	help_id = 0;
#endif

	if(bsp->ErrPos != 0){
		mst=0;
		SetTimer(_TID_FMOVE, 1, NULL); //作成

		KillTimer(_TID_OBJBEE);
		SetTimer(_TID_OBJBEE, 10, NULL); //作成
	}

	Invalidate(0);

}


/// EOF ///


