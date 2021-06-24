// QROQSDocFile.cpp : CQROQSDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CQROQSDocFile

#include"QROQSView.h"
#include"other.h"
BOOL CQROQSDoc::SimHalt()
{
	if(PetMode!=_EDIT){
		if(AfxMessageBox("�Эڰ��ݒ��ł��D\n���f���܂����H",MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)
			==IDCANCEL){
			return false;
		}
		POSITION p= GetFirstViewPosition();
		CQROQSView *pView = (CQROQSView *)GetNextView(p);
		pView->SendMessage(WM_COMMAND,ID_COM_HALT);
	}
	return true;
}

void CQROQSDoc::InitTreeView()
{

	CTreeCtrl *pModTree = GetModuleTree();
	pModTree->DeleteAllItems();

	POSITION p = GetFirstViewPosition();
	CQROQSView *pView = (CQROQSView *)GetNextView(p);
	pView->bsp_tree = &bsp_tree;
	pView->bsp = bsp_tree.GetRootBSP();
	pView->viewDiam = pView->bsp->viewDiam;
	pView->SendMessage(WM_MOUSEWHEEL);
	pView->FitView();
	pView->InitModuleTree(bsp_tree);
	pView->ExpandAll(pModTree);
	GetModTreeDlg()->SelectRootModule();


	SetTitle(bsp_tree.GetRootName());
}

static int _hinst = 1;
//�I�[�o�[���C�h
BOOL CQROQSDoc::OnNewDocument()
{
	if(SimHalt()==false) return 0;

	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

//	fprintf(tfp,"CQROQSDoc::OnNewDocument %d\n",docst_);
//	TRACE("CQROQSDoc::OnNewDocument %d\n",docst_);

	//���łɃE�C���h�E���J���Ă����ԂȂ��docst_=3
	if(_hinst==1){
		//�ʏ�N��
		docst_ = 1;
	}
	else{
		//�R�}���h�V�K�쐬
		docst_ = 3;
	}
	_hinst=0;

	//���̎��_�ŕύX�ۑ��m�F�͂��łɍs���Ă���

	//�V�K�����N���A
	bsp_tree.reset();

	InitTreeView();

	return TRUE;
}

BOOL CQROQSDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	//	TRACE("on save:%s\n",lpszPathName);

	POSITION p = GetFirstViewPosition();
	CQROQSView *pView = (CQROQSView *)GetNextView(p);


	pView->bsp->Write(lpszPathName );

	pView->bsp->ModifiedFlag = 0;
	
	//����������1��Ԃ�
	return 1;

//	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CQROQSDoc::OnOpenDocument(LPCTSTR lpszPathName)
{

//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;

	//�A�C�R���Ƀt�@�C�����h���b�v�����ꍇ�AlpszPathName�ɂ�
	//	���łɃt�@�C�����������Ă���B

	//TRACE("on open:%s\n",lpszPathName);
//	fprintf(tfp,"CQROQSDoc::OnOpenDocument %d\n",docst_);
//	TRACE("CQROQSDoc::OnOpenDocument %d\n",docst_);

	//���łɃE�C���h�E���J���Ă����ԂȂ��docst_=3
	if(__argc>=2 && _hinst){
	//	fprintf(tfp,"%s\n",__argv[1]);

		string fpath=lpszPathName;
		fpath.erase( fpath.rfind('\\') );
		filepath = fpath.c_str();

		docst_=2;
	}
	else{
		docst_ = 3;
	}
	_hinst=0;

	try{
		BSPetriTree bspt;
		bspt.Open(lpszPathName);
		bsp_tree = bspt;
	//	TRACE("set %d\n",bsp_tree.bsps.size());
	}
	catch(char *err){
//		fprintf(tfp,"%s\n",err);
//		TRACE("error %s\n",err);
		if(docst_==2){
			//�A�C�R���Ƀh���b�O���ĊJ���Ȃ�����
			filepath.Empty();
		}
		else{
			throw err;
		}
	}


	InitTreeView();


	return 1;
}

void CQROQSDoc::OnFileOpen()
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	//	TRACE("file open doc\n");
	if(SimHalt()==false) return;

	if(SaveModifiedBSP()==IDCANCEL) return;

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
	
	//�Ō��\\�������Ă��Ȃ��Ă�����
	fd.m_ofn.lpstrInitialDir = filepath;	//�����f�B���N�g��
	
	if(fd.DoModal() != IDOK) return;
		
	CString filename = fd.GetPathName();
	//	TRACE("file name:%s %d\n",filepath,filepath.GetLength());
	string fpath=(LPCTSTR)filename;
	fpath.erase( fpath.rfind('\\') );
	filepath = fpath.c_str();
//	TRACE("file path:%s\n",filepath);

	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v

	TraceClear();
	try{
		OnOpenDocument((LPCTSTR)filename);
	}
	catch(char * err){
//		TRACE("err\n");
//		top_bsp.PetClear();
		Trace("%s %s\n",(LPCTSTR)filename,err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return;
	}

	Trace("%s : �J���܂����D\r\n",(LPCTSTR)filename);

	SetTitle(bsp_tree.GetRootName());
	
	UpdateAllViews(NULL);
	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
}

int CQROQSDoc::OnFileSave() //�㏑���ۑ�
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	
//	Trace("CQROQSDoc::OnFileSave %s\n",(LPCTSTR)filepath);

	if(filepath.IsEmpty() || GetTitle()=="untitle"){
//		TRACE("no name\n");
		return OnFileSaveAs();
	}

	// ���Ƀt�@�C�������݂��邩�m�F
	CString filename;
	filename.Format("%s\\%s.bsp",filepath,GetTitle());
//	TRACE("%s\n",(LPCTSTR)filename);
	if(GetFileAttributes(filename)==0xFFFFFFFF){//�t�@�C�����Ȃ�
//		TRACE("no file %s\n",filename);
		return OnFileSaveAs();
	}

	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v

	try{
		OnSaveDocument(filename);
	}
	catch(char * err){
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return 1;
	}
	Trace(filename + " : �㏑���ۑ����܂����D\r\n");
	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����

	return 0;
}

int CQROQSDoc::OnFileSaveAs() //���O��t���ĕۑ�
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	//	TRACE("file save as doc\n");

	if(GetTitle()=="untitle"){
		if(AfxMessageBox("ٰ�BSP����ݒ肵�ĕۑ����Ă��������D",MB_OKCANCEL|MB_ICONWARNING|MB_DEFBUTTON1)
			==IDCANCEL){
			return 1;
		}
		POSITION p = GetFirstViewPosition();
		CQROQSView *pView = (CQROQSView *)GetNextView(p);
		pView->SendMessage(WM_COMMAND,ID_BSP_PROPATY);
		return 1;
	}

	
	CFileDialog fd(
		false,	//�ۑ�
		"bsp",	//�f�t�H���g�̊g���q
		GetTitle()+".bsp",	//�f�t�H���g�̃t�@�C����
		OFN_OVERWRITEPROMPT|
		OFN_HIDEREADONLY|
		OFN_LONGNAMES,	//�t���O
		"�ޯļر�����ȯ� (*.bsp)|*.bsp|"
		"�S�Ă�̧�� (*.*)|*.*||",	//�t�B���^
		NULL	//�e�E�B���h�E
		);
	
	fd.m_ofn.lpstrInitialDir = filepath;	//�����f�B���N�g��
	
	if(fd.DoModal() != IDOK) return 1;


	CString filename = fd.GetPathName();
	//	TRACE("file name:%s %d\n",filepath,filepath.GetLength());
	string fpath=(LPCTSTR)filename;
	fpath.erase( fpath.rfind('\\') );
	filepath = fpath.c_str();

	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v

	try{
		OnSaveDocument(filename);
	}
	catch(char * err){
		Trace(err);
		AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
		return 1;
	}

	Trace(filename + " : �ۑ����܂����D\r\n");

	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����
	return 0;
}



void CQROQSDoc::OnFileSaveAll() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
//	TRACE("save all\n");

	CString get_path;
	if(getsetPath( AfxGetApp()->GetMainWnd()->m_hWnd, filepath, get_path)){
//		TRACE("getpath %s\n",get_path);
	}
	else return;

	bsp_tree.WriteAll((LPCTSTR)get_path);
	
	map<string,BSPetri*>::iterator bspit;
	bspit = bsp_tree.bsps.begin();
	for(;bspit!=bsp_tree.bsps.end();++bspit){
		Trace("%s ",bspit->first.c_str());
		bspit->second->ModifiedFlag = 0;
	}

	Trace("\n%s �Ɉꊇ�ۑ����܂����D\n",(LPCTSTR)get_path);
	filepath = get_path;
}

void CQROQSDoc::OnUpdateFileSaveAll(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	
}

/// EOF///

