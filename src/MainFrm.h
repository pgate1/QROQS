// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9B0737A2_600F_4B93_8763_206FD3DA6059__INCLUDED_)
#define AFX_MAINFRM_H__9B0737A2_600F_4B93_8763_206FD3DA6059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////�ǉ�
#include"stdafx.h"

#include"ModuleTreeDlgBar.h"

#include"MyTreeCtrl.h"


#ifdef _LIGHT
#define CNewFrameWnd CFrameWnd
#else
#define ID_MODULE_UP_SELECT DROPDOWN(ID_MODULE_UP)
#endif

class CMainFrame : public CNewFrameWnd
{
	
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)


// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o

#ifdef _LIGHT
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndObjectToolBar;
#else
	CStatusBarXP  m_wndStatusBar;

	CToolBarXP    m_wndToolBar;
	CToolBarXP    m_wndObjectToolBar;

	CMyTreeCtrl	 m_treePopup;
#endif

//////�ǉ�
	CModuleTreeDlgBar m_wndModuleTreeDlgBar;

	int ModTreeShow;

	CProgressCtrl m_cpc;

public:
	CProgressCtrl* GetProgress()
	{
		return &m_cpc;
	}
    CToolBar* GetObjToolBar()
	{
		return &m_wndObjectToolBar;
	}
    CModuleTreeDlgBar* GetModuleTreeDlgBar()
	{
		return &m_wndModuleTreeDlgBar;
	}


	void SaveWindowPlacement(LPWINDOWPLACEMENT);
	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void InitShowWindow();

	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);

	void FitDockBars();

#ifndef _LIGHT
	void SetStyleMode(UINT style);
#endif

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnViewModtree();
	afx_msg void OnUpdateViewModtree(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

#ifndef _LIGHT
	afx_msg void OnStyleIcy();
	afx_msg void OnUpdateStyleIcy(CCmdUI* pCmdUI);
	afx_msg void OnStyleXp();
	afx_msg void OnUpdateStyleXp(CCmdUI* pCmdUI);
	afx_msg void OnStyleXp2003();
	afx_msg void OnUpdateStyleXp2003(CCmdUI* pCmdUI);
	afx_msg void OnStyleOriginal();
	afx_msg void OnUpdateStyleOriginal(CCmdUI* pCmdUI);
#endif

#ifndef _LIGHT
	void OnModuleUpSelect();
#endif
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__9B0737A2_600F_4B93_8763_206FD3DA6059__INCLUDED_)

/// EOF ///

