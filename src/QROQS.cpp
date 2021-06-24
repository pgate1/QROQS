// QROQS.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"

#define GLOBAL_QROQS_VALUE_DEFINE
#include "QROQS.h"

#include "MainFrm.h"
#include "QROQSDoc.h"
#include "QROQSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp

BEGIN_MESSAGE_MAP(CQROQSApp, CWinApp)
	//{{AFX_MSG_MAP(CQROQSApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp クラスの構築

CQROQSApp::CQROQSApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。

//	tfp=fopen("test.txt","w");
//	setbuf(tfp, NULL);//バッファを使わん
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CQROQSApp オブジェクト

CQROQSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp クラスの初期化


OSVERSIONINFO Global_OsVersionInfo;     // OSバージョン
CHAR Global_SystemName[256];            // OSの名前

VOID WINAPI Global_GetOsVersion()
{
    // OSバージョン
    ZeroMemory(&Global_OsVersionInfo,sizeof(OSVERSIONINFO));
    Global_OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&Global_OsVersionInfo);

    // OSの名前
    switch(Global_OsVersionInfo.dwPlatformId){
    case VER_PLATFORM_WIN32s:
        wsprintf(Global_SystemName,"Windows3.1");
        break;
    case VER_PLATFORM_WIN32_WINDOWS:
        if(Global_OsVersionInfo.dwMajorVersion == 4){
            if(Global_OsVersionInfo.dwMinorVersion == 0){
                wsprintf(Global_SystemName,"Windows95");
            }else if(Global_OsVersionInfo.dwMinorVersion == 1){
                wsprintf(Global_SystemName,"Windows98");
            }else{
                wsprintf(Global_SystemName,"Later Windows98");
            }
        }else{
            wsprintf(Global_SystemName,"Windows %d.%d",
                Global_OsVersionInfo.dwMajorVersion,
                Global_OsVersionInfo.dwMinorVersion);
        }
        break;
    case VER_PLATFORM_WIN32_NT:
        if(Global_OsVersionInfo.dwMajorVersion == 5){
            wsprintf(Global_SystemName,"Windows2000");
        }else{
            wsprintf(Global_SystemName,"WindowsNT %d.%d",
                Global_OsVersionInfo.dwMajorVersion,
                Global_OsVersionInfo.dwMinorVersion);
        }
        break;
    }
}


BOOL CQROQSApp::InitInstance()
{
	AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。


#ifdef _AFXDLL
	Enable3dControls();		// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

//	fprintf(tfp,"CQROQSApp::InitInstance\n");
//	TRACE("CQROQSApp::InitInstance\n");

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
// レジストリに保存する場合
//	SetRegistryKey(_T("QROQS"));

//	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)
	//追加
	// iniファイルを実行ファイルと同じディレクトリに作成
    char dir[MAX_PATH];
    ::GetModuleFileName(NULL, dir, MAX_PATH);  // 実行ファイルのパスを取得
    strrchr(dir, '\\')[1] = '\0';
//	TRACE("%s\n",dir);
	exe_dir = dir;		// exeのディレクトリを取っておく
    ::SetCurrentDirectory(dir);                // ついでのカレントディレクトリ変更
    strcat(dir, _T("QROQS.ini"));                // iniファイル名を付け足す。
    free((void*)m_pszProfileName);             // メモリの解放
    m_pszProfileName = _tcsdup(dir);           // iniファイルのパスを設定
       
	//TRACE("%s\n",m_pszProfileName);

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CQROQSDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CQROQSView));
	AddDocTemplate(pDocTemplate);

#ifndef _LIGHT
	style_mode = GetProfileInt("App","Style",CNewMenu::STYLE_XP);

	CNewMenu::SetMenuDrawMode(style_mode);

	// NewMenu 1.18
	// メニューのアイコンを明るくする
	CNewMenu::SetXpBlendig(false);

	// NewMenu 1.21
	// アクセラレータキーを表示しない
//	CNewMenu::SetAcceleratorsDraw (false);

#endif

	// DDE Execute open を使用可能にします。
	EnableShellOpen();               // Here!
	RegisterShellFileTypes(TRUE);    // Here!

	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// コマンドラインでディスパッチ コマンドを指定します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// ウインドウ位置の復元
/*
	// CDocTemplate経由でアクセス
	POSITION posDocTemp = GetFirstDocTemplatePosition();//まずCDocTemplateを取得
	CDocTemplate* pDocTemplate = GetNextDocTemplate(posDocTemp);
	POSITION posDoc = pDocTemplate->GetFirstDocPosition( );//次にCDocumentを取得
	CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);

//	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
//	pWnd->InitShowWindow();
*/


	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

//	m_pMainWnd->SetWindowText("QROQS 3.58");//ウインドウのタイトル

	// OSバージョンチェック
	Global_GetOsVersion();



	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
#ifndef _LIGHT
	CButtonXP m_btnOk;
#endif
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_shaine;
	CString	m_Date;
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_shaine = _T("");
	m_Date = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_SHAINE, m_shaine);
	DDX_Text(pDX, IDC_EDIT1, m_Date);
	//}}AFX_DATA_MAP
#ifndef _LIGHT
	DDX_Control(pDX, IDOK, m_btnOk);
#endif
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CQROQSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

	aboutDlg.m_Date.Format("Last update\r\n%s %s",__TIME__,__DATE__);
	
	if(Qmode){
	aboutDlg.m_shaine.Format(
		"業務部長	\r\n	SAQAMOTO Hiroqazu\r\n\r\n"
		"スーパー部長	\r\n	QOZASA Taqeshi\r\n\r\n"
		"酔いどれ部長	\r\n	NAGAMOTO Taicho\r\n\r\n"
		"激しい部長	\r\n	QAWAJIRI Qensuqe\r\n\r\n"
		"愉快なパンチョ \r\n	TAQANO Tomoaqi\r\n\r\n"
		"酔いどれ常務	\r\n	SUENAGA Yujirou\r\n\r\n"
		"酔いどれ専務	\r\n	SHIBATA Yuithiro\r\n\r\n"
		"酔いどれシャツ王	\r\n	OGURI Qiyoshi"
		);
	}
	else{
	aboutDlg.m_shaine.Format(
		"SAKAMOTO Hirokazu\r\n\r\n"
		"KOZASA Takeshi\r\n\r\n"
		"NAGAMOTO Taichi\r\n\r\n"
		"KAWAJIRI Kensuke\r\n\r\n"
		"TAKANO Tomoaki\r\n\r\n"
		"SUENAGA Yujirou\r\n\r\n"
		"SHIBATA Yuichiro\r\n\r\n"
		"OGURI Kiyoshi"
		);
	}

	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp メッセージ ハンドラ


int CQROQSApp::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	WriteProfileInt("App","Style",style_mode);

	return CWinApp::ExitInstance();
}

/// EOF ///
