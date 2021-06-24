
#include "stdafx.h"

#include "BSPetri.h"
#include"QROQS.h"
#include"other.h"

BSPetri::BSPetri()
{
	PetClear();
	ModifiedFlag = 0;
}

BSPetri::BSPetri(const BSPetri &bsp)
{
	*this = bsp;
}

BSPetri & BSPetri::operator = (const BSPetri &bsp)
{
	if(this != &bsp){

	//	TRACE("bsp copy %d\n",rand());

		PetClear();
		ObjMap = bsp.ObjMap;
		TokenMap = bsp.TokenMap;

		ArcInMap = bsp.ArcInMap;
		ArcOutMap = bsp.ArcOutMap;
		ArcMap = bsp.ArcMap;

		map<int,bspPlace*>::const_iterator pit, pit_end;
		pit = bsp.PlaceMap.begin();
		pit_end = bsp.PlaceMap.end();
		for(;pit!=pit_end;++pit){
			PlaceMap[pit->first] = new bspPlace;
			*(PlaceMap[pit->first]) = *(pit->second);
		}

		map<int,bspTransition*>::const_iterator tit, tit_end;
		tit = bsp.TranMap.begin();
		tit_end = bsp.TranMap.end();
		for(;tit!=tit_end;++tit){
			TranMap[tit->first] = new bspTransition;
			*(TranMap[tit->first]) = *(tit->second);
		}

		map<int,bspUhzi*>::const_iterator uit, uit_end;
		uit = bsp.UhziMap.begin();
		uit_end = bsp.UhziMap.end();
		for(;uit!=uit_end;++uit){
			UhziMap[uit->first] = new bspUhzi;
			*UhziMap[uit->first] = *(uit->second);
		}

		map<int,bspKemmakix*>::const_iterator kit, kit_end;
		kit = bsp.KemMap.begin();
		kit_end = bsp.KemMap.end();
		for(;kit!=kit_end;++kit){
			KemMap[kit->first] = new bspKemmakix;
			*KemMap[kit->first] = *(kit->second);
		}

		ModMap = bsp.ModMap;
		Memo = bsp.Memo;

		ObjId = bsp.ObjId;
		ArcId = bsp.ArcId;

		ErrPos = bsp.ErrPos;//コンパイルでエラーを起こしたやつ
		globalRect = bsp.globalRect;
		objRect = bsp.objRect;
		Globalpoint = bsp.Globalpoint;

		confSimMaxTurn = bsp.confSimMaxTurn;
		confSimBinValue = bsp.confSimBinValue;

		simTranFireableStop=bsp.simTranFireableStop;

		module_name = bsp.module_name;
		ModifiedFlag = bsp.ModifiedFlag;

		TextMap = bsp.TextMap;
		intrack = bsp.intrack;
		trackRect = bsp.trackRect;

		traFireable = bsp.traFireable;
		traNum = bsp.traNum;

		IOTranSet = bsp.IOTranSet;

		viewDiam = bsp.viewDiam;

#ifndef _LIGHT
		//verilog
		verilog_out_dir=bsp.verilog_out_dir;
		vObjMap.clear();
		vArcMap.clear();
		vArcMapId=1;
		delay_slack = bsp.delay_slack;

	// C
		func_in_index_transrate.clear();
		func_out_index_transrate.clear();
#endif

	}
	return *this;
}

#ifdef _DEBUG
void BSPetri::arc_check()
{
	multimap<int,int>::iterator aioit, aioit_end;
	aioit = ArcInMap.begin();
	aioit_end = ArcInMap.end();
	for(; aioit!=aioit_end; ++aioit){
		if(ObjMap.find(aioit->first)==ObjMap.end()){
			TRACE("arcの入力のobjectがない：内部error\n");
		}
		if(ArcMap.find(aioit->second)==ArcMap.end()){
			TRACE("入力側のarcデータが無い %d　：内部error\n", aioit->second);
		}
	}
	aioit=ArcOutMap.begin();
	aioit_end = ArcOutMap.end();
	for(; aioit!=aioit_end; ++aioit){
		if(ObjMap.find(aioit->first)==ObjMap.end()){
			TRACE("アークの出力のブジェクトがない：内部エラー\n");
		}
		if(ArcMap.find(aioit->second)==ArcMap.end()){
			TRACE("出力側のアークデータが無い %d　：内部エラー\n",aioit->second);
		}
	}
}
#endif

#ifdef _DEBUG
void BSPetri::obj_check()
{

	map<int,bspObject>::iterator oit;
	oit=ObjMap.begin();
	for(;oit!=ObjMap.end();++oit){
		switch(oit->second.type){
		case _PLACE:break;
		case _TRANSITION:
			if(TranMap.find(oit->first)==TranMap.end()
				|| TranMap[oit->first]==0){
				TRACE("トランジションデータが存在しません：内部エラー\n");
				continue;
			}
			break;
		case _UHZI:break;
		case _KEMMAKIX:break;
		case _MODULE:break;
		default:
			TRACE("データが存在しないオブジェクトだ！：内部エラー\n");
			ObjMap.erase(oit);
			oit=ObjMap.end();
			break;
		}
	}
}
#endif

int BSPetri::dObjAdd(CPoint gpoint,int obj)
{
	ModifiedFlag = 1;

	switch(obj){
	case _PLACE:
		PlaceMap[ObjId] = new bspPlace;
		break;
	case _TRANSITION:
		TranMap[ObjId] = new bspTransition;//create
		break;
	case _UHZI:
		UhziMap[ObjId] = new bspUhzi;
		break;
	case _KEMMAKIX:
		KemMap[ObjId] = new bspKemmakix;
		break;
	case _MODULE:
		ModMap[ObjId];
		break;
	default:
		TRACE("追加できないオブジェクトです:内部エラー\n");
		return 0;
//		throw "追加できないオブジェクトです:内部エラー";
	}

	ObjMap[ObjId] = bspObject(obj, gpoint);

	//なんとなく…
	EnlargeGlobalRect(gpoint.x,gpoint.y);

	return ObjId++;
}

//dデータのリストから削除する
void BSPetri::dObjRemove(int rp)
{

	if(ObjMap.find(rp)==ObjMap.end()){
		TRACE("存在しないオブジェクトをけそうとしている：内部エラー\n");
		return;
	}

	ModifiedFlag = 1;

	switch(ObjMap[rp].type){
	case _PLACE:
	//	TRACE("p erase %d\n",rp);
		delete PlaceMap[rp];
		PlaceMap.erase(rp);
		break;
	case _TRANSITION:
		delete TranMap[rp];
		TranMap.erase(rp);
		break;
	case _UHZI:
		delete UhziMap[rp];
		UhziMap.erase(rp);
		break;
	case _KEMMAKIX:
		delete KemMap[rp];
		KemMap.erase(rp);
		break;
	case _MODULE:
		ModMap.erase(rp);
		break;
	default:
		TRACE( "ありえない削除が行われようとしている…：内部エラー");
		return;
	//	throw "ありえない削除が行われようとしている…：内部エラー";
	}

	TextMap.erase(rp);

	ObjMap.erase(rp);
}



int BSPetri::ArcChain(int vArcBase, int vArcFollow, vector<CPoint> *varcBwt,string v_name)
{
//	TRACE("base %d follow %d\n",vArcBase, vArcFollow);
	if(vArcBase==0 || vArcFollow==0) return 0;

	ModifiedFlag = 1;
	multimap<int,int>::iterator iap,oap,iap_end;

	//同じアークならば上書き
	iap=ArcInMap.lower_bound(vArcBase);
	iap_end = ArcInMap.upper_bound(vArcBase);
	for(;iap!=iap_end;++iap){
		if(ArcMap[iap->second].Out==vArcFollow){
		//	TRACE("arc over write %d\n",iap->second);
			ArcMap[iap->second].SetBwt(varcBwt);
			ArcMap[iap->second].value_name = v_name;
			return iap->second;
		}
	}

	//逆のアークならば削除
	if(DelArc(vArcFollow, vArcBase)==0){
	//	TRACE("arc del\n");
		return 0;
	}

	//ウージケムマキの接続制限
	if(ObjMap[vArcBase].type==_PLACE
		&& ObjMap[vArcFollow].type==_UHZI
	|| ObjMap[vArcBase].type==_KEMMAKIX
		&& ObjMap[vArcFollow].type==_PLACE){
		return 0;
	}

	//同じも逆も無ければ追加
//	TRACE("arc add\n");
	ArcInMap.insert(make_pair<int,int>(vArcBase,ArcId));
	ArcOutMap.insert(make_pair<int,int>(vArcFollow,ArcId));
	ArcMap[ArcId] = bspArc(vArcBase, vArcFollow, v_name);
	ArcMap[ArcId].SetBwt(varcBwt);


	// ﾄﾗﾝｼﾞｼｮﾝとの接続なら、パストラ解除判定
	if(ObjMap[vArcBase].type==_TRANSITION){
		if(ArcInMap.count(vArcBase) != 1){
			TranMap[vArcBase]->pass_tra = false;
		}
	}
	else if(ObjMap[vArcFollow].type==_TRANSITION){
		if(ArcOutMap.count(vArcFollow) != 1){
			TranMap[vArcFollow]->pass_tra = false;
		}
	}


	return ArcId++;
}

//GlobalRectの拡大
void BSPetri::EnlargeGlobalRect(int gx, int gy)
{
	if(globalRect.IsRectEmpty()){
		globalRect.SetRect(gx-20,gy-20,gx+20,gy+20);
		return;
	}

	if(gx < globalRect.left)		globalRect.left = gx;
	else if(gx > globalRect.right)	globalRect.right = gx;
	
	if(gy < globalRect.top)			globalRect.top = gy;
	else if(gy > globalRect.bottom)	globalRect.bottom = gy;
}

void BSPetri::NormalizeGlobalRect()
{

	globalRect.SetRectEmpty();
	map<int,bspObject>::iterator oit, oit_end;
	oit = ObjMap.begin();
	oit_end = ObjMap.end();
	for(;oit!=oit_end;++oit){
		EnlargeGlobalRect(oit->second.gpoint.x,oit->second.gpoint.y);
	}

	objRect = globalRect;

}

void BSPetri::DelObject(int PutIt)
{
	if(ObjMap.find(PutIt)==ObjMap.end()){
		TRACE("DelObject 存在しないIDです %d　：内部エラー\n",PutIt);
		return;
	}


	//トークンが配置されているか
	map<int,bspToken>::iterator tokit;
	tokit=TokenMap.end();
	if(ObjMap[PutIt].type==_PLACE){
		tokit=TokenMap.find(PutIt);
		if(tokit!=TokenMap.end()){//トークンを削除
			TokenMap.erase(tokit);
			return;
		}
	}

	//トークンを保持していなかったらプリミティブを削除
	multimap<int,int>::iterator aioit;
	//プリミティブから繋がっている出力アークを削除
	while((aioit=ArcInMap.lower_bound(PutIt))!=ArcInMap.upper_bound(PutIt)){
		DelArc(aioit->first, ArcMap[aioit->second].Out);
	}
	//プリミティブに繋がっている入力アークを削除
	while((aioit=ArcOutMap.lower_bound(PutIt))!=ArcOutMap.upper_bound(PutIt)){
		DelArc(ArcMap[aioit->second].In,aioit->first);
	}
	//プリミティブを削除
	dObjRemove(PutIt);

}

int BSPetri::DelArc(int vArcBase, int vArcFollow)
{

	ModifiedFlag = 1;
	multimap<int,int>::iterator iap,oap,iap_end;
	iap=ArcInMap.lower_bound(vArcBase);
	iap_end = ArcInMap.upper_bound(vArcBase);
	for(;iap!=iap_end;++iap){
		if(ArcMap[iap->second].Out==vArcFollow){
			ArcMap.erase(iap->second);
			oap=ArcOutMap.lower_bound(vArcFollow);
			for(;oap!=ArcOutMap.upper_bound(vArcFollow);++oap){
				if(oap->second==iap->second){
					ArcOutMap.erase(oap);
					break;
				}
			}
			ArcInMap.erase(iap);
			return 0;
		}
	}

	//無かった
	return 1;
}


void BSPetri::Add(BSPetri &bsp, set<int> &puttrack)
{

	bsp.SelectAllObj();

//	TRACE("all %d\n",bsp.intrack.size());

	Add(bsp, bsp.intrack, puttrack);
}

void BSPetri::Add(BSPetri &bsp, set<int> &track, set<int> &puttrack)
{

	ModifiedFlag = 1;
	map<int,int> paste_obj;
	
	puttrack.clear();

	int dobj_key;
	set<int>::iterator tkp,tkp_end;
	tkp = track.begin();
	tkp_end = track.end();
	for(;tkp!=tkp_end;++tkp){
//		TRACE("track %d\n",bsp.vObjMap[*tkp].type);

		// 各オブジェクトのデータを追加
		dobj_key = dObjAdd(bsp.ObjMap[*tkp].gpoint, bsp.ObjMap[*tkp].type);
if(dobj_key==0) continue;
		paste_obj[*tkp] = dobj_key;
		puttrack.insert(dobj_key);

		switch(bsp.ObjMap[*tkp].type){
		case _PLACE:
			break;
		case _TRANSITION:
			*TranMap[dobj_key] = *bsp.TranMap[*tkp];
			break;
		case _UHZI:
			*UhziMap[dobj_key] = *bsp.UhziMap[*tkp];
			break;
		case _KEMMAKIX:
			*KemMap[dobj_key] = *bsp.KemMap[*tkp];
			break;
		case _MODULE:
			ModMap[dobj_key] = bsp.ModMap[*tkp];
			break;
		default:
			TRACE("定義されてないｵﾌﾞｼﾞｪｸﾄです. :内部エラー");
		}

		//トークンを貼り付け
		if(bsp.TokenMap.find(*tkp) != bsp.TokenMap.end()){
			TokenMap[dobj_key] = bsp.TokenMap[*tkp];
		}
	}


	map<int,bspArc>::iterator ait,ait_end;
	int darc_key;
	int i;
	//アークを貼り付け（アークの全検索）
	ait = bsp.ArcMap.begin();
	ait_end = bsp.ArcMap.end();
	for(;ait!=ait_end;++ait){
		bspArc &arc = ait->second;
		if(track.find(arc.In)!=track.end()
			&& track.find(arc.Out)!=track.end()){
			
			darc_key = ArcChain(
				paste_obj[arc.In],
				paste_obj[arc.Out]
				,NULL
				,arc.value_name);
			if(arc.bwtpoint != NULL){
				ArcMap[darc_key].bwtpoint 
					= new vector<CPoint>( arc.bwtpoint->size() );
				for(i=0;i<arc.bwtpoint->size();++i){
					(*ArcMap[darc_key].bwtpoint)[i] = (*arc.bwtpoint)[i];
				}
			}
			
		}
	}


/*
#ifdef _DEBUG
	arc_check();
#endif
*/
}



void BSPetri::SetModuleName(string mname)
{
	ModifiedFlag = 1;
	module_name = mname;
}

void BSPetri::PutToken(int obj_id,int color,int value)
{
	if(obj_id==0) return;
	if(ObjMap.find(obj_id)==ObjMap.end()) return;
	if(ObjMap[obj_id].type!=_PLACE) return;
	ModifiedFlag = 1;
	TokenMap[obj_id].Color = color;
	TokenMap[obj_id].Value = value;
}


//アークに設定されている変数名が使われていなければクリアする
void BSPetri::ArcValReset(int tranPos)
{
	struct innerfunc{
		static void var_check(int tranPos,
			map<string,int> &io_var,
			multimap<int,int> &aio_map,
			map<int,bspArc> &ArcMap)
		{
			multimap<int,int>::iterator aioit;

			aioit=aio_map.lower_bound(tranPos);
			for(;aioit!=aio_map.upper_bound(tranPos);++aioit){
				if(io_var.find(
					ArcMap[aioit->second].value_name
					)==io_var.end()){
					ArcMap[aioit->second].value_name.erase();
				}
			}
			return;
		}
	};

	innerfunc::var_check(tranPos,TranMap[tranPos]->in_var,ArcOutMap,ArcMap);
	innerfunc::var_check(tranPos,TranMap[tranPos]->out_var,ArcInMap,ArcMap);
}



/*
//いるかなと思ったけど、使いづらいな
void *GetDataFromId(int obj_id)
{
	if(vObjMap.find( obj_id )==vObjMap.end()) return NULL;
	switch(vObjMap[obj_id].type){
		case
	}
	return NULL;
}
*/

#ifndef _LIGHT
//レジスタウージをトラプレに変換
//チェックなしで使用可能
void BSPetri::UhziRegExpand()
{
	map<int,bspUhzi*>::iterator uit;
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();){
		// レジスタウージでなければ展開しない
		if(uit->second->addp == 1){
			++uit;
			continue;
		}
		// 接続先のプレースが無ければ展開しない
		multimap<int,int>::iterator aioit;
		aioit = ArcInMap.lower_bound( uit->first );
		if(aioit == ArcInMap.upper_bound( uit->first )){
			++uit;
			continue;
		}


	//	TRACE("%s %d %s\n",module_name.c_str(),uit->first,uit->second->input_name.c_str());

		int p_id;
		p_id = ArcMap[ aioit->second ].Out;

		CPoint gp;
		gp = ObjMap[ uit->first ].gpoint;

		int ut_id, t_id;
		
		ut_id = t_id = dObjAdd(gp, _TRANSITION);
		TranMap[t_id]->fstring = "o1=i1;\no2=i1;";

		ArcChain(t_id, p_id, NULL, "o1");
		gp.y += 40;
		p_id = dObjAdd(gp, _PLACE);	// 1

		ArcChain(t_id, p_id, NULL, "o2");
		gp.x -= 40;
		t_id = dObjAdd(gp, _TRANSITION);
		ArcChain(p_id, t_id);
		gp.x -= 40;
		p_id = dObjAdd(gp, _PLACE);	// 2
		PutToken(p_id, _WTOKEN, uit->second->In);
		ArcChain(t_id, p_id);
		gp.y -= 40;
		t_id = dObjAdd(gp, _TRANSITION);
		ArcChain(p_id, t_id);
		gp.x += 40;
		p_id = dObjAdd(gp, _PLACE);	// 3
		PutToken(p_id, _BTOKEN, uit->second->In);
		ArcChain(t_id, p_id);

		ArcChain(p_id, ut_id);

		ut_id = uit->first;
		++uit;
		DelObject( ut_id );
	}
}

// 接続されていないウージを削除
// チェックなしで使用可能
void BSPetri::UhziNojointDelete()
{
	map<int,bspUhzi*>::iterator uit;
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();){
		if(ArcInMap.count( uit->first )==0){
			int u_id = uit->first;
			++uit;
			DelObject( u_id );
		}
		else ++uit;
	}
}


// 接続されていないウージを削除
// チェックなしで使用可能
void BSPetri::UhziNoIndustryCheck()
{
return;
}

//破棄ケムマキをトラプレに変換
// チェックなしで使用可能
void BSPetri::KemBinExpand()
{

	map<int,bspKemmakix*>::iterator kit;
	kit = KemMap.begin();
	for(;kit!=KemMap.end();){
		if(kit->second->dline != 2){
			++kit;
			continue;
		}

		multimap<int,int>::iterator aioit;
		aioit = ArcOutMap.lower_bound( kit->first );
		if(aioit == ArcOutMap.upper_bound( kit->first )){
			++kit;
			continue;
		}
		int p_id;
		p_id = ArcMap[ aioit->second ].In;

		CPoint gp;
		gp = ObjMap[ kit->first ].gpoint;

		int kt_id, t_id;
		
		kt_id = t_id = dObjAdd(gp, _TRANSITION);

		ArcChain(p_id, t_id, NULL, "i1");// 入力変数につなぐこと
		gp.x += 40;
		p_id = dObjAdd(gp, _PLACE);	// 1

		ArcChain(t_id, p_id);
		gp.x += 40;
		t_id = dObjAdd(gp, _TRANSITION);
		ArcChain(p_id, t_id);
		gp.y += 40;
		p_id = dObjAdd(gp, _PLACE);	// 2
		PutToken(p_id, _WTOKEN, 0);
		ArcChain(t_id, p_id);
		gp.x -= 40;
		t_id = dObjAdd(gp, _TRANSITION);
		ArcChain(p_id, t_id);
		gp.x -= 40;
		p_id = dObjAdd(gp, _PLACE);	// 3
		PutToken(p_id, _BTOKEN, 0);
		ArcChain(t_id, p_id);

		ArcChain(p_id, kt_id);

		kt_id = kit->first;
		++kit;
		DelObject( kt_id );
	}

}


//消費しないケムマキをﾄﾗﾝｼﾞｼｮﾝに変換
// チェックなしで使用可能
void BSPetri::KemBinToTran()
{
	map<int,bspKemmakix*>::iterator kit;
	kit = KemMap.begin();
	for(;kit!=KemMap.end();){
		if(kit->second->dline == 3){
			int k_id = kit->first;
			++kit;
			CPoint gp;
			gp = ObjMap[k_id].gpoint;
			int p_id;
			p_id = ArcMap[ArcOutMap.lower_bound(k_id)->second].In;
			DelObject( k_id );

			int t_id;
			t_id = dObjAdd(gp, _TRANSITION);
			ArcChain(p_id, t_id);

//			TranMap[t_id]->fstring.erase();
			TranMap[t_id]->in_place_num = 1;
			TranMap[t_id]->out_place_num = 0;
			TranMap[t_id]->loop_num = 0;
		}
		else ++kit;
	}
}

// 生産しないウージをﾄﾗﾝｼﾞｼｮﾝに変換
void BSPetri::UhziRegToTran()
{

}

void BSPetri::PlaceNojointChelt()
{
	map<int,bspPlace*>::iterator pit,pit_end;
	int p_id, flag;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;){
		p_id = pit->first;
		++pit;
		flag = (ArcInMap.lower_bound(p_id)==ArcInMap.upper_bound(p_id));
		if(ArcOutMap.lower_bound(p_id)==ArcOutMap.upper_bound(p_id)){	// 入力アーク無し
			if(flag){	// 出力アーク無し
				DelObject( p_id );
			}
			else{	// 出力アークありなので、生産しないウージをつなぐ
				CPoint gp;
				gp = ObjMap[p_id].gpoint;
				gp.x -= 40;

				// 始端のﾌﾟﾚｰｽに白トークンがあったら、余分なプレトラをつなぐ
				if(TokenMap.find(p_id)!=TokenMap.end()
					&& TokenMap[p_id].Color==_WTOKEN){
					int t_id;
					t_id = dObjAdd(gp, _TRANSITION);
					ArcChain(t_id, p_id);
					gp.x-=40;
					p_id = dObjAdd(gp, _PLACE);
					ArcChain(p_id, t_id);
					gp.x-=40;
				}

				int u_id;
				u_id = dObjAdd(gp, _UHZI);
				UhziMap[u_id]->dline = 3;	// 値を流す設定だが、値が無いので生産しない
				UhziMap[u_id]->addp = 1;
				ArcChain(u_id, p_id);
			}
		}
		else if(flag){	// 入力あり出力無しなので、消費しないケムマキをつなぐ
			CPoint gp;
			gp = ObjMap[p_id].gpoint;
			gp.x += 40;
			int k_id;
			k_id = dObjAdd(gp, _KEMMAKIX);
			KemMap[k_id]->dline = 3;	// 消費しない
			ArcChain(p_id, k_id);
		}
	}
}

//消費しないケムマキを削除
// チェックなしで使用可能
void BSPetri::KemNoConsumDelete()
{
	map<int,bspKemmakix*>::iterator kit;
	kit = KemMap.begin();
	for(;kit!=KemMap.end();){
		if(kit->second->dline == 3){
			int k_id = kit->first;
			++kit;
			DelObject( k_id );
		}
		else ++kit;
	}
}

// 接続されていないケムマキを削除
// チェックなしで使用可能
void BSPetri::KemNojointDelete()
{
	map<int,bspKemmakix*>::iterator kit;
	kit = KemMap.begin();
	for(;kit!=KemMap.end();){
		if(ArcOutMap.count( kit->first )==0){
			int kt_id = kit->first;
			++kit;
			DelObject( kt_id );
		}
		else ++kit;
	}
}



void BSPetri::C_Analysis()
{


	int i;
	multimap<int,int>::iterator aioit;
	
	//パストラのみのループ
	int pid1,tid1;
	int pid2,tid2;
	int pid3,tid3;
	int in,out;
	int init_v;
	
	set<int> delete_place; // 削除するプレース
	//フィードバックループの検索

	map<int,bspTransition*>::iterator tit,tit_end;

	//フィードバックループの検索
	tit=TranMap.begin();
	for(;tit!=TranMap.end();++tit){


		bspTransition *tran = tit->second;
		int t_id = tit->first;

		// 初期化
		tran->loop_num=0;
		tran->init_value.clear();
		//TRACE("%d\n",tran->incount);
		tran->init_value.resize(tran->in_place_num);
		tran->loopbackout.clear();
		tran->loopbackout.resize(tran->out_place_num);
		for(i=0;i<tran->out_place_num;i++) tran->loopbackout[i] = 0;
		tran->loopbackin.clear();
		tran->loopbackin.resize(tran->in_place_num);
		for(i=0;i<tran->in_place_num;i++) tran->loopbackin[i] = 0;
		
		if(tran->pass_tra) continue;

		delete_place.clear();
		if(tran->in_place_num && tran->out_place_num){
			aioit=ArcInMap.lower_bound(t_id);
			for(;aioit!=ArcInMap.upper_bound(t_id);++aioit){
				//１つ目のプレース
				pid1 = ArcMap[aioit->second].Out;
				if(ArcInMap.count(pid1)!=1) continue;
				//１つ目のトラかケム
				tid1=ArcMap[ArcInMap.lower_bound(pid1)->second].Out;
				if(TranMap.find(tid1)==TranMap.end()) continue;
				if(!TranMap[tid1]->pass_tra) continue;
				//２つ目のプレース
				pid2=ArcMap[ArcInMap.lower_bound(tid1)->second].Out;
				if(ArcInMap.count(pid2)!=1) continue;
				//２つ目のトラかケム
				tid2 = ArcMap[ArcInMap.lower_bound(pid2)->second].Out;
				if(TranMap.find(tid2)==TranMap.end()) continue;
				if(!TranMap[tid2]->pass_tra) continue;
				//３つ目のプレース
				pid3 = ArcMap[ArcInMap.lower_bound(tid2)->second].Out;
				if(ArcInMap.count(pid3)!=1) continue;
				//３つ目のトラかケム
				tid3 = ArcMap[ArcInMap.lower_bound(pid3)->second].Out;
				if(TranMap.find(tid3)==TranMap.end()) continue;
				if(tid3!=t_id) continue;	// 一周した
				
				//黒トークンが配置されて否かったらだめ
				if(TokenMap.find(pid3)!=TokenMap.end()
						&& TokenMap[pid3].Color==_BTOKEN){
					init_v = TokenMap[pid3].Value;
				}
				else if(TokenMap.find(pid2)!=TokenMap.end()
						&& TokenMap[pid2].Color==_BTOKEN){
					init_v = TokenMap[pid2].Value;
				}
				else continue;
				
				//フィードバックループであった
				//trace("	%d: %d->%d->%d->%d->%d\n",tid3,pid1,tid1,pid2,tid2,pid3);
				
				//TRACE("f %d <- %d\n",pid3,tid3);
				
				//	TRACE("%d\n",tran->out_arc_to_joint.size());
				
				tran->loop_num++;
				
				tran->init_value[
					tran->in_arc_to_joint[
					ArcInMap.lower_bound(pid3)->second
					]
					] = init_v;
				
				in = tran->in_arc_to_joint[
					ArcInMap.lower_bound(pid3)->second
					];
				out = tran->out_arc_to_joint[
					ArcOutMap.lower_bound(pid1)->second
					];
				
				tran->loopbackout[out] = in + 1;
				tran->loopbackin[in] = out + 1;
				//		TRACE("in %d %d out %d %d\n",in,tran->incount,out,tran->outcount);
				
				tran->in_place_num--;
				tran->out_place_num--;
				
				delete_place.insert( pid1 );
				if(TokenMap.find(pid2)!=TokenMap.end()) DelObject( pid2 );
				DelObject( pid2 );
				delete_place.insert( pid3 );
				
				DelObject( tid1 );
				DelObject( tid2 );
				
			}
		}
	
	//	TRACE("in %d out %d\n",tran->in_place_num,tran->out_place_num);
	


		set<int>::iterator sit;
		sit = delete_place.begin();
		for(;sit!=delete_place.end();){
			//		TRACE("%d\n",*sit);
			if(TokenMap.find(*sit)!=TokenMap.end()) DelObject( *sit );
			DelObject(*(sit++));
		}
	//	TRACE("in %d out %d\n",tran->in_place_num,tran->out_place_num);
	}

}

#endif // non _LIGHT

int BSPetri::GetOutputCount()
{
	int n=0;
	map<int,bspKemmakix*>::iterator kit;
	kit = KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		if(kit->second->dline<=1){
			n++;
		}
	}
	return n;
}

void BSPetri::SelectAllObj()
{

	intrack.clear();
	map<int,bspObject>::iterator oit,oit_end;
	oit = ObjMap.begin();
	oit_end = ObjMap.end();
	for(;oit!=oit_end;++oit){
	//	TRACE("in %d\n",oit->second.type);
		intrack.insert(oit->first);
	}

}


void BSPetri::TrackRectSet()
{

	// trackRectはグローバル座標を示す

	if(intrack.size()==0) return;

	CPoint mpoint;
	mpoint = ObjMap[*intrack.begin()].gpoint;

	trackRect.SetRectEmpty();
	trackRect.left=trackRect.right=mpoint.x;
	trackRect.top=trackRect.bottom=mpoint.y;

	set<int>::iterator iit,iit_end;
	iit=intrack.begin();
	iit_end = intrack.end();
	for(;iit!=iit_end;++iit){
  		mpoint=ObjMap[*iit].gpoint;

  		//trackRectの拡大
		if(mpoint.x<trackRect.left) trackRect.left=mpoint.x;
		else if(mpoint.x>trackRect.right) trackRect.right=mpoint.x;
		if(mpoint.y<trackRect.top) trackRect.top=mpoint.y;
		else if(mpoint.y>trackRect.bottom) trackRect.bottom=mpoint.y;
	}
	

	trackRect.InflateRect(20, 20);//余分に

//	TRACE("%d %d %d %d\n",trackRect.left,trackRect.top,trackRect.right,trackRect.bottom);

}

void BSPetri::PlaceFusion(set<int> &track, int mpid)
{

	if(ObjMap.find(mpid)==ObjMap.end()) return;
	if(ObjMap[mpid].type!=_PLACE) return;
	if(track.find(mpid)==track.end()) return;

	set<int>::iterator sit;
	// 融合先のﾌﾟﾚｰｽを探す
	sit = track.begin();
	//融合
	multimap<int,int>::iterator aioit;
	for(;sit!=track.end();++sit){
		if(ObjMap[*sit].type==_PLACE){
			if(*sit==mpid) continue;
			
			//入力アークをつなぎなおす
			aioit = ArcOutMap.lower_bound(*sit);
			for(;aioit!=ArcOutMap.upper_bound(*sit);++aioit){
				ArcChain(
					ArcMap[aioit->second].In,
					mpid,
					ArcMap[aioit->second].bwtpoint,
					ArcMap[aioit->second].value_name
					);
			}

			//出力アークをつなぎなおす
			aioit = ArcInMap.lower_bound(*sit);
			for(;aioit!=ArcInMap.upper_bound(*sit);++aioit){
				ArcChain(
					mpid,
					ArcMap[aioit->second].Out,
					ArcMap[aioit->second].bwtpoint,
					ArcMap[aioit->second].value_name
					);
			}

			// 中間座標に配置
//			vObjMap[mpid].gpoint.x = (vObjMap[mpid].gpoint.x + vObjMap[*sit].gpoint.x) / 2;
//			vObjMap[mpid].gpoint.y = (vObjMap[mpid].gpoint.y + vObjMap[*sit].gpoint.y) / 2;

			// トークンが置かれているなら削除する
			if(TokenMap.find(*sit)!=TokenMap.end()){
				DelObject( *sit );
			}
			DelObject( *sit );
		}
	}

	track.clear();

}


void BSPetri::PlaceFission(set<int> &track)
{

	if(track.size()==0) return;

	set<int> retset;
	CPoint gp;
	multimap<int,int>::iterator aioit;
	int p_id;
	set<int>::iterator sit;

	sit = track.begin();
	for(;sit!=track.end();++sit){
		if(ObjMap[*sit].type==_PLACE){
			if(ArcOutMap.count(*sit) + ArcInMap.count(*sit) < 2){
				retset.insert(*sit);
				continue;
			}

			gp = ObjMap[*sit].gpoint;
			
			//入力アークをつなぎなおす
			aioit = ArcOutMap.lower_bound(*sit);
			for(;aioit!=ArcOutMap.upper_bound(*sit);++aioit){
				p_id = dObjAdd(
					Betwixtpoint(
						ObjMap[ArcMap[aioit->second].In].gpoint,
						gp, 1, 20)
					, _PLACE);
	//			TRACE("%d\n",p_id);
				retset.insert(p_id);
				ArcChain(
					ArcMap[aioit->second].In,
					p_id,
					ArcMap[aioit->second].bwtpoint,
					ArcMap[aioit->second].value_name
					);
			}

			//出力アークをつなぎなおす
			aioit = ArcInMap.lower_bound(*sit);
			for(;aioit!=ArcInMap.upper_bound(*sit);++aioit){
				p_id = dObjAdd(Betwixtpoint(
					gp,
					ObjMap[ArcMap[aioit->second].Out].gpoint,
					0, 20)
					, _PLACE);
//				TRACE("%d\n",p_id);
				retset.insert(p_id);
				ArcChain(
					p_id,
					ArcMap[aioit->second].Out,
					ArcMap[aioit->second].bwtpoint,
					ArcMap[aioit->second].value_name
					);
			}

			// トークンが置かれているなら削除する
			if(TokenMap.find(*sit)!=TokenMap.end()){
				DelObject( *sit );
			}
			DelObject( *sit );
		}
	}

	track = retset;

}


//移動はグローバルで行うこと
void BSPetri::ObjMove(set<int> &track,int move_x, int move_y, int arc_in_move, int arc_out_move)
{

	ModifiedFlag = 1;

	CPoint mp;
	mp.x = move_x;
	mp.y = move_y;
	
	////オブジェクトの移動
	set<int>::iterator trp,trp_end;
	trp = track.begin();
	trp_end = track.end();
	for(;trp!=trp_end;++trp){
		ObjMap[*trp].gpoint += mp;
	}

	//両方とも移動オブジェクトにつながってるアークの移動
	multimap<int,int>::iterator aps,ape;
	trp = track.begin();
	trp_end = track.end();
	for(;trp!=trp_end;++trp){
	//オブジェクトの出力に関して
		aps=ArcInMap.lower_bound(*trp);
		ape=ArcInMap.upper_bound(*trp);
		for(;aps!=ape;++aps){
			if(
				(arc_out_move==TRUE
				|| track.find(ArcMap[aps->second].Out)!=track.end())
				&& ArcMap[aps->second].bwtpoint!=NULL){

				ArcBwtMove(ArcMap[aps->second].bwtpoint, mp);
			}
		}
		//オブジェクトの入力に関して
		aps=ArcOutMap.lower_bound(*trp);
		ape=ArcOutMap.upper_bound(*trp);
		for(;aps!=ape;++aps){
			if(
				arc_in_move==TRUE
				&& track.find(ArcMap[aps->second].In)==track.end()
				&& ArcMap[aps->second].bwtpoint!=NULL){

				ArcBwtMove(ArcMap[aps->second].bwtpoint, mp);
			}
		}
	}

}

void BSPetri::SetMemo(const char *memo_str)
{
	Memo = memo_str;
	ModifiedFlag = 1;
}

void BSPetri::PetClear()
{

	Globalpoint.x = 0;
	Globalpoint.y = 0;

	ErrPos=0;
	ArcInMap.clear();

	ArcOutMap.clear();
	ArcMap.clear();
	ObjMap.clear();
	TokenMap.clear();
	
	map<int,bspTransition*>::iterator tit,tit_end;
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		delete tit->second;
	}
	TranMap.clear();

	map<int,bspPlace*>::iterator pit,pit_end;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		delete pit->second;
	}
	PlaceMap.clear();

	map<int,bspUhzi*>::iterator uit,uit_end;
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		delete uit->second;
	}
	UhziMap.clear();
	
	map<int,bspKemmakix*>::iterator kit,kit_end;
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		delete kit->second;
	}
	KemMap.clear();

	simTranFireableStop.clear();

	ObjId=1;
	ArcId=1;
	
	globalRect.SetRectEmpty();
	objRect.SetRectEmpty();

	confSimMaxTurn = 5000;

	module_name.erase();

	ModMap.clear();
	
	map<int,BSPetri*>::iterator mit,mit_end;
	mit=ModuleMap.begin();
	mit_end = ModuleMap.end();
	for(;mit!=mit_end;++mit){
		delete mit->second;
	}
	ModuleMap.clear();

	TextMap.clear();

	viewDiam = 1.0;

	Memo.erase();

	ModifiedFlag = 1;

}

BSPetri::~BSPetri()
{
	PetClear();
}

/// EOF ///
