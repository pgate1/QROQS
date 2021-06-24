// MyTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyTreeCtrl.h"

#include"BSPetri.h"
#include"other.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

CMyTreeCtrl::CMyTreeCtrl() : m_bEnableLastAction (false)
{
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool CMyTreeCtrl::XPopupCtrlEvent::OnInit ()
{
    METHOD_PROLOGUE_(CMyTreeCtrl, PopupCtrlEvent);

 //   TRACE (_T("CMyTreeCtrl::OnInit ()\n"));

    return true;
}

///////////////////////////////////////////////////////////////////////////////
void CMyTreeCtrl::XPopupCtrlEvent::OnShow ()
{
    METHOD_PROLOGUE_(CMyTreeCtrl, PopupCtrlEvent);

  //  TRACE (_T("CMyTreeCtrl::OnShow ()\n"));
    pThis->SetFocus();
}

///////////////////////////////////////////////////////////////////////////////

void CMyTreeCtrl::XPopupCtrlEvent::OnHide (bool bAbort)
{
    METHOD_PROLOGUE_(CMyTreeCtrl, PopupCtrlEvent);

	CTreeCtrl *pModTree = GetModuleTree();

	//選択したモジュールに移動する
   if ( pModTree != NULL && !bAbort )
    {

	   HTREEITEM hitem = (HTREEITEM)pThis->GetItemData (pThis->GetSelectedItem());
//	   TRACE("selected %s %d\n",pThis->GetItemText(pThis->GetSelectedItem()), hitem );

	   if(hitem == pModTree->GetRootItem()){
		   pModTree->SelectItem( hitem );
		   return;
	   }

	   if(PetMode == _EDIT){
	//	   TRACE("in %d\n",GetModTreeDlg()->IsNamed(hitem));
		   if(!GetModTreeDlg()->IsNamed(hitem)){
			  // AfxMessageBox("モジュール名が設定されていません");
			   return;
		   }
	   }

		pModTree->SelectItem( hitem );
    }

	/*

    TRACE (_T("CMyTreeCtrl::OnHide (Abort = %s)\n"), bAbort ? _T("true") : _T("false"));

    CEditView* pView = (CEditView*)((CMDIFrameWnd*)AfxGetMainWnd())->GetActiveFrame()->GetActiveView();

    if ( pView != NULL && !bAbort )
    {
        CString sText;
        int nLength = pView->GetWindowTextLength();

        sText.Format (_T("CMyTreeCtrl::Selection = \"%s\"\r\n"), pThis->GetItemText (pThis->GetSelectedItem()));

        pView->GetEditCtrl().SetSel (nLength + 1, nLength + 1, TRUE);
        pView->GetEditCtrl().ReplaceSel (sText);
        nLength = pView->GetWindowTextLength();
        pView->GetEditCtrl().SetSel (nLength + 1, nLength + 1);
    }

  */
}

///////////////////////////////////////////////////////////////////////////////
int CMyTreeCtrl::XPopupCtrlEvent::OnKeyDown (UINT nChar, UINT, UINT)
{
    if ( nChar == VK_RETURN )
    {
        return CPopup::end;
    }
    return CPopup::doNothing;
}

///////////////////////////////////////////////////////////////////////////////
int CMyTreeCtrl::XPopupCtrlEvent::OnLButtonDown (UINT, CPoint pt)
{
    METHOD_PROLOGUE_(CMyTreeCtrl, PopupCtrlEvent);

    UINT uFlags;

    // Store information used in next OnLButtonUp
    pThis->m_bEnableLastAction = pThis->HitTest (pt, &uFlags) == NULL ||
                                 (uFlags & (TVHT_ONITEMBUTTON|TVHT_ONITEMINDENT)) == 0;

    return CPopup::doNothing;
}

///////////////////////////////////////////////////////////////////////////////
int CMyTreeCtrl::XPopupCtrlEvent::OnLButtonUp (UINT, CPoint pt)
{
    METHOD_PROLOGUE_(CMyTreeCtrl, PopupCtrlEvent);

    UINT uFlags;

    if ( pThis->m_bEnableLastAction &&
         pThis->HitTest (pt, &uFlags) != NULL &&
         (uFlags & (TVHT_ONITEMBUTTON|TVHT_ONITEMINDENT)) == 0 )
    {
        return CPopup::end;
    }
    return CPopup::doNothing;
}

///////////////////////////////////////////////////////////////////////////////
IPopupCtrlEvent* CMyTreeCtrl::XPopupCtrlEvent::GetInterfaceOf (HWND)
{
    // Not used here because this control doesn't include other controls
    return NULL;
}
