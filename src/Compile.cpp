// Compile.cpp: CCompile �N���X�̃C���v�������e�[�V����
//
/*
	Copyright Kozasa
*/
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Compile.h"

#include "QROQS.h"

#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>

#undef _DEBUG


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCompile::CCompile()
{
enzansi[0].c = LPAR;
enzansi[0].pri = 1;
enzansi[1].c = RPAR;
enzansi[1].pri = 1;
enzansi[2].c = NOT;
enzansi[2].pri = 7;
enzansi[3].c = AND;
enzansi[3].pri = 6;
enzansi[4].c = XOR;
enzansi[4].pri = 5;
enzansi[5].c = OR;
enzansi[5].pri = 4;
s = 0;
p = 0;
q = 1;
keta = 0;
}

CCompile::~CCompile()
{

}
/****************************************************

	�R���p�C�����ɌĂяo���̂�Kaiseki�ŁA
	�V�~�����[�V�������ɌĂяo���̂�Keisan�ł��B

****************************************************/

//	�\���`�F�b�N�ƌ�u�L�@�ւ̕ϊ�
void CCompile::Kaiseki(const char *fstring, char *fspost, int n, int r_add){
	int i, j, l, ipn;
	int end_flag = 0;
	char buffer[BUFFER_SIZE];

	row_add = r_add;

	l = 0;
	s = 0;

	while(1){

//	�o�b�t�@�ɕ�������i�[
		i = 0;
		while(fstring[l] != NEXT && fstring[l] != NULL){
			if(i >= BUFFER_SIZE){
				if(Qmode)
					sprintf(errormoji, "%d�Ԗڂ̎��Ȃ񂾂��ǂ��C����ȂɐH�ׂ��Ȃ��悧�c�D", q+row_add);
				else
					sprintf(errormoji, "%d�Ԗڂ̎����ޯ̧���ށi%d�j���ް���܂����D", q+row_add, BUFFER_SIZE);
				throw errormoji;
			}
		buffer[i++] = fstring[l++];
		}
		buffer[i] = fstring[l++];

//	 �X�y�[�X�̏���
		i = 0;
		while(buffer[i] != NEXT && buffer[i] != NULL){ 
			if(buffer[i] == SPACE){
				j = i;
				while(buffer[j] != NEXT && buffer[j] != NULL){
					buffer[j] = buffer[j + 1];
					j++;
				}
			}
			i++;
		}

//	���̓v���[�X�̗L�����`�F�b�N
		p = 0;
		while(buffer[p] != NEXT && buffer[p] != NULL){
			if(isdigit(buffer[p])){
				keta = 0;
				ipn = Inplacenumber(&buffer[p], 0);
				if(ipn > ATAI_SIZE)
					if(Qmode)
						throw "�����܂���C������ڰ���2���܂łŶ���݂��Ă��炦��ł���m(_ _)m";
					else{
						sprintf(errormoji, "������ڰ��̐�������i%d�j�𒴂��܂����D", ATAI_SIZE);
						throw errormoji;
					}
				else if(ipn < 1 || ipn > n){
					if(Qmode)
						sprintf(errormoji, "���񂽂����C%d�Ԗڂ̎��Ԉ���Ă��c�D", q+row_add);
					else
						sprintf(errormoji, "%d�Ԗڂ̎��ɑ��݂��Ȃ�������ڰ������g�p����Ă��܂��D", q+row_add);
					throw errormoji;
				}
				else if(keta == 2)
					p++;
			}
			p++;
		}

		Moji old, t;

		i = 0;
		old.type = START;
		stack.push(0);

		while(end_flag != 1){

			t = GetMoji(buffer[i++]);

//	�\���`�F�b�N
			if(old.type == START && t.type != ID && t.type != OPN && t.type != OPL ||
				old.type == ID && t.type != NUM ||
				old.type == NUM && t.type != NUM && t.type != OP && t.type != OPR && t.type != FIN && t.type != END ||
				old.type == OPN && t.type != ID && t.type != OPN && t.type != OPL ||
				old.type == OP && t.type != ID && t.type != OPN && t.type != OPL ||
				old.type == OPL && t.type != ID && t.type != OPN && t.type != OP && t.type != OPL ||
				old.type == OPR && t.type != OP && t.type != OPR && t.type != FIN && t.type != END){
				if(Qmode)
					sprintf(errormoji, "�����A%d�Ԃ��̂��[�I���炟����Ȃ̔F�߂˂���B����Ȃ̎�����Ȃ�!!", q+row_add);
				else
					sprintf(errormoji, "%d�Ԗڂ̎����Ԉ���Ă��܂��D", q+row_add);
				throw errormoji;
			}
			else if(t.type == FIN || t.type == END){
				t.type = OPR;
				t.op = 1;
				Parse(t, fspost);
				fspost[s++] = NEXT;
				end_flag = 1;
				while(!stack.empty())
					Pop();
			}
			else{
				Parse(t, fspost);
			}
			old = t;
		}
		end_flag = 0;
		if(fstring[l - 1] == NULL || fstring[l] == NULL)
			break;
		q++;
	}
fspost[s] = '\0';
	s = 0;
}

//	i�i�j�H
int CCompile::Inplacenumber(const char *buffer, int ipn){
	if(isdigit(buffer[keta]) == 0){
		return ipn;
	}
	else{
		switch(buffer[keta++]){
		case '0': return Inplacenumber(buffer, ipn * 10 + 0);
		case '1': return Inplacenumber(buffer, ipn * 10 + 1);
		case '2': return Inplacenumber(buffer, ipn * 10 + 2);
		case '3': return Inplacenumber(buffer, ipn * 10 + 3);
		case '4': return Inplacenumber(buffer, ipn * 10 + 4);
		case '5': return Inplacenumber(buffer, ipn * 10 + 5);
		case '6': return Inplacenumber(buffer, ipn * 10 + 6);
		case '7': return Inplacenumber(buffer, ipn * 10 + 7);
		case '8': return Inplacenumber(buffer, ipn * 10 + 8);
		case '9': return Inplacenumber(buffer, ipn * 10 + 9);
		default: if(Qmode)
					 throw"���₢��A���肦����āc�D";
				 else{
				     sprintf(errormoji, "fatal error : buffer[%d]�ɗ\�����Ȃ��l���i�[����Ă��܂��D", keta - 1);
					 throw errormoji;
				 }
		}
	}
}

//	�����̎���
Moji CCompile::GetMoji(char c){
	Moji t = {0, 0, START};

	t.c = c;
	if(c == NULL)
		t.type = END;
	else if(c == NEXT)
		t.type = FIN;
	else if(c == I)
		t.type = ID;
	else if(isdigit(c))
		t.type = NUM;
	else if(c == LPAR){
		t.type = OPL;
		t.op = 0;
	}
	else if(c == RPAR){
		t.type = OPR;
		t.op = 1;
	}
	else if(c == NOT){
		t.type = OPN;
		t.op = 2;
	}
	else if(c == AND){
		t.type = OP;
		t.op = 3;
	}
	else if(c == XOR){
		t.type = OP;
		t.op = 4;
	}
	else if(c == OR){
		t.type = OP;
		t.op = 5;
	}
	else{
		if(Qmode)
			sprintf(errormoji, "%d�Ԗڂ̎����ĂȂ񂩕ςȂ̓��ꂽ�H", q+row_add);
		else
			sprintf(errormoji, "%d�Ԗڂ̎��ɋ�����Ă��Ȃ��������܂܂�Ă��܂��D", q+row_add);
		throw errormoji;
	}
	return t;
}

//	�����̑���
void CCompile::Parse(Moji t, char *fspost){
	int i;

	switch(t.type){
	case ID:
		fspost[s++] = t.c;
		break;
	case NUM:
		fspost[s++] = t.c;
		break;
	case OPN:
		ParseOp(t.op, fspost);
		break;
	case OP:
		ParseOp(t.op, fspost);
		break;
	case OPL:
		stack.push(0);
		break;
	case OPR:
		while(enzansi[i = Pop()].c != LPAR){
			fspost[s++] = enzansi[i].c;
		}
		break;
	default : if(Qmode)
				  throw "���߂�Ȃ����C�l�̂����ł��D";
			  else
				  throw"fatal error : t.type�ɗ\�����Ȃ��l���i�[����Ă��܂��D";
	}
}

//	���Z�q�̑���
void CCompile::ParseOp(int opnum, char *fspost){
	static Enzansi st, op;

	st = enzansi[Current()];
	op = enzansi[opnum];

	if(st.pri < op.pri || (st.pri == op.pri && op.c == NOT)){
		stack.push(opnum);
	}
	else{
		fspost[s++] = st.c;
		Pop();
		ParseOp(opnum, fspost);
	}
}

//stack����f�[�^��Pop����
int CCompile::Pop(){
	int i = 0;

	if(stack.empty()){
		if(Qmode)
			throw "������!? stack underflow...�������̑Ή����Ƃ�ĂȂ��H";
		else
			throw "fatal error : stack underflow.";
	}
	i = stack.top();
	stack.pop();
	return i;
}

//stack�̐擪�̒l���擾����
int CCompile::Current(){
	if(stack.empty()){
		if(Qmode)
			throw "���ꂠ���!? stack underflow...�������̑Ή����Ƃ�Ă܂���";
		else
			throw "fatal error : stack underflow.";
	}
	return stack.top();
}

//	�֐��v�Z
void CCompile::Keisan(const char *fspost, int *i, int *o, int n, int m){
	int j, k;
	int atai[ATAI_SIZE];
	Moji t;

	if(n > ATAI_SIZE)
		if(Qmode)
			throw "������2���܂łł��肢�˂��Č������̂Ɂc�D";
		else
			throw "fatal error : ������ڰ��̐�������𒴂��Ă��܂��D";

	for(k = 0; k < n; k++)
		atai[k] = i[k];

	s = 0;

	for(j = 0; j < m; j++){
		t = GetMoji(fspost[s++]);
		while(t.type != FIN && t.type != END){
			Compute(fspost, t, atai);
			t = GetMoji(fspost[s++]);
		}
		o[j] = Pop();
	}
	while(!stack.empty())
		Pop();
}

//	���Z
void CCompile::Compute(const char *fspost, Moji t, int *atai){
	int ipn;

	switch(t.type){
	case ID:
		break;
	case NUM:
		keta = 0;
		ipn = Inplacenumber(&fspost[s - 1], 0);
		stack.push(atai[ipn - 1]);
		if(keta == 2)
			s++;
		break;
	case OPN:
		stack.push(!(Pop()));
		break;
	case OP:
		if(t.c == AND)
			stack.push(Pop() & Pop());
		else if(t.c == XOR)
			stack.push(Pop() ^ Pop());
		else
			stack.push(Pop() | Pop());
		break;
	case FIN:
		break;
	default : if(Qmode)
				  throw "������l�̐ӔC�ł��ˁD�S���l�̂����Ȃ񂾁c�D";
			  else
				  throw "fatal error : t.type�ɗ\�����Ȃ��l���i�[����Ă��܂��D";
	}
}

//�@�����Ŏg�p���ꂽ���̓v���[�X�̐���Ԃ��֐�
int CCompile::UsedPlaceNumber(const char *fstring){
	int i, pos, num = 0;
	int place_array[ATAI_SIZE];

	for(i = 0; i < ATAI_SIZE; i++)
		place_array[i] = 0;

	i = 0;
	while(fstring[i] != NULL){
		if(fstring[i++] == I){
			keta = 0;
			pos = Inplacenumber(&fstring[i], 0);
			if(keta == 1)
				i++;
			else if(keta == 2)
				i = i + 2;
			else{
				if(Qmode)
					throw "���񂳂�C���������Ă���ł����c�D����Ȃ�������Ȃ��v���[�X���������Ă��������x����₹�񂺁D";
				else
					throw "fatal error : ���ɑ��݂��Ȃ�������ڰ������g�p����Ă��܂��D";
			}
		place_array[pos - 1] = 1;
		}
	}

	for(i = 0; i < ATAI_SIZE; i++)
		num += place_array[i];

	return num;
}

/// EOF ///
