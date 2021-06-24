/******************************************
	QROQS over to C Language !!
	Created by QROQS
******************************************/
#ifndef BitSerialPetriNetC_untitle_H_INCLUDED
#define BitSerialPetriNetC_untitle_H_INCLUDED

#include "QROQS.h"

BSPetri *bsp_untitle_init(void);
int bsp_untitle_io(BSPetri *bsp);
void bsp_untitle_in_print(BSPetri *bsp);
void bsp_untitle_out_print(BSPetri *bsp);

#endif
