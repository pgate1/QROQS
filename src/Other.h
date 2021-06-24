
#ifndef OTHER_H_INCLUDED
#define OTHER_H_INCLUDED

/*
//メモリ解放用マクロ
#define SAFE_DELETE(p)       { if(p!=NULL) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p!=NULL) { (p)->Release(); (p)=NULL; } }
*/

#define NOTE(s)     AfxMessageBox(s,MB_OK|MB_ICONINFORMATION)
#define YESNO(s)    AfxMessageBox(s,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)
#define OKCANCEL(s) AfxMessageBox(s,MB_OKCANCEL|MB_ICONQUESTION|MB_DEFBUTTON2)
#define ALERT(s)    AfxMessageBox(s,MB_OK|MB_ICONSTOP)


#define M_PI 3.14159265358979

#include "stdafx.h"
#include "QROQS.h"

#include"MainFrm.h"
#include"ModuleTreeDlgBar.h"


void StatusOut(const char*, ...);
void StatusOut(CString);
CString StatusGet();

void Trace(const char*, ...);
void Trace(CString );
void TraceClear();

void ProgSet(int abs,int add=0);

//更新
void BitBltRect(CDC *,CDC *,CRect );


CPoint Betwixtpoint(CPoint ,CPoint ,int ,int );

void StringToList(list<string> &StringList,CString);

void add_carriage(CString &str);
void add_carriage(string &str);


//バージョン
extern OSVERSIONINFO Global_OsVersionInfo;
#define IsWindowsNT()           (Global_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
#define IsWindows()             (Global_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
#define IsWindows_NT40_Later()  (IsWindowsNT() || (Global_OsVersionInfo.dwMajorVersion >= 4))
#define IsWindows_2000_Later()  (IsWindowsNT() || (Global_OsVersionInfo.dwMajorVersion >= 5))
#define IsWindows_95_Later()    (IsWindows() || (Global_OsVersionInfo.dwMinorVersion >= 0))
#define IsWindows_98_Later()    (IsWindows() || (Global_OsVersionInfo.dwMinorVersion > 0))
extern CHAR Global_SystemName[];
#define Global_GetSystemName()      (Global_SystemName)

void DispLastError(void);

//半透明
#define WS_EX_LAYERED	0x80000
#define LWA_COLORKEY	1
#define LWA_ALPHA		2
typedef BOOL (WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
//半透明表示
BOOL SetWindowStyleLayered(HWND hwnd);
BOOL WindowStyleLayered(HWND hwnd);
BOOL SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

void ArcBwtMove(vector<CPoint> *bwtpoint, CPoint mp);

void rect_back(CDC *pDC,LPBYTE lpBMP,int width,int height,COLORREF &basecoat);

CModuleTreeDlgBar* GetModTreeDlg();
CTreeCtrl * GetModuleTree();

void SetClipboardString(const char *st);


typedef BOOL (CALLBACK *LPCALLBACK_MOVEMOUSE)(LONG x,LONG y,LPVOID parm);
BOOL MoveMouse(LONG x,LONG y,LONG pps,LPCALLBACK_MOVEMOUSE cb=NULL,LPVOID parm=0);


int getsetPath(HWND m_hWnd, CString &initPath, CString &getPath);

/*
//multimapでkeyを持つエントリをカウントする
template <class Key,class T>
int bound_count(multimap<Key,T>& m_map,const Key& key) {
	multimap<Key, T>::iterator it;
	it=m_map.lower_bound(key);
	int i;
	for(i=0;it!=m_map.upper_bound(key);it++) i++;
	return i;
}
*/

//データで検索
template<typename Iterator, typename T>
Iterator find_by_value(Iterator first, Iterator last, const T& value) {
  while ( first != last && first->second != value ) ++first;
  return first;
}

CPoint alignPoint(CPoint gp,int chip_size,int hatch_size);

#endif

/// EOF ///

