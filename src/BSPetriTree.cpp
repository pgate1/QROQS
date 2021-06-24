
#include "stdafx.h"

#include "BSPetriTree.h"
#include "other.h"




BSPetriTree::BSPetriTree()
{
	reset();

}


BSPetriTree::BSPetriTree(const BSPetriTree &tree)
{
//	TRACE("copy\n");
	*this = tree;
}

BSPetriTree & BSPetriTree::operator = (const BSPetriTree &tree)
{
//	TRACE("=\n");
	if(this!=&tree){
	//	TRACE("tree copy %d\n",rand());
		clear();

		map<string,BSPetri*>::const_iterator bspit;
		bspit = tree.bsps.begin();
		for(;bspit!=tree.bsps.end();++bspit){
			bsps[bspit->first] = new BSPetri;
			*(bsps[bspit->first]) = *(bspit->second);
		}

		root_name = tree.root_name;
		output_max = tree.output_max;
	}
	return *this;
}

void BSPetriTree::reset()
{
	clear();
	bsps[DEFAULT_ROOT_NAME] = new BSPetri;
	bsps[DEFAULT_ROOT_NAME]->module_name = DEFAULT_ROOT_NAME;
}

void BSPetriTree::clear()
{
	map<string,BSPetri*>::iterator bspit;
	bspit = bsps.begin();
	for(;bspit!=bsps.end();++bspit){
		delete bspit->second;
	}
	bsps.clear();
	root_name = DEFAULT_ROOT_NAME;
	output_max = 0;
}

void BSPetriTree::Add(const char *to_name,BSPetriTree *from_bspt,  const char *from_name, set<int> &intrack, set<int> &puttrack)
{
	puttrack.clear();
	//�ǉ��ł��邩���ׂĎ��s���邱��
	if(IsAbleAdds(to_name,from_bspt,from_name,intrack)==0){
		return;
	}

	set<string> mset;
	UnderMnameGet( from_bspt, from_bspt->GetRootBSP(), mset);

	//���̂��Ȃ��Ȃ�R�s�[
	set<string>::iterator sit;
	sit = mset.begin();
	for(; sit!= mset.end();++sit){
		if(bsps.find( *sit ) == bsps.end()){
			bsps[ *sit ] = new BSPetri;
			*(bsps[ *sit ]) = *(from_bspt->GetBSP((*sit).c_str()) );
		}
	}

	GetBSP(to_name)->Add(*(from_bspt->GetBSP(from_name)), intrack, puttrack);
}

BOOL BSPetriTree::IsInitBSP()
{
	if(bsps.size()!=1) return 0;
	if(bsps.begin()->second->ObjMap.size()!=0) return 0;
	
	return 1;
}

BSPetri *BSPetriTree::GetBSP(const char *mname)
{
	if(bsps.find(mname)==bsps.end()) return NULL;
	return bsps[mname];
}

BOOL BSPetriTree::IsRefer(const char *mname)
{
	if(mname==NULL) return 0;
	if(mname[0]=='\0') return 0;

	map<string, BSPetri*>::iterator bspit;
	map<int,string>::iterator mit;
	
	bspit = bsps.begin();
	for(;bspit!=bsps.end();++bspit){
		mit = find_by_value(
			bspit->second->ModMap.begin(),
			bspit->second->ModMap.end(),
			mname);
		if(mit != bspit->second->ModMap.end()){
			return 1; // �Q�Ƃ���Ă���
		}
	}

	// �Q�Ƃ���Ă��Ȃ�
	return 0;
}


void BSPetriTree::DeleteNonRefer()
{

	set<string> mset;
	UnderMnameGet(this, GetRootBSP(), mset);
	mset.insert(root_name);

	map<string, BSPetri*>::iterator bspit;
	bspit = bsps.begin();
	for(;bspit!=bsps.end();){
		if(mset.find(bspit->first)==mset.end()){
			map<string, BSPetri*>::iterator del_bspit;
			del_bspit = bspit;
			++bspit;
			//��u�C���N�������g�Ŏ��̃C�e���[�^�ƂȂ�
			delete del_bspit->second;
			bsps.erase(del_bspit);
		}
		else ++bspit;
	}
}

BSPetri *BSPetriTree::GetRootBSP()
{
	return bsps[root_name];
}

const char *BSPetriTree::GetRootName()
{
	return GetRootBSP()->module_name.c_str();
}

BSPetri* BSPetriTree::GetChildBSP(BSPetri *cur_bsp,int obj_id)
{
	if(cur_bsp==NULL) return NULL;
	if(cur_bsp->ModMap.find(obj_id)==cur_bsp->ModMap.end()) return NULL;
	return GetBSP( cur_bsp->ModMap[obj_id].c_str() );
}

//�S�Ẳ��ʃ��W���[�������擾
void BSPetriTree::UnderMnameGet(
	BSPetriTree *bspt,
	BSPetri *cur_bsp,
	set<string> &mset)
{
	map<int,string>::iterator mit,mit_end;
	mit = cur_bsp->ModMap.begin();
	mit_end = cur_bsp->ModMap.end();
	for(;mit!=mit_end;++mit){
		if(!mit->second.empty()
			&& mset.find(mit->second) == mset.end()){
			mset.insert(mit->second);
			UnderMnameGet(bspt, bspt->GetBSP(mit->second.c_str()), mset);
		}
	}
}


//mname�����S�Ă̏�ʃ��W���[�����W�߂�
int BSPetriTree::TopMnameGet(
	BSPetriTree *bspt,
	BSPetri *cur_bsp,
	const char *mname,
	set<string> &mset)
{
	int ret = 0;
	map<int,string>::iterator mit,mit_end;
	mit = cur_bsp->ModMap.begin();
	mit_end = cur_bsp->ModMap.end();
	for(;mit!=mit_end;++mit){
		if(!mit->second.empty()
			&& mset.find(mit->second)==mset.end()){
			ret += TopMnameGet(bspt,bspt->GetBSP(mit->second.c_str()),mname,mset);
		}
	}
	if(cur_bsp->module_name==mname || ret){
		mset.insert(cur_bsp->module_name);
		return 1;
	}
	return 0;
}

//���ʂ��܂߂����W���[������mset�Ɋ܂܂�Ă��邩
int BSPetriTree::MnameFindUnder(
	BSPetriTree *bspt,
	BSPetri *cur_bsp,
	set<string> &mset)
{
	if(mset.find(cur_bsp->module_name) != mset.end()) return 1;
	map<int,string>::iterator mit,mit_end;
	mit = cur_bsp->ModMap.begin();
	mit_end = cur_bsp->ModMap.end();
	for(;mit!=mit_end;++mit){
		if(!mit->second.empty()
			&& MnameFindUnder(bspt, bspt->GetBSP(mit->second.c_str()), mset)==1){
			return 1;
		}
	}
	return 0;
}

int BSPetriTree::IsAbleAdds(
	const char *to_name,
	BSPetriTree *from_bspt,
	const char *from_name,
	set<int> &intrack)
{
	//�ǉ��ł��邩���ׂĎ��s���邱��
	set<string> mset;
	map<int,string>::iterator mit;
	mit = from_bspt->GetBSP(from_name)->ModMap.begin();
	for(;mit != from_bspt->GetBSP(from_name)->ModMap.end();++mit){
		if(!mit->second.empty() && intrack.find(mit->first)!=intrack.end()){
			mset.insert(mit->second);
		}
	}

	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit != mset.end(); ++sit){
		if(IsAbleAdd(this, to_name, from_bspt, (*sit).c_str())==0){
			return 0;
		}
	}

	return 1;
}

int BSPetriTree::IsAbleAdd(
	BSPetriTree *to_tree,
	const char *to_parent_mname,
	BSPetriTree *from_tree,
	const char *from_mname)
{
	// to_parent_name �Ɂ@from_mname��ǉ��ł��邩�ǂ���

//	TRACE("%s <- %s\n",to_parent_mname,from_mname);

	set<string> mset;
	TopMnameGet(to_tree, to_tree->GetRootBSP(), to_parent_mname, mset);
/*
set<string>::iterator sit;
sit = mset.begin();
for(;sit!=mset.end();++sit){
	TRACE("set %s\n",(*sit).c_str());
}
*/
	if(MnameFindUnder(from_tree, from_tree->GetBSP(from_mname), mset)==1){
		//�ǉ��ł��Ȃ�
		return 0;
	}

	//�ǉ��ł���
	return 1;
}


int BSPetriTree::SetModuleNameCheck(const char *cur_name, int id, const char *mname)
{
	err_msg.empty();

	if(mname==NULL || mname[0]=='\0'){
		err_msg = "Ӽޭ�ٖ���ݒ肵�Ă��������D";
		return 0;
	}
	if(strcmp(mname,cur_name)==0){
		err_msg = "����Ӽޭ�ق��ċA�I�Ɏg�p���邱�Ƃ͂ł��܂���D";
		return 0;
	}

//��������n�܂�Ӽޭ�ٖ��ɕύX�͂ł��邪�A�R���p�C�����ɂ͂˂�
//	if(isdigit(mname[0])) return 0;

//	TRACE("change %s[%d].name -> %s\n",cur_name,id,mname);

	//root�͂��ł��ύX�ł���
	if(id==0){//root
		if(bsps.find(mname)!=bsps.end()){
			//���݂��郂�W���[�����ɂ͕ύX�ł��Ȃ�
			err_msg = "ٰ�Ӽޭ�ق����łɑ��݂���Ӽޭ�ٖ��ɕύX���邱�Ƃ͂ł��܂���D";
			return 0;
		}

		// ���łɑ��̃��W���[������Q�Ƃ���Ă���ꍇ�͖��O�̕ύX�͂ł��Ȃ�
		if(IsRefer(root_name.c_str())){
			err_msg = "���łɑ���Ӽޭ�ق���Q�Ƃ���Ă��邽�ߕύX�ł��܂���D";
			return 0;
		}
	}
	else{
		//���݂��郂�W���[�����Ȃ�
		//��ʃ��W���[���ɃJ�����g�ȉ��̃��W���[�����������Ƃ��m�F����
		if(bsps.find(mname)!=bsps.end()){
			if(IsAbleAdd(this, cur_name, this, mname)==0){
			//	TRACE("no\n");
				err_msg = "���łɏ��Ӽޭ�قɉ���Ӽޭ�ق����݂��Ă���C�ċA�\���ƂȂ邽�ߕύX�ł��܂���D";
				return 0;
			}
			// ���łɑ��̃��W���[������Q�Ƃ���Ă���ꍇ�͖��O�̕ύX�͂ł��Ȃ�
			if(IsRefer(bsps[cur_name]->ModMap[id].c_str())){
				err_msg = "���łɑ���Ӽޭ�ق���Q�Ƃ���Ă��邽�ߕύX�ł��܂���D";
				return 0;
			}
		//	TRACE("ok\n");
		}
	}

	return 1;
}


int BSPetriTree::SetModuleName(const char *cur_name, int id, const char *mname)
{
	err_msg.empty();

	if(mname==NULL || mname[0]=='\0'){
		err_msg = "Ӽޭ�ٖ���ݒ肵�Ă��������D";
		return 0;
	}
	if(strcmp(mname,cur_name)==0){
		err_msg = "����Ӽޭ�ٖ��͍ċA�\���ƂȂ邽�ߎg�p�ł��܂���D";
		return 0;
	}


//��������n�܂�Ӽޭ�ٖ��ɕύX�͂ł��邪�A�R���p�C�����ɂ͂˂�
//	if(isdigit(mname[0])) return 0;

//	TRACE("change %s[%d].name -> %s\n",cur_name,id,mname);

	//root�͂��ł��ύX�ł���
	if(id==0){//root
		if(bsps.find(mname)!=bsps.end()){
			//���݂��郂�W���[�����ɂ͕ύX�ł��Ȃ�
			err_msg = "ٰ�Ӽޭ�ٖ������łɑ��݂���Ӽޭ�ٖ��ɕύX���邱�Ƃ͂ł��܂���D";
			return 0;
		}

		// ���łɑ��̃��W���[������Q�Ƃ���Ă���ꍇ�͖��O�̕ύX�͂ł��Ȃ�
		if(IsRefer(root_name.c_str())){
			err_msg = "���łɑ���Ӽޭ�ق���Q�Ƃ���Ă��邽�ߕύX�ł��܂���D";
			return 0;
		}

		bsps[mname] = bsps[root_name];
		// ���̂������Ă��܂��ƁA�Q�Ƃ���Ă����炦�炢���ƂɂȂ�
		// �ł������Ȃ��Ƃ��������̂ŁA�ǂ��ɂ�����
		bsps.erase(root_name);

		root_name = mname;
		bsps[mname]->SetModuleName( mname );
	}
	else if(bsps.find(mname)!=bsps.end()){
		//���݂��郂�W���[�����Ȃ�
		//��ʃ��W���[���ɃJ�����g�ȉ��̃��W���[�����������Ƃ��m�F����

		if(IsAbleAdd(this, cur_name, this, mname)==0){
		//	TRACE("no\n");
			err_msg = "���łɏ��Ӽޭ�قɉ���Ӽޭ�ق����݂��邽�ߕύX�ł��܂���D";
			return 0;
		}
		
		/*
		// ���łɑ��̃��W���[������Q�Ƃ���Ă���ꍇ�͖��O�̕ύX�͂ł��Ȃ�
		if(IsRefer(bsps[cur_name]->ModMap[id].c_str())){
			err_msg = "���łɑ���Ӽޭ�ق���Q�Ƃ���Ă��邽�ߕύX�ł��܂���D";
			return 0;
		}
		*/
	//	TRACE("in list\n");
		bsps[cur_name]->ModMap[id] = mname;
	}
	else{	// ROOT�ł��Ȃ����A�������O���Ȃ�
		bsps[mname] = new BSPetri;
		bsps[cur_name]->ModMap[id] = mname;
		bsps[mname]->SetModuleName( mname );
	//	TRACE("new bsp\n");
	}

	return 1;
}




void BSPetriTree::ExpandBSPAll(BSPetri *cur_bsp, int move_flag)
{

	if(cur_bsp==NULL) return;
	if(cur_bsp->ModMap.size()==0) return;

	map<int,bspUhzi*>::iterator uit;
	map<int,bspKemmakix*>::iterator kit;
	int mpid,cpid;
	set<int> places;
	multimap<int,int>::iterator aioit,aioit_end;
	set<int> track, aset, delset;
	map<string,int> tukmap;
	map<string,int>::iterator tukit;

	map<int,string>::iterator mit;
	mit = cur_bsp->ModMap.begin();
	for(;mit!=cur_bsp->ModMap.end();){
		if(mit->second.empty()) continue;//�`�F�b�N�ς݂Ȃ炠�肦�Ȃ�

		BSPetri *bsp = GetBSP(mit->second.c_str());
		
		ExpandBSPAll( bsp, move_flag );

//	TRACE("sub %s\n",bsp->module_name.c_str());

// �W�J���鎞�ɁA����̃I�u�W�F�N�g���L����
		if(move_flag){
			bsp->NormalizeGlobalRect();
			int half_gWidth = bsp->globalRect.Width() >> 1;
			int half_gHeight = bsp->globalRect.Height() >> 1;

			CPoint mgp,gp;
			mgp = cur_bsp->ObjMap[mit->first].gpoint;
			map<int,bspObject>::iterator oit;
			oit = cur_bsp->ObjMap.begin();
			for(;oit!=cur_bsp->ObjMap.end();++oit){
				gp = oit->second.gpoint;
				if(gp.x!=mgp.x){
					gp.x += (gp.x>mgp.x ? half_gWidth : -half_gWidth );
				}
				if(gp.y!=mgp.y){
					gp.y += (gp.y>mgp.y ? half_gHeight : -half_gHeight );
				}
				oit->second.gpoint = gp;
			}

		}
	
		//�q���W���[���̃I�u�W�F�N�g���R�s�[
		cur_bsp->Add( *bsp , track);

		if(move_flag){
/*
TRACE("mgp %d\n",cur_bsp->ObjMap[mit->first].gpoint.x);
TRACE("wid %d\n",bsp->globalRect.Width()>>1);
TRACE("rig %d\n",bsp->globalRect.left);
*/
		// �W�J������A�悳���ȏꏊ�Ɉړ�
		cur_bsp->ObjMove(track, 
			cur_bsp->ObjMap[mit->first].gpoint.x - (bsp->globalRect.Width()>>1) - bsp->globalRect.left,
			cur_bsp->ObjMap[mit->first].gpoint.y - (bsp->globalRect.Height()>>1) - bsp->globalRect.top,
			0, 0);
		}


	//	TRACE("track %d\n",track.size());


		//���̓A�[�N
		tukmap.clear();
		uit = cur_bsp->UhziMap.begin();
		for(;uit!=cur_bsp->UhziMap.end();++uit){
			if(track.find( uit->first ) != track.end()){
				tukmap[uit->second->input_name] = uit->first;
			}
		}

		aset.clear();
		aioit = cur_bsp->ArcOutMap.lower_bound( mit->first );
		aioit_end = cur_bsp->ArcOutMap.upper_bound( mit->first );
		for(;aioit!=aioit_end;++aioit){
			aset.insert(aioit->second);
		}

		delset.clear();

		set<int>::iterator asit;
		asit = aset.begin();
		for(;asit!=aset.end();++asit){
			// �ڑ���̳��ނ�����
			tukit = tukmap.find(cur_bsp->ArcMap[ *asit ].value_name);
			if(tukit==tukmap.end()) continue;

			//�ڑ���̃v���[�X���擾
			if(cur_bsp->ArcInMap.lower_bound(tukit->second)==cur_bsp->ArcInMap.upper_bound(tukit->second)) continue;

			mpid = cur_bsp->ArcMap[ cur_bsp->ArcInMap.lower_bound( tukit->second )->second ].Out;
			cpid = cur_bsp->ArcMap[ *asit ].In;
			places.clear();
			places.insert(mpid);
			places.insert(cpid);

			cur_bsp->PlaceFusion(places, cpid);
			delset.insert(tukit->second);
		}


		// �o��
		tukmap.clear();
		kit = cur_bsp->KemMap.begin();
		for(;kit!=cur_bsp->KemMap.end();++kit){
			if(track.find( kit->first ) != track.end()){
				tukmap[kit->second->output_name] = kit->first;
			}
		}

		aset.clear();
		aioit = cur_bsp->ArcInMap.lower_bound( mit->first );
		aioit_end = cur_bsp->ArcInMap.upper_bound( mit->first );
		for(;aioit!=aioit_end;++aioit){
			aset.insert(aioit->second);
		}

		//���̓A�[�N
		asit = aset.begin();
		for(;asit!=aset.end();++asit){
			// �ڑ���̳��ނ�����
			tukit = tukmap.find(cur_bsp->ArcMap[ *asit ].value_name);
			if(tukit==tukmap.end()) continue;

			//�ڑ���̃v���[�X���擾
			if(cur_bsp->ArcOutMap.lower_bound(tukit->second)==cur_bsp->ArcOutMap.upper_bound(tukit->second)) continue;

			mpid = cur_bsp->ArcMap[ cur_bsp->ArcOutMap.lower_bound( tukit->second )->second ].In;
			cpid = cur_bsp->ArcMap[ *asit ].Out;
			places.clear();
			places.insert(mpid);
			places.insert(cpid);

			cur_bsp->PlaceFusion(places, cpid);
			delset.insert(tukit->second);
		}

		int did;
		set<int>::iterator sit;
		sit = delset.begin();
		for(;sit!=delset.end();){
			did = *sit;
			++sit;
			cur_bsp->DelObject( did );
		}

		int mid = mit->first;
		++mit;
		cur_bsp->DelObject( mid );
	}

//	TRACE("%s end\n",cur_bsp->module_name.c_str());


}



void BSPetriTree::SetRootBSP(const char *mname)
{
	root_name = mname;
}

void BSPetriTree::Open(const char *fname)
{
	reset();

	try{
		bsps[DEFAULT_ROOT_NAME]->Open(fname);
	}
	catch(char *err){
		throw err;
	}

	//���W���[�������ݒ肳��Ă���Ȃ�ύX����
	if(bsps[DEFAULT_ROOT_NAME]->module_name.empty()){
		bsps[DEFAULT_ROOT_NAME]->module_name = DEFAULT_ROOT_NAME;
	}
	else{
		SetModuleName(DEFAULT_ROOT_NAME, 0, bsps[DEFAULT_ROOT_NAME]->module_name.c_str());
		GetRootBSP()->ModifiedFlag = 0;
	}

	struct func{
		static void openModule(
			map<string,BSPetri*> &bsps,
			string &mname,
			string &dir)
		{
			map<int,string>::iterator mit,mit_end;
			mit = bsps[mname]->ModMap.begin();
			mit_end = bsps[mname]->ModMap.end();
			for(;mit!=mit_end;++mit){
				if(!mit->second.empty()
					&& bsps.find( mit->second ) == bsps.end()){
			//		TRACE("open %s\n",mit->second.c_str());
					bsps[ mit->second ] = new BSPetri;
					try{
						bsps[ mit->second ]->Open( (dir + mit->second + ".bsp").c_str());
					}
					catch(char *err){
						bsps[ mit->second ]->PetClear();
						bsps[ mit->second ]->SetModuleName( mit->second.c_str() );
						TRACE("no open %s\n",err);
						Trace("%s : %s\n",(dir + mit->second + ".bsp").c_str(),err);
					}
					openModule( bsps, mit->second, dir);
				}
			}
		}
	};

	string dir=fname;
	dir.erase( dir.rfind('\\')+1 );
	func::openModule(bsps, root_name, dir);

//	TRACE("%d\n",bsps.size());
}


//�w��f�B���N�g���Ɉꊇ�ۑ�
void BSPetriTree::WriteAll(const char *fpath)
{
//	TRACE("write all\n");
//	DeleteNonRefer();
	char *fname = new char[1024];
	map<string,BSPetri*>::iterator bspit;
	bspit = bsps.begin();
	for(;bspit!=bsps.end();++bspit){
		sprintf(fname,"%s\\%s.bsp",fpath,bspit->second->module_name.c_str());
//		TRACE("save %s\n",fname);
		bspit->second->Write(fname);
	}
	delete[] fname;
}


//���ʎQ�ƗD��őS�Ẳ��ʃ��W���[�������擾
int BSPetriTree::UnderMnameGetRefers(
	BSPetriTree *bspt,
	BSPetri *cur_bsp,
	map<string,int> &rfmap,
	int ref)
{
	map<int,string>::iterator mit,mit_end;
	mit = cur_bsp->ModMap.begin();
	mit_end = cur_bsp->ModMap.end();
	for(;mit!=mit_end;++mit){
		if(!mit->second.empty()){
			rfmap[mit->second] = ref;
			ref++;
			ref = UnderMnameGetRefers(bspt, bspt->GetBSP(mit->second.c_str()), rfmap, ref);
		}
	}
	return ref;
}

#ifndef _LIGHT


//#define ROHM06
#define ROHM035

void write_makefile(const char *dir, const char *module_name, const map<int,string> &remap){
	char *fname = new char[MAX_PATH];
	sprintf(fname,"%s/Makefile",dir);
	FILE *fp;
	fp=fopen(fname,"w");

	delete[] fname;
///home/cad/vdec-lib/rohm6/verilog/rohm06.v
///home/cad/vdec-lib/rohm35/lib/rohm035_EXD/verilog/ROHM035.v
	time_t t_now;
	t_now = time(NULL);
	
	fprintf(fp,
		"#\n"
		"# created by QROQS\n"
		"#	model    : Makefile\n"
		"#	function : Operate verilog simulator & design compiler.\n"
		"#	update   : %s"
		"#\n\n"
		,ctime(&t_now));

	fprintf(fp,"VERILOG	= "
		"ncverilog"
//		"verilog"
		"\n");

	fprintf(fp, "VFLAGS	= "
		"+access+r"
		"\n");

	fprintf(fp, "CELLLIB	= "
#ifdef ROHM035
		"/home/cad/vdec-lib/rohm35/lib/rohm035_EXD/verilog/ROHM035.v"
#endif
#ifdef ROHM06
		"/home/cad/vdec-lib/rohm6/verilog/rohm06.v"
#endif
		"\n");

	fprintf(fp,
		"verilog:\n"
		"	$(VERILOG) $(VFLAGS) "
				"%s_sim.v ", module_name);

	map<int,string>::const_iterator mit;
	mit = remap.begin();
	for(;mit!=remap.end();++mit){
		const char *mn = mit->second.c_str();
		fprintf(fp,"%s/%s.v ", mn,mn);
		fprintf(fp,"%s/%s_transition.v ", mn,mn);
		fprintf(fp,"%s/%s_pipe.v ", mn,mn);
		fprintf(fp,"%s/%s_pjoint.v ", mn,mn);
		fprintf(fp,"%s/%s_delay.v ", mn,mn);
		fprintf(fp,"%s/%s_trafunc.v ", mn,mn);
		fprintf(fp,"%s/%s_guard.v ", mn,mn);
		fprintf(fp,"%s/%s_gdelay.v ", mn,mn);
	}

		fprintf(fp,
				"tools/arbiter.vnet "
				"tools/muller_c.vnet "
				"tools/kemmakixuhzi.v "
				"-v $(CELLLIB)\n"
			);

//	fprintf(fp,"tf:\n");
//	fprintf(fp,"	dc_shell -f trafunc.delay.dc | tee trafunc.dc.log\n");
	
	fprintf(fp,"dc:\n");
	fprintf(fp,"	make delayadd\n");
	fprintf(fp,"	dc_shell -f %s.dc | tee %s.dc.log\n",module_name,module_name);


	fprintf(fp,
		"sim:\n"
		"	$(VERILOG) $(VFLAGS) "
				"%s_sim.v ", module_name);

	mit = remap.begin();
	for(;mit!=remap.end();++mit){
		const char *mn = mit->second.c_str();
		fprintf(fp,"%s/%s.vnet ", mn,mn);
	}
		fprintf(fp,
				"tools/arbiter.vnet "
				"tools/muller_c.vnet "
				"tools/kemmakixuhzi.v "
				"-v $(CELLLIB)\n"
			);
/*
	fprintf(fp,
		"sim:\n"
		"	verilog %s_sim.v %s.vnet tools/kemmakixuhzi.v "
#ifdef ROHM035
		"-v /home/cad/vdec-lib/rohm35/lib/rohm035_EXD/verilog/ROHM035.v\n"
#endif
#ifdef ROHM06
		"-v /home/cad/vdec-lib/rohm6/verilog/rohm06.v\n"
#endif
		,module_name,module_name);
*/

	fprintf(fp,"area:\n");
	fprintf(fp,"	dc_shell -f %s.area.dc | tee %s.area.dc.log\n",module_name,module_name);



	fprintf(fp,
		"delayadd: tools/delayadd.c\n"
		"	gcc -o tools/delayadd tools/delayadd.c\n"
		);

	fprintf(fp,
		"clean:\n"
		"	rm -rf *.rep *.log verilog.log command.log\n"
		);
	fclose(fp);
}

void write_dc_src(const char *dir, const char *module_name, const map<int,string> &remap)
{

	{
	char *fname = new char[MAX_PATH];
	FILE *fp;

	sprintf(fname,"%s/%s.dc",dir,module_name);
	fp=fopen(fname,"w");
	delete[] fname;

/*
	// ���ʂ���
	map<int,string>::const_reverse_iterator reit;
	reit = remap.rbegin();
	for(;reit!=remap.rend();++reit){
*/
	// ��ʂ���
	map<int,string>::const_iterator reit;
	reit = remap.begin();
	for(;reit!=remap.end();++reit){
		fprintf(fp,"cd %s\n",reit->second.c_str());
		fprintf(fp,"include %s.dc\n",reit->second.c_str());
		fprintf(fp,"remove_design %s -hierarchy\n",reit->second.c_str());
		fprintf(fp,"cd ../\n");
	}

//	fprintf(fp,"current_design = %s\n",module_name);
//	fprintf(fp,"set_wire_load_model -name 1k\n");
	
	//�Эڰ��݂̂��߂ɏ�������
//	fprintf(fp,"write -format verilog -hierarchy -output %s.vnet %s\n",module_name,module_name);
//	fprintf(fp,"report_area > %s.rep\n",module_name);
//	fprintf(fp,"report_timing >> %s.rep\n",module_name);

	fprintf(fp,"quit\n");

	fclose(fp);
	}



	/////  area
	{
	char *fname = new char[MAX_PATH];
	FILE *fp;

	sprintf(fname,"%s/%s.area.dc",dir,module_name);
	fp=fopen(fname,"w");
	delete[] fname;

	// �ʐς����邽�߂ɁA�ǂݍ���
	fprintf(fp,"read -format verilog { ");
	// ��ʂ���
	map<int,string>::const_iterator reit;
	reit = remap.begin();
	for(;reit!=remap.end();++reit){
		fprintf(fp,"%s/%s.vnet ",reit->second.c_str(),reit->second.c_str());
	}
	fprintf(fp,"tools/arbiter.vnet tools/muller_c.vnet }\n");

	fprintf(fp,"current_design = %s\n",module_name);
	fprintf(fp,"set_wire_load_model -name 1k\n");
	//�Эڰ��݂̂��߂ɏ�������
//	fprintf(fp,"write -format verilog -hierarchy -output %s.vnet %s\n",module_name,module_name);
	fprintf(fp,"report_area > %s.area.rep\n",module_name);
//	fprintf(fp,"report_timing >> %s.rep\n",module_name);

	fprintf(fp, "sh tail -23 %s.area.rep\n", module_name);

	fprintf(fp,"quit\n");

	fclose(fp);
	}



}

void write_uhzi_data(const char *dir,map<int,bspUhzi*> &UhziMap)
{
	int i,j;
	FILE *fp;
	char *fname = new char[MAX_PATH];
	//�E�[�W�̓��̓f�[�^
	map<int,bspUhzi*>::iterator uit;
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){

		bspUhzi *uhzi = uit->second;

		//TRACE("u %d\n",uit->second.dmax);
		if(uhzi->input_name.empty()){
			sprintf(fname,"%s/i%d.data",dir,uit->first);
		}
		else{
			sprintf(fname,"%s/%s.data",dir,uhzi->input_name.c_str());
		}
		fp=fopen(fname,"w");
//		setbuf(fp, NULL);

		if(uhzi->addp==1){
//			udmax[uit->first] = 0;
			uhzi->Set();
			for(;uhzi->endflag==false;){
				j=0;
				for(i=0;i<uhzi->dmax;i++){
					fprintf(fp,"%d ",uhzi->InputData[i]);
					j++;
					if(j>=128){
						fprintf(fp,"\n");
						j=0;
					}
				}
				fprintf(fp,"\n");
//				udmax[uit->first] += uhzi->dmax;
				uhzi->Set();
			}
		}
		else{
			// ���W�X�^�E�[�W�̓g���v���ɕϊ�����Ă�͂�
		}

		fclose(fp);
	}
	delete[] fname;
}


void write_out_comp(const char *dir,map<int,bspKemmakix*> &KemMap)
{
	int i;
	FILE *fp;
	char *fname = new char[MAX_PATH];
	//�E�[�W�̓��̓f�[�^
	map<int,bspKemmakix*>::iterator kit;
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){

		bspKemmakix *kem = kit->second;
		if(kem->compString==NULL || kem->compString->size()==0) continue;

		//TRACE("u %d\n",uit->second.dmax);
		if(kem->output_name.empty()){
			sprintf(fname,"%s/o%d.comp",dir,kit->first);
		}
		else{
			sprintf(fname,"%s/%s.comp",dir,kem->output_name.c_str());
		}
		fp=fopen(fname,"w");
//		setbuf(fp, NULL);


		list<string>::iterator sit;
		sit = kem->compString->begin();
		for(;sit!=kem->compString->end();++sit){
			int max_len;
			max_len = sit->length();
			// ���s�̔r���i�����Ƃ������@�����邾�낤��
			char c = (*sit)[max_len-1];
			if(c!='0' && c!='1') max_len--;
			c = (*sit)[max_len-1];
			if(c!='0' && c!='1') max_len--;
			if(kem->dline==0){
				for(i=0;i<max_len;i++){
					fprintf(fp, "%c\n", (*sit)[i]);
				}
			}
			else{
				for(i=max_len-1;i>=0;i--){
					fprintf(fp, "%c\n", (*sit)[i]);
				}
			}
		}

		fclose(fp);
	}
	delete[] fname;
}


void BSPetriTree::OutVerilog(const char *dir,
								const char *module_compile_option,
							 const char *trafunc_compile_option,
							 void (*trace)(const char* format, ...),
							 void (*progset)(int abs,int step))
{


//		TRACE("s %d\n",MAX_PATH);
		// root�ȉ��̃��W���[�������擾
	set<string> mset;
	UnderMnameGet(this, GetRootBSP(), mset);
	mset.insert(root_name);


	UhziNojointDelete(mset);
	KemNojointDelete(mset);
	PlaceNojointChelt(mset);

	KemBinExpand(mset);
	UhziRegExpand(mset);

	try{

	BSPCompile(mset);
//	TRACE("analysis\n");

	Trace("��͒�...\n");

	// �eBSP��̨����ޯ��ڑ��̉�͂ƕs�v�g���v���̍폜
	C_Analysis(mset);
//	TRACE("expand\n");
	}
	catch(char *err){
//		trace(err);
		throw err;
	}
	catch(RetEx &err){
//		trace(err.err);
		throw err.err;
	}


//	UhziNoIndustryCheck(mset);

//	KemBinToTran(mset);
//	UhziRegToTran(mset);



	FILE *log_fp;
	{
		char *fname=new char[MAX_PATH];
		sprintf(fname,"%s/QROQS.log",dir);
		log_fp = fopen(fname,"w");
		delete[] fname;
		if(log_fp==NULL){
			throw "̧�ق��J���܂���D";
		}
	}

	{
		char *vdir = new char[MAX_PATH];
		set<string>::iterator sit;
		sit = mset.begin();
		for(;sit!=mset.end();++sit){
			sprintf(vdir,"%s/%s",dir,(*sit).c_str());
		//	TRACE("dir:%s\n",vdir);
			CreateDirectory(vdir,NULL);
			GetBSP(sit->c_str())->delay_slack = GetRootBSP()->delay_slack;
			GetBSP(sit->c_str())->v_debug = GetRootBSP()->v_debug;
			GetBSP((*sit).c_str())->OutVerilog(vdir, module_compile_option,trafunc_compile_option, trace);
		}
		delete[] vdir;
	}


	// sim_module
	trace("�Эڰ���Ӽޭ�ق��o�͒�...");
	GetRootBSP()->write_sim_module(dir,GetRootName());

	map<int,string> remap;
	{
		map<string,int> rfmap;
		rfmap[GetRootName()] = 1;
		UnderMnameGetRefers(this, GetRootBSP(), rfmap, 2);
		map<string,int>::iterator mit;
		mit = rfmap.begin();
		for(;mit!=rfmap.end();++mit){
			remap[mit->second] = mit->first;
		}
		rfmap.clear();
	}


	//Makfile
	write_makefile(dir, GetRootName(), remap);

	// dc
	write_dc_src(dir, GetRootName(), remap);

	{
#define VMOD_DIR "bspv"	// QROQS.exe���猩���c�[�����W���[���Q�f�B���N�g��

		char *fname = new char[MAX_PATH];
		sprintf(fname,"%s/tools",dir);
		CreateDirectory(fname,NULL);
		sprintf(fname,"%s/tools/delayadd.c",dir);
		CopyFile(VMOD_DIR"/delayadd.c",fname,false);//�㏑��
		sprintf(fname,"%s/tools/kemmakixuhzi.v",dir);
		CopyFile(VMOD_DIR"/kemmakixuhzi.v",fname,false);//�㏑��
		sprintf(fname,"%s/tools/muller_c.vnet",dir);
		CopyFile(VMOD_DIR"/muller_c.vnet",fname,false);//�㏑��
		sprintf(fname,"%s/tools/arbiter.vnet",dir);
		CopyFile(VMOD_DIR"/arbiter.vnet",fname,false);//�㏑��



		trace("�����ް�̧�ق��o�͂��Ă��܂�...\n");
		sprintf(fname,"%s/indata",dir);
		CreateDirectory(fname,NULL);
		write_uhzi_data(fname,GetRootBSP()->UhziMap);

		trace("�ƍ��p�ް�̧�ق��o�͂��Ă��܂�...\n");
		sprintf(fname,"%s/outcomp",dir);
		CreateDirectory(fname,NULL);
		write_out_comp(fname,GetRootBSP()->KemMap);

		sprintf(fname,"%s/outdata",dir);
		CreateDirectory(fname,NULL);
		delete[] fname;
	}


	fclose(log_fp);

}


void BSPetriTree::UhziRegExpand(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->UhziRegExpand();
	}
}

void BSPetriTree::UhziRegToTran(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->UhziRegToTran();
	}
}

void BSPetriTree::UhziNojointDelete(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->UhziNojointDelete();
	}
}


void BSPetriTree::UhziNoIndustryCheck(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->UhziNoIndustryCheck();
	}
}

void BSPetriTree::KemBinExpand(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->KemBinExpand();
	}
}

void BSPetriTree::KemBinToTran(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->KemBinToTran();
	}
}

void BSPetriTree::KemNoConsumDelete(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->KemNoConsumDelete();
	}
}

void BSPetriTree::PlaceNojointChelt(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->PlaceNojointChelt();
	}
}

void BSPetriTree::KemNojointDelete(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->KemNojointDelete();
	}
}

void BSPetriTree::C_Analysis(set<string> &mset)
{
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		bsps[*sit]->C_Analysis();
	}
}

void BSPetriTree::OutC(const char *dir,
					   int main_out_flag, int QROQS_out_flag, int Makefile_out_flag,
					   void (*trace)(const char* format, ...))
{

//		TRACE("s %d\n",MAX_PATH);
		// root�ȉ��̃��W���[�������擾
	set<string> mset;
	UnderMnameGet(this, GetRootBSP(), mset);
	mset.insert(root_name);

	UhziNojointDelete(mset);
	UhziRegExpand(mset);
	KemBinExpand(mset);
	KemNoConsumDelete(mset);
	KemNojointDelete(mset);

//return;
	BSPCompile(mset);
//	TRACE("analysis\n");

	trace("��͒�...\n");

	// �eBSP��̨����ޯ��ڑ��̉�͂ƕs�v�g���v���̍폜
	C_Analysis(mset);
//	TRACE("expand\n");

//TRACE("exp %d\n",GetRootBSP()->ArcInMap.count(222));
	trace("�W�J��...\n");
	try{
		ExpandBSPAll(GetRootBSP(), false);
	}
	catch(char *err){
		TRACE("%s\n",err);
	}
//TRACE("exp %d\n",GetRootBSP()->ArcInMap.count(222));
//return;
	trace("�ڑ���...\n");
	GetRootBSP()->C_Compile();


	char *fname=new char[MAX_PATH];
	sprintf(fname,"%s/out.txt",dir);
	FILE *fp;
	fp=fopen(fname,"w");
	if(fp==NULL){
		delete[] fname;
		throw "̧�ق��J���܂���D";
	}
	fprintf(fp,"C����L�q�̏o�͂����s���܂����D");
	fclose(fp);


	GetRootBSP()->OutC(dir,root_name.c_str(),trace,output_max, main_out_flag, QROQS_out_flag, Makefile_out_flag);


	//�㏈��
	sprintf(fname,"%s/out.txt",dir);
	remove(fname);

	delete[] fname;

}
#endif	//non _LIGHT


void BSPetriTree::NormalizeGlobalRect()
{
	map<string,BSPetri*>::iterator bspit;
	bspit = bsps.begin();
	for(;bspit!=bsps.end();++bspit){
		bspit->second->NormalizeGlobalRect();
	}
}

BSPetriTree::~BSPetriTree()
{
	clear();
}

/// EOF ///
