// CongGene.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CConfGene �_�C�A���O


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
// CConfGene ���b�Z�[�W �n���h��

#include"QROQSView.h"
#include "other.h"

BOOL CConfGene::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
//	TRACE("gene init\n");

	m_Qmode = Qmode?1:0;

	POSITION p;
	p = m_pDoc->GetFirstViewPosition();
	CQROQSView *pView;
	pView = (CQROQSView *)m_pDoc->GetNextView(p);


	// �A�C�e���̒ǉ�
	
	m_ObjList.AddString("��ڰ�");
	m_ObjList.SetItemData(0,_PLACE_COL);
	m_ObjList.AddString("��ݼ޼��");
	m_ObjList.SetItemData(1,_TRAN_COL);
	m_ObjList.AddString("�߽��ݼ޼��");
	m_ObjList.SetItemData(2,_PASSTRA_COL);
	m_ObjList.AddString("����");
	m_ObjList.SetItemData(3,_UHZI_COL);
	m_ObjList.AddString("��Ϸ");
	m_ObjList.SetItemData(4,_KEM_COL);
	m_ObjList.AddString("Ӽޭ��");
	m_ObjList.SetItemData(5,_MOD_COL);
	m_ObjList.AddString("���");
	m_ObjList.SetItemData(6,_ARC_COL);
	m_ObjList.AddString("�w�i");
	m_ObjList.SetItemData(7,_BACK_COL);
	m_ObjList.AddString("��د��");
	m_ObjList.SetItemData(8,_GLID_COL);
	m_ObjList.AddString("÷��");
	m_ObjList.SetItemData(9,_TEXT_COL);

	vcolmap	= pView->vColMap;

	nSel=0;
	selobj=_PLACE_COL;
	m_ObjList.SelectString(0,"��ڰ�");

	update=0;

#ifdef _LIGHT
	m_SkinUse = 0;
#else
	m_SkinUse = pView->SkinUse;
#endif
	m_PrintColor = pView->PrintColor;

	m_DiamDelta = pView->diamDelta;

	m_Selected = false;


	m_SliderR.SetRange(0,255);		//�X���C�_�[�͈̔͂�ݒ肷��
	m_SliderR.SetPageSize(1);
	m_SliderG.SetRange(0,255);		//�X���C�_�[�͈̔͂�ݒ肷��
	m_SliderG.SetPageSize(1);
	m_SliderB.SetRange(0,255);		//�X���C�_�[�͈̔͂�ݒ肷��
	m_SliderB.SetPageSize(1);


	UpdateData(false);

	SliderSet();

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void mRGB(COLORREF col,int *r,int *g,int *b)
{
	(*r) = col & 0x0000ff;
	col >>= 8;
	(*g) = col & 0x0000ff;
	(*b) = col >> 8;
}

// view�̐F�����ɃX���C�_�[��ݒ肷��
void CConfGene::SliderSet() 
{	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
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
	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}

void CConfGene::OnSelchangeObjList() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

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
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
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
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	UpdateData(true);
	m_SliderR.SetPos( m_ColR );
	viewRedraw();
}

void CConfGene::OnChangeEditG() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	UpdateData(true);
	m_SliderG.SetPos( m_ColG );
	viewRedraw();
}

void CConfGene::OnChangeEditB() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	UpdateData(true);
	m_SliderB.SetPos( m_ColB );
	viewRedraw();
}

void CConfGene::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	CWnd* myPict=GetDlgItem(IDC_COLREF);
	CClientDC myDC(myPict);
	CRect vrect;
	myPict->GetClientRect(&vrect);
	myDC.FillSolidRect(vrect,RGB(m_ColR,m_ColG,m_ColB));

	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}

void CConfGene::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

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

	// ��U�װ�\���������B
	pView->bsp->ErrPos = 0;

	if(update==1){
	}
	else{
		// ��ݾقŃ��Z�b�g
		pView->vColMap = vcolmap;
		pView->viewPenBrushChange();
		GetParent()->GetParent()->Invalidate(0);
	}

}

void CConfGene::OnColorPicker() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

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
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
