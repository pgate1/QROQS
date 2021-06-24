// ConfSim.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "ConfSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfSim �_�C�A���O

CConfSim::CConfSim(CWnd* pParent /*=NULL*/)
	: CDialog(CConfSim::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfSim)
	m_SimSpeed = 10;
	m_TokenMove = FALSE;
	m_SimAnime = FALSE;
	m_SimundoSize = 0;
	m_ViewDataSize = 0;
	m_BinValue = FALSE;
	m_TokenTrans = FALSE;
	//}}AFX_DATA_INIT
}

void CConfSim::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfSim)
	DDX_Text(pDX, IDC_CONF_SIM_SPEED, m_SimSpeed);
	DDV_MinMaxUInt(pDX, m_SimSpeed, 1, 5000);
	DDX_Check(pDX, IDC_TOKENMOVE, m_TokenMove);
	DDX_Check(pDX, IDC_CONF_SIM_ANIMATION, m_SimAnime);
	DDX_Text(pDX, IDC_UNDOSIZE, m_SimundoSize);
	DDV_MinMaxInt(pDX, m_SimundoSize, 0, 100);
	DDX_Text(pDX, IDC_DATA_VIEW_SIZE, m_ViewDataSize);
	DDV_MinMaxInt(pDX, m_ViewDataSize, 0, 1024);
	DDX_Check(pDX, IDC_CONF_SIM_BINVAL, m_BinValue);
	DDX_Check(pDX, IDC_TOKENTRANS, m_TokenTrans);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfSim, CDialog)
	//{{AFX_MSG_MAP(CConfSim)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FASTSIM, OnFastsim)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfSim ���b�Z�[�W �n���h��

#include"QROQSView.h"

BOOL CConfSim::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
//	TRACE("sim init\n");

	m_SimSpeed = m_pDoc->SimSpeed;

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	m_SimAnime = pView->confSimAnime;
	m_TokenMove = pView->confSimTokenMove;
	m_TokenTrans = pView->confSimTokenTrans;
	m_SimundoSize = pView->SimundoSize;
	m_ViewDataSize = pView->confDataViewMax;
	m_BinValue = pView->confSimBinValue;

	UpdateData(false);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CConfSim::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	m_pDoc->SimSpeed = m_SimSpeed;

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	pView->confSimAnime = m_SimAnime;
	pView->confSimTokenMove = m_TokenMove;
	pView->confSimTokenTrans = m_TokenTrans;
	pView->SimundoSize = m_SimundoSize;
	pView->confDataViewMax = m_ViewDataSize;
	pView->confSimBinValue = m_BinValue;

}


void CConfSim::OnFastsim() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	m_TokenMove = false;
	m_TokenTrans = false;
	m_SimundoSize = 0;
	m_SimSpeed = 1;
	m_BinValue = false;

	UpdateData(false);

}

//	EOF	//
