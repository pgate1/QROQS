
#include "stdafx.h"

#include "BSPetriTree.h"

void BSPetriTree::AttachProg(void (*prog)(int abs,int add))
{
	progset = prog;
}

void BSPetriTree::Compile()
{
	f_check_halt = 0;
	err_mname.erase();

	try{

	struct func{
		static void bsp_compile(
			map<string,BSPetri*> &bsps,
			BSPetri* cur_bsp,
			BSPetriTree *bspt)
		{
			map<int,string>::iterator mit,mit_end;

			multimap<int,int>::iterator aioit;
			map<int,bspUhzi*>::iterator uit;
			map<int,bspKemmakix*>::iterator kit;
			BSPetri *child_bsp=NULL;
			string a_name;

			bspt->progset(0,1);
			//trace("	%s\n", cur_bsp->module_name.c_str());
		//	TRACE("%s\n",cur_bsp->module_name.c_str());
			if(bspt->f_check_halt) throw "";

			try{
				cur_bsp->traNum = cur_bsp->TranMap.size();

				//���ʃ��W���[����S�Đڑ�
				mit = cur_bsp->ModMap.begin();
				mit_end = cur_bsp->ModMap.end();
				for(;mit!=mit_end;++mit){
					if(mit->second.empty()){
						cur_bsp->ErrPos = mit->first;
						throw "Ӽޭ�ق�ݒ肵�Ă��������D";
					}
				
					child_bsp = cur_bsp->ModuleMap[ mit->first ] = new BSPetri();
					*child_bsp = *bsps[ mit->second ];
					//����ɉ��ʃ��W���[��
					bsp_compile(bsps, child_bsp, bspt);

					cur_bsp->traNum += child_bsp->traNum;

					//����
					aioit = cur_bsp->ArcOutMap.lower_bound( mit->first );
					for(;aioit!=cur_bsp->ArcOutMap.upper_bound( mit->first );++aioit){
						a_name = cur_bsp->ArcMap[ aioit->second ].value_name;
						//���W���[���ւ̃A�[�N�ϐ����ݒ肳��Ă��Ȃ�
						if(a_name.empty()){
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ق̓��͂ւ̱���ڑ����s���Ă��������D";
						}
						uit = child_bsp->UhziMap.begin();
						for(;uit!=child_bsp->UhziMap.end();++uit){
							bspUhzi *uhzi = uit->second;
							if(uhzi->input_name == a_name){
								if(uhzi->Pp==0){
									child_bsp->ErrPos = uit->first;
									cur_bsp->ErrPos = mit->first;
									throw "���ނ���ڰ��ɐڑ����Ă��������D";
								}
								if(uhzi->addp==0){
									child_bsp->ErrPos = uit->first;
									cur_bsp->ErrPos = mit->first;
									throw "ڼ޽����ނɂ͐ڑ��ł��܂���D";
								}
								cur_bsp->PlaceMap[ cur_bsp->ArcMap[ aioit->second ].In ]->real_place
									= child_bsp->PlaceMap[uhzi->Pp]->GetRealPlace();
								uhzi->Pp = 0;
								break;
							}
						}
						//��������
						if(uit == child_bsp->UhziMap.end()){
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ٓ��ɓ��͂ƑΉ����鳰�ނ�����܂���D";
						}
						else{
							++uit;
							for(;uit!=child_bsp->UhziMap.end();++uit){
								if(uit->second->input_name == a_name){
									child_bsp->ErrPos = uit->first;
									cur_bsp->ErrPos = mit->first;
									throw "�������O�����³��ނ��������݂��܂��D";
								}
							}
						}
					}
					
					uit = child_bsp->UhziMap.begin();
					for(;uit!=child_bsp->UhziMap.end();++uit){
						if(uit->second->Pp!=0 && uit->second->addp==1){
							child_bsp->ErrPos = uit->first;
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ٓ��ɐڑ�����Ă��Ȃ����ނ����݂��܂��Dڼ޽����ވȊO�̳��ނ͑S�ď��Ӽޭ�ق���ڑ����Ă��������D";
						}
					}


					//�o��
					aioit = cur_bsp->ArcInMap.lower_bound( mit->first );
					for(;aioit!=cur_bsp->ArcInMap.upper_bound( mit->first );++aioit){
						a_name = cur_bsp->ArcMap[ aioit->second ].value_name;
						//���W���[������̃A�[�N�ϐ����ݒ肳��Ă��Ȃ�
						if(a_name.empty()){
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ُo�̱͂���ڑ����s���Ă��������D";
						}
						kit = child_bsp->KemMap.begin();
						for(;kit!=child_bsp->KemMap.end();++kit){
							if(kit->second->dline<=1
								&& kit->second->output_name == a_name){
								if(kit->second->Pp==0){
									child_bsp->ErrPos = kit->first;
									cur_bsp->ErrPos = mit->first;
									throw "��ڰ�����ڑ����Ă��������D";
								}
								child_bsp->PlaceMap[kit->second->Pp]->real_place
									= cur_bsp->PlaceMap[ cur_bsp->ArcMap[ aioit->second ].Out ]->GetRealPlace();
								kit->second->Pp=0;
								break;
							}
						}
						//��������
						if(kit == child_bsp->KemMap.end()){
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ٓ��ɏo�ͱ���ƑΉ������Ϸ������܂���D";
						}
						else{
							++kit;
							for(;kit!=child_bsp->KemMap.end();++kit){
								if(kit->second->output_name == a_name){
									child_bsp->ErrPos = kit->first;
									cur_bsp->ErrPos = mit->first;
									throw "�������O�����¹�Ϸ���������݂��܂��D";
								}
							}
						}
					}

					kit = child_bsp->KemMap.begin();
					for(;kit!=child_bsp->KemMap.end();++kit){
						if(kit->second->Pp!=0 && kit->second->dline<2){
							child_bsp->ErrPos = kit->first;
							cur_bsp->ErrPos = mit->first;
							throw "Ӽޭ�ٓ��̏o�͹�Ϸ�͑S�ĊO���ɐڑ����Ă��������D";
						}
					}

				}

			//	TRACE("%s\n",cur_bsp->module_name.c_str());
			}
			catch(char *err){
				if(bspt->f_check_halt){
					throw "";
				}
				else{
					RetEx ret;
					ret.err = err;
					ret.mname = cur_bsp->module_name;
					ret.err_id = cur_bsp->ErrPos;
					ret.child_err_id = child_bsp ? child_bsp->ErrPos : 0;
					throw ret;
				}
			}
		}

		static void transition_joint(
			BSPetri* cur_bsp,
			BSPetriTree *bspt)
		{
			if(bspt->f_check_halt) throw "";

			map<int,bspTransition*>::iterator tit,tit_end;
			int i;
			tit = cur_bsp->TranMap.begin();
			tit_end = cur_bsp->TranMap.end();
			for(;tit!=tit_end;++tit){
				bspTransition &tran = *(tit->second);
				tran.InPlaces.resize(tran.in_place_num);
				tran.OutPlaces.resize(tran.out_place_num);
				for(i=0;i<tran.in_place_num;i++){
					tran.InPlaces[i] = cur_bsp->PlaceMap[tran.InPlaceList[i]]->GetRealPlace();
				}
				for(i=0;i<tran.out_place_num;i++){
					tran.OutPlaces[i] = cur_bsp->PlaceMap[tran.OutPlaceList[i]]->GetRealPlace();
				}
			}

			//����ɉ��ʃ��W���[��
			map<int,BSPetri*>::iterator modit,modit_end;
			modit = cur_bsp->ModuleMap.begin();
			modit_end = cur_bsp->ModuleMap.end();
			for(;modit!=modit_end;++modit){
				transition_joint( modit->second, bspt);
			}
		}
	};

	progset(0,0);

	// root�ȉ��̃��W���[����S�ăR���p�C��
	set<string> mset;
	UnderMnameGet(this, GetRootBSP(), mset);
	mset.insert(root_name);
	BSPCompile(mset);

	progset(5,0);

	// ���W���[���̐ڑ�
//	Trace("bsps joint...\n");
	func::bsp_compile(bsps, GetRootBSP(), this);

	//trace_buf += "tracsition joint...\n";
	func::transition_joint(GetRootBSP(), this);

	}
	catch(RetEx err_ret){
//		TRACE("eret %s\n",err_ret.mname.c_str());
		err_mname = err_ret.mname;
		bsps[ err_mname ]->ErrPos = err_ret.err_id;
		if(err_ret.child_err_id){
			bsps[ bsps[err_mname]->ModMap[err_ret.err_id] ]->ErrPos = err_ret.child_err_id;
		}
		throw err_ret.err;
	}
	catch(char *err){
		throw err;
	}

//	TRACE("compile end.\n");
	progset(100,0);
}

void BSPetriTree::BSPCompile(set<string> &mset)
{

	// �֘A���郂�W���[����S�ăR���p�C��
//	TRACE("bsps compile...\n");
	set<string>::iterator sit;
	sit = mset.begin();
	for(;sit!=mset.end();++sit){
		try{
			if(isdigit((*sit)[0])){
				throw "Ӽޭ�ٖ��͉p������n�߂Ă��������D";
			}
			//TRACE("%s\n",sit->c_str());

			bsps[*sit]->confSimBinValue = confSimBinValue;
		//	TRACE("%d\n",confSimBinValue);

			bsps[*sit]->RealPlaceClear();
			bsps[*sit]->PetCom();
		}
		catch(char *err){
			RetEx ret;
			ret.err = err;
			ret.mname = *sit;
			ret.err_id = bsps[*sit]->ErrPos;
		//	TRACE("%s %s\n",(*sit).c_str(),err);
			throw ret;
		}
	}
}

void BSPetriTree::PetSimUhziSet()
{

	struct func{
		static void sub_bsp(
			BSPetri* cur_bsp)
		{
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sub_bsp( bspit->second );
			}
			cur_bsp->PetSimUhziSet();
		}
	};

	func::sub_bsp( GetRootBSP() );

}

BOOL BSPetriTree::PetSimUhziIndustry()
{

	struct func{
		static BOOL sub_bsp(
			BSPetri* cur_bsp)
		{
			int ret=0;
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret+=sub_bsp( bspit->second );
			}
			ret+=cur_bsp->PetSimUhziIndustry();
			return ret;
		}
	};

	return func::sub_bsp( GetRootBSP() );

}

BOOL BSPetriTree::PetSimKemConsum()
{


	struct func{
		static BOOL sub_bsp(
			BSPetri* cur_bsp)
		{
			int ret=0;
			ret += cur_bsp->PetSimKemConsum();

			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret += sub_bsp( bspit->second );
			}

			return ret;
		}
	};

	return func::sub_bsp( GetRootBSP() );

}


BOOL BSPetriTree::PetSimKemCheck()
{
	int ret=0;

	struct func{
		static BOOL sub_bsp(
			BSPetri* cur_bsp)
		{
			int ret=0;
			ret += cur_bsp->PetSimKemCheck();

			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret += sub_bsp( bspit->second );
			}

			return ret;
		}
	};

	return func::sub_bsp( GetRootBSP() );
}


int BSPetriTree::PetSimTraCheck()
{
	int ret=0;

	struct func{
		static int sub_bsp(
			BSPetri* cur_bsp)
		{

			// ���ʃ��W���[���̔��΃`�F�b�N���s���Ă���
			int rets=0;
			int ret;
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret = sub_bsp( bspit->second );
				rets += ret;
			}
			
			if(cur_bsp->traFireable==0) cur_bsp->traFireable = rets;
			rets += cur_bsp->PetSimTraCheck();
			cur_bsp->traFireable = rets;

			return rets;
		}
	};


	ret = func::sub_bsp( GetRootBSP() );


	return ret;
}

BOOL BSPetriTree::PetSimTraFire()
{
	int ret=0;

	struct func{
		static BOOL sub_bsp(
			BSPetri* cur_bsp)
		{
			if(cur_bsp->traFireable==0) return 0;

			int rets=0,ret;
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret = sub_bsp( bspit->second );
				rets += ret;
			}
			rets += cur_bsp->PetSimTraFire();
			return rets;
		}
	};

	ret = func::sub_bsp( GetRootBSP() );
	return ret?1:0;

}

int BSPetriTree::PetSimUhziCheck()
{
	int ret = 0;
	struct func{
		static int sub_bsp(
			BSPetri* cur_bsp)
		{
			int ret = 0;
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				ret += sub_bsp( bspit->second );
			}
			ret += cur_bsp->PetSimUhziCheck();
			return ret;
		}
	};

	ret += func::sub_bsp( GetRootBSP() );
	return ret;
}

// �Эڰ��݂ŁA���޹�Ϸ�̏�Ԃ����Z�b�g
void BSPetriTree::PetSimUKReset()
{
	struct func{
		static void sub_bsp(
			BSPetri* cur_bsp)
		{
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sub_bsp( bspit->second );
			}
			cur_bsp->PetSimUKReset();
		}
	};

	func::sub_bsp( GetRootBSP() );
}


// �Эڰ��݂̏�Ԃ����Z�b�g
void BSPetriTree::PetSimReset()
{
	struct func{
		static void sub_bsp(
			BSPetri* cur_bsp)
		{
			map<int,BSPetri*>::iterator bspit,bspit_end;
			bspit = cur_bsp->ModuleMap.begin();
			bspit_end = cur_bsp->ModuleMap.end();
			for(;bspit!=bspit_end;++bspit){
				sub_bsp( bspit->second );
			}
			cur_bsp->PetSimReset();
		}
	};

	func::sub_bsp( GetRootBSP() );

}

void BSPetriTree::PetSimBlast(int pm)
{
	pm = 2;
	int roop,roop_max;
	int ret;
	roop_max = GetRootBSP()->confSimMaxTurn;
	for(roop=0;;++roop){

		if(roop>roop_max){
			throw "�Эڰ��݂��������邽�߈�U��~���܂����D";
		}
		
		//if(pm == 2){
			ret =0;
			ret += PetSimKemConsum();
			ret += PetSimUhziIndustry();
			ret += PetSimTraCheck();
			ret += PetSimTraFire();
			ret += PetSimUhziCheck();
			ret += PetSimKemCheck();
			if(ret==0) throw "�Эڰ��݂��I�����܂��D";
	//	}

	}
}

void BSPetriTree::HaltSimulation()
{
	map<int,BSPetri*>::iterator mit,mit_end;
	mit=GetRootBSP()->ModuleMap.begin();
	mit_end = GetRootBSP()->ModuleMap.end();
	for(;mit!=mit_end;++mit){
		delete mit->second;
	}
	GetRootBSP()->ModuleMap.clear();
}

/// EOF ///
