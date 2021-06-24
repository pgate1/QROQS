#if !defined(AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_)
#define AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutCDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// COutCDlg �_�C�A���O


#include"QROQSDoc.h"

class COutCDlg : public CDialog
{
// �R���X�g���N�V����
public:
	COutCDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CQROQSDoc *m_pDoc;

// �_�C�A���O �f�[�^
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;

	//{{AFX_DATA(COutCDlg)
	enum { IDD = IDD_OUT_C };
	CComboBox	m_OutDir;
	CString	m_ModuleName;
	int		m_KemMax;
	BOOL	m_MainOut;
	BOOL	m_QROQSOut;
	BOOL	m_MakeOut;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COutCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(COutCDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OUTCDLG_H__A03914B7_D9D3_4B7E_84A1_23E919493C02__INCLUDED_)
