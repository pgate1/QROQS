// QROQSDoc.cpp : CQROQSDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "QROQS.h"

#include "QROQSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQROQSDoc


IMPLEMENT_DYNCREATE(CQROQSDoc, CDocument)

BEGIN_MESSAGE_MAP(CQROQSDoc, CDocument)
//{{AFX_MSG_MAP(CQROQSDoc)
ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
ON_COMMAND(ID_FILE_SAVE, OnFileSave)
ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE_ALL, OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, OnUpdateFileSaveAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQROQSDoc クラスの構築/消滅

#include"other.h"

CQROQSDoc::CQROQSDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
//	fprintf(tfp,"CQROQSDoc::CQROQSDoc\n");
//	TRACE("CQROQSDoc::CQROQSDoc\n");

	CQROQSApp* pApp=(CQROQSApp *)AfxGetApp();
	SimSpeed = pApp->GetProfileInt("Simulation","SimSpeed",10);
	Qmode = pApp->GetProfileInt("General","Qmode",false);

#ifndef _LIGHT
	verilog_out_dir_list.push_back(
			pApp->GetProfileString("Verilog","VerilogOutDir0",pApp->exe_dir)
		);
	if(verilog_out_dir_list.back() != pApp->exe_dir){
		CString dlist;
		int i;
		for(i=1;i<10;++i){
			dlist.Format("VerilogOutDir%d",i);
			verilog_out_dir_list.push_back(
				pApp->GetProfileString("Verilog",dlist,pApp->exe_dir)
			);
			if(verilog_out_dir_list.back() == pApp->exe_dir) break;
		}
	}

	verilog_out_sync = pApp->GetProfileInt("Verilog","VerilogOutSync",false);
	verilog_debug = pApp->GetProfileInt("Verilog","VerilogDebug",false);

	c_out_dir_list.push_back(
			pApp->GetProfileString("C_Language","COutDir0",pApp->exe_dir)
		);
	if(c_out_dir_list.back() != pApp->exe_dir){
		CString dlist;
		int i;
		for(i=1;i<10;++i){
			dlist.Format("COutDir%d",i);
			c_out_dir_list.push_back(
				pApp->GetProfileString("C_Language",dlist,pApp->exe_dir)
			);
			if(c_out_dir_list.back() == pApp->exe_dir) break;
		}
	}

	// \r\nを付けたものを取り扱う
	verilog_trafunc_option = pApp->GetProfileString("Verilog","TrafuncCompileOption",
			"set_wire_load_model -name 1k\r\n"
			"max_area 0\r\n"
			"max_delay 0.0 all_outputs()\r\n"
			"compile -map_effort high\r\n"
		);
	verilog_module_option = pApp->GetProfileString("Verilog","ModuleCompileOption",
			"set_wire_load_model -name 1k\r\n"
			"max_area 0\r\n"
			"max_delay 0.0 all_outputs()\r\n"
			"uniquify\r\n"
			"compile -ungroup_all -map_effort high\r\n"
		);
	verilog_trafunc_option.Replace(";","\r\n");
	verilog_module_option.Replace(";","\r\n");
#endif

	docst_ = 0;
	
}



CQROQSDoc::~CQROQSDoc()
{
//	TRACE("doc death\n");

	CWinApp* pApp=AfxGetApp();
	pApp->WriteProfileInt("Simulation","SimSpeed",SimSpeed);
	pApp->WriteProfileInt("General","Qmode",Qmode);

//	TRACE("v %d\n",verilog_out_dir_list.size());

#ifndef _LIGHT
	CString dlist;
	int i;
	for(i=0;i<10 && !verilog_out_dir_list.empty();++i){
		dlist.Format("VerilogOutDir%d",i);
		pApp->WriteProfileString("Verilog",dlist,verilog_out_dir_list.front());
		verilog_out_dir_list.pop_front();
	}

	pApp->WriteProfileInt("Verilog","VerilogOutSync",verilog_out_sync);
	pApp->WriteProfileInt("Verilog","VerilogDebug",verilog_debug);

	for(i=0;i<10 && !c_out_dir_list.empty();++i){
		dlist.Format("COutDir%d",i);
		pApp->WriteProfileString("C_Language",dlist,c_out_dir_list.front());
		c_out_dir_list.pop_front();
	}

	verilog_trafunc_option.Replace("\r\n",";");
	verilog_module_option.Replace("\r\n",";");
	pApp->WriteProfileString("Verilog","TrafuncCompileOption",verilog_trafunc_option);
	pApp->WriteProfileString("Verilog","ModuleCompileOption",verilog_module_option);
#endif

}


/////////////////////////////////////////////////////////////////////////////
// CQROQSDoc シリアライゼーション

void CQROQSDoc::Serialize(CArchive& ar)
{
	return;
//	TRACE("seria\n");
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
		//TRACE("write name:%s\n",ar.GetFile()->GetFilePath());
		
		//ここは使わない
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
		
		//ココも使わない
	}
}

/////////////////////////////////////////////////////////////////////////////
// CQROQSDoc クラスの診断

#ifdef _DEBUG
void CQROQSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CQROQSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQROQSDoc コマンド


//// EOF /////

int CQROQSDoc::SaveModifiedBSP() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	int ret=IDYES;
	map<string,BSPetri*>::iterator bspit;
	bspit = bsp_tree.bsps.begin();
	for(;bspit != bsp_tree.bsps.end();++bspit){
		if(bspit->second->ModifiedFlag){

			//移動
	//		TRACE("modified %s\n",bspit->first.c_str());
			GetModTreeDlg()->SelectModule(bspit->first.c_str());

			ret = AfxMessageBox( (bspit->first + " : 変更されています．\n保存しますか？").c_str(),
				MB_YESNOCANCEL|MB_ICONEXCLAMATION);
			if(ret==IDYES){
				if(OnFileSave()){
					return IDCANCEL;
				}
			}
			else if(ret==IDNO){
			}
			else break;
		}
	}

	return ret;
}

BOOL CQROQSDoc::SaveModified() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(SaveModifiedBSP()==IDCANCEL) return 0;
	return CDocument::SaveModified();
}

/// EOF ///
