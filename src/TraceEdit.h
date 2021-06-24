#if !defined(AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_)
#define AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTraceEdit ウィンドウ

class CTraceEdit : public CEdit
{
// コンストラクション
public:
	CTraceEdit();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTraceEdit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTraceEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTraceEdit)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_)
