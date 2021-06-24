#if !defined(AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_)
#define AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlacefuncDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPlacefuncDlg ダイアログ

class CPlacefuncDlg : public CDialog
{
// コンストラクション
public:
	CPlacefuncDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPlacefuncDlg)
	enum { IDD = IDD_PLACEFUNC };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPlacefuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPlacefuncDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_)
