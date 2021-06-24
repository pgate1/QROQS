// ConfEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "ConfEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfEdit �_�C�A���O
#include"QROQSView.h"

CConfEdit::CConfEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CConfEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfEdit)
	m_HatchSize = 10;
	m_UndoSize = 0;
	m_ArcInMove = FALSE;
	m_ArcOutMove = FALSE;
	m_AlwaysAlign = FALSE;
	m_GridType = -1;
	//}}AFX_DATA_INIT
}

void CConfEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfEdit)
	DDX_Text(pDX, IDC_HATCHSIZE, m_HatchSize);
	DDV_MinMaxUInt(pDX, m_HatchSize, 1, 5000);
	DDX_Text(pDX, IDC_UNDOSIZE, m_UndoSize);
	DDV_MinMaxInt(pDX, m_UndoSize, 0, 10);
	DDX_Check(pDX, IDC_CHECK_ARC_IN_MOVE, m_ArcInMove);
	DDX_Check(pDX, IDC_CHECK_ARC_OUT_MOVE, m_ArcOutMove);
	DDX_Check(pDX, IDC_ALWAYS_ALIGN, m_AlwaysAlign);
	DDX_Radio(pDX, IDC_GRID_LINE, m_GridType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfEdit, CDialog)
	//{{AFX_MSG_MAP(CConfEdit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfEdit ���b�Z�[�W �n���h��

BOOL CConfEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
//	TRACE("edit init\n");

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	m_HatchSize = pView->HatchSize;
	m_UndoSize = pView->UndoSize;

	m_ArcInMove = pView->confEditArcinMove;
	m_ArcOutMove = pView->confEditArcoutMove;

	m_AlwaysAlign = pView->confEditAlwaysAlign;

	m_GridType = pView->confEditGridType;

	UpdateData(false);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CConfEdit::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	pView->HatchSize = m_HatchSize;
	pView->UndoSize = m_UndoSize;

	pView->confEditArcinMove = m_ArcInMove;
	pView->confEditArcoutMove = m_ArcOutMove;

	pView->confEditAlwaysAlign = m_AlwaysAlign;

	pView->confEditGridType = m_GridType;
}


//	EOF	//

