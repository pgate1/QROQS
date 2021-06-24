#include "stdafx.h" 
#include "QROQS.h"

#include"other.h"

void StatusOut(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char *buf = new char[1024];
	if(vsprintf(buf,format,ap)==EOF){
		*buf='\0';
	}
	va_end(ap);
	CString s;
	s.Format("%s", buf);
	delete[] buf;
//	TRACE("%s\n",s);
	StatusOut(s);
}

void StatusOut(CString s)
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

	pStatusBar->SetPaneText(0, s);    // メッセージ区画(インデックスは０番)

}

/*
CString StatusGet()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatusBar = (CStatusBar*)pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);

	CString s;
	pStatusBar->GetPaneText(0, s);    // メッセージ区画(インデックスは０番)

	return s;
}
*/
void Trace(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char *buf = new char[1024];
	if(vsprintf(buf,format,ap)<0){
		TRACE("Traceでの出力ﾊﾞｯﾌｧｵｰﾊﾞｰﾌﾛｰ:内部エラー\n");
		delete[] buf;
		return;
	}
	va_end(ap);
	CString s;
	s.Format("%s", buf);
	delete[] buf;
//	TRACE("%s\n",s);
	Trace(s);
}

#include "TraceDlg.h"
#include "QROQSView.h"

void Trace(CString s)
{
	if(s.IsEmpty()) return;

	if(s.GetAt( s.GetLength()-1 ) != '\n'){
		s += "\n";
	}

	add_carriage(s);

	CEdit *ed;
	
	CQROQSView *pView = (CQROQSView*)(((CMainFrame*)::AfxGetMainWnd())->GetActiveView());
	if(pView==NULL) return;
	CTraceDlg* poDlg = pView->m_wndTraceDlg;
	if(poDlg==NULL) return;
	poDlg->m_Output += s;
	poDlg->UpdateData(false);
	ed = (CEdit*)poDlg->GetDlgItem(IDC_OUTPUTMSG);
	ed->LineScroll(9999);	//最後までスクロール
	ed->LineScroll(-1);
	poDlg->ShowWindow(true);

	pView->SetFocus();

}

void TraceClear()
{

	CTraceDlg* poDlg = ((CQROQSView*)(((CMainFrame*)::AfxGetMainWnd())->GetActiveView()))->m_wndTraceDlg;
	if(poDlg==NULL) return;
	poDlg->ShowWindow(false);
	poDlg->m_Output.Empty();
	poDlg->UpdateData(false);
}

void ProgSet(int abs,int add)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(add==0) pFrame->GetProgress()->SetPos(abs);
	else pFrame->GetProgress()->OffsetPos(add);
}

//更新
void BitBltRect(CDC *toDC, CDC *fromDC, CRect rect)
{
	toDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		fromDC, rect.left, rect.top, SRCCOPY);
}


CPoint Betwixtpoint(CPoint inp, CPoint outp, int edge, int len)
{
//	TRACE("bwt point \n");
	CPoint p;
	double m, n=0;
	//  double pai=3;/*ゆとり教育*/
	if(edge==0)
	{
		p.x = outp.x;
		p.y = outp.y;
		outp.x = inp.x;
		outp.y = inp.y;
		inp.x = p.x;
		inp.y = p.y;
	}
	if(inp.x < outp.x)
    {
		if(outp.x != inp.x)
			n = (double)(inp.y-outp.y)/(outp.x-inp.x);
		m = atan(n);
		p.x = (int)(outp.x-len*cos(m));
		p.y = (int)(outp.y+len*sin(m));
    }
	else if(inp.x > outp.x)
    {
		if(outp.x != inp.x)
			n = (double)(inp.y-outp.y)/(-outp.x+inp.x);
		m = atan(n);
		p.x = (int)(outp.x+len*cos(m));
		p.y = (int)(outp.y+len*sin(m));
    }
	else{
		if(outp.y > inp.y) 
			p.y = outp.y - len;
		else 
			p.y = outp.y + len;
		p.x = inp.x;
	}
	return p;
}

void StringToList(list<string> &clist, CString cs)
{
	clist.clear();

//TRACE("ddd");
	if(cs.IsEmpty()) return;
//TRACE("sss");
	char *is = new char[cs.GetLength()+1];
	int c1,c2;
	strcpy(is, (LPCTSTR)cs);

	c1=0; c2=0;
	while(c1 < cs.GetLength()){
		while(is[c1]=='\r' || is[c1]=='\n') c1++;
		if(is[c1]=='\0') break;
		c2=c1;
		while(is[c2]!='\r' && is[c2]!='\0') c2++;
		is[c2]='\0';
		clist.push_back(is+c1);
		c1=c2+2;
		//	TRACE("%d,%d\n",c1,UhziList.GetAt(p).InputString.GetLength());
	}
	delete[] is;
}


// =============================================
// DispLastError
// 概要  : GetLastError() で取得できるエラー内容を
//       : 文字列でダイアログに表示する関数
// 引数  : none
// 戻り値: none
// =============================================
void DispLastError(void)
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                  FORMAT_MESSAGE_FROM_SYSTEM | 
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
                  (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (const char *)lpMsgBuf, "結果表示", MB_OK);
    LocalFree(lpMsgBuf);
}


//carriageリターンを追加する
void add_carriage(CString &str)
{
	str.Remove('\r');
	str.Replace("\n","\r\n");
}

//carriageリターンを追加する
void add_carriage(string &str)
{
	int i;
	i=0;
	while((i=str.find('\n',i))!=str.npos){
		if(i==0 || str[i-1]!='\r'){
			str.insert(i,1,'\r');
			i+=2;
		}
		else ++i;
	}
}

/*
		SetWindowLong(m_wndTraceOutDlg->GetSafeHwnd(), GWL_EXSTYLE,
			GetWindowLong(m_wndTraceOutDlg->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_wndTraceOutDlg->GetSafeHwnd(), 0, 0, LWA_ALPHA);
  */

//ウィンドウ・スタイルをレイヤードウィンドウに変更する関数
BOOL SetWindowStyleLayered(HWND hwnd)
{
	if(!IsWindows_2000_Later()) return false;

//TRACE("win %d\n",IsWindows_2000_Later());
	BOOL ret = FALSE;

//	HMODULE hModule = LoadLibrary("user32.dll");
//	if ( hModule ) {
//		SLWA slwa = (SLWA)GetProcAddress(hModule, "SetLayeredWindowAttributes");
//		if ( slwa ) {
			SetWindowLong(hwnd, GWL_EXSTYLE,
				GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED
				);
			ret = TRUE;
//		}
//		FreeLibrary(hModule);
//	}
	return ret;
}
/*
//レイヤードウィンドウかどうかを確認する関数
BOOL WindowStyleLayered(HWND hwnd)
{
	return ((GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED) != 0) ? TRUE : FALSE;
}
*/

//レイヤードウィンドウの透明度を設定する関数
BOOL SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	if(!IsWindows_2000_Later()) return false;

	BOOL ret = FALSE;

	HMODULE hModule = LoadLibrary("user32.dll");
	if ( hModule ) {
		SLWA slwa = (SLWA)GetProcAddress(hModule, "SetLayeredWindowAttributes");
		if ( slwa ) {
			ret = slwa(hwnd, crKey, bAlpha, dwFlags);
		}
		FreeLibrary(hModule);
	}
	return ret;
}

void ArcBwtMove(vector<CPoint> *bwtpoint, CPoint mp)
{
	int i;
	for(i=0;i<bwtpoint->size();++i){
		(*bwtpoint)[i] += mp;
	}
}

#ifndef _LIGHT
inline void fillrect(CDC *pDC,int iWidth,int iHeight,int x1,int y1,int x2,int y2,COLORREF col);
inline void shadowrect(LPBYTE lpBMP,int iWidth,int iHeight,int x1,int y1,int x2,int y2);
inline void uprect(CDC *pDC,int iWidth,int iHeight,int x1,int y1,int x2,int y2,int r, int g, int b);

void rect_back(CDC *pDC,LPBYTE lpBMP,int width,int height,COLORREF &basecoat)
{

	int br,bg,bb;
	int i,x1,y1,x2,y2,si,r,g,b;

	br=rand()%60+190;
	bg=rand()%60+190;
	bb=rand()%60+190;

	r=br+20; if(r>0xff) r=0xff;
	g=bg+20; if(g>0xff) g=0xff;
	b=bb+20; if(b>0xff) b=0xff;
	basecoat = RGB(r,g,b);

	pDC->FillSolidRect(0,0,width,height,RGB(br,bg,bb));
//	pDC->FillSolidRect(0,0,width,height,RGB(255,255,255));

	int sd=rand()%40+20;

	for(i=0;i<200;i++){
		//チップの座標
		x1=rand()%width;
		y1=rand()%height;
		//チップの大きさ
		x2=x1+rand()%30+20;
		y2=y1+rand()%30+20;

		//チップの色
		si=rand()%40-30;

		r=si+br+rand()%sd; if(r>0xff) r=0xff; else if(r<0) r=0;
		g=si+bg+rand()%sd; if(g>0xff) g=0xff; else if(g<0) g=0;
		b=si+bb+rand()%sd; if(b>0xff) b=0xff; else if(b<0) b=0;

		fillrect(pDC,width,height,x1,y1,x2,y2,RGB(r,g,b));
		uprect(pDC,width,height,x1,y1,x2,y2,r,g,b);
		shadowrect(lpBMP,width,height,x1,y1,x2,y2);
	}
}

inline void fillrect(CDC *pDC,int iWidth,int iHeight,int x1,int y1,int x2,int y2,COLORREF col)
{
	int x,y,x3,y3;

	x3=x2-iWidth;
	y3=y2-iHeight;
	if(x2<0) x2=0;
	else if(x2>iWidth) x2=iWidth;
	if(y2<0) y2=0;
	else if(y2>iHeight) y2=iHeight;

	//通常の四角
	for(y=y1;y<y2;y++){
		for(x=x1;x<x2;x++)	pDC->SetPixel(x,y,col);
		for(x=0;x<x3;x++)	pDC->SetPixel(x,y,col);
	}
	for(y=0;y<y3;y++){
		for(x=x1;x<x2;x++)	pDC->SetPixel(x,y,col);
		for(x=0;x<x3;x++)	pDC->SetPixel(x,y,col);
	}
	return;
}
inline void uprect(CDC *pDC,int iWidth,int iHeight,int x1,int y1,int x2,int y2,int r, int g, int b)
{
	int x,y,x3,y3,rr,gg,bb;

	x3=x2-iWidth;
	y3=y2-iHeight;
	if(x2<0) x2=0;
	else if(x2>iWidth) x2=iWidth;
	if(y2<0) y2=0;
	else if(y2>iHeight) y2=iHeight;

	rr=r-10;
	if(rr<0) rr=0;
	gg=g-10;
	if(gg<0) gg=0;
	bb=b-10;
	if(bb<0) bb=0;
	COLORREF col;
	col=RGB(rr,gg,bb);
	
	//下影
	y=y3-1<0?y2-1:y3-1;
	for(x=x1;x<x2;x++)	pDC->SetPixel(x,y,col);
	for(x=0;x<x3-1;x++)	pDC->SetPixel(x,y,col);
	//右影
	x=x3-1<0?x2-1:x3-1;
	for(y=y1;y<y2-1;y++)	pDC->SetPixel(x,y,col);
	for(y=0;y<y3-1;y++)		pDC->SetPixel(x,y,col);

	rr=r+10;
	if(rr>255) rr=255;
	gg=g+10;
	if(gg>255) gg=255;
	bb=b+10;
	if(bb>255) bb=255;

	col =RGB(rr,gg,bb);
	//上光
	y=y1;
	for(x=x1;x<x2;x++)	pDC->SetPixel(x,y,col);
	for(x=0;x<x3;x++)	pDC->SetPixel(x,y,col);
	//左光
	x=x1;
	for(y=y1;y<y2;y++)	pDC->SetPixel(x,y,col);
	for(y=0;y<y3;y++)	pDC->SetPixel(x,y,col);

	return;
}
inline void shadowrect(LPBYTE image,int iWidth,int iHeight,int x1,int y1,int x2,int y2)
{
	int x,y,x3,y3,i,j,m,n,s;

	s=20;//初期
	m=4;//段数
	n=7;//速度

	x3=x2-iWidth;
	y3=y2-iHeight;
	//影

	struct func{
		static void pcalc(LPBYTE image,int pos,int j){
			image[pos]-=j;
			image[pos+1]-=j;
			image[pos+2]-=j;
		}
	};

#define PIXEL 3

	//下
	j=s;
	i=1;
	for(y=y3<0?y2:y3;i<m&&y<iHeight;i++,y++){
		for(x=x1+i;x<x2+i&&x<iWidth;x++)	func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		for(x=0;x<x3+i;x++)					func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		j-=n;
	}
	for(y=0;i<m;i++,y++){
		for(x=x1+i;x<x2+i&&x<iWidth;x++)	func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		for(x=0;x<x3+i;x++)					func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		j-=n;
	}

	//右
	j=s;
	i=1;
	for(x=x3<0?x2:x3;i<m&&x<iWidth;i++,x++){
		for(y=y1+i;y<y2+i-1&&y<iHeight;y++)	func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		for(y=0;y<y3+i-1;y++)				func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		j-=n;
	}
	for(x=0;i<m;i++,x++){
		for(y=y1+i;y<y2+i-1&&y<iHeight;y++)	func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		for(y=0;y<y3+i-1;y++)				func::pcalc(image,(y*iWidth+x)*PIXEL,j);
		j-=n;
	}

	return;
}
#endif // non _LIGHT

CModuleTreeDlgBar* GetModTreeDlg()
{
	return ((CMainFrame*)::AfxGetMainWnd())->GetModuleTreeDlgBar();
}

CTreeCtrl * GetModuleTree()
{
	CModuleTreeDlgBar* pDlgBar = ((CMainFrame*)::AfxGetMainWnd())->GetModuleTreeDlgBar();
	return (CTreeCtrl *)pDlgBar->GetDlgItem(IDC_MODTREE);
}


//文字列をクリップボードへコピーする  
void SetClipboardString(const char *st)
{
	HANDLE hMem;
	LPVOID lpVoid;
	BOOL rc = OpenClipboard(0);
	if( rc == FALSE ) {
		;	//　エラー処理
	}
	else {
		EmptyClipboard();
		hMem = ::GlobalAlloc( GHND, strlen( st ) + 1 );
		lpVoid = ::GlobalLock( hMem );
		strcpy( ( char* )lpVoid, st );
		::SetClipboardData( CF_TEXT, hMem );
		::GlobalUnlock( hMem );
		::CloseClipboard();
	}
}


// コールバック関数のプロトタイプ
//typedef BOOL (CALLBACK *LPCALLBACK_MOVEMOUSE)(LONG x,LONG y,LPVOID parm);
//-----------------------------------------------------------------------------
// マウス移動
//-----------------------------------------------------------------------------
// BOOL MoveMouse(
//   LONG x,                    // 移動位置X
//   LONG y,                    // 移動位置Y
//   LONG pps,                  // 移動速度(毎秒ピクセル数)
//   LPCALLBACK_MOVEMOUSE cb,   // コールバック関数のポインタ
//   LPVOID parm                // コールバック関数のパラメータ
// );
//
// コールバック関数へは、座標(x,y)と任意のパラメータ(parm)が渡される。
// 関数の返値がFALSEのときこの関数は強制終了し、返値はFALSEとなる。
//-----------------------------------------------------------------------------
//BOOL MoveMouse(LONG x,LONG y,LONG pps,LPCALLBACK_MOVEMOUSE cb=NULL,LPVOID parm=0)
BOOL MoveMouse(LONG x,LONG y,LONG pps,LPCALLBACK_MOVEMOUSE cb,LPVOID parm)
{
    POINT point;
    LONG xx,yy,mov,movtimes,timorg,timwrk,mx,my;

    // 現在の位置を取得し、各軸の移動距離と斜め移動距離を求める
    GetCursorPos(&point);
    xx = x - point.x;
    yy = y - point.y;
    mov = (LONG)sqrt(xx * xx + yy * yy);

    // 移動時間を求める
    movtimes = MulDiv(1000,mov,pps);

    // 移動ルーチン
    timorg = (LONG)GetTickCount();
    do{
        // 移動位置を計算
        if((timwrk = (LONG)GetTickCount() - timorg) >= movtimes){
            mx = x;
            my = y;
        }else{
            mx = point.x + MulDiv(xx,timwrk,movtimes);
            my = point.y + MulDiv(yy,timwrk,movtimes);
        }

        // マウス移動
        SetCursorPos(mx,my);

        // コールバック呼び出し
        if(cb != NULL){if(!cb(mx,my,parm))return FALSE;}
    }while(timwrk < movtimes);

    return TRUE;
}

int CALLBACK BrowseCallbackProc(
    HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    switch(uMsg)
    {
		//初期選択フォルダ
    case BFFM_INITIALIZED:
        {
			if( lpData != NULL){
				SendMessage( hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData );
			}
        }
        break;

    case BFFM_SELCHANGED:
		//選択時にパスを表示
        {
            TCHAR szText[MAX_PATH] = {0};
            SHGetPathFromIDList(
                reinterpret_cast<LPITEMIDLIST>(lParam), szText);
            SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0,
                reinterpret_cast<LPARAM>(szText));
        }
        break;
    }

    return 0;
}

int getsetPath(HWND hWnd, CString &initPath, CString &getPath)
{
	TCHAR szBuff[1024];
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
    
	bi.hwndOwner = hWnd;

	bi.pidlRoot = NULL;
	bi.pszDisplayName = szBuff;
	bi.lpszTitle = _T("フォルダを選択してください");
	bi.ulFlags = BIF_RETURNONLYFSDIRS| BIF_STATUSTEXT;

	TCHAR szRootDir[1024];

	// 最後に\があるとうまく動作しない
//	lstrcpy(szRootDir,"A:\\MyDocument\\test");
	lstrcpy(szRootDir, (LPCTSTR)initPath );

//	TRACE("szRootDir %s\n",szRootDir);

	bi.lParam = (LPARAM)szRootDir;
	bi.iImage = 0;
	bi.lpszTitle = _T("Current folder is:");
    bi.lpfn = BrowseCallbackProc;

	LPITEMIDLIST pList = ::SHBrowseForFolder(&bi);
	if (pList) {
	   	if (::SHGetPathFromIDList(pList, szBuff)) {
			//szBuffに選択したフォルダ名が入る
			getPath = szBuff;
			return 1;
		}
	}
	return 0;
}


/*
オブジェクトの整列
グローバル座標による
*/
CPoint alignPoint(CPoint gp,int chip_size,int hatch_size)
{
	int half=hatch_size>>1;

	if(gp.x>=0){
		gp.x += -(gp.x % hatch_size) +half;
	}
	else{
		gp.x += -(gp.x % hatch_size) -half;
	}

	if(gp.y>=0){
		gp.y += -(gp.y % hatch_size) +half;
	}
	else{
		gp.y += -(gp.y % hatch_size) -half;
	}

	return gp;
}

/// EOF ///

