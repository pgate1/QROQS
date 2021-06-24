#if !defined(AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_)
#define AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : �w�b�_�[ �t�@�C��
//

#include"QROQSDoc.h"

#include"ConfGene.h"
#include"ConfEdit.h"
#include"ConfCompile.h"
#include"ConfSim.h"


/////////////////////////////////////////////////////////////////////////////
// CConfigDlg �_�C�A���O

class CConfigDlg : public CDialog
{

	CConfGene m_ConfGene;
	CConfEdit m_ConfEdit;
	CConfCompile m_ConfCompile;
	CConfSim m_ConfSim;

public:
	//�ǉ�
	CQROQSDoc *m_pDoc;

// �R���X�g���N�V����
public:
	CConfigDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
	CButtonXP m_btnCancel;
#endif
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIGDLG };
	CTabCtrl	m_Tab;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	//�ǉ�
	int CheckDlgData();
	void showTab(int);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CONFIGDLG_H__BA727B12_6AAB_4199_AE28_5C0EE0CB62F7__INCLUDED_)


/// EOF ///

