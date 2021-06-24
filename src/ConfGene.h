#if !defined(AFX_CONFGENE_H__73DE0EED_C0A5_4346_9316_C4728B4EBD86__INCLUDED_)
#define AFX_CONFGENE_H__73DE0EED_C0A5_4346_9316_C4728B4EBD86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfGene.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CConfGene ダイアログ

#include"QROQSDoc.h"


class CConfGene : public CDialog
{
// コンストラクション
public:
	CConfGene(CWnd* pParent = NULL);   // 標準のコンストラクタ


	CQROQSDoc *m_pDoc;

	int nSel,selobj;
	map<int,COLORREF> vcolmap;
	int update;


// ダイアログ データ
	//{{AFX_DATA(CConfGene)
	enum { IDD = IDD_CONF_GENE };
	CSliderCtrl	m_SliderG;
	CSliderCtrl	m_SliderB;
	CSliderCtrl	m_SliderR;
	CListBox	m_ObjList;
	int		m_Qmode;
	BOOL	m_SkinUse;
	BOOL	m_PrintColor;
	float	m_DiamDelta;
	BOOL	m_Selected;
	int		m_ColR;
	int		m_ColB;
	int		m_ColG;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConfGene)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	void SliderSet();
	void viewRedraw();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConfGene)
	afx_msg void OnComConfFont();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeObjList();
	afx_msg void OnCheckSelected();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditR();
	afx_msg void OnChangeEditG();
	afx_msg void OnChangeEditB();
	afx_msg void OnPaint();
	afx_msg void OnColorPicker();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONFGENE_H__73DE0EED_C0A5_4346_9316_C4728B4EBD86__INCLUDED_)

