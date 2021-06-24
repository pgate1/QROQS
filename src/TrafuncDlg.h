#if !defined(AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_)
#define AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrafuncDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTrafuncDlg �_�C�A���O
#include"BSPetriTree.h"

class CTrafuncDlg : public CDialog
{

public:
	BSPetriTree *m_bspt;
	string m_mname;
	int m_id;

	BSPetri *m_bsp;//Sim�̎��̂�


// �R���X�g���N�V����
public:
	CTrafuncDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^



// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
	CButtonXP m_btnTrans;
	CButtonXP m_btnCheck;
#endif
	//{{AFX_DATA(CTrafuncDlg)
	enum { IDD = IDD_TRAFUNC };
	CString	m_Trafunc;
	CString	m_AbletermAcnt;
	BOOL	m_FireableStop;
	BOOL	m_Common;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTrafuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTrafuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TRAFUNCDLG_H__6912125B_8E74_486B_AD68_79D1D8B86A63__INCLUDED_)

/// EOF ///

