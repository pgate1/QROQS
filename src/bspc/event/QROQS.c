/******************************************
	QROQS over to C Language !!
	Writen by pgate1
	Last Update : 17:13 2003/11/23
******************************************/
#include "QROQS.h"

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
	)
{
	int i,j;
	int obj_size;
	BSPetri *bsp;
	Place *place;
	Tran *tran;
	Uhzi *uhzi;
	Kem *kem;
	Object *obj;
	
	bsp = (BSPetri *)malloc(sizeof(BSPetri));
	if(bsp==0) return 0;

	memset(bsp, 0, sizeof(BSPetri) );
	
	bsp->place_size	= place_size;
	bsp->tran_size	= tran_size;
	bsp->uhzi_size	= uhzi_size;
	bsp->kem_size	= kem_size;
	bsp->obj_size	= obj_size	= tran_size + uhzi_size + kem_size;
	
	place	= bsp->place	= (Place*)malloc(sizeof(Place)*place_size);
	tran	= bsp->tran		= (Tran*)malloc(sizeof(Tran)*tran_size);
	uhzi	= bsp->uhzi		= (Uhzi*)malloc(sizeof(Uhzi)*uhzi_size);
	kem		= bsp->kem		= (Kem*)malloc(sizeof(Kem)*kem_size);
	obj		= bsp->obj		= (Object*)malloc(sizeof(Object)*obj_size);

	bsp->init_token = init_token;
	bsp->init_value = init_value;
	bsp->init_fire = init_fire;
	bsp->feedback_init = feedback_init;

	if(place==0 || tran==0 || uhzi==0 || kem==0 || obj==0){
		bsp_exit(bsp);
		return 0;
	}

	memset(place, 0, sizeof(Place)*place_size);
	memset(tran, 0, sizeof(Tran)*tran_size);
	memset(uhzi, 0, sizeof(Uhzi)*uhzi_size);
	memset(kem, 0, sizeof(Kem)*kem_size);
	memset(obj, 0, sizeof(Object)*obj_size);

	// Initializing Object
	{
		int in_p, out_p, jt_p;
		in_p = 0;
		out_p = 0;
		jt_p = 0;
		for(i=0; i<obj_size; i++){
			obj[i].in_place_size	= in_place_size[i];
			obj[i].out_place_size	= out_place_size[i];
			obj[i].joint_size	= joint_size[i];
			obj[i].in_place	 = (Place**)malloc(sizeof(Place*)*in_place_size[i]);
			obj[i].out_place = (Place**)malloc(sizeof(Place*)*out_place_size[i]);
			obj[i].joints = (Object**)malloc(sizeof(Object*)*joint_size[i]);

			if(!obj[i].in_place || !obj[i].out_place || !obj[i].joints){
				bsp_exit(bsp);
				return 0;
			}

			for(j=0; j<in_place_size[i]; j++,in_p++){
				obj[i].in_place[j]	= &place[in_place[in_p]];
			}
			for(j=0; j<out_place_size[i]; j++,out_p++){
				obj[i].out_place[j] = &place[out_place[out_p]];
			}
			for(j=0; j<joint_size[i]; j++,jt_p++){
				obj[i].joints[j] = & obj[ joints[jt_p] ];
			}
		}
	}

	// Initializing Transition
	{
		int in_max, out_max;
		in_max = 1;
		out_max = 1;
		for(i=0; i<tran_size; i++){
			tran[i].guard = trafuncs[guard[i]];
			tran[i].func = trafuncs[func[i]];
			tran[i].feedback_size = feedback_size[i];
			tran[i].feedback = (char *)malloc(sizeof(char)*feedback_size[i]);
			if(!tran[i].feedback){
				bsp_exit(bsp);
				return 0;
			}

			obj[i].type = _TRAN;
			obj[i].prim.tran = &tran[i];

			if(in_place_size[i]+feedback_size[i] > in_max)
				in_max = in_place_size[i]+feedback_size[i];
			if(out_place_size[i]+feedback_size[i] > out_max)
				out_max = out_place_size[i]+feedback_size[i];
		}
		bsp->in_max = in_max;
		bsp->out_max = out_max;
	}

	// Initializing Uhzi
	for(i=0; i<uhzi_size; i++){
		obj[tran_size+i].type = _UHZI;
		obj[tran_size+i].prim.uhzi = &uhzi[i];
	}

	// Initializing Kemmakix
	for(i=0; i<kem_size; i++){
		obj[tran_size+uhzi_size+i].type = _KEM;
		obj[tran_size+uhzi_size+i].prim.kem = &kem[i];
	}

	return bsp;
}

int bsp_io(BSPetri *bsp, int u_dmax[], int k_dmax[], char **input_data, char **output_data)
{
	int i;
	// Initializing Uhzi
	for(i=0; i<bsp->uhzi_size; i++){
		bsp->uhzi[i].dmax = u_dmax[i];
		if(!input_data[i]) return 0;
		bsp->uhzi[i].data = input_data[i];
	}

	// Initializing Kemmakix
	for(i=0; i<bsp->kem_size; i++){
		bsp->kem[i].dmax = k_dmax[i];
		if(!output_data[i]) return 0;
		bsp->kem[i].data = output_data[i];
	}

	return 1;
}

int bsp_io_data(BSPetri *bsp, int u_dmax[], int k_dmax[], char *in[])
{
	int i,j;

	for(i=0;i<bsp->uhzi_size;i++){
		bsp->uhzi[i].dmax = u_dmax[i];
		bsp->uhzi[i].data = (char *)malloc(sizeof(char)*u_dmax[i]);

		for(j=0;j<u_dmax[i];j++){
			bsp->uhzi[i].data[j] = in[i][j];
		}
	}

	for(i=0;i<bsp->kem_size;i++){
		bsp->kem[i].dmax = k_dmax[i];
		bsp->kem[i].data = (char *)malloc(sizeof(char)*k_dmax[i]);
	}

	return 1;
}

void bsp_in_print( BSPetri *bsp )
{
	int i,j;
	for(i=0;i<bsp->uhzi_size;i++){
		printf("in%d :\n",i);
		for(j=0;j<bsp->uhzi[i].dmax;j++){
			printf("%d",bsp->uhzi[i].data[j] & 0x01);
		}
		printf("\n");
	}
}

void bsp_bsp_in_print(
	BSPetri *bsp,
	char *input_name[]
	)
{
	int i,j;
	for(i=0;i<bsp->uhzi_size;i++){
		printf("in%d [%s] :\n",i,input_name[i]);
		for(j=0;j<bsp->uhzi[i].dmax;j++){
			printf("%d",bsp->uhzi[i].data[j] & 0x01);
		}
		printf("\n");
	}
}

int bsp_reset(BSPetri *bsp)
{
	int i,j,feedback_n;
	Object *obj;
	Place *place;
	Tran *tran;

	if(!bsp) return 0;

	// Reseting Token
	place = &bsp->place[0];
	for(i=0; i<bsp->place_size; i++){
		place[i].token_state = bsp->init_token[i];
		place[i].token_value = bsp->init_value[i];
	}

	// Reseting Fire
	obj = &bsp->obj[0];
	for(i=0; i<bsp->obj_size; i++){
		obj[i].state = bsp->init_fire[i];
	}

	// Reseting Fire
	feedback_n = 0;
	tran = &bsp->tran[0];
	for(i=0; i<bsp->tran_size; i++){
		for(j=0;j<tran[i].feedback_size;j++,feedback_n++){
			tran[i].feedback[j] = bsp->feedback_init[feedback_n];
		}
	}


	return 1;
}


#define _EVENT_

int bsp_run(BSPetri *bsp, int loop_max )
{
	int i,j,loop;
	Object *obj;
	Tran *tran;
	Uhzi *uhzi;
	Kem *kem;
	char in_next_state, out_next_state;
	char *in_value, *out_value;
	Place **in_places, **out_places;
#ifdef _EVENT_
	int event_top, event_last, event_size;
	Object **event;
#else
	int oid;
#endif
	if(!bsp) return 1;

	in_value = (char*)malloc(sizeof(char) * bsp->in_max);
	out_value = (char*)malloc(sizeof(char) * bsp->out_max);
	if(!in_value || !out_value) return 1;
#ifdef _EVENT_
	event_size = bsp->obj_size + 1;
	event = (Object **)malloc(sizeof(Object*)*event_size);
	if(!event) return 1;
#endif
	//uhzi
	uhzi = &bsp->uhzi[0];
	for(i=0;i<bsp->uhzi_size;i++){
		uhzi[i].ptr = 0;
	}
	//kem
	kem = &bsp->kem[0];
	for(i=0;i<bsp->kem_size;i++){
		kem[i].ptr = 0;
	}
	obj = &bsp->obj[0];

	// Adding all objects into event list.
	for(i=0;i<bsp->obj_size;i++){
		obj[i].event = 1;
	}

#ifdef _EVENT_
	// Adding all objects into event list.
	event_top=0;
	for(event_last=0; event_last<bsp->obj_size; event_last++){
		event[event_last] = &obj[event_last];
		obj[event_last].event = 1;
	}
#endif

	//printf("event start\n");
	// Event start.
	loop = 0;
#ifdef _EVENT_
	for(;event_top!=event_last;){

	//	if(loop>loop_max) break;
	//	loop++;
		
		obj = event[event_top];
#else
	for(obj=(Object*)1;obj;){

	//	if(loop>loop_max) break;
	//	loop++;

		obj = 0;
		for(oid=0;oid<bsp->obj_size;oid++){
		if(bsp->obj[oid].event==0) continue;
		obj = &bsp->obj[oid];
#endif
	//	if(obj->type==_TRAN && rand()%10<5) continue;
	//	printf("%d	type %d \n",o,obj->type);

		// Checking input place
		in_places = obj->in_place;
		in_next_state = obj->state==0 ? _BTOKEN_EXIST : _WTOKEN_EXIST;
		for(i=0;i<obj->in_place_size;i++){
			if(in_places[i]->token_state != in_next_state){
				goto OBJ_END;
			}
		}
		// Cheching output place
		out_places = obj->out_place;
		out_next_state = obj->state==0 ? _WTOKEN_OFF : _BTOKEN_OFF;
		for(i=0;i<obj->out_place_size;i++){
			if(out_places[i]->token_state != out_next_state){
				goto OBJ_END;
			}
		}

		// Woking objects
		switch(obj->type){
		case _TRAN:
			// Get Token Value.
			if(in_next_state == _BTOKEN_EXIST || out_next_state == _WTOKEN_OFF){
				if(obj->prim.tran->func){
					tran = obj->prim.tran;
					for(i=0;i<obj->in_place_size;i++){
						in_value[i] = in_places[i]->token_value;
					}
					for(j=0;j<tran->feedback_size;j++,i++){
						in_value[i] = tran->feedback[j];
					}

					// Checking Guard tarm
					if(tran->guard){
						tran->guard(in_value,out_value);
						if((out_value[0] & 0x01) == 0){
							goto OBJ_END;
						}
					}

					// Fire !
					//printf("	fire !\n");
					tran->func(in_value, out_value);
					for(i=0;i<obj->out_place_size;i++){
						out_places[i]->token_value = out_value[i];
					}
					for(j=0;j<tran->feedback_size;j++,i++){
						tran->feedback[j] = out_value[i];
					}
				}
				else{ // pass tra
					out_places[0]->token_value = in_places[0]->token_value;
				}
			}
			break;
		case _UHZI:
//			printf("uhzi\n");
			uhzi = obj->prim.uhzi;
			// No data. No industry.
			if(uhzi->ptr == uhzi->dmax) goto OBJ_END;
			// Get Token Value.
			if(out_next_state == _WTOKEN_OFF){
				out_places[0]->token_value = uhzi->data[uhzi->ptr];
			}
			// Next data
			if(out_next_state == _BTOKEN_OFF) uhzi->ptr++;		
		//	printf("	industry\n");
			break;
		case _KEM:
			kem = obj->prim.kem;
			// No size No Consum
			if(kem->ptr == kem->dmax) goto KEM_END;
			// Get Token Data
			if(in_next_state == _BTOKEN_EXIST){
				kem->data[kem->ptr] = in_places[0]->token_value;
			}
			// Next data
			if(in_next_state == _WTOKEN_EXIST) kem->ptr++;
		//	printf("	Consum %d\n",kem->ptr);
KEM_END:
			break;
		}
		
		// Put Token to output places.
		for(i=0;i<obj->out_place_size;i++){
			out_places[i]->token_state = in_next_state;
		}
		out_next_state ^= 0x10;
		// Get Token from input places.
		for(i=0;i<obj->in_place_size;i++){
			in_places[i]->token_state = out_next_state;
		}

		obj->state = 1 - obj->state;

		// Adding events.
#ifdef _EVENT_
		for(i=0;i<obj->joint_size;i++){
			//	printf("add event\n");
			if(obj->joints[i]->event) continue;
			obj->joints[i]->event = 1;
			event[event_last] = obj->joints[i];
			event_last++;
			if(event_last==event_size) event_last=0;
		}
#else
		for(i=0;i<obj->joint_size;i++){
			obj->joints[i]->event = 1;
		}
#endif
		
OBJ_END:
		
		obj->event = 0;
#ifdef _EVENT_
		event_top++;
		if(event_top==event_size) event_top=0;
#else
		}
#endif
	}

#ifdef _EVENT_
	free(event);
#endif

	free(in_value);
	free(out_value);

	return 0;
}

void bsp_out_print( BSPetri *bsp )
{
	int i,j;
	for(i=0;i<bsp->kem_size;i++){
		printf("out%d :\n",i);
		for(j=0;j<bsp->kem[i].ptr;j++){
			printf("%d",bsp->kem[i].data[j] & 0x01);
		}
		printf("\n");
	}
}

void bsp_bsp_out_print(
	BSPetri *bsp,
	char *output_name[],
	unsigned char dline[],
	int bits[],
	unsigned char hex[])
{
	int kn,i,j,l,d;

	for(kn=0;kn<bsp->kem_size;kn++){
		printf("out%d [%s] :\n",kn,output_name[kn]);
		for(j=0;j<bsp->kem[kn].ptr;j+=bits[kn]){
			if(j+bits[kn]>bsp->kem[kn].ptr){//éÛÇØéÊÇËÇ´Ç¡ÇƒÇ¢Ç»Ç¢
				if(dline[kn]==0){//è„à Ç©ÇÁ
					for(i=j; i<bsp->kem[kn].ptr; ++i) printf("%d",bsp->kem[kn].data[i] & 0x01);
				}
				else{
					for(i=j+bits[kn]-1; i>=bsp->kem[kn].ptr; --i) printf(" ");
					for(; i>=j; --i) printf("%d",bsp->kem[kn].data[i] & 0x01);
				}
			}
			else{//éÛÇØéÊÇËÇ´Ç¡ÇƒÇ¢ÇÈèÍçá
				if(hex[kn]==1 && bits[kn]%4==0){
					if(dline[kn]==0){
						for(i=j; i<j+bits[kn];){
							d=0;
							for(l=0;l<4;l++,i++){
								d <<= 1;
								d |= bsp->kem[kn].data[i] & 0x01;
							}
							printf("%0X",d);
						}
					}
					else{
						for(i=j+bits[kn]-1; i>=j;){
							d=0;
							for(l=0;l<4;l++,i--){
								d <<= 1;
								d |= bsp->kem[kn].data[i] & 0x01;
							}
							printf("%0X",d);
						}
					}
				}
				else{
					if(dline[kn]==0){
						for(i=j; i<j+bits[kn]; ++i) printf("%d",bsp->kem[kn].data[i] & 0x01);
					}
					else{
						for(i=j+bits[kn]-1; i>=j; --i) printf("%d",bsp->kem[kn].data[i] & 0x01);
					}
				}
			}
			printf("\n");
		}
	}
}

void bsp_io_exit(BSPetri *bsp)
{
	int i;
	for(i=0;i<bsp->uhzi_size;i++)	free(bsp->uhzi[i].data);
	for(i=0;i<bsp->kem_size;i++)	free(bsp->kem[i].data);
}

void bsp_exit(BSPetri *bsp)
{
	int i;

	if(!bsp) return;

	if(bsp->place) free(bsp->place);

	if(bsp->tran){
		for(i=0;i<bsp->tran_size;i++) free(bsp->tran[i].feedback);
		free(bsp->tran);
	}

	if(bsp->uhzi)	free(bsp->uhzi);
	if(bsp->kem)	free(bsp->kem);
	
	if(bsp->obj){
		for(i=0;i<bsp->obj_size;i++){
			if(bsp->obj[i].joints)		free( bsp->obj[i].joints );
			if(bsp->obj[i].in_place)	free( bsp->obj[i].in_place );
			if(bsp->obj[i].out_place)	free( bsp->obj[i].out_place );
		}
		free(bsp->obj);
	}

	free(bsp);
}

/// EOF ///
