// QROQSViewModule.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"
#include "QROQSView.h"

#include "other.h"

void CQROQSView::InitModuleTree(BSPetriTree &bsp_tree)
{

//	TRACE("init tree\n");

	struct func{
		static void add_tree(
			BSPetriTree &bspt,
			CTreeCtrl *pModTree,
			HTREEITEM htree,
			BSPetri *cur_bsp)
		{
	//		TRACE("root %s\n",cur_bsp->module_name.c_str());

			HTREEITEM chtree, nextitem;
			map<int,HTREEITEM> tmap;
			int idata;

			//�f�[�^�ɑ��݂��Ȃ�������c���[����폜
			chtree = pModTree->GetChildItem(htree);
			for(;chtree!=NULL;){
				// �o�^
				idata = pModTree->GetItemData(chtree);
			//	TRACE("%d\n", idata);
				tmap[idata] = chtree;
				
				nextitem = pModTree->GetNextSiblingItem(chtree);
				if(cur_bsp->ModMap.find(idata) == cur_bsp->ModMap.end()){
				//	TRACE("del %s\n",pModTree->GetItemText(chtree));
					pModTree->DeleteItem(chtree);
					tmap.erase(idata);
				}
				chtree = nextitem;
			}

		//	TRACE("idata %d\n", tmap.size());

			map<int, string>::iterator mit,mit_end;
			mit = cur_bsp->ModMap.begin();
			mit_end = cur_bsp->ModMap.end();
			for(;mit!=mit_end;++mit){
				idata = mit->first;
				if(tmap.find(idata)==tmap.end()){
					// �c���[�ɑ��݂��Ȃ��̂ŐV�K�ǉ�����
					chtree = pModTree->InsertItem(
						mit->second.empty() ? "no_name" : mit->second.c_str(),
						htree, TVI_LAST);
					pModTree->SetItemData( chtree, idata);
				//	TRACE("add tree\n");
				}
				else{
					chtree = tmap[idata];
					if(!mit->second.empty()
						&& pModTree->GetItemText(chtree) != mit->second.c_str()){
						// ���O���Ⴄ�̂ŕύX
				//		TRACE("change name : %s -> %s\n", pModTree->GetItemText(chtree), mit->second.c_str());
						pModTree->SetItemText(chtree, mit->second.c_str());
					}
				}

				if(!mit->second.empty()){
				//	TRACE("%s %d\n",mit->second.c_str(),bspt.GetBSP(mit->second.c_str()));
					add_tree(bspt, pModTree, chtree, bspt.GetBSP(mit->second.c_str()));
				}
			}

			return;
		}
	};


	CTreeCtrl *pModTree = GetModuleTree();

	HTREEITEM htree;
	htree = pModTree->GetRootItem();
	if(htree==NULL){
		// �Ȃ��Ȃ�ǉ�
	//	TRACE("add root %s\n",bsp_tree.root_name.c_str());
		htree = pModTree->InsertItem(bsp_tree.root_name.c_str(),TVI_ROOT, TVI_LAST);
		pModTree->SetItemData( htree, 0);
	}
	else if(pModTree->GetItemText(htree) != bsp_tree.root_name.c_str()){
		// ���O���Ⴄ�Ȃ�ύX
		pModTree->SetItemText(htree, bsp_tree.root_name.c_str());
	}

	// ���b�N�������čĕ`�悳���Ȃ�
	CWnd::LockWindowUpdate();
	func::add_tree(bsp_tree, pModTree, htree, bsp_tree.GetRootBSP());
	CWnd::UnlockWindowUpdate();

}


void CQROQSView::OnModuleUp()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
//	TRACE("module up\n");
	CTreeCtrl *pModTree = GetModuleTree();
	if(current_item == pModTree->GetRootItem()){
		return;
	}

	current_item = pModTree->GetParentItem(current_item);
	pModTree->SelectItem(current_item);

	pos_=1;
	Invalidate(0);
}

void CQROQSView::OnUpdateModuleUp(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������

//	pCmdUI->Enable(current_item != GetModuleTree()->GetRootItem());
}


void CQROQSView::OnModFunction()
{
	if(bsp->ModMap[PutIt].empty()){
		//module_name���ݒ肳��Ă��Ȃ��̂�
		//�ݒ�_�C�A���O��\������
		if(PetMode==_EDIT){
			OnModulePropaty();
		}
	}
	else{
		//module name���ݒ肳��Ă���Ȃ牺�ʃ��W���[���Ɉړ�����
		CTreeCtrl *pModTree = GetModuleTree();

		HTREEITEM hitem;
		hitem = pModTree->GetChildItem(current_item);
		for(;hitem!=NULL;){
			if(pModTree->GetItemData(hitem) == PutIt) break;
			hitem = pModTree->GetNextSiblingItem(hitem);
		}
		if(hitem==NULL){
			TRACE("���ʃ��W���[�����Ȃ��F�����G���[");
			return;
		}
		
		pModTree->SelectItem(hitem);

		if(bsp->ErrPos){		//�G���[�\��
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 20, NULL); //�쐬
		}
	}

	Invalidate(0);
}

#include"ModuleManagerDlg.h"

void CQROQSView::OnModManager() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
	CModuleManagerDlg mmDlg;
	mmDlg.m_bsp_tree = bsp_tree;
	mmDlg.m_SelMod = bsp->module_name.c_str();
	if(mmDlg.DoModal()==IDCANCEL){
		return;
	}
/*
	if(!mmDlg.m_SelMod.IsEmpty()){
		GetModTreeDlg()->SelectModule((LPCTSTR)mmDlg.m_SelMod);
	}
*/
}


#include"TokenfuncDlg.h"

void CQROQSView::OnModulePropaty() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
//	TRACE("propaty %s %d\n",bsp->module_name.c_str(),PutIt);
//TRACE("p %d\n",PutIt);
	//�ݒ肷�郂�W���[���Ɉړ����Ă�������

	CTokenfuncDlg mnameDlg;
	if(bsp->ModMap.find(PutIt)!=bsp->ModMap.end()){
		mnameDlg.m_input = bsp->ModMap[PutIt].c_str();
	}
	else if(PutIt==0){
		mnameDlg.m_input = bsp_tree->GetRootName();
	}
	else{
		mnameDlg.m_input.Empty();
	}
	mnameDlg.m_str = "Ӽޭ�ٖ���ݒ肵�Ă��������D";

	if(mnameDlg.DoModal()==IDOK){
//		TRACE("set %s\n",mnameDlg.m_input);
		if(mnameDlg.m_input.IsEmpty()) return;
		UndoSave();
		int ret;
		ret = bsp_tree->SetModuleName(bsp->module_name.c_str(), PutIt, (LPCTSTR)mnameDlg.m_input);
		if(ret==0){
			CString cs;
			cs = mnameDlg.m_input + " : �ύX�ł��܂���D\n";
			cs += bsp_tree->err_msg.c_str();
			AfxMessageBox(cs,MB_OK|MB_DEFBUTTON2);
#ifndef _LIGHT
			help_id = 4;
#endif
			return;
		}

		if(PutIt==0){
			bsp = bsp_tree->GetRootBSP();
			CQROQSDoc *pDoc = GetDocument();
			pDoc->SetTitle( bsp->module_name.c_str() );
		}

		InitModuleTree(*bsp_tree);
		ExpandAll(GetModuleTree());
		Invalidate(0);
	}

}

// Ӽޭ���ذ����
void CQROQSView::ExpandAll(CTreeCtrl *pTree)
{
	return;	// �W�J���Ȃ�

	struct func{
		static void expand(CTreeCtrl *pTree, HTREEITEM cur)
		{
			for(;cur!=NULL;){
				pTree->Expand(cur, TVE_EXPAND);
				expand(pTree, pTree->GetChildItem(cur));
				cur = pTree->GetNextSiblingItem( cur );
			}
		}
	};
	// ���b�N�������čĕ`�悳���Ȃ�
	CWnd::LockWindowUpdate();
	func::expand(pTree, pTree->GetRootItem());
	CWnd::UnlockWindowUpdate();

}

void CQROQSView::OnBspPropaty()
{
	PutIt = 0;

	CTreeCtrl *pModTree = GetModuleTree();
	pModTree->SelectItem( pModTree->GetRootItem() );
	OnModulePropaty();
}


void CQROQSView::OnModuleRead()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	CFileDialog fd(
		TRUE,	//�J��
		"bsp",	//�f�t�H���g�̊g���q
		"",	//�f�t�H���g�̃t�@�C����
		OFN_FILEMUSTEXIST|
		OFN_HIDEREADONLY|
		OFN_LONGNAMES,	//�t���O
		"�ޯļر�����ȯ� (*.bsp)|*.bsp|"
		"�S�Ă�̧�� (*.*)|*.*||",	//�t�B���^
		NULL	//�e�E�B���h�E
		);
	
	CQROQSDoc *pDoc = GetDocument();
	fd.m_ofn.lpstrInitialDir = pDoc->filepath;	//�����f�B���N�g��
	
	if(fd.DoModal() != IDOK) return;
	
	CString filename = fd.GetPathName();

	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v

	try{
		UndoSave();

		BSPetriTree read_bsp_tree;
		read_bsp_tree.Open((LPCTSTR)filename);

		string mname;
		mname = read_bsp_tree.GetRootName();

		int ret;
		ret = bsp_tree->SetModuleName(bsp->module_name.c_str(), PutIt, mname.c_str());
		if(ret==0){
			string err;
			err = mname + " : �ύX�ł��܂���D\n" + bsp_tree->err_msg;
			AfxMessageBox(err.c_str(),MB_OK|MB_DEFBUTTON2);
#ifndef _LIGHT
			help_id = 4;
#endif
			return;
		}	
		
		InitModuleTree(*bsp_tree);


		//���W���[�����\��t�����邩�`�F�b�N
		//�S�ē\��t��
		set<int> oset;
		map<int,bspObject>::iterator oit,oit_end;
		oit = read_bsp_tree.GetRootBSP()->ObjMap.begin();
		oit_end = read_bsp_tree.GetRootBSP()->ObjMap.end();
		for(;oit!=oit_end;++oit){
			oset.insert(oit->first);
		}
		if(bsp_tree->IsAbleAdds(mname.c_str(),&read_bsp_tree,read_bsp_tree.GetRootName(),oset)==0){
			AfxMessageBox("Ӽޭ�ق�ǂݍ��ނ��Ƃ͂ł��܂���D\n���łɏ��Ӽޭ�قŉ���Ӽޭ�ق��g�p����Ă��܂��D",MB_OK|MB_DEFBUTTON2);
			OnEditUndo();
			return;
		}



		//�ǂݍ��񂾃��W���[���͏㏑��
		//�_�X�g���W���[���͍폜����
		//�Q�Ƃ���Ă��郂�W���[��������Ȃ�㏑�����Ȃ�

		delete bsp_tree->GetBSP(mname.c_str());
		bsp_tree->bsps.erase( mname );
//		bsp_tree->DeleteNonRefer();

		map<string,BSPetri*>::iterator bspit;
		bspit = read_bsp_tree.bsps.begin();
		for(;bspit!=read_bsp_tree.bsps.end();++bspit){
			if(bsp_tree->GetBSP( bspit->first.c_str() ) == NULL){
				bsp_tree->bsps[ bspit->first ] = new BSPetri;
				*(bsp_tree->bsps[ bspit->first ]) = *(bspit->second);
			}
		}

		Trace("%s : �ǂݍ��݂܂����D\r\n",(LPCTSTR)filename);
	}
	catch(char * err){
//		TRACE("err\n");
//		top_bsp.PetClear();
		Trace("%s %s\n",(LPCTSTR)filename,err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return;
	}

	
	InitModuleTree(*bsp_tree);
	ExpandAll(GetModuleTree());
	Invalidate(0);

	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����

}


void CQROQSView::OnModExpandall() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
#ifdef _LIGHT
	AfxMessageBox("Light�ް�ޮ݂ł͋@�\��������Ă��܂��D",MB_OK);
#else
	if(AfxMessageBox(
		"�S�Ă�Ӽޭ�ق�W�J���܂��D\n��낵���ł����H",
		MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
		return;
	}

	//��U�R���p�C��
	OnComCompile();
	SetTimer(_TID_MEA_CHECK, 100, NULL);
#endif

}

#ifndef _LIGHT
void CQROQSView::ModExpandallPost()
{

	if(compOk == false) return;
	OnComHalt();

	Trace("�W�J��...\n");

	BSPetriTree exp_bsp_tree;

	exp_bsp_tree = *bsp_tree;
	exp_bsp_tree.NormalizeGlobalRect();

time_t st;
st = time(NULL);
	exp_bsp_tree.ExpandBSPAll( exp_bsp_tree.GetBSP(bsp->module_name.c_str()) , true);
Trace("expand %d sec\n",time(NULL)-st);

	*bsp = *( exp_bsp_tree.GetBSP(bsp->module_name.c_str()) );

	bsp_tree->DeleteNonRefer();

	InitModuleTree(*bsp_tree);
	pos_ = 1;
	Invalidate(0);

	Trace("Ӽޭ�ق�W�J���܂����D\n");
}
#endif



void CQROQSView::OnUpdateModulePropaty(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(true);
}

void CQROQSView::OnUpdateModuleRead(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(true);
}


/// EOF ///
