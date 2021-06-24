// QROQSDoc.h : CQROQSDoc �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_)
#define AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////  �ǉ��@�E�E�E�E�E�E�E�E�E
//#define GLOBAL_DOC_VALUE_DEFINE

#include"BSPetriTree.h"


class CQROQSDoc : public CDocument
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CQROQSDoc();
	DECLARE_DYNCREATE(CQROQSDoc)

// �A�g���r���[�g
public:


	BSPetriTree bsp_tree;

	UINT SimSpeed;

	CString filepath;//�y�g���l�b�g�t�@�C���i�U��p�X�j

#ifndef _LIGHT
	list<CString> verilog_out_dir_list, c_out_dir_list;
	int verilog_out_sync,verilog_debug;
	CString verilog_trafunc_option, verilog_module_option;
#endif


	int docst_;


// �I�y���[�V����
public:

	int SaveModifiedBSP();
	BOOL SimHalt();
	void InitTreeView();

//�I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CQROQSDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR);//�ǉ�
	virtual BOOL OnOpenDocument(LPCTSTR);//�ǉ�
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CQROQSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_QROQSDOC_H__89EA5475_348A_4040_A159_08650977D351__INCLUDED_)

/// EOF ///

