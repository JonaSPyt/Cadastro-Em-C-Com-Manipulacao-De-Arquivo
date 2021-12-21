#ifndef DISCIPLINA_H  
#define DISCIPLINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "professor.h"

#define MAX 100

//struct definida da disciplina
typedef struct disciplina
{
	int id;
	int id_professor;
    char nome[MAX];
	char descricao[500];
	char num_creditos[MAX];
    char carga_hora[MAX];
} t_disciplina;

t_disciplina *obter_disciplina(FILE *arq_disciplina, int id);
int existe_disciplina(FILE *arq_disciplina, int id);
void atualizar_disciplina(FILE *arq_disciplina, t_disciplina *arq_atualizar);


int str_somente_numeros(char str[]);

//funções
void cadastrar_disciplina();
void listar_disciplina();
void listar_disciplina_oa();
void pesquisar_disciplina();
void alterar_disciplina();
void excluir_disciplina();
void professor_da_disciplina();


#endif