#if !defined(AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_)
#define AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMemoDlg �_�C�A���O

class CMemoDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CMemoDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMemoDlg)
	enum { IDD = IDD_MEMO_DLG };
	CString	m_Memo;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMemoDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MEMODLG_H__21B84AD5_C2B2_42BC_85B9_3BCE218A4167__INCLUDED_)
