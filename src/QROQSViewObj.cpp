// QROQSViewObj.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"

#include "other.h"

/////////////////////////////////////////////////////////////////////////////
// CQROQSViewObj

void CQROQSView::OnObjPlace() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _PLACE;
	now_command=_PLACE;
	StatusOut("��ڰ���z�u���Ă�������");
	Invalidate(0);
}

void CQROQSView::OnObjTransition() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _TRANSITION;
	now_command=_TRANSITION;
	StatusOut("��ݼ޼�݂�z�u���Ă�������");
	Invalidate(0);
}

void CQROQSView::OnObjUhzi() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _UHZI;
	now_command=_UHZI;
	StatusOut("����(�����ݼ޼��)��z�u���Ă�������");
	Invalidate(0);
}

void CQROQSView::OnObjKemmakix() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _KEMMAKIX;
	now_command=_KEMMAKIX;
	StatusOut("��Ϸ(�ݸ��ݼ޼��)��z�u���Ă�������");
	Invalidate(0);
}

void CQROQSView::OnObjBtoken() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _BTOKEN;
	now_command=_BTOKEN;
	StatusOut("İ�݂�z�u���Ă�������");
	Invalidate(0);
}


void CQROQSView::OnObjModule() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	viewReset();
	SelectOBJ = _MODULE;
	now_command=_MODULE;
	StatusOut("Ӽޭ�ق�z�u���Ă�������");
	Invalidate(0);

}

void CQROQSView::OnObjArc() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _ARC;
	now_command=_ARC;
	StatusOut("�ڑ�����I�����Ă�������");
	Invalidate(0);
}

void CQROQSView::OnComChain() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _PLACE;
	now_command=_CHAIN;
	StatusOut("�A���z�u���s���܂�");
	Invalidate(0);
}



void CQROQSView::OnObjHand() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _HAND;
	now_command=_HAND;
	StatusOut("��ި");
	Invalidate(0);
}

void CQROQSView::OnObjDelete() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	viewReset();
	SelectOBJ = _DELETE;
	now_command=_DELETE;
	StatusOut("�폜�����޼ު�Ă�I�����Ă�������");
	Invalidate(0);
}


void CQROQSView::OnTimer(UINT nIDEvent)
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	switch(nIDEvent){
	case _TID_PETSIM:
		//	TRACE("st2 %d\n",st2);
		PetSim();
		break;
#ifndef _LIGHT
	case _TID_BEETOKEN:{
		if(bee_token.size()==0) return;
		
		errnudRect.SetRectEmpty();
			
		CClientDC pDC(this);
		//�T�u�ŃN���A
		BitBltRect(&pDC, &bufDC, erroudRect);
		
		int i;
		//�g�[�N�����΂�
		CRect bee_rect;
		for(i=0;i<bee_token.size();++i){
			bee_rect = bee_token[i].view(this);
			errnudRect.UnionRect(errnudRect, bee_rect);
		}
		if(bee_token[0].dd > 8){
			KillTimer(_TID_BEETOKEN);
			bee_token.clear();
			return;
		}
		
		errnudRect.InflateRect(1,1);
		erroudRect = errnudRect;
		}
		break;
#endif
	case _TID_OBJBEE:{//�G���[�\��
		
		int i;
		static int beetu=1;
		
		if(bsp->ObjMap.find(bsp->ErrPos)==bsp->ObjMap.end()){
			bsp->ErrPos=0;
		}
		if(bsp->ErrPos==0){
			KillTimer(_TID_OBJBEE);
			bee_err.clear();
			beetu=1;
			return;
		}
		
		errnudRect.SetRectEmpty();

		
		//�T�u�ŃN���A
		BitBltRect(&bufDC, &m1DC, erroudRect);
		
		CPoint mp;

		//�����΂�
		int num=Qmode==1?200:2;
		if(beetu){
			beetu=0;
			bee_err.resize(num);
			for(i=0;i<num;++i) bee_err[i]=Beecir();
		}
		mp = bsp->ObjMap[bsp->ErrPos].gpoint-bsp->Globalpoint;
		int obj_type = bsp->ObjMap[bsp->ErrPos].type;
		CRect bee_rect;
		for(i=0;i<num;++i){
			bee_rect=bee_err[i].view(this,mp,obj_type,obj_type==_TRANSITION&&bsp->TranMap[bsp->ErrPos]->pass_tra?0:1);
			bee_rect.InflateRect(2,2);
			errnudRect.UnionRect(errnudRect, bee_rect);
		}
	
		CClientDC pDC(this);
		errnudRect.InflateRect(3,3);
		erroudRect.UnionRect(erroudRect,errnudRect);
		BitBltRect(&pDC, &bufDC, erroudRect);
		erroudRect = errnudRect;
		   }
		break;
	case _TID_FMOVE:{//�G���[�\��
		if(bsp->ObjMap.find(bsp->ErrPos)==bsp->ObjMap.end())
			break;
		static CPoint gmp;
		static int nnu;
//		TRACE("s %d\n",mst);
		if(mst==0){
			nnu=10*viewDiam;
			if(nnu==0){
				gmp.x=0;
				gmp.y=0;
			}
			else{
				gmp=bsp->ObjMap[bsp->ErrPos].gpoint-bsp->Globalpoint;
				gmp.x -= (viewRect.Width()>>1);
				gmp.y -= (viewRect.Height()>>1);
				gmp.x/=nnu;
				gmp.y/=nnu;
			}
			mst=1;
		}
		else if(mst<=nnu){
			CPoint ep;
			ep = bsp->Globalpoint+gmp;
			FaceMove((ep.x-bsp->Globalpoint.x)*viewDiam,(ep.y-bsp->Globalpoint.y)*viewDiam);
			mst++;
		}
		else{
			gmp=bsp->ObjMap[bsp->ErrPos].gpoint;
			gmp.x -= (viewRect.Width()>>1);
			gmp.y -= (viewRect.Height()>>1);
			FaceMove((gmp.x-bsp->Globalpoint.x)*viewDiam,(gmp.y-bsp->Globalpoint.y)*viewDiam);
			KillTimer(_TID_FMOVE);
		}
		Invalidate(0);

		}
		break;
#ifndef _LIGHT
	case _TID_DEMO:	//�f���̎��s
		KillTimer(_TID_DEMO);
//		if(m_LeaveFlag) break;
		OnComDemo();
		break;
#endif
	case _TID_INIT:	//������
		KillTimer(_TID_INIT);
		SetWindowLong(m_wndTraceDlg->GetSafeHwnd(), GWL_EXSTYLE,
			GetWindowLong(m_wndTraceDlg->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_wndTraceDlg->GetSafeHwnd(), 0, 120, LWA_ALPHA);
		TraceClear();
		break;
	case _TID_CHECK:	// BSP�`�F�b�N�X���b�h�Ď�
		DWORD dwParam;
		GetExitCodeThread(check_thread->m_hThread, &dwParam);
		if(dwParam == STILL_ACTIVE) break;
		// CWinThread�͏I������Ǝ����Ńn���h���������悤�ɂȂ��Ă���
		delete check_thread;
		check_thread = NULL;
		KillTimer(_TID_CHECK);
		CheckEnd();
		break;

#ifndef _LIGHT
	case _TID_VOUT_CHECK:	// verilog out
		if(compOk && !check_thread){
			KillTimer(_TID_VOUT_CHECK);
			FileOutVerilogPost();
		}
		break;
	case _TID_COUT_CHECK:	// C out
		if(compOk && !check_thread){
			KillTimer(_TID_COUT_CHECK);
			FileOutCPost();
		}
		break;
	case _TID_MEA_CHECK:	// Mod Expand All
		if(compOk && !check_thread){
			KillTimer(_TID_MEA_CHECK);
			ModExpandallPost();
		}
		break;
	case _TID_RES_CHECK:	// Research
		if(compOk && !check_thread){
			KillTimer(_TID_RES_CHECK);
			ResearchPost();
		}
		break;
#endif
	}


	CView::OnTimer(nIDEvent);
}


void CQROQSView::OnComClear() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	if(AfxMessageBox("�S�č폜���܂��D\n��낵���ł����H",
		MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
		return;
	}
	
	viewReset();
	KillTimer(_TID_OBJBEE);
	
	OnComSelectall();
	OnEditDelete();
	
	TraceClear();
	Invalidate(0);
}

void CQROQSView::OnComWrite() //�N�C�b�N�Z�[�u
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	AfxGetApp()->DoWaitCursor(1); //�}�E�X�J�[�\�������v

	CQROQSApp *pApp = (CQROQSApp*)AfxGetApp();
	::SetCurrentDirectory(pApp->exe_dir);

	char *ftemp=new char[1024];
	try{
		sprintf(ftemp,"%s\\temp\\temp.txt",(LPCTSTR)pApp->exe_dir);
		FILE *fp = fopen(ftemp,"w");
		if(fp==NULL){
			throw "temp/temp.txt ̧�ق��J���܂���ł����D";
		}
		delete[] ftemp;
		fprintf(fp,"RootBSP=%s;\n",bsp_tree->GetRootName());
		fclose(fp);
		bsp_tree->WriteAll("temp");
	}
	catch(char *err){
		delete[] ftemp;
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return;
	}
	StatusOut("����̧�ق֕ۑ����܂���");
	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
}

void CQROQSView::OnComOpen() //�N�C�b�N���[�h
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	AfxGetApp()->DoWaitCursor(1); //�}�E�X�J�[�\�������v
	
	viewReset();
	undo_bsp.clear();

	CQROQSApp *pApp = (CQROQSApp*)AfxGetApp();
	::SetCurrentDirectory(pApp->exe_dir);

	char *ftemp = new char[1024];
	char *temp_root = new char[100];
	CQROQSDoc *pDoc=GetDocument();
	try{
		sprintf(ftemp,"%s\\temp\\temp.txt",(LPCTSTR)pApp->exe_dir);
		FILE *fp = fopen(ftemp,"r");
		if(fp==NULL){
			throw "temp/temp.txt ̧�ق��J���܂���ł����D";
		}
		fscanf(fp,"RootBSP=%[^;];\n",temp_root);
		fclose(fp);
		sprintf(ftemp,"%s\\temp\\%s.bsp",(LPCTSTR)pApp->exe_dir,temp_root);

		pDoc->OnOpenDocument( ftemp );
		delete[] temp_root;
		delete[] ftemp;
	}
	catch(char * err){
		delete[] temp_root;
		delete[] ftemp;
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return;
	}
	pDoc->docst_=0;
	TraceClear();
	StatusOut("����̧�ق�ǂݍ��݂܂���");
	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	pos_=1;
	
	Invalidate(0);
	
	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
}


#include"ConfigDlg.h"

void CQROQSView::OnComConfig() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CConfigDlg cfDlg;
	CQROQSDoc *pDoc = GetDocument();
	cfDlg.m_pDoc = pDoc;
	
	cfDlg.DoModal();
}


/*
�e�I�u�W�F�N�g�ݒ�_�C�A���O�́APutIt�ϐ��ɃI�u�W�F�N�g�̈ʒu��񂪓����Ă��邱��
*/
#include"TrafuncDlg.h"

void CQROQSView::OnTraFunction()
{
	CTrafuncDlg *tfDlg = new CTrafuncDlg;
	
	tfDlg->m_id = PutIt;
	tfDlg->m_mname = bsp->module_name;
	tfDlg->m_bspt = bsp_tree;
	tfDlg->m_bsp = bsp;
	tfDlg->Create(IDD_TRAFUNC,this);
	Invalidate(0);
}


#include"UhzifuncDlg.h"

void CQROQSView::OnUhziFunction()
{
	CUhzifuncDlg *ufDlg = new CUhzifuncDlg;
	
	ufDlg->m_id = PutIt;
	ufDlg->m_mname = bsp->module_name;
	ufDlg->m_bspt = bsp_tree;
	ufDlg->Create(IDD_UHZIFUNC,this);
	Invalidate(0);
}


#include"KemfuncDlg.h"

void CQROQSView::OnKemFunction()
{
	CQROQSDoc *pDoc = GetDocument();
	CKemfuncDlg *kfDlg = new CKemfuncDlg;
	
	kfDlg->m_id = PutIt;
	kfDlg->m_mname = bsp->module_name;
	kfDlg->m_bspt = bsp_tree;
	kfDlg->Create(IDD_KEMFUNC,this);
	Invalidate(0);
}


void CQROQSView::OnComAlign()
{
	if(bsp->intrack.size()==0) return;

	UndoSave();
	bsp->ModifiedFlag = 1;

	//�I�u�W�F�N�g�𐮗�
	set<int>::iterator sit,sit_end;
	sit=bsp->intrack.begin();
	sit_end = bsp->intrack.end();
	for(;sit!=sit_end;++sit){
		bsp->ObjMap[*sit].gpoint =
			alignPoint(bsp->ObjMap[*sit].gpoint,vChipSize,HatchSize);
	}

	//�A�[�N�̒��p�_�𐮗�
	vector<CPoint>::iterator bit;
	map<int,bspArc>::iterator ait,ait_end;
	ait=bsp->ArcMap.begin();
	ait_end = bsp->ArcMap.end();
	for(;ait!=ait_end;++ait){
		if(bsp->intrack.find(ait->second.In)!=bsp->intrack.end()
			&& bsp->intrack.find(ait->second.Out)!=bsp->intrack.end()
			&& ait->second.bwtpoint!=NULL){
			bit=ait->second.bwtpoint->begin();
			for(;bit!=ait->second.bwtpoint->end();++bit){
				*bit = alignPoint(*bit,vChipSize,HatchSize);
			}
		}
	}

	
	bsp->TrackRectSet();
	pos_=1;

	Invalidate(0);
}


void CQROQSView::OnViewObjId() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	EnableObjId = EnableObjId ? false : true;
	Invalidate(0);
}


void CQROQSView::OnViewOldToken() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	EnableOldToken = EnableOldToken ? false : true;
	Invalidate(0);
}



void CQROQSView::OnComIndata() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	/*
	CTokenfuncDlg getDlg;
	getDlg.m_input.Empty();
	getDlg.m_str = "��ݼ޼��ID���w��";
	if(getDlg.DoModal()!=IDOK) return;
	int tid = atoi((LPCTSTR)getDlg.m_input);
	*/

//	TRACE("t %d\n",PutIt);
	int tid = PutIt;
	if(bsp->TranMap.find(tid)==bsp->TranMap.end()) return;

	bspTransition *tran = bsp->TranMap[tid];

	multimap<int,int>::iterator aioit;
	
	if(bsp->PlaceMap[bsp->ArcMap[bsp->ArcOutMap.lower_bound(tid)->second].In]->GetTokenFlag()){
		Trace("�����ް�\n");
		aioit = bsp->ArcOutMap.lower_bound(tid);
		for(;aioit!=bsp->ArcOutMap.upper_bound(tid);++aioit){
			Trace("%s	: %d\n",bsp->ArcMap[aioit->second].value_name.c_str(), bsp->PlaceMap[bsp->ArcMap[aioit->second].In]->GetTokenValue());
		}
	}
	else{
		Trace("�o���ް�\n");
		aioit = bsp->ArcInMap.lower_bound(tid);
		for(;aioit!=bsp->ArcInMap.upper_bound(tid);++aioit){
			Trace("%s	: %d\n",bsp->ArcMap[aioit->second].value_name.c_str(), bsp->PlaceMap[bsp->ArcMap[aioit->second].Out]->GetTokenValue());
		}
	}

}


//////////////�A�b�v�f�[�g���b�Z�[�W ///////////////////////



void CQROQSView::OnUpdateObjPlace(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_PLACE);
}

void CQROQSView::OnUpdateObjTransition(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_TRANSITION);
}

void CQROQSView::OnUpdateObjUhzi(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_UHZI);
}

void CQROQSView::OnUpdateObjKemmakix(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_KEMMAKIX);
}

void CQROQSView::OnUpdateObjBtoken(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_BTOKEN);
}

void CQROQSView::OnUpdateObjArc(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_ARC);
}

void CQROQSView::OnUpdateComChain(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_CHAIN);
}


void CQROQSView::OnUpdateObjModule(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_MODULE);
}



void CQROQSView::OnUpdateObjHand(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT?true:false);
	pCmdUI->SetCheck(now_command==_HAND);
}

void CQROQSView::OnUpdateObjDelete(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
	pCmdUI->SetCheck(now_command==_DELETE);
}


void CQROQSView::OnUpdateComClear(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateComWrite(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}
void CQROQSView::OnUpdateComOpen(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateComArcsort(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(false);
}

void CQROQSView::OnUpdateViewObjId(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck((EnableObjId==1) ? true:false);
}

void CQROQSView::OnUpdateViewOldToken(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck((EnableOldToken==1) ? true:false);
}
/// EOF ///

