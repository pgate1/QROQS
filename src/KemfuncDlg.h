#if !defined(AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_)
#define AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KemfuncDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CKemfuncDlg �_�C�A���O

#include"BSPetriTree.h"

class CKemfuncDlg : public CDialog
{

public:
	BSPetriTree *m_bspt;
	string m_mname;
	int m_id;



// �R���X�g���N�V����
public:
	CKemfuncDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CKemfuncDlg)
	enum { IDD = IDD_KEMFUNC };
	CSpinButtonCtrl	m_SpinBits;
	int		m_Getline;
	CString	m_compData;
	CString	m_kOutputData;
	CString	m_PassStatus;
	CString	m_PassStatusAll;
	CString	m_OutputName;
	BOOL	m_Common;
	int		m_Bits;
	BOOL	m_Hex;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CKemfuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CKemfuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRedraw();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_KEMFUNCDLG_H__BC4551D0_4716_4479_92B5_BCECB9F0E2BB__INCLUDED_)

/// EOF ///

