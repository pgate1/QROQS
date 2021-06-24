#if !defined(AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_)
#define AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindObjDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg �_�C�A���O

#include"BSPetri.h"

class CFindObjDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CFindObjDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^


	BSPetri *m_bsp;
	int nSelID;

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnCancel;
	CButtonXP m_btnSearch;
#endif
	//{{AFX_DATA(CFindObjDlg)
	enum { IDD = IDD_FIND_OBJ };
	CListCtrl	m_ObjList;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CFindObjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CFindObjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	virtual void OnCancel();
	afx_msg void OnItemchangedObjList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_FINDOBJDLG_H__702EB2B0_836F_4811_9396_0C05C40561E6__INCLUDED_)
