
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

	// 最初は全てのﾄﾗﾝｼﾞｼｮﾝを探索
	traFireable = true;
	ofpt = 0.0;

}

//ペトリネットのコンパイル＆チェック
void BSPetri::PetCom()
{

	PetSimReset();

	//コマンド部分に映してもいい？
	//	TRACE("compile\n");
	
	map<int,bspToken>::iterator tokit,tokit_end;

	AnyTokenSet();


	multimap<int,int>::iterator aioit;
	
	//トークンの配置チェック
	try{
		int tid, pid;
		tokit = TokenMap.begin();
		tokit_end = TokenMap.end();
		for(;tokit!=tokit_end;++tokit){
			if(tokit->second.Color==_BTOKEN){

				// ﾌﾟﾚｰｽの入力が1であるか
				if(ArcOutMap.count(tokit->first) != 1){
					ErrPos = tokit->first;
					throw "合流ﾌﾟﾚｰｽに黒ﾄｰｸﾝを配置することはできません．";
				}

				aioit = ArcOutMap.lower_bound(tokit->first);
				tid = ArcMap[ aioit->second ].In;

				// パストラであるかどうか
				if(ArcOutMap.count(tid) != 1 || ArcInMap.count(tid) != 1){
					ErrPos = tid;
					throw "ﾊﾟｽﾄﾗﾝｼﾞｼｮﾝである必要があります．";
				}

				aioit = ArcOutMap.lower_bound( tid );
				pid = ArcMap[ aioit->second ].In;
				if( TokenMap.find( pid ) == TokenMap.end()
					|| TokenMap[ pid ].Color == _BTOKEN){
					ErrPos = pid;
					throw "黒ﾄｰｸﾝの直後に白ﾄｰｸﾝを配置してください．";
				}

				// ﾌﾟﾚｰｽの出力が1かどうか
				if(ArcInMap.count(pid)!=1){
					ErrPos = pid;
					throw "分岐点に白ﾄｰｸﾝを配置することはできません．";
				}

				PlaceMap[pid]->SetTokenValue( tokit->second.Value );//念のため
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
			//トランジション関数から変数を抽出
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
	//ウージやケムマキに接続されているプレースに
	//初期トークンを配置することはできない
	uit=UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		if(TokenMap.find(uit->second->Pp)!=TokenMap.end()
			//&& vTokenMap[uit->second.Pp].Color==_BTOKEN
			){
			ErrPos=uit->second->Pp;
			throw "ｳｰｼﾞから接続されているﾌﾟﾚｰｽにﾄｰｸﾝを配置することはできません．";
		}
	}

	//ケムマキはしろトークンが嫌い
	kit=KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		if(TokenMap.find(kit->second->Pp)!=TokenMap.end()
		//	&& TokenMap[kit->second.Pp].Color==_WTOKEN
			){
			ErrPos = kit->second->Pp;
			throw "ｹﾑﾏｷに接続しているﾌﾟﾚｰｽにﾄｰｸﾝを配置することはできません．";
		}
	}

	map<int,string>::iterator mit,mit_end;
	mit = ModMap.begin();
	mit_end = ModMap.end();
	for(;mit!=mit_end;++mit){
		aioit = ArcOutMap.lower_bound(mit->first);
		for(;aioit!=ArcOutMap.upper_bound(mit->first);++aioit){
			if(TokenMap.find( ArcMap[ aioit->second ].In ) != TokenMap.end()){
				throw "ﾓｼﾞｭｰﾙに接続しているﾌﾟﾚｰｽにﾄｰｸﾝを配置することはできません．";
			}
		}
		aioit = ArcInMap.lower_bound(mit->first);
		for(;aioit!=ArcInMap.upper_bound(mit->first);++aioit){
			if(TokenMap.find( ArcMap[ aioit->second ].Out ) != TokenMap.end()){
				throw "ﾓｼﾞｭｰﾙに接続しているﾌﾟﾚｰｽにﾄｰｸﾝを配置することはできません．";
			}
		}
	}



	try{
		//トランジション関数のコンパイル
//	TRACE("Compile...\n");
//		Trace("トランジション関数のコンパイル中...%d\r\n",TranList.GetCount());
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


	// 入出力につながってるﾄﾗﾝｼﾞｼｮﾝを登録
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
	// トランジションメモリ読み込み
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
//接続情報の生成
/////////////////////////////////////////////////////////////////////////////////
	//プレースとトランジションの接続情報作成
	multimap<int,int> TfromP,TtoP;//過去通過トークン用
	multimap<int,int> PtoT;//共有プレース
	ap = vArcMap.begin();
	for(;ap!=vArcMap.end();++ap){
		switch(vObjMap[ap->second.vIn].type){//接続元で分岐
		case _PLACE:
			//プレースからトランジション
			if(vObjMap[ap->second.vOut].type==_TRANSITION
	//			|| vObjMap[ap->second.vOut].type==_MODULE
				){
				TfromP.insert(make_pair(ap->second.vOut,ap->second.vIn));
				PtoT.insert(make_pair(ap->second.vIn,ap->second.vOut));
			}
			break;
		case _TRANSITION:
//		case _MODULE:
			//トランジションからプレース
			if(vObjMap[ap->second.vOut].type==_PLACE){
				TtoP.insert(make_pair(ap->second.vIn,ap->second.vOut));
			}
			break;
		default:;
		}
	}

	//プレースfromプレース
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
//接続情報の生成end
/////////////////////////////////////////////////////////////////////////////////


*/

/*
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->Pp){
			if(PlaceMap[uit->second->Pp].GetOldColor()!=_WTOKEN){
				vErrPos = uit->second->Pp;
				throw "ウージ直後の過去通過トークンが黒です。白なるようにトークンを配置してください";
			}
		}
	}
*/

/*
/////////////////////////////////////////////////////////////////////////////////
//	プレースを共有するトランジションの全てにガード式が記述されているかどうかチェック
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
			throw "ガード式が記述されていません";
		}
	}
*/

}

void BSPetri::AnyTokenSet()
{

	//全てのトークンを破棄
	map<int,bspPlace*>::iterator pit,pit_end;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		pit->second->Reset();
	}


	//	TRACE("トークンを配置\n");
	map<int,bspToken>::iterator tokit,tokit_end;
	tokit = TokenMap.begin();
	tokit_end = TokenMap.end();
	for(;tokit!=tokit_end;++tokit){
		if(PlaceMap.find(tokit->first)==PlaceMap.end()){
			TRACE("存在しないプレースにトークンが配置されている. :内部エラー\n");
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
		//接続を解除

		tran->IOPlaceClear();
	
		//トランジションに接続されているアークの本数分だけプレースリストの配列を用意する
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
		//ウージの値をセット
//		Trace("入力値を設定しています...\r\n");

		uit = UhziMap.begin();
		for(;uit!=UhziMap.end();++uit){
			errp = uit->first;
			uit->second->Compile();
		}

	//	TRACE("max %d\n",Uhzi::datamax);

		//ケムマキの値をリセット
//		Trace("ケムマキの設定中...\r\n");
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


//とにかくアークを接続する
void BSPetri::AnyArcSet()
{

	map<int,bspArc>::iterator ait,ait_end;

#ifdef _DEBUG
	try{
		ait=ArcMap.begin();
		ait_end = ArcMap.end();
		for(;ait!=ait_end;++ait){
			if(ObjMap.find(ait->second.In)==ObjMap.end()){
				throw "接続元のオブジェクトが存在しません. :内部エラー\n";
			}
			if(ObjMap.find(ait->second.Out)==ObjMap.end()){
				throw "接続先のオブジェクトが存在しません. :内部エラー\n";
			}
			switch(ObjMap[ait->second.In].type){
			case _TRANSITION:
			case _MODULE:
				switch(ObjMap[ait->second.Out].type){
				case _PLACE: break;
				default:
					ErrPos = ait->second.Out;
					throw "この接続は許されていません. :内部エラー";
				}
				break;
			case _PLACE:
				switch(ObjMap[ait->second.Out].type){
				case _TRANSITION:
				case _MODULE:
				case _KEMMAKIX: break;
				default:
					ErrPos=ait->second.Out;
					throw "プレースからの接続は許されていません. :内部エラー";
				}
				break;
			case _UHZI:
				switch(ObjMap[ait->second.Out].type){
				case _PLACE: break;
				default:
					ErrPos=ait->second.Out;
					throw "ウージからの接続は許可されていません. :内部エラー";
				}
				break;
			case _KEMMAKIX:;
			default:
				ErrPos=ait->second.In;
				throw "このオブジェクトから接続することはできません. :内部エラー";

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
	//トランジションに接続されているアーク
	int i,j;
	string a_val;

	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		int tid = tit->first;
		bspTransition *tran = tit->second;
		//入力
		aioit = ArcOutMap.lower_bound(tid);
		//ジョイント番号が大きい方から見る
		i=tran->in_place_num -1;
		//接続されているプレース数が１なら
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
			else throw "入力変数に接続してください．";
		}
		if(i!=tran->in_var.size()-1){
			throw "全ての入力変数を接続してください．";
		}
	//	TRACE("in %d",i);
		
		//出力
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
			else throw "出力変数に接続してください．";
		}
		if(i!=tran->out_var.size()-1){
			throw "全ての出力変数を接続してください．";
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

	//ウージからのアーク
	map<int,bspUhzi*>::iterator uit;
	try{
		uit=UhziMap.begin();
		for(;uit!=UhziMap.end();++uit){
			//出力
			aioit = ArcInMap.lower_bound(uit->first);
			if(aioit==ArcInMap.upper_bound(uit->first)) continue;
			uit->second->Pp = ArcMap[aioit->second].Out;
			++aioit;
			if(aioit!=ArcInMap.upper_bound(uit->first)){
				throw "ｳｰｼﾞからの出力はひとつのみにしてください．";
			}
/*
			aioit = ArcOutMap.lower_bound(uit->second.Pp);
			++aioit;
			if(aioit!=ArcOutMap.upper_bound(uit->second.Pp)){
				throw "ｳｰｼﾞから合流することはできません";
			}
*/			
		}
	}
	catch(char *err){
		ErrPos=uit->first;
		throw err;
	}

	//ケムマキに接続されているアーク
	map<int,bspKemmakix*>::iterator kit;
	try{
		kit=KemMap.begin();
		for(;kit!=KemMap.end();++kit){
			aioit = ArcOutMap.lower_bound(kit->first);
			if(aioit==ArcOutMap.upper_bound(kit->first)) continue;
			kit->second->Pp = ArcMap[aioit->second].In;
			++aioit;
			if(aioit!=ArcOutMap.upper_bound(kit->first)){
				throw "ｹﾑﾏｷへの入力はひとつのみにしてください．";
			}
/*
			// モジュールを含めた選択分岐でできるかもしれない
			aioit = ArcInMap.lower_bound(kit->second->Pp);
			++aioit;
			if(aioit!=ArcInMap.upper_bound(kit->second->Pp)){
				throw "複数のｹﾑﾏｷに接続することはできません";
			}
*/			
		}
	}
	catch(char *err){
		ErrPos=kit->first;
		throw err;
	}

		
}


// ｹﾑﾏｷうじのりせっと
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
	//ウージセット

	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		uit->second->Set();
	}

}

//設定された値を出し切ったか
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
	//ウージがトークンを生産

	for(uit=UhziMap.begin();uit!=UhziMap.end();++uit){
		ret +=uit->second->Industry(PlaceMap);
	}

	return ret;
}

int BSPetri::PetSimTraCheck()
{
	int ret=0,rnum;

	//全てのトランジションを発火可能かチェック
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
				//発火可能ストップオプション
				if(simTranFireableStop.find(tit->first)!=simTranFireableStop.end()){
					ErrPos = tit->first;
					throw "発火可能ストップオプション";
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
	//発火
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
	//ケムマキがトークンを消費できるかチェック
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
		throw err;//まるなげ
	}
	return ret;
}
BOOL BSPetri::PetSimKemConsum()
{

	int ret=0;
	//TRACE("kemmakix\n");
	//ケムマキがトークンを消費
	map<int,bspKemmakix*>::iterator kit;
	try{
		for(kit=KemMap.begin();kit!=KemMap.end();++kit){
			ret += kit->second->Consum(PlaceMap,confSimBinValue);
		}
	}
	catch(char *err){
		ErrPos = kit->first;
		throw err;//まるなげ
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
