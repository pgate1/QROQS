// QROQSViewMouse.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#include "other.h"

/////////////////////////////////////////////////////////////////////////////


BOOL CQROQSView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	if( pHandlerInfo == NULL ){	//�@���łɊ��蓖�Ă��Ă��郁�b�Z�[�W���Ȃ��ꍇ�c�c

		if(tra_io_var!=0){
			int i;
		for( i = 0; i <= 99; ++i ){
			if( nID == (UINT)( ID_IOMENU_0000 + i ) ){//�@�������̃��b�Z�[�W�Ȃ�c�c
				
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
					//�@�V���{�����I���ɂ��܂��i��������Ȃ��ƃ��j���[�s�̂܂܂ł��j�B
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


// CQROQSView �N���X�̃��b�Z�[�W �n���h��
BOOL CQROQSView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
 
    switch(pMsg->message){

	case WM_MBUTTONDOWN:
//		TRACE("m down\n");
		//��ʂ�����
	
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
			//positionface�̍X�V
			pos_=1;
		}
		Invalidate(0);
		break;
	case WM_MOUSEHOVER:
		// �E�B���h�E�Ƀ}�E�X�|�C���^������Ă��Ă��琔�b���o�߂��܂����B");
	//	SetFocus();
	//	TRACE("mouse in\n");
	    break;
    case WM_MOUSELEAVE:
		//�E�B���h�E����}�E�X�|�C���^������܂���
//		TRACE("mouse leave");
        m_LeaveFlag = TRUE;	//���̌��o�̂��߂̗\��
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
	case _PLACE:		return "��ڰ�";
	case _TRANSITION:	return "��ݼ޼��";
	case _UHZI:			return "����";
	case _KEMMAKIX:		return "��Ϸ";
	case _MODULE:		return "Ӽޭ��";
	}
	return "�s���ȵ�޼ު��";
}

CPoint CQROQSView::mouse_to_alignPoint(CPoint point)
{
	CPoint mp;
	mp = mDiampoint(viewDiam,point) + bsp->Globalpoint;
	mp = alignPoint(mp, vChipSize, HatchSize) - bsp->Globalpoint;
	return mp;
}


float zoom_diam(
	// �r���[����
	int vWidth, int vHeight,
	// �Y�[������̈�̻���
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

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
	

	// �C�ӂ̑I��̈���g��
	if(GetAsyncKeyState( VK_RBUTTON )&0x8000 || zoomrect_flag){
		ZoomRect( point );
		Invalidate();
		return;
	}

	CPoint mp,gp;

	//�����ɃI�u�W�F�N�g�����邩�ǂ���
	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		PutIt=0;
	}
	//	TRACE("mouse selectOBJ %d\n",SelectOBJ);

	int obj_type=_NONE;
	if(PutIt!=0) obj_type=bsp->ObjMap[PutIt].type;
	

	//�V�~�����Ƀg�[�N����z�u
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
			
			//�Ă������A�v���[�X��I��������Ɓ[��������񂽂��������Ƃɂ���H
			//�q�����Ă�A�[���폜
			/*
			if(AfxMessageBox("������H",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
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
			//�g�[�N����u�����Ƃ��Ă���v���[�X��
			//���łɃg�[�N������������F��ς���?

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

			//Ctrl��������Ă�����I�u�W�F�N�g��ǉ��I��
			if(pressKey==VK_CONTROL){
				bsp->intrack.insert(PutIt);
				bsp->TrackRectSet();
				Invalidate(0);
				return;
			}

			SetCapture();//���o�[�o���h�Ƌ�������̂ŁA�C������
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

			StatusOut("�ׯ�ݸޒ�...");


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
			//�I�������I�u�W�F�N�g�ɂ��킹�Ċg������
			bsp->TrackRectSet();

			StatusOut("%d �̵�޼ު�Ă�I��",bsp->intrack.size());

		}
	}

	//�I�u�W�F�N�g��u����Ȃ�u��
	if(PutIt == NULL &&
		(now_command==_PLACE
		|| now_command==_TRANSITION
		|| now_command==_MODULE
		|| now_command==_UHZI
		|| now_command==_KEMMAKIX
		|| now_command==_CHAIN)){//�����Ȃ��̂Œu��

		bsp->intrack.clear();

		UndoSave();

		// �R���g���[�������Œ�ʒu�z�u
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
			//�A���z�u�v���[�X��
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
			StatusOut("����%s��z�u���Ă�������",type_to_str(SelectOBJ));
		}
		else if(now_command==_MODULE){

		//	TRACE("%s\n",bsp->ModMap[obj].c_str());

			InitModuleTree(*bsp_tree);
			ExpandAll(GetModuleTree());

		}

		pos_ = 1;
	}
	else if(PutIt==NULL && now_command==_ARC){
		if(ArcPic==true){//betwixt �����Ȃ��Ƃ���A�o�R�_
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

		if(ArcPic==false && vArcBase!=NULL){//�ڑ���

			vArcFollow = NULL;
			vArcBase = PutIt;//�ڑ���
			varcBwt.clear();
			ArcPic = TRUE;
			vArcVal.erase();

			//�ڑ�����
			if(now_command==_UHZI){
				SelectOBJ=_PLACE;
				StatusOut("�ڑ������ڰ���I�����Ă�������");
			}
			else if(now_command==_CHAIN){
				if(obj_type==_TRANSITION || obj_type==_UHZI){
					SelectOBJ=_PLACE;
				}
				else if(obj_type==_PLACE){
					SelectOBJ=_TRANSITION;
				}
				StatusOut("����%s��z�u���Ă�������",type_to_str(SelectOBJ));
			}
		}
		else if(ArcPic==true && vArcFollow){//�ڑ���
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
			//�N���A
			BitBltRect(&myDC, &m1DC, viewRect);
		}
	}
	

//	TRACE("lbuttondown last obj %d\n",bsp->vObjMap.size());
	//TRACE("mouse down end\n");

/*
#ifdef _DEBUG
	//�`�F�b�N
	if(bsp->vObjMap.find(0)!=bsp->vObjMap.end()){
		TRACE("OnLButtonDown �[���I�u�W�F�N�g�����݂��Ă��܂��B�����G���[");
	}
	if(bsp->vArcMap.find(0)!=bsp->vArcMap.end()){
		TRACE("OnLButtonDown �[���A�[�N�����݂��Ă��܂��B�����G���[");
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	OnKeyDown(VK_ESCAPE,0,0);
	

	PutIt = PosFaceSearch(point);
	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		return;
	}
	
	
	if(SelectOBJ==_BTOKEN && bsp->ObjMap[PutIt].type==_PLACE){//���̓g�[�N���̐ݒ�
		if(bsp->TokenMap.find(PutIt) != bsp->TokenMap.end()){//���̃v���[�X�̓g�[�N����ێ����Ă���Bp�̓g�[�N�����X�g�̈ʒu
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

//	TRACE("move p (%d, %d)\n",point.x,point.y);

#ifndef _LIGHT
	if(Qmode){
		// �}�E�X�J�[�\�����璹��
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

		// ���܂ɃR�A�_���v
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
		//m_stcMessage.SetWindowText("�E�B���h�E�Ƀ}�E�X�|�C���^������Ă��܂����B");
        m_LeaveFlag = FALSE;		
        // ����WM_MOUSELEAVE,WM_MOUSEHOVER�̌��o�\��
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 0;//�b��
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
	
	//�T�u�ŃN���A
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
		// �}�E�X�|�C���^���悹����G���[�\�����~�߂�
		if(PutIt==bsp->ErrPos){
			bsp->ErrPos=0;
			Invalidate(0);
		}
		
		// �c�[���`�b�v��\��
		if(bsp->ObjMap[PutIt].type==_TRANSITION
			&& tip_displayed==0){
			m_strTip = bsp->TranMap[PutIt]->fstring.substr(0,1024).c_str();
			if(m_strTip.GetLength() > 1020){
				m_strTip += "\r\n�@�@�@�i�S�ĕ\���ł��܂���ł����j";
			}

			SetWindowStyleLayered(m_toolTip.GetSafeHwnd());
			SetLayeredWindowAttributes(m_toolTip.GetSafeHwnd(),0/*���������*/, 200, LWA_ALPHA);

		//	m_toolTip.Update(); //�c�[���`�b�v�̏�������
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
	//�V�~�����Ƀg�[�N����z�u�ł��邩�ǂ���
	if(PetMode==_SIMULATION){
		if(PutIt && bsp->vObjMap[PutIt].type==_PLACE){
			StatusOut("%s�g�[�N���������z�u�ł��܂�",
				bsp->PlaceMap[PutIt]->GetOldColor()==_WTOKEN?"L:0 R:1 ��":"��");
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
		//��ʂ̈ړ�
		//	TRACE("fmove\n");
		FaceMove(point);
		Invalidate(0);
	}
	else if(SelectOBJ==_DELETE){
		if(PutIt!=0){
			if(obj_type==_PLACE && bsp->TokenMap.find(PutIt)!=bsp->TokenMap.end()){
				StatusOut("����İ�݂��폜���܂����H");
			}
			else StatusOut("����%s���폜���܂����H",type_to_str(obj_type));

			CPoint mp;
			mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
			mp=Diampoint(viewDiam,mp);
			
			bufDC.SelectObject(&pen_wred5);
			Yard(&bufDC,mp,vChipSize+6);
			
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
		else{
			StatusOut("�폜�����޼ު�Ă�I�����Ă�������");
		}
	}
	else if(SelectOBJ==_BTOKEN){
		if(PutIt && bsp->ObjMap[PutIt].type==_PLACE){
			StatusOut("������İ�݂�z�u���܂����H");
			//�g�[�N�����u����
			CPoint mp;
			mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
			mp=Diampoint(viewDiam,mp);
			bufDC.SelectObject(&pen_wred5);
			Yard(&bufDC,mp,vChipSize+4);
			
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
		else{
			StatusOut("İ�݂�z�u������ڰ���I�����Ă�������");
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


	//�A�[�N�̐ڑ����\��
	if(ArcPic==false && PutIt!=NULL && (	//�ڑ���
		now_command==_ARC 
		|| now_command==_UHZI && obj_type==_UHZI
		|| now_command==_KEMMAKIX && obj_type==_PLACE
		|| now_command==_CHAIN
		)
		){
		
		vArcBase = PutIt;//�ڑ���

		if(!Hungface){
			CPoint mp;
			mp = bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;//�ւ񂩂������
			mp=Diampoint(viewDiam,mp);

			bufDC.SelectObject(&pen_wgreen5);
			Yard(&bufDC, mp, vChipSize+4);
			nrect.SetRect(mp.x-vChipSize,mp.y-vChipSize,mp.x+vChipSize,mp.y+vChipSize);
			nudRect.UnionRect(nudRect,nrect);
		}
	}
	else if(ArcPic){	//�ڑ���
		CPoint inp,outp;

		//���
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
			//�A�[�N�Őڑ��\
			
			vArcFollow = PutIt;

			if(!Hungface){
				CPoint mp;
				mp=bsp->ObjMap[PutIt].gpoint - bsp->Globalpoint;
				mp=Diampoint(viewDiam,mp);
				//�͂�
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
			// �R���g���[�������Œ�ʒu�z�u
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

			// �t�̱�������邩�ǂ�������
			{
				multimap<int,int>::iterator aioit;
				aioit=bsp->ArcInMap.lower_bound(vArcFollow);
				for(;aioit!=bsp->ArcInMap.upper_bound(vArcFollow);++aioit){
					if(bsp->ArcMap[aioit->second].Out==vArcBase){
						//TRACE("���Ⴍ\n");
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
						bufDC.TextOut(fillrect.left+8,fillrect.top+2,"�ڑ�����");

						nudRect.UnionRect(nudRect,fillrect);
					}
				}
				//���������B
			}

			nrect.SetRect(inp, outp);
			nrect.NormalizeRect();
			nrect.InflateRect(40,40);
			nudRect.UnionRect(nudRect,nrect);

			//�ϐ��̕\��
			if(varcBwt.size()==0){
				bufDC.SetTextColor(vColMap[_TEXT_COL]);
				PrintArcValue(&bufDC, mDiampoint(viewDiam,inp), mDiampoint(viewDiam,outp), vArcVal.c_str());
			}
		}
	}
	else vArcBase=NULL;

	//Fly�I�u�W�F�N�g�̕\��
	if(PutIt==NULL && !Hungface &&
		(
		now_command==_PLACE
		|| now_command==_TRANSITION
		|| now_command==_UHZI
		|| now_command==_KEMMAKIX
		|| now_command==_MODULE
		|| now_command==_CHAIN
		)){//�u����

		CPoint mp;
		// �R���g���[�������Œ�ʒu�z�u
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
	nudRect.InflateRect(20, 20);//�]����
	oudRect.UnionRect(oudRect, nudRect);
	BitBltRect(&myDC, &bufDC, oudRect);
	oudRect=nudRect;
	
//	TRACE("%d %d %d %d\n",oudRect.left,oudRect.top,oudRect.right,oudRect.bottom);
//	TRACE("gp %d %d\n",Globalpoint.x ,Globalpoint.y);
	
	CView::OnMouseMove(nFlags, point);
}

//�ړ��̓O���[�o�����W�ōs������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

	//	ReleaseCapture();//���o�[�o���h�Ƌ�������̂ŁA�C������
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


	if(HandlingTrack){//����ł����痣��
		ReleaseCapture();
		HandlingTrack = false;
		pos_ = 1;
		Invalidate(0);
	}
	else if(SelectOBJ==_ARC){
		if(ArcPic && vArcFollow){//�ڑ���iLButtonDown�Ɠ����j
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

//	TRACE("m %d %d\n",m_MouseLButtonDown,m_MouseRButtonDown);

	// �C�ӂ̑I��̈���g��
	if(GetAsyncKeyState( VK_LBUTTON )&0x8000 || zoomrect_flag){
		ZoomRect( point );
		Invalidate();
		return;
	}



	Oldpoint = point;	//View

	//�R���g���[���L�[��������Ă���Ȃ�΁A
	//��ʂ��n���O����
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
	//�V�~�����Ƀg�[�N����z�u
	if(PetMode==_SIMULATION){
		if(PutIt && obj_type==_PLACE){
			bsp->PlaceMap[PutIt]->SetToken(bsp->PlaceMap[PutIt]->GetOldColor()==_WTOKEN?_BTOKEN:_WTOKEN,1);
		}
	}
	*/


	if(SelectOBJ==_BTOKEN && PutIt && obj_type==_PLACE){
		//�g�[�N����u�����Ƃ��Ă���v���[�X��
		//���łɃg�[�N������������F��ς���
		map<int,bspToken>::iterator tokit;
		tokit = bsp->TokenMap.find(PutIt);
		if(tokit != bsp->TokenMap.end()){//�g�[�N�����z�u����Ă���
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	//��ʂ��͂Ȃ�
	if(Hungface==true){
		PostMessage(WM_MBUTTONUP);
		return;
	}


	// R�����������Ɨ��������̍��W��������烁�j���[�͏o���Ȃ�
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
				menu.AppendMenu(MF_STRING, ID_EDIT_COPY, "��߰");
				menu.AppendMenu(MF_STRING, ID_EDIT_CUT, "�؂���");
				menu.AppendMenu(MF_STRING, ID_EDIT_DELETE, "�폜");
				menu.AppendMenu(MF_STRING, ID_COM_ALIGN, "����");
				menu.AppendMenu(MF_STRING, ID_COM_TEXT, "÷��");

				if(obj_type){
					menu.AppendMenu(MF_SEPARATOR);
				}
			}

			switch(obj_type){
			case _PLACE:
				if(PetMode==_EDIT){
					menu.AppendMenu(MF_STRING, ID_COM_PLACE_FUSION, "��ڰ��Z��");
					menu.AppendMenu(MF_STRING, ID_COM_PLACE_FISSION, "��ڰ�����");
				}
				break;
			case _TRANSITION:
				menu.AppendMenu(MF_STRING, ID_TRA_FUNCTION, "��ݼ޼�ݕҏW");
				if(PetMode==_SIMULATION){
					menu.AppendMenu(MF_STRING, ID_COM_INDATA, "���o���ް��\��");
				}
				break;
			case _UHZI:
				menu.AppendMenu(MF_STRING, ID_UHZI_FUNCTION, "���ޕҏW");
				break;
			case _KEMMAKIX:
				menu.AppendMenu(MF_STRING, ID_KEM_FUNCTION, "��Ϸ�ҏW");
				break;
			case _MODULE:
				menu.AppendMenu(MF_STRING, ID_MOD_FUNCTION, "Ӽޭ�قֈړ�");
				if(PetMode==_EDIT){
					menu.AppendMenu(MF_STRING, ID_MODULE_READ, "Ӽޭ�ٓǂݍ���");
					menu.AppendMenu(MF_STRING, ID_MODULE_PROPATY, "Ӽޭ�ٕύX");
				}
				break;
			}
			
#ifndef _LIGHT
			menu.LoadToolBar(ToolId,RGB(192,192,192));
#endif

//			TRACE("%d\n",PutIt);

			CPoint p=point;
			ClientToScreen(&p);
			m_LeaveFlag = TRUE;	//���̌��o�̂��߂̗\��
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,this);
			return;

		}
	}

	if((SelectOBJ==_ARC || now_command==_CHAIN) && PutIt && (obj_type==_TRANSITION || obj_type==_MODULE)){
		if(ArcPic==true && vArcBase==PutIt) return;//�����Ȃ烊�^�[��
		if(ArcPic==true 
			&& (bsp->ObjMap[vArcBase].type==_TRANSITION || bsp->ObjMap[vArcBase].type==_MODULE)) return;

		//�g�����W�V�������j���[�|�b�v�A�b�v
#ifdef _LIGHT
		CMenu arc_menu;
#else
		CNewMenu arc_menu;
#endif
		arc_menu.CreatePopupMenu(); // �g�b�v���x�����j���[ 

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

				//�A�[�N�Ɏg�p����Ă���ϐ�����o�^
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

				Menu.AppendMenu(MF_STRING ,ID_IOMENU_0000 ,"�V�K�ϐ�");
				
				return;
			}
		};

	//	TRACE("%d %d %d\n",ArcPic,vArcBase,vArcFollow);

		if(obj_type==_TRANSITION){
			bspTransition &tran = *(bsp->TranMap[PutIt]);
			if(ArcPic==true){//�ڑ���i���̓A�[�N)
				if(tran.in_var.size()==0){
					try{
						tran.FuncEduction();
					}
					catch(char *err){
						bsp->ErrPos = PutIt;
						Trace("��ݼ޼�݊֐����s���ł��D\n%s",err);
					}
				}
#ifndef _LIGHT
		
				arc_menu.SetMenuTitle( "Input", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,tran.in_var,bsp->ArcOutMap,bsp->ArcMap,arc_menu);
			}
			else{//�ڑ���(�o�̓A�[�N)
				if(tran.out_var.size()==0){
					try{
						tran.FuncEduction();
					}
					catch(char *err){
						bsp->ErrPos = PutIt;
						Trace("��ݼ޼�݊֐����s���ł��D\n%s",err);
					}
				}
#ifndef _LIGHT
				arc_menu.SetMenuTitle( "Output", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
				innerfunc::arc_to_menu(PutIt,tran.out_var,bsp->ArcInMap,bsp->ArcMap,arc_menu);
			}
		}
		else if(obj_type==_MODULE){
			
			// ���W���[���̓��o�͂Ƃ��đI���ł���̂́A���O�����Ă�����̂̂�

			BSPetri *sub_bsp = bsp_tree->GetBSP( bsp->ModMap[PutIt].c_str() );
			map<string,int> iovar;
			if(ArcPic==true){//�ڑ���i���̓A�[�N)
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
			else{//�ڑ���(�o�̓A�[�N)
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


		// ���C���E�B���h�E�֕\�����Ă��܂��B
		CPoint p=point;
		ClientToScreen(&p);
//		m_LeaveFlag = TRUE;	//���̌��o�̂��߂̗\��

		//���͏o�͂�����̃A�[�N������
		tra_io_var = (ArcPic==1 ? 1 : 2);
		arc_menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,this);
	}

	if(now_command!=_CHAIN && PutIt == NULL && PetMode==_EDIT){
		//��ʃ��j���[
		
		CMenu cput_menu;
		cput_menu.CreatePopupMenu();
		cput_menu.AppendMenu(MF_STRING, ID_EDIT_PASTE_HORZ, "��������");
		cput_menu.AppendMenu(MF_STRING, ID_EDIT_PASTE_VERT, "��������");

#ifdef _LIGHT
		CMenu menu;
		menu.CreatePopupMenu();
#else
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.SetMenuTitle( "Layout", MFT_GRADIENT|MFT_TOP_TITLE);
#endif
		menu.AppendMenu(MF_STRING, ID_COM_ZOOMRECT, "�ް�");
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, "�\��t��");
//		TRACE("%d\n",(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->vObjMap.size()!=0));
		menu.AppendMenu(MF_POPUP |
			((PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0) ? NULL: MF_GRAYED ),
			(UINT)cput_menu.m_hMenu, "�A���z�u");

#ifndef _LIGHT
		menu.LoadToolBar(IDR_TOOLBAR_SUB);
#endif

		// ���C���E�B���h�E�֕\�����Ă��܂��B
		CPoint p=point;
		ClientToScreen(&p);
		m_LeaveFlag = TRUE;	//���̌��o�̂��߂̗\��
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y,AfxGetMainWnd());
	}
	
	Invalidate(0);


	CView::OnRButtonUp(nFlags, point);
}



BOOL CQROQSView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	int shadow_wheel=0;

	if(nFlags && MK_MBUTTON){//�^���̃{�^��
		//	Trace("kkk");
		shadow_wheel = 1;
	}
	//	TRACE("key : %d\n",pressKey);
	int i;
	
	if(pressKey=='s' || shadow_wheel){
		if(zDelta<0){//��O�[���g��
			diamDist /= diamDelta;
			if(diamDist>1) diamDist=1;
		}
		else if(zDelta>0){//�������[���k��
			diamDist *= diamDelta;
			if(diamDist<0.025) diamDist=(float)0.025;
		}
	}
	else{
	//		TRACE("mouse wheel m %d\n",zDelta);
		if(zDelta<0){//��O�[���g��
			for(i=0;i<(-zDelta / WHEEL_DELTA);i++){
				viewDiam /= diamDelta;
			}
		}
		else if(zDelta>0){//�������[���k��
			for(i=0;i<(zDelta / WHEEL_DELTA);i++){
				viewDiam *= diamDelta;
			}
		}
		else{
			// viewDiam�͌��ݐݒ肳��Ă���l���g�p����
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
			//���߂̂��߂��ꂼ��̃I�u�W�F�N�g�̌��_�̐F�𓧉ߐF�Ƃ���
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	/*
	SB_LEFT  //���[�փX�N���[��  
	//SB_LINELEFT  //���փX�N���[��  �i�{�^��
	//SB_PAGELEFT  //1 �y�[�W���փX�N���[��  �i�V���t�g
	//SB_PAGERIGHT  //1 �y�[�W�E�փX�N���[��  �i�V���t�g
	//SB_LINERIGHT  //�E�փX�N���[��  �i�{�^��
	SB_RIGHT  //�E�[�փX�N���[��  
	SB_ENDSCROLL  //�X�N���[���I��  
	//SB_THUMBTRACK://�X���C�_�[�h���b�O��
	SB_THUMBPOSITION //��Έʒu�փX�N���[��  �i�o�[�������ꂽ
	*/

	
	
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_HORZ,&scrollinfo);
	int sflag=false;
	
	switch(nSBCode){
	case SB_LINELEFT://���փX�N���[��  �i�{�^��
		FaceMove(-10,0);
		Invalidate(0);
		break;
	case SB_PAGELEFT:  //1 �y�[�W���փX�N���[��  �i�V���t�g
		FaceMove(-viewRect.Width(),0);
		Invalidate(0);
		break;
	case SB_THUMBTRACK://�X���C�_�[�h���b�O��
		bsp->Globalpoint.x = scrollinfo.nTrackPos;   //�X���C�_�[�h���b�O���̈ʒu
		scrollinfo.nPos = bsp->Globalpoint.x;   
		sflag=1;
		break;
	case SB_PAGERIGHT:  //1 �y�[�W�E�փX�N���[��  �i�V���t�g
		FaceMove(viewRect.Width(),0);
		Invalidate(0);
		break;
	case SB_LINERIGHT://�E�փX�N���[��  �i�{�^��
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	/*
	SB_TOP  //��ԏ�܂ŃX�N���[��  
	//SB_LINEUP  //1 �s��փX�N���[��  �i�{�^��
	//SB_PAGEUP  //1 �y�[�W��փX�N���[��  �i�V���t�g
	//SB_PAGEDOWN  //1 �y�[�W���փX�N���[��  �i�V���t�g
	//SB_LINEDOWN  //1 �s���փX�N���[�� (�{�^�� 
	SB_BOTTOM://��ԉ��܂ŃX�N���[��
	SB_ENDSCROLL  //�X�N���[���I��  
	//SB_THUMBTRACK://�X���C�_�[�h���b�O��
	SB_THUMBPOSITION //��Έʒu�փX�N���[��  �i�o�[�������ꂽ
	*/
	
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT,&scrollinfo);
	int sflag=false;
	
	switch(nSBCode){
	case SB_LINEUP:  //1 �s��փX�N���[��  �i�{�^��
		FaceMove(0,-10);
		Invalidate(0);
		break;
	case SB_PAGEUP:  //1 �y�[�W��փX�N���[��  �i�V���t�g
		FaceMove(0,-viewRect.Height());
		Invalidate(0);
		break;
	case SB_THUMBTRACK://�X���C�_�[�h���b�O��        
		bsp->Globalpoint.y = scrollinfo.nTrackPos;    //�X���C�_�[�h���b�O���̈ʒu
		scrollinfo.nPos = bsp->Globalpoint.y;   
		sflag=1;
		break;
	case SB_PAGEDOWN:  //1 �y�[�W���փX�N���[��  �i�V���t�g
		FaceMove(0,viewRect.Height());
		Invalidate(0);
		break;
	case SB_LINEDOWN:  //1 �s���փX�N���[�� (�{�^�� 
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
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
//	fprintf(tfp,"CQROQSView::OnSize %d\n",bsp);
//	TRACE("CQROQSView::OnSize\n");


	if(bsp==NULL) return;
	

	GetClientRect(viewRect);
//	TRACE("%d,%d\n",cx,cy);
//	TRACE("viewRect %d %d %d %d\n",viewRect.left,viewRect.top,viewRect.right,viewRect.bottom);	
		
	//DIB�ň�����悤�ɕ����S�̔{���Ƃ���
	if(viewRect.right%4) viewRect.right += (4 - viewRect.right%4);


	ScrollSet();

	//position face�̏�����
	if(Posface != NULL) delete[] Posface;
	Posface = new int[viewRect.Width()*viewRect.Height()];
	
	oudRect = viewRect;
	nudRect = viewRect;
	
	BITMAPINFOHEADER m1BmInfoHed;
	BITMAPINFO m1BmInfo;

	ZeroMemory(&m1BmInfoHed, sizeof(BITMAPINFOHEADER));
	
	m1BmInfoHed.biSize=sizeof(BITMAPINFOHEADER); /* �\���̂̑傫�� */
	m1BmInfoHed.biWidth=viewRect.Width();                     /* ���� */
	m1BmInfoHed.biHeight=-viewRect.Height();                    /* ���� */
	m1BmInfoHed.biPlanes=1;                     /* �v���[���̐� */
	m1BmInfoHed.biBitCount=24;                  /* �v���[���̐F�� */
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

	miniBmInfo->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); // �\���̂̑傫�� 
	miniBmInfo->bmiHeader.biWidth=viewRect.Width();              // ���� 
	miniBmInfo->bmiHeader.biHeight=-viewRect.Height();         // ���� 
	miniBmInfo->bmiHeader.biPlanes=1;            //�v���[���̐� 
	miniBmInfo->bmiHeader.biBitCount=24;          //�v���[���̐F�� 
	miniBmInfo->bmiHeader.biCompression=BI_RGB;
		
	/*
	//�J���[�e�[�u�����g���ꍇ
	RGBQUAD *minilpRGB;
	minilpRGB = (RGBQUAD*)(miniBmInfo+sizeof(BITMAPINFOHEADER));
	//miniBmInfo->bmiColors[0]=minilpRGB[0]; // �J���[�e�[�u���̐擪
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
	


	// ���b�Z�[�W�_�C�A���O�̈ړ�
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
		// �X�N���[���o�[������ɓ��삷��悤�ɁAviewRect�ɒl�����Ă���
		viewRect.SetRect(0,0,600,400);
	}

	if(bsp->ObjMap.size()!=0){
		//�t�@�C�����J�����璆�S��\��
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
	//Globalpoint�ɍ��킹�Ċg��
	
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
	scrollinfo.nPage = viewRect.Width();	//�y�[�W��
	scrollinfo.nPos = bsp->Globalpoint.x;
	scrollinfo.fMask |= SIF_ALL ;//| SIF_DISABLENOSCROLL;
	SetScrollInfo(SB_HORZ, &scrollinfo, true);


	GetScrollInfo(SB_VERT, &scrollinfo);
	scrollinfo.nMin = bsp->globalRect.top;
	scrollinfo.nMax = bsp->globalRect.bottom;
	scrollinfo.nPage = viewRect.Height();	//�y�[�W��
	scrollinfo.nPos = bsp->Globalpoint.y;
	scrollinfo.fMask |= SIF_ALL ;//| SIF_DISABLENOSCROLL;
	SetScrollInfo(SB_VERT, &scrollinfo, true);
	
}

void CQROQSView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	

	//�őO�ʂ�
	::SetForegroundWindow(this->GetSafeHwnd());
//	ForegroundWindow(this->GetSafeHwnd());

	CQROQSDoc *pDoc = GetDocument();
	if(pDoc->SimHalt()==false) return;
	if(pDoc->SaveModifiedBSP()==IDCANCEL) return;


	//�S�ăN���A
	viewReset();

	undo_bsp.clear();
	
	KillTimer(_TID_OBJBEE);


	//�ǂݍ���
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

	//������Oldpoint�͕ύX�����
	//�X�N���[���o�[�����ڑ��삳�ꂽ�ꍇ�Ƌ�������̂�

	CPoint p = Oldpoint;
	p.x -= mx;
	p.y -= my;
	
	FaceMove(p);
}

void CQROQSView::FaceMove(CPoint point)
{
//	TRACE("face st %d %d\n",Oldpoint.x,Oldpoint.y);

	//��ʂ̈ړ�
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

//�g�����W�V�����̓��͕ϐ��֐ڑ�����
void CQROQSView::OnTraInsort()
{
//	TRACE("in var %d\n",tra_io_var);
	if(tra_io_var==0){
		//�_�C�A���O
		CTokenfuncDlg tofDlg;
		tofDlg.m_input = "";
		tofDlg.m_str = "���͕ϐ����L�����Ă��������D";
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
	vArcBase = PutIt;//�ڑ���
	varcBwt.clear();
	ArcPic = TRUE;

	if(tra_io_var==0){
		//�_�C�A���O
		CTokenfuncDlg tofDlg;
		tofDlg.m_input = "";
		tofDlg.m_str = "�o�͕ϐ����L�����Ă��������D";
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

	StatusOut("�ϐ� %s �̏o����ڰ���I�����Ă�������",vArcVal.c_str());

	tra_io_var=0;
}


BOOL CQROQSView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

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

	//����ɂ��J�[�\���������I�Ɍ��ɖ߂��Ȃ�
//	return 0;

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CQROQSView::OnComZoomrect() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	zoomrect_flag = 1;

	StatusOut("�g��\���������̈���ׯ�ݸނ��Ă�������");

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

	StatusOut("��ި");

	return;
}


/// EOF ///


