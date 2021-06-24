// QROQSViewExport.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"
#include "QROQSView.h"

#include "other.h"

void CQROQSView::OnComOfflineHelp() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

//	LPCTSTR pURL = "help/index.html";
//	LPCTSTR pURL = "http://oguri-195.cis.nagasaki-u.ac.jp/~sakamoto/QROQS/QROQS.html";
	LPCTSTR pURL = "http://pgate1.at-ninja.jp/QROQS/QROQS.html";
    ::ShellExecute( 0, _T("open"), pURL, 0, 0, SW_SHOWNORMAL);

}


#ifndef _LIGHT
#include "OutVerilogDlg.h"
#endif

void CQROQSView::OnFileOutVerilog() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

//	AfxMessageBox("���݂��̋@�\�͉��蒆�ł��邽�߁A���p�ł��܂���B�J���҂��������Ă�������",MB_OK);
//	return;

#ifdef _LIGHT
	AfxMessageBox("Light�ް�ޮ݂̂��ߋ@�\��������Ă��܂��D",MB_OK);
#else


	CQROQSDoc *pDoc= GetDocument();

	COutVerilogDlg vgDlg;
	vgDlg.m_pDoc = pDoc;
	vgDlg.m_OverPlus = 0.0;
	vgDlg.m_ModuleName = bsp_tree->GetRootName();
	if(vgDlg.DoModal()==IDCANCEL){
		return;
	}

	bsp->delay_slack=vgDlg.m_OverPlus;
	bsp->out_sync = vgDlg.m_VSync;
	bsp->v_debug = vgDlg.m_VDebug;

	if(::GetFileAttributes(pDoc->verilog_out_dir_list.front()) == 0xffffffff){
		Trace("̫��ނ����݂��܂���D");
		return;
	}

	vout_dir=pDoc->verilog_out_dir_list.front() + "\\" + bsp->module_name.c_str();

	if(::GetFileAttributes(vout_dir) != 0xffffffff){
		CString cs;
		cs.Format("%s : �㏑�����܂��D\n��낵���ł����H",bsp_tree->GetRootName());
		if(AfxMessageBox(cs,MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
			return;
		}
	}

	CreateDirectory(vout_dir,NULL);

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	//��U�R���p�C��
	OnComCompile();
	SetTimer(_TID_VOUT_CHECK, 100, NULL);

#endif

}


#ifndef _LIGHT
// ���X���b�h��
UINT FileOutVerilogRun(LPVOID lpParam)
{
	CQROQSView *pView = (CQROQSView*)lpParam;

	CQROQSDoc *pDoc= pView->GetDocument();
	CString csOptModule, csOptTrafunc;
	csOptModule = pDoc->verilog_module_option;
	csOptModule.Remove('\r');
	csOptTrafunc = pDoc->verilog_trafunc_option;
	csOptTrafunc.Remove('\r');

	BSPetriTree v_bsp_tree;
	v_bsp_tree = *(pView->bsp_tree);

	try{
		v_bsp_tree.OutVerilog(pView->vout_dir,(LPCTSTR)csOptModule,
			(LPCTSTR)csOptTrafunc,Trace,ProgSet);
/*
// �f�o�O�p�@�W�J�������̂�\������
		string s = bsp->module_name;
		pDoc->bsp_tree = v_bsp_tree;
		bsp = pDoc->bsp_tree.GetBSP(s.c_str());
		pos_ = 1;
*/
		Trace("Verilog-HDL���o�͂��܂����D\r\n");
		return 0;
	}
	catch(char * err){
		//TRACE("err\n");
		Trace("%s\n",err);
		/*
		if(bsp->vObjMap.find(bsp->vErrPos)!=bsp->vObjMap.end()){
			//�G���[�I�u�W�F�N�g�̏ꏊ�Ɉړ�
			//mst=0;
			//SetTimer(_TID_FMOVE, 1, NULL); //�쐬
			
			StatusOut("Verilog-HDL�ɏo�͒��ɃG���[���������܂����B������x�G�f�B�b�g���s���Ă�������");
			//�G���[�\��
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 10, NULL); //�쐬
		}
*/
	}

	return 0;
}
#endif


#ifndef _LIGHT
void CQROQSView::FileOutVerilogPost()
{
	if(compOk == false) return;
	OnComHalt();
	
	/*
	// �`�F�b�N�X���b�h�N��
	verilog_thread = AfxBeginThread(FileOutVerilogRun, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
	verilog_thread->m_bAutoDelete = FALSE;
*/
	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v

	FileOutVerilogRun((LPVOID)this);

	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v
}
#endif



#ifndef _LIGHT
#include "OutCDlg.h"
#endif

void CQROQSView::OnFileOutC() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

#ifdef _LIGHT
	AfxMessageBox("Light�ް�ޮ݂̂��ߋ@�\��������Ă��܂��D",MB_OK);

#else

	CQROQSDoc *pDoc= GetDocument();
	cout_Dlg.m_pDoc = pDoc;
	cout_Dlg.m_ModuleName = bsp_tree->GetRootName();
	if(bsp_tree->output_max==0) bsp_tree->output_max = KEM_DATA_MAX;
	cout_Dlg.m_KemMax = bsp_tree->output_max;
	if(cout_Dlg.DoModal()==IDCANCEL){
		return;
	}

	bsp_tree->output_max = cout_Dlg.m_KemMax;

	if(::GetFileAttributes(pDoc->c_out_dir_list.front()) == 0xffffffff){
		Trace("̫��ނ����݂��܂���D");
		return;
	}

	cout_dir = pDoc->c_out_dir_list.front() + "\\" + bsp->module_name.c_str();

	if(::GetFileAttributes(cout_dir) != 0xffffffff){
		CString cs;
		cs.Format("%s : �㏑�����܂��D\n��낵���ł����H",bsp_tree->GetRootName());
		if(AfxMessageBox(cs,MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)==IDCANCEL){
			return;
		}
	}
	CreateDirectory(cout_dir,NULL);

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	//��U�R���p�C��
	OnComCompile();

	SetTimer(_TID_COUT_CHECK, 100, NULL);

#endif
}

#ifndef _LIGHT
void CQROQSView::FileOutCPost()
{

	if(compOk == false) return;
	OnComHalt();

	time_t out_st;
	out_st = time(NULL);

	BSPetriTree c_bsp_tree;

	c_bsp_tree = *bsp_tree;

//	TRACE("%s\n",dir);

	AfxGetApp()->DoWaitCursor(1);//�}�E�X�J�[�\�������v
	try{
		c_bsp_tree.OutC(cout_dir, cout_Dlg.m_MainOut, cout_Dlg.m_QROQSOut, cout_Dlg.m_MakeOut, Trace);

/*
// �f�o�O�p�@�W�J�������̂�\������
		string s = bsp->module_name;
		pDoc->bsp_tree = c_bsp_tree;
		bsp = pDoc->bsp_tree.GetBSP(s.c_str());
		pos_ = 1;
*/

		Trace("C����L�q���o�͂��܂����D\r\n");
	}
	catch(char * err){
		//TRACE("err\n");
		Trace("%s\n",err);
		/*
		if(bsp->vObjMap.find(bsp->vErrPos)!=bsp->vObjMap.end()){
			//�G���[�I�u�W�F�N�g�̏ꏊ�Ɉړ�
			//mst=0;
			//SetTimer(_TID_FMOVE, 1, NULL); //�쐬
			
			StatusOut("Verilog-HDL�ɏo�͒��ɃG���[���������܂����B������x�G�f�B�b�g���s���Ă�������");
			//�G���[�\��
			KillTimer(_TID_OBJBEE);
			SetTimer(_TID_OBJBEE, 10, NULL); //�쐬
		}
*/
	}
	AfxGetApp()->DoWaitCursor(-1);//�}�E�X�J�[�\�������v����

	Trace("Time : %d\n",time(NULL)-out_st);

}
#endif


/// EOF ///
