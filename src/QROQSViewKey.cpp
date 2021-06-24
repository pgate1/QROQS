// QROQSViewMouse.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#include "other.h"

/////////////////////////////////////////////////////////////////////////////
// CQROQSView クラスのメッセージ ハンドラ

void CQROQSView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

#ifndef _LIGHT
	if(pressKey=='s'){
		//デモSTOP
		KillTimer(_TID_DEMO);

		if(nChar == VK_UP){
			OnMouseWheel(0, WHEEL_DELTA, 0);
		}
		if(nChar == VK_DOWN){
			OnMouseWheel(0, -WHEEL_DELTA, 0);
		}
	}
#else
	if(0){}
#endif

	/*
	else if(pressKey=='p'){
		OnObjPlace();
	}
	else if(pressKey=='t'){
		OnObjTransition();
	}
	*/
	else{
		
		int ad=vChipSize;
		if(nFlags & 0x4000) ad=vChipSize<<1;//押しつづけて移動速度アップ
		if(nChar == VK_LEFT){ FaceMove(-ad,0); Invalidate(0); }
		if(nChar == VK_UP){ FaceMove(0,-ad); Invalidate(0); }
		if(nChar == VK_RIGHT){ FaceMove(ad,0); Invalidate(0); }
		if(nChar == VK_DOWN){ FaceMove(0,ad); Invalidate(0); }
		
		if(nChar == VK_CONTROL) pressKey = VK_CONTROL;
		if(nChar == VK_SPACE){
			pressKey = VK_SPACE;
			Invalidate(0);
		}
		if(nChar == VK_SHIFT) pressKey = VK_SHIFT;
		if(nChar == VK_ESCAPE){
			switch(now_command){
			case _PLACE:		OnObjPlace();		break;
			case _TRANSITION:	OnObjTransition();	break;
			case _UHZI:			OnObjUhzi();		break;
			case _KEMMAKIX:		OnObjKemmakix();	break;
			case _BTOKEN:		OnObjBtoken();		break;
			case _ARC:			OnObjArc();			break;
			case _CHAIN:		OnComChain();		break;
			case _MODULE:		OnObjModule();		break;
			default: viewReset();
			}
			Invalidate(0);
		}
		
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQROQSView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	pressKey = nChar;
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CQROQSView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	if(nChar>=0x25 && nChar<=0x28);
	else pressKey = NULL;//カーソルキー以外なら

	Invalidate(0);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



void CQROQSView::OnComDemo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください

#ifdef _LIGHT
	AfxMessageBox("Lightﾊﾞｰｼﾞｮﾝでは機能制限されています．",MB_OK);
#else

	static FILE *comfp=NULL;

	if(comfp==NULL){
		if(AfxMessageBox("ﾃﾞﾓを実行します．\nよろしいですか？\n\n中断するにはSキーを押してください．",MB_OKCANCEL)==IDCANCEL){
			return;
		}
	}

	if(comfp==NULL){
		if(bsp->ObjMap.size()!=0){
			Trace("ｵﾌﾞｼﾞｪｸﾄが配置されているためﾃﾞﾓを実行できません．");
			return;
		}
		::SetCurrentDirectory(((CQROQSApp*)AfxGetApp())->exe_dir);
		comfp=fopen("command_multi.txt","r");
		if(comfp==NULL){
			Trace("ｺﾏﾝﾄﾞﾌｧｲﾙを開けませんでした．");
			return;
		}
		//windowのセット
		CMainFrame *pWnd=(CMainFrame *)AfxGetMainWnd();
		pWnd->ShowControlBar(GetModTreeDlg(),false, TRUE);

		GetParent()->SetWindowPos(NULL,0,0,1024,768,SWP_SHOWWINDOW);
		SetTimer(_TID_DEMO, 500, NULL); //作成
		return;
	}

	char *comstr=new char[256];
	char *ret;
	ret=fgets(comstr,255,comfp);
	while(ret!=NULL && (comstr[0]=='/' || comstr[0]=='\n')){
		ret=fgets(comstr,255,comfp);
	}
	if(ret==NULL){
		KillTimer(_TID_DEMO);
		delete[] comstr;
		fclose(comfp);
		comfp=NULL;
		return;
	}


	StatusOut("ﾃﾞﾓﾝｽﾄﾚｰｼｮﾝ中...(sｷｰ押しっぱなで中断)");


	CToolBar* pToolBar = ((CMainFrame*)::AfxGetMainWnd())->GetObjToolBar();
	CRect mrect;
	pToolBar->GetClientRect(mrect);
	pToolBar->ClientToScreen(mrect);

	CRect vrect = viewRect;
	ClientToScreen(vrect);


	#define speed 300

	struct innerfunc{
		static void key_put(int nCode)
		{
			keybd_event(nCode, 0, 0, 0);
			keybd_event(nCode, 0, KEYEVENTF_KEYUP, 0);
		}
		static void mouse_click()
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0 ,0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0 ,0);
		}
		static void mmouse_click(CRect mrect,int vm)
		{
			MoveMouse(mrect.left+10+vm*24,mrect.top+10,10*speed);
//			SetCursorPos(mrect.left+10+vm*24,mrect.top+10);
			mouse_click();
			return;
		}
		static void mouse_rclick()
		{
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0 ,0);
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0 ,0);
		}
		static void vmouse_move(CRect vrect,int vx, int vy, int ax=0, int ay=0)
		{
			MoveMouse(vrect.left+V_CHIP_SIZE*vx+(V_CHIP_SIZE>>1)+ax,vrect.top+V_CHIP_SIZE*vy+(V_CHIP_SIZE>>1)+ay,10*speed);
//			SetCursorPos(vrect.left+40*vx+20+ax,vrect.top+40*vy+20+ay);
		}
		static void vmouse_click(CRect vrect,int vx, int vy)
		{
			vmouse_move(vrect,vx,vy);
			mouse_click();
			return;
		}
		static void vmouse_menu(CRect vrect,int mm)
		{
			POINT mp;
			GetCursorPos(&mp);
			mp.x += 20;
			mp.y += 18+10+20*mm;
			MoveMouse(mp.x,mp.y,3*speed);
//			SetCursorPos(mp.x,mp.y);
			innerfunc::mouse_click();
		}
	};

	int i;

	static int offset_x=0,offset_y=0;
	int x,y,ax,ay;


	if(!strncmp(comstr,"select",6)){
		sscanf(comstr,"select(%d)",&i);
		innerfunc::mmouse_click(mrect,i);
	}
	else if(!strncmp(comstr,"move_click",10)){
		x=y=ax=ay=-1;
		sscanf(comstr,"move_click(%d,%d,%d,%d)",&x,&y,&ax,&ay);
		if(x!=-1 && y!=-1){
			x += offset_x;
			y += offset_y;
			if(ax!=-1 && ay!=-1){
				innerfunc::vmouse_move(vrect,x,y,ax,ay);
				innerfunc::mouse_click();
			}
			else{
				innerfunc::vmouse_click(vrect,x,y);
			}
		}
	}
	else if(!strncmp(comstr,"move_dblclk",11)){
		x=y=-1;
		sscanf(comstr,"move_dblclk(%d,%d)",&x,&y);
		if(x!=-1 && y!=-1){
			x += offset_x;
			y += offset_y;
			innerfunc::vmouse_click(vrect,x,y);
			innerfunc::mouse_click();
		}
	}
	else if(!strncmp(comstr,"key_input",9)){
		x=0;
		y=1;
		sscanf(comstr,"key_input(%x,%d)",&x,&y);
		for(i=0;i<y;++i){
			innerfunc::key_put(x);
		}
	}
	else if(!strncmp(comstr,"put_string",10)){
		string str;
		int esc_flag=0;
		i=12;
		for(;;){
			while(i<strlen(comstr) && comstr[i]!='\"'){
				if(comstr[i]=='\\') esc_flag=1;
				else if(esc_flag==1){
					switch(comstr[i]){
					case 'r': str+='\r'; break;
					case 'n': str+='\n'; break;
					}
					esc_flag=0;
				}
				else str+=comstr[i];
				++i;
			}
			if(i==strlen(comstr)){
				fgets(comstr,255,comfp);
				i=0;
			}
			else break;
		}

		SetClipboardString(str.c_str());
#define VK_V 0x56
		keybd_event(VK_CONTROL, 0, 0, 0);
		innerfunc::key_put(VK_V);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	}
	else if(!strncmp(comstr,"move_rclick",11)){
		x=y=-1;
		sscanf(comstr,"move_rclick(%d,%d)",&x,&y);
		if(x!=-1 && y!=-1){
			x += offset_x;
			y += offset_y;
			innerfunc::vmouse_move(vrect,x,y);
			innerfunc::mouse_rclick();
		}
	}
	else if(!strncmp(comstr,"menu_click",10)){
		sscanf(comstr,"menu_click(%d)",&i);
		innerfunc::vmouse_menu(vrect,i);
	}
	else if(!strncmp(comstr,"rclick",6)){
		innerfunc::mouse_rclick();
	}
	else if(!strncmp(comstr,"track",5)){
		sscanf(comstr,"track(%d,%d,%d,%d)",&x,&y,&ax,&ay);
		x += offset_x;
		y += offset_y;
		innerfunc::vmouse_move(vrect,x,y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0 ,0);
		ax += offset_x;
		ay += offset_y;
		innerfunc::vmouse_move(vrect,ax,ay);
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0 ,0);
	}
	else if(!strncmp(comstr,"mtrack",6)){
		sscanf(comstr,"mtrack(%d,%d,%d,%d)",&x,&y,&ax,&ay);
		x += offset_x;
		y += offset_y;
		innerfunc::vmouse_move(vrect,x,y);
		mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0 ,0);
		ax += offset_x;
		ay += offset_y;
		innerfunc::vmouse_move(vrect,ax,ay);
		mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0 ,0);
	}
	else if(!strncmp(comstr,"trace",5)){
		i=strlen(comstr)-1;
		while(i>0 && comstr[i]!=')') i--;
		comstr[i]='\n';
		comstr[i+1]='\0';
		Trace(comstr+6);
	}
	else if(!strncmp(comstr,"zoom_up",7)){
		OnMouseWheel(0, -WHEEL_DELTA, 0);
	}
	else if(!strncmp(comstr,"zoom_down",9)){
		OnMouseWheel(0, WHEEL_DELTA, 0);
	}
	else if(!strncmp(comstr,"offset",6)){
		sscanf(comstr,"offset(%d,%d)",&offset_x,&offset_y);
	}
	else if(!strncmp(comstr,"sleep",5)){
		sscanf(comstr,"sleep(%d)",&i);
		Sleep(i);
	}
	else if(!strncmp(comstr,"nop",3)){
	}
	else if(!strncmp(comstr,"end",3)){
		delete[] comstr;
		fclose(comfp);
		return;
	}
	else{
		TRACE("unknown : %s\n",comstr);
	}


	delete[] comstr;

	SetTimer(_TID_DEMO, 1000/speed, NULL); //作成

#endif	// _LIGHT

}

void CQROQSView::OnUpdateComDemo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
//	pCmdUI->Enable(false);
}




/// EOF ///

