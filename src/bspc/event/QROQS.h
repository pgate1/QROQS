/******************************************
	QROQS over to C Language !!
	Writen by pgate1
	Last Update : 17:13 2003/11/23
******************************************/
#ifndef BitSerialPetriNetC_H_INCLUDED
#define BitSerialPetriNetC_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#define _BTOKEN_OFF		0x10
#define _WTOKEN_OFF		0x00
#define _BTOKEN_EXIST	0x11
#define _WTOKEN_EXIST	0x01

typedef struct place {
	char token_state;
	char token_value;
} Place;

typedef struct transition {
	void (*guard)(char *in, char *out);
	void (*func)(char *in, char *out);
	unsigned char feedback_size;
	char *feedback;
} Tran;

typedef struct uhzi {
	int ptr, dmax;
	char *data;
} Uhzi;

typedef struct kemmakix {
	int ptr, dmax;
	char *data;
} Kem;

typedef struct object {
	char type;
	union {
		Tran *tran;
		Uhzi *uhzi;
		Kem  *kem;
	} prim;
	unsigned short in_place_size, out_place_size, joint_size;
	Place **in_place, **out_place;
	struct object **joints;
	char state;
	char event;
} Object;

enum {
	_NONE,
	_PLACE,
	_TRAN,
	_UHZI,
	_KEM
};

typedef struct bspetri {
	int place_size, tran_size, uhzi_size, kem_size, obj_size;
	Place *place;
	Tran *tran;
	Uhzi *uhzi;
	Kem *kem;
	Object *obj;
	char *init_token, *init_value, *init_fire, *feedback_init;
	int in_max, out_max;
} BSPetri;

BSPetri *bsp_init(
	int place_size,
	int tran_size,
	int uhzi_size,
	int kem_size,
	
	char init_token[],
	char init_value[],
	char init_fire[],
	unsigned char in_place_size[],
	unsigned char out_place_size[],
	void (*trafuncs[])(char*,char*),
	short func[],
	short guard[],
	unsigned char feedback_size[],
	char feedback_init[],
	int in_place[],
	int out_place[],
	short joint_size[],
	int joints[]
	);
int bsp_io(BSPetri *bsp, int u_dmax[], int k_dmax[], char **input_data, char **output_data);
int bsp_io_data(BSPetri *bsp, int u_dmax[], int k_dmax[], char *in[]);
void bsp_in_print( BSPetri *bsp );
void bsp_bsp_in_print(BSPetri *bsp, char *input_name[]);
int bsp_reset(BSPetri *bsp);
int bsp_run(BSPetri *bsp, int loop_max );
void bsp_out_print( BSPetri *bsp );
void bsp_bsp_out_print(BSPetri *bsp, char *output_name[], unsigned char dline[], int bits[], unsigned char hex[]);
void bsp_io_exit(BSPetri *bsp);
void bsp_exit(BSPetri *bsp);

#endif

// EOF //
