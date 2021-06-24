// FindObjDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "qroqs.h"
#include "FindObjDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg �_�C�A���O


CFindObjDlg::CFindObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindObjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindObjDlg)
	//}}AFX_DATA_INIT
}


void CFindObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindObjDlg)
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjList);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, ID_SEARCH, m_btnSearch);
#endif
}


BEGIN_MESSAGE_MAP(CFindObjDlg, CDialog)
	//{{AFX_MSG_MAP(CFindObjDlg)
	ON_BN_CLICKED(ID_SEARCH, OnSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJ_LIST, OnItemchangedObjList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindObjDlg ���b�Z�[�W �n���h��

BOOL CFindObjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	//��s�I��
	ListView_SetExtendedListViewStyle(
		(HWND)m_ObjList.m_hWnd, LVS_EX_FULLROWSELECT);

//�@���X�g�R���g���[���̃X�^�C���ݒ�i�O���b�h���C���̕\���j
	DWORD dwStyle = ListView_GetExtendedListViewStyle( (HWND)m_ObjList.m_hWnd );
	dwStyle |= LVS_EX_GRIDLINES; // | LVS_EX_FULLROWSELECT;//CHECKBOXES,HEADERRAGDROP
	ListView_SetExtendedListViewStyle( (HWND)m_ObjList.m_hWnd, dwStyle );


	// �J�����̒ǉ�
	m_ObjList.InsertColumn( 0, "ID", LVCFMT_LEFT, 75, -1);
	m_ObjList.InsertColumn( 1, "Type", LVCFMT_LEFT, 75, -1);
	m_ObjList.InsertColumn( 2, "Option", LVCFMT_LEFT, 75, -1);


	// �A�C�e���̒ǉ�
	CString cs;
	int    nItem;
	map<int,bspObject>::iterator oit,oit_end;
	oit=m_bsp->ObjMap.begin();
	oit_end = m_bsp->ObjMap.end();
	for(nItem=0;oit!=oit_end;++oit,++nItem){
		cs.Format("%d",oit->first);
		bspObject &obj = oit->second;
		m_ObjList.InsertItem( nItem, cs, 0);
		switch(oit->second.type){
		case _PLACE: cs.Format("place"); break;
		case _TRANSITION: cs.Format("transition"); break;
		case _UHZI: cs.Format("uhzi"); break;
		case _KEMMAKIX: cs.Format("kemmakix"); break;
		case _MODULE: cs.Format("module"); break;
		}
		m_ObjList.SetItem( nItem, 1, LVIF_TEXT, cs, 0, 0, 0, 0);

		CString opt_str;
		if(obj.type==_MODULE){
			opt_str = m_bsp->ModMap[oit->first].c_str();
		}
		else if(obj.type==_UHZI){
			opt_str = m_bsp->UhziMap[oit->first]->input_name.c_str();
		}
		else if(obj.type==_KEMMAKIX){
			opt_str = m_bsp->KemMap[oit->first]->output_name.c_str();
		}
		if(!opt_str.IsEmpty()) opt_str += " ";
		if(m_bsp->TextMap.find(oit->first)!=m_bsp->TextMap.end()){
			opt_str += m_bsp->TextMap[oit->first].c_str();
		}
		if(!opt_str.IsEmpty()) opt_str += " ";
		if(obj.type==_TRANSITION
			&& m_bsp->simTranFireableStop.find(oit->first)!=m_bsp->simTranFireableStop.end()){
			opt_str += "SimFireStop";
		}

		if(!opt_str.IsEmpty()){
			m_ObjList.SetItem( nItem, 2, LVIF_TEXT, opt_str, 0, 0, 0, 0);
		}
	}


	nSelID=0;

	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow(false);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CFindObjDlg::OnSearch() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nSel;
	nSel=m_ObjList.GetSelectionMark();
	if(nSel==-1){
		nSelID=0;
		CDialog::OnOK();
	}
//	TRACE("n %d\n",nSel);

	CString cs;
	cs=m_ObjList.GetItemText(nSel, 0);

	nSelID=atoi((LPCTSTR)cs);

//	TRACE("id %d\n",nSelID);

	CDialog::OnOK();
}


void CFindObjDlg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B

	nSelID=0;

	CDialog::OnCancel();
}

void CFindObjDlg::OnItemchangedObjList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	//���X�g�̃A�C�e�����I�����ꂽ��\���{�^�����g�p�ł���悤�ɂ���
	CButton *bt;
	bt=(CButton *)GetDlgItem(ID_SEARCH);
	bt->EnableWindow();

	*pResult = 0;
}

/// EOF ///
