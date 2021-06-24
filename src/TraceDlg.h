#if !defined(AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_)
#define AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceDlg.h : �w�b�_�[ �t�@�C��
//

#include "TraceEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTraceDlg �_�C�A���O

class CTraceDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CTraceDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CBrush m_brDlg;

	CTraceEdit m_TraceEdit;
	BOOL m_LeaveFlag;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTraceDlg)
	enum { IDD = IDD_TRACEOUT };
	CString	m_Output;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTraceDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTraceDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetfocusOutputmsg();
	afx_msg void OnKillfocusOutputmsg();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TRACEDLG_H__947049E7_38AC_4E91_8292_29F4F3E3DC3D__INCLUDED_)
