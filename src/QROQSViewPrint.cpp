// QROQSViewPrint.cpp : CQROQSView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"
#include "QROQSView.h"


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CQROQSView クラスの印刷


//プリンタの使用可能チェック
BOOL printerCheck()
{
    PRINTDLG p;
    p.hDevMode = 0;
    if( AfxGetApp()->GetPrinterDeviceDefaults(&p ) )
        if( p.hDevMode != NULL )    return TRUE;
    return FALSE;
}

BOOL CQROQSView::OnPreparePrinting(CPrintInfo* pInfo)
{
//	TRACE("p1\n");


	if(printerCheck()==false){
	    AfxMessageBox( "ﾌﾟﾘﾝﾀを使用できません．");
		return false;
	}


//	FitView();


	if(PrintColor==0){
		delete pen_arc;
		delete pen_place;
		delete pen_transition;
		delete pen_transition_func;
		delete pen_uhzi;
		delete pen_kemmakix;
		delete pen_token;
		delete pen_module;

		delete brush_uhzi;
		delete brush_kemmakix;
		delete brush_module;
		delete pen_hatch;
		
		int i;
		i=1;
		pen_arc = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_place = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_transition = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_transition_func = new CPen(PS_SOLID, i ,RGB(0,0,0));
		pen_uhzi = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_kemmakix = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_module = new CPen(PS_SOLID, i, RGB(0,0,0));
		pen_token = new CPen(PS_SOLID, i, RGB(0,0,0));
		
		brush_uhzi = new CBrush(RGB(0,0,0));
		brush_kemmakix = new CBrush(RGB(0,0,0));
		brush_module = new CBrush(RGB(0,0,0));
		pen_hatch=new CPen(PS_SOLID, 1,RGB(0,0,0));
	}



	int m_nNowPage=0;
	int m_nBeginPage =0, m_nEndPage = 0;

	pInfo->SetMaxPage(1);

	m_nBeginPage =1;
	m_nEndPage=1;


	BOOL bPreview = pInfo->m_bPreview;	//プレビューフラグのバックアップ
	if(bPreview){	//印刷プレビューの時
		pInfo->m_nCurPage = m_nNowPage+1;	//現在表示中のページをプレビュー開始時のページに
		m_nBeginPage = m_nEndPage = 0;
	}
	else{	//印刷の時
		if(m_nBeginPage>0 && m_nEndPage>0){	//印刷範囲の指定有り
			pInfo->m_bPreview = TRUE;	//プレビューのふりをすることにより、印刷ダイアログを出さない
		}
	}


	// デフォルトの印刷準備
	BOOL ret = DoPreparePrinting(pInfo);
	pInfo->m_bPreview = bPreview;	//プレビューフラグを元に戻す

	if(m_nBeginPage>0 && m_nEndPage>0){	//印刷範囲の指定有り
		PRINTDLG *p = &pInfo->m_pPD->m_pd;
		p->nFromPage = 1;//m_nBeginPage;	//印刷開始ページ
		p->nToPage   = 1;//m_nEndPage;	//最終印刷ページ
	}

	return ret;

	// デフォルトの印刷準備
//	return DoPreparePrinting(pInfo);
}

void CQROQSView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
	TRACE("p2\n");


	print_now_page = 1;

	DEVMODE *dp=pInfo->m_pPD->GetDevMode();
	
	dp->dmPaperSize		= DMPAPER_A4;        	//Ａ４用紙
	dp->dmOrientation	= DMORIENT_LANDSCAPE;	//横向き
//	dp->dmScale			= 500;	//スケール
//	dp->dmPrintQuality	= DMRES_HIGH;	//解像度
//	dp->dmColor			= DMCOLOR_CIKIR ;	//カラー

	pDC->ResetDC(dp);



}


void CQROQSView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	TRACE("print_now_page %d\n",pInfo->m_nCurPage);

	printRect = pInfo->m_rectDraw;
	if(viewRect.Width()>viewRect.Height()){
		printRect.bottom = (double)viewRect.Height() * printRect.Width() / viewRect.Width();
	}
	else{
		printRect.right = (double)viewRect.Width() * printRect.Height() / viewRect.Height();
	}



//	TRACE("pRect %d %d\n",printRect.Width(),printRect.Height());

//	bsp->Globalpoint.x = bsp->globalRect.left
//		+ viewRect.Width()*(pInfo->m_nCurPage-1);


	CView::OnPrint(pDC, pInfo);
}


void CQROQSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
	TRACE("p3\n");

	viewPenBrushChange();
	
	PetMode=_EDIT;

	Invalidate(0);

}

void CQROQSView::OnFilePrint() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	TRACE("CQROQSView::OnFilePrint\n");
//	PostMessage(WM_COMMAND, ID_FILE_PRINT_DIRECT);

	CView::OnFilePrint();
}

void CQROQSView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	
}


/// EOF ///


