// QROQSViewPrint.cpp : CQROQSView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CQROQSView �N���X�̈��


//�v�����^�̎g�p�\�`�F�b�N
BOOL printerCheck()
{
    PRINTDLG p;
    p.hDevMode = 0;
    if( AfxGetApp()->GetPrinterDeviceDefaults(&p ) )
        if( p.hDevMode != NULL )    return TRUE;
    return FALSE;
}

BOOL CQROQSView::OnPreparePrinting(CPrintInfo* pInfo)
{
//	TRACE("p1\n");


	if(printerCheck()==false){
	    AfxMessageBox( "��������g�p�ł��܂���D");
		return false;
	}


//	FitView();


	if(PrintColor==0){
		delete pen_arc;
		delete pen_place;
		delete pen_transition;
		delete pen_transition_func;
		delete pen_uhzi;
		delete pen_kemmakix;
		delete pen_token;
		delete pen_module;

		delete brush_uhzi;
		delete brush_kemmakix;
		delete brush_module;
		delete pen_hatch;
		
		int i;
		i=1;
		pen_arc = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_place = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_transition = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_transition_func = new CPen(PS_SOLID, i ,RGB(0,0,0));
		pen_uhzi = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_kemmakix = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_module = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_token = new CPen(PS_SOLID, i, RGB(0,0,0));
		
		brush_uhzi = new CBrush(RGB(0,0,0));
		brush_kemmakix = new CBrush(RGB(0,0,0));
		brush_module = new CBrush(RGB(0,0,0));
		pen_hatch=new CPen(PS_SOLID, 1,RGB(0,0,0));
	}



	int m_nNowPage=0;
	int m_nBeginPage =0, m_nEndPage = 0;

	pInfo->SetMaxPage(1);

	m_nBeginPage =1;
	m_nEndPage=1;


	BOOL bPreview = pInfo->m_bPreview;	//�v���r���[�t���O�̃o�b�N�A�b�v
	if(bPreview){	//����v���r���[�̎�
		pInfo->m_nCurPage = m_nNowPage+1;	//���ݕ\�����̃y�[�W���v���r���[�J�n���̃y�[�W��
		m_nBeginPage = m_nEndPage = 0;
	}
	else{	//����̎�
		if(m_nBeginPage>0 && m_nEndPage>0){	//����͈͂̎w��L��
			pInfo->m_bPreview = TRUE;	//�v���r���[�̂ӂ�����邱�Ƃɂ��A����_�C�A���O���o���Ȃ�
		}
	}


	// �f�t�H���g�̈������
	BOOL ret = DoPreparePrinting(pInfo);
	pInfo->m_bPreview = bPreview;	//�v���r���[�t���O�����ɖ߂�

	if(m_nBeginPage>0 && m_nEndPage>0){	//����͈͂̎w��L��
		PRINTDLG *p = &pInfo->m_pPD->m_pd;
		p->nFromPage = 1;//m_nBeginPage;	//����J�n�y�[�W
		p->nToPage   = 1;//m_nEndPage;	//�ŏI����y�[�W
	}

	return ret;

	// �f�t�H���g�̈������
//	return DoPreparePrinting(pInfo);
}

void CQROQSView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
	TRACE("p2\n");


	print_now_page = 1;

	DEVMODE *dp=pInfo->m_pPD->GetDevMode();
	
	dp->dmPaperSize		= DMPAPER_A4;        	//�`�S�p��
	dp->dmOrientation	= DMORIENT_LANDSCAPE;	//������
//	dp->dmScale			= 500;	//�X�P�[��
//	dp->dmPrintQuality	= DMRES_HIGH;	//�𑜓x
//	dp->dmColor			= DMCOLOR_CIKIR ;	//�J���[

	pDC->ResetDC(dp);



}


void CQROQSView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	TRACE("print_now_page %d\n",pInfo->m_nCurPage);

	printRect = pInfo->m_rectDraw;
	if(viewRect.Width()>viewRect.Height()){
		printRect.bottom = (double)viewRect.Height() * printRect.Width() / viewRect.Width();
	}
	else{
		printRect.right = (double)viewRect.Width() * printRect.Height() / viewRect.Height();
	}



//	TRACE("pRect %d %d\n",printRect.Width(),printRect.Height());

//	bsp->Globalpoint.x = bsp->globalRect.left
//		+ viewRect.Width()*(pInfo->m_nCurPage-1);


	CView::OnPrint(pDC, pInfo);
}


void CQROQSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
	TRACE("p3\n");

	viewPenBrushChange();
	
	PetMode=_EDIT;

	Invalidate(0);

}

void CQROQSView::OnFilePrint() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	TRACE("CQROQSView::OnFilePrint\n");
//	PostMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);

	CView::OnFilePrint();
}

void CQROQSView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	
}


/// EOF ///


