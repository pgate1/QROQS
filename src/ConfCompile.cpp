// ConfCompile.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "ConfCompile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfCompile �_�C�A���O

#include"QROQSView.h"


CConfCompile::CConfCompile(CWnd* pParent /*=NULL*/)
	: CDialog(CConfCompile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfCompile)
	m_CompSave = FALSE;
	//}}AFX_DATA_INIT
}

void CConfCompile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfCompile)
	DDX_Check(pDX, IDC_CONF_EDIT_COMPSAVE, m_CompSave);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfCompile, CDialog)
	//{{AFX_MSG_MAP(CConfCompile)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfCompile ���b�Z�[�W �n���h��

BOOL CConfCompile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	m_CompSave = pView->confCompileSave;

	UpdateData(false);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CConfCompile::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	pView->confCompileSave = m_CompSave;

}


//  EOF //

