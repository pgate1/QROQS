#if !defined(AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_)
#define AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMemoDlg ダイアログ

class CMemoDlg : public CDialog
{
// コンストラクション
public:
	CMemoDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMemoDlg)
	enum { IDD = IDD_MEMO_DLG };
	CString	m_Memo;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMemoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_)
