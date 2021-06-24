// Compile.h: CCompile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_)
#define AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

#define I			'i'
#define LPAR		'('
#define RPAR		')'
#define NOT			'~'
#define AND			'&'
#define XOR			'^'
#define OR			'|'
#define NEXT		'\n'
#define SPACE		' '
#define ZERO		'0'
#define BUFFER_SIZE	1000
#define MOJI_SIZE	200
#define ATAI_SIZE	99

typedef struct Enzansi{
	int c;
	int pri;
} Enzansi;


typedef enum Type{
	START,		/* zero */
	END,		/* NULL */
	FIN,		/* ����֐��̏I��� */
	ID,			/* ���ʎq */
	NUM,		/* ���� */
	OPN,		/* NOT */
	OP, 		/* ���Z�q */
	OPL, 		/* ������ */
	OPR, 		/* �E���� */
} Type;

typedef struct Moji{
	int c;
	int op;
	Type type;
} Moji;

class CCompile  
{
public:
	CCompile();

	//	�\���`�F�b�N�ƌ�u�L�@�ւ̕ϊ�
	void CCompile::Kaiseki(const char *fstring, char *fspost, int n, int r_add=0);

	//	i�i�j�H
	int CCompile::Inplacenumber(const char *buffer, int m);

	//	�����̎���
	Moji CCompile::GetMoji(char c);

	//	�����̑���
	void CCompile::Parse(Moji t, char *fspost);

	//	���Z�q�̑���
	void CCompile::ParseOp(int opnum, char *fspost);

	//stack����f�[�^��Pop����
	int CCompile::Pop();

	//stack�̐擪�̒l���擾����
	int CCompile::Current();

	//	�֐��v�Z�ƃg�[�N���̃Z�b�g
	void CCompile::Keisan(const char *fspost, int *i, int *o, int n, int m);

	//	���Z
	void CCompile::Compute(const char *fspost, Moji t, int *atai);

	//�@�����Ŏg�p���ꂽ���̓v���[�X�̐���Ԃ��֐�
	int CCompile::UsedPlaceNumber(const char *fstring);

stack<int>	stack;

Enzansi enzansi[6];

int p;
int q;
int keta;		// Inplacenumber�̕Ԃ�l�̌���
int s;

int row_add;

char errormoji[MOJI_SIZE];

	virtual ~CCompile();

};




#endif // !defined(AFX_COMPILE_H__3211EA1C_024F_4418_B768_99D18ED06CAB__INCLUDED_)
