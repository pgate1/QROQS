#if !defined(AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_)
#define AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleManagerDlg.h : ヘッダー ファイル
//

#include "BSPetriTree.h"
/////////////////////////////////////////////////////////////////////////////
// CModuleManagerDlg ダイアログ

class CModuleManagerDlg : public CDialog
{
// コンストラクション
public:
	CModuleManagerDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ


	BSPetriTree *m_bsp_tree;

	CString m_RootName;
	map<CString,int> m_Maxtime;

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
	CButtonXP m_btnSearch;
#endif
	//{{AFX_DATA(CModuleManagerDlg)
	enum { IDD = IDD_MOD_MANAGE };
	CListCtrl	m_ModList;
	BOOL	m_Root;
	int		m_SimTime;
	CString	m_SelMod;
	//}}AFX_DATA


	void InitItems();


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CModuleManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CModuleManagerDlg)
	afx_msg void OnSearch();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedModList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickModList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRoot();
	afx_msg void OnChangeSimtime();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_)
