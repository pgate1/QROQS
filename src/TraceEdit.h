#if !defined(AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_)
#define AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTraceEdit �E�B���h�E

class CTraceEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CTraceEdit();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTraceEdit)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CTraceEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CTraceEdit)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TRACEEDIT_H__68FC6088_1F00_49CE_89C9_9A4151E56796__INCLUDED_)
