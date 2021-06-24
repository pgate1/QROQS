#ifndef BitSerialPetriNet_H_INCLUDED
#define BitSerialPetriNet_H_INCLUDED


#include"BSPetriObj.h"


class vArc{

public:
	int in_obj,out_obj;
	int in_line,out_line;
	string in_var,out_var;

	vArc(int i_m=0,int i_l=0,int o_m=0,int o_l=0){
		in_obj=i_m;
		out_obj=o_m;
		in_line=i_l;
		out_line=o_l;
	};
	~vArc(){};
};

class vObj{
public:
	int type;

	// 0..�̃C���f�N�X
	vector<int> joint_in,joint_out;

	vObj(){};
	~vObj(){};
};


class vModule{
public:
	// ArcMap ���� ���o�̓f�[�^�z��(0,1,2...)�ւ̕ϊ�
	map<int,int> in_arc_to_joint,out_arc_to_joint;

	// 1,2,3...������o�͕ϐ��ւ̕ϊ�
	map<int,string> in_vars, out_vars;
};



class vTran{
public:

	vector<int> data_only_out, data_only_in;
	int pipe_size;
};

class BSPetri
{

// �A�g���r���[�g
public:

	map<int, bspObject> ObjMap;
	map<int, bspToken> TokenMap;

	multimap<int, int> ArcInMap,ArcOutMap;
	map<int, bspArc> ArcMap;

	map<int, bspTransition*> TranMap;
	map<int, bspPlace*> PlaceMap;
	map<int, bspUhzi*> UhziMap;
	map<int, bspKemmakix*> KemMap;
	
	map<int, string> ModMap;
	map<int, BSPetri*> ModuleMap;	// �Эڰ��ݎ��Ɏg�p

	set<int> intrack;
	CRect trackRect;	//�O���[�o�����W������
	float viewDiam;

	int ObjId, ArcId;
	int ErrPos;//�R���p�C���ŃG���[���N���������
	int traFireable;
	int traNum;

	float ofpt;	// �O��̔��Η�

	string Memo;

	CRect globalRect, objRect;
	CPoint Globalpoint;

	set<int> simTranFireableStop;
	int confSimMaxTurn;
	int confSimBinValue;

	string module_name;
	int ModifiedFlag;

	set<int> conflict_tran;

	map<int,string> TextMap;


	// �V�~�����[�V�������ɁA�񓮍쎞�͓��o�͂̂ݒ��ׂ�
	set<int> IOTranSet;

//	map<string,TMemory> TMemMap;

#ifndef _LIGHT
	//verilog
	string verilog_out_dir;
	map<int,vObj> vObjMap;
	map<int,vArc> vArcMap;
	int vArcMapId;
	double delay_slack;
	int out_sync,v_debug;

	//C
	map<int,map<int,int> >
		func_in_index_transrate,
		func_out_index_transrate;

#endif

// �I�y���[�V����
public:

	BSPetri();
	BSPetri(const BSPetri &bsp);
	BSPetri & operator = (const BSPetri &bsp);

	int dObjAdd(CPoint gpoint,int type);
	void dObjRemove(int rp);
	int ArcChain(int vArcBase, int vArcFollow, vector<CPoint> *varcBwt=NULL, string v_name="");
	void EnlargeGlobalRect(int gx,int gy);
	void NormalizeGlobalRect();
	void DelObject(int PutIt);
	int DelArc(int vArcBase, int vArcFollow);
	//BSP�̒ǉ�
	void Add(BSPetri &bsp, set<int> &intrack, set<int> &puttrack);
	void Add(BSPetri &bsp, set<int> &puttrack);

	void Write(const char *fname );

	void SetMemo(const char *memo_str);
	void SetPasstraArc();

#ifndef _LIGHT
	//verilog�ɏo��
	void OutVerilog(const char *dir,
		const char *module_compile_option,
		const char *trafunc_compile_option,
		void (*trace)(const char* format, ...));
	void write_sim_module(const char *dir,const char *module_name);
	void write_top_module(const char *dir,const char *module_name, 
		map<int,vModule> &vModMap, map<int, vTran> &vTranMap,
			set<int> &SelectPlaceSet);
	int delay_data(int tid);
	void write_pjoint(const char *dir,const char *module_name);
	void write_dataline(const char *dir, const char *module_name, const map<int,bspTransition*> &TranMap,
		const map<int, map<int,string> > &tf_in_vars, const map<int, map<int,string> > &tf_out_vars,
		const map<int,vTran> &vTranMap);
	void write_tran(FILE *fp,const char *module_name,bspTransition &tran,int tid,
		const map<int, map<int,string> > &tf_in_vars,
		const vTran &vtran);
	void write_delay(const char *dir, const char *module_name,
				 int func_size, double delay_slack);
	void write_guard_delay(const char *dir, const char *module_name,
				 int func_size, double delay_slack);
	void write_delay_module(FILE *fp,const char *module_name,
		const char *delay_name,int delay,int tid);

	// C����ɏo��
	void PlaceNojointChelt();
	void UhziRegExpand();
	void UhziRegToTran();
	void UhziNojointDelete();
	void UhziNoIndustryCheck();
	void KemNoConsumDelete();
	void KemNojointDelete();
	void KemBinExpand();
	void KemBinToTran();
	void OutC(const char *dir,const char *module_name,void (*trace)(const char* format, ...),int output_max,
		int main_out_flag,int QROQS_out_flag,int Makefile_out_flag);
	void C_Analysis();
	void C_Compile();

#endif	// non _LIGHT

	void PetCom();
	void PetComUK();
	void PetSimReset();
	void PetSimUKReset();
	void PetSimUhziSet();
	int PetSimUhziCheck();
	BOOL PetSimUhziIndustry();
	int PetSimTraCheck();
	BOOL PetSimTraFire();
	BOOL PetSimKemConsum();
	BOOL PetSimKemCheck();

	void Open(const char *fname );
	void PetClear();

	void SetModuleName(string mname);
	void PutToken(int obj_id,int color,int value);

//	void *GetDataFromId(int m_id);	//obj_id����f�[�^�A�h���X���擾

	int GetOutputCount();	//�j���A����Ȃ��P���}�L�ȊO�̐�

	//�g�����W�V�����֐����ύX���ꂽ��ڑ�����Ă���A�[�N�̕ϐ��𒲂ׂ�
	void ArcValReset(int tranPos);

	//�Ƃɂ����ڑ�����
	void AnyArcSet();
	void AnyArcUKSet();
	void AnyTranSet();
	void AnyTokenSet();

	//�]���v���[�X�̉���
	void RealPlaceClear();

	void SelectAllObj();
	//intrack�̃I�u�W�F�N�g���W�Ńg���b�L���O��`��ݒ�
	void TrackRectSet();

	// �I������Ă��镡������ڰ����ɂ܂Ƃ߂��蕪������
	void PlaceFusion(set<int> &track,int mpid);
	void PlaceFission(set<int> &rettrack);

	void ObjMove(set<int> &track,int move_x, int move_y, int arc_in_move, int arc_out_move);

#ifdef _DEBUG
	//�`�F�b�N�֐�
	void arc_check();
	void obj_check();
#endif
	
	~BSPetri();
};



enum{
	_EDIT,
	_SIMULATION,
	_PRINT
};



#endif


/// EOF ///

