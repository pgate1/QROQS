#if !defined(AFX_UHZIFUNCDLG_H__564AE6DE_A70C_47C2_B530_B33FA3EE8A61__INCLUDED_)
#define AFX_UHZIFUNCDLG_H__564AE6DE_A70C_47C2_B530_B33FA3EE8A61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UhzifuncDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CUhzifuncDlg �_�C�A���O
#include"BSPetriTree.h"


class CUhzifuncDlg : public CDialog
{

public:
	BSPetriTree *m_bspt;
	string m_mname;
	int m_id;


// �R���X�g���N�V����
public:
	CUhzifuncDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CUhzifuncDlg)
	enum { IDD = IDD_UHZIFUNC };
	int		m_radio;
	int		m_line;
	int		m_In;
	CString	m_uInputData;
	CString	m_InputName;
	BOOL	m_Common;
	BOOL	m_Hex;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CUhzifuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CUhzifuncDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_UHZIFUNCDLG_H__564AE6DE_A70C_47C2_B530_B33FA3EE8A61__INCLUDED_)

/// EOF ///

