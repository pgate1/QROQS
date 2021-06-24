#if !defined(AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_)
#define AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfSim.h : �w�b�_�[ �t�@�C��
//

#include"QROQSDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CConfSim �_�C�A���O

class CConfSim : public CDialog
{

public:
	CQROQSDoc *m_pDoc;

// �R���X�g���N�V����
public:
	CConfSim(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CConfSim)
	enum { IDD = IDD_CONF_SIM };
	UINT	m_SimSpeed;
	BOOL	m_TokenMove;
	BOOL	m_SimAnime;
	int		m_SimundoSize;
	int		m_ViewDataSize;
	BOOL	m_BinValue;
	BOOL	m_TokenTrans;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CConfSim)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CConfSim)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnFastsim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CONFSIM_H__F74E6AA1_7A57_4342_80F8_ED4BA0F26F05__INCLUDED_)

