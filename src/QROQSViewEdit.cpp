// QROQSViewEdit.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSView.h"


#include"FindObjDlg.h"

#include "other.h"

void CQROQSView::OnEditFind() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	CFindObjDlg objDlg;
	objDlg.m_bsp = bsp;
	if(objDlg.DoModal()==IDCANCEL){
		return;
	}

	bsp->ErrPos = objDlg.nSelID;

	if(bsp->ObjMap.find(bsp->ErrPos) != bsp->ObjMap.end()){
		//�I�u�W�F�N�g�̏ꏊ�Ɉړ�
		mst=0;
		SetTimer(_TID_FMOVE, 1, NULL); //�쐬

		//�h���
		KillTimer(_TID_OBJBEE);
		SetTimer(_TID_OBJBEE, 10, NULL); //�쐬
	}

}

void CQROQSView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	
}

void CQROQSView::OnEditUndo() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(undo_bsp[bsp->module_name].size()==0) return;

	CPoint gpoint;
	gpoint = bsp->Globalpoint;
	*bsp = undo_bsp[bsp->module_name].front();
	bsp->Globalpoint = gpoint;

	undo_bsp[bsp->module_name].pop_front();

	bsp->intrack.clear();

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	viewReset();
	
	Invalidate(0);
}


void CQROQSView::UndoSave()
{
	if(PetMode!=_EDIT) return;
	if(UndoSize==0) return;

	if(undo_bsp[bsp->module_name].size()>=UndoSize){
	//	TRACE("pop\n");
		undo_bsp[bsp->module_name].pop_back();
	}

	undo_bsp[bsp->module_name].push_front(*bsp);

//	TRACE("undo size %d/%d\n", undo_bsp[bsp->module_name].size(), UndoSize);
}

void CQROQSView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT && undo_bsp[bsp->module_name].size()!=0);
}

void CQROQSView::OnComSelectall() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	bsp->SelectAllObj();
	bsp->TrackRectSet();

	Invalidate(0);
}

void CQROQSView::OnUpdateComSelectall(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT );
}


void CQROQSView::OnComObjlap()
{

	//�I�u�W�F�N�g��0,1�̎��͋C�����邱��

	multimap<long, int> xmap;

	map<int,bspObject>::iterator oit,oit_end;
	oit = bsp->ObjMap.begin();
	oit_end = bsp->ObjMap.end();
	for(;oit!=oit_end;++oit){
		xmap.insert(
			make_pair<long,int>(oit->second.gpoint.x, oit->first)
			);
	}

#define _EDIT_KASANARI_SIZE 30

	multimap<long,int>::iterator sit,osit;

	sit = xmap.begin();
	osit = sit;
	if(sit!=xmap.end()) ++sit;
	for(;sit!= xmap.end();++sit){
		if(sit->first - osit->first < _EDIT_KASANARI_SIZE){
		//	TRACE("%d - %d = %d\n",sit->first,osit->first,sit->first-osit->first);
			if(abs(bsp->ObjMap[sit->second].gpoint.y - bsp->ObjMap[osit->second].gpoint.y) < _EDIT_KASANARI_SIZE){
			//	TRACE("	%d - %d = %d\n",bsp->ObjMap[sit->second].gpoint.y , bsp->ObjMap[osit->second].gpoint.y,bsp->ObjMap[sit->second].gpoint.y - bsp->ObjMap[osit->second].gpoint.y);
				break;
			}
		}
		osit = sit;
	}

	TraceClear();

	if(sit==xmap.end()){
		Trace("�d�Ȃ��Ĕz�u����Ă����޼ު�Ă͑��݂��܂���ł����D\n");
		return;
	}

	bsp->ErrPos = sit->second;
#ifndef _LIGHT
	help_id=42;
#endif
	Trace("��޼ު�Ă��d�Ȃ��Ĕz�u����Ă��܂��D\n");

}


/*
�R�s�[�A�y�[�X�g�́A�R�s�[�p��BSPetri���g�p���āA
�y�g���l�b�g�̃R�s�[�R���X�g���N�^������Ƃ���
*/
void CQROQSView::OnEditCopy() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
		
//	TRACE("intrack %d\n",intrack.size());

	// �R�s�[�������ɁA�R�s�[���̍��W�̓��肪�o���Ȃ�������^�������Ă���
	CRect vrect = viewRect;
	vrect.DeflateRect(40,40);
	if(!vrect.PtInRect(Oldpoint)){
		Oldpoint.x = viewRect.Width() >> 1;
		Oldpoint.y = viewRect.Height() >> 1;
	}

	//�V�K�R�s�[
	copy_bsp_tree.reset();
	set<int> puttrack;
	copy_bsp_tree.Add(copy_bsp_tree.root_name.c_str(), bsp_tree, bsp->module_name.c_str(), bsp->intrack, puttrack);
	//tkOldpoint�ɃO���[�o�����W��˂�����ł���
	tkOldpoint = mDiampoint(viewDiam,Oldpoint) + bsp->Globalpoint;
	pasteOldpoint = tkOldpoint;


//	TRACE("in copy %d\n",intrack.size());
	
}

void CQROQSView::OnEditPaste() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	//���W���[�����\��t�����邩�`�F�b�N
	//�S�ē\��t��
	set<int> oset;
	map<int,bspObject>::iterator oit;
	oit = copy_bsp_tree.GetRootBSP()->ObjMap.begin();
	for(;oit != copy_bsp_tree.GetRootBSP()->ObjMap.end();++oit){
		oset.insert(oit->first);
	}
	if(bsp_tree->IsAbleAdds(bsp->module_name.c_str(),&copy_bsp_tree,copy_bsp_tree.root_name.c_str(),oset)==0){
		AfxMessageBox("�����ɓ\��t���邱�Ƃ͂ł��܂���D\n����Ӽޭ�ق͂��łɏ��Ӽޭ�قŎg�p����Ă��܂��D",MB_OK|MB_DEFBUTTON2);
		return;
	}

	UndoSave();

	bsp_tree->Add(bsp->module_name.c_str(), &copy_bsp_tree, copy_bsp_tree.root_name.c_str(), oset, bsp->intrack);

	//TRACE("paste %d\n",intrack.size());

	// �S���������W�Ƀy�[�X�g�������͂��炷
	CPoint old_point;
	old_point = mDiampoint(viewDiam,Oldpoint) + bsp->Globalpoint;
	if(old_point == pasteOldpoint){
		Oldpoint.x += 40;
		Oldpoint.y += 40;
		old_point.x += 40;
		old_point.y += 40;
	}
	pasteOldpoint = old_point;

	ObjTrackMove(bsp->intrack,pasteOldpoint);
	bsp->TrackRectSet();


	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	pos_=1;
	
	Invalidate(0);
}

void CQROQSView::OnEditPasteHorz() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	copy_bsp_tree.GetRootBSP()->SelectAllObj();
	copy_bsp_tree.GetRootBSP()->TrackRectSet();
	CPoint mp;
	mp.x=copy_bsp_tree.GetRootBSP()->trackRect.Width()+V_CHIP_SIZE;
	mp.y=0;

	PasteChain(mp);
}

void CQROQSView::OnEditPasteVert() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	copy_bsp_tree.GetRootBSP()->SelectAllObj();
	copy_bsp_tree.GetRootBSP()->TrackRectSet();
	CPoint mp;
	mp.x=0;
	mp.y=copy_bsp_tree.GetRootBSP()->trackRect.Height()+V_CHIP_SIZE;

	PasteChain(mp);
}

#include"TokenfuncDlg.h"

void CQROQSView::PasteChain(CPoint mp) 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	//���W���[�����\��t�����邩�`�F�b�N
	//�S�ē\��t��
	set<int> oset;
	map<int,bspObject>::iterator oit;
	oit = copy_bsp_tree.GetRootBSP()->ObjMap.begin();
	for(;oit != copy_bsp_tree.GetRootBSP()->ObjMap.end();++oit){
		oset.insert(oit->first);
	}
	if(bsp_tree->IsAbleAdds(bsp->module_name.c_str(),&copy_bsp_tree,copy_bsp_tree.root_name.c_str(),oset)==0){
		AfxMessageBox("�����ɓ\��t���邱�Ƃ͂ł��܂���D\n����Ӽޭ�ق͂��łɏ��Ӽޭ�قŎg�p����Ă��܂��D",MB_OK|MB_DEFBUTTON2);
		return;
	}

	int n=0;

	//�_�C�A���O
	CTokenfuncDlg tofDlg;
	tofDlg.m_input.Format("1");
	tofDlg.m_str = "�z�u��";
	if(tofDlg.DoModal()==IDOK){
		n = atoi((LPCTSTR)tofDlg.m_input);
	}
	if(n<=0) return;

	if(n>1000){
		AfxMessageBox("1000�ȏ�̔z�u�͑������܂��I\n�������悤�Ƃ����̂��D");
		return;
	}

	UndoSave();
	int i;
	CPoint gp;
	gp=mDiampoint(viewDiam,Oldpoint)+bsp->Globalpoint;
	set<int> tkset;
	set<int>::iterator sit,sit_end;
	bsp->intrack.clear();
	for(i=0;i<n;++i){
		bsp_tree->Add(bsp->module_name.c_str(), &copy_bsp_tree, copy_bsp_tree.root_name.c_str(), oset, tkset);

		//TRACE("paste %d\n",intrack.size());
		ObjTrackMove(tkset,gp);
		gp += mp;
		sit=tkset.begin();
		sit_end = tkset.end();
		for(;sit!=sit_end;++sit){
			bsp->intrack.insert(*sit);
		}
	}

	bsp->TrackRectSet();

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());

	pos_=1;
	
	Invalidate(0);
}

void CQROQSView::OnEditDelete()
{
	UndoSave();
	set<int>::iterator tkp;
	tkp=bsp->intrack.begin();
	for(;tkp!=bsp->intrack.end();++tkp){
		if(bsp->ObjMap.find(*tkp)==bsp->ObjMap.end()){
		//	throw "�[���I�u�W�F�N�g���폜���悤�Ƃ��܂���";
		}

		if(bsp->ObjMap[*tkp].type==_PLACE){
			if(bsp->TokenMap.find(*tkp)!=bsp->TokenMap.end()){
				bsp->TokenMap.erase(*tkp);
			}
		}
		
		//�ڑ�����Ă���A�[�N�Ƌ��ɍ폜
		bsp->DelObject(*tkp);
	}
	
	bsp->trackRect.SetRectEmpty();
	bsp->intrack.clear();
/*
#ifdef _DEBUG
	//DEBUG�`�F�b�N
	if(bsp->vObjMap.find(0)!=bsp->vObjMap.end()){
		TRACE("OnEditDelete �[���I�u�W�F�N�g�����݂��Ă��܂��B�����G���[");
	}
	if(bsp->vArcMap.find(0)!=bsp->vArcMap.end()){
		TRACE("OnEditDelete �[���A�[�N�����݂��Ă��܂��B�����G���[");
	}
#endif
*/

//	TRACE("s %d\n",bsp->ModMap.size());

	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());

	pos_=1;
	Invalidate(0);
}

void CQROQSView::OnEditCut() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	/*
	//DEBUG�`�F�b�N
	if(intrack.find(0)!=intrack.end()){
		TRACE("OnEditCut �[���I�u�W�F�N�g�����݂��Ă��܂��B�����G���[");
	}
	*/

	/*
#ifdef _DEBUG
	copy_bsp_tree.GetRootBSP()->arc_check();
#endif
*/
	OnEditCopy();
	/*
	//DEBUG�`�F�b�N
	if(intrack.find(0)!=intrack.end()){
		TRACE("OnEditCut �[���I�u�W�F�N�g�����݂��Ă��܂��B�����G���[");
	}
	*/
	OnEditDelete();
}


void CQROQSView::OnComPlaceFusion() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if(bsp->intrack.size()==0) return;
	if(PutIt==0) return;
	UndoSave();
	bsp->PlaceFusion(bsp->intrack,PutIt);
	pos_=1;
	Invalidate(0);
}


void CQROQSView::OnComPlaceFission() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	if(bsp->intrack.size()==0) return;
	UndoSave();
	bsp->PlaceFission(bsp->intrack);
	bsp->TrackRectSet();
	pos_=1;
	Invalidate(0);

}


void CQROQSView::OnUpdateComPlaceFusion(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(bsp->intrack.size()!=0);
}


void CQROQSView::OnUpdateComAlign(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(bsp->intrack.size()!=0);
}

void CQROQSView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT);
}

void CQROQSView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}

void CQROQSView::OnUpdateEditPasteHorz(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}

void CQROQSView::OnUpdateEditPasteVert(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(PetMode==_EDIT && copy_bsp_tree.GetRootBSP()->ObjMap.size()!=0);
}


void CQROQSView::OnComText() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	if(bsp->ObjMap.find(PutIt)==bsp->ObjMap.end()){
		return;
	}

	//�_�C�A���O
	CTokenfuncDlg tofDlg;
	if(bsp->TextMap.find(PutIt)!=bsp->TextMap.end()){
		tofDlg.m_input = bsp->TextMap[PutIt].c_str();
	}
	else{
		tofDlg.m_input.Empty();
	}
	tofDlg.m_str = "÷�Ă��L�����Ă��������D";
	if(tofDlg.DoModal()!=IDOK){
		return;
	}
	
	if(tofDlg.m_input.IsEmpty()){
		bsp->TextMap.erase(PutIt);
	}
	else{
		bsp->TextMap[PutIt] = (LPCTSTR)tofDlg.m_input;
	}

	Invalidate(0);

}


/// EOF ///
