// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

///�ǉ�
#define GLOBAL_MAINFRM_DEFINE

#include "stdafx.h"
#include "QROQS.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CNewFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CNewFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_MODTREE, OnViewModtree)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MODTREE, OnUpdateViewModtree)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_INITMENUPOPUP()
	ON_WM_INITMENU()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP

#ifndef _LIGHT
	ON_COMMAND(ID_STYLE_ICY, OnStyleIcy)
	ON_UPDATE_COMMAND_UI(ID_STYLE_ICY, OnUpdateStyleIcy)
	ON_COMMAND(ID_STYLE_XP, OnStyleXp)
	ON_UPDATE_COMMAND_UI(ID_STYLE_XP, OnUpdateStyleXp)
	ON_COMMAND(ID_STYLE_XP2003, OnStyleXp2003)
	ON_UPDATE_COMMAND_UI(ID_STYLE_XP2003, OnUpdateStyleXp2003)
	ON_COMMAND(ID_STYLE_ORIGINAL, OnStyleOriginal)
	ON_UPDATE_COMMAND_UI(ID_STYLE_ORIGINAL, OnUpdateStyleOriginal)
#endif

#ifndef _LIGHT
	ON_COMMAND(ID_MODULE_UP_SELECT, OnModuleUpSelect)
#endif


END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_PROG,
	ID_INDICATOR_RUNS,
	ID_INDICATOR_POSX,
	ID_INDICATOR_POSY,
};




/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����

CMainFrame::CMainFrame()
{
	// TODO: ���̈ʒu�Ƀ����o�̏����������R�[�h��ǉ����Ă��������B
	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	ModTreeShow = pApp->GetProfileInt("MainFrame","ModuleTreeShow",0);
}

CMainFrame::~CMainFrame()
{
//	TRACE("mainframe deth\n");

	CWinApp* pApp=AfxGetApp();
	pApp->WriteProfileInt("MainFrame","ModuleTreeShow",ModTreeShow);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CNewFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

//	fprintf(tfp,"CMainFrame::OnCreate\n");
//	TRACE("CMainFrame::OnCreate\n");


	
	//�X�e�[�^�X�o�[
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}


	//�f�t�H���g�̃c�[���o�[
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}
	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�͈ȉ��̂R�s���폜
	//       ���Ă��������B
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);



	//�I�u�W�F�N�g�c�[���o�[
	if (!m_wndObjectToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		 | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndObjectToolBar.LoadToolBar(IDR_OBJECT_MINI))
	{
		//AfxMessageBox("m_wndObjectToolBar�̍쐬�Ɏ��s���܂���");
		TRACE0("Failed to create toolbar\n");
		return -1;      // �쐬�Ɏ��s
	}
	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�͈ȉ��̂R�s���폜
	//       ���Ă��������B
	m_wndObjectToolBar.EnableDocking(CBRS_ALIGN_ANY);

#ifndef _LIGHT
	//�h���b�v�_�E���X�^�C���̐ݒ�
	m_wndObjectToolBar.SetButtonStyle (
		m_wndObjectToolBar.CommandToIndex (ID_MODULE_UP), 
		m_wndObjectToolBar.GetButtonStyle(m_wndObjectToolBar.CommandToIndex (ID_MODULE_UP))
		| TBSTYLE_DROPDOWN);

    // Initialization of the TreeCtrl
    m_treePopup.Create (WS_CHILD|TVS_HASLINES|TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_DISABLEDRAGDROP, CRect (0, 0, 144, 159), this, 1);
#endif

	// ���W���[���c���[�_�C�A���O�o�[�̐���
	if (!m_wndModuleTreeDlgBar.Create(this, IDD_MODTREE,  WS_VISIBLE | CBRS_TOP
		 |CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | WS_THICKFRAME|CBRS_BORDER_3D|CBRS_BORDER_RIGHT,IDC_MODTREE) )
	{
		TRACE0("Failed to create Module Tree bar\n");
		return -1;      // �쐬�Ɏ��s
	}
	m_wndModuleTreeDlgBar.EnableDocking(CBRS_ALIGN_LEFT);


	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar);
	DockControlBarLeftOf(&m_wndObjectToolBar,&m_wndToolBar);
	DockControlBar(&m_wndModuleTreeDlgBar,AFX_IDW_DOCKBAR_LEFT);



//	LoadBarState("BarState");

	if(ModTreeShow==0){
		ShowControlBar(&m_wndModuleTreeDlgBar, false, TRUE);
	}

#ifndef _LIGHT
	m_DefaultNewMenu.LoadToolBar(IDR_MAINFRAME);
	m_DefaultNewMenu.LoadToolBar(IDR_OBJECT_MINI);
	m_DefaultNewMenu.LoadToolBar(ToolId,RGB(192,192,192));
#endif


	// ���엦�y�C��
	int nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_RUNS);
	m_wndStatusBar.SetPaneInfo(nIndex, ID_INDICATOR_RUNS,
		m_wndStatusBar.GetPaneStyle(nIndex), 60);


	RECT rect;
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROG);
	m_wndStatusBar.SetPaneInfo(nIndex, ID_INDICATOR_PROG,
		m_wndStatusBar.GetPaneStyle(nIndex), 100);
	m_wndStatusBar.GetItemRect(nIndex,&rect);//ID_��}���������Έʒu���w�肵�܂��B
	if(!::IsWindow(m_cpc.m_hWnd))
	{
		m_cpc.Create(WS_VISIBLE|WS_CHILD,rect,&m_wndStatusBar,1); 
		//�v���O���X�o�[���쐬���܂��B
		m_cpc.SetRange(0,100);//�͈͂��w�肵�܂��B
		m_cpc.SetStep(1);//�����l���w�肵�܂��B
	}

	InitShowWindow();


	return 0;
}

void CMainFrame::InitShowWindow()
{
	WINDOWPLACEMENT wp;
	if(LoadWindowPlacement(&wp)){
//		ShowWindow(SW_MINIMIZE);
	   SetWindowPlacement(&wp);
	 //���E�C���h�E�̈ʒu��ݒ肷��MFC�֐�
	}
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CNewFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C�����āAWindow �N���X��X�^�C����
	//       �C�����Ă��������B


//	cs.x = -100;
//	cs.y = -100;
//	cs.cx = 10;
//	cs.cy = 10;
	cs.cx = 700;
	cs.cy = 600;


	// �^�C�g����ύX���Ȃ�
//	cs.style &= ~FWS_ADDTOTITLE;
	// �A�v���P�[�V�����^�C�g����擪�ɂ���ݒ�
//	cs.style &= ~FWS_PREFIXTITLE;

	cs.style &= ~WS_VISIBLE;

    
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CNewFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CNewFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W �n���h��

#include "QROQSView.h"

#include "other.h"

void CMainFrame::OnViewModtree() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	ShowControlBar(&m_wndModuleTreeDlgBar
		, !m_wndModuleTreeDlgBar.IsWindowVisible(), TRUE);
	ModTreeShow = !m_wndModuleTreeDlgBar.IsWindowVisible();
}

void CMainFrame::OnUpdateViewModtree(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(m_wndModuleTreeDlgBar.IsWindowVisible());
}

void CMainFrame::OnClose()
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������

//	SaveBarState("BarState");
//	TRACE("close\n");
//	return;
	

	CQROQSDoc *pDoc = (CQROQSDoc*)GetActiveDocument();
	if(!pDoc->SimHalt()) return;

	CQROQSView* pView = (CQROQSView*) GetActiveView();
	if(pView->check_thread){
		SetTimer(_TID_PETSIM, 10, NULL);
		return;
	}

	CNewFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	switch(nIDEvent){
	case 1:
		CQROQSView* pView = (CQROQSView*) GetActiveView();
		if(!pView->check_thread){
			KillTimer(1);
			PostMessage(WM_CLOSE);
		}
		break;
	}

	CNewFrameWnd::OnTimer(nIDEvent);
}

BOOL CMainFrame::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
    CString strText=AfxGetApp()->GetProfileString("Option","Position","");
    if(strText.IsEmpty()) return FALSE;

    int cRead=_stscanf(strText,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
        &pwp->flags,
        &pwp->showCmd,
        &pwp->ptMinPosition.x,
        &pwp->ptMinPosition.y,
        &pwp->ptMaxPosition.x,
        &pwp->ptMaxPosition.y,
        &pwp->rcNormalPosition.left,
        &pwp->rcNormalPosition.top,
        &pwp->rcNormalPosition.right,
        &pwp->rcNormalPosition.bottom);
        //��i�͂W�i�@
    if(cRead !=10) return FALSE;

    return TRUE;
}

//�������݊֐� 
void CMainFrame::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
    CString strText;
    strText.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
	    pwp->flags,
	    pwp->showCmd,
	    pwp->ptMinPosition.x,
	    pwp->ptMinPosition.y,
	    pwp->ptMaxPosition.x,
	    pwp->ptMaxPosition.y,
	    pwp->rcNormalPosition.left,
	    pwp->rcNormalPosition.top,
	    pwp->rcNormalPosition.right,
	    pwp->rcNormalPosition.bottom);

    AfxGetApp()->WriteProfileString("Option","Position",strText);
}

void CMainFrame::OnDestroy() 
{

//	TRACE("main dead\n");

	WINDOWPLACEMENT wp;
	if(GetWindowPlacement(&wp)){
	//���E�C���h�E�̈ʒu���擾����MFC�֐�
	  SaveWindowPlacement(&wp);
	}

	CNewFrameWnd::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

}



void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CNewFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
//	TRACE("CMainFrame::OnSize\n");

//	FitDockBars();

	// �v���O���X�o�[
	RECT rect;
	if(::IsWindow(m_wndStatusBar.m_hWnd)
	&& ::IsWindow(m_cpc.m_hWnd))
	{//�ǂ�����쐬�ς݂̎��ɏ������܂��B
		m_wndStatusBar.GetItemRect(m_wndStatusBar.CommandToIndex(ID_INDICATOR_PROG),&rect);//ID_��}���������Έʒu���w�肵�܂��B
		m_cpc.SetWindowPos(&wndTop,rect.left,rect.top,
			rect.right-rect.left,rect.bottom-rect.top,0);
		//�v���O���X�o�[�̈ʒu�A�傫�����w�肵�܂��B
	}



}
void CMainFrame::FitDockBars()
{

	return;

	CRect rect;
	CWnd *dWnd;

	//���W���[���c���[�E�C���h�E
	GetClientRect(&rect);     // �^�u�R���g���[���̃E�B���h�E�̈�i�X�N���[�����W�n�j
	if(rect.right<=0 || rect.bottom<=0) return;
//	TRACE("(%d,%d)(%d,%d)\n",rect.left,rect.top,rect.right,rect.bottom);

	rect.top = 0;
	rect.left = 0;
	rect.right = 100;
	rect.bottom -= 100;

	m_wndModuleTreeDlgBar.MoveWindow(rect);
	dWnd = m_wndModuleTreeDlgBar.GetDlgItem(IDC_MODTREE);
	if(dWnd!=NULL){
		rect.top	= 10;
		rect.left	= 2;
		rect.right	-= 4;
		rect.bottom -= 4;
		dWnd->MoveWindow(rect);
	}

}

#ifndef _LIGHT
void CMainFrame::OnModuleUpSelect()
{

	//�f�[�^�̒ǉ�
	m_treePopup.DeleteAllItems();

	struct func{
		static int set_tree(
			CTreeCtrl *pPopTree,
			HTREEITEM pop_cur_item,
			CTreeCtrl *pModTree,
			HTREEITEM mod_cur_item,
			HTREEITEM current_item)
		{
			int ret=0;
			HTREEITEM mod_citem,pop_citem;
			mod_citem = pModTree->GetChildItem(mod_cur_item);
			for(;mod_citem!=NULL;){
				pop_citem = pPopTree->InsertItem(pModTree->GetItemText(mod_citem), pop_cur_item);
				pPopTree->SetItemData(pop_citem, (DWORD)mod_citem);
				ret += set_tree(pPopTree, pop_citem, pModTree, mod_citem, current_item);
				mod_citem = pModTree->GetNextSiblingItem(mod_citem);
			}

			if(mod_cur_item==current_item){
				pPopTree->SelectItem(pop_cur_item);
				pPopTree->Expand (pop_cur_item, TVE_EXPAND);
			}
			if(ret){
				pPopTree->Expand (pop_cur_item, TVE_EXPAND);
				return 1;
			}
			return 0;
		}
	};

	CTreeCtrl *pModTree = GetModuleTree();
	CQROQSView* pView = (CQROQSView*) GetActiveView();

    HTREEITEM hRoot = m_treePopup.InsertItem (pModTree->GetItemText( pModTree->GetRootItem() ));
	m_treePopup.SetItemData( hRoot, (DWORD)pModTree->GetRootItem() );
	func::set_tree(&m_treePopup, hRoot, pModTree, pModTree->GetRootItem(), pView->current_item);


    CRect rcCaller;

    // Get rect of toolbar item
    m_wndObjectToolBar.GetItemRect (m_wndObjectToolBar.CommandToIndex (ID_MODULE_UP), rcCaller);
    m_wndObjectToolBar.ClientToScreen (rcCaller);
    rcCaller.top++;
    rcCaller.bottom--;

    CPopup* pPopup = (CPopup*)RUNTIME_CLASS(CPopup)->CreateObject();

    // Popup the TreeCtrl
    VERIFY(pPopup->Display (&m_treePopup, this, rcCaller, &m_treePopup.m_xPopupCtrlEvent, WS_TABBORDER));

}

#endif // non _LIGHT


void CMainFrame::OnMove(int x, int y) 
{
	CNewFrameWnd::OnMove(x, y);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	CQROQSView *pView = (CQROQSView*)GetActiveView();
	if(pView==NULL) return;

	// �����g���[�X�_�C�A���O��A�����Ĉړ�������
	pView->PostMessage(WM_SIZE);

}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{

	CNewFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
 
}

void CMainFrame::OnInitMenu(CMenu* pMenu) 
{

	CMenu *pSub =pMenu->GetSubMenu(5);	//�u�ݒ�v���j���[���擾

	if(!pSub){
		CNewFrameWnd::OnInitMenu(pMenu);
		return;
	}

	CString cs;
	if(!pSub->GetMenuString(ID_COM_CONFIG,cs,MF_BYCOMMAND)){
		CNewFrameWnd::OnInitMenu(pMenu);
		return;
	}

	CMenu *pSkinMenu=pSub ->GetSubMenu(1);	//�uskin�v���j���[���擾

	int i;
	//�ȑO�̃��j���[���폜
	UINT mx=pSkinMenu->GetMenuItemCount();
	for(i=0;i<mx;i++){
//		CString str;pTest->GetMenuString(0,str,MF_BYPOSITION);TRACE("remove[%d]%s\n",i,str);
		pSkinMenu->RemoveMenu(0,MF_BYPOSITION);
	}

	//���j���[��ǉ�

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SetCurrentDirectory(pApp->exe_dir);

	CFileFind FileFind;
    // skin�t�H���_�̂��ׂẴt�@�C��������
    CString strSearchFile = pApp->exe_dir + _T("skin\\*");
//	TRACE("%s\n",strSearchFile);
    
    if(!FileFind.FindFile(strSearchFile)) 
        return;

	// ���ݑI������Ă���skin�Ƀ`�F�b�N�����邽��
	CQROQSView *pView = (CQROQSView*)GetActiveView();
	if(pView==NULL) return;
   
	i=0;
    BOOL bContinue = TRUE;
    while(bContinue){ 
        
        bContinue = FileFind.FindNextFile();
        
        // �h�b�g("." , "..")�̏ꍇ ����
        if(FileFind.IsDots())
            continue;
        
        // �f�B���N�g���̏ꍇ�A
        if(FileFind.IsDirectory()){
			CString fPath = (LPCTSTR)FileFind.GetFilePath() + FileFind.GetFilePath().ReverseFind('\\')+1;
			pSkinMenu->AppendMenu(MF_STRING,IDM_SKINS+i, fPath);
			if(fPath == pView->SkinDir){
				pSkinMenu->CheckMenuItem(i,MF_BYPOSITION |MF_CHECKED);
			}
			i++;
		}
        else{    // �t�@�C���̏ꍇ
//            TRACE("%s\n",FileFind.GetFilePath()); 
		}
    }

	DrawMenuBar();

	CNewFrameWnd::OnInitMenu(pMenu);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
}

#ifndef _LIGHT

void CMainFrame::SetStyleMode(UINT style)
{
	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	pApp->style_mode = style;

//	CNewMenu::SetMenuDrawMode(style);
	AfxMessageBox("�X�^�C���𔽉f���邽��QROQS���ċN�����Ă��������D");
}

void CMainFrame::OnStyleXp() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
//		TRACE("style XP\n");
	SetStyleMode(CNewMenu::STYLE_XP);
}

void CMainFrame::OnStyleXp2003() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetStyleMode(CNewMenu::STYLE_XP_2003);
}

void CMainFrame::OnStyleIcy() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetStyleMode(CNewMenu::STYLE_ICY);
}

void CMainFrame::OnStyleOriginal() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	SetStyleMode(CNewMenu::STYLE_ORIGINAL);	
}

void CMainFrame::OnUpdateStyleXp(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(((CQROQSApp*)AfxGetApp())->style_mode==CNewMenu::STYLE_XP);
}

void CMainFrame::OnUpdateStyleXp2003(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(((CQROQSApp*)AfxGetApp())->style_mode==CNewMenu::STYLE_XP_2003);
}

void CMainFrame::OnUpdateStyleIcy(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(((CQROQSApp*)AfxGetApp())->style_mode==CNewMenu::STYLE_ICY);	
}

void CMainFrame::OnUpdateStyleOriginal(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->SetCheck(((CQROQSApp*)AfxGetApp())->style_mode==CNewMenu::STYLE_ORIGINAL);
}
#endif

static int f_fullscreen = 0;
void CMainFrame::OnViewFullscreen() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������

	if(f_fullscreen){
		ModifyStyle(0, WS_CAPTION);
		ModifyStyle(0, WS_THICKFRAME);

		// �t���[���E�B���h�E��ʏ�\��
		ShowWindow(SW_RESTORE);
	}
	else{
		// ���C���t���[���E�B���h�E�̃|�C���^���擾
		//CMainFrame * pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

		// WS_CAPTION �� WS_THICKFRAME�X�^�C��������
		ModifyStyle(WS_CAPTION, 0);
		ModifyStyle(WS_THICKFRAME, 0);

		// ���j���[������
	//	SetMenu(NULL);

		// �t���[���E�B���h�E���ő�\��
		ShowWindow(SW_MAXIMIZE);
	}
	f_fullscreen = !f_fullscreen;
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: ���̈ʒu�� command update UI �n���h���p�̃R�[�h��ǉ����Ă�������
	pCmdUI->Enable(true);
	pCmdUI->SetCheck(f_fullscreen);
}

/// EOF ///
