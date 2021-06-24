
#include "stdafx.h"
#include "BSPetriobj.h"
#include"other.h"
#include"compile.h"

#include"BSPetri.h"

//////////////////  Place //////////////////////
bspPlace::bspPlace()
{
//	TRACE("crate\n");
	TokenFlag = false;
	TokenColor=_BTOKEN;
	TokenValue=2;//不定値
	oldcol=_BTOKEN;
	real_place = NULL;

#ifndef _LIGHT
	//verilog
	joint_in_set.clear();//競合グループのプレース群
	joint_out_set.clear();//共有グループのトランジション群
#endif

}

bspPlace::bspPlace(const bspPlace &place)
{
//	TRACE("copy\n");
	*this = place;
}

bspPlace & bspPlace::operator = (const bspPlace &place)
{
//	TRACE("=\n");
	if(this!=&place){

	//	TRACE("place copy %d\n",rand());

		TokenFlag  = place.TokenFlag;//false;
		TokenColor = place.TokenColor;//_BTOKEN;
		TokenValue = place.TokenValue;//2;

		oldcol = place.oldcol;//_BTOKEN;

		real_place = place.real_place;

#ifndef _LIGHT
		//verilog
		joint_in_set.clear();//競合グループのプレース群
		joint_out_set.clear();//共有グループのトランジション群
#endif
	}
	return *this;
}

void bspPlace::Reset()
{
	TokenFlag = false;
	oldcol=_WTOKEN;
}

void bspPlace::SetToken(
		int color,//トークンの色
		int value//トークンの値
		)
{
	if(real_place){
		real_place->SetToken(color, value);
		return;
	}

	TokenColor = color;
	TokenValue = value;
	TokenFlag = true;

	oldcol=color;
}

void bspPlace::SetOldColor(int col)
{
	if(real_place){
		real_place->SetOldColor(col);
		return;
	}
	oldcol = col;
}


bool bspPlace::GetTokenFlag()
{
	if(real_place) return real_place->GetTokenFlag();
	return TokenFlag;
}

int bspPlace::GetTokenColor()
{
	if(real_place) return real_place->GetTokenColor();
	return TokenColor;
}

int bspPlace::GetTokenValue()
{
//	TRACE("tv %d\n",TokenValue);
	if(real_place) return real_place->GetTokenValue();
	return TokenValue;
}

void bspPlace::RemoveToken()
{
	if(real_place){
		real_place->RemoveToken();
		return;
	}
	TokenFlag = false;
	oldcol = TokenColor;
}

int bspPlace::GetOldColor()
{
	if(real_place) return real_place->GetOldColor();
	return oldcol;
}
void bspPlace::SetTokenColor(int col)
{
	if(real_place){
		real_place->SetTokenColor(col);
		return;
	}
	TokenColor = col;
	oldcol = col;
}

void bspPlace::SetTokenValue(int val)
{
	if(real_place){
		real_place->SetTokenValue(val);
		return;
	}
	TokenValue = val;
}

void bspPlace::SetTokenFlag(bool flag)
{
	if(real_place){
		real_place->SetTokenFlag(flag);
		return;
	}
	TokenFlag = flag;
}




bspPlace *bspPlace::GetRealPlace()
{
	if(real_place) return real_place->GetRealPlace();
	return this;
}

bspPlace::~bspPlace()
{
//	TRACE("death\n");
}

/////////////////////   Transition   ///////////////////

#define DEFAULT_TRAFUNC "o1=i1;"
bspTransition::bspTransition()
{
//	TRACE("create\n");
	FireableFlag = false;

	InPlaceList.clear();
	OutPlaceList.clear();

	fstring=DEFAULT_TRAFUNC;
	fstr.erase();
	fspost = NULL;
	fapost = NULL;
	inval = NULL;//入力値
	outval =NULL;

	pass_tra = 1;

#ifndef _LIGHT
	non_reg=0;
#endif

}

bspTransition::bspTransition(const bspTransition &tran)
{
	fspost = NULL;
	fapost = NULL;
	inval = NULL;
	outval =NULL;
//	TRACE("copy\n");
	*this = tran;
}

bspTransition & bspTransition::operator = (const bspTransition &tran)
{

	if(this!=&tran){
//		TRACE("tran copy\n");
		fstring = tran.fstring;
		in_var = tran.in_var;
		out_var = tran.out_var;

		InPlaceList = tran.InPlaceList;
		OutPlaceList = tran.OutPlaceList;

		if(fspost) delete[] fspost;
		if(tran.fspost){
			fspost = new char[strlen(tran.fspost)+1];
			strcpy(fspost, tran.fspost);
		}
		else fspost = NULL;

		if(fapost) delete[] fapost;
		if(tran.fapost){
			fapost = new char[strlen(tran.fapost)+1];
			strcpy(fapost, tran.fapost);
		}
		else fapost = NULL;

		if(inval) delete[] inval;
		if(tran.inval) inval = new int[tran.incount];
		else inval = NULL;

		if(outval) delete[] outval;
		if(tran.outval){
			outval = new int[tran.outcount];
			if(tran.outcount) outval[0] = tran.outval[0];
		}
		else outval = NULL;

		FireableFlag = tran.FireableFlag;

		fs_spcut = tran.fs_spcut;
		fstr = tran.fstr;
		astr = tran.astr;

		in_arc_to_joint = tran.in_arc_to_joint;
		out_arc_to_joint = tran.out_arc_to_joint;

		pass_tra = tran.pass_tra;

		incount = tran.incount;
		outcount = tran.outcount;

		in_place_num = tran.in_place_num;
		out_place_num = tran.out_place_num;


#ifndef _LIGHT
		//verilog
		init_value = tran.init_value;
		loopbackout = tran.loopbackout;
		loopbackin = tran.loopbackin;
		loop_num = tran.loop_num;
		non_reg=0;
#endif

	}
	return *this;
}

void bspTransition::IOPlaceClear()
{
	InPlaceList.clear();
	OutPlaceList.clear();

	InPlaces.clear();
	OutPlaces.clear();

	in_arc_to_joint.clear();
	out_arc_to_joint.clear();
}

string bspTransition::GetInValue(int n)
{
	return find_by_value(
		in_var.begin(),
		in_var.end(),
		n)->first;
}

string bspTransition::GetOutValue(int n)
{
	return find_by_value(
		out_var.begin(),
		out_var.end(),
		n)->first;
}

void bspTransition::FuncCheck()
{
	FuncEduction();
	CompileFuncA();
}

void varreplace_str(string &str,
			 map<string,int> &in_var,
			 map<string,int> &out_var);

void bspTransition::CompileFunc(map<int,bspPlace*> &_PlaceMap)
{
	FireableFlag = false;
	fired_flag=0;

//TRACE("in\n");
	if(InPlaceList.size()==0){
		throw "入力ﾌﾟﾚｰｽを接続してください．";
	}
	if(OutPlaceList.size()==0){
		throw "出力ﾌﾟﾚｰｽを接続してください．";
	}

	int i;

	CompileFuncA();

	//verilog：組み合わせ回路の入力値を推定するのは大変
	//トランジション関数が記述されているにもかかわらず
	//出力プレースに初期黒トークンがあるのは禁止
//	TRACE("out %d\n",outcount);
	string s;
	int j,st;
	j=0;
	st=0;
	for(i=0;i<outcount;++i){
		j=fstr.find('\n',j)+1;
		s=fstr.substr(st,j-st-1);

//		TRACE("%s\n",s.c_str());
		if(_PlaceMap[OutPlaceList[i]]->GetTokenFlag()==1
			&& _PlaceMap[OutPlaceList[i]]->GetTokenColor()==_BTOKEN){
			if(!(s[0]=='i'
				&& isdigit(s[1])
				&& (
					(s.size()==2 && s[2]=='\0')
					|| (s.size()>2 && isdigit(s[2]) && s[3]=='\0')
					)
				)){
				throw "関数が記述されているﾄﾗﾝｼﾞｼｮﾝの出力に黒ﾄｰｸﾝを配置することはできません．この制限は解決されていません．";
			}
		}
		st=j;
	}

//	TRACE("in %d out %d\n",incount,outcount);

	if(inval != NULL)	delete[] inval;
	inval = new int[incount];//入力値
	if(outval != NULL)	delete[] outval;
	outval = new int[outcount];//出力のため

	// 白トークン通過のための初期化
	if(outcount){
		outval[0] = _PlaceMap[OutPlaceList[0]]->GetTokenValue();
	}

//	TRACE(":%s:",fspost);
	//パストラ
	if(in_place_num==1 && out_place_num==1
		&& fspost && strcmp(fspost, "i1\n")==0 && fapost==NULL){
		pass_tra = 1;
	}
	else pass_tra=0;


//	TRACE("compile end\n");
}

void bspTransition::CompileFuncA()
{
	//入出力変数の置き換え
	fstr=fs_spcut;
//	TRACE("f:%s\n",fstr.c_str());
	varreplace_str(fstr, in_var, out_var);

	//ガード式の分別
	astr.erase();
	if(fstr[0]=='?'){
		int p=fstr.find('\n');
		astr=fstr.substr(1,p);
		fstr.erase(0,p+1);
	}

	//出力変数を削除
	int p=0;
	while((p=fstr.find('o',p) )!= fstr.npos){
		if(p!=0 && fstr[p-1]!='\n'){
	//		printf("出力変数を書くこと\n");
	//		break;
		}
		fstr.erase(p, fstr.find('=',p)-p+1);
	}


//	TRACE("astr:\n%s",astr.c_str());
//	TRACE("fstr:%s\n",fstr.c_str());

	CCompile tcomp;
	//ガード式のみのコンパイル
	if(fapost!=NULL) delete[] fapost;
	if(!astr.empty()){
		fapost=new char[astr.length()+1];
		try{
			tcomp.Kaiseki(astr.c_str(), fapost, incount);
		}
		catch(char *err){
//			TRACE("%s\n",err);
			delete[] fapost;
			fapost = NULL;
			throw err;//まる投げ
		}
	}
	else fapost=NULL;

//	if(fapost) TRACE("fapost  : \n%s\n",fapost);


	//トランジション関数のコンパイル
	if(fspost!=NULL) delete[] fspost;
	if(!fstr.empty()){
		fspost=new char[fstr.length()+1];
		try{
			tcomp.Kaiseki(fstr.c_str(), fspost, incount, astr.empty()?0:1);
		}
		catch(char *err){
//			TRACE("%s\n",err);
			delete[] fspost;
			fspost = NULL;
			throw err;//まる投げ
		}
	}
	else fspost=NULL;

//	TRACE("p %d\n",InPlaceList[1]);
//	if(fspost) TRACE("fspost  : \n%s\n",fspost);

}

//スペースやコメントを省く
int bspTransition::cutsp_str(const string &in_str, string &out_str)
{

//	TRACE("len in %d\n",in_str.length());
//	TRACE("%s/\n",in_str.c_str());
//	TRACE("len out %d\n",out_str.length());
	out_str.erase();
	if(in_str.empty()){
		return 0;
	}
	out_str=in_str;
	int i,j,o,v,now_var,io_d;
	char c,oc;
	v=1;
	now_var=0;
	io_d=1;//in:0  out:1
	oc='\0';
	i=0;
	o=0;
	while((c=in_str[i])!='\0'){
	//	TRACE("%c",c);
		if(c==' ' || c=='\t' || c=='\r' || c=='\n') v=0;
		else if(c=='/'){
			if((j=in_str.find('\n',i))==in_str.npos) break;
			i=j;
			v=0;
		}
		else if(c=='&' || c=='|' || c=='^' || c=='(' || c==')' || c=='~' || c=='?' || c=='='){
			if(c=='?' || c=='=') io_d=0;
			v=0;
			now_var=0;
			out_str[o++]=c;
			oc=c;
		}
		else if(c==';'){
			if(oc=='&' || oc=='|' || oc=='^' || oc=='(' || oc=='~' || oc=='?' || oc=='='){
				out_str.erase();
				throw "正規化：入力変数がありません．";
				return -1;
			}
			if(io_d==1){
				throw "正規化：式を記入してください．";
				return -1;
			}
			out_str[o++]='\n';
			v=0;
			io_d=1;
			now_var=0;
			oc=c;
		}
		else{
			if(v==0 && now_var==0){ now_var=1; v=1; }
			else if(v==0 && now_var==1){
				out_str.erase();
				if(io_d==0) throw "正規化：ｾﾐｺﾛﾝを付けてください．";
				else throw "正規化：演算子がありません．";
				return -1;
			}
	//		TRACE("%c\n",c);
			out_str[o++]=c;
			oc=c;
		}
		++i;
	}
	if(o>0 && out_str[o-1]!='\n'){
		out_str.erase();
		throw "正規化：最後の式にｾﾐｺﾛﾝを付けてください．";
		return -1;
	}
	out_str[o]='\0';
	out_str.resize(o);
	return 0;
}

//入出力変数の抽出
int bspTransition::var_str(const string &str,
			 map<string,int> &in_var,
			 map<string,int> &out_var)
{
	in_var.clear();
	out_var.clear();
	if(str.empty()){
		return 0;
	}

	int io_d,afact;
	int i,o;
	i=0;
	o=0;
	afact=0;
	//入力:0
	//出力:1
	io_d=1;

	string op_chars,ng_chars;
	op_chars="\n&|^()~";
	ng_chars="!#%-@*+/><[]{}:?.\"'$\\`,";
	string bufstr;
	char c;
	int st,iv_n,ov_n,v;
	iv_n=1;
	ov_n=1;
	v=1;
	if(str[0]=='?'){
		afact=1;
		++i;
		io_d=0;
		v=0;
	}
	st=i;
	while((c=str[i])!='\0'){
		if(c=='='){
			if(v!=1){
				if(afact) throw "ｶﾞｰﾄﾞ式がおかしい．";
				else throw "出力変数がおかしい．";
				return -3;
			}
			if(io_d!=1){
				if(afact) throw "ｶﾞｰﾄﾞ式にｾﾐｺﾛﾝを付けてください．";
				else throw "ｾﾐｺﾛﾝを付けてください．";
				return -3;
			}
			bufstr=str.substr(st,i-st);
			if(out_var.find(bufstr)==out_var.end()){
				out_var[bufstr] = ov_n;
				++ov_n;
			}
			else{
				throw "出力変数が重複しています．";//,bufstr.c_str());
				return -1; //err
			}
			if(in_var.find(bufstr)!=in_var.end()){
				throw "ﾌｨｰﾄﾞﾊﾞｯｸ入力は使用できません．";//,bufstr.c_str());
				return -2;//err
			}
			v=0;
			io_d=0;
		}
		else if(c=='\n' || c=='&' || c=='|' || c=='^' || c=='(' || c==')' || c=='~'){
			if(v==1){
				bufstr=str.substr(st,i-st);
				if(in_var.find(bufstr)==in_var.end()){
					in_var[bufstr] = iv_n;
					++iv_n;
				}
				if(out_var.find(bufstr)!=out_var.end()){
					throw "ﾌｨｰﾄﾞﾊﾞｯｸ入力は使用できません．";//,bufstr.c_str());
					return -2;//err
				}
				v=0;
			}
			if(c=='\n'){
				afact=0;
				io_d=1;
			}
		}
		else if(ng_chars.find(c)!=ng_chars.npos){
			throw "!#%-@*+/><[]{}:?.\"'$\\`, は使用できません．";
		}
		else if(v==0){
			st=i;
			v=1;
		}
		++i;
	}
	if(v!=0){
		if(afact) throw "ｶﾞｰﾄﾞ式がおかしい．";
		else throw "最後の式にｾﾐｺﾛﾝを付けてください．";
		return -3;
	}
	if(io_d!=1){
		if(afact) throw "ｶﾞｰﾄﾞ式にｾﾐｺﾛﾝを付けてください．";
		else throw "ｾﾐｺﾛﾝを付けてください．";
		return -3;
	}

	// 数字から始まる変数は使用できない
	map<string,int>::iterator vit;
	try{
		vit = in_var.begin();
		for(;vit!=in_var.end();++vit){
			if( isdigit( (vit->first)[0] ) ){
				throw vit->first;
			}
		}
		vit = out_var.begin();
		for(;vit!=out_var.end();++vit){
			if( isdigit( (vit->first)[0] ) ){
				throw vit->first;
			}
		}
	}
	catch(string errv){
		char thbuf[100];
		sprintf(thbuf,"%s : 変数の1文字目は英字にしてください．\n",errv.c_str());
		throw thbuf;
	}

	return 0;
}

//入出力変数の置き換え
void varreplace_str(string &str,
			 map<string,int> &in_var,
			 map<string,int> &out_var)
{
	if(str.empty()){
		return;
	}
	int io_d;
	int i,o;
	i=0;
	o=0;
	//入力:0
	//出力:1
	io_d=1;
	string bufstr;
	char c,*bufc;
	bufc=new char[5];
	int st,v;
	v=1;
	if(str[0]=='?'){
		++i;
		io_d=0;
		v=0;
	}
	st=i;
	while((c=str[i])!='\0'){
		if(c=='='){
			bufstr=str.substr(st,i-st);
			sprintf(bufc,"o%d",out_var[bufstr]);
			str.replace(st,i-st,bufc);
			i=st+strlen(bufc);
			v=0;
			io_d=0;
		}
		else if(c=='\n' || c=='&' || c=='|' || c=='^' || c=='(' || c==')' || c=='~'){
			if(v==1){
				bufstr=str.substr(st,i-st);
			//	TRACE("%d,",in_var[bufstr]);
				sprintf(bufc,"i%d",in_var[bufstr]);
				str.replace(st,i-st,bufc);
				i=st+strlen(bufc);
				v=0;
			}
			if(c=='\n') io_d=1;
		}
		else if(v==0){
			st=i;
			v=1;
		}
		++i;
	}

	delete[] bufc;

}


bool bspTransition::Fireablecheck()
{

	if(fired_flag==1){
		fired_flag=0;
		return 0;
	}

	if(FireableFlag) return true;

	fired_flag=0;
	FireableFlag=0;

	int col,i;
	//絶対発火条件のチェック
	col=AnyFireableCheck();
	if(FireableFlag==0) return false;


	FireableFlag=0;

//TRACE("out same\n");

	if(pass_tra){
		FireableFlag = true;
		return true;
	}
	//BSP独自のチェック
//TRACE("get value\n");
	//黒トークンのタイミングで入力値を取っておく
	//白トークンもこの値が使われる
//ガード式がある場合は値をとる
	int z_flag=0;
//	if(fapost!=NULL || col == _BTOKEN){
		for(i=0;i<incount;++i){
			inval[i] = InPlaces[i]->FastGetTokenValue();

			if(inval[i]==2){
				z_flag=1;
				throw "関数への入力値が不定です．";
			}
			
		//	TRACE("inval %d\n",inval[i]);
		}
//	}

	//TRACE("able calc\n");
	//ガード式の計算
	if(fapost!=NULL /*&& col==_BTOKEN*/){
		if(z_flag==1){
			throw "ｶﾞｰﾄﾞ式への入力値が不定です．";
		}
		
		//		TRACE("fa check\n");
		CCompile tcomp;
		int outv[1];
		tcomp.Keisan(fapost, inval, outv, incount, 1);
		if(outv[0] == 0) return false;
	}

	//発火条件が整った
	FireableFlag = true;
	return true;
}

int bspTransition::AnyFireableCheck()
{

	int i;
	//出力プレースがからとか
	for(i=0;i<out_place_num;++i){
		if(OutPlaces[i]->FastGetTokenFlag())
			return 0;//出力プレースにトークンがある
	}

	int col;
//TRACE("token color\n");
	
//	TRACE("%d\n",InPlaceList);

	if(InPlaces[0]->FastGetTokenFlag()){
		col = InPlaces[0]->FastGetTokenColor();
#ifdef _DEBUG
		if(col==0) throw "フラグが存在し、トークンがない：内部エラー";
#endif
	} else return false;//入力プレースにトークンがない
	for(i=1;i<in_place_num;++i){
		if(InPlaces[i]->FastGetTokenFlag()){
			if(InPlaces[i]->FastGetTokenColor() != col){
				return false;
//				if(Qmode) throw "トークンの色が違うよ，設計やり直したら？";
//				throw "ハングアップしました．";
			}
		} else return false;//入力プレースにトークンがない
	}

	//出力プレースを前回通過した色と同じ色のトークンか？
	for(i=0;i<out_place_num;++i){
		if(OutPlaces[i]->FastGetOldColor() == col)
			return false;//同じ色なので、発火できない
	}

	FireableFlag=1;

	return col;
}

bool bspTransition::Fireable()
{
	return FireableFlag;
}

BOOL bspTransition::Fire()
{

	if(FireableFlag==false) return 0;

	// ランダムｼﾐｭﾚｰｼｮﾝ
//	if(rand()%10<(rand()>>8)%10) return 1;

	fired_flag=0;
	FireableFlag=0;

	int col,i;

	//競合合流のための絶対発火条件のチェック
	col=AnyFireableCheck();
	if(FireableFlag==0) return 0;

	col=InPlaces[0]->FastGetTokenColor();

	if(pass_tra){
		OutPlaces[0]->FastSetTokenValue( InPlaces[0]->FastGetTokenValue() );
	}
	else{
/*
		// メモリトランジションの場合
		if(!tname.empty()){
			tmem->GetValue(  inval, outval, incount, outcount);
		}
		else{
*/
		//黒トークンの場合に関数計算を行う
	//	if(col == _BTOKEN){
			CCompile tcomp;
			tcomp.Keisan(fspost, inval, outval, incount, outcount);
	//	}

	//}

	//TRACE("fired\n");
		for(i=0; i<outcount; ++i){
			OutPlaces[i]->FastSetTokenValue(outval[i]);
	//		TRACE("val %d\n",outval[i]);
		}
		for(; i<out_place_num; ++i){
			OutPlaces[i]->FastSetTokenValue(2);//不定値
		//	TRACE("val %d\n",outval[i]);
		}
	}

	AnyFire(col);

	return 1;
}

//とにかく発火、トークンの値はプレースに依存する
void bspTransition::AnyFire(int col)
{
	int i;
	for(i=0; i<out_place_num;i++){
		OutPlaces[i]->FastSetTokenColor( col );
		OutPlaces[i]->FastSetTokenFlag(true);
	}
	
	//入力プレースのトークンを消費
	for(i=0;i<in_place_num;i++){
		InPlaces[i]->FastRemoveToken();
	}

	FireableFlag = false;
	fired_flag=1;
}


// define構文を展開する
// `define AAA bbb
//		`AAA
void replace_define(const string &in_str, string &dd_str)
{
	dd_str.erase();

	map<string,string> def_map;
	def_map.clear();

	int st=0,d=0;
	for(;;){

		d = in_str.find("`define",st);
		if(d==in_str.npos){
			dd_str += in_str.substr(st);
			break;
		}

		dd_str += in_str.substr(st,d-st);

		d+=7;

		d = in_str.find_first_not_of(" \t", d);
		int from_st;
		from_st = d;	// DEFの先頭

		d = in_str.find_first_of(" \t",d);
		int from_end;
		from_end = d;

		d = in_str.find_first_not_of(" \t", d);
		int to_st;
		to_st = d;

		d = in_str.find_first_of(" \t\n",d);
		int to_end;
		to_end = d;

		def_map[ "`" + in_str.substr(from_st,from_end-from_st) ] = in_str.substr(to_st, to_end-to_st);
		st = d;
	}

//	TRACE("def %s\n",dd_str.c_str());

	map<string,string>::iterator dit;
	dit = def_map.begin();
	for(;dit!=def_map.end();++dit){
//		TRACE("[%s]->[%s]\n",dit->first.c_str(),dit->second.c_str());
		int d;
		d = 0;
		for(;;){
			d = dd_str.find( dit->first, d);
			if(d==dd_str.npos){
				break;
			}
			dd_str.replace(d,dit->first.length(),dit->second);
		}
	}

//	TRACE("dd %s\n",dd_str.c_str());

}

void bspTransition::FuncEduction()
{

	if(fstring == DEFAULT_TRAFUNC){
		pass_tra = 1;
	}
	else pass_tra = 0;

	string fs_defined;
	// define の置き換え
	replace_define(fstring, fs_defined);

	//スペースやコメントを省く
	cutsp_str(fs_defined, fs_spcut);

//	TRACE("f%s\n",fs_spcut.c_str());

	//入出力変数の抽出
	var_str(fs_spcut, in_var, out_var);

	incount = in_var.size();
	outcount = out_var.size();

}

bspTransition::~bspTransition()
{
//	TRACE("death\n");
	if(fspost != NULL) delete[] fspost;
	if(fapost != NULL) delete[] fapost;
	if(inval != NULL) delete[] inval;
	if(outval != NULL) delete[] outval;

}

//////////////////   Uhzi ///////////////////////////


bspUhzi::bspUhzi(int dadd,int line)
{
	ptr = 0;
	oldColor = _WTOKEN;
	dmax = 0;
	addp = dadd;
	InputData = NULL;
	In = 0;//デフォルト値
	dline=line;
	lptr=NULL;
	endflag=false;
	bits = 1;//2進
	fired = 0;

}

bspUhzi::bspUhzi(const bspUhzi &uhzi)
{
	InputData = NULL;
//	TRACE("copy\n");
	*this = uhzi;
}

bspUhzi & bspUhzi::operator = (const bspUhzi &uhzi)
{
//	TRACE("=\n");
	if(this!=&uhzi){

	//	TRACE("uhzi copy %d\n",rand());

		oldColor=_WTOKEN;
		
		//マーキング情報
		addp=uhzi.addp;//１：ビット列　０：固定ビット
		Pp = uhzi.Pp;

		InputString = uhzi.InputString;

		dline=uhzi.dline;//流す方向　0：上位（左）1：下位から（右）　
		In=uhzi.In;//固定ビットの時に保持する値

		//内部で使用する
		if(InputData) delete[] InputData;
		InputData=NULL;
		dmax=uhzi.dmax;
		ptr=0;
		lptr = InputString.begin();

		endflag=false;//ウージのデータが全て終わったら１
		bits = uhzi.bits;

		input_name = uhzi.input_name;

		fired = 0;


	}
	return *this;
}

void bspUhzi::Compile()
{

	Reset();

	Pp=0;

	//設定された値のチェックを行う
	if(addp==1){
		int i,j;
		char c;
		CString s;
		list<string>::iterator p;
		p = lptr;
		for(j=1; p!=InputString.end(); ++p, ++j){
		//	TRACE("%s\n", InputString->GetAt(p));
			for(i=0; i<p->length(); ++i){
				c = (*p)[i];
				if(isspace(c)) continue;
				if(bits==4){
					if( isxdigit( c ) ) continue;
					throw "入力ﾃﾞｰﾀは16進数で入力してください．";
				}
				if(c=='0' || c=='1') continue;
				throw "入力ﾃﾞｰﾀは2進数で入力してください．";
			}
		}

	}
#ifdef _DEBUG
	else if(addp==0){
		if(In!=1 && In!=0) throw "ウージにセットされた値が不正：内部エラー";
		//TRACE("In %d\n", In);
	}
	else throw "ウージのaddpがおかしい：内部エラー";
#endif

}

//設定されているデータを準備する
void bspUhzi::Set()
{

	if(Pp==0) return;

	if(addp==1){

		if(lptr==InputString.end()){
			ptr=0;
			dmax=0;
			endflag = true;
			return;
		}
		ptr = 0;
		dmax = 0;

		int i;
		for(i=0;i<lptr->length();++i){
			if( isspace((*lptr)[i]) ) continue;
			dmax++;
		}
		
		char c[2];
		c[1]='\0';
		int ci;
		string instr;
		instr.erase();
		if(bits==1)	instr = (*lptr);
		else if(bits==4){
			for(i=0;i<lptr->length();++i){
				c[0]=(*lptr)[i];
				if( isspace(c[0]) ) continue;
				sscanf(c, "%1x", &ci);
				instr += ((ci & 0x08)>>3) + '0';
				instr += ((ci & 0x04)>>2) + '0';
				instr += ((ci & 0x02)>>1) + '0';
				instr += ((ci & 0x01)) + '0';
			}
			dmax <<= 2;
		}
//		TRACE("%s\n",instr.c_str());

		if(InputData!=NULL) delete[] InputData;	//長さが違うかも知れないので一度破棄
		InputData = new int[dmax];

		if(dline==0){//上位ビットから流す
			for(i=0; i<dmax; ++i){
				InputData[i] = instr[i] - '0';
			}
		}
		else{//下位ビットから流す
			for(i=dmax-1; i>=0; --i){
				InputData[i] = instr[dmax-i-1] - '0';
			}
		}
		++lptr;
	}
	else if(addp==0){
	}
}

void bspUhzi::Reset()
{
	if(InputData!=NULL) delete[] InputData;
	InputData = NULL;

	dmax = 0;
	ptr = 0;
	oldColor = _WTOKEN;
	endflag = false;
	fired =0;

	if(addp==1){
		lptr = InputString.begin();
	}
	else if(addp==0){
		dmax = 1;
	}

}

// 0:発火できない
BOOL bspUhzi::Check(map<int,bspPlace*> &PlaceMap)
{
	if(Pp==0) return 0;

	if(PlaceMap[Pp]->GetTokenFlag()) return 0;

	if(endflag==true) return 0;

	if(addp==0) return 1;

	return 1;
}

//トークンを生産
BOOL bspUhzi::Industry(map<int,bspPlace*> &PlaceMap)
{
	fired=0;

	if(endflag) return 0;
	if(Pp==0) return 0;

	int ret = 0;
	int val;
	bspPlace *ppa;
	ppa = PlaceMap[Pp]->GetRealPlace();

	//初回
	if(addp==1 && ptr>=dmax){
		Set();
	}

	if(ppa->GetTokenFlag()==FALSE && ppa->GetOldColor()==oldColor){
		if(ptr<dmax){
			if(addp==0) val = In;
			else val = InputData[ptr];

			if(oldColor==_WTOKEN){
				oldColor = _BTOKEN;
			}
			else if(oldColor==_BTOKEN){
				oldColor = _WTOKEN;
				ptr += addp;
			}
#ifdef _DEBUG
			else throw "ウージに未定義の色が設定されました、内部エラー";
#endif
			ppa->SetToken(oldColor,val);
			//TRACE("industry col %d val %d\n",oldColor,val);

			ret = 1;
			fired=1;
		}

		if(addp==1 && ptr>=dmax){
			Set();
		}
	}

	return ret;
}

void bspUhzi::UndoIndustry()
{
	if(Pp==0) return;

	endflag=0;

	if(oldColor==_BTOKEN){
		oldColor=_WTOKEN;
	}
	else if(oldColor==_WTOKEN){
		oldColor=_BTOKEN;
		if(addp==0) return;
		if(ptr>0) ptr-=addp;
		else if(ptr==0){
			lptr--;
			if(dmax!=0) lptr--;
			Set();
			ptr=dmax-1;
		}
	}

}


void bspUhzi::ValuePrint(CDC *pDC,CPoint mp,float vdiam, int view_sel,int view_max)
{

	/*
		表示形式は機能別に大別すること
	*/

	string str;
	int i,j;

//	TRACE("uuu\n");
	if(addp==0){
		str.resize(4);
		str[0] = '<';
		str[1] = In + '0';
		str[2] = '>';
	}
	else if(InputString.empty()) return;
	else{
		if(view_sel){
			string &s = *(InputString.begin());
			int len = s.length();
			if(dline==0){	// 上位ビットから流す
				if(len>view_max){
					str.resize(view_max+4);
					for(j=0;j<view_max;j++){
						str[j] = s[j];
					}
					for(;j<view_max+3;j++) str[j]='.';
				}
				else{
					str = s;
				}
			}
			else{	// 下位ビットから流す
				if(len>view_max){
					str.resize(view_max+4);
					for(j=0;j<3;j++) str[j]='.';
					for(i=len-view_max;i<len;j++,i++){
						str[j] = s[i];
					}
				}
				else{
					str = s;
				}
			}
		}
		else{//PetMode==_SIMULATION
			if(Pp==0){
				return;
			}
			else if(ptr==dmax){
				if(endflag) str="End";
				else str="Ready";
			}
			else if(dline==0){	// 上位ビットから流す
				if(dmax-ptr>view_max){
					str.resize(view_max+4);
					for(j=0,i=ptr;j<view_max;j++,i++){
						str[j] = InputData[i] + '0';
					}
					for(;j<view_max+3;j++) str[j]='.';
				}
				else{
					str.resize(dmax-ptr+1);
					for(j=0,i=ptr;i<dmax;j++,i++){
						str[j] = InputData[i] + '0';
					}
				}
			}
			else{	// 下位ビットから流す
				if(dmax-ptr>view_max){
					str.resize(view_max+4);
					for(j=0;j<3;j++) str[j]='.';
					for(i=ptr+view_max-1;j<view_max+3;j++,i--){
						str[j] = InputData[i] + '0';
					}
				}
				else{
					str.resize(dmax-ptr+1);
					for(j=0,i=dmax-1;i>=ptr;j++,i--){
						str[j] = InputData[i] + '0';
					}
				}
			}

		}
	}

	if(!str.empty()){
		pDC->TextOut(mp.x-20*vdiam, mp.y-20*vdiam, str.c_str());
	}

}

bspUhzi::~bspUhzi()
{
	if(InputData) delete[] InputData;
}

//////////////////////		Kemmakix	//////////////////


bspKemmakix::bspKemmakix(int line)
{
	oldColor = _WTOKEN;

	ptr = 0;
	dline = line;
	compString = NULL;
	dmax = 0;

	bits=16;
	hex = 0;
	fired = 0;

}

bspKemmakix::bspKemmakix(const bspKemmakix &kem)
{
	compString = NULL;
//	TRACE("copy\n");
	*this = kem;
}

bspKemmakix & bspKemmakix::operator = (const bspKemmakix &kem)
{
//	TRACE("=\n");
	if(this!=&kem){

	//	TRACE("kem copy %d\n",rand());

		oldColor=_WTOKEN;

		//マーキング情報
		dline=kem.dline;//受け取る方向　0：上位から（左）1：下位（右）　２：破棄
		Pp = kem.Pp;
		
		if(compString) delete compString;
		if(kem.compString != NULL){
			compString = new list<string>;
			*compString = *kem.compString;
		}
		else compString=NULL;

		OutputData = kem.OutputData;

		ptr=0;
		dmax = kem.dmax;

		bits = kem.bits;
		hex = kem.hex;

		output_name = kem.output_name;

		fired = 0;


	}
	return *this;
}

void bspKemmakix::Compile()
{

//	TRACE("kmax %d\n",datamax);
	Reset();
	Pp=0;

}

void bspKemmakix::Reset()
{

	fired=0;
	ptr = 0;
	dmax =0;

	oldColor = _WTOKEN;

	if(dline==3){
	}
	else{
		OutputData.clear();
		dmax = KEM_DATA_MAX;
		OutputData.resize( KEM_DATA_MAX );
	}

}

BOOL bspKemmakix::Check(map<int,bspPlace*> &PlaceMap)
{
	fired=0;
	if(Pp==0) return 0;
	if(dline==3) return 0;	//消費しないモード

	if(PlaceMap[Pp]->GetTokenFlag()){
		fired=1;
	}
	return fired;
}

BOOL bspKemmakix::Consum(map<int,bspPlace*> &PlaceMap,int bin_value)
{
	fired=0;
	if(Pp==0) return 0;
	if(dline==3) return 0;	//消費しないモード

	if(PlaceMap[Pp]->GetTokenFlag()){
		if(PlaceMap[Pp]->GetTokenColor()==_BTOKEN
			&& oldColor==_WTOKEN){//黒トークンを消費
		
			if(dline!=2 || bin_value){
				OutputData[ptr++] = PlaceMap[Pp]->GetTokenValue();
				if(ptr>=dmax){
					dmax += KEM_DATA_MAX;
					OutputData.resize( dmax );
				}
			}
		}
		else if(PlaceMap[Pp]->GetTokenColor()==_WTOKEN 
			&& oldColor==_BTOKEN){//白トークンを消費
			//何もしない
		}

		oldColor = PlaceMap[Pp]->GetTokenColor();

		fired=1;
		PlaceMap[Pp]->RemoveToken();
	}
	return fired;
}

void bspKemmakix::UndoConsum()
{
	if(Pp==0) return;
	if(dline==3) return;	//消費しないモード
	
	//TRACE("consum\n");
	if(oldColor==_BTOKEN){//黒トークンを消費していた
		if(ptr>0){
			ptr--;
		}
		oldColor=_WTOKEN;
	}
	else if(oldColor==_WTOKEN){//白トークンを消費していた
		oldColor=_BTOKEN;
	}


}

//長すぎたら省略しろ
void bspKemmakix::ValuePrint(CDC *m1DC,CPoint mp,float vdiam,int PetMode, int view_max)
{
	if(Pp==0) return;
	if(dline==2) return;	//破棄モード
	if(dline==3) return;	//消費しないモード

//	TRACE("%d\n",datamax);


	if(PetMode==_SIMULATION){
		string str;
		int i,j;
		char c[2];
		c[1]='\0';
		if(ptr==0){
		//	s = OutputString->back();
		}
		else{
			if(dline==0){//上位ビットから受け取る場合
				if(ptr>view_max){
					str.resize(view_max+4);
					for(j=0;j<3;j++) str[j]='.';
					for(i=ptr-view_max; i<ptr; i++,j++){
						str[j] = OutputData[i] + '0';
					}
				}
				else{
					str.resize(ptr+1);
					for(i=0; i<ptr; i++){
						str[i] = OutputData[i] + '0';
					}
				}
			}
			else if(dline==1){//dline==1 下位ビットから受け取る場合
				if(ptr>view_max){
					str.resize(view_max+4);
					for(i=ptr-1,j=0; j<view_max; i--,j++){
						str[j] = OutputData[i] + '0';
					}
					for(;j<view_max+3;j++) str[j]='.';
				}
				else{
					str.resize(ptr+1);
					for(i=ptr-1,j=0; i>=0; i--,j++){
						str[j] = OutputData[i] + '0';
					}
				}
			}

			m1DC->TextOut(mp.x-20*vdiam, mp.y-20*vdiam, str.c_str());
		}

	}
}


bspKemmakix::~bspKemmakix()
{
	if(compString != NULL) delete compString;
}
//////////////////	Token	//////////////////////

bspToken::bspToken(int color,int value)
{
	Color=color;
	Value=value;
}

bspToken::bspToken(const bspToken &token)
{
//	TRACE("copy\n");
	*this = token;
}

bspToken & bspToken::operator = (const bspToken &token)
{
//	TRACE("=\n");
	if(this!=&token){

	//	TRACE("copy token %d\n",rand());

		Color = token.Color;
		Value = token.Value;

	}
	return *this;
}

bspToken::~bspToken()
{
}

//////////////////		Arc		/////////////////////////

bspArc::bspArc(int in,int out,string v_name)
{
	In = in;
	Out = out;
	bwtpoint = NULL;
	value_name=v_name;
}

bspArc::bspArc(const bspArc &arc)
{
//	TRACE("copy\n");
	*this = arc;
}

bspArc & bspArc::operator = (const bspArc &arc)
{
//	TRACE("=\n");
	if(this!=&arc){

	//	TRACE("copy arc %d\n",rand());

		if(arc.bwtpoint!=NULL){
			bwtpoint = new vector<CPoint>( arc.bwtpoint->size() );
			for(int i=0;i<arc.bwtpoint->size();++i){
				(*bwtpoint)[i] = (*arc.bwtpoint)[i];
			}
		}
		else bwtpoint=NULL;

		In = arc.In;//vObjListの位置
		Out = arc.Out;//vObjListの位置
		value_name = arc.value_name;
	}
	return *this;
}

void bspArc::SetBwt(vector<CPoint> *bwt)
{
	if(bwtpoint!=NULL) delete bwtpoint;
	if(bwt==NULL || bwt->size()==0){
		bwtpoint = NULL;
	}
	else{
		bwtpoint = new vector<CPoint>(*bwt);
	}
}

bspArc::~bspArc()
{
//	TRACE("arc delete\n");
	if(bwtpoint!=NULL) delete bwtpoint;
}

////////////////////////	Object	//////////////////////////////

bspObject::bspObject(int obj_type, CPoint p)
{
	gpoint = p;
	type = obj_type;
}

bspObject::bspObject(const bspObject &object)
{
//	TRACE("copy\n");
	*this = object;
}

bspObject & bspObject::operator = (const bspObject &object)
{
//	TRACE("=\n");
	if(this!=&object){
	//	TRACE("copy object %d\n",rand());
		gpoint = object.gpoint;
		type = object.type;
	}
	return *this;
}

bspObject::~bspObject()
{
}



/// EOF ////
