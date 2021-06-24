// QROQSDoc.h : CQROQSDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_)
#define AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////  追加　・・・・・・・・・
//#define GLOBAL_DOC_VALUE_DEFINE

#include"BSPetriTree.h"


class CQROQSDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CQROQSDoc();
	DECLARE_DYNCREATE(CQROQSDoc)

// アトリビュート
public:


	BSPetriTree bsp_tree;

	UINT SimSpeed;

	CString filepath;//ペトリネットファイル（振るパス）

#ifndef _LIGHT
	list<CString> verilog_out_dir_list, c_out_dir_list;
	int verilog_out_sync,verilog_debug;
	CString verilog_trafunc_option, verilog_module_option;
#endif


	int docst_;


// オペレーション
public:

	int SaveModifiedBSP();
	BOOL SimHalt();
	void InitTreeView();

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CQROQSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR);//追加
	virtual BOOL OnOpenDocument(LPCTSTR);//追加
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CQROQSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CQROQSDoc)
	afx_msg void OnFileOpen();
	afx_msg int OnFileSave();
	afx_msg int OnFileSaveAs();
	afx_msg void OnFileSaveAll();
	afx_msg void OnUpdateFileSaveAll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_)

/// EOF ///

