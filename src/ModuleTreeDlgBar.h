#if !defined(AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_)
#define AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleTreeDlgBar.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CModuleTreeDlgbar �_�C�A���O

class CModuleTreeDlgBar : public CDialogBar
{
// �R���X�g���N�V����
public:
	CModuleTreeDlgBar(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	void SelectModule(const char *mname);
	void SelectRootModule();
	BOOL IsNamed(HTREEITEM hitem);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CModuleTreeDlgBar)
	enum { IDD = IDD_MODTREE };
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CModuleTreeDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CModuleTreeDlgBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedModtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickModtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MODULETREEDLGBAR_H__C02E2714_30A8_4C64_A82A_21CC405B2AD7__INCLUDED_)
