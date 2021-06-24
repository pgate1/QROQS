/******************************************
	QROQS over to C Language !!
	Created by QROQS
******************************************/
#include "bsp_untitle.h"

void trafunc_1(char *i,char *o)
{
	o[0] = i[0];
}
void trafunc_2(char *i,char *o)
{
	o[0] = ((i[0] ^ i[1]) ^ i[2]);
	o[1] = (i[2] & (i[0] | i[1])) | (i[0] & i[1]);
}

unsigned char bsp_global_in_place_size[]={1,1,3,1,0,0,0,1,1};
unsigned char bsp_global_out_place_size[]={1,1,2,1,1,1,1,0,0};
short bsp_global_func[]={1,1,2,1};
short bsp_global_guard[]={0,0,0,0};
unsigned char bsp_global_feedback_size[]={0,0,0,0};
unsigned char bsp_global_init_token[]={0,0,0,0,0,0,0,0};
unsigned char bsp_global_init_value[]={0,0,0,0,0,0,0,0};
unsigned char bsp_global_init_fire[]={0,0,0,0,0,0,0,0,0};
unsigned char bsp_global_feedback_init[]={0};
int bsp_global_in_place[]={0, 2, 1,3,4, 6, 5, 7};
int bsp_global_out_place[]={1, 3, 5,6, 7, 0, 2, 4};
short bsp_global_joint_size[]={2,2,5,2,1,1,1,1,1};
int bsp_global_joints[]={4,2, 5,2, 0,1,6,7,3, 2,8, 0, 1, 2, 2, 3};

BSPetri *bsp_untitle_init(void)
{	
	void (*trafuncs[])(char*,char*)={
		0,
		trafunc_1,
		trafunc_2,
		0
	};

	return bsp_init(
		8,
		4,
		3,
		2,

		bsp_global_init_token,
		bsp_global_init_value,
		bsp_global_init_fire,
		bsp_global_in_place_size,
		bsp_global_out_place_size,
		trafuncs,
		bsp_global_func,
		bsp_global_guard,
		bsp_global_feedback_size,
		bsp_global_feedback_init,
		bsp_global_in_place,
		bsp_global_out_place,
		bsp_global_joint_size,
		bsp_global_joints
		);
}

int bsp_untitle_io(BSPetri *bsp)
{
	int u_dmax[]={8,8,8};
	int k_dmax[]={2,8};
	char *in[]={
		"01010101",
		"00110011",
		"00001111",
		0 };

	return bsp_io_data(
		bsp,
		u_dmax,
		k_dmax,
		in
		);
}

void bsp_untitle_in_print( BSPetri *bsp )
{
	char *input_name[]={
		"in_a",
		"in_b",
		"c_in"};

	bsp_bsp_in_print(
		bsp,
		input_name
		);
}

void bsp_untitle_out_print(BSPetri *bsp)
{
	char *output_name[]={
		"sum_out",
		"c_out"};

	unsigned char dline[] = {1,0};
	int bits[] = {16,16};
	unsigned char hex[] = {0,0};

	bsp_bsp_out_print(
		bsp,
		output_name,
		dline,
		bits,
		hex
		);
}
