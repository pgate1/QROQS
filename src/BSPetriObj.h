#ifndef BitSerialPetriNetMethod_H_INCLUDED
#define BitSerialPetriNetMethod_H_INCLUDED


#include"stdafx.h"


class RetEx{
public:
	char *err;
	string mname;
	int err_id, child_err_id;

	RetEx(){
		child_err_id=0;
	};
};

//ここら辺はファイルに保存されるので変更できない
/*
#define	_NONE		0
#define _PLACE		1
#define _TRANSITION	2
#define _UHZI		3
#define _KEMMAKIX	4
#define _MODULE		5

#define _BTOKEN		6
#define _WTOKEN		7
#define _INDET		8
*/
enum{
	_NONE,
	_PLACE,
	_TRANSITION,
	_UHZI,
	_KEMMAKIX,
	_MODULE,

	_BTOKEN,
	_WTOKEN,
	_INDET,

	_ARC,
	_CHAIN,

	_HAND,
	_DELETE,

	_OBJECT,
};


class bspToken{
public:
	int Color;
	int Value;

	bspToken(int color=_BTOKEN,int value=0);
	bspToken(const bspToken &token);
	bspToken & operator = (const bspToken &token);
	~bspToken();
};

class bspArc{

public:
	vector<CPoint> *bwtpoint;
	int In;//vObjListの位置
	int Out;//vObjListの位置
	string value_name;

	bspArc(int in=0,int out=0,string v_name="");
	bspArc(const bspArc &arc);
	bspArc & operator = (const bspArc &arc);

	void SetBwt(vector<CPoint> *bwt);
	~bspArc();
};

class bspObject{
public:

	int type;//オブジェクトタイプ
	CPoint gpoint;//グローバル座標

	bspObject(int obj_type=0, CPoint p=CPoint(0,0));
	bspObject(const bspObject &object);
	bspObject & operator = (const bspObject &object);
	~bspObject();
};

/*
class TMemory{

	vector<vector<int> > mem;

public:
	void Set(const char *name){
		FILE *fp;
		fp=fopen();

		mem.resize(5);
		for(i=0;i<5;i++)
			mem[i].resize(10000);

		mem[0][0]=1;
		mem[1][0]=0;
...
  ..
  ...
  ...

		fclose(fp);
	};
	void GetValue(int *inval, int *outval, int incount,int outcount){
		out = mem[in];
	};
};

  */

class bspPlace{
private:
	bool TokenFlag;//トークンの有無
	int TokenColor;//トークンの色
	int TokenValue;//トークンの値
	int oldcol; //前回通過したトークンの色

public:
	bspPlace *real_place;


#ifndef _LIGHT
	//verilog
	int mid;
	set<int> joint_in_set,//競合グループのプレース群
		joint_out_set;//共有グループのトランジション群
#endif

	bspPlace();//コンストラクタ
	bspPlace(const bspPlace &place);
	bspPlace & operator = (const bspPlace &place);
	void Reset();
	void SetToken(int ,int );//トークンをセット、（色、値）
	void SetOldColor(int col);
	//トークンがあるかどうか　ある:TRUE ない:FALSE

	bool GetTokenFlag();
	int GetTokenColor();//トークンの色を取得  黒:_BTOKEN　白:_WTOKEN
	int GetTokenValue();//トークンの値を取得
	void RemoveToken();//トークンを消費
	void SetTokenValue(int val);
	int GetOldColor();
	void SetTokenColor(int col);
	void SetTokenFlag(bool flag=true);


	bspPlace *GetRealPlace();
	~bspPlace();


void FastSetToken(
		int color,//トークンの色
		int value//トークンの値
		)
{
	TokenColor = color;
	TokenValue = value;
	TokenFlag = true;
	oldcol=color;
}

void FastSetOldColor(int col)
{
	oldcol = col;
}

bool FastGetTokenFlag()
{
	return TokenFlag;
}

int FastGetTokenColor()
{
	return TokenColor;
}

int FastGetTokenValue()
{
	return TokenValue;
}

void FastRemoveToken()
{
	TokenFlag = false;
	oldcol = TokenColor;
}

int FastGetOldColor()
{
	return oldcol;
}
void FastSetTokenColor(int col)
{
	TokenColor = col;
	oldcol = col;
}

void FastSetTokenValue(int val)
{
	TokenValue = val;
}

void FastSetTokenFlag(bool flag)
{
	TokenFlag = flag;
}


};

class bspTransition{

public:
	bool FireableFlag;
	int fired_flag;//発火したフラグ

	string fstring;
	vector<int>
		InPlaceList,//入力プレースのリスト
		OutPlaceList;//出力プレースのリスト
	vector<bspPlace*>
		InPlaces, OutPlaces;
	int incount, outcount;
	char *fspost;
	char *fapost;
	int *inval;//入力値
	int *outval;//出力のため

	map<string,int> in_var,out_var;
	string fs_spcut,fstr,astr;

	int in_place_num,out_place_num;

	// ArcMap から 入出力データ配列(0,1,2...)への変換
	map<int,int> in_arc_to_joint,out_arc_to_joint;
	
	int pass_tra;


//	string tname;
//	TMemory *tmem;


#ifndef _LIGHT
	//verilog
	vector<int> init_value,loopbackout,loopbackin;
	int init_fire,delay;
	int loop_num,non_reg;

	// C
	int guard2id;
	int func2id;
#endif
	

	bspTransition();
	bspTransition(const bspTransition &tran);
	bspTransition & operator = (const bspTransition &tran);
	void CompileFunc(map<int,bspPlace*> &_PlaceMap);
	int ableterm(int);
	bool Fireablecheck();
	bool Fireable();
	BOOL Fire();
	void IOPlaceClear();
	void FuncCheck();
	void CompileFuncA();

	//入出力変数の抽出
	int var_str(const string &str,
			 map<string,int> &in_var,
			 map<string,int> &out_var);
	//スペースやコメントを省く
	int cutsp_str(const string &in_str, string &out_str);
	void FuncEduction();


	string GetInValue(int n);
	string GetOutValue(int n);


	//とにかくチェック
	int AnyFireableCheck();
	//とにかく発火
	void AnyFire(int col);

	~bspTransition();
};


class bspUhzi{
	int oldColor;

public:
	//グラフ情報
	int Pp;//プレースのデータプレースリストにおける位置
	
	//マーキング情報
	int addp;//１：ビット列　０：固定ビット
	list<string> InputString;
	int dline;//流す方向　0：上位（左）1：下位から（右）　
	int In;//固定ビットの時に保持する値
	int bits;	// 2進か16進か

	//内部で使用する
	int *InputData;
	int dmax;
	int ptr;
	list<string>::iterator lptr;
	bool endflag;//ウージのデータが全て終わったら１
	int fired;

	string input_name;

	bspUhzi(int dadd=0,int line=0);
	bspUhzi(const bspUhzi &uhzi);
	bspUhzi & operator = (const bspUhzi &uhzi);
	void Compile();
	void Set();
	BOOL Check(map<int,bspPlace*> &PlaceMap);
	BOOL Industry(map<int,bspPlace*> &PlaceMap);
	void Reset();
	void UndoIndustry();
	void ValuePrint(CDC *, CPoint ,float vdiam, int view_sel, int view_max);
	~bspUhzi();
};

#define KEM_DATA_MAX 64

class bspKemmakix{
	int oldColor;
public:
	//グラフ情報
	int Pp;

	//マーキング情報
	int bits;	// 結果のビット数
	int hex;	// １６進表示するかどうか
	int dline;//受け取る方向　0：上位から（左）1：下位（右）　２：破棄  3:消費しない
	list<string> *compString;

	//内部で使用する
	vector<int> OutputData;
	int dmax;
	int ptr;
	int fired;

	string output_name;

	bspKemmakix(int line=0);
	bspKemmakix(const bspKemmakix &kem);
	bspKemmakix & operator = (const bspKemmakix &kem);
	void Compile();
	BOOL Consum(map<int,bspPlace*> &PlaceMap,int confSimBinValue);
	BOOL Check(map<int,bspPlace*> &PlaceMap);
	void UndoConsum();
	void Reset();
	void ValuePrint(CDC *,CPoint ,float vdiam, int PetMode, int view_max);
	~bspKemmakix();
};



#endif

/// EOF ///
