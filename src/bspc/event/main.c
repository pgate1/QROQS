#include <time.h>

#include "bsp_untitle.h"

int main(void)
{



	BSPetri *bsp_un;

	bsp_un = bsp_untitle_init();
	if(!bsp_un){
		printf("init error !!\n");
		return 1;
	}

	bsp_untitle_io( bsp_un );
	bsp_untitle_in_print( bsp_un );
//	bsp_in_print( bsp_un );

	bsp_reset( bsp_un );

//	srand(time(NULL));
	{
	time_t st;
	int i;
	st = time(NULL);
	for(i=0;i<1000000;i++){
		bsp_run( bsp_un, 1000);	// loop max
	}
	printf("time %d\n",time(NULL)-st);
	}

	//bsp_out_print( bsp_un );
	bsp_untitle_out_print( bsp_un );

//	getchar();
	
	bsp_io_exit( bsp_un );
	bsp_exit( bsp_un );
	
	return 0;
}
