// TraceEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "TraceEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraceEdit

CTraceEdit::CTraceEdit()
{
}

CTraceEdit::~CTraceEdit()
{
}


BEGIN_MESSAGE_MAP(CTraceEdit, CEdit)
	//{{AFX_MSG_MAP(CTraceEdit)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceEdit ���b�Z�[�W �n���h��

BOOL CTraceEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	HCURSOR cursor=NULL;
	cursor = AfxGetApp()->LoadCursor(IDC_CUR_HAND_HOLD);
			/*
	if(now_command==_HAND){
		if(HandlingTrack==true){

		}
		else if(PutIt!=0){
			cursor = AfxGetApp()->LoadCursor(IDC_CUR_HAND);
		}
	}*/
/*
	if(cursor!=NULL){
		SetCursor(cursor);
		return 0;
	}
*/
	return CEdit::OnSetCursor(pWnd, nHitTest, message);
}

void CTraceEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

//	OnActivate(WA_ACTIVE, NULL, 0);
//	GetParent()->SetFocus();

	CEdit::OnLButtonDown(nFlags, point);
}
