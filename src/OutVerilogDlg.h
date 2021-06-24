#if !defined(AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_)
#define AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutVerilogDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// COutVerilogDlg �_�C�A���O


#include"QROQSDoc.h"

class COutVerilogDlg : public CDialog
{
// �R���X�g���N�V����
public:
	COutVerilogDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CQROQSDoc *m_pDoc;

// �_�C�A���O �f�[�^
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
	//{{AFX_DATA(COutVerilogDlg)
	enum { IDD = IDD_OUT_VERILOG };
	CComboBox	m_OutDir;
	CString	m_ModuleName;
	double	m_OverPlus;
	BOOL	m_VSync;
	BOOL	m_VDebug;
	CString	m_OptModule;
	CString	m_OptTrafunc;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COutVerilogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(COutVerilogDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OUTVERILOGDLG_H__94C32924_CB5B_4E4D_897D_6B12C60B8900__INCLUDED_)
