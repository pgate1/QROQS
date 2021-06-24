// Compile.h: CCompile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_)
#define AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

#define I			'i'
#define LPAR		'('
#define RPAR		')'
#define NOT			'~'
#define AND			'&'
#define XOR			'^'
#define OR			'|'
#define NEXT		'\n'
#define SPACE		' '
#define ZERO		'0'
#define BUFFER_SIZE	1000
#define MOJI_SIZE	200
#define ATAI_SIZE	99

typedef struct Enzansi{
	int c;
	int pri;
} Enzansi;


typedef enum Type{
	START,		/* zero */
	END,		/* NULL */
	FIN,		/* ある関数の終わり */
	ID,			/* 識別子 */
	NUM,		/* 数字 */
	OPN,		/* NOT */
	OP, 		/* 演算子 */
	OPL, 		/* 左括弧 */
	OPR, 		/* 右括弧 */
} Type;

typedef struct Moji{
	int c;
	int op;
	Type type;
} Moji;

class CCompile  
{
public:
	CCompile();

	//	構文チェックと後置記法への変換
	void CCompile::Kaiseki(const char *fstring, char *fspost, int n, int r_add=0);

	//	iナニ？
	int CCompile::Inplacenumber(const char *buffer, int m);

	//	文字の識別
	Moji CCompile::GetMoji(char c);

	//	文字の操作
	void CCompile::Parse(Moji t, char *fspost);

	//	演算子の操作
	void CCompile::ParseOp(int opnum, char *fspost);

	//stackからデータをPopする
	int CCompile::Pop();

	//stackの先頭の値を取得する
	int CCompile::Current();

	//	関数計算とトークンのセット
	void CCompile::Keisan(const char *fspost, int *i, int *o, int n, int m);

	//	演算
	void CCompile::Compute(const char *fspost, Moji t, int *atai);

	//　式中で使用された入力プレースの数を返す関数
	int CCompile::UsedPlaceNumber(const char *fstring);

stack<int>	stack;

Enzansi enzansi[6];

int p;
int q;
int keta;		// Inplacenumberの返り値の桁数
int s;

int row_add;

char errormoji[MOJI_SIZE];

	virtual ~CCompile();

};




#endif // !defined(AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_)
