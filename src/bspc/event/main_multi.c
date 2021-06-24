#include <time.h>

#include "bsp_untitle.h"

int main(void)
{

	int i,j;
	int **indata, **outdata;
	int u_dmax[]={8,8,8};
	int k_dmax[]={8,8};
	int ret;

	BSPetri *bsp_un;
	time_t st;

	bsp_un = bsp_untitle_init();
	if(!bsp_un){
		printf("init error !!\n");
		return 1;
	}


	srand(time(NULL));

	indata = (int **)malloc(sizeof(int *)*(3+1));
	for(i=0;i<3;i++){
		indata[i] = (int *)malloc(sizeof(int)*8);
		for(j=0;j<8;j++){
			indata[i][j] = rand();
		}
	}
	indata[3] = 0;

	indata[0][0] &= 0xffffff7f;
	indata[0][1] |= 0x00000080;
	indata[0][2] &= 0xffffff7f;
	indata[0][3] |= 0x00000080;
	indata[0][4] &= 0xffffff7f;
	indata[0][5] |= 0x00000080;
	indata[0][6] &= 0xffffff7f;
	indata[0][7] |= 0x00000080;

	indata[1][0] &= 0xffffff7f;
	indata[1][1] &= 0xffffff7f;
	indata[1][2] |= 0x00000080;
	indata[1][3] |= 0x00000080;
	indata[1][4] &= 0xffffff7f;
	indata[1][5] &= 0xffffff7f;
	indata[1][6] |= 0x00000080;
	indata[1][7] |= 0x00000080;

	indata[2][0] &= 0xffffff7f;
	indata[2][1] &= 0xffffff7f;
	indata[2][2] &= 0xffffff7f;
	indata[2][3] &= 0xffffff7f;
	indata[2][4] |= 0x00000080;
	indata[2][5] |= 0x00000080;
	indata[2][6] |= 0x00000080;
	indata[2][7] |= 0x00000080;


	for(i=0;i<3;i++){
		printf("in%d : ",i);
		for(j=0;j<8;j++) printf("%d",(indata[i][j]&0x00000080)?1:0);
		printf("\n");
	}


	outdata = (int **)malloc(sizeof(int *)*2);
	for(i=0;i<2;i++){
		outdata[i] = (int *)malloc(sizeof(int)*8);
	}

	ret = bsp_io( bsp_un, u_dmax, k_dmax, indata, outdata);
	if(ret==0){
		printf("io_data error\n");
		return 1;
	}

	bsp_reset( bsp_un );

	st = time(NULL);
	for(i=0;i<100;i++){
		bsp_run( bsp_un, 0 );	// no Reset
	}
	printf("time %d\n",time(NULL)-st);
	
	for(i=0;i<2;i++){
		printf("out%d : ",i);
		for(j=0;j<8;j++) printf("%d",(outdata[i][j]&0x00000080)?1:0);
		printf("\n");
	}

	for(i=0;i<3;i++) free(indata[i]);
	free(indata);
	for(i=0;i<2;i++) free(outdata[i]);
	free(outdata);

	bsp_exit( bsp_un );
	
	return 0;
}
