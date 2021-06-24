// TraceDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "TraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg �_�C�A���O


CTraceDlg::CTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTraceDlg)
	m_Output = _T("");
	//}}AFX_DATA_INIT
}


void CTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraceDlg)
	DDX_Text(pDX, IDC_OUTPUTMSG, m_Output);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CTraceDlg)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_EN_SETFOCUS(IDC_OUTPUTMSG, OnSetfocusOutputmsg)
	ON_EN_KILLFOCUS(IDC_OUTPUTMSG, OnKillfocusOutputmsg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg ���b�Z�[�W �n���h��

void CTraceDlg::PostNcDestroy() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	delete this;

	CDialog::PostNcDestroy();
}

HBRUSH CTraceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	switch(nCtlColor){
	case CTLCOLOR_DLG:
		return (HBRUSH) m_brDlg;
	default:
		break;
	}

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;
}

BOOL CTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	
	m_brDlg.CreateSolidBrush(RGB(255,255,255));

	m_TraceEdit.SubclassDlgItem(IDC_OUTPUTMSG, this);

	m_LeaveFlag = TRUE;

	ModifyStyle( WS_THICKFRAME, 0 ); // �T�C�Y�ύX�g����
	ModifyStyle( WS_CAPTION, 0);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}



BOOL CTraceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	

    switch(pMsg->message){

	case EN_SETFOCUS:/*WM_MOUSEHOVER:*/
		// �E�B���h�E�Ƀ}�E�X�|�C���^������Ă��Ă��琔�b���o�߂��܂����B");
	//	SetFocus();
//		TRACE("mouse in\n");

	    break;
    case WM_MOUSELEAVE:
		//�E�B���h�E����}�E�X�|�C���^������܂���
	//	TRACE("mouse leave\n");
        m_LeaveFlag = TRUE;	//���̌��o�̂��߂̗\��
		break;
/*
	case WM_SETFOCUS:
		TRACE("dlg focus\n");
		break;
*/

		/*
	case EN_KILLFOCUS:
		TRACE("ENKILL\n");
		break;
		*/
    }

	return CDialog::PreTranslateMessage(pMsg);
}



void CTraceDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
/*
	TRACE("move\n");
	if(m_LeaveFlag){
		//m_stcMessage.SetWindowText("�E�B���h�E�Ƀ}�E�X�|�C���^������Ă��܂����B");
        m_LeaveFlag = FALSE;		
        // ����WM_MOUSELEAVE,WM_MOUSEHOVER�̌��o�\��
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 0;//�b��
        tme.hwndTrack = this->m_hWnd;
        TrackMouseEvent(&tme);
	}
*/

	CDialog::OnMouseMove(nFlags, point);
}


void CTraceDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
//	TRACE("kill focus\n");

/*
	GetSystemMenu(FALSE)->EnableMenuItem(SC_SIZE, MF_DISABLED);

	ModifyStyle(WS_SYSMENU, 0);
	ModifyStyle(WS_CAPTION, 0);
	ModifyStyle(WS_THICKFRAME, 0);

	SetWindowPos(&wndNoTopMost, 0,0,0,0, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE |
		SWP_FRAMECHANGED | SWP_SHOWWINDOW);
*/


}

void CTraceDlg::OnSetfocusOutputmsg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	/*
	ModifyStyle(0, WS_CAPTION);
	ModifyStyle(0, WS_THICKFRAME);
	ModifyStyle(0, WS_SYSMENU );
		
	GetSystemMenu(FALSE)->EnableMenuItem(SC_SIZE, MF_ENABLED);

	SetWindowPos(&wndNoTopMost, 0,0,0,0, 
		//SWP_NOACTIVATE | 
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOSIZE |
		SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_SHOWWINDOW );
*/
}

void CTraceDlg::OnKillfocusOutputmsg() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	

}

void CTraceDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
//	TRACE("gg\n");
}

/// EOF ///
