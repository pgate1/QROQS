
#include "stdafx.h"

#include "BSPetri.h"
#include"QROQS.h"
#include"other.h"

/*
template <class T, typename U>
struct select2nd : public std::unary_function<T, U>
{
  const U& operator() (const T& x) const { return x.second; }
};
*/

void BSPetri::PetSimReset()
{
	intrack.clear();

	// �ŏ��͑S�Ă���ݼ޼�݂�T��
	traFireable = true;
	ofpt = 0.0;

}

//�y�g���l�b�g�̃R���p�C�����`�F�b�N
void BSPetri::PetCom()
{

	PetSimReset();

	//�R�}���h�����ɉf���Ă������H
	//	TRACE("compile\n");
	
	map<int,bspToken>::iterator tokit,tokit_end;

	AnyTokenSet();


	multimap<int,int>::iterator aioit;
	
	//�g�[�N���̔z�u�`�F�b�N
	try{
		int tid, pid;
		tokit = TokenMap.begin();
		tokit_end = TokenMap.end();
		for(;tokit!=tokit_end;++tokit){
			if(tokit->second.Color==_BTOKEN){

				// ��ڰ��̓��͂�1�ł��邩
				if(ArcOutMap.count(tokit->first) != 1){
					ErrPos = tokit->first;
					throw "������ڰ��ɍ�İ�݂�z�u���邱�Ƃ͂ł��܂���D";
				}

				aioit = ArcOutMap.lower_bound(tokit->first);
				tid = ArcMap[ aioit->second ].In;

				// �p�X�g���ł��邩�ǂ���
				if(ArcOutMap.count(tid) != 1 || ArcInMap.count(tid) != 1){
					ErrPos = tid;
					throw "�߽��ݼ޼�݂ł���K�v������܂��D";
				}

				aioit = ArcOutMap.lower_bound( tid );
				pid = ArcMap[ aioit->second ].In;
				if( TokenMap.find( pid ) == TokenMap.end()
					|| TokenMap[ pid ].Color == _BTOKEN){
					ErrPos = pid;
					throw "��İ�݂̒���ɔ�İ�݂�z�u���Ă��������D";
				}

				// ��ڰ��̏o�͂�1���ǂ���
				if(ArcInMap.count(pid)!=1){
					ErrPos = pid;
					throw "����_�ɔ�İ�݂�z�u���邱�Ƃ͂ł��܂���D";
				}

				PlaceMap[pid]->SetTokenValue( tokit->second.Value );//�O�̂���
			}
		}
	}
	catch(char *err){
		throw err;
	}



//	TRACE("tran io set\n");

	map<int,bspTransition*>::iterator tit,tit_end;

	int errp=0;
	try{

		AnyTranSet();

		tit = TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			//�g�����W�V�����֐�����ϐ��𒊏o
			errp = tit->first;
			tit->second->FuncEduction();
		}

		PetComUK();
	}
	catch(char * err){
		ErrPos = errp;
		throw err;
	}

	AnyArcSet();
	AnyArcUKSet();


	map<int,bspUhzi*>::iterator uit,uit_end;
	map<int,bspKemmakix*>::iterator kit,kit_end;

	//verilog
	//�E�[�W��P���}�L�ɐڑ�����Ă���v���[�X��
	//�����g�[�N����z�u���邱�Ƃ͂ł��Ȃ�
	uit=UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		if(TokenMap.find(uit->second->Pp)!=TokenMap.end()
			//&& vTokenMap[uit->second.Pp].Color==_BTOKEN
			){
			ErrPos=uit->second->Pp;
			throw "���ނ���ڑ�����Ă�����ڰ���İ�݂�z�u���邱�Ƃ͂ł��܂���D";
		}
	}

	//�P���}�L�͂���g�[�N��������
	kit=KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		if(TokenMap.find(kit->second->Pp)!=TokenMap.end()
		//	&& TokenMap[kit->second.Pp].Color==_WTOKEN
			){
			ErrPos = kit->second->Pp;
			throw "��Ϸ�ɐڑ����Ă�����ڰ���İ�݂�z�u���邱�Ƃ͂ł��܂���D";
		}
	}

	map<int,string>::iterator mit,mit_end;
	mit = ModMap.begin();
	mit_end = ModMap.end();
	for(;mit!=mit_end;++mit){
		aioit = ArcOutMap.lower_bound(mit->first);
		for(;aioit!=ArcOutMap.upper_bound(mit->first);++aioit){
			if(TokenMap.find( ArcMap[ aioit->second ].In ) != TokenMap.end()){
				throw "Ӽޭ�قɐڑ����Ă�����ڰ���İ�݂�z�u���邱�Ƃ͂ł��܂���D";
			}
		}
		aioit = ArcInMap.lower_bound(mit->first);
		for(;aioit!=ArcInMap.upper_bound(mit->first);++aioit){
			if(TokenMap.find( ArcMap[ aioit->second ].Out ) != TokenMap.end()){
				throw "Ӽޭ�قɐڑ����Ă�����ڰ���İ�݂�z�u���邱�Ƃ͂ł��܂���D";
			}
		}
	}



	try{
		//�g�����W�V�����֐��̃R���p�C��
//	TRACE("Compile...\n");
//		Trace("�g�����W�V�����֐��̃R���p�C����...%d\r\n",TranList.GetCount());
		tit = TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			errp = tit->first;
			tit->second->CompileFunc(PlaceMap);
		}

	}
	catch(char * err){
//		TRACE("err\n");
		ErrPos = errp;
		throw err;
	}


	// ���o�͂ɂȂ����Ă���ݼ޼�݂�o�^
	IOTranSet.clear();
	int tid;
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		aioit = ArcInMap.lower_bound( uit->second->Pp );
		for(;aioit!=ArcInMap.upper_bound( uit->second->Pp );++aioit){
			tid = ArcMap[ aioit->second ].Out;
			if(ObjMap[ tid ].type==_TRANSITION){
				IOTranSet.insert(tid);
			}
		}
	}
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		aioit = ArcOutMap.lower_bound( kit->second->Pp );
		for(;aioit!=ArcOutMap.upper_bound( kit->second->Pp );++aioit){
			tid = ArcMap[ aioit->second ].In;
			if(ObjMap[ tid ].type==_TRANSITION){
				IOTranSet.insert(tid);
			}
		}
	}



//	TRACE("ggg\n");

	/*
	// �g�����W�V�����������ǂݍ���
	TMemMap.clear();
	map<int,Transition>::iterator tit;
	tit = TranMap.begin();
	for(;tit!=TranMap.end();++tit){
		if(!tit->second.tname.empty()){
			if(TMemMap.find(tit->second.tname) == TMemMap.end()){
				TMemMap[tit->second.tname].Set(tit->second.tname.c_str());
			}
			tit->second.tmem = &TMemMap[tit->second.tname];
		}
		tit->second.tmem = NULL;
	}
*/


	map<int,bspArc>::iterator ait;
/*
/////////////////////////////////////////////////////////////////////////////////
//�ڑ����̐���
/////////////////////////////////////////////////////////////////////////////////
	//�v���[�X�ƃg�����W�V�����̐ڑ����쐬
	multimap<int,int> TfromP,TtoP;//�ߋ��ʉ߃g�[�N���p
	multimap<int,int> PtoT;//���L�v���[�X
	ap = vArcMap.begin();
	for(;ap!=vArcMap.end();++ap){
		switch(vObjMap[ap->second.vIn].type){//�ڑ����ŕ���
		case _PLACE:
			//�v���[�X����g�����W�V����
			if(vObjMap[ap->second.vOut].type==_TRANSITION
	//			|| vObjMap[ap->second.vOut].type==_MODULE
				){
				TfromP.insert(make_pair(ap->second.vOut,ap->second.vIn));
				PtoT.insert(make_pair(ap->second.vIn,ap->second.vOut));
			}
			break;
		case _TRANSITION:
//		case _MODULE:
			//�g�����W�V��������v���[�X
			if(vObjMap[ap->second.vOut].type==_PLACE){
				TtoP.insert(make_pair(ap->second.vIn,ap->second.vOut));
			}
			break;
		default:;
		}
	}

	//�v���[�Xfrom�v���[�X
	multimap<int,int> PfromP;
	multimap<int,int>::iterator t2pit,t4pit;
	tp = TranMap.begin();
	for(;tp!=TranMap.end();++tp){
		t2pit=TtoP.lower_bound(tp->first);
		for(;t2pit!=TtoP.upper_bound(tp->first);++t2pit){
			t4pit=TfromP.lower_bound(tp->first);
			for(;t4pit!=TfromP.upper_bound(tp->first);++t4pit){
				PfromP.insert(make_pair(t2pit->second,t4pit->second));
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////
//�ڑ����̐���end
/////////////////////////////////////////////////////////////////////////////////


*/

/*
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->Pp){
			if(PlaceMap[uit->second->Pp].GetOldColor()!=_WTOKEN){
				vErrPos = uit->second->Pp;
				throw "�E�[�W����̉ߋ��ʉ߃g�[�N�������ł��B���Ȃ�悤�Ƀg�[�N����z�u���Ă�������";
			}
		}
	}
*/

/*
/////////////////////////////////////////////////////////////////////////////////
//	�v���[�X�����L����g�����W�V�����̑S�ĂɃK�[�h�����L�q����Ă��邩�ǂ����`�F�b�N
/////////////////////////////////////////////////////////////////////////////////
	int pid;
	multimap<int,int>::iterator p2tit;
	set<int> atran;
	p2tit=PtoT.begin();
	for(;p2tit!=PtoT.end();){
		if(bound_count(PtoT,p2tit->first)==1){
			++p2tit;
			continue;
		}
		pid=p2tit->first;
		for(;p2tit!=PtoT.upper_bound(pid);++p2tit){
			atran.insert(p2tit->second);
		}
	}
	set<int>::iterator atit;
	atit=atran.begin();
	for(;atit!=atran.end();++atit){
//		TRACE("%d ",*atit);
		if(TranMap[*atit].astr.empty()){
			vErrPos=*atit;
			throw "�K�[�h�����L�q����Ă��܂���";
		}
	}
*/

}

void BSPetri::AnyTokenSet()
{

	//�S�Ẵg�[�N����j��
	map<int,bspPlace*>::iterator pit,pit_end;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		pit->second->Reset();
	}


	//	TRACE("�g�[�N����z�u\n");
	map<int,bspToken>::iterator tokit,tokit_end;
	tokit = TokenMap.begin();
	tokit_end = TokenMap.end();
	for(;tokit!=tokit_end;++tokit){
		if(PlaceMap.find(tokit->first)==PlaceMap.end()){
			TRACE("���݂��Ȃ��v���[�X�Ƀg�[�N�����z�u����Ă���. :�����G���[\n");
		}
		PlaceMap[tokit->first]
			->SetToken(tokit->second.Color, tokit->second.Value);
	}
}

void BSPetri::AnyTranSet()
{
	map<int,bspTransition*>::iterator tit,tit_end;
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition *tran = tit->second;
		//�ڑ�������

		tran->IOPlaceClear();
	
		//�g�����W�V�����ɐڑ�����Ă���A�[�N�̖{���������v���[�X���X�g�̔z���p�ӂ���
		tran->in_place_num = ArcOutMap.count(tit->first);
		tran->InPlaceList.resize(tran->in_place_num);

		tran->out_place_num = ArcInMap.count(tit->first);
//		if(module_name=="CAM") if(tit->first==222) TRACE("e %d\n",tran->out_place_num);
		tran->OutPlaceList.resize(tran->out_place_num);
	}

}

void BSPetri::PetComUK()
{

	map<int,bspUhzi*>::iterator uit;
	map<int,bspKemmakix*>::iterator kit;

	int errp=0;
	try{

		//	TRACE("Uhzi compile\n");
		//�E�[�W�̒l���Z�b�g
//		Trace("���͒l��ݒ肵�Ă��܂�...\r\n");

		uit = UhziMap.begin();
		for(;uit!=UhziMap.end();++uit){
			errp = uit->first;
			uit->second->Compile();
		}

	//	TRACE("max %d\n",Uhzi::datamax);

		//�P���}�L�̒l�����Z�b�g
//		Trace("�P���}�L�̐ݒ蒆...\r\n");
		kit = KemMap.begin();
		for(;kit!=KemMap.end();++kit){
			errp = kit->first;
			kit->second->Compile();
		}
	}
	catch(char * err){
		ErrPos = errp;
		throw err;
	}

}


//�Ƃɂ����A�[�N��ڑ�����
void BSPetri::AnyArcSet()
{

	map<int,bspArc>::iterator ait,ait_end;

#ifdef _DEBUG
	try{
		ait=ArcMap.begin();
		ait_end = ArcMap.end();
		for(;ait!=ait_end;++ait){
			if(ObjMap.find(ait->second.In)==ObjMap.end()){
				throw "�ڑ����̃I�u�W�F�N�g�����݂��܂���. :�����G���[\n";
			}
			if(ObjMap.find(ait->second.Out)==ObjMap.end()){
				throw "�ڑ���̃I�u�W�F�N�g�����݂��܂���. :�����G���[\n";
			}
			switch(ObjMap[ait->second.In].type){
			case _TRANSITION:
			case _MODULE:
				switch(ObjMap[ait->second.Out].type){
				case _PLACE: break;
				default:
					ErrPos = ait->second.Out;
					throw "���̐ڑ��͋�����Ă��܂���. :�����G���[";
				}
				break;
			case _PLACE:
				switch(ObjMap[ait->second.Out].type){
				case _TRANSITION:
				case _MODULE:
				case _KEMMAKIX: break;
				default:
					ErrPos=ait->second.Out;
					throw "�v���[�X����̐ڑ��͋�����Ă��܂���. :�����G���[";
				}
				break;
			case _UHZI:
				switch(ObjMap[ait->second.Out].type){
				case _PLACE: break;
				default:
					ErrPos=ait->second.Out;
					throw "�E�[�W����̐ڑ��͋�����Ă��܂���. :�����G���[";
				}
				break;
			case _KEMMAKIX:;
			default:
				ErrPos=ait->second.In;
				throw "���̃I�u�W�F�N�g����ڑ����邱�Ƃ͂ł��܂���. :�����G���[";

			}
		}
	}
	catch(char *err){
		throw err;
	}
#endif


	multimap<int,int>::iterator aioit;
	map<int,bspTransition*>::iterator tit,tit_end;
	try{
	//�g�����W�V�����ɐڑ�����Ă���A�[�N
	int i,j;
	string a_val;

	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		int tid = tit->first;
		bspTransition *tran = tit->second;
		//����
		aioit = ArcOutMap.lower_bound(tid);
		//�W���C���g�ԍ����傫�������猩��
		i=tran->in_place_num -1;
		//�ڑ�����Ă���v���[�X�����P�Ȃ�
		if(i==0 && tran->incount==1){
			tran->InPlaceList[0] = ArcMap[aioit->second].In;
			tran->in_arc_to_joint[aioit->second]=0;
			++aioit;
		}
		for(;aioit!=ArcOutMap.upper_bound(tid);++aioit){
			a_val = ArcMap[aioit->second].value_name;
			if(a_val.empty()){
				tran->InPlaceList[i] = ArcMap[aioit->second].In;
				tran->in_arc_to_joint[aioit->second]=i;
				--i;
			}
			else if(tran->in_var.find(a_val)!= tran->in_var.end()
				&& tran->in_var[a_val]-1<=i){
				j = tran->in_var[a_val] - 1;
				tran->InPlaceList[j] = ArcMap[aioit->second].In;
				tran->in_arc_to_joint[aioit->second] = j;
			}
			else throw "���͕ϐ��ɐڑ����Ă��������D";
		}
		if(i!=tran->in_var.size()-1){
			throw "�S�Ă̓��͕ϐ���ڑ����Ă��������D";
		}
	//	TRACE("in %d",i);
		
		//�o��
		aioit=ArcInMap.lower_bound(tid);
		i=tran->out_place_num -1;
		if(i==0 && tran->outcount==1){	
			tran->OutPlaceList[0] = ArcMap[aioit->second].Out;
			tran->out_arc_to_joint[aioit->second]=0;
			++aioit;
		}
		for(;aioit!=ArcInMap.upper_bound(tid);++aioit){
			a_val = ArcMap[aioit->second].value_name;
			if(a_val.empty()){
				tran->OutPlaceList[i] = ArcMap[aioit->second].Out;
				tran->out_arc_to_joint[aioit->second]=i;
				--i;
			}
			else if(tran->out_var.find(a_val)!= tran->out_var.end()
				&& tran->out_var[a_val]-1<=i){
				j=tran->out_var[a_val]-1;
				tran->OutPlaceList[j] = ArcMap[aioit->second].Out;
				tran->out_arc_to_joint[aioit->second]=j;
			}
			else throw "�o�͕ϐ��ɐڑ����Ă��������D";
		}
		if(i!=tran->out_var.size()-1){
			throw "�S�Ă̏o�͕ϐ���ڑ����Ă��������D";
		}

	//	TRACE("out %d",i);
	}

	}
	catch(char *err){
		ErrPos=tit->first;
//		TRACE("tra err %d\n",vErrPos);
		throw err;
	}


}

void BSPetri::AnyArcUKSet()
{
	multimap<int,int>::iterator aioit;

	//�E�[�W����̃A�[�N
	map<int,bspUhzi*>::iterator uit;
	try{
		uit=UhziMap.begin();
		for(;uit!=UhziMap.end();++uit){
			//�o��
			aioit = ArcInMap.lower_bound(uit->first);
			if(aioit==ArcInMap.upper_bound(uit->first)) continue;
			uit->second->Pp = ArcMap[aioit->second].Out;
			++aioit;
			if(aioit!=ArcInMap.upper_bound(uit->first)){
				throw "���ނ���̏o�͂͂ЂƂ݂̂ɂ��Ă��������D";
			}
/*
			aioit = ArcOutMap.lower_bound(uit->second.Pp);
			++aioit;
			if(aioit!=ArcOutMap.upper_bound(uit->second.Pp)){
				throw "���ނ��獇�����邱�Ƃ͂ł��܂���";
			}
*/			
		}
	}
	catch(char *err){
		ErrPos=uit->first;
		throw err;
	}

	//�P���}�L�ɐڑ�����Ă���A�[�N
	map<int,bspKemmakix*>::iterator kit;
	try{
		kit=KemMap.begin();
		for(;kit!=KemMap.end();++kit){
			aioit = ArcOutMap.lower_bound(kit->first);
			if(aioit==ArcOutMap.upper_bound(kit->first)) continue;
			kit->second->Pp = ArcMap[aioit->second].In;
			++aioit;
			if(aioit!=ArcOutMap.upper_bound(kit->first)){
				throw "��Ϸ�ւ̓��͂͂ЂƂ݂̂ɂ��Ă��������D";
			}
/*
			// ���W���[�����܂߂��I�𕪊�łł��邩������Ȃ�
			aioit = ArcInMap.lower_bound(kit->second->Pp);
			++aioit;
			if(aioit!=ArcInMap.upper_bound(kit->second->Pp)){
				throw "�����̹�Ϸ�ɐڑ����邱�Ƃ͂ł��܂���";
			}
*/			
		}
	}
	catch(char *err){
		ErrPos=kit->first;
		throw err;
	}

		
}


// ��Ϸ�����̂肹����
void BSPetri::PetSimUKReset()
{

	map<int,bspUhzi*>::iterator uit;
	map<int,bspKemmakix*>::iterator kit;

	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		uit->second->Reset();
	}

	kit = KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		kit->second->Reset();
	}

}

void BSPetri::PetSimUhziSet()
{
	if(UhziMap.size()==0) return;

	map<int, bspUhzi*>::iterator uit;
	//�E�[�W�Z�b�g

	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		uit->second->Set();
	}

}

//�ݒ肳�ꂽ�l���o���؂�����
int BSPetri::PetSimUhziCheck()
{
//	if(UhziMap.size()==0) return 0;

	int ret=0;
	map<int,bspUhzi*>::iterator uit;
	for(uit=UhziMap.begin();uit!=UhziMap.end();++uit){
		ret += uit->second->Check(PlaceMap);
	}
	return ret;
}

BOOL BSPetri::PetSimUhziIndustry()
{
//	if(UhziMap.size()==0) return 0;

	int ret=0;
	map<int, bspUhzi*>::iterator uit;
	//�E�[�W���g�[�N���𐶎Y

	for(uit=UhziMap.begin();uit!=UhziMap.end();++uit){
		ret +=uit->second->Industry(PlaceMap);
	}

	return ret;
}

int BSPetri::PetSimTraCheck()
{
	int ret=0,rnum;

	//�S�Ẵg�����W�V�����𔭉Ή\���`�F�b�N
	map<int, bspTransition*>::iterator tit,tit_end;
	try{

		if(traFireable==0){
			set<int>::iterator sit;
			rnum = 0;
			sit = IOTranSet.begin();
			for(;sit!=IOTranSet.end();++sit){
				rnum += TranMap[*sit]->Fireablecheck();
				if(rnum) break;
			}
			if(rnum==0) return 0;
		}

		tit=TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			rnum = tit->second->Fireablecheck();
			if(rnum){
				ret++;
	/*
				//���Ή\�X�g�b�v�I�v�V����
				if(simTranFireableStop.find(tit->first)!=simTranFireableStop.end()){
					ErrPos = tit->first;
					throw "���Ή\�X�g�b�v�I�v�V����";
				}
	*/
			}
		}
	}
	catch(char *err){
		ErrPos = tit->first;
		throw err;
	}

	return ret;
}

BOOL BSPetri::PetSimTraFire()
{
	//����
	int ret=0;

	map<int, bspTransition*>::iterator tit,tit_end;
	try{
		tit=TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			ret += tit->second->Fire();
		}
	}
	catch(char * err){
		ErrPos = tit->first;
		throw err;
	}

	return ret?1:0;
}

BOOL BSPetri::PetSimKemCheck()
{

	int ret=0;
	//TRACE("kemmakix\n");
	//�P���}�L���g�[�N��������ł��邩�`�F�b�N
	map<int,bspKemmakix*>::iterator kit,kit_end;
	try{

		kit=KemMap.begin();
		kit_end = KemMap.end();
		for(;kit!=kit_end;++kit){
			ret += kit->second->Check(PlaceMap);
		}
	}
	catch(char *err){
		ErrPos = kit->first;
		throw err;//�܂�Ȃ�
	}
	return ret;
}
BOOL BSPetri::PetSimKemConsum()
{

	int ret=0;
	//TRACE("kemmakix\n");
	//�P���}�L���g�[�N��������
	map<int,bspKemmakix*>::iterator kit;
	try{
		for(kit=KemMap.begin();kit!=KemMap.end();++kit){
			ret += kit->second->Consum(PlaceMap,confSimBinValue);
		}
	}
	catch(char *err){
		ErrPos = kit->first;
		throw err;//�܂�Ȃ�
	}
	return ret;
}

void BSPetri::RealPlaceClear()
{
	map<int,bspPlace*>::iterator pit,pit_end;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		pit->second->real_place = NULL;
	}
}

/// EOF ///
