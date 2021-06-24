// TrafuncDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "QROQS.h"
#include "TrafuncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrafuncDlg ダイアログ


CTrafuncDlg::CTrafuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrafuncDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrafuncDlg)
	m_Trafunc = _T("");
	m_AbletermAcnt = _T("");
	m_FireableStop = FALSE;
	m_Common = FALSE;
	//}}AFX_DATA_INIT

}

void CTrafuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrafuncDlg)
	DDX_Text(pDX, IDC_TRANSITIONFUNC, m_Trafunc);
	DDX_Text(pDX, IDC_ABLETERM, m_AbletermAcnt);
	DDX_Check(pDX, IDC_FIREABLESTOP, m_FireableStop);
	DDX_Check(pDX, IDC_COMMON, m_Common);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON1, m_btnTrans);
	DDX_Control(pDX, IDC_BUTTON2, m_btnCheck);
#endif
}

BEGIN_MESSAGE_MAP(CTrafuncDlg, CDialog)
	//{{AFX_MSG_MAP(CTrafuncDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrafuncDlg メッセージ ハンドラ


#include "other.h"


BOOL CTrafuncDlg::OnInitDialog() 
{

	//半透明用
	SetWindowStyleLayered(GetSafeHwnd());

	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	//透明用最初は完全表示
	if(IsWindows_2000_Later()){
		SetLayeredWindowAttributes(GetSafeHwnd(),0/*無視される*/, 0xff, LWA_ALPHA);
	}
	else{
		SetDlgItemText(IDC_BUTTON1,"透過\n(Win2kのみ)");
		CButton *myButton;
		myButton=(CButton *)GetDlgItem(IDC_BUTTON1);
		myButton->EnableWindow(false);
	}

	bspTransition *m_Tra = m_bsp->TranMap[m_id];

	add_carriage(m_Tra->fstring);

	m_Trafunc = m_Tra->fstring.c_str();

	if(m_Trafunc.IsEmpty()){
		//無くてもいい？
		m_Trafunc="//Pass req only.\r\n";
	}

	if(m_bsp->simTranFireableStop.find(m_id) != m_bsp->simTranFireableStop.end()){
		m_FireableStop=1;
	}

	UpdateData(false);

	if(PetMode==_SIMULATION){
		CEdit *myED;
		myED=(CEdit *)GetDlgItem(IDC_TRANSITIONFUNC);
		myED->SetReadOnly(true);
	}


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CTrafuncDlg::OnOK()
{
	// TODO: この位置にその他の検証用のコードを追加してください
	
	UpdateData(true);

	if(PetMode==_EDIT){

//		TRACE("name %s\n",m_mname.c_str());
		m_bsp = m_bspt->GetBSP(m_mname.c_str());
		if(m_bsp==NULL) goto ON_OK;
//TRACE("up\n");
		if(m_bsp->TranMap.find(m_id)==m_bsp->TranMap.end()) goto ON_OK;
		bspTransition *m_Tra = m_bsp->TranMap[m_id];

		m_bsp->ModifiedFlag = 1;
		m_Tra->fstring = (LPCTSTR)m_Trafunc;

//		TRACE("%s\n",m_Trafunc);


		try{
			m_Tra->FuncEduction();

			//アークの変数をリセット
			m_bsp->ArcValReset(m_id);
		}
		catch(char *err){
			err;
//			MessageBox(err,"",NULL);
//			return;
		}

		if(m_Common){
		//	TRACE("common\n");
			map<int,bspTransition*>::iterator tit,tit_end;
			tit = m_bsp->TranMap.begin();
			tit_end = m_bsp->TranMap.end();
			for(;tit!=tit_end;++tit){
				if( m_bsp->intrack.find( tit->first ) == m_bsp->intrack.end()) continue;
				*(tit->second) = *m_Tra;

				if(tit->first != m_id){
					try{
						tit->second->FuncEduction();
						m_bsp->ArcValReset(tit->first);
					}
					catch(char *err){err;}
				}

			}
		}

	}

	if(m_FireableStop==1){
		m_bsp->simTranFireableStop.insert(m_id);
	}
	else{
		m_bsp->simTranFireableStop.erase(m_id);
	}

ON_OK:
	GetParent()->Invalidate();
	CDialog::OnOK();
}


void CTrafuncDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	delete this;

	CDialog::PostNcDestroy();
}

/*
発火可能条件の評価
*/
void CTrafuncDlg::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください


	if(IsWindows_2000_Later()){
		//透明用
		static int fshow=1;
		if(fshow){
			SetLayeredWindowAttributes(GetSafeHwnd(),0//無視される
				, 150, LWA_ALPHA);
			SetDlgItemText(IDC_BUTTON1,"透過解除");
			fshow=0;
		}
		else{
			SetLayeredWindowAttributes(GetSafeHwnd(),0//無視される
				, 0xff, LWA_ALPHA);
			SetDlgItemText(IDC_BUTTON1,"透過する");
			fshow=1;
		}
	}


}


void CTrafuncDlg::OnButton2()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	UpdateData(true);

	bspTransition tra;
	tra.fstring = (LPCTSTR)m_Trafunc;
	try{
		tra.FuncCheck();
		m_AbletermAcnt = "All green.\r\n";

		map<string,int>::iterator vit;
		m_AbletermAcnt += "input.\r\n";
		vit = tra.in_var.begin();
		for(;vit!=tra.in_var.end();++vit){
			m_AbletermAcnt += "	";
			m_AbletermAcnt += vit->first.c_str();
			m_AbletermAcnt += "\r\n";
		}
		m_AbletermAcnt += "output.\r\n";
		vit = tra.out_var.begin();
		for(;vit!=tra.out_var.end();++vit){
			m_AbletermAcnt += "	";
			m_AbletermAcnt += vit->first.c_str();
			m_AbletermAcnt += "\r\n";
		}

	}
	catch(char *err){
//		MessageBox(err,"",NULL);
		m_AbletermAcnt.Format("%s",err);
	}

	UpdateData(false);
}


/// EOF ///

