#ifndef PROFESSOR_H  
#define PROFESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// struct do professor
typedef struct professor
{
	int id;
	char nome[MAX];
    int rg[MAX];
    char cpf[MAX];
    char endereco[MAX];
    float salario[MAX];
    int data_nasc[MAX];

} t_professor;

// funções

void cadastrar_professor();
void listar_professor();
void listar_professor_oa();
void pesquisar_professor();
void alterar_professor();
void excluir_professor();
void disciplinas_de_um_professor();

t_professor *obter_professor(FILE *arq_professor, int id_professor);
int existe_professor(FILE *arq_professor, int id_professor);
void atualizar_professor(FILE *arq_professor, t_professor *arqp_atualizar);

int str_somente_numeros(char str[]);

#endif