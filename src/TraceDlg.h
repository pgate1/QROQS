#if !defined(AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_)
#define AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceDlg.h : ヘッダー ファイル
//

#include "TraceEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg ダイアログ

class CTraceDlg : public CDialog
{
// コンストラクション
public:
	CTraceDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CBrush m_brDlg;

	CTraceEdit m_TraceEdit;
	BOOL m_LeaveFlag;

// ダイアログ データ
	//{{AFX_DATA(CTraceDlg)
	enum { IDD = IDD_TRACEOUT };
	CString	m_Output;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTraceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTraceDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetfocusOutputmsg();
	afx_msg void OnKillfocusOutputmsg();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_)
