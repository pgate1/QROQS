//
// created by QROQS
//	model    : delay add script
//	function : create delay.vnet by delay.data & trafunc.rep
//	update   : 17:38 2004/08/05
//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//#define BUF_DELAY 100	//BUF1
//#define BUF_DELAY 80	//0.6INV1*2
#define BUF_DELAY 80	//0.35INV1*2

double get_rep_delay(FILE *fp, const char *module_name, const char *func_name, int func_n)
{
	double rep_delay=-1;
	char rdata[256];
	char buf[100];

	while(fgets(rdata,255,fp)!=NULL){
		if(!strncmp(rdata,"Report : timing",15)) break;
	}

	sprintf(buf,"Design : %s_%s_%d",module_name,func_name,func_n);
	while(fgets(rdata,255,fp)!=NULL){
		if(!strncmp(rdata,buf,strlen(buf))) break;
	}

	while(fgets(rdata,255,fp)!=NULL){
		if(strncmp(rdata,"  data arrival time",19)==0){
			sscanf(rdata,"  data arrival time %lf\n",&rep_delay);
			break;
		}
		else if(strncmp(rdata,"No paths.",9)==0){
			rep_delay=0.0;
			break;
		}
	}

	return rep_delay;
}

int delay_to_buf(int delay)
{
	int n,m;
	n=delay/BUF_DELAY;
	m=delay-n*BUF_DELAY;
	if(m>0) n++;
	return n;
}

void write_vnet(FILE *fp, const char *module_name, const char *delay_name,
				int tra_id, int delay_buf_n ,int total_delay_up)
{

	int i;

	fprintf(fp,"module %s_%s_%d(\n",module_name,delay_name,tra_id);
	fprintf(fp,"	req_in, req_out\n");
	fprintf(fp,");\n");

	fprintf(fp,"input	req_in;\n");
	fprintf(fp,"output	req_out;\n");

/*
//BUF1で遅延を付加する
	fprintf(fp,"	wire w1,w%d;\n",delay_buf_n+1);
	fprintf(fp,"	assign	w1 = req_in;\n");
	for(i=1;i<=delay_buf_n;i++){
		fprintf(fp,"	BUF1 buf%d(.A(w%d), .Y(w%d));\n",i,i,i+1);
	}
	fprintf(fp,"	assign	req_out = w%d;\n",delay_buf_n+1);
*/

//INV1で遅延を付加する
	if(delay_buf_n==0){
		fprintf(fp,"	assign req_out = req_in;\n");
	}
	else{
		fprintf(fp,"	wire w1,w%d;\n",delay_buf_n*2+1);
		fprintf(fp,"	assign	w1 = req_in;\n");
		for(i=1;i<=delay_buf_n*2;i++){
			fprintf(fp,"	INV1 buf%d(.A(w%d), .Y(w%d));\n",i,i,i+1);
		}
		fprintf(fp,"	assign	req_out = w%d;\n",delay_buf_n*2+1);
	}

//そのまま、テスト用
//	fprintf(fp,"	assign	#%d req_out=req_in;\n",total_delay_up);

	fprintf(fp,"endmodule\n");
}

int main( int argc, char *argv[] )
{
	char vdata[256];
	int func_n;
	double rep_delay,slack;
	int i;
	FILE *data_fp;
	char *module_name;
	int *delays;
	FILE *rep_fp;
	FILE *vnet_fp;
	int tra_id;
	int delay_up,delay_down,total_delay_up;
	int delay_buf_n;
	int max_buf, max_buf_id;

	printf("input %s %s -> output %s\n",argv[1],argv[2],argv[3]);

	data_fp=fopen(argv[1],"r");
	if(data_fp==NULL){
		printf("no delay.data !\n");
		return 3;
	}

	module_name = (char *)malloc(sizeof(char)*256);
	fgets(vdata,255,data_fp);
	sscanf(vdata,"%s\n",module_name);
	printf("module %s\n",module_name);
	fgets(vdata,255,data_fp);
	sscanf(vdata,"%d,%lf;\n",&func_n,&slack);

	delays = (int*)malloc(sizeof(int)*func_n);

	rep_fp=fopen(argv[2],"r");
	if(rep_fp){
		for(i=1;i<=func_n;i++){

			//レポートファイルから遅延時間を取得する
			rep_delay = get_rep_delay(rep_fp, module_name, argv[4], i);
			if(rep_delay==-1){
				printf("Don't get data arrival time from report file!\n");
				break;
			}
			printf("func %d  rep delay  %f ns\n",i ,rep_delay);

			delays[i-1] = (int)( rep_delay * 1000 );

		}
		fclose(rep_fp);
	}


	vnet_fp=fopen(argv[3],"w");

	max_buf = 0;

	while(fgets(vdata,255,data_fp)!=NULL){
		sscanf(vdata,"%d,%d,%d,%d;"
			,&tra_id,&delay_up,&delay_down,&func_n);

		if(func_n==0) total_delay_up=delay_up;
		else total_delay_up = delays[func_n-1] + delay_up;
		total_delay_up += (int)( total_delay_up * slack);
		printf("	delay_%d total delay	%d ps\n",tra_id,total_delay_up);
		//	rep_delay	ns
		//	func_delay	ps
		//見積もったディレイから、何個のBUFが必要か見積もる
		delay_buf_n = delay_to_buf( total_delay_up );
		printf("	BUF1 * %d\n",delay_buf_n);

		if(delay_buf_n > max_buf){
			max_buf = delay_buf_n;
			max_buf_id = tra_id;
		}

		//vdファイルに書き込む
		write_vnet(vnet_fp, module_name, argv[5], tra_id, delay_buf_n ,total_delay_up);
	}

	fclose(vnet_fp);

	fclose(data_fp);

	free( module_name);

	free( delays);

	printf("max buf %d   ID %d\n",max_buf,max_buf_id);

	return 0;

}


