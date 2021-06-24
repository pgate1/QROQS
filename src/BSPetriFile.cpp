#include "stdafx.h"

#include "BSPetri.h"
#include"QROQS.h"
	
//データで検索
template<typename T>
void non_erase(map<int,bspObject> &ObjMap,map<int, T> &DataMap)
{
	map<int,T>::iterator it;
	it = DataMap.begin();
	for(;it!=DataMap.end();){
		if(ObjMap.find( it->first ) == ObjMap.end()){
			DataMap.erase(++it);
		}
		else ++it;
	}
}


void BSPetri::Write(const char *fname)
{

	vector<CPoint> *varcBwt;
//TRACE("write tmp\n");
	int i, j,objid;
	FILE *fp;
	fp = fopen(fname,"w");
	if(fp==NULL){
		throw "ﾌｧｲﾙｵｰﾌﾟﾝｴﾗｰ．";
	}

	setbuf(fp, NULL);//バッファを使わん
	

	// 念のためゼロオブジェクトの削除
	if(ObjMap.find(0)!=ObjMap.end()) dObjRemove(0);
	// データのみが存在するなら削除

	non_erase(ObjMap, PlaceMap);
	non_erase(ObjMap, TranMap);
	non_erase(ObjMap, UhziMap);
	non_erase(ObjMap, KemMap);
	non_erase(ObjMap, ModMap);

	map<int,bspPlace*>::iterator pit,pit_end;
	map<int,bspTransition*>::iterator tit,tit_end;
	map<int,bspUhzi*>::iterator uit,uit_end;
	map<int,bspKemmakix*>::iterator kit,kit_end;
	map<int,string>::iterator mit,mit_end;


	fprintf(fp, "=== QROQS Objects Data Ver.2.00 ===\n\n");
	
	objid=1;
	map<int,int> placemap,tranmap,uhzimap,kemmap,modulemap,objmap;

	//一文字目で判断している
	fprintf(fp, ".section[root]{\n");
	fprintf(fp, "module[%s]\n",module_name.c_str());
	fprintf(fp, "turn_max[%d]\n",confSimMaxTurn);

	i=0;
	for(;;){
		i=Memo.find('}',i);
		if(i==Memo.npos) break;
		Memo.insert(i,"\\");
		i+=2;
	}
	fprintf(fp, "comment{\n");
	for(i=0;i<Memo.length();i++){
		if(Memo[i]=='\r') continue;
		fprintf(fp,"%c",Memo[i]);
	}
	if(Memo[i-1]!='\n') fprintf(fp,"\n");
	fprintf(fp,"}\n");
	
	fprintf(fp, "}\n");


	fprintf(fp, ".section[place]{\n/pset[ID]\n");
	pit=PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(; pit!=pit_end; ++objid,++pit){
		placemap[pit->first] = objid;
		fprintf(fp, "pset[%d]\n", objid);//objid 0でもよい
	}
	fprintf(fp, "}\n");
	
	fprintf(fp, ".section[transition]{\n/tran[ID,f1:f2..]\n");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(; tit!=tit_end; ++objid,++tit){
		tranmap[tit->first] = objid;
		fprintf(fp, "tran[%d,", objid);//objid 0でもよい
		j = 0;
		bspTransition *tran = tit->second;
		while(j<tran->fstring.size() 
			&& tran->fstring[j]!='\0'){
			if(tran->fstring[j]=='\r'){
			}
			else{
				fprintf(fp,"%c",tran->fstring[j]);
			}
			++j;
		}
		fprintf(fp,"]\n");
	}
	fprintf(fp,"}\n");
	
//TRACE("uhzi\n");
	fprintf(fp,".section[uhzi]{\n/uset[ID,PlaceID,inc,line]=(d1,d2,d3...)\n");
	list<string>::iterator ulit,ulit_end;
	uit=UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++objid,++uit){
		bspUhzi *uhzi = uit->second;
		uhzimap[uit->first]=objid;
		fprintf(fp,"uset[%d,%d,%d,%d]={\n"
			,objid// 0でもよい
			,uhzi->bits
			,uhzi->addp
			,uhzi->dline);
		if(uhzi->addp==1 && uhzi->InputString.size()){
			ulit=uhzi->InputString.begin();
			ulit_end = uhzi->InputString.end();
			for(;ulit!=ulit_end;++ulit){
				fprintf(fp,"data(%s)\n",ulit->c_str());
			}
		}
		else if(uhzi->addp==0){
			fprintf(fp,"data(%d)\n",uhzi->In);
		}
		
		if(!uhzi->input_name.empty()){
			fprintf(fp,"}[%s]\n",uhzi->input_name.c_str());
		}
		else{
			fprintf(fp,"}\n");
		}
	}
	fprintf(fp,"}\n");
	
//	TRACE("kem\n");
	fprintf(fp,".section[kemmakix]{\n/kset[ID,PlaceID,line]\n");
	kit=KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++objid,++kit){
		bspKemmakix *kem = kit->second;
		kemmap[kit->first] = objid;
		fprintf(fp,"kset[%d,%d,%d,%d]{\n"
			,objid// 0でもよい
			,kem->bits
			,kem->dline
			,kem->hex);
//		TRACE("%d line %d\n",kp->first,kp->second.dline);
		if(kem->compString){
			list<string>::iterator it,it_end;
			it = kem->compString->begin();
			it_end = kem->compString->end();
			for(;it!=it_end;++it){
				fprintf(fp,"data(%s)\n",it->c_str());
			}
		}
		if(!kem->output_name.empty()){
			fprintf(fp,"}[%s]\n",kem->output_name.c_str());
		}
		else{
			fprintf(fp,"}\n");
		}
	}
	fprintf(fp,"}\n");

	fprintf(fp, ".section[module]{\n/module[ID,name]\n");
	mit = ModMap.begin();
	mit_end = ModMap.end();
	for(; mit!=mit_end; ++objid,++mit){
		modulemap[mit->first] = objid;
		fprintf(fp, "module[%d,", objid);//objid 0でもよい
		fprintf(fp,"%s",mit->second.c_str());
		fprintf(fp,"]\n");
	}
	fprintf(fp,"}\n");
	
	fprintf(fp,".section[vobject]{\n");
	map<int,bspObject>::iterator oit,oit_end;
	oit = ObjMap.begin();
	oit_end = ObjMap.end();
	for(i=1;oit!=oit_end;++i,++oit){

		fprintf(fp,"obj[%d,%d,"
			,0// 0でもよい
			,oit->second.type);
		switch(oit->second.type){
		case _PLACE:
			j=placemap[oit->first];
			break;
		case _TRANSITION:
			j=tranmap[oit->first];
			break;
		case _UHZI:
			j=uhzimap[oit->first];
			break;
		case _KEMMAKIX:
			j=kemmap[oit->first];
			break;
		case _MODULE:
			j=modulemap[oit->first];
			break;
#ifdef _DEBUG
		default:
			throw "保存できないオブジェクト：内部エラー";
#endif
		}
		objmap[oit->first]=j;
		fprintf(fp,"%d]=(%d,%d)\n"
			,j
			,oit->second.gpoint.x
			,oit->second.gpoint.y);
	}
	fprintf(fp,"}\n");
	
	fprintf(fp,".section[token]{\n/tset[PlaceID]=(color,value)\n");
	map<int,bspToken>::iterator tokit,tokit_end;
	tokit=TokenMap.begin();
	tokit_end = TokenMap.end();
	for(;tokit!=tokit_end;++tokit){
		fprintf(fp,"tset[%d]=(%d,%d)\n",
			objmap[tokit->first]
			,tokit->second.Color==_BTOKEN?5:6
			,tokit->second.Value);
	}
	fprintf(fp,"}\n");
	
	fprintf(fp,".section[arc]{\n/arc[In,Out]\n");
	map<int,bspArc>::iterator ait,ait_end;
	ait=ArcMap.begin();
	ait_end = ArcMap.end();
	for(;ait!=ait_end;++ait){
		bspArc &arc = ait->second;
		fprintf(fp,"arc[%d,%d"
			,objmap[arc.In]
			,objmap[arc.Out]);
		if(!arc.value_name.empty()){
			fprintf(fp,",%s",arc.value_name.c_str());
		}
		fprintf(fp,"]");
		varcBwt=arc.bwtpoint;
		if(varcBwt!=NULL){
			for(i=0;i<varcBwt->size();++i){
				fprintf(fp,":(%d,%d)",(*varcBwt)[i].x,(*varcBwt)[i].y);
			}
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"}\n");


	fprintf(fp, ".section[simTranOpt]{\n");
	set<int>::iterator sit;
	sit = simTranFireableStop.begin();
	for(; sit!=simTranFireableStop.end();++sit){
		fprintf(fp,"%d\n",objmap[*sit]);
	}
	fprintf(fp,"}\n");


	fprintf(fp, ".section[text]{\n");
	map<int,string>::iterator textit,textit_end;
	textit = TextMap.begin();
	textit_end = TextMap.end();
	for(; textit!=textit_end;++textit){
		fprintf(fp,"text[%d]{\n",objmap[textit->first]);
		fprintf(fp,"%s\n}\n",textit->second.c_str());
	}
	fprintf(fp,"}\n");

	
	fprintf(fp,"=== END ===\n");
	
	fclose(fp);


}

void BSPetri::Open(const char *fname)
{	

//	TRACE("BSPetri::Open %s\n",fname);
//	fprintf(tfp,"BSPetri::Open %s\n",fname);

	CPoint point;
	int id,num,c,col,val,i,j;

	vector<CPoint> varcBwt;
	CString s,s2;
	FILE *fp;
	fp=fopen(fname, "r");
	if(fp==NULL){
#ifndef _LIGHT
		if(Qmode) throw " ファイルが！ファイルガァッ！！";
#endif
		throw "ﾌｧｲﾙを開けませんでした．";
	}
#define READ_MAX 1024
	char *buf = new char[READ_MAX+1];

	map<int,bspTransition*>::iterator tit;
	map<int,bspPlace>::iterator pit;
	map<int,bspUhzi>::iterator uit;
	map<int,bspKemmakix>::iterator kit;



	fgets(buf,READ_MAX,fp);
	if(strncmp(buf,"=== QROQS Objects Data Ver.2.00 ===",35)){
		fclose(fp);
		delete[] buf;
		//TRACE("no my file\n");
#ifndef _LIGHT
		if(Qmode) throw "オレのファイルじゃねえ！!";
#endif
		throw "BSPﾌｧｲﾙではありません．";
	}


	PetClear();
//	TRACE("grect (%d,%d):[%d,%d]\n",globalRect.left,globalRect.top,globalRect.Width(),globalRect.Height());

	try{

	while(fgets(buf, READ_MAX, fp)){
//fprintf(tfp,"%s",buf);
//TRACE("%s",buf);


		if(!strncmp(buf,".section[root]",14)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='m'){
					char *name = new char[200];
					name[0]='\0';
					sscanf(buf,"module[%[^]]]",name);
					module_name = name;
					delete[] name;
				}
				else if(buf[0]=='t'){
					sscanf(buf,"turn_max[%d]",&confSimMaxTurn);
				}
				else if(buf[0]=='c'){	// Memo
					for(;;){
						fgets(buf,READ_MAX,fp);
						if(buf[0]=='}' && buf[1]=='\n') break;
						c=0;
						while(buf[c]!='\0'){
							if(buf[c]=='\\' && buf[c+1]=='}'){
								c++;
							}
							Memo += buf[c];
							c++;
						}
					}
				}
			}
		}

		else if(!strncmp(buf,".section[place]",15)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='p'){
					sscanf(buf,"pset[%d]",&id);
					PlaceMap[ObjId++] = new bspPlace;
					//printf("	place add %d\n",id);
				}
			}
		}
		else if(!strncmp(buf,".section[transition]",20)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='t'){
					//				TRACE("%s",buf);
					sscanf(buf,"tran[%d,",&id);
					id=ObjId;
					++ObjId;
					//printf("	transition add %d %d\n",id,num);
					TranMap[id] = new bspTransition;
					TranMap[id]->fstring.erase();
					i=0;
					while(buf[i]-',') ++i;
					++i;
					c=i;
					while(buf[c]!=']'){
						while(buf[c]!='\0' && buf[c]!=']') ++c;
						if(buf[c]=='\0'){
							s.Format("%s",buf+i);
							TranMap[id]->fstring += s;
							//TRACE("set:%s\n",s);
							fgets(buf, READ_MAX, fp);
							//TRACE("in:%s\n",buf);
							i=0;
							c=0;
						}
					}
					buf[c]='\0';
					s.Format("%s",buf+i);
					TranMap[id]->fstring+=s;
					try{
						TranMap[id]->FuncEduction();
					}
					catch(char *err){
						err;
					}
				}
			}
		}

		else if(!strncmp(buf,".section[uhzi]",14)){
		//	TRACE("uhzi\n");
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='u'){
					sscanf(buf,"uset[%d,%d,%d,%d]",&id,&num,&col,&val);
				//	TRACE("uadd id %d\n",id);
					id=ObjId;
					UhziMap[ObjId++] = new bspUhzi(col,val);
					UhziMap[id]->bits = num==0 ? 1 : num;
						
					//printf("	uhzi add %d %d %d\n",id,num,c);
					fgets(buf,READ_MAX,fp);
					if(col==1){
						while(buf[0]=='d'){
							i=strlen(buf);
							j=5;
							while(buf[i-2]!=')'){
								UhziMap[id]->InputString.push_back(buf+j);
								fgets(buf,READ_MAX,fp);
								i=strlen(buf);
								j=0;
							}
							buf[i-2]='\0';
							UhziMap[id]->InputString.push_back(buf+j);
							fgets(buf,READ_MAX,fp);
						}
					}
					else{
						sscanf(buf,"data(%d)",&UhziMap[id]->In);
						fgets(buf,READ_MAX,fp);
					}

					if(buf[1]=='['){
						char *ch = strchr(buf,']');
						if(ch!=NULL){
							*ch='\0';
							UhziMap[id]->input_name = buf+2;
						}

					//	TRACE("name %s\n",UhziMap[id].input_name.c_str());
					}
				}
			}
		}
		else if(!strncmp(buf,".section[kemmakix]",18)){
			char *is = new char[READ_MAX+1];
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='k'){
					col=0;	// hex
					sscanf(buf,"kset[%d,%d,%d,%d]{",&id,&num,&val,&col);
					id=ObjId;
					KemMap[ObjId++] = new bspKemmakix(val);
					if(num) KemMap[id]->bits = num;
					KemMap[id]->hex = col;
					
					fgets(buf,READ_MAX,fp);
					KemMap[id]->compString = new list<string>;
					while(buf[0]=='d'){
						sscanf(buf,"data(%[^)])",is);
						KemMap[id]->compString->push_back(is);
						fgets(buf,READ_MAX,fp);
					}

					if(buf[1]=='['){
						char *ch = strchr(buf,']');
						if(ch!=NULL){
							*ch='\0';
							KemMap[id]->output_name = buf+2;
						}
					}
					//printf("	kemmakix add %d %d\n",id,num);
				}
			}
			delete[] is;
		}

		else if(!strncmp(buf,".section[module]",16)){
			char *name = new char[200];
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='m'){
					//				TRACE("%s",buf);
					name[0]='\0';
					sscanf(buf,"module[%d,%[^]]]",&id, name);
					id=ObjId;
					++ObjId;
					ModMap[id] = name;
				}
			}
			delete[] name;
		}

		else if(!strncmp(buf,".section[vobject]",17)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='o'){
					sscanf(buf,"obj[%d,%d,%d]=(%d,%d)",&col,&num,&id,&point.x,&point.y);
					
					//if(vp==NULL) throw "オブジェクトが読み込めませんでした";
					ObjMap[id] = bspObject(num, point);
				}
			}
		}
		else if(!strncmp(buf,".section[token]",15)){
	//		TRACE("read token\n");
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='t'){
					sscanf(buf,"tset[%d]=(%d,%d)",&id,&col,&val);
					TokenMap[id] = bspToken(col==5?_BTOKEN:_WTOKEN,val);
				}
			}
		}
		else if(!strncmp(buf,".section[arc]",13)){
		//	TRACE("read arc\n");
			char *v_name=new char[200];
			int col,val;
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='a'){
					//TRACE("%s",buf);
					*v_name='\0';
					sscanf(buf,"arc[%d,%d,%[^]]]",&col,&val,v_name);
					//TRACE("a %d %d %s\n",col,val,v_name);

					ArcInMap.insert(make_pair<int,int>(col,ArcId));
					ArcOutMap.insert(make_pair<int,int>(val,ArcId));
					ArcMap[ArcId] = bspArc(col, val, v_name);

					//printf("	token %d %d %d\n",id,col,val);
					c=0;
					while(buf[c]!=':' && buf[c]!='\0') ++c;
					if(buf[c]==':'){
						varcBwt.clear();
						while(buf[c]==':'){
							sscanf(buf+c,":(%d,%d)",&point.x,&point.y);
							//	TRACE("p(%d,%d) ",point.x,point.y);
							varcBwt.push_back(point);
							++c;
							while(buf[c]!=':' && buf[c]!='\0') ++c;
						}
						ArcMap[ArcId].SetBwt(&varcBwt);
					}
					++ArcId;
				}
			}
			delete[] v_name;
		}
		else if(!strncmp(buf,".section[simTranOpt]",20)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				sscanf(buf,"%d\n",&id);
				simTranFireableStop.insert(id);
			}
		}
		else if(!strncmp(buf,".section[text]",14)){
			while(fgets(buf, READ_MAX, fp)){
				if(buf[0]=='}') break;
				if(buf[0]=='t'){
					sscanf(buf,"text[%d]{\n",&id);
					fgets(buf, READ_MAX, fp);
					buf[strlen(buf)-1] = '\0';
					TextMap[id] = buf;
					fgets(buf, READ_MAX, fp);
				}
			}
		}

		else if(!strncmp(buf,"=== END ===",11)) break;
	}
	
	if(strncmp(buf,"=== END ===",11)){
#ifndef _LIGHT
		if(Qmode) throw " : 読み込みに失敗した…";
#endif
		throw "BSPﾌｧｲﾙの読み込みに失敗しました．";
	}
	
	}
	catch(char *err){
//fprintf(tfp,"fgets %s\n",err);
		fclose(fp);
		delete[] buf;
		throw err;
	}

	fclose(fp);
	delete[] buf;

//fprintf(tfp,"open end\n");

	NormalizeGlobalRect();

	SetPasstraArc();

	ModifiedFlag = 0;
//	TRACE("grect (%d,%d)(%d,%d)\n",globalRect.left,globalRect.top,globalRect.right,globalRect.bottom);
}

void BSPetri::SetPasstraArc()
{

	map<int,bspTransition*>::iterator tit,tit_end;
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		if(tit->second->pass_tra==false) continue;
		if(ArcOutMap.count(tit->first)!=1 || ArcInMap.count(tit->first)!=1){
			tit->second->pass_tra = false;
		}
	}


}

/// EOF ///

