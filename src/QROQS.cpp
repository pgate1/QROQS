// QROQS.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
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
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp �N���X�̍\�z

CQROQSApp::CQROQSApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

//	tfp=fopen("test.txt","w");
//	setbuf(tfp, NULL);//�o�b�t�@���g���
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CQROQSApp �I�u�W�F�N�g

CQROQSApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQROQSApp �N���X�̏�����


OSVERSIONINFO Global_OsVersionInfo;     // OS�o�[�W����
CHAR Global_SystemName[256];            // OS�̖��O

VOID WINAPI Global_GetOsVersion()
{
    // OS�o�[�W����
    ZeroMemory(&Global_OsVersionInfo,sizeof(OSVERSIONINFO));
    Global_OsVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&Global_OsVersionInfo);

    // OS�̖��O
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

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������
	// ��������Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ���
	// ���폜���Ă��������B


#ifdef _AFXDLL
	Enable3dControls();		// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif

//	fprintf(tfp,"CQROQSApp::InitInstance\n");
//	TRACE("CQROQSApp::InitInstance\n");

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͏����ȂǓK�؂Ȃ��̂�
	// �ύX���Ă��������B
// ���W�X�g���ɕۑ�����ꍇ
//	SetRegistryKey(_T("QROQS"));

//	LoadStdProfileSettings();  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)
	//�ǉ�
	// ini�t�@�C�������s�t�@�C���Ɠ����f�B���N�g���ɍ쐬
    char dir[MAX_PATH];
    ::GetModuleFileName(NULL, dir, MAX_PATH);  // ���s�t�@�C���̃p�X���擾
    strrchr(dir, '\\')[1] = '\0';
//	TRACE("%s\n",dir);
	exe_dir = dir;		// exe�̃f�B���N�g��������Ă���
    ::SetCurrentDirectory(dir);                // ���ł̃J�����g�f�B���N�g���ύX
    strcat(dir, _T("QROQS.ini"));                // ini�t�@�C������t�������B
    free((void*)m_pszProfileName);             // �������̉��
    m_pszProfileName = _tcsdup(dir);           // ini�t�@�C���̃p�X��ݒ�
       
	//TRACE("%s\n",m_pszProfileName);

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CQROQSDoc),
		RUNTIME_CLASS(CMainFrame),       // ���C�� SDI �t���[�� �E�B���h�E
		RUNTIME_CLASS(CQROQSView));
	AddDocTemplate(pDocTemplate);

#ifndef _LIGHT
	style_mode = GetProfileInt("App","Style",CNewMenu::STYLE_XP);

	CNewMenu::SetMenuDrawMode(style_mode);

	// NewMenu 1.18
	// ���j���[�̃A�C�R���𖾂邭����
	CNewMenu::SetXpBlendig(false);

	// NewMenu 1.21
	// �A�N�Z�����[�^�L�[��\�����Ȃ�
//	CNewMenu::SetAcceleratorsDraw (false);

#endif

	// DDE Execute open ���g�p�\�ɂ��܂��B
	EnableShellOpen();               // Here!
	RegisterShellFileTypes(TRUE);    // Here!

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	// �E�C���h�E�ʒu�̕���
/*
	// CDocTemplate�o�R�ŃA�N�Z�X
	POSITION posDocTemp = GetFirstDocTemplatePosition();//�܂�CDocTemplate���擾
	CDocTemplate* pDocTemplate = GetNextDocTemplate(posDocTemp);
	POSITION posDoc = pDocTemplate->GetFirstDocPosition( );//����CDocument���擾
	CDocument* pDoc = pDocTemplate->GetNextDoc(posDoc);

//	CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
//	pWnd->InitShowWindow();
*/


	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

//	m_pMainWnd->SetWindowText("QROQS 3.58");//�E�C���h�E�̃^�C�g��

	// OS�o�[�W�����`�F�b�N
	Global_GetOsVersion();



	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifndef _LIGHT
	CButtonXP m_btnOk;
#endif
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_shaine;
	CString	m_Date;
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
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
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CQROQSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

	aboutDlg.m_Date.Format("Last update\r\n%s %s",__TIME__,__DATE__);
	
	if(Qmode){
	aboutDlg.m_shaine.Format(
		"�Ɩ�����	\r\n	SAQAMOTO Hiroqazu\r\n\r\n"
		"�X�[�p�[����	\r\n	QOZASA Taqeshi\r\n\r\n"
		"�����ǂꕔ��	\r\n	NAGAMOTO Taicho\r\n\r\n"
		"����������	\r\n	QAWAJIRI Qensuqe\r\n\r\n"
		"�����ȃp���`�� \r\n	TAQANO Tomoaqi\r\n\r\n"
		"�����ǂ�햱	\r\n	SUENAGA Yujirou\r\n\r\n"
		"�����ǂ�ꖱ	\r\n	SHIBATA Yuithiro\r\n\r\n"
		"�����ǂ�V���c��	\r\n	OGURI Qiyoshi"
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
// CQROQSApp ���b�Z�[�W �n���h��


int CQROQSApp::ExitInstance() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	WriteProfileInt("App","Style",style_mode);

	return CWinApp::ExitInstance();
}

/// EOF ///
