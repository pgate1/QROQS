
#include "stdafx.h"

#include "BSPetri.h"
#include "other.h"



//#define MCR_REQ_DELAY 490
//#define MCS_REQ_DELAY 470

#define RESET_DELAY	(2460)

#define UHZI_REQ_DELAY (10)
//#define MULLERC_DELAY (200)
//#define TRAN_REQ_DELAY (700)
//#define KEM_ACK_DELAY (490)
//#define DELAY_SLACK (0.0)

enum{
	_VNONE,
	_VTRAN,
	_VUHZI,
	_VKEM,
	_VCONFSELECTP,
	_VMOD
};



void print_combo(FILE *fp,const char *Format,const char *Into, const char *End,int n)
{
	if(n==0) return;
	int i;
	for(i=1;i<n;++i){
		fprintf(fp,Format,i);
		fprintf(fp,Into);
	}
	fprintf(fp,Format,n);
	fprintf(fp,End);
}


void print_combo(FILE *fp,const char *Format,const char *Into, const char *End,set<int> &setn)
{
	if(setn.size()==0) return;
	int i;
	set<int>::iterator sit;
	sit=setn.begin();
	for(i=1;i<setn.size();++i,++sit){
		fprintf(fp,Format,*sit);
		fprintf(fp,Into);
	}
	fprintf(fp,Format,*sit);
	fprintf(fp,End);
}

void write_pipe(FILE *fp, const char *module_name, bspTransition &tran, int tid, int in)
{

	fprintf(fp,"\n");


//	TRACE("t %d in %d\n",tid,in);

// inが　1というのもありうる
	// ﾚｼﾞｽﾀうーじとか

	//moduleの宣言
	fprintf(fp,"module %s_pipe_%d_%c(\n",module_name,in,tran.init_fire==1?'s':'r');
	print_combo(fp, "ir_%d,", " ", "\n", in);
	fprintf(fp,"req_out,\n");
	fprintf(fp,"reset\n);\n");

	//端子の宣言
	fprintf(fp,"input	");
	print_combo(fp, "ir_%d", ", ", ";\n", in);
	fprintf(fp,"output	req_out;\n");
	fprintf(fp,"input	reset;\n");

	fprintf(fp,"	wire	r1,r2;\n");

#define __NEW_PIPE
#ifdef __NEW_PIPE
	if(in==1){
		fprintf(fp,"	assign	r1 = ir_1;\n");

		fprintf(fp,"	assign	r2 = ir_1;\n");
	}
	else if(in==2){
		fprintf(fp,"	assign	r1 = ir_1;\n");
		fprintf(fp,"	assign	r2 = ir_2;\n");
	}
	else if(in==3){
		fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch mc_t \n*/\n");
		fprintf(fp,"	muller_c_%c	mc_t(.a(ir_1), .b(ir_2), .q(r1), .reset(reset));\n"
			,tran.init_fire==1?'s':'r');
		fprintf(fp,"	assign	r2 = ir_3;\n");
	}
	else{
		//2入力にしてしまう
		fprintf(fp,"	assign	r1 = ");
		print_combo(fp, "ir_%d", " & ", ";\n",in);
		fprintf(fp,"	assign	r2 = ");
		print_combo(fp, "ir_%d", " | ", ";\n",in);
	}

/*
	if(in==1){
		fprintf(fp,"	assign	req_out = r1 & reset;\n");
	}
	else{
	*/
		fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch mc \n*/\n");
		fprintf(fp,"	muller_c_%c	mc(.a(r1), .b(r2), .q(req_out), .reset(reset));\n"
			,tran.init_fire==1?'s':'r');
//	}

#else
/*
	//リクエストを全てC素子をとる
	int i;
	int dan,d,o;

	for(i=1;i<=in;++i){
		fprintf(fp,"	wire tnm_1_%d_r;\n",i);
		fprintf(fp,"	assign tnm_1_%d_r = ir_%d;\n",i,i);
	}
	for(dan=1,o=d=in;o>1;d=o,++dan){
		for(i=1,o=1;i<=(d%2==1?d-1:d);++i,++o){
			fprintf(fp,
				"/* synopsys dc_script_begin\n"
				"   set_dont_touch {mc_%d_%d_r}\n/\n",dan,i);
			fprintf(fp,"	muller_c_%c	mc_%d_%d_r(\n",tran.init_fire==1?'s':'r',dan,i);
			fprintf(fp,"		.a(tnm_%d_%d_r),\n",dan,i);
			++i;
			fprintf(fp,"		.b(tnm_%d_%d_r),\n",dan,i);
			fprintf(fp,"		.q(tnm_%d_%d_r),\n",dan+1,o);
			fprintf(fp,"		.reset(reset));\n");
		}
		if(d%2==1){
			fprintf(fp,"	wire tnm_%d_%d_r;\n",dan+1,o);
			fprintf(fp,"	assign tnm_%d_%d_r = tnm_%d_%d_r;\n",dan+1,o,dan,i);
		}
		else --o;
	}
	fprintf(fp,"	assign req_out = tnm_%d_1_r;\n",dan);
*/
#endif


	fprintf(fp,"endmodule\n");
	
}


//使用されている入力変数
void func_var(const string &str, set<int> &vset)
{
	int j,d;
	vset.clear();
	for(j=0;str[j]!='\0';++j){
		j=str.find('i',j);
		if(j==str.npos) break;
		++j;
		d=str[j]-'0';
		if(isdigit(str[j+1])){
			++j;
			d*=10;
			d+=str[j]-'0';
		}
		if(vset.find(d)==vset.end()){
			vset.insert(d);
		}
	}
}


//使用されている入力変数
void func_var(const string &str, vector<int> &vset)
{
	if(str.empty()) return;
	int j,d;
	for(j=0;str[j]!='\0';++j){
		j=str.find('i',j);
		if(j==str.npos) break;
		++j;
		d=str[j]-'0';
		if(isdigit(str[j+1])){
			++j;
			d*=10;
			d+=str[j]-'0';
		}
		vset[d-1] = 1;
	}
}

void BSPetri::write_tran(FILE *fp,const char *module_name, bspTransition &tran,int tid,
						 const map<int, map<int,string> > &tf_in_vars,
						 const vTran &vtran)
{
	int i,o;

	map<int,const char*> out_vstr,in_vstr;
	for(i=1;i<=tran.incount;++i){
		in_vstr[i] = tran.GetInValue(i).c_str();
	}
	for(o=1;o<=tran.outcount;++o){
		out_vstr[o] = tran.GetOutValue(o).c_str();
	}

	fprintf(fp,"\n");
	
	//moduleの宣言
	fprintf(fp,"module %s_tran_%d(\n",module_name,tid);
	for(i=1;i<=tran.incount;++i){
		if(tran.loopbackin[i-1]) continue;
		fprintf(fp," i_%d_di_%s,\n",i,in_vstr[i]);
		if(vtran.data_only_in[i-1]) continue;
		fprintf(fp," i_%d__ri, i_%d_ao,\n",i,i);
	}
	for(;i<=tran.in_place_num+tran.loop_num;++i){
		if(tran.loopbackin[i-1]) continue;
		fprintf(fp," i_%d__ri, i_%d_ao,\n",i,i);
	}
	for(o=1;o<=tran.outcount;++o){
		if(tran.loopbackout[o-1]) continue;
		fprintf(fp," o_%d_do_%s,\n",o,out_vstr[o]);
		if(vtran.data_only_out[o-1]) continue;
		fprintf(fp," o_%d__ro, o_%d_ai,\n",o,o);
	}
	for(;o<=tran.out_place_num+tran.loop_num;++o){
		if(tran.loopbackout[o-1]) continue;
		fprintf(fp," o_%d__ro, o_%d_ai,\n",o,o);
	}
	fprintf(fp," reset\n);\n");
	
	//端子の宣言
	for(i=1;i<=tran.incount;++i){
		if(tran.loopbackin[i-1]) continue;
		fprintf(fp,"input	i_%d_di_%s;\n",i,in_vstr[i]);
		if(vtran.data_only_in[i-1]) continue;
		fprintf(fp,"input	i_%d__ri;\n",i);
		fprintf(fp,"output	i_%d_ao;\n",i);
	}
	for(;i<=tran.in_place_num+tran.loop_num;++i){
		if(tran.loopbackin[i-1]) continue;
		fprintf(fp,"input	i_%d__ri;\n",i);
		fprintf(fp,"output	i_%d_ao;\n",i);
	}
	for(o=1;o<=tran.outcount;++o){
		if(tran.loopbackout[o-1]) continue;
		fprintf(fp,"output	o_%d_do_%s;\n",o,out_vstr[o]);
		if(vtran.data_only_out[o-1]) continue;
		fprintf(fp,"input	o_%d_ai;\n",o);
		fprintf(fp,"output	o_%d__ro;\n",o);
	}
	for(;o<=tran.out_place_num+tran.loop_num;++o){
		if(tran.loopbackout[o-1]) continue;
		fprintf(fp,"input	o_%d_ai;\n",o);
		fprintf(fp,"output	o_%d__ro;\n",o);
	}
	fprintf(fp,"input	reset;\n");


	// ループデータwireは、外部接続がないため、内部wireだけでよい
	for(i=1;i<=tran.incount;++i){
		if(tran.loopbackin[i-1]){
			fprintf(fp,"wire	i_%d_di_%s;\n",i,in_vstr[i]);
		}
	}
	for(o=1;o<=tran.outcount;++o){
		if(tran.loopbackout[o-1]){
			fprintf(fp,"wire	o_%d_do_%s;\n",o,out_vstr[o]);
		}
	}


	fprintf(fp,"	wire	req;\n");
	for(i=1;i<=tran.in_place_num+tran.loop_num;i++){
		if(tran.loopbackin[i-1]) continue;
		if(vtran.data_only_in[i-1]) continue;
		fprintf(fp,"	assign	i_%d_ao = req;\n",i);
	}


	for(i=1;i<=tran.incount;++i){
		if(tran.init_value[i-1]==1){
			fprintf(fp,"	supply1	Hi;\n");
			break;
		}
	}


	// ﾚｼﾞｽﾀー＞組み合わせ回路wire
	if(tran.non_reg==0){
		for(i=1;i<=tran.incount;++i){
			fprintf(fp,"	wire	d_%s;\n",in_vstr[i]);
		}
	}

	// ガード式に使用されている入力変数番号にフラグを立てる
	vector<int> guard_vars;
	guard_vars.resize(tran.incount);
	for(i=0;i<guard_vars.size();i++) guard_vars[i]=0;
//	TRACE("%s\n",tran.astr.c_str());
	func_var(tran.astr, guard_vars);



	fprintf(fp,"	wire req_out;\n");

	if( tran.guard2id ){

		//ガード式が記述されている場合
		fprintf(fp,"	wire	r_guard;\n");
		fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch {");
		fprintf(fp,"guard_%d} \n*/\n",tid);
		fprintf(fp,"	%s_guard_%d	guard_%d(",module_name,tran.guard2id,tid);
		for(i=1;i<=tran.incount;i++){
			if(guard_vars[i-1]==0) continue;
			if(tran.loopbackin[i-1]){
				fprintf(fp,".%s(d_%s), ",in_vstr[i],in_vstr[i]);
			}
			else{
				fprintf(fp,".%s(i_%d_di_%s), ",in_vstr[i],i,in_vstr[i]);
			}
		}
		fprintf(fp,".guard_out(r_guard));\n");

		// ガード式用遅延ﾓｼﾞｭｰﾙ
		for(i=1;i<=tran.incount;i++){
			if(guard_vars[i-1]==0) continue;
			if(vtran.data_only_in[i-1]) continue;
			if(tran.loopbackin[i-1]) continue;
			fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch {");
			fprintf(fp,"gdelay_%d} \n*/\n",i);
			fprintf(fp,"	wire	g_%d, g_%d_ri;\n",i,i);
			fprintf(fp,"	%s_gdelay_%d	gdelay_%d(\n",module_name,tid,i);
			fprintf(fp,"		.req_in(i_%d__ri),\n",i);
			fprintf(fp,"		.req_out(g_%d));\n",i);
			fprintf(fp,"	assign g_%d_ri = i_%d__ri & g_%d;\n",i,i,i);
		}
		// functionは合成できない？
	}

	//pipe
	fprintf(fp,"	%s_pipe_%d_%c pipe(\n"
		,module_name
		,vtran.pipe_size
		,tran.init_fire==1?'s':'r');

	int pi;
	pi = 1;
	for(i=1;i<=tran.in_place_num+tran.loop_num;++i){
		if(tran.loopbackin[i-1]) continue;
		if(i<=tran.incount && vtran.data_only_in[i-1]) continue;
		if(i<=tran.incount && guard_vars[i-1]){
			fprintf(fp,"		.ir_%d(g_%d_ri & r_guard),\n",pi,i);
		}
		else fprintf(fp,"		.ir_%d(i_%d__ri),\n",pi,i);
		pi++;
	}
	for(o=1;o<=tran.out_place_num+tran.loop_num;++o){
		if(tran.loopbackout[o-1]) continue;
		if(o<=tran.outcount && vtran.data_only_out[o-1]) continue;
		fprintf(fp,"		.ir_%d(~o_%d_ai),\n",pi,o);
		pi++;
	}
	// 出力がないならループバックのアックを入れる
	// 出力があるなら必要ないだろう
	if(tran.loop_num && tran.out_place_num==0){
		fprintf(fp,"		.ir_%d(~req_out),\n",pi);
		pi++;
	}
	fprintf(fp,"		.req_out(req),\n");
	fprintf(fp,"		.reset(reset)\n		);\n");

	
	//レジスタ
	if(tran.non_reg==1){
		fprintf(fp,"// Register reduction.\n");
		fprintf(fp,"	assign o_1_do_%s = i_1_di_%s;\n",out_vstr[1],in_vstr[1]);
		//TRACE("non\n");
	}
	else{
		for(i=1;i<=tran.incount;++i){
			if(tran.loopbackin[i-1]==0){
				if(tran.init_value[i-1]==0){
					// 立ち上がりスルー、立下りホールド
					fprintf(fp,"	DFR	inbuf_%d_r( .C(req), .XR(reset),\n",i);
					fprintf(fp,"		.D(i_%d_di_%s), .Q(d_%s) );\n",i,in_vstr[i],in_vstr[i]);
					/*
					fprintf(fp,"	DLR	inbuf_%d_r(\n",i);
					fprintf(fp,"		.G(req),\n");
					fprintf(fp,"		.D(i_%d_d_i),\n",i);
					fprintf(fp,"		.XR(reset),\n");
					fprintf(fp,"		.Q(data_%d)\n	);\n",i);
					*/
				}
				else if(tran.init_value[i-1]==1){
					// 立ち上がりスルー、立下りホールド
					fprintf(fp,"	DFSR	inbuf_%d_s( .C(req), .XS(reset), .XR(Hi),\n",i);
					fprintf(fp,"		.D(i_%d_di_%s), .Q(d_%s), .XQ(w%d) );\n",i,in_vstr[i],in_vstr[i],i);
					//wはどこにもつないではいけない。この線は使用しない
					
					//fprintf(fp,"	BKEEP	kp(.Y(w%d));\n",i);
					
					/*
					fprintf(fp,"	DLSR	inbuf_%d_s(\n",i);
					fprintf(fp,"		.G(req),\n");
					fprintf(fp,"		.D(i_%d_d_i),\n",i);
					fprintf(fp,"		.Q(data_%d),\n",i);
					fprintf(fp,"		.XS(reset),\n");
					fprintf(fp,"		.XR(Hi),\n");
					//wはどこにもつないではいけない。この線は使用しない
					fprintf(fp,"		.XQ(w%d)\n		);\n",i);
					*/
				}

			}

			//ループバックでイネーブルつきの奴を使う必要は無い
			//しかし、値保持のために必要
			else{//loopback
				fprintf(fp,"//	ループバックレジスタです\n");
				if(tran.init_value[i-1]==0){
					// 立ち上がりホールド、立ち下げスルー
					fprintf(fp,"	DFNR	inbuf_%d_r( .XC(req), .XR(reset),\n",i);
					fprintf(fp,"		.D(i_%d_di_%s), .Q(d_%s) );\n",i,in_vstr[i],in_vstr[i]);
				}
				else if(tran.init_value[i-1]==1){
					
					fprintf(fp,"	DFNSR	inbuf_%d_s( .XC(req), .XS(reset), .XR(Hi),\n",i);
					fprintf(fp,"		.D(i_%d_di_%s), .Q(d_%s), .XQ(w%d) );\n",i,in_vstr[i],in_vstr[i],i);
					//wはどこにもつないではいけない。この線は使用しない
				}
			}
		}
	}
	

	fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch {");
	fprintf(fp,"delay_%d} \n*/\n",tid);

	//次の段に選択分岐がある場合は立下りを早くする
	int noid=0;
	for(o=1;o<=tran.outcount;o++){
		if(tran.loopbackout[o-1]) continue;
		noid = vArcMap[vObjMap[tid].joint_out[o-1]].out_obj;
		if(vObjMap[noid].type==_VCONFSELECTP) break;
		noid = 0;
	}
	if(noid){
		//立下りの遅延迂回
		//立下りはnegedge width以上でなければならない
		fprintf(fp,"	wire o_r;\n");
		fprintf(fp,"	%s_delay_%d	delay_%d(\n",module_name,tid,tid);
		fprintf(fp,"		.req_in(req),\n");
		fprintf(fp,"		.req_out(o_r) );\n");

		fprintf(fp,"	assign	req_out = req & o_r;\n");
	}
	else{
		//迂回ナシ
		fprintf(fp,"	%s_delay_%d	delay_%d(\n",module_name,tid,tid);
		fprintf(fp,"		.req_in(req),\n");
		fprintf(fp,"		.req_out(req_out) );\n",o);
	}

	for(o=1;o<=tran.out_place_num+tran.loop_num;o++){
		if(tran.loopbackout[o-1]) continue;
		if(vtran.data_only_out[o-1]) continue;
		fprintf(fp,"	assign	o_%d__ro = req_out;\n",o);
	}

		

	if(tran.non_reg==0){
		
		//トランジション関数
		fprintf(fp,"/* synopsys dc_script_begin\n   set_dont_touch {");
		fprintf(fp,"func_%d} \n*/\n",tid);
		if(tran.fspost){

			//トランジション関数モジュール
			fprintf(fp,"	%s_trafunc_%d	func_%d(\n",module_name,tran.func2id,tid);
			map<int,string>::const_iterator vit,vit_end;
			vit = tf_in_vars.find(tran.func2id)->second.begin();
			vit_end = tf_in_vars.find(tran.func2id)->second.end();
			for(;vit!=vit_end;++vit){
				fprintf(fp,"		.%s(d_%s),\n",vit->second.c_str(),vit->second.c_str());
			}

			for(o=1;;){
				fprintf(fp,"		.%s(o_%d_do_%s)",out_vstr[o],o,out_vstr[o]);
				o++;
				if(o>tran.outcount) break;
				fprintf(fp,",\n");
			}
			fprintf(fp,"	);\n");
		}
		
		// ループバック
		for(o=1;o<=tran.outcount;o++){
			i=tran.loopbackout[o-1];
			if(i==0) continue;
			// ループバックした値が入力として使用されていないならループしない
			if(i > tran.incount) continue;
			fprintf(fp,"	assign	i_%d_di_%s = o_%d_do_%s;\n",i,in_vstr[i],o,out_vstr[o]);
		}
		
	}// ~non_reg

	
	fprintf(fp,"endmodule\n");
	
	
}

void BSPetri::write_delay_module(FILE *fp,const char *module_name,
								 const char *delay_name,int delay,int tid)
{
	
	fprintf(fp,"\n");
	
	//moduleの宣言
	fprintf(fp,"module %s_%s_%d(\n",module_name,delay_name,tid);
	fprintf(fp," req_in, req_out\n);\n");
	
	fprintf(fp,"input	req_in;\n");
	fprintf(fp,"output	req_out;\n");

	delay += delay * delay_slack;
	fprintf(fp,"	assign	#%d	req_out = req_in;\n",delay);
	
	fprintf(fp,"endmodule\n");

}


//#define ROHM06
#define ROHM035

int BSPetri::delay_data(int tid)
{


//int d_tran=674;	// test

	int delay, ntid, d;
	int reg_set, next_reg_setup, next_mc_set;

	map<string,int>::iterator vit;

	bspTransition &tran = *(TranMap[tid]);

	//リクエストを伝えるのみの場合、とりあえず
	if(tran.incount==0 && tran.outcount==0) return 300;

	delay = 0;
	//	+入力変数に対応するレジスタのset
	reg_set = 0;
	vit=tran.in_var.begin();
	for(;vit!=tran.in_var.end();++vit){
		d = 0;
		if(tran.loopbackin[vit->second -1]==0){
			if(tran.init_value[vit->second -1]==0){
#ifdef ROHM035
				d = 530;	//0.35DFR
#endif
#ifdef ROHM06
				d = 390;	//0.6DFR
#endif
			}
			else{
#ifdef ROHM035
				d = 550;	//0.35DFSR
#endif
#ifdef ROHM06
				d = 450;	//0.6DFSR
#endif
			}
		}
		else{//feedbackloop
			if(tran.init_value[vit->second -1]==0){
#ifdef ROHM035
				d = 580;	//0.35DFNR
#endif
#ifdef ROHM06
				d = 440;	//0.6DFNR
#endif
			}
			else{
#ifdef ROHM035
				d = 630;	//0.35DFNSR
#endif
#ifdef ROHM06
				d = 510;	//0.6DFNSR
#endif
			}
		}
		if(d > reg_set) reg_set = d;
	}
	if(tran.non_reg==1) reg_set=0;

//if(tid==d_tran) TRACE("	set max %d\n",maxd);

	delay += reg_set;

	next_mc_set = 999999;

	//	+次のレジスタのsetup
	next_reg_setup = 0;

	vit = tran.out_var.begin();
	for(;vit!=tran.out_var.end();++vit){

		if(tran.loopbackout[vit->second-1]){//feedback
			ntid=0;
		}
		else ntid = vArcMap[vObjMap[tid].joint_out[vit->second-1]].out_obj;
			
		d = 0;
		//	TRACE("%d\n",tid);
		if(ntid==0){//feedback
			if(tran.init_value[tran.loopbackout[vit->second-1]-1]==0){
#ifdef ROHM035
				d = 230;	//0.35DFNR
#endif
#ifdef ROHM06
				d = 210;	//0.6DFNR
#endif
			}
			else{
#ifdef ROHM035
				d = 250;	//0.35DFNSR
#endif
#ifdef ROHM06
				d = 230;	//0.6DFNSR
#endif
			}
		}
		else if(TranMap.find(ntid)==TranMap.end() ){
			//	TRACE("%d\n",ntid);
			//ケムマキのばあいとりあえず
#ifdef ROHM035
			d = 320;
#endif
#ifdef ROHM06
			d = 300;
#endif
		}
		else if(TranMap[ntid]->non_reg==1){
			//なし
		}
		else{
			if(TranMap[ntid]->init_value[vArcMap[vObjMap[tid].joint_out[vit->second-1]].out_line-1] ==0){
#ifdef ROHM035
				d = 280;	//0.35DFR
#endif
#ifdef ROHM06
				d = 260;	//0.6DFR
#endif
			}
			else{
#ifdef ROHM035
				d = 320;	//0.35DFSR
#endif
#ifdef ROHM06
				d = 300;	//0.6DFSR
#endif
			}
		}
		if(d > next_reg_setup) next_reg_setup = d;
		

		d = 0;
		//if(tid==d_tran) TRACE("	setup max %d\n",maxd);
		
		//	-次のC素子のset(上)
		if(ntid==0) ntid=tid;
		if(tran.non_reg==1){
		}
		
		//	else if(mObjMap[ntid].type==_VCONFSELECTP){	//次に選択分岐などがある場合は遅延減少はしない?
		//		TRACE("%d\n",ntid);
		//	}
		
		else if( TranMap.find(ntid)==TranMap.end()){
			//ケムマキの場合とりあえず
#ifdef ROHM035
			d = 250;	//MCR_REQ_DELAY;//muller_c_r
#endif
#ifdef ROHM06
			d = 330;	//MCR_REQ_DELAY;//muller_c_r
#endif
		}
		else if( TranMap[ntid]->init_fire==0){
#ifdef ROHM035
			d = 420;	//MCR_REQ_DELAY;//muller_c_r
#endif
#ifdef ROHM06
			d = 330;	//MCR_REQ_DELAY;//muller_c_r
#endif
		}
		else{
#ifdef ROHM035
			d = 250;	//MCS_REQ_DELAY;//muller_c_s
#endif
#ifdef ROHM06
			d = 330;	//MCS_REQ_DELAY;//muller_c_s
#endif
		}
		
		if(d < next_mc_set) next_mc_set = d;
	}
//if(tid==d_tran) TRACE("	cdelay max %d\n",maxd);

	delay += next_reg_setup;
	delay -= next_mc_set;

	if(delay<0) delay=0;

	return delay;

}

void print_vars(FILE *fp, const map<string,int> &vars)
{
	map<string,int>::const_iterator vit;
	vit = vars.begin();
	for(;vit!=vars.end();++vit){
		fprintf(fp,"%s,",vit->first.c_str());
	}

}

void write_func(const char *dir,const char *module_name,
				const char *func_name,
				map<string,int> &funcMap,
				map<int, map<int,string> > &in_vars,
				map<int, map<int,string> > &out_vars)
{
	FILE *fp;
	char *fname=new char[MAX_PATH];

	sprintf(fname,"%s/%s_%s.v",dir,module_name,func_name);
	fp=fopen(fname,"w");

	delete[] fname;

	time_t t_now;
	t_now = time(NULL);
	fprintf(fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : %s\n"
		"//	function : transition guard or function\n"
		"//	update   : %s"
		"//\n"
		,func_name,ctime(&t_now));

	fprintf(fp,"\n");


	map<string,int>::iterator tfit;

	map<int, map<string,int>::iterator> find_tfmap;
	tfit = funcMap.begin();
	for(;tfit!=funcMap.end();++tfit){
		find_tfmap[tfit->second] = tfit;
	}

	int func_n;
	for(func_n=1;func_n<=funcMap.size();func_n++){

		tfit = find_tfmap[func_n];

		fprintf(fp,"\n");
		//moduleの宣言
		fprintf(fp,"module %s_%s_%d(\n",module_name,func_name,func_n);

		map<int,string>::iterator vit,vit_end;

		vit = in_vars[func_n].begin();
		vit_end = in_vars[func_n].end();
		for(;vit!=vit_end;){
			fprintf(fp,"%s",vit->second.c_str());
			++vit;
			if(vit==vit_end) break;
			fprintf(fp,",");
		}
		vit = out_vars[func_n].begin();
		vit_end = out_vars[func_n].end();
		if(vit!=vit_end) fprintf(fp,",");
		for(;vit!=vit_end;){
			fprintf(fp,"%s",vit->second.c_str());
			++vit;
			if(vit==vit_end) break;
			fprintf(fp,",");
		}
		fprintf(fp," );\n");

		
		//端子の宣言
		vit = in_vars[func_n].begin();
		vit_end = in_vars[func_n].end();
		if(vit!=vit_end){
			fprintf(fp,"input ");
			for(;;){
				fprintf(fp,"%s",vit->second.c_str());
				++vit;
				if(vit==vit_end) break;
				fprintf(fp,",");
			}
			fprintf(fp,";\n");
		}
		vit = out_vars[func_n].begin();
		vit_end = out_vars[func_n].end();
		if(vit!=vit_end){
			fprintf(fp,"output ");
			for(;vit!=vit_end;){
				fprintf(fp,"%s",vit->second.c_str());
				++vit;
				if(vit==vit_end) break;
				fprintf(fp,",");
			}
			fprintf(fp,";\n");
		}

		{
			string fstr;
			fstr = tfit->first;
			int i,j,st;
			j=0;
			st=0;
			for(i=1;;i++){
				//そのままのトランジション関数
				j=fstr.find('\n',j);
				if(j==fstr.npos) break;
				j++;

				fprintf(fp,"	assign	%s;\n",fstr.substr(st,j-st-1).c_str());
				
				st=j;
			}
		}

		fprintf(fp,"endmodule\n");
	}

	fclose(fp);
}



//アービタモジュールの出力
void BSPetri::write_pjoint(const char *dir, const char *module_name)
{
	FILE *fp;
	char *fname=new char[MAX_PATH];
	sprintf(fname,"%s/%s_pjoint.v",dir,module_name);
	fp=fopen(fname,"w");


	delete[] fname;

	time_t t_now;
	t_now = time(NULL);
	fprintf(fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : place joint\n"
		"//	function : select & conflict arbiter module\n"
		"//	update   : %s"
		"//\n"
		,ctime(&t_now));

	fprintf(fp,"\n\n");

	////////////////	プレースモジュール	////////////////
	set<string> writed_pjoint;
	char buf[20];

	int i;

	map<int,vObj>::iterator moit,moit_end;
	moit = vObjMap.begin();
	moit_end = vObjMap.end();
	for(;moit!=moit_end;++moit){
		if(moit->second.type!=_VCONFSELECTP) continue;

		sprintf(buf,"%d_%d",moit->second.joint_in.size(),moit->second.joint_out.size());
		if(writed_pjoint.find(buf)!=writed_pjoint.end()) continue;
		writed_pjoint.insert(buf);

		fprintf(fp,"\n");

	fprintf(fp,"module %s_pjoint_%d_%d(\n",module_name,
		moit->second.joint_in.size(),moit->second.joint_out.size());
	//fprintf(fp,"module %s_pjoint_%d(\n",module_name,moit->first);

	for(i=1;i<=moit->second.joint_in.size();++i){
		fprintf(fp,"	i_%d__ri, i_%d_ao, i_%d_di,\n",i,i,i);
	}
	for(i=1;i<=moit->second.joint_out.size();++i){
		fprintf(fp,"	o_%d__ro, o_%d_ai, o_%d_do,\n",i,i,i);
	}
	fprintf(fp,"	reset\n	);\n");
	fputc('\n',fp);

	//input output
	for(i=1;i<=moit->second.joint_in.size();++i){
		fprintf(fp,"input	i_%d__ri, i_%d_di;\n",i,i);
		fprintf(fp,"output	i_%d_ao;\n",i);
	}
	for(i=1;i<=moit->second.joint_out.size();++i){
		fprintf(fp,"input	o_%d_ai;\n",i);
		fprintf(fp,"output	o_%d__ro, o_%d_do;\n",i,i);
	}
	fprintf(fp,"input	reset;\n");
	fputc('\n',fp);

	fprintf(fp,"	wire req_in,data_in,ack_in;\n");

	int o;

//#define ARBITER

	if(moit->second.joint_in.size()>1){
#ifdef ARBITER
	int dan,d;
		fprintf(fp,"	/* input %d conflict, tornament by %d arbiter. */\n"
			,moit->second.joint_in.size(),moit->second.joint_in.size()-1);
		for(i=1;i<=moit->second.joint_in.size();++i){
			fprintf(fp,"	wire tnm_1_%d_r,tnm_1_%d_a,tnm_1_%d_d;\n",i,i,i);
			fprintf(fp,"	assign	tnm_1_%d_r = i_%d__ri,\n",i,i);
			fprintf(fp,"		i_%d_ao = tnm_1_%d_a,\n",i,i);
			fprintf(fp,"		tnm_1_%d_d = i_%d_di;\n",i,i);
		}
		for(dan=1,o=d=moit->second.joint_in.size();o>1;d=o,++dan){
			for(i=1,o=1;i<=(d%2==1?d-1:d);++i,++o){
				fprintf(fp,
					"/* synopsys dc_script_begin\n"
					"   set_dont_touch {arb_%d_%d}\n*/\n",dan,i);
				fprintf(fp,"	arbiter	arb_%d_%d(\n",dan,i);
				fprintf(fp,"		.i_1__ri(tnm_%d_%d_r), .i_1_ao(tnm_%d_%d_a), .i_1_di(tnm_%d_%d_d),\n"
					,dan,i,dan,i,dan,i);
				++i;
				fprintf(fp,"		.i_2__ri(tnm_%d_%d_r), .i_2_ao(tnm_%d_%d_a), .i_2_di(tnm_%d_%d_d),\n"
					,dan,i,dan,i,dan,i);
				fprintf(fp,"		.o_1__ro(tnm_%d_%d_r), .o_1_ai(tnm_%d_%d_a), .o_1_do(tnm_%d_%d_d),\n"
					,dan+1,o,dan+1,o,dan+1,o);
				fprintf(fp,"		.reset(reset));\n");
			}
			if(d%2==1){
				fprintf(fp,"	wire tnm_%d_%d_r,tnm_%d_%d_a,tnm_%d_%d_d;\n",dan+1,o,dan+1,o,dan+1,o);
				fprintf(fp,"	assign	tnm_%d_%d_r = tnm_%d_%d_r,\n",dan+1,o,dan,i);
				fprintf(fp,"		tnm_%d_%d_a = tnm_%d_%d_a,\n",dan,i,dan+1,o);
				fprintf(fp,"		tnm_%d_%d_d = tnm_%d_%d_d;\n",dan+1,o,dan,i);
			}
			else --o;
		}
		fprintf(fp,"	assign	req_in = tnm_%d_1_r,\n",dan);
		fprintf(fp,"		data_in = tnm_%d_1_d,\n",dan);
		fprintf(fp,"		tnm_%d_1_a = ack_in;\n",dan);
#else
		
		//競合合流が選択分岐で競合しないようになっているならアービタはいらないはず
		//アック幅が短すぎる?
		fprintf(fp,"	assign req_in = ");
		print_combo(fp,"i_%d__ri", " | ", ";\n", moit->second.joint_in.size());
		print_combo(fp,"	assign i_%d_ao = ack_in;\n","","",moit->second.joint_in.size());

		for(i=1;i<=moit->second.joint_in.size();++i){
			fprintf(fp,"	wire data_%d;\n",i);
			fprintf(fp,"	assign data_%d = i_%d__ri & i_%d_di;\n",i,i,i);
		}
		fprintf(fp,"	assign data_in = ");
		print_combo(fp,"data_%d"," | ",";\n",moit->second.joint_in.size());
		
#endif
	}
	else{
		fprintf(fp,
			"	assign	req_in = i_1__ri,\n"
			"		data_in = i_1_di,\n"
			"		i_1_ao = ack_in;\n"
		);
	}


	for(o=1;o<=moit->second.joint_out.size();++o){
		fprintf(fp,"	assign	o_%d__ro = req_in,\n",o);
		fprintf(fp,"		o_%d_do = data_in;\n",o);
	}

	//アックをすべてor
	if(moit->second.joint_out.size()>=2){
		fprintf(fp,"	/* select, all ack throw. */\n");
	}
	fprintf(fp,"	assign ack_in = ");
	print_combo(fp,"o_%d_ai"," | ",";\n",moit->second.joint_out.size());

	fprintf(fp,"endmodule\n");


	}//for

	fclose(fp);
}


void write_controlline(const char *dir, const char *module_name, 
					   map<int,bspTransition*> &TranMap, map<int,vTran> &vTranMap)
{
	time_t t_now;
	t_now = time(NULL);

	char *fname=new char[MAX_PATH];

	FILE *pfp;

	set<string> pipe_set;
	sprintf(fname,"%s/%s_pipe.v",dir,module_name);
	pfp=fopen(fname,"w");
	delete[] fname;
	fprintf(pfp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : pipe\n"
		"//	function : control pipes\n"
		"//	update   : %s"
		"//\n"
		,ctime(&t_now));
	char *buf = new char[30];
	map<int,bspTransition*>::iterator tit,tit_end;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition &tran=*(tit->second);

		// bufはここだけで識別のために使用する。
		sprintf(buf,"pipe_%d_%c"
			,vTranMap[tit->first].pipe_size
			,tran.init_fire==1?'s':'r');

//		TRACE("%s\n",buf);

		if(pipe_set.find(buf)==pipe_set.end()){
			pipe_set.insert(buf);
			write_pipe(pfp, module_name, tran, tit->first, vTranMap[tit->first].pipe_size);
		}

	}
	fclose(pfp);
	delete[] buf;
}

void BSPetri::write_dataline(const char *dir, const char *module_name,const map<int,bspTransition*> &TranMap,
							 const map<int, map<int,string> > &tf_in_vars, const map<int, map<int,string> > &tf_out_vars,
							 const map<int,vTran> &vTranMap)
{

	time_t t_now;
	t_now = time(NULL);
	int i;
	char *fname=new char[MAX_PATH];

	int muller_c_number,register_number;
	muller_c_number=0;
	register_number=0;
	map<string,int> reg_number;

	reg_number["DFR"]=0;
	reg_number["DFSR"]=0;
	reg_number["DFNR"]=0;
	reg_number["DFNSR"]=0;

	FILE *dfp;
	sprintf(fname,"%s/%s_transition.v",dir,module_name);
	dfp=fopen(fname,"w");
	delete[] fname;
	fprintf(dfp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : transition\n"
		"//	function : transition modules\n"
		"//	update   : %s"
		"//\n"
		,ctime(&t_now));

	map<int,bspTransition*>::const_iterator tit,tit_end;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition &tran=*(tit->second);

		switch(vTranMap.find(tit->first)->second.pipe_size){
		case 0: break;
		case 1:	break;
		case 2: muller_c_number += 1; break;
		case 3: muller_c_number += 2; break;
		default: muller_c_number += 1;
		}

		if(tran.non_reg==0){
			register_number += tran.incount;

			for(i=0;i<tran.incount;++i){
				if(tran.loopbackin[i]){//feedback
					if(tran.init_value[i]==1){
						++reg_number["DFNSR"];
					}
					else{
						++reg_number["DFNR"];
					}
				}
				else{
					if(tran.init_value[i]==1){
						++reg_number["DFSR"];
					}
					else{
						++reg_number["DFR"];
					}
				}
			}
		}

		//データライン
		write_tran(dfp, module_name, tran, tit->first, tf_in_vars, vTranMap.find(tit->first)->second);
	}

	fclose(dfp);

	/*
	trace("使用しているC素子の数 : %d\n",muller_c_number);
	trace("使用しているレジスタ数 : %d\n",register_number);
	map<string,int>::iterator mit;
	mit=reg_number.begin();
	for(;mit!=reg_number.end();++mit){
		trace("	%s : %d 個\n",mit->first.c_str(),mit->second);
	}
	*/

}


void write_dc_script(const char *dir, const char *module_name,
					const char *module_compile_option,
				 	const char *trafunc_compile_option,
					 int func_size, int guard_size)
{
	int i;
	char *fname = new char[MAX_PATH];
	FILE *fp;

	sprintf(fname,"%s/%s.dc",dir,module_name);
	fp=fopen(fname,"w");
	delete[] fname;
	
	//trace("スクリプトの出力中...\n");
	fprintf(fp,"sh rm %s_trafunc.rep %s_trafunc.vnet %s_delay.vnet\n",
		module_name,module_name,module_name);
	fprintf(fp,"read -format verilog %s_trafunc.v\n",module_name);
	for(i=1;i<=func_size;++i){
		fprintf(fp,"current_design = %s_trafunc_%d\n",module_name,i);

		fprintf(fp,"%s\n",trafunc_compile_option);

		/*
		fprintf(fp,"set_wire_load_model -name 1k\n");
		fprintf(fp,"max_area 0\n");
		fprintf(fp,"max_delay 0.0 all_outputs()\n");
///		fprintf(fp,"compile -ungroup_all -map_effort high\n");
		fprintf(fp,"compile %s\n",trafunc_compile_option);
		*/
//		fprintf(fp,"write -format verilog -output %s_trafunc_%d.vnet\n",module_name,i);
//		fprintf(fp,"sh cat %s_trafunc_%d.vnet >> %s_trafunc.vnet\n",module_name,i,module_name);
//		fprintf(fp,"sh rm %s_trafunc_%d.vnet\n",module_name,i);
		fprintf(fp,"report_area >> %s_trafunc.rep\n",module_name);
		fprintf(fp,"report_timing >> %s_trafunc.rep\n",module_name);
//		fprintf(fp,"remove_design %s_trafunc_%d\n",module_name,i);
	}
	fprintf(fp,"sh ../tools/delayadd %s_delay.data %s_trafunc.rep %s_delay.vnet trafunc delay\n",module_name,module_name,module_name);

	fprintf(fp,"sh rm %s_guard.rep %s_guard.vnet %s_gdelay.vnet\n",
		module_name,module_name,module_name);
	fprintf(fp,"read -format verilog %s_guard.v\n",module_name);
	for(i=1;i<=guard_size;++i){
		fprintf(fp,"current_design = %s_guard_%d\n",module_name,i);

		fprintf(fp,"%s\n",trafunc_compile_option);

		/*
		fprintf(fp,"set_wire_load_model -name 1k\n");
		fprintf(fp,"max_area 0\n");
		fprintf(fp,"max_delay 0.0 all_outputs()\n");
//		fprintf(fp,"compile -ungroup_all -map_effort high\n");
		fprintf(fp,"compile %s\n",trafunc_compile_option);
		*/
//		fprintf(fp,"write -format verilog -output %s_trafunc_%d.vnet\n",module_name,i);
//		fprintf(fp,"sh cat %s_trafunc_%d.vnet >> %s_trafunc.vnet\n",module_name,i,module_name);
//		fprintf(fp,"sh rm %s_trafunc_%d.vnet\n",module_name,i);
		fprintf(fp,"report_area >> %s_guard.rep\n",module_name);
		fprintf(fp,"report_timing >> %s_guard.rep\n",module_name);
//		fprintf(fp,"remove_design %s_guard_%d\n",module_name,i);
	}
	fprintf(fp,"sh ../tools/delayadd %s_gdelay.data %s_guard.rep %s_gdelay.vnet guard gdelay\n",module_name,module_name,module_name);

	fprintf(fp,"read -format verilog { ");
	fprintf(fp,"%s.v ",module_name);
	fprintf(fp,"%s_transition.v ",module_name);
	fprintf(fp,"%s_pipe.v ",module_name);
	fprintf(fp,"%s_pjoint.v ",module_name);
	fprintf(fp,"%s_delay.vnet ",module_name);
	fprintf(fp,"%s_gdelay.vnet }\n",module_name);

	fprintf(fp,"current_design = %s\n",module_name);

	fprintf(fp,"%s\n",module_compile_option);
/*
	fprintf(fp,"set_wire_load_model -name 1k\n");
	fprintf(fp,"max_area 0\n");
	fprintf(fp,"max_delay 0 all_outputs()\n");
	fprintf(fp,"uniquify\n");
	fprintf(fp,"compile %s\n",module_compile_option);
*/
//	fprintf(fp,"write -format verilog -hierarchy -output %s.vnet %s\n",module_name,module_name);
	fprintf(fp,"write -format verilog -hierarchy -output %s.vnet %s\n",module_name,module_name);
//	fprintf(fp,"write -hierarchy -output %s.db %s\n",module_name,module_name);

//	fprintf(fp,"report_area > %s.rep\n",module_name);
//	fprintf(fp,"report_timing >> %s.rep\n",module_name);


	fclose(fp);

}

void BSPetri::write_delay(const char *dir, const char*module_name,
				 int func_size, double delay_slack)
{
	time_t t_now;
	t_now = time(NULL);
	char *fname=new char[MAX_PATH];

	FILE *delay_fp,*delay_data_fp;

	sprintf(fname,"%s/%s_delay.v",dir,module_name);
	delay_fp=fopen(fname,"w");
	sprintf(fname,"%s/%s_delay.data",dir,module_name);
	delay_data_fp=fopen(fname,"w");
	delete[] fname;

	fprintf(delay_fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : delay\n"
		"//	function : setup + set + function delay\n"
		"//	update   : %s"
		"//\n"
		,ctime(&t_now));
	fprintf(delay_data_fp,"%s\n",module_name);
	fprintf(delay_data_fp,"%d,%f;\n",func_size,delay_slack);

	map<int,bspTransition*>::iterator tit,tit_end;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition &tran=*(tit->second);

		tran.delay = delay_data(tit->first);
	//	TRACE("d %d\n",tran.delay);

		fprintf(delay_data_fp,"%d,%d,%d,%d;\n",tit->first,tran.delay,999,tran.func2id);

		write_delay_module(delay_fp, module_name, "delay", tran.delay, tit->first);
	}
	fclose(delay_fp);
	fclose(delay_data_fp);

}


void BSPetri::write_guard_delay(const char *dir, const char*module_name,
				 int func_size, double delay_slack)
{
	time_t t_now;
	t_now = time(NULL);
	char *fname=new char[MAX_PATH];

	FILE *delay_fp,*delay_data_fp;

	sprintf(fname,"%s/%s_gdelay.v",dir,module_name);
	delay_fp=fopen(fname,"w");
	sprintf(fname,"%s/%s_gdelay.data",dir,module_name);
	delay_data_fp=fopen(fname,"w");
	delete[] fname;

	fprintf(delay_fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : guard delay\n"
		"//	function : guard delay (.v no delay)\n"
		"//	update   : %s"
		"//\n"
		,ctime(&t_now));
	fprintf(delay_data_fp,"%s\n",module_name);
	fprintf(delay_data_fp,"%d,%f;\n",func_size,delay_slack);

	map<int,bspTransition*>::iterator tit,tit_end;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition &tran=*(tit->second);

		if(tran.guard2id==0) continue;

		fprintf(delay_data_fp,"%d,%d,%d,%d;\n",tit->first,0,999,tran.guard2id);

		write_delay_module(delay_fp, module_name, "gdelay", 0, tit->first);
	}
	fclose(delay_fp);
	fclose(delay_data_fp);

}

void BSPetri::OutVerilog(const char *dir,
	const char *module_compile_option,
	const char *trafunc_compile_option,
	void (*trace)(const char* format, ...))
{



	trace("BSP : %s を出力しています\n",module_name.c_str());

	map<int,vModule> vModMap;
	map<int,vTran> vTranMap;
	map<int,bspTransition*>::iterator tit,tit_end;
	multimap<int,int>::iterator aioit;

//アーク接続が複数あるプレースのmidは0だ。

	set<int> SelectPlaceSet;
	{

	trace("ｵﾌﾞｼﾞｪｸﾄの登録中...\n");
	vObjMap.clear();

	bspTransition *tran;
	int obj_id,obj_type;
	int i_num,o_num;
	map<int,bspObject>::iterator oit,oit_end;
//TRACE("module map\n");
	oit=ObjMap.begin();
	oit_end = ObjMap.end();
	for(;oit!=oit_end;++oit){
		obj_id = oit->first;

		switch(oit->second.type){
		case _PLACE:
			i_num = ArcOutMap.count(obj_id);
			o_num = ArcInMap.count(obj_id);
			if(i_num==1 && o_num==1){	// パスプレ
				PlaceMap[obj_id]->mid=0;
				continue;
			}
			else if(i_num>=1 && o_num>=1){	// 複数つながってる
				PlaceMap[obj_id]->mid = obj_id;
				obj_type = _VCONFSELECTP;

				if(o_num>=2){	// 選択分岐
					SelectPlaceSet.insert(obj_id);
				}
			}
			else{
//				TRACE("%d %d\n",i_num,o_num);
				ErrPos = obj_id;
				throw "ｱｰｸ接続不備のﾌﾟﾚｰｽがあります．";
			}

			break;
		case _TRANSITION:
			tran = TranMap[obj_id];
			obj_type = _VTRAN;
			i_num = tran->in_place_num + tran->loop_num;
			o_num = tran->out_place_num + tran->loop_num;
			int i,o;
			vTranMap[obj_id];	// 生成
			vTranMap[obj_id].data_only_in.resize(i_num);
			for(i=0;i<i_num;i++) vTranMap[obj_id].data_only_in[i] = 0;
			vTranMap[obj_id].data_only_out.resize(o_num);
			for(o=0;o<o_num;o++) vTranMap[obj_id].data_only_out[o] = 0;
			break;
		case _UHZI:
			obj_type = _VUHZI;
			i_num = 0;
			o_num = 1;
			break;
		case _KEMMAKIX:
			obj_type = _VKEM;
			i_num = 1;
			o_num = 0;
			break;
		case _MODULE:
			obj_type = _VMOD;
			i_num = ArcOutMap.count(obj_id);
			o_num = ArcInMap.count(obj_id);
			break;
		}

		vObjMap[obj_id].type = obj_type;
		vObjMap[obj_id].joint_in.resize(i_num);
		vObjMap[obj_id].joint_out.resize(o_num);
	}

//	trace("input %d\noutput %d\n",UhziMap.size(),KemMap.size());

	}




	{
		// データだけ伝達すればいいパストラを検索
	
		int from_tid,to_tid,pid;
		set<int> to_tran;
		int from_aid,to_aid;
		// トラー＞パスプレ＊ー＞トラ
		tit = TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			if(tit->second->out_place_num<2) continue;
			from_tid = tit->first;
			to_tran.clear();
			aioit = ArcInMap.lower_bound(from_tid);
			for(;aioit!=ArcInMap.upper_bound(from_tid);++aioit){
				from_aid = aioit->second;
				pid = ArcMap[from_aid].Out;
				if(PlaceMap[pid]->mid) continue; // パスプレかどうか
				to_aid = ArcInMap.lower_bound(pid)->second;
				to_tid = ArcMap[to_aid].Out;
				if(TranMap.find(to_tid)==TranMap.end()) continue;
				if(to_tran.find(to_tid)==to_tran.end()){
					to_tran.insert(to_tid);	// req ack有効パスプレ
				}
				else{
					vTranMap[from_tid].data_only_out[
						TranMap[from_tid]->out_arc_to_joint[from_aid]
						] = 1;
					vTranMap[to_tid].data_only_in[
						TranMap[to_tid]->in_arc_to_joint[to_aid]
						] = 1;
				}
			}
		}

	
	// コントロールラインの待合数を求める
		int pipe_size,i,o;
		tit=TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			bspTransition *tran = tit->second;

			pipe_size = tran->in_place_num + tran->out_place_num 
				+ (tran->loop_num && tran->out_place_num==0 ? 1 : 0);
			
			for(i=1;i<=tran->in_place_num+tran->loop_num;i++){
				if(vTranMap[tit->first].data_only_in[i-1]) pipe_size--;
			}
			for(o=1;o<=tran->out_place_num+tran->loop_num;o++){
				if(vTranMap[tit->first].data_only_out[o-1]) pipe_size--;
			}
			vTranMap[tit->first].pipe_size = pipe_size;
		}

/*
		{
			// ちぇっく
			map<int,vTran>::iterator vtit;
			vtit = vTranMap.begin();
			for(;vtit!=vTranMap.end();++vtit){
				TRACE("t %d : ",vtit->first);
				for(i=0;i<vtit->second.data_only_in.size();i++){
					TRACE("%d ",vtit->second.data_only_in[i]);
				}
				for(o=0;o<vtit->second.data_only_out.size();o++){
					TRACE("%d ",vtit->second.data_only_out[o]);
				}
				TRACE("\n");
			}
		}
*/
	}



// モジュールの入出力をインデクスに変換しろ
	// アーク変数をあつめて、ソートされるので、その順序でつなぐ
	{
	int i,o;

	map<string,int> vamap;
	map<int,string>::iterator moit;
	moit = ModMap.begin();
	for(;moit!=ModMap.end();++moit){
		int m_id = moit->first;
		vModMap[m_id];	// 生成

		// 入力側
		vamap.clear();
		vModMap[m_id].in_arc_to_joint.clear();
		aioit = ArcOutMap.lower_bound(m_id);
		for(;aioit!=ArcOutMap.upper_bound(m_id);++aioit){
			vamap[ArcMap[aioit->second].value_name] = aioit->second;
		}
		map<string,int>::iterator vait;
		vait = vamap.begin();
		for(i=0;vait!=vamap.end();++vait,i++){
			vModMap[m_id].in_arc_to_joint[vait->second] = i;
			vModMap[m_id].in_vars[i+1] = vait->first;
		}

		// 出力側
		vamap.clear();
		vModMap[m_id].out_arc_to_joint.clear();
		aioit = ArcInMap.lower_bound(m_id);
		for(;aioit!=ArcInMap.upper_bound(m_id);++aioit){
			vamap[ArcMap[aioit->second].value_name] = aioit->second;
		}
		vait = vamap.begin();
		for(o=0;vait!=vamap.end();++vait,o++){
			vModMap[m_id].out_arc_to_joint[vait->second] = o;
			vModMap[m_id].out_vars[o+1] = vait->first;
		}
	}

	}



	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		tit->second->non_reg=0;
	}


	{
//		trace("連続プレトラを検索しています...\n");
		int pid1,tid1,pid2,tid2,tid3;
		
		// ｹﾑﾏｷへ入力しているﾌﾟﾚｰｽ
		set<int> pick_place,next_place;
		map<int,bspKemmakix*>::iterator kit;
		kit=KemMap.begin();
		for(;kit!=KemMap.end();++kit){
			//	TRACE("%d\n",kit->second.Pp);
			pick_place.insert(kit->second->Pp);
		}

		// パストラ以外のﾄﾗﾝｼﾞｼｮﾝに接続している入力ﾌﾟﾚｰｽ
		tit=TranMap.begin();
		tit_end = TranMap.end();
		for(;tit!=tit_end;++tit){
			if(tit->second->pass_tra) continue;
			aioit=ArcOutMap.lower_bound(tit->first);
			for(;aioit!=ArcOutMap.upper_bound(tit->first);++aioit){
				pid1=ArcMap[aioit->second].In;
					//	TRACE("%d\n",pid1);
				pick_place.insert(pid1);
			}
		}

		// 競合合流、選択分岐ﾌﾟﾚｰｽも追加
		set<int>::iterator sit;
		sit=SelectPlaceSet.begin();
		for(;sit!=SelectPlaceSet.end();++sit){
			pick_place.insert(*sit);
		}
		
		//	TRACE("select\n");
		set<int>::iterator psit,psit_end;
		for(;pick_place.size();){
			psit=pick_place.begin();
			psit_end = pick_place.end();
			for(;psit!=psit_end;++psit){

			//	TRACE("s %d\n",*psit);
				aioit=ArcOutMap.lower_bound(*psit);
				for(;aioit!=ArcOutMap.upper_bound(*psit);++aioit){
					tid1 = ArcMap[aioit->second].In;
					if(TranMap.find(tid1)==TranMap.end()) continue;
					if(!TranMap[tid1]->pass_tra) continue;
					pid1 = ArcMap[ArcOutMap.lower_bound(tid1)->second].In;
					if(PlaceMap[pid1]->mid!=0) continue;
					tid2 = ArcMap[ArcOutMap.lower_bound(pid1)->second].In;
					if(TranMap.find(tid2)==TranMap.end()) continue;
					if(!TranMap[tid2]->pass_tra) continue;
					
					pid2 = ArcMap[ArcOutMap.lower_bound(tid2)->second].In;
					if(PlaceMap[pid2]->mid) continue;
					tid3 = ArcMap[ArcOutMap.lower_bound(pid2)->second].In;
					
					// 入力がウージだったりﾓｼﾞｭｰﾙだったり出力のみのやつだったりしたら
					// 合流してたり
					// ﾚｼﾞｽﾀは削減するな、タイミングが合わん
					if(TranMap.find(tid3)==TranMap.end()) continue;
					if(TranMap[tid3]->in_place_num==0) continue;
					
					if(PlaceMap[pid1]->GetTokenFlag() && PlaceMap[pid1]->GetTokenColor()==_BTOKEN){
						next_place.insert(pid1);
						continue;
					}
					
					//連プレ
				//	TRACE("	トラ %d のレジスタは削減されます\n",tid2);
					//		TRACE("%d\n",pid2);
					TranMap[tid2]->non_reg=1;
					next_place.insert(pid2);
				}
			}
			pick_place.clear();
			pick_place=next_place;
			next_place.clear();
		}

	}



	{
//	trace("パイプラインの接続を行っています...\n");
	vArcMap.clear();
	vArcMapId=1;
	map<int,bspPlace*>::iterator pit,pit_end;
	int p_in_arc,p_out_arc;
	int in_id,out_id;
	int in_joint,out_joint;
//TRACE("module arc\n");
	//入出力アークが１本のプレースについて、その両端を接続する
	pit=PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		if(vObjMap.find(pit->first)!=vObjMap.end()) continue;
		if(pit->second->mid!=0) continue;

		p_in_arc = ArcOutMap.lower_bound(pit->first)->second;
		in_id = ArcMap[p_in_arc].In;
		switch(ObjMap[in_id].type){
		case _TRANSITION:
			in_joint = TranMap[in_id]->out_arc_to_joint[p_in_arc];
			break;
		case _UHZI:
			in_joint = 0;
			break;
		case _MODULE:
			in_joint = vModMap[in_id].out_arc_to_joint[p_in_arc];
			break;
#ifdef _DEBUG
		default: throw "ﾌﾟﾚｰｽの入力がおかしい：内部エラー";
#endif
		}

		p_out_arc = ArcInMap.lower_bound(pit->first)->second;
		out_id = ArcMap[p_out_arc].Out;
		switch(ObjMap[out_id].type){
		case _TRANSITION:
			out_joint = TranMap[out_id]->in_arc_to_joint[p_out_arc];
			break;
		case _KEMMAKIX:
			out_joint = 0;
			break;
		case _MODULE:
			out_joint = vModMap[out_id].in_arc_to_joint[p_out_arc];
			break;
#ifdef _DEBUG
		default: throw "プレースの出力がおかしい：内部エラー";
#endif
		}

		vArcMap[vArcMapId] = vArc(in_id,in_joint+1, out_id, out_joint+1);
		vObjMap[in_id].joint_out[in_joint] = vArcMapId;
		vObjMap[out_id].joint_in[out_joint] = vArcMapId;
		vArcMapId++;
	}


//	trace("競合合流・選択分岐プレースを接続しています...\n");
	//入出力が複数あるプレース(_VCONFSELECTP)を接続する
	pit=PlaceMap.begin();
	pit_end = PlaceMap.end();
	for(;pit!=pit_end;++pit){
		if(vObjMap.find(pit->first)==vObjMap.end()) continue;
		if(vObjMap[pit->first].type!=_VCONFSELECTP) continue;

		//プレースの入力
		out_id = pit->first;
		out_joint = 0;
		aioit = ArcOutMap.lower_bound(pit->first);
		for(;aioit!=ArcOutMap.upper_bound(pit->first);++aioit,++out_joint){
			p_in_arc = aioit->second;
			in_id = ArcMap[p_in_arc].In;

			switch(ObjMap[in_id].type){
			case _TRANSITION:
				in_joint = TranMap[in_id]->out_arc_to_joint[p_in_arc];
				break;
			case _UHZI:
				in_joint = 0;
				break;
			case _MODULE:
				in_joint = vModMap[in_id].out_arc_to_joint[p_in_arc];
				break;
#ifdef _DEBUG
			default: throw "ﾌﾟﾚｰｽの入力がおかしい：内部エラー";
#endif
			}

		//	TRACE("join %d %d\n",in_id,out_id);

			vArcMap[vArcMapId] = vArc(in_id,in_joint+1, out_id, out_joint+1);
			vObjMap[in_id].joint_out[in_joint] = vArcMapId;
			vObjMap[out_id].joint_in[out_joint] = vArcMapId;
			vArcMapId++;
		}

		//プレースの出力
		in_id=pit->first;
		in_joint=0;
		aioit=ArcInMap.lower_bound(pit->first);
		for(;aioit!=ArcInMap.upper_bound(pit->first);++aioit,++in_joint){
			p_out_arc = aioit->second;
			out_id=ArcMap[p_out_arc].Out;

			switch(ObjMap[out_id].type){
			case _TRANSITION:
				out_joint = TranMap[out_id]->in_arc_to_joint[p_out_arc];
				break;
			case _KEMMAKIX:
				out_joint = 0;
				break;
			case _MODULE:
				out_joint = vModMap[out_id].in_arc_to_joint[p_out_arc];
				break;
#ifdef _DEBUG
			default: throw "プレースの出力がおかしい：内部エラー";
#endif
			}

		//	TRACE("join %d %d\n",in_id,out_id);

			vArcMap[vArcMapId] = vArc(in_id,in_joint+1, out_id, out_joint+1);
			vObjMap[in_id].joint_out[in_joint] = vArcMapId;
			vObjMap[out_id].joint_in[out_joint] = vArcMapId;
			vArcMapId++;
		}
	}

	}
	

	{
//	trace("初期マーキングを行っています...\n");
//TRACE("set muller\n");
	//トランジションの出力プレースに全て同じ色のトークンがあったらそれをリセットとする

	int fire;
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition *tran = tit->second;

		tran->init_fire=0;

		// ループバックがあるなら、初期発火はない
		if(tran->loop_num) continue;

		fire=1;
		//入力は見ない
		//出力を見る
		aioit = ArcInMap.lower_bound(tit->first);
		for(;aioit!=ArcInMap.upper_bound(tit->first);++aioit){
			if(PlaceMap[ArcMap[aioit->second].Out]->GetOldColor() != _BTOKEN){
				fire=0;
				break;
			}
		}
		//初期発火の設定
		if(fire==1){
			tran->init_fire=1;
		}
	}

//	TRACE("set token\n");
	//初期トークンの値をトランジションのラッチに設定する
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
		bspTransition *tran = tit->second;
		//出力プレースに黒トークンがあれば値を初期値とする
		aioit = ArcInMap.lower_bound(tit->first);
		for(;aioit!=ArcInMap.upper_bound(tit->first);++aioit){
			if(PlaceMap[ArcMap[aioit->second].Out]->GetTokenFlag()
				&& PlaceMap[ArcMap[aioit->second].Out]->GetTokenColor()==_BTOKEN){
				tran->init_value[tran->out_arc_to_joint[aioit->second]]
					= PlaceMap[ArcMap[aioit->second].Out]->GetTokenValue();
			}
		}
	}

	}


	map<int, map<int, string> > tf_in_vars, tf_out_vars, tg_in_vars, tg_out_vars;
	map<string,int> trafuncMap, traguardMap;

	{
	trace("ﾄﾗﾝｼﾞｼｮﾝ関数の登録中...\n");
//TRACE("tra func\n");
	//トランジション関数の登録


	string s,astr,fstr;
	//トランジション関数を全て抽出

	bspTransition func_tran;

	int func_id=1,guard_id =1;
	// 後置を出す時
	tit=TranMap.begin();
	tit_end = TranMap.end();
	for(;tit!=tit_end;++tit){
	//	TRACE("tra %d\n",tit->first);

		bspTransition &tran = *(tit->second);
		
		tran.func2id = 0;
		tran.guard2id = 0;

	//入出力変数の置き換え
	fstr = tran.fs_spcut;
	//ガード式の分別
	astr.erase();
	if(fstr[0]=='?'){
		int p=fstr.find('\n');
		astr = fstr.substr(1,p);
		fstr.erase(0,p+1);
	}

		if( fstr.empty() ) s = "";
		else s = fstr;
		if(trafuncMap.find(s)==trafuncMap.end()){
		//	TRACE("%s/\n",s.c_str());
			trafuncMap[s] = func_id;

			func_tran.var_str(fstr,func_tran.in_var,func_tran.out_var);

			map<string,int>::iterator vit;
			vit = func_tran.in_var.begin();
			for(;vit!=func_tran.in_var.end();++vit){
		//		TRACE("%s:\n",vit->first.c_str());
				tf_in_vars[func_id][vit->second] = vit->first;
			}
			vit = func_tran.out_var.begin();
			for(;vit!=func_tran.out_var.end();++vit){
		//		TRACE("%s:\n",vit->first.c_str());
				tf_out_vars[func_id][vit->second] = vit->first;
			}

			func_id++;
		}
		tran.func2id = trafuncMap[s];

		//ガード式も登録
		if( !astr.empty() ){

			s = "guard_out=" + astr;
			if(traguardMap.find(s)==traguardMap.end()){
			//	TRACE("%s/\n",s.c_str());
				traguardMap[s] = guard_id;

			func_tran.var_str(s,func_tran.in_var,func_tran.out_var);

			map<string,int>::iterator vit;
			vit = func_tran.in_var.begin();
			for(;vit!=func_tran.in_var.end();++vit){
		//		TRACE("%s:\n",vit->first.c_str());
				tg_in_vars[guard_id][vit->second] = vit->first;
			}
			tg_out_vars[guard_id][1] = "guard_out";

				guard_id++;
			}
			tran.guard2id = traguardMap[s];
		}


	}

	}


	trace("pipe 出力中...\n");
	write_controlline(dir, module_name.c_str(), TranMap, vTranMap);

	trace("transition 出力中...\n");
	write_dataline(dir, module_name.c_str(), TranMap, tf_in_vars, tf_out_vars, vTranMap);

	trace("delay data...\n");

//	TRACE("delay_slack %f\n",delay_slack);


	write_delay(dir, module_name.c_str(), trafuncMap.size(), delay_slack);
	write_guard_delay(dir, module_name.c_str(), traguardMap.size(), delay_slack);


	trace("\n pjoint.v");
	write_pjoint(dir, module_name.c_str());


//TRACE("write tra func\n");
	//トランジション関数
	trace(" trafunc.v");
	write_func(dir,module_name.c_str(),"trafunc",trafuncMap,tf_in_vars,tf_out_vars);
	write_func(dir,module_name.c_str(),"guard",traguardMap,tg_in_vars,tg_out_vars);

	tf_in_vars.clear();
	tf_out_vars.clear();


	trace(" %s.v\n",module_name.c_str());
	write_top_module(dir,module_name.c_str(),vModMap,vTranMap, SelectPlaceSet);

	vObjMap.clear();
	vArcMap.clear();
	vModMap.clear();
	vTranMap.clear();

	write_dc_script(dir,module_name.c_str(),
			module_compile_option,
			trafunc_compile_option,
			trafuncMap.size(), traguardMap.size());

	trafuncMap.clear();

}

void set_uk_names(const map<int,bspUhzi*> &UhziMap, const map<int,bspKemmakix*> &KemMap,
				  map<int,const char *> &uname, map<int,const char *> &kname,
				  set<char *> &delnames)
{
	map<int,bspUhzi*>::const_iterator uit;
	map<int,bspKemmakix*>::const_iterator kit;

	char *buf = new char[100];
	char *nbuf;
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->dline==3) continue;
		if(uit->second->input_name.empty()){
			sprintf(buf,"u%d",uit->first);
			nbuf = new char[strlen(buf)+1];
			strcpy(nbuf, buf);
			uname[uit->first] = nbuf;
			delnames.insert(nbuf);
		}
		else{
			uname[uit->first] = uit->second->input_name.c_str();
		}
	}
	kit = KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		if(kit->second->dline==3) continue;
		if(kit->second->output_name.empty()){
			sprintf(buf,"k%d",kit->first);
			nbuf = new char[strlen(buf)+1];
			strcpy(nbuf, buf);
			kname[kit->first] = nbuf;
			delnames.insert(nbuf);
		}
		else{
			kname[kit->first] = kit->second->output_name.c_str();
		}
	}
	delete[] buf;
}

void free_uk_names(map<int,const char *> &uname, map<int,const char *> &kname,
				   set<char *> &delnames)
{
	uname.clear();
	kname.clear();
	set<char *>::iterator sit;
	sit = delnames.begin();
	for(;sit!=delnames.end();++sit){
		delete[] *sit;
	}
	delnames.clear();
}

//トップモジュールの出力
void BSPetri::write_top_module(const char *dir,const char *module_name,map<int,vModule> &vModMap,
							   map<int, vTran> &vTranMap,
							   	set<int> &SelectPlaceSet)
{
	FILE *fp;
	char *fname=new char[MAX_PATH];
	sprintf(fname,"%s/%s.v",dir,module_name);
	fp=fopen(fname,"w");
	delete[] fname;

	time_t t_now;
	t_now = time(NULL);
	fprintf(fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : %s\n"
		"//	function : top module\n"
		"//	update   : %s"
		"//\n"
		,module_name
		,ctime(&t_now));

	fprintf(fp,"\n\n");

	map<int,bspUhzi*>::iterator uit;
	map<int,bspKemmakix*>::iterator kit;

	// ウージ、ケムマキの名前を決定
	map<int, const char *> uname, kname;
	set<char *> delnames;
	set_uk_names(UhziMap, KemMap, uname, kname, delnames);



	int i;
	fprintf(fp,"module %s(\n",module_name);
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->dline==3) continue;
		fprintf(fp,"	i_%s__ri, i_%s_ao, i_%s_di,\n",uname[uit->first],uname[uit->first],uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		if(kit->second->dline==3) continue;
		fprintf(fp,"	o_%s__ro, o_%s_ai, o_%s_do,\n",kname[kit->first],kname[kit->first],kname[kit->first]);
	}

	map<int,vObj>::iterator moit,moit_end;

	//verilog-debug
	if(v_debug==1){
		if(SelectPlaceSet.size()!=0){
			fprintf(fp,"	debug_select,\n");
		}
	}

	fprintf(fp,"	reset\n	);\n");
	fputc('\n',fp);

	//input output
	uit = UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->dline==3) continue;
		fprintf(fp,"input	i_%s__ri, i_%s_di;\n",uname[uit->first],uname[uit->first]);
		fprintf(fp,"output	i_%s_ao;\n",uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		if(kit->second->dline==3) continue;
		fprintf(fp,"input	o_%s_ai;\n",kname[kit->first]);
		fprintf(fp,"output	o_%s__ro, o_%s_do;\n",kname[kit->first],kname[kit->first]);
	}

	//verilog-debug
	if(v_debug==1){
		if(SelectPlaceSet.size()!=0){
			fprintf(fp,"output	debug_select;\n");
		}
	}

	fprintf(fp,"input	reset;\n");
	fputc('\n',fp);


	//verilogでは、モジュール間の接続のみに使用される
	//１ビットのwireは、宣言を省略することができる。

	vArc varc;
	multimap<int,int>::iterator aioit;


	map<int,vArc>::iterator vait,vait_end;
	vait=vArcMap.begin();
	if(vait->first==0) ++vait;
	vait_end = vArcMap.end();
	for(;vait!=vait_end;++vait){
		varc = vait->second;
		fprintf(fp,"	wire	w_%d_%d_%d_%d_r, w_%d_%d_%d_%d_a, w_%d_%d_%d_%d_d;\n"
			,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
			,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
			,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
	}


	// 生産しないウージ、もしくは消費しないケムマキ用のLow
	uit=UhziMap.begin();
	kit=KemMap.begin();
	for(;uit!=UhziMap.end();++uit){
		if(uit->second->dline==3) break;
	}
	if(uit==UhziMap.end()){
		for(;kit!=KemMap.end();++kit){
			if(kit->second->dline==3) break;
		}
	}
	if(uit!=UhziMap.end() || kit!=KemMap.end()){
		fprintf(fp,"	supply0	Low;\n");
	}




	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		varc = vArcMap[vObjMap[uit->first].joint_out[0]];
		if(uit->second->dline==3){	// 生産しないウージ
			fprintf(fp,"// No Industry Uhzi\n");
			fprintf(fp,"	assign	w_%d_%d_%d_%d_r = Low,\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
			// ackは解放しておく
			fprintf(fp,"		w_%d_%d_%d_%d_d = Low;\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
		}
		else{
			fprintf(fp,"	assign	w_%d_%d_%d_%d_r = i_%s__ri,\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line, uname[uit->first]);
			fprintf(fp,"		i_%s_ao = w_%d_%d_%d_%d_a,\n"
				,uname[uit->first], varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
			fprintf(fp,"		w_%d_%d_%d_%d_d = i_%s_di;\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line, uname[uit->first]);
		}
	}


	int o;
	//トランジション、選択分岐、競合合流の接続
	const char *vstrp;
	moit = vObjMap.begin();
	moit_end = vObjMap.end();
	for(;moit!=moit_end;++moit){
		switch(moit->second.type){
		case _VTRAN:
			fprintf(fp,"	%s_tran_%d	tran_%d(\n",module_name,moit->first,moit->first);

			//入力
			for(i=1;i<=TranMap[moit->first]->incount;++i){
				if(moit->second.joint_in[i-1]==0) continue; // ループバック
				varc = vArcMap[moit->second.joint_in[i-1]];
				vstrp = TranMap[varc.out_obj]->GetInValue(varc.out_line).c_str();
				if(vTranMap[moit->first].data_only_in[i-1]){
					// データだけでよい
					fprintf(fp,
						"		.i_%d_di_%s(w_%d_%d_%d_%d_d),\n"
						,i, vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						);
				}
				else{
					fprintf(fp,
						"		.i_%d__ri(w_%d_%d_%d_%d_r), .i_%d_ao(w_%d_%d_%d_%d_a), .i_%d_di_%s(w_%d_%d_%d_%d_d),\n"
						,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						,i, vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						);
				}
			}
			for(;i<=moit->second.joint_in.size();++i){
				if(moit->second.joint_in[i-1]==0) continue;
				varc = vArcMap[moit->second.joint_in[i-1]];
				fprintf(fp,
					"		.i_%d__ri(w_%d_%d_%d_%d_r), .i_%d_ao(w_%d_%d_%d_%d_a),\n"
					,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}

			//出力
			for(o=1;o<=TranMap[moit->first]->outcount;++o){
				if(moit->second.joint_out[o-1]==0) continue;
				varc = vArcMap[moit->second.joint_out[o-1]];
				vstrp = TranMap[varc.in_obj]->GetOutValue(varc.in_line).c_str();
				if(vTranMap[moit->first].data_only_out[o-1]){
					// データだけでよい
					fprintf(fp,
						"		.o_%d_do_%s(w_%d_%d_%d_%d_d),\n"
						,o, vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						);
				}
				else{
					fprintf(fp,
						"		.o_%d__ro(w_%d_%d_%d_%d_r), .o_%d_ai(w_%d_%d_%d_%d_a), .o_%d_do_%s(w_%d_%d_%d_%d_d),\n"
						,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						,o, vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
						);
				}
			}
			for(;o<=moit->second.joint_out.size();++o){
				if(moit->second.joint_out[o-1]==0) continue;
				varc = vArcMap[moit->second.joint_out[o-1]];
				fprintf(fp,
					"		.o_%d__ro(w_%d_%d_%d_%d_r), .o_%d_ai(w_%d_%d_%d_%d_a),\n"
					,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}

			fprintf(fp,"		.reset(reset)\n	);\n");

			break;
		case _VCONFSELECTP:
			fprintf(fp,"	%s_pjoint_%d_%d	pjoint_%d(\n",module_name,
				moit->second.joint_in.size(),moit->second.joint_out.size(),moit->first);
//			fprintf(fp,"	%s_pjoint_%d	pjoint_%d(\n",module_name,moit->first,moit->first);
			//入力
			for(i=1;i<=moit->second.joint_in.size();++i){
				varc = vArcMap[moit->second.joint_in[i-1]];
				fprintf(fp,
					"		.i_%d__ri(w_%d_%d_%d_%d_r), .i_%d_ao(w_%d_%d_%d_%d_a), .i_%d_di(w_%d_%d_%d_%d_d),\n"
					,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,i, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}
			//出力
			for(o=1;o<=moit->second.joint_out.size();++o){
				varc = vArcMap[moit->second.joint_out[o-1]];
				fprintf(fp,
					"		.o_%d__ro(w_%d_%d_%d_%d_r), .o_%d_ai(w_%d_%d_%d_%d_a), .o_%d_do(w_%d_%d_%d_%d_d),\n"
					,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,o, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}
			fprintf(fp,"		.reset(reset)\n	);\n");

			if(v_debug==1){
				if(moit->second.joint_out.size()>=2){
					fprintf(fp,
						"	//	SELECT MODULE DEBUG\n"
						"	wire debug_select_%d;\n",moit->first);
					fprintf(fp,"	assign debug_select_%d = ",moit->first);
					for(o=1;o<=moit->second.joint_out.size();++o){
						varc = vArcMap[moit->second.joint_out[o-1]];
						fprintf(fp,"w_%d_%d_%d_%d_a"
							, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
						if(o!=moit->second.joint_out.size()){
							fprintf(fp," & ");
						}
					}
					fprintf(fp,";\n");
				}
			}

			break;
		case _VMOD:
		
			fprintf(fp,
					"/* synopsys dc_script_begin\n"
					"   set_dont_touch {%s_%d}\n*/\n",ModMap[moit->first].c_str(),moit->first);
		
			fprintf(fp,"	%s	%s_%d(\n",ModMap[moit->first].c_str(),ModMap[moit->first].c_str(),moit->first);

			//入力
			for(i=1;i<=moit->second.joint_in.size();++i){
				varc = vArcMap[moit->second.joint_in[i-1]];
				vstrp = vModMap[moit->first].in_vars[i].c_str();
				fprintf(fp,
					"		.i_%s__ri(w_%d_%d_%d_%d_r), .i_%s_ao(w_%d_%d_%d_%d_a), .i_%s_di(w_%d_%d_%d_%d_d),\n"
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}
			//出力
			for(o=1;o<=moit->second.joint_out.size();++o){
				varc = vArcMap[moit->second.joint_out[o-1]];
				vstrp = vModMap[moit->first].out_vars[o].c_str();
				fprintf(fp,
					"		.o_%s__ro(w_%d_%d_%d_%d_r), .o_%s_ai(w_%d_%d_%d_%d_a), .o_%s_do(w_%d_%d_%d_%d_d),\n"
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					,vstrp, varc.in_obj, varc.in_line, varc.out_obj, varc.out_line
					);
			}
			fprintf(fp,"		.reset(reset)\n	);\n");

			break;
		default:
			continue;
		}
	}


	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		varc = vArcMap[vObjMap[kit->first].joint_in[0]];
		if(kit->second->dline==3){	// 消費しないケムマキ
			// reqは解放
			fprintf(fp,"// No Consum Kemmakix\n");
			fprintf(fp,"	assign	w_%d_%d_%d_%d_a = Low;\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
			// dataは解放
		}
		else{
			fprintf(fp,"	assign	o_%s__ro = w_%d_%d_%d_%d_r,\n"
				,kname[kit->first], varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
			fprintf(fp,"		w_%d_%d_%d_%d_a = o_%s_ai,\n"
				,varc.in_obj, varc.in_line, varc.out_obj, varc.out_line, kname[kit->first]);
			fprintf(fp,"		o_%s_do = w_%d_%d_%d_%d_d;\n"
				,kname[kit->first], varc.in_obj, varc.in_line, varc.out_obj, varc.out_line);
		}
	}


	free_uk_names(uname,kname,delnames);

	if(v_debug==1){
		if(SelectPlaceSet.size()!=0){
			fprintf(fp,
				"	//	SELECT MODULE DEBUG\n"
				"	assign debug_select = ");
			print_combo(fp,
				"debug_select_%d ", "| ", ";\n"
				,SelectPlaceSet);
		}
	}


	fprintf(fp,"endmodule\n");

	fclose(fp);
}

void BSPetri::write_sim_module(const char *dir,const char *module_name)
{
	char *fname=new char[MAX_PATH];
	sprintf(fname,"%s/%s_sim.v",dir,module_name);
	FILE *fp;
	fp=fopen(fname,"w");
	delete[] fname;

	time_t t_now;
	t_now = time(NULL);
	fprintf(fp,
		"//\n"
		"// created by QROQS\n"
		"//	model    : %s_sim\n"
		"//	function : simulasion module\n"
		"//	update   : %s"
		"//\n"
		,module_name,ctime(&t_now));

	fprintf(fp,"\n");

	fprintf(fp,"`timescale 1ps/1ps\n\n");
	fprintf(fp,"module %s_sim;\n",module_name);

	map<int,bspUhzi*>::iterator uit;
	map<int,bspKemmakix*>::iterator kit;


	// 生産しないウージ、消費しないケムマキを削除
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();){
		if(uit->second->dline==3){
			int uid = uit->first;
			++uit;
			DelObject(uid);
		}
		else ++uit;
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();){
		if(kit->second->dline==3){
			int kid = kit->first;
			++kit;
			DelObject(kid);
		}
		else ++kit;
	}


	// ウージ、ケムマキの名前を決定
	map<int, const char *> uname, kname;
	set<char *> delnames;
	set_uk_names(UhziMap, KemMap, uname, kname, delnames);


	/*
	//wire
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"	wire	u_%d__r, u_%d_a, u_%d_d;\n",uit->first,uit->first,uit->first);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"	wire	k_%d__r, k_%d_a, k_%d_d;\n",kit->first,kit->first,kit->first);
	}
*/

	//reg
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"	reg	swu_%s;\n",uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"	reg	swk_%s;\n",kname[kit->first]);
	}
	fprintf(fp,"	reg	reset;\n");
	fputc('\n',fp);

	//uhzi
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"	uhzi	uhzi_%s( .switch(swu_%s),\n",
			uname[uit->first], uname[uit->first]);
		fprintf(fp,"		.req_out(u_%s__r), .ack_in(u_%s_a),\n",
			uname[uit->first], uname[uit->first]);
		fprintf(fp,"		.data_out(u_%s_d), .reset(reset) );\n\n", uname[uit->first]);
	}

	//kemmakix
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"	kemmakix	kem_%s( .switch(swk_%s),\n",
			kname[kit->first], kname[kit->first]);
		fprintf(fp,"		.req_in(k_%s__r), .ack_out(k_%s_a),\n", 
			kname[kit->first], kname[kit->first]);
		fprintf(fp,"		.data_in(k_%s_d), .reset(reset) );\n\n", kname[kit->first]);
	}


	int i;
	//top module
	fprintf(fp,"	%s	%s_top(\n",module_name,module_name);
	uit=UhziMap.begin();
	for(i=1;uit!=UhziMap.end();++uit,++i){
		fprintf(fp,"		.i_%s__ri(u_%s__r), .i_%s_ao(u_%s_a), .i_%s_di(u_%s_d),\n"
			,uname[uit->first],uname[uit->first],uname[uit->first],uname[uit->first],uname[uit->first],uname[uit->first]);
	}
	kit=KemMap.begin();
	for(i=1;kit!=KemMap.end();++kit){
		fprintf(fp,"		.o_%s__ro(k_%s__r), .o_%s_ai(k_%s_a), .o_%s_do(k_%s_d),\n"
			,kname[kit->first],kname[kit->first],kname[kit->first],kname[kit->first],kname[kit->first],kname[kit->first]);
	}

	map<int,vObj>::iterator moit;

	/*
	//verilog-debug
	if(v_debug==1){
		if(SelectPlaceSet.size()!=0){
			fprintf(fp,"		.debug_select(debug_select),\n");
		}
	}
	*/

	fprintf(fp,"		.reset(reset)\n	);\n");
	fputc('\n',fp);

	fprintf(fp,"	initial begin\n");
	fprintf(fp,"	#0\n");
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"		swu_%s <= 0;\n",uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"		swk_%s <= 0;\n",kname[kit->first]);
	}
	fprintf(fp,"		reset <= 1;\n");
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"		$readmemb(\"indata/%s.data\",uhzi_%s.mem);\n",uname[uit->first],uname[uit->first]);
	}

	fprintf(fp,"	#100\n");
	fprintf(fp,"		reset <= 0;\n");
	fprintf(fp,"	#%d\n",RESET_DELAY + 2000);
	fprintf(fp,"		reset <= 1;\n");
	fprintf(fp,"	#100\n");

	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"		swu_%s <= 1;\n",uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"		swk_%s <= 1;\n",kname[kit->first]);
	}

	fprintf(fp,"	#2000000\n");
	uit=UhziMap.begin();
	for(;uit!=UhziMap.end();++uit){
		fprintf(fp,"		swu_%s = 0;\n",uname[uit->first]);
	}
	kit=KemMap.begin();
	for(;kit!=KemMap.end();++kit){
		fprintf(fp,"		$writememb(\"outdata/%s.data\",kem_%s.mem);\n",kname[kit->first],kname[kit->first]);
	}
	fprintf(fp,"	#10\n");
	fprintf(fp,"		$shm_close();\n");
	fprintf(fp,"		$finish;\n");

	fprintf(fp,"	end\n");


	free_uk_names(uname, kname, delnames);



	fputc('\n',fp);

	fprintf(fp,"	initial begin\n");
	fprintf(fp,"		$shm_open(\"%s_sim.shm\");\n",module_name);
	fprintf(fp,"		$shm_probe(\"A\");\n");
	fprintf(fp,"		$shm_probe(%s_top, \"A\");\n",module_name);

	fprintf(fp,"	end\n");

/*
	if(v_debug==1){
		if(SelectPlaceSet.size()!=0){
			fprintf(fp,
				"\n\n"
				"	//	SELECT MODULE DEBUG\n"
				"	always @(posedge debug_select) begin\n"
				"		$display($time,,,\" select ERROR !!");
			fprintf(fp," ('_';");
			fprintf(fp,
				"\\n\\n\");\n"
				"	end\n");
		}
	}
*/

	fprintf(fp,"endmodule\n");

	fclose(fp);
}

/// EOF ///
