#if !defined(AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_)
#define AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TokenfuncDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTokenfuncDlg �_�C�A���O

class CTokenfuncDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CTokenfuncDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CTokenfuncDlg)
	enum { IDD = IDD_TOKENFUNC };
	CString	m_str;
	CString	m_input;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTokenfuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTokenfuncDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TOKENFUNCDLG_H__99F4202B_90BD_43FC_9EF7_FFB402565EBF__INCLUDED_)

/// EOF ///

