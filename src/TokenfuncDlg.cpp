// TokenfuncDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "QROQS.h"
#include "TokenfuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg �_�C�A���O

CTokenfuncDlg::CTokenfuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTokenfuncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTokenfuncDlg)
	m_str = _T("");
	m_input = _T("");
	//}}AFX_DATA_INIT
}

void CTokenfuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTokenfuncDlg)
	DDX_Text(pDX, IDC_EDIT_STR, m_str);
	DDV_MaxChars(pDX, m_str, 100);
	DDX_Text(pDX, IDC_TOKENVAL, m_input);
	DDV_MaxChars(pDX, m_input, 100);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CTokenfuncDlg, CDialog)
	//{{AFX_MSG_MAP(CTokenfuncDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg ���b�Z�[�W �n���h��


BOOL CTokenfuncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	UpdateData(false);

	CEdit *myED=(CEdit *)GetDlgItem(IDC_TOKENVAL);
	myED->SetFocus();
    myED->SetSel(0, -1);
	// �t�H�[�J�X���ړ������A�����ɓ��͂ł���悤�ɂ���
	return false;
  

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CTokenfuncDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(true);

	CDialog::OnOK();
}


/// EOF ///
