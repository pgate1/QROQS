#if !defined(AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_)
#define AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfCompile.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CConfCompile �_�C�A���O

#include"QROQSDoc.h"


class CConfCompile : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// �R���X�g���N�V����
public:
	CConfCompile(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CConfCompile)
	enum { IDD = IDD_CONF_COMPILE };
	BOOL	m_CompSave;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CConfCompile)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CConfCompile)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CONFCOMPILE_H__2204B907_4FFF_4ACC_B510_D2E57212C5FC__INCLUDED_)

