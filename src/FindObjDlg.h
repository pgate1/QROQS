#if !defined(AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_)
#define AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindObjDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg ダイアログ

#include"BSPetri.h"

class CFindObjDlg : public CDialog
{
// コンストラクション
public:
	CFindObjDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ


	BSPetri *m_bsp;
	int nSelID;

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnCancel;
	CButtonXP m_btnSearch;
#endif
	//{{AFX_DATA(CFindObjDlg)
	enum { IDD = IDD_FIND_OBJ };
	CListCtrl	m_ObjList;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CFindObjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CFindObjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	virtual void OnCancel();
	afx_msg void OnItemchangedObjList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_)
