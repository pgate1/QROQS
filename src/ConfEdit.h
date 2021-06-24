#if !defined(AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_)
#define AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfEdit.h : �w�b�_�[ �t�@�C��
//

#include"QROQSDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CConfEdit �_�C�A���O

class CConfEdit : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// �R���X�g���N�V����
public:
	CConfEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CConfEdit)
	enum { IDD = IDD_CONF_EDIT };
	UINT	m_HatchSize;
	int		m_UndoSize;
	BOOL	m_ArcInMove;
	BOOL	m_ArcOutMove;
	BOOL	m_AlwaysAlign;
	int		m_GridType;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CConfEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CConfEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CONFEDIT_H__03369A3E_44B6_42E5_8BA0_6DC68E817114__INCLUDED_)

