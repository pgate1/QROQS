#if !defined(AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_)
#define AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleManagerDlg.h : �w�b�_�[ �t�@�C��
//

#include "BSPetriTree.h"
/////////////////////////////////////////////////////////////////////////////
// CModuleManagerDlg �_�C�A���O

class CModuleManagerDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CModuleManagerDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^


	BSPetriTree *m_bsp_tree;

	CString m_RootName;
	map<CString,int> m_Maxtime;

// �_�C�A���O �f�[�^
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


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CModuleManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MODULEMANAGERDLG_H__1819D172_EC43_4E28_BCCC_0A776F4D6BEB__INCLUDED_)
