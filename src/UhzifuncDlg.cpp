// UhzifuncDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "QROQS.h"
#include "UhzifuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUhzifuncDlg �_�C�A���O


CUhzifuncDlg::CUhzifuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUhzifuncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUhzifuncDlg)
	m_radio = 0;
	m_line = 0;
	m_In = 0;
	m_uInputData = _T("");
	m_InputName = _T("");
	m_Common = FALSE;
	m_Hex = FALSE;
	//}}AFX_DATA_INIT

}

void CUhzifuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUhzifuncDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Radio(pDX, IDC_RADIO3, m_line);
	DDX_Text(pDX, IDC_UHZIVALUE, m_In);
	DDV_MinMaxInt(pDX, m_In, 0, 1);
	DDX_Text(pDX, IDC_UHZIVALUESET, m_uInputData);
	DDX_Text(pDX, IDC_INPUTNAME, m_InputName);
	DDV_MaxChars(pDX, m_InputName, 30);
	DDX_Check(pDX, IDC_COMMON, m_Common);
	DDX_Check(pDX, IDC_CHECK_HEX, m_Hex);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CUhzifuncDlg, CDialog)
	//{{AFX_MSG_MAP(CUhzifuncDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUhzifuncDlg ���b�Z�[�W �n���h��

#include "other.h"

BOOL CUhzifuncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������

	bspUhzi *m_Uhzi = m_bspt->GetBSP(m_mname.c_str())->UhziMap[m_id];


	m_radio = m_Uhzi->addp;

	m_uInputData.Empty();

	list<string>::iterator ulit,ulit_end;
	ulit=m_Uhzi->InputString.begin();
	ulit_end = m_Uhzi->InputString.end();
	for(;ulit!=ulit_end;++ulit){
		m_uInputData += ulit->c_str();
		m_uInputData += "\r\n";
	}


	m_In = m_Uhzi->In;
	m_line = m_Uhzi->dline;//���������@0�F��ʁi���j1�F���ʂ���i�E�j
	m_Hex = (m_Uhzi->bits==4);
	m_InputName = m_Uhzi->input_name.c_str();
	m_Common = 0;
//TRACE("line %d\n",m_line);
	UpdateData(false);

	if(PetMode==_EDIT){
		CEdit *myED=(CEdit *)GetDlgItem(IDC_UHZIVALUESET);
		myED->SetReadOnly(!m_radio);
		myED=(CEdit *)GetDlgItem(IDC_UHZIVALUE);
		myED->SetReadOnly(m_radio);

		CButton *myB=(CButton *)GetDlgItem(IDC_RADIO3);
		if(myB && m_radio==0) myB->EnableWindow(false);
		myB=(CButton *)GetDlgItem(IDC_RADIO4);
		if(myB && m_radio==0) myB->EnableWindow(false);
	}
	else{
		CEdit *myED=(CEdit *)GetDlgItem(IDC_UHZIVALUESET);
		myED->SetReadOnly(true);
		myED=(CEdit *)GetDlgItem(IDC_UHZIVALUE);
		myED->SetReadOnly(true);

		CButton *myB=(CButton *)GetDlgItem(IDC_RADIO1);
		if(myB) myB->EnableWindow(false);
		myB=(CButton *)GetDlgItem(IDC_RADIO2);
		if(myB) myB->EnableWindow(false);
		myB=(CButton *)GetDlgItem(IDC_RADIO3);
		if(myB) myB->EnableWindow(false);
		myB=(CButton *)GetDlgItem(IDC_RADIO4);
		if(myB) myB->EnableWindow(false);
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CUhzifuncDlg::OnRadio1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(true);

	CEdit *myED=(CEdit *)GetDlgItem(IDC_UHZIVALUESET);
	myED->SetReadOnly(true);
	myED=(CEdit *)GetDlgItem(IDC_UHZIVALUE);
	myED->SetReadOnly(false);

	CButton *myB=(CButton *)GetDlgItem(IDC_RADIO3);
	if(myB) myB->EnableWindow(false);
	myB=(CButton *)GetDlgItem(IDC_RADIO4);
	if(myB) myB->EnableWindow(false);
}

void CUhzifuncDlg::OnRadio2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(true);

	CEdit *myED=(CEdit *)GetDlgItem(IDC_UHZIVALUE);
	myED->SetReadOnly(true);
	myED=(CEdit *)GetDlgItem(IDC_UHZIVALUESET);
	myED->SetReadOnly(false);

	CButton *myB=(CButton *)GetDlgItem(IDC_RADIO3);
	if(myB) myB->EnableWindow(true);
	myB=(CButton *)GetDlgItem(IDC_RADIO4);
	if(myB) myB->EnableWindow(true);
}

void CUhzifuncDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	UpdateData(true);

	if(PetMode==_EDIT){

		BSPetri *bsp = m_bspt->GetBSP(m_mname.c_str());
		if(bsp==NULL) goto ON_OK;

		if(bsp->UhziMap.find(m_id)==bsp->UhziMap.end()) goto ON_OK;
		bspUhzi *m_Uhzi = bsp->UhziMap[m_id];

		bsp->ModifiedFlag = 1;
		m_Uhzi->addp = m_radio;
	//	TRACE("addp %d\n",m_Uhzi->addp);
		m_Uhzi->In = m_In;
	//	TRACE("%s",m_uInputData);
		m_Uhzi->bits = m_Hex==1 ? 4 : 1;

		int i=0;
		char c;
		for(i=0;i<m_uInputData.GetLength();++i){
			c = m_uInputData[i];
//			TRACE("%c",c);
			if(isspace(c)) continue;
			if(m_Hex){
				if( isxdigit( c ) ) continue;
				AfxMessageBox("���̓f�[�^��16�i���œ��͂��Ă��������D");
				return;
			}
			if(c=='0' || c=='1') continue;
			AfxMessageBox("���̓f�[�^��2�i���œ��͂��Ă��������D");
			return;
		}


		StringToList(m_Uhzi->InputString, m_uInputData);
	//	TRACE("%s",m_Uhzi->InputString->begin()->c_str());
		m_Uhzi->dline = m_line;
		m_Uhzi->input_name = m_InputName;

		if(m_Common){
		//	TRACE("common\n");
			map<int,bspUhzi*>::iterator uit;
			uit = bsp->UhziMap.begin();
			for(;uit!=bsp->UhziMap.end();++uit){
				if( bsp->intrack.find( uit->first ) == bsp->intrack.end()) continue;
				*(uit->second) = *m_Uhzi;
			}
		}

	}

ON_OK:
	GetParent()->Invalidate();
	CDialog::OnOK();
}

void CUhzifuncDlg::PostNcDestroy() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	delete this;

	CDialog::PostNcDestroy();
}


/// EOF ///

