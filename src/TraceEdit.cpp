// TraceEdit.cpp : インプリメンテーション ファイル
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
// CTraceEdit メッセージ ハンドラ

BOOL CTraceEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
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
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

//	OnActivate(WA_ACTIVE, NULL, 0);
//	GetParent()->SetFocus();

	CEdit::OnLButtonDown(nFlags, point);
}
