
#include "stdafx.h"

#include "BSPetri.h"
#include "other.h"

#include "v2c.h"



void trafunc_num_2_num(const char *in_str, map<int,int> &new_num_map, string &out_str)
{
	out_str.erase();
	char buf[10];
	int j,d;
	for(j=0;in_str[j]!='\0';++j){
		while(in_str[j]!='i'){
			out_str = out_str + in_str[j];
			j++;
			if(in_str[j]=='\0') return;
		}
		j++;
		d=in_str[j]-'0';
		if(isdigit(in_str[j+1])){
			++j;
			d*=10;
			d+=in_str[j]-'0';
		}

		sprintf(buf,"i%d",new_num_map[d]);	//ここが変換のポイント
		out_str = out_str + buf;
	}
	
}

//ペトリネットのコンパイル＆チェック
void BSPetri::C_Compile()
{


	try{
		AnyTokenSet();
		AnyTranSet();
		AnyArcUKSet();
	}
	catch(char *err){
		err;
	}

//	FILE *fp;
//	fp=fopen("test.txt","w");


	func_in_index_transrate.clear();
	func_out_index_transrate.clear();

	// ﾌｨｰﾄﾞﾊﾞｯｸループを削除した状態での接続

	multimap<int,int>::iterator aioit;
	map<int,bspTransition*>::iterator tit,tit_end;

	map<int,int> in_map,out_map;
	//トランジションに接続されているアーク
	int i,j;
	string a_val;
	string str;
	map<int,int> used;


	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		int tid = tit->first;
//		fprintf(fp,"t %d\n",tid);

		bspTransition *tran = tit->second;

		if(tran->pass_tra){
			//in
			aioit = ArcOutMap.lower_bound(tid);
			tran->InPlaceList[0] = ArcMap[aioit->second].In;
//			tran->in_arc_to_joint[aioit->second] = 0;
			func_in_index_transrate[tid][1] = 1;
			//out
			aioit = ArcInMap.lower_bound(tid);
			tran->OutPlaceList[0] = ArcMap[aioit->second].Out;
//			tran->out_arc_to_joint[aioit->second] = 0;
			func_out_index_transrate[tid][1] = 1;
			continue;
		}

/*
		tran->in_var.clear();
		tran->in_var["aaa"] = 1;
		tran->in_var["bbb"] = 2;
		tran->in_var["ccc"] = 3;
		tran->in_var["ddd"] = 4;
		tran->in_place_num = 3;
		tran->incount = 4;
		tran->loop_num = 3;
		tran->loopbackin.clear();
		tran->loopbackin.resize(6);
		tran->loopbackin[0] = 0;
		tran->loopbackin[1] = 1;
		tran->loopbackin[2] = 1;
		tran->loopbackin[3] = 0;
		tran->loopbackin[4] = 1;
		tran->loopbackin[5] = 0;
*/

		struct func{
			static void io_sort(
					int io_size, int var_size,
					vector<int> &loopback, map<int,int> &io_map
				)
			{
				int i,k;
				// 未使用入力のループバックは排除
				for(i=io_size;i>var_size;i--) loopback[i-1] = 0;
				io_map.clear();
				map<int,int> used;
				used.clear();
				k = i = io_size;
				for(;i>0;i--){
					if(loopback[i-1]){
						io_map[i] = k;
						used[k] = 1;
						k--;
					}
				}
				k = i = io_size;
				for(;i>0;i--){
					if(loopback[i-1]==0){
						while(used[k]) k--;
						io_map[i] = k;
						used[k] = 1;
						k--;
					}
				}
				return;
			}
		};
		func::io_sort(
			tran->in_place_num+tran->loop_num, tran->incount,
			tran->loopbackin, in_map );
		func::io_sort(
			tran->out_place_num+tran->loop_num, tran->outcount,
			tran->loopbackout, out_map );

		// 入力を出力に対応させる
		if(tran->loop_num){
			used.clear();
			for(i=1;i<=tran->out_place_num + tran->loop_num;i++){
				if(tran->loopbackout[i-1]){
					used[ in_map[tran->loopbackout[i-1]] ] =
						out_map[i] - tran->out_place_num + tran->in_place_num;
				}
			}
			for(i=1;i<=tran->in_place_num + tran->loop_num;i++){
				if(tran->loopbackin[i-1]){
					in_map[i] = used[ in_map[i] ];
				}
			}
		}
/*
		struct func{
			static void io_print(
				FILE *fp, map<int,int> &io_map, int io_size,
				map<string,int> &io_var, vector<int> &loopback
				)
			{
				int i;
		//		TRACE("s %d\n",new_num_map.size());
				if(io_map.size()!=io_size) fprintf(fp,"error\n");
				map<string,int>::iterator vit;
				map<int,int>::iterator mit;
				for(i=1;i<=io_size;i++){
					fprintf(fp,"	%d ",i);
					vit = find_by_value(io_var.begin(),io_var.end(),i);
					if(vit==io_var.end()) fprintf(fp,"%15s ","");
					else fprintf(fp,"%15s ",vit->first.c_str());
					fprintf(fp,"%c  ",loopback[i-1]?'l':' ');
					mit = find_by_value(io_map.begin(),io_map.end(),i);
					vit = find_by_value(io_var.begin(),io_var.end(),mit->first);
					if(vit==io_var.end()) fprintf(fp,"%15s ","");
					else fprintf(fp,"%15s ",vit->first.c_str());
					fprintf(fp,"%c ;\n",loopback[mit->first-1]?'l':' ');
				}
				return;
			}
		};
		func::io_print(
			fp, in_map, tran->in_place_num+tran->loop_num,
			tran->in_var, tran->loopbackin );
		func::io_print(
			fp, out_map, tran->out_place_num+tran->loop_num,
			tran->out_var, tran->loopbackout );
*/


		// 接続

		i = tran->in_place_num + tran->loop_num;
		aioit = ArcOutMap.lower_bound(tid);
		for(;aioit!=ArcOutMap.upper_bound(tid);++aioit){
			a_val = ArcMap[aioit->second].value_name;
			if(a_val.empty()){
				j = in_map[i] - 1;
				i--;
			}
			else{
				j = in_map[tran->in_var[a_val]] - 1;
			}
			tran->InPlaceList[j] = ArcMap[aioit->second].In;
//			tran->in_arc_to_joint[aioit->second] = j;
		}
		
		i = tran->out_place_num + tran->loop_num;
		aioit = ArcInMap.lower_bound(tid);
		for(;aioit!=ArcInMap.upper_bound(tid);++aioit){
			a_val = ArcMap[aioit->second].value_name;
			if(a_val.empty()){
				j = out_map[i] - 1;
				i--;
			}
			else{
				j = out_map[tran->out_var[a_val]] - 1;
			}
			tran->OutPlaceList[j] = ArcMap[aioit->second].Out;
//			tran->out_arc_to_joint[aioit->second] = j;
		}


//		TRACE("in %d -> %d\n",tran->in_place_num+tran->loop_num,new_num_map.size());

		// 入力変数の置き換え
		if(tran->loop_num){
			if(tran->fspost){
				trafunc_num_2_num(tran->fspost, in_map, str);
				//TRACE("rep %s\n",str.c_str());
				delete[] tran->fspost;
				tran->fspost = new char[str.length()+1];
				strcpy(tran->fspost,str.c_str());
			}
			if(tran->fapost){
				trafunc_num_2_num(tran->fapost, in_map, str);
			//	TRACE("%s\n",str.c_str());
				delete[] tran->fapost;
				tran->fapost = new char[str.length()+1];
				strcpy(tran->fapost,str.c_str());
			}
		}

		func_in_index_transrate[tid] = in_map;

//		TRACE("out %d -> %d\n",tran->out_place_num+tran->loop_num,new_num_map.size());

		// 論理式の配置換え
		if(tran->loop_num && tran->fspost){
			map<int,string> funcs;
			funcs.clear();
			string fstr;
			fstr = tran->fspost;
			int i,j,st;
			j=0;
			st=0;
			for(i=1;;i++){
				//後置記法のトランジション関数
				j=fstr.find('\n',j);
				if(j==fstr.npos) break;
				j++;
				funcs[out_map[i]] = fstr.substr(st,j-st-1);
				st=j;
			}
			// 不定値の出力の場合
			for(;i<=tran->out_place_num+tran->loop_num;i++){
				funcs[out_map[i]] = "i1";
			}
	//		TRACE("s %d\n",funcs.size());

			fstr.erase();
			map<int,string>::iterator fit;
			fit = funcs.begin();
			for(;fit!=funcs.end();++fit){
	//			TRACE("%s\n",fit->second.c_str());
				fstr += fit->second + "\n";
			}

			delete[] tran->fspost;
			tran->fspost = new char[fstr.length()+1];
			strcpy(tran->fspost,fstr.c_str());
	//		TRACE("{%s}",tran->fspost);
		}
	
		func_out_index_transrate[tid] = out_map;
	//	TRACE("out %d",i);
	}


//	fclose(fp);


	
}



//#define CMOD_DIR "bspc/feedback"
#define CMOD_DIR "bspc/event"

//#define _C_DEBUG


//通常のトランジション関数（インデクス変換されているかもしれない）
//をCの配列形式に変換する(i2 -> i[1])
void trafunc_num_2_array(const char *in_str, string &out_str)
{
	out_str.erase();
	char buf[10];
	int j,d;
	for(j=0;in_str[j]!='\0';++j){
		while(in_str[j]!='i'){
			out_str = out_str + in_str[j];
			j++;
			if(in_str[j]=='\0') return;
		}
		j++;
		d=in_str[j]-'0';
		if(isdigit(in_str[j+1])){
			++j;
			d*=10;
			d+=in_str[j]-'0';
		}

		sprintf(buf,"i[%d]",d-1);	//ここが配列変換のポイント
		out_str = out_str + buf;
	}
	
}


void func_2_index(map<int,int> &funcvar2index, string &s);


void BSPetri::OutC(const char *dir,const char *mod_name,void (*trace)(const char* format, ...),
				   int output_max, int main_out_flag, int QROQS_out_flag, int Makefile_out_flag)
{

	char *fname=new char[MAX_PATH];


	trace("BSP : %s を出力しています．\n",mod_name);


	if(QROQS_out_flag){
		sprintf(fname,"%s/QROQS.c",dir);
		CopyFile(CMOD_DIR"/QROQS.c",fname,false);//上書き
		sprintf(fname,"%s/QROQS.h",dir);
		CopyFile(CMOD_DIR"/QROQS.h",fname,false);//上書き
	}


	time_t t_now;
	t_now = time(NULL);

	FILE *fp;

	int i;

	/*
	ウージは複数値の入力のみ扱う
	ケムマキは値を記録するもののみあつかう
	*/

	//	ヘッダの出力
	trace("bsp_%s.h\n",mod_name);
	sprintf(fname,"%s/bsp_%s.h",dir,mod_name);
	fp = fopen(fname, "w");
	setbuf(fp, NULL);//バッファを使わん
	fprintf(fp,
		"/******************************************\n"
		"	bsp_%s.h\n"
		"	QROQS over to C Language !!\n"
		"	Created by QROQS\n"
		"	Update   : %s"
		,mod_name, ctime(&t_now));

	fprintf(fp,"\nInput\n");
	map<int,bspUhzi*>::iterator uit,uit_end;
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(i=0;uit!=uit_end;++uit,i++){
		if(uit->second->input_name.empty()){
			fprintf(fp,"	%2d : uhzi_%d\n",i,uit->first);
		}
		else{
			fprintf(fp,"	%2d : %s\n",i,uit->second->input_name.c_str());
		}
	}
	fprintf(fp,"Output\n");
	map<int,bspKemmakix*>::iterator kit,kit_end;
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(i=0;kit!=kit_end;++kit,i++){
		if(kit->second->output_name.empty()){
			fprintf(fp,"	%2d : kem_%d\n",i,kit->first);
		}
		else{
			fprintf(fp,"	%2d : %s\n",i,kit->second->output_name.c_str());
		}
	}
	fprintf(fp,"******************************************/\n");

	fprintf(fp,"#ifndef BitSerialPetriNetC_%s_H_INCLUDED\n",mod_name);
	fprintf(fp,"#define BitSerialPetriNetC_%s_H_INCLUDED\n\n",mod_name);
	fprintf(fp,"#include \"QROQS.h\"\n\n");
	fprintf(fp,"BSPetri *bsp_%s_init(void);\n",mod_name);
	fprintf(fp,"int bsp_%s_io(BSPetri *bsp);\n",mod_name);
	fprintf(fp,"void bsp_%s_in_print(BSPetri *bsp);\n",mod_name);
	fprintf(fp,"void bsp_%s_out_print(BSPetri *bsp);\n\n",mod_name);
	fprintf(fp,"#endif\n");
	fclose(fp);



	//	BSPソースコードの出力
	trace("bsp_%s.c\n",mod_name);
	sprintf(fname,"%s/bsp_%s.c",dir,mod_name);
	fp = fopen(fname, "w");
	setbuf(fp, NULL);//バッファを使わん

	fprintf(fp,
		"/******************************************\n"
		"	bsp_%s.c\n"
		"	QROQS over to C Language !!\n"
		"	Created by QROQS\n"
		"	Update   : %s"
		"******************************************/\n"
		,mod_name, ctime(&t_now));
	fprintf(fp,"#include \"bsp_%s.h\"\n",mod_name);







	trace("ﾄﾗﾝｼﾞｼｮﾝ関数の登録中...\n");
//TRACE("tra func\n");
	//トランジション関数の登録
	map<string,int> trafuncMap;


	map<int,int> deletra;


	string s;
	int j,st;
	//トランジション関数を全て抽出

	int func_id=1;
	// 後置を出す時
	map<int,bspTransition*>::iterator tit,tit_end;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
	//	TRACE("tra %d\n",tit->first);

		bspTransition &tran = *(tit->second);

		if(tran.pass_tra){
			tran.func2id = 0;
			tran.guard2id = 0;
			continue;
		}
		else{
			if(tran.fspost){
		//		TRACE("%s;\n%s;\n",tran.fapost,tran.fspost);
		//		TRACE("%d\n",strlen(tran.fspost));
				s = tran.fspost;
			}
			else s = "";
			if(trafuncMap.find(s)==trafuncMap.end()){
				//TRACE("%s/\n",s.c_str());
				trafuncMap[s] = func_id;
				func_id++;
			}
			tran.func2id = trafuncMap[s];
deletra[tran.func2id] = tit->first;

			//ガード式も登録
			if( tran.fapost ){
				s = tran.fapost;
	//			s.erase(s.find('\n'));
				if(trafuncMap.find(s)==trafuncMap.end()){
					//TRACE("%s/\n",s.c_str());
					trafuncMap[s] = func_id;
					func_id++;
				}
				tran.guard2id = trafuncMap[s];
deletra[tran.guard2id] = tit->first;
			}
			else{
				tran.guard2id = 0;
			}
		}
	}




	//	トランジション関数の出力
	char *buf;
	map<string,int>::iterator fit;
	int func_n;
	for(func_n=1;func_n<=trafuncMap.size();++func_n){
		fit=find_by_value(trafuncMap.begin(),trafuncMap.end(),func_n);

		fprintf(fp,"\n");
		//関数宣言
		fprintf(fp,"void trafunc_%d(char *i,char *o)\n{\n",func_n);

		j=0;
		st=0;
		for(i=0;;i++){
			//後置記法のトランジション関数
			j=fit->first.find('\n',j);
			if(j==fit->first.npos) break;
			j++;

	//		TRACE("%s/",fit->first.c_str());
			//ここですでにインデクス変換されているトランジション関数を
			//Verilogの優先順位からCの優先順位に変更するため括弧をつけまくる

			buf = new char[(j-st)*5];
			readtext(fit->first.substr(st,j-st-1).c_str(), buf);

	//sprintf(buf,"%s",fit->first.c_str());
			//さらに、配列形式にする
			/*
			if(TranMap[deletra[func_n]]->func2id!=func_n){
				TRACE("def func %d\n%s\n",deletra[func_n],buf);
			}
			*/
		//	TRACE("%s/\n",buf);
			trafunc_num_2_array(buf, s);
			delete[] buf;

			fprintf(fp,"	o[%d] = %s;\n",i,s.c_str());

			st=j;
		}

		fprintf(fp,"}\n");
	}

/*
tit = TranMap.begin();
for(;tit!=TranMap.end();++tit){
	if(tit->second->pass_tra) continue;
	fprintf(fp,"tran %d\n",tit->first);
	if(tit->second->fapost) fprintf(fp,"	? %s",tit->second->fapost);
	fprintf(fp,"%s\n\n",tit->second->fspost);
}
*/


	//入出力プレース数
	fprintf(fp,"unsigned char bsp_global_in_place_size[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		fprintf(fp,"%d,",tit->second->in_place_num);
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	for(i=0;i<UhziMap.size();i++)	fprintf(fp,"0,");
	for(i=0;i<KemMap.size();i++)	fprintf(fp,"1,");
	fprintf(fp," 0 };\n");
	fprintf(fp,"unsigned char bsp_global_out_place_size[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		fprintf(fp,"%d,",tit->second->out_place_num);
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	for(i=0;i<UhziMap.size();i++)	fprintf(fp,"1,");
	for(i=0;i<KemMap.size();i++)	fprintf(fp,"0,");
	fprintf(fp," 0 };\n");


	trace("初期ﾏｰｷﾝｸﾞを行っています...\n");

	map<int,bspPlace*>::iterator pit,pit_end;

/*
#define _BTOKEN_OFF		0x10	// 16
#define _WTOKEN_OFF		0x00	//  0
#define _BTOKEN_EXIST	0x11	// 17
#define _WTOKEN_EXIST	0x01	//  1
*/

	fprintf(fp,"char bsp_global_init_token[]={");
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		if(pit->second->GetTokenFlag()==true){
			if(pit->second->GetOldColor()==_BTOKEN){
#ifndef _C_DEBUG
				fprintf(fp,"17,");
			}
			else{
				fprintf(fp,"1,");
			}
		}
		else{
			fprintf(fp,"0,");
		}
#else
				fprintf(fp,"_BTOKEN_EXIST, // %d\n",pit->first);
			}
			else{
				fprintf(fp,"_WTOKEN_EXIST, // %d\n",pit->first);
			}
		}
		else{
			fprintf(fp,"_WTOKEN_OFF, // %d\n",pit->first);
		}
#endif
	}
	fprintf(fp," 0 };\n");



	fprintf(fp,"char bsp_global_init_value[]={");
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		if(pit->second->GetTokenFlag()==true){
			fprintf(fp,"%d,",pit->second->GetTokenValue());
		}
		else{
			fprintf(fp,"0,");
		}
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",pit->first);
#endif
	}
	fprintf(fp," 0 };\n");

//TRACE("set muller\n");
	//トランジションの出力プレースに全て同じ色のトークンがあったらそれをリセットとする

	fprintf(fp,"char bsp_global_init_fire[]={");
	int fire;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		if(tit->second->out_place_num){
			fire=1;
			//入力は見ない
			//出力を見る
			for(i=0;i<tit->second->out_place_num;i++){
				if(PlaceMap[tit->second->OutPlaceList[i]]->GetOldColor() != _BTOKEN){
					fire=0;
					break;
				}
			}
		//	TRACE(" %d,",fire);
			//初期発火の設定
			fprintf(fp,"%d,",fire);
		}
		else{
			fprintf(fp,"0,");
		}
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	for(i=0;i<UhziMap.size();i++){
		fprintf(fp,"0,");
	}
	for(i=0;i<KemMap.size();i++){
		fprintf(fp,"0,");
	}
	fprintf(fp," 0 };\n");

	trace("ﾄﾗﾝｼﾞｼｮﾝ関数の出力中...\n");
	int o;

	//トランジション関数の宣言
	fprintf(fp,"unsigned short bsp_global_func[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		fprintf(fp,"%d,",tit->second->func2id);
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	fprintf(fp," 0 };\n");

	//ガード式の宣言
	fprintf(fp,"unsigned short bsp_global_guard[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		fprintf(fp,"%d,",tit->second->guard2id);
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	fprintf(fp," 0 };\n");


	//ﾌｨｰﾄﾞﾊﾞｯｸ数
	fprintf(fp,"unsigned char bsp_global_feedback_size[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		fprintf(fp,"%d,",tit->second->loop_num);
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	fprintf(fp," 0 };\n");


	//ﾌｨｰﾄﾞﾊﾞｯｸの初期値
	map<int,int>::iterator vit;
	fprintf(fp,"char bsp_global_feedback_init[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		if(tit->second->loop_num==0) continue;

		for(i=tit->second->in_place_num+1;i<=tit->second->in_place_num+tit->second->loop_num;i++){
			vit = find_by_value(
				func_in_index_transrate[tit->first].begin(),
				func_in_index_transrate[tit->first].end(),
				i);
			if(vit==func_in_index_transrate[tit->first].end()){	// 値を使用しないﾌｨｰﾄﾞﾊﾞｯｸ
				fprintf(fp,"0,");
			}
			else{
				fprintf(fp,"%d,",tit->second->init_value[vit->first-1]);
			}
		}

#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	fprintf(fp," 0 };\n");


	//マップ変換
	i=0;	//配列なので0から
	map<int,int> obj_map;
	pit = PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit,i++){
		obj_map[ pit->first ] = i;
	}
	i=0;
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit,i++){
		obj_map[ tit->first ] = i;
	}
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit,i++){
		obj_map[ uit->first ] = i;
	}
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit,i++){
		obj_map[ kit->first ] = i;
	}


	trace("接続情報の出力中...\n");
	map<int,int>::iterator ioit;
	multimap<int,int>::iterator aioit;

	fprintf(fp,"int bsp_global_in_place[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		for(i=0;i<tit->second->in_place_num;i++){
			fprintf(fp,"%d,",obj_map[ tit->second->InPlaceList[i] ] );
		}
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		fprintf(fp,"%d,",obj_map[ ArcMap[ArcOutMap.lower_bound(kit->first)->second].In ] );
	}
	fprintf(fp," 0 };\n");




	fprintf(fp,"int bsp_global_out_place[]={");
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		for(o=0;o<tit->second->out_place_num;o++){
			fprintf(fp,"%d,",obj_map[ tit->second->OutPlaceList[o] ] );
		}
#ifdef _C_DEBUG
		fprintf(fp," // %d\n",tit->first);
#endif
	}
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		fprintf(fp,"%d,",obj_map[ ArcMap[ArcInMap.lower_bound(uit->first)->second].Out ] );
	}
	fprintf(fp," 0 };\n");

	multimap<int,int>::iterator aioit2;

	map<int,set<int> > iotuk;

	int tid;
	tit = TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		tid = tit->first;
		// 入力
		aioit = ArcOutMap.lower_bound(tid);
		for(;aioit!=ArcOutMap.upper_bound(tid);++aioit){
			aioit2 = ArcOutMap.lower_bound( ArcMap[aioit->second].In );
			for(;aioit2!=ArcOutMap.upper_bound( ArcMap[aioit->second].In );++aioit2){
				iotuk[ obj_map[tid] ].insert( ArcMap[aioit2->second ].In );
			}
		}
		// 出力
		aioit = ArcInMap.lower_bound(tid);
		for(;aioit!=ArcInMap.upper_bound(tid);++aioit){
			aioit2 = ArcInMap.lower_bound( ArcMap[aioit->second].Out );
			for(;aioit2!=ArcInMap.upper_bound( ArcMap[aioit->second].Out );++aioit2){
				iotuk[ obj_map[tid] ].insert( ArcMap[aioit2->second ].Out );
			}
		}
	}
	int pid;
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		pid = ArcMap[ArcInMap.lower_bound(uit->first)->second].Out;
		aioit = ArcInMap.lower_bound( pid );
		for(;aioit!=ArcInMap.upper_bound( pid );++aioit){
			iotuk[ obj_map[uit->first] ].insert( ArcMap[aioit->second].Out );
		}
	}
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		pid = ArcMap[ArcOutMap.lower_bound(kit->first)->second].In;
		aioit = ArcOutMap.lower_bound( pid );
		for(;aioit!=ArcOutMap.upper_bound( pid );++aioit){
			iotuk[ obj_map[kit->first] ].insert( ArcMap[aioit->second].In );
		}
	}

	map<int,set<int> >::iterator msit,msit_end;
	set<int>::iterator sit;
	msit = iotuk.begin();
	msit_end = iotuk.end();
	fprintf(fp,"short bsp_global_joint_size[]={");
	for(;msit!=msit_end;++msit){
		fprintf(fp,"%d,",msit->second.size() );
	}
	fprintf(fp," 0 };\n");

	msit = iotuk.begin();
	msit_end = iotuk.end();
	fprintf(fp,"int bsp_global_joints[]={");
	for(;msit!=msit_end;++msit){
//		fprintf(fp,"		");
		sit = msit->second.begin();
		for(;sit!=msit->second.end();++sit){
			fprintf(fp,"%d,",obj_map[ *sit ] );
		}
//		fprintf(fp,"\n");
	}
	fprintf(fp," 0 };\n");



	fprintf(fp,"\n");
	fprintf(fp,"BSPetri *bsp_%s_init(void)\n{\n",mod_name);


	fprintf(fp,"	void (*trafuncs[])(char*,char*)={\n");
	fprintf(fp,"		0,\n");
	for(i=1;i<=trafuncMap.size();i++){
		fprintf(fp,"		trafunc_%d,\n",i);
	}
	fprintf(fp,"		0 };\n");


	fprintf(fp,"	return bsp_init(\n");
	fprintf(fp,"		%d,\n",PlaceMap.size());
	fprintf(fp,"		%d,\n",TranMap.size());
	fprintf(fp,"		%d,\n",UhziMap.size());
	fprintf(fp,"		%d,\n",KemMap.size());
	fprintf(fp,
		"		bsp_global_init_token,\n"
		"		bsp_global_init_value,\n"
		"		bsp_global_init_fire,\n"
		"		bsp_global_in_place_size,\n"
		"		bsp_global_out_place_size,\n"
		"		trafuncs,\n"
		"		bsp_global_func,\n"
		"		bsp_global_guard,\n"
		"		bsp_global_feedback_size,\n"
		"		bsp_global_feedback_init,\n"
		"		bsp_global_in_place,\n"
		"		bsp_global_out_place,\n"
		"		bsp_global_joint_size,\n"
		"		bsp_global_joints\n"
		"		);\n"
		"}\n\n"
		);



	fprintf(fp,"int bsp_%s_io(BSPetri *bsp)\n{\n",mod_name);


	trace("入力値を設定しています...\n");
	map<int,int> udmax;
	
	fprintf(fp,"	char *in[]={\n");
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		bspUhzi *uhzi = uit->second;
		fprintf(fp,"		\"");
//		TRACE("u%d:%d\n",uit->first,uit->second.addp);
		if(uhzi->addp==1){
			udmax[uit->first] = 0;
			uhzi->Set();
		//	trace("	dmax %d\n",uit->second.dmax);
			//	TRACE("end %d\n",uit->second.endflag);
			for(;uhzi->endflag==false;){
				for(i=0;i<uhzi->dmax;i++){
					fprintf(fp,"%d",uhzi->InputData[i]);
				}
				udmax[uit->first] += uhzi->dmax;
				uhzi->Set();
				//TRACE("	dmax %d\n",uit->second.dmax);
			}
		}
		else{
			// レジスタウージはトラプレに変換されてるはず
		}
		fprintf(fp,"\",\n");
	}
	fprintf(fp,"		0 };\n");


	fprintf(fp,"	int u_dmax[]={");
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(;uit!=uit_end;++uit){
		fprintf(fp,"%d,",udmax[uit->first]);
	}
	fprintf(fp," 0 };\n");

	udmax.clear();



	fprintf(fp,"	int k_dmax[]={");
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		fprintf(fp,"%d,",output_max/*kit->second.dmax*/);
	}
	fprintf(fp," 0 };\n");

	fprintf(fp,
		"	return bsp_io_data(\n"
		"		bsp,\n"
		"		u_dmax,\n"
		"		k_dmax,\n"
		"		in\n"
		"		);\n"
		"}\n\n"
		);


	fprintf(fp,"void bsp_%s_in_print(BSPetri *bsp)\n{\n",mod_name);

	fprintf(fp,"	char *input_name[]={\n");
	uit = UhziMap.begin();
	uit_end = UhziMap.end();
	for(i=0;uit!=uit_end;++uit,i++){
		if(uit->second->input_name.empty()){
			fprintf(fp,"		\"uhzi_%d\",\n",uit->first);
		}
		else{
			fprintf(fp,"		\"%s\",\n",uit->second->input_name.c_str());
		}
	}
	fprintf(fp,"		0 };\n");

	fprintf(fp,
		"	bsp_bsp_in_print(\n"
		"		bsp,\n"
		"		input_name\n"
		"		);\n"
		"}\n\n"
		);


	fprintf(fp,"void bsp_%s_out_print(BSPetri *bsp)\n{\n",mod_name);

	fprintf(fp,"	char *output_name[]={\n");
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		if(kit->second->output_name.empty()){
			fprintf(fp,"		\"kem_%d\",\n",kit->first);
		}
		else{
			fprintf(fp,"		\"%s\",\n",kit->second->output_name.c_str());
		}
	}
	fprintf(fp,"		0 };\n");

	fprintf(fp,"	char dline[]={");
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		fprintf(fp,"%d,",kit->second->dline);
	}
	fprintf(fp," 0};\n");

	fprintf(fp,"	int bits[]={");
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		fprintf(fp,"%d,",kit->second->bits);
	}
	fprintf(fp," 0};\n");

	fprintf(fp,"	char hex[]={");
	kit = KemMap.begin();
	kit_end = KemMap.end();
	for(;kit!=kit_end;++kit){
		fprintf(fp,"%d,",kit->second->hex);
	}
	fprintf(fp," 0};\n");

	fprintf(fp,
		"	bsp_bsp_out_print(\n"
		"		bsp,\n"
		"		output_name,\n"
		"		dline, bits, hex\n"
		"		);\n"
		"}\n"
		);



	fclose(fp);


	// Makefile の出力
	if(Makefile_out_flag){
		trace("Makefileを出力しています...\n");
		sprintf(fname,"%s/Makefile",dir);
		fp = fopen(fname, "w");
		setbuf(fp, NULL);//バッファを使わん
		fprintf(fp,
			"all:\n"
			"	gcc -o %s main.c bsp_%s.c QROQS.c -O2\n"
			,mod_name,mod_name);
		fclose(fp);
	}

	if(main_out_flag){
		// main.c の出力
		trace("main.cを出力しています...\n");
		sprintf(fname,"%s/main.c",dir);
		fp = fopen(fname, "w");
		setbuf(fp, NULL);//バッファを使わん

		fprintf(fp,"#include \"bsp_%s.h\"\n",mod_name);
		fprintf(fp,"int main(void)\n{\n");
		fprintf(fp,"	BSPetri *bsp_%s;\n",mod_name);

		fprintf(fp,
			"	bsp_%s = bsp_%s_init();\n"
			"	if(!bsp_%s){\n"
			"		printf(\"init error !!\\n\");\n"
			"		return 1;\n"
			"	}\n"
			,mod_name,mod_name,mod_name
		);

		fprintf(fp,"	bsp_%s_io( bsp_%s );\n",mod_name,mod_name);
		fprintf(fp,"	//bsp_%s_in_print( bsp_%s );\n",mod_name,mod_name);
		fprintf(fp,"	bsp_reset( bsp_%s );\n",mod_name);
		fprintf(fp,"	bsp_run( bsp_%s, %d ); // max_time\n",mod_name,confSimMaxTurn *10);
		fprintf(fp,"	bsp_%s_out_print( bsp_%s );\n",mod_name,mod_name);
		fprintf(fp,"	bsp_io_exit( bsp_%s );\n",mod_name);
		fprintf(fp,"	bsp_exit( bsp_%s );\n",mod_name);
		fprintf(fp,"	return 0;\n}\n");
		fclose(fp);

	}//main_out_flag

	delete[] fname;


}

///   EOF   ///

