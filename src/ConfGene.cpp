// CongGene.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "qroqs.h"
#include "ConfGene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfGene ダイアログ


CConfGene::CConfGene(CWnd* pParent /*=NULL*/)
	: CDialog(CConfGene::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfGene)
	m_Qmode = 0;
	m_SkinUse = FALSE;
	m_PrintColor = FALSE;
	m_DiamDelta = 0.0f;
	m_Selected = FALSE;
	m_ColR = 0;
	m_ColB = 0;
	m_ColG = 0;
	//}}AFX_DATA_INIT
}

void CConfGene::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfGene)
	DDX_Control(pDX, IDC_SLIDER_G, m_SliderG);
	DDX_Control(pDX, IDC_SLIDER_B, m_SliderB);
	DDX_Control(pDX, IDC_SLIDER_R, m_SliderR);
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjList);
	DDX_Radio(pDX, IDC_RADIO1, m_Qmode);
	DDX_Check(pDX, IDC_SKIN_USE, m_SkinUse);
	DDX_Check(pDX, IDC_PRINT_COLOR, m_PrintColor);
	DDX_Text(pDX, IDC_DIAMDELTA, m_DiamDelta);
	DDV_MinMaxFloat(pDX, m_DiamDelta, 1.e-004f, 10000.f);
	DDX_Check(pDX, IDC_CHECK_SELECTED, m_Selected);
	DDX_Text(pDX, IDC_EDIT_R, m_ColR);
	DDV_MinMaxInt(pDX, m_ColR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_B, m_ColB);
	DDV_MinMaxInt(pDX, m_ColB, 0, 255);
	DDX_Text(pDX, IDC_EDIT_G, m_ColG);
	DDV_MinMaxInt(pDX, m_ColG, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfGene, CDialog)
	//{{AFX_MSG_MAP(CConfGene)
	ON_BN_CLICKED(ID_COM_CONF_FONT, OnComConfFont)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_OBJ_LIST, OnSelchangeObjList)
	ON_BN_CLICKED(IDC_CHECK_SELECTED, OnCheckSelected)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_R, OnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_G, OnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_B, OnChangeEditB)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COLOR_PICKER, OnColorPicker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfGene メッセージ ハンドラ

#include"QROQSView.h"
#include "other.h"

BOOL CConfGene::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
//	TRACE("gene init\n");

	m_Qmode = Qmode?1:0;

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);


	// アイテムの追加
	
	m_ObjList.AddString("ﾌﾟﾚｰｽ");
	m_ObjList.SetItemData(0,_PLACE_COL);
	m_ObjList.AddString("ﾄﾗﾝｼﾞｼｮﾝ");
	m_ObjList.SetItemData(1,_TRAN_COL);
	m_ObjList.AddString("ﾊﾟｽﾄﾗﾝｼﾞｼｮﾝ");
	m_ObjList.SetItemData(2,_PASSTRA_COL);
	m_ObjList.AddString("ｳｰｼﾞ");
	m_ObjList.SetItemData(3,_UHZI_COL);
	m_ObjList.AddString("ｹﾑﾏｷ");
	m_ObjList.SetItemData(4,_KEM_COL);
	m_ObjList.AddString("ﾓｼﾞｭｰﾙ");
	m_ObjList.SetItemData(5,_MOD_COL);
	m_ObjList.AddString("ｱｰｸ");
	m_ObjList.SetItemData(6,_ARC_COL);
	m_ObjList.AddString("背景");
	m_ObjList.SetItemData(7,_BACK_COL);
	m_ObjList.AddString("ｸﾞﾘｯﾄﾞ");
	m_ObjList.SetItemData(8,_GLID_COL);
	m_ObjList.AddString("ﾃｷｽﾄ");
	m_ObjList.SetItemData(9,_TEXT_COL);

	vcolmap	= pView->vColMap;

	nSel=0;
	selobj=_PLACE_COL;
	m_ObjList.SelectString(0,"ﾌﾟﾚｰｽ");

	update=0;

#ifdef _LIGHT
	m_SkinUse = 0;
#else
	m_SkinUse = pView->SkinUse;
#endif
	m_PrintColor = pView->PrintColor;

	m_DiamDelta = pView->diamDelta;

	m_Selected = false;


	m_SliderR.SetRange(0,255);		//スライダーの範囲を設定する
	m_SliderR.SetPageSize(1);
	m_SliderG.SetRange(0,255);		//スライダーの範囲を設定する
	m_SliderG.SetPageSize(1);
	m_SliderB.SetRange(0,255);		//スライダーの範囲を設定する
	m_SliderB.SetPageSize(1);


	UpdateData(false);

	SliderSet();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void mRGB(COLORREF col,int *r,int *g,int *b)
{
	(*r) = col & 0x0000ff;
	col >>= 8;
	(*g) = col & 0x0000ff;
	(*b) = col >> 8;
}

// viewの色を元にスライダーを設定する
void CConfGene::SliderSet() 
{	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	COLORREF col;
	if(selobj!=_BACK_COL && selobj!=_GLID_COL && selobj!=_TEXT_COL){
		col = pView->vColMap[selobj + m_Selected*_SELECTED_COL];
	}
	else{
		col = pView->vColMap[selobj];
	}

	mRGB(col,&m_ColR,&m_ColG,&m_ColB);

	m_SliderR.SetPos( m_ColR );
	m_SliderG.SetPos( m_ColG );
	m_SliderB.SetPos( m_ColB );

	UpdateData(false);

	viewRedraw();
	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

void CConfGene::OnSelchangeObjList() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	nSel = m_ObjList.GetCurSel();
	
	selobj = m_ObjList.GetItemData(nSel);
//	TRACE("%d\n",nSel);

	CButton *myButton;
	myButton=(CButton *)GetDlgItem(IDC_CHECK_SELECTED);
	if(selobj==_BACK_COL || selobj==_GLID_COL || selobj==_TEXT_COL){
		myButton->EnableWindow(false);
	}
	else{
		myButton->EnableWindow(true);
	}
		
	SliderSet();
}

void CConfGene::OnCheckSelected() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData(true);
	SliderSet();
}

void CConfGene::viewRedraw()
{

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	int sel;
	sel = selobj;
	if(selobj!=_BACK_COL && selobj!=_GLID_COL && selobj!=_TEXT_COL){
		sel += m_Selected*_SELECTED_COL;
	}
	pView->vColMap[sel] = RGB(m_ColR,m_ColG,m_ColB);

	pView->viewPenBrushChange();
	GetParent()->GetParent()->Invalidate(0);

	OnPaint();
}

void CConfGene::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
//	TRACE("scroll\n");

	UpdateData(true);

	if((CSliderCtrl*)pScrollBar==&m_SliderR){
		m_ColR = m_SliderR.GetPos();
		viewRedraw();
	}
	if((CSliderCtrl*)pScrollBar==&m_SliderG){
		m_ColG = m_SliderG.GetPos();
		viewRedraw();
	}
	if((CSliderCtrl*)pScrollBar==&m_SliderB){
		m_ColB = m_SliderB.GetPos();
		viewRedraw();
	}
	
	UpdateData(false);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CConfGene::OnChangeEditR() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	UpdateData(true);
	m_SliderR.SetPos( m_ColR );
	viewRedraw();
}

void CConfGene::OnChangeEditG() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	UpdateData(true);
	m_SliderG.SetPos( m_ColG );
	viewRedraw();
}

void CConfGene::OnChangeEditB() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	UpdateData(true);
	m_SliderB.SetPos( m_ColB );
	viewRedraw();
}

void CConfGene::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	CWnd* myPict=GetDlgItem(IDC_COLREF);
	CClientDC myDC(myPict);
	CRect vrect;
	myPict->GetClientRect(&vrect);
	myDC.FillSolidRect(vrect,RGB(m_ColR,m_ColG,m_ColB));

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}

void CConfGene::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

//	TRACE("gene destroy %d\n",update);
	Qmode = m_Qmode?true:false;

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);
#ifndef _LIGHT
	pView->SkinUse = m_SkinUse;
#endif
	pView->PrintColor = m_PrintColor;
	pView->diamDelta = m_DiamDelta;

	// 一旦ｴﾗｰ表示をけす。
	pView->bsp->ErrPos = 0;

	if(update==1){
	}
	else{
		// ｷｬﾝｾﾙでリセット
		pView->vColMap = vcolmap;
		pView->viewPenBrushChange();
		GetParent()->GetParent()->Invalidate(0);
	}

}

void CConfGene::OnColorPicker() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	UpdateData(true);

	CColorDialog dlgColor(RGB(m_ColR,m_ColG,m_ColB),CC_FULLOPEN);
	if (dlgColor.DoModal() == IDOK) {
		mRGB(dlgColor.GetColor(),&m_ColR,&m_ColG,&m_ColB);
		m_SliderR.SetPos( m_ColR );
		m_SliderG.SetPos( m_ColG );
		m_SliderB.SetPos( m_ColB );
		viewRedraw();
		UpdateData(false);
	}
}

void CConfGene::OnComConfFont() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	/*
	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);

	LOGFONT lf;
	pView->vcFont.GetLogFont(&lf);

	CFontDialog fDlg(&lf, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	if(fDlg.DoModal()==IDOK){

	}
	*/
}




//	EOF //
