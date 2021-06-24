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

//������ӂ̓t�@�C���ɕۑ������̂ŕύX�ł��Ȃ�
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
	int In;//vObjList�̈ʒu
	int Out;//vObjList�̈ʒu
	string value_name;

	bspArc(int in=0,int out=0,string v_name="");
	bspArc(const bspArc &arc);
	bspArc & operator = (const bspArc &arc);

	void SetBwt(vector<CPoint> *bwt);
	~bspArc();
};

class bspObject{
public:

	int type;//�I�u�W�F�N�g�^�C�v
	CPoint gpoint;//�O���[�o�����W

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
	bool TokenFlag;//�g�[�N���̗L��
	int TokenColor;//�g�[�N���̐F
	int TokenValue;//�g�[�N���̒l
	int oldcol; //�O��ʉ߂����g�[�N���̐F

public:
	bspPlace *real_place;


#ifndef _LIGHT
	//verilog
	int mid;
	set<int> joint_in_set,//�����O���[�v�̃v���[�X�Q
		joint_out_set;//���L�O���[�v�̃g�����W�V�����Q
#endif

	bspPlace();//�R���X�g���N�^
	bspPlace(const bspPlace &place);
	bspPlace & operator = (const bspPlace &place);
	void Reset();
	void SetToken(int ,int );//�g�[�N�����Z�b�g�A�i�F�A�l�j
	void SetOldColor(int col);
	//�g�[�N�������邩�ǂ����@����:TRUE �Ȃ�:FALSE

	bool GetTokenFlag();
	int GetTokenColor();//�g�[�N���̐F���擾  ��:_BTOKEN�@��:_WTOKEN
	int GetTokenValue();//�g�[�N���̒l���擾
	void RemoveToken();//�g�[�N��������
	void SetTokenValue(int val);
	int GetOldColor();
	void SetTokenColor(int col);
	void SetTokenFlag(bool flag=true);


	bspPlace *GetRealPlace();
	~bspPlace();


void FastSetToken(
		int color,//�g�[�N���̐F
		int value//�g�[�N���̒l
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
	int fired_flag;//���΂����t���O

	string fstring;
	vector<int>
		InPlaceList,//���̓v���[�X�̃��X�g
		OutPlaceList;//�o�̓v���[�X�̃��X�g
	vector<bspPlace*>
		InPlaces, OutPlaces;
	int incount, outcount;
	char *fspost;
	char *fapost;
	int *inval;//���͒l
	int *outval;//�o�͂̂���

	map<string,int> in_var,out_var;
	string fs_spcut,fstr,astr;

	int in_place_num,out_place_num;

	// ArcMap ���� ���o�̓f�[�^�z��(0,1,2...)�ւ̕ϊ�
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

	//���o�͕ϐ��̒��o
	int var_str(const string &str,
			 map<string,int> &in_var,
			 map<string,int> &out_var);
	//�X�y�[�X��R�����g���Ȃ�
	int cutsp_str(const string &in_str, string &out_str);
	void FuncEduction();


	string GetInValue(int n);
	string GetOutValue(int n);


	//�Ƃɂ����`�F�b�N
	int AnyFireableCheck();
	//�Ƃɂ�������
	void AnyFire(int col);

	~bspTransition();
};


class bspUhzi{
	int oldColor;

public:
	//�O���t���
	int Pp;//�v���[�X�̃f�[�^�v���[�X���X�g�ɂ�����ʒu
	
	//�}�[�L���O���
	int addp;//�P�F�r�b�g��@�O�F�Œ�r�b�g
	list<string> InputString;
	int dline;//���������@0�F��ʁi���j1�F���ʂ���i�E�j�@
	int In;//�Œ�r�b�g�̎��ɕێ�����l
	int bits;	// 2�i��16�i��

	//�����Ŏg�p����
	int *InputData;
	int dmax;
	int ptr;
	list<string>::iterator lptr;
	bool endflag;//�E�[�W�̃f�[�^���S�ďI�������P
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
	//�O���t���
	int Pp;

	//�}�[�L���O���
	int bits;	// ���ʂ̃r�b�g��
	int hex;	// �P�U�i�\�����邩�ǂ���
	int dline;//�󂯎������@0�F��ʂ���i���j1�F���ʁi�E�j�@�Q�F�j��  3:����Ȃ�
	list<string> *compString;

	//�����Ŏg�p����
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
