// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_)
#define AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B


//#include <afxctl.h> //�ǉ�?
#define _WIN32_WINNT 0x0400	//TrackMouseEvent()�p
//#define _WIN32_WINNT 0x0500	//SetLayeredWindowAttributes()�p

#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#include <afxtempl.h>

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
//#include<afxpriv.h>


#include <algorithm>
#include<time.h>
#include<math.h>

#include<string>
#include<vector>
#include<map>
#include<list>
#include<stack>
#include<set>
#include<deque>

#include <process.h>    /* _beginthread, _endthread */

using namespace std;

#ifndef _LIGHT
#include"Tools/Tools.h"
#include"Tools/StatusBarXP.h"
#include"Tools/ToolBarXP.h"
#include"Tools/Popup.h"
#include"Tools/ButtonXP.h"

//#define USE_NEW_DOCK_BAR
#include"NewMenu/NewMenu.h"
//#include"NewMenu/NewToolBar.h"
#endif

#endif // _AFX_NO_AFXCMN_SUPPORT



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__CC5E6AA6_50A1_4920_8C4C_DD1226946F63__INCLUDED_)

/// EOF ///

