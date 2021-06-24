// TraceDlg.cpp : インプリメンテーション ファイル
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
// CTraceDlg ダイアログ


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
// CTraceDlg メッセージ ハンドラ

void CTraceDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
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

	// TODO: この位置で DC のアトリビュートを変更してください
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

BOOL CTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	m_brDlg.CreateSolidBrush(RGB(255,255,255));

	m_TraceEdit.SubclassDlgItem(IDC_OUTPUTMSG, this);

	m_LeaveFlag = TRUE;

	ModifyStyle( WS_THICKFRAME, 0 ); // サイズ変更枠除去
	ModifyStyle( WS_CAPTION, 0);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



BOOL CTraceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	

    switch(pMsg->message){

	case EN_SETFOCUS:/*WM_MOUSEHOVER:*/
		// ウィンドウにマウスポインタがやってきてから数秒が経過しました。");
	//	SetFocus();
//		TRACE("mouse in\n");

	    break;
    case WM_MOUSELEAVE:
		//ウィンドウからマウスポインタが離れました
	//	TRACE("mouse leave\n");
        m_LeaveFlag = TRUE;	//次の検出のための予約
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
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
/*
	TRACE("move\n");
	if(m_LeaveFlag){
		//m_stcMessage.SetWindowText("ウィンドウにマウスポインタがやってきました。");
        m_LeaveFlag = FALSE;		
        // 次のWM_MOUSELEAVE,WM_MOUSEHOVERの検出予約
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 0;//秒数
        tme.hwndTrack = this->m_hWnd;
        TrackMouseEvent(&tme);
	}
*/

	CDialog::OnMouseMove(nFlags, point);
}


void CTraceDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

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
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	

}

void CTraceDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
//	TRACE("gg\n");
}

/// EOF ///
