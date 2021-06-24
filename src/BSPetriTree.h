#ifndef BitSerialPetriNetTree_H_INCLUDED
#define BitSerialPetriNetTree_H_INCLUDED

#include"stdafx.h"
#include"BSPetri.h"


/**********************************************::
	BSPツリー単体のクラス
	複数ツリーの管理はCQROQSDocumentで行う
************************************************/
#define DEFAULT_ROOT_NAME "untitle"
class BSPetriTree
{



// アトリビュート
public:

	map<string, BSPetri*> bsps;
	string root_name;

	string err_mname;
	string err_msg;

	int output_max;
	int confSimBinValue;

	void (*progset)(int abs,int add);
	char f_check_halt;

// オペレーション
public:

	BSPetriTree();
	BSPetriTree(const BSPetriTree &bspTree);
	BSPetriTree & operator = (const BSPetriTree &bspTree);
	void reset();
	void clear();
	void AttachProg(void (*prog)(int abs,int add));
	void Compile();
	void HaltSimulation();
	void Add(const char *to_name,BSPetriTree *from_bspt,  const char *from_name, set<int> &intrack, set<int> &puttrack);
	int IsAbleAdds(const char *to_name,BSPetriTree *from_bspt,  const char *from_name, set<int> &intrack);
	void UnderMnameGet(BSPetriTree *bspt, BSPetri *cur_bsp, set<string> &mset);
	int TopMnameGet(BSPetriTree *bspt,BSPetri *cur_bsp,const char *mname,set<string> &mset);
	int MnameFindUnder(	BSPetriTree *bspt,BSPetri *cur_bsp,set<string> &mset);
	int UnderMnameGetRefers(BSPetriTree *bspt, BSPetri *cur_bsp, map<string,int> &rfmap, int ref);
	int IsAbleAdd(BSPetriTree *to_tree,const char *to_mname,BSPetriTree *from_tree,	const char *from_mname);
	BOOL IsInitBSP();
	void DeleteNonRefer();
	BOOL IsRefer(const char *mname);
	BSPetri *GetChildBSP(BSPetri* cur_bsp,int obj_id);
	BSPetri *GetBSP(const char *mname);
	BSPetri *GetRootBSP();
	const char *GetRootName();
	int SetModuleNameCheck(const char *cur_name, int id, const char *mname);
	int SetModuleName(const char *cur_name, int id, const char *mname);
	void Open(const char *fname);
	void WriteAll(const char *fpath);
	void SetRootBSP(const char *mname);

	void BSPCompile(set<string> &mset);
	void NormalizeGlobalRect();

	void PetSimUhziSet();
	BOOL PetSimUhziIndustry();
	BOOL PetSimKemConsum();
	BOOL PetSimKemCheck();
	int PetSimTraCheck();
	BOOL PetSimTraFire();
	int PetSimUhziCheck();
	void PetSimBlast(int pm);
	void PetSimUKReset();
	void PetSimReset();

	void ExpandBSPAll(BSPetri *cur_bsp, int move_flag);

#ifndef _LIGHT
	void PlaceNojointChelt(set<string> &mset);
	void UhziNojointDelete(set<string> &mset);
	void UhziNoIndustryCheck(set<string> &mset);
	void UhziRegExpand(set<string> &mset);
	void UhziRegToTran(set<string> &mset);
	void KemNoConsumDelete(set<string> &mset);
	void KemNojointDelete(set<string> &mset);
	void KemBinExpand(set<string> &mset);
	void KemBinToTran(set<string> &mset);
	void OutVerilog(const char *dir,
		const char *module_compile_option,
		const char *trafunc_compile_option,
		void (*trace)(const char* format, ...),void (*progset)(int abs,int step));
	void OutC(const char *dir,int main_out_flag,int QROQS_out_flag,int Makefile_out_flag,void (*trace)(const char* format, ...));
	void C_Analysis(set<string> &mset);
#endif

	~BSPetriTree();
};


#endif


/// EOF ///

