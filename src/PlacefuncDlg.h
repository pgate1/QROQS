#if !defined(AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_)
#define AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlacefuncDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPlacefuncDlg �_�C�A���O

class CPlacefuncDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CPlacefuncDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPlacefuncDlg)
	enum { IDD = IDD_PLACEFUNC };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPlacefuncDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPlacefuncDlg)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PLACEFUNCDLG_H__92329D43_76A2_40A5_A652_EE4A6CA92686__INCLUDED_)
