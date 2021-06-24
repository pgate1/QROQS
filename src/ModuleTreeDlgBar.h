#if !defined(AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_)
#define AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleTreeDlgBar.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CModuleTreeDlgbar ダイアログ

class CModuleTreeDlgBar : public CDialogBar
{
// コンストラクション
public:
	CModuleTreeDlgBar(CWnd* pParent = NULL);   // 標準のコンストラクタ

	void SelectModule(const char *mname);
	void SelectRootModule();
	BOOL IsNamed(HTREEITEM hitem);

// ダイアログ データ
	//{{AFX_DATA(CModuleTreeDlgBar)
	enum { IDD = IDD_MODTREE };
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CModuleTreeDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CModuleTreeDlgBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedModtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickModtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_)
