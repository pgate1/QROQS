// KemfuncDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "QROQS.h"
#include "KemfuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKemfuncDlg �_�C�A���O


CKemfuncDlg::CKemfuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKemfuncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKemfuncDlg)
	m_Getline = 0;
	m_compData = _T("");
	m_kOutputData = _T("");
	m_PassStatus = _T("");
	m_PassStatusAll = _T("");
	m_OutputName = _T("");
	m_Common = FALSE;
	m_Bits = 1;
	m_Hex = FALSE;
	//}}AFX_DATA_INIT
}

void CKemfuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKemfuncDlg)
	DDX_Control(pDX, IDC_SPIN_BITS, m_SpinBits);
	DDX_Radio(pDX, IDC_RADIO1, m_Getline);
	DDX_Text(pDX, IDC_COMPDATA, m_compData);
	DDX_Text(pDX, IDC_OUTDATA, m_kOutputData);
	DDX_Text(pDX, IDC_PASSSTATUS, m_PassStatus);
	DDX_Text(pDX, IDC_PASSSTATUSALL, m_PassStatusAll);
	DDX_Text(pDX, IDC_OUTPUTNAME, m_OutputName);
	DDV_MaxChars(pDX, m_OutputName, 30);
	DDX_Check(pDX, IDC_COMMON, m_Common);
	DDX_Text(pDX, IDC_BITS, m_Bits);
	DDV_MinMaxInt(pDX, m_Bits, 1, 1024);
	DDX_Check(pDX, IDC_CHECK_HEX, m_Hex);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
#endif
}

BEGIN_MESSAGE_MAP(CKemfuncDlg, CDialog)
	//{{AFX_MSG_MAP(CKemfuncDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_REDRAW, OnRedraw)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKemfuncDlg ���b�Z�[�W �n���h��


#include"other.h"

BOOL CKemfuncDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	bspKemmakix *m_Kem = m_bspt->GetBSP(m_mname.c_str())->KemMap[m_id];
	m_Getline = m_Kem->dline;
	m_OutputName = m_Kem->output_name.c_str();
	m_PassStatusAll.Empty();

	m_Bits = m_Kem->bits;
	if(m_Bits==0){
		bspKemmakix K;
		m_Bits = K.bits;
	}
	m_Hex = m_Kem->hex;

	m_SpinBits.SetBuddy( GetDlgItem(IDC_BITS) );
	m_SpinBits.SetRange(1,1024);
	m_SpinBits.SetPos(m_Bits);


	UpdateData(false);

	OnRedraw();

	if(m_Getline==2){	//�g�[�N����j�����郂�[�h
		OnRadio3();
	}
	else if(m_Getline==3){	//����Ȃ����[�h
		OnRadio4();
	}
	if(PetMode==_SIMULATION){
		CEdit *myED;
		myED=(CEdit *)GetDlgItem(IDC_COMPDATA);
		myED->SetReadOnly(true);

		CButton *myB;
		myB=(CButton *)GetDlgItem(IDC_RADIO1);
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

void CKemfuncDlg::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	
	UpdateData(true);

	if(PetMode==_EDIT){
		BSPetri *bsp = m_bspt->GetBSP(m_mname.c_str());
		if(bsp==NULL) goto ON_OK;
		if(bsp->KemMap.find(m_id)==bsp->KemMap.end()) goto ON_OK;
		bspKemmakix *m_Kem = bsp->KemMap[m_id];

		bsp->ModifiedFlag = 1;
		m_Kem->dline = m_Getline;
		m_Kem->output_name = m_OutputName;
		m_Kem->bits = m_Bits;
		m_Kem->hex = m_Hex;

	//	TRACE("%s\n", m_compData);
		if(!m_compData.IsEmpty()){
			if(m_Kem->compString==NULL) m_Kem->compString = new list<string>;
			StringToList(*(m_Kem->compString), m_compData);
		}

		if(m_Common){
		//	TRACE("common\n");
			map<int,bspKemmakix*>::iterator kit;
			kit = bsp->KemMap.begin();
			for(;kit!=bsp->KemMap.end();++kit){
				if( bsp->intrack.find( kit->first ) == bsp->intrack.end()) continue;
				*(kit->second) = *m_Kem;
			}
		}

	}

ON_OK:
	GetParent()->Invalidate();
	CDialog::OnOK();
}

void CKemfuncDlg::PostNcDestroy() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	delete this;

	CDialog::PostNcDestroy();
}

void CKemfuncDlg::OnRadio1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CEdit *myED;
	myED=(CEdit *)GetDlgItem(IDC_COMPDATA);
	myED->SetReadOnly(false);
}

void CKemfuncDlg::OnRadio2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnRadio1();
}

void CKemfuncDlg::OnRadio3() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CEdit *myED;
	myED=(CEdit *)GetDlgItem(IDC_COMPDATA);
	myED->SetReadOnly(true);
}


void CKemfuncDlg::OnRadio4() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnRadio3();
}


void CKemfuncDlg::OnRedraw() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	UpdateData(true);

	BSPetri *bsp = m_bspt->GetBSP(m_mname.c_str());
	if(bsp==NULL){
		m_kOutputData = "���ł�BSP��\r\n�폜����Ă��܂��D";
		UpdateData(false);
		return;
	}
	if(bsp->KemMap.find(m_id)==bsp->KemMap.end()){
		m_kOutputData = "���łɹ�Ϸ��\r\n�폜����Ă��܂��D";
		UpdateData(false);
		return;
	}
	bspKemmakix *m_Kem =  bsp->KemMap[m_id];



	m_kOutputData.Empty();
	list<string> OutputString;

	string s;
	char c[2];
	c[1]='\0';
	for(int j=0;j<m_Kem->ptr;j+=m_Bits){
		s.erase();
		for(int i=j; i<j+m_Bits; ++i){
			if(i>=m_Kem->ptr) break;
			c[0] = m_Kem->OutputData[i] + '0';
			if(m_Kem->dline==0){//��ʃr�b�g����󂯎��ꍇ
				s = s + c;
			}
			else{//dline==1 ���ʃr�b�g����󂯎��ꍇ
				s = c + s;
			}
		}
		OutputString.push_back( s );
	}


	list<string> out_list;
	list<string>::iterator it,it_end,cit,cit_end;

	// �o�͌`����out_str�Ɋi�[
	if(m_Hex && m_Bits%4==0){
		it = OutputString.begin();
		it_end = OutputString.end();
		for(;it!=it_end;++it){
			if(it->length()%4==0){
				string outstr;
				char co[2];
				for(int i=0;i<m_Bits&&i<it->length();i+=4){
					sprintf(co,"%01X", (((*it)[i]-'0')<<3) | (((*it)[i+1]-'0')<<2) | (((*it)[i+2]-'0')<<1) | ((*it)[i+3]-'0') );
					outstr += co;
				}
				out_list.push_back( outstr );
			}
			else{
				out_list.push_back( *it );
			}
		}
	}
	else{
		out_list = OutputString;
	}

	it = out_list.begin();
	it_end = out_list.end();
	for(;it!=it_end;++it){
		m_kOutputData += (*it + "\r\n").c_str();
	}

//TRACE("%s\n",m_Kem->compString);
	
	m_compData.Empty();
	if(m_Kem->compString){
		it = out_list.begin();
		it_end = out_list.end();
		cit = m_Kem->compString->begin();
		cit_end = m_Kem->compString->end();
		for(;cit!=cit_end;++cit,++it){
			m_compData += (*cit + "\r\n").c_str();
		}
		for(;it!=it_end;++it){
			m_compData += "\r\n";
		}
	}

	if(m_Kem->compString){
		m_PassStatus.Empty();
		//�ƍ�
		int flag=true;
		it = out_list.begin();
		it_end = out_list.end();
		cit = m_Kem->compString->begin();
		cit_end = m_Kem->compString->end();
		for(;;++it,++cit){
			if(it==it_end) break;
			if(cit==cit_end){
				for(;it!=it_end;++it){
					m_PassStatus += " -\r\n";
				}
				break;
			}
			if(*it == *cit){
				m_PassStatus += "��\r\n";
			}
			else{
				m_PassStatus += "�~\r\n";
				flag=false;
			}
		}
		if(flag) m_PassStatusAll = "��";
		else m_PassStatusAll = "�~";
	}


	UpdateData(false);

	CEdit *passEdit=(CEdit *)GetDlgItem(IDC_PASSSTATUS);
	CScrollBar *pscBar = (CScrollBar *)GetDlgItem(IDC_DATA_SCBAR);
	SCROLLINFO scrollinfo;
	scrollinfo.nMin = 0;
	scrollinfo.nMax = passEdit->GetLineCount() -2;
	scrollinfo.nPage = 10;	//�y�[�W��
	scrollinfo.nPos = 0;
	scrollinfo.fMask |= SIF_ALL ;//| SIF_DISABLENOSCROLL;
	pscBar->SetScrollInfo( &scrollinfo);

}


void CKemfuncDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	CEdit *passEdit=(CEdit *)GetDlgItem(IDC_PASSSTATUS);
	CEdit *outEdit=(CEdit *)GetDlgItem(IDC_OUTDATA);
	CEdit *compEdit=(CEdit *)GetDlgItem(IDC_COMPDATA);

	int pos, old_pos;
	int maxpos = passEdit->GetLineCount();

	SCROLLINFO scrollinfo;
	pScrollBar->GetScrollInfo( &scrollinfo);

//	TRACE("np %d\n",scrollinfo.nPage);

	old_pos = pos = pScrollBar->GetScrollPos();

	switch(nSBCode){

/*
	SB_TOP  //��ԏ�܂ŃX�N���[��  
	//SB_LINEUP  //1 �s��փX�N���[��  �i�{�^��
	//SB_PAGEUP  //1 �y�[�W��փX�N���[��  �i�V���t�g
	//SB_PAGEDOWN  //1 �y�[�W���փX�N���[��  �i�V���t�g
	//SB_LINEDOWN  //1 �s���փX�N���[�� (�{�^�� 
	SB_BOTTOM://��ԉ��܂ŃX�N���[��
	//SB_ENDSCROLL  //�X�N���[���I��  
	//SB_THUMBTRACK://�X���C�_�[�h���b�O��
	SB_THUMBPOSITION //��Έʒu�փX�N���[��  �i�o�[�������ꂽ
*/

	case SB_PAGEUP:
		pos -= scrollinfo.nPage;
		break;
	case SB_LINEUP:
		pos--;
		break;
	case SB_THUMBTRACK:
		pos = nPos;
		break;
	case SB_LINEDOWN:
		pos++;
		break;
	case SB_PAGEDOWN:
		pos += scrollinfo.nPage;
		break;
	case SB_ENDSCROLL:
		return;
	}

	pScrollBar->SetScrollPos(pos);

	pos = pos - old_pos;

//	TRACE("sc %d\n",pos);

	//Edit1�փX�N���[���o�[�̈ʒu���o��
	passEdit->LineScroll(pos);
	outEdit->LineScroll(pos);
	compEdit->LineScroll(pos);

//	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

/// EOF ///
