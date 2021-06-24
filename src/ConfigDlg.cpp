// ConfigDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "QROQS.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg �_�C�A���O

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg ���b�Z�[�W �n���h��

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
//	TRACE("conf init\n");

	m_Tab.InsertItem(0,"���");
	m_Tab.InsertItem(1,"Edit");
	m_Tab.InsertItem(2,"Compile");
	m_Tab.InsertItem(3,"Simulation");

	//��ʐݒ�
	m_ConfGene.m_pDoc = m_pDoc;
	m_ConfGene.Create(IDD_CONF_GENE, this);

	//�G�f�B�b�g�t�F�[�Y�ݒ�
	m_ConfEdit.m_pDoc = m_pDoc;
    m_ConfEdit.Create(IDD_CONF_EDIT, this);

	m_ConfCompile.m_pDoc = m_pDoc;
    m_ConfCompile.Create(IDD_CONF_COMPILE, this);
	//�V�~�����[�V�����t�F�[�Y�ݒ�
	m_ConfSim.m_pDoc = m_pDoc;
    m_ConfSim.Create(IDD_CONF_SIM, this);

    // �^�u�R���g���[���̕\���̈�̎擾
    CRect rect;
    m_Tab.GetWindowRect(&rect);     // �^�u�R���g���[���̃E�B���h�E�̈�i�X�N���[�����W�n�j
    ScreenToClient(&rect);          // �N���C�A���g���W�n�ɏC��
    m_Tab.AdjustRect(FALSE, &rect); // �E�B���h�E�̈�Ɋ�Â��A�\�����̗̈�ɒ���

	// �_�C�A���O�{�b�N�X�̈ʒu���킹
    m_ConfGene.MoveWindow(&rect);
    m_ConfEdit.MoveWindow(&rect);
    m_ConfCompile.MoveWindow(&rect);
    m_ConfSim.MoveWindow(&rect);

	// �ŏ��̃y�[�W��\��
	showTab(0);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CConfigDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	showTab(m_Tab.GetCurSel());
	*pResult = 0;
}

int CConfigDlg::CheckDlgData()
{
	if(m_ConfGene.UpdateData(TRUE)==0) return 0;
    if(m_ConfEdit.UpdateData(TRUE)==0) return 1;
	if(m_ConfCompile.UpdateData(TRUE)==0) return 2;
    if(m_ConfSim.UpdateData(TRUE)==0) return 3;
    return 4;
}

void CConfigDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	int nCurSel=CheckDlgData();
    if(nCurSel<4){	//DDV��肪������
		showTab(nCurSel);
		return;
	}

	m_ConfGene.update=1;

	GetParent()->Invalidate();

    CDialog::OnOK();

}

void CConfigDlg::showTab(int nCurSel){
	m_Tab.SetCurSel(nCurSel);

	// �S�Ẵy�[�W���\���ɂ���
	m_ConfGene.ShowWindow(SW_HIDE);
	m_ConfEdit.ShowWindow(SW_HIDE);
	m_ConfCompile.ShowWindow(SW_HIDE);
	m_ConfSim.ShowWindow(SW_HIDE);
	
	// �����ɊY������y�[�W�݂̂�\������
	switch(nCurSel){
	case 0: m_ConfGene.ShowWindow(SW_SHOW); break;
	case 1: m_ConfEdit.ShowWindow(SW_SHOW); break;
	case 2: m_ConfCompile.ShowWindow(SW_SHOW); break;
	case 3: m_ConfSim.ShowWindow(SW_SHOW); break;
//	default: throw "�K�肳��Ă��Ȃ��v���p�e�B�y�[�W:�����G���[";
	}
}

void CConfigDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	m_ConfGene.update = 0;

	CDialog::OnCancel();
}


/// EOF ////

