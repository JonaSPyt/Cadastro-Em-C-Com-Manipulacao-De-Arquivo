#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif

#include "disciplina.h"
#include "professor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int comparar_nome (const void *a, const void *b) {
    return strcmp (((t_professor *)a)->nome,((t_professor *)b)->nome);

}


//função cadastrar professor
void cadastrar_professor()
{
	FILE *arq_professor = fopen("professor.bin", "a+b");
 
	if(arq_professor == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1);
	}
	int cont_bytes = 0;
 
	fseek(arq_professor, 0, SEEK_END);
	cont_bytes = ftell(arq_professor);
 
	t_professor professor;
 
	if(cont_bytes == 0)
	{
		professor.id = 1;
	}
	else
	{
		t_professor ultimo_professor;
 
		fseek(arq_professor, cont_bytes - sizeof(t_professor), SEEK_SET);
 
		fread(&ultimo_professor, sizeof(t_professor), 1, arq_professor);
 
		professor.id = ultimo_professor.id + 1;
	}

    printf("\nDigite o nome do professor: ");
    gets(professor.nome);
	printf("\nDigite o cpf do professor: ");
	scanf("%10s%*c", professor.cpf);
    printf("\nDigite o rg do professor: ");
    scanf("%99[^\n]%*c", professor.rg);
	printf("\nDigite o endereco do professor: ");
	scanf("%99[^\n]%*c", professor.endereco);
	printf("\nDigite o salario do professor: ");
	scanf("%99[^\n]%*c", professor.salario);
	printf("\nDigite a data de nascimento do professor: ");
	scanf("%10s%*c", professor.data_nasc);


	fseek(stdin, 0, SEEK_END); 
	fseek(arq_professor, 0, SEEK_END);
	fwrite(&professor, sizeof(t_professor), 1, arq_professor);

 
	fclose(arq_professor);
	system(limpar_tela);
 
	printf("\nProfessor \"%s\" cadastrado com sucesso!\n", professor.nome);
    printf("\nRg:\"%s\"\n", professor.rg);
    printf("\nCpf:\"%s\"\n", professor.cpf);
    printf("\nEndereco:\"%s\"\n", professor.endereco);
    printf("\nSalario:\"%s\"\n", professor.salario);
    printf("\nData de nascimento: \"%s\"\n", professor.data_nasc);
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END);
}

//função listar professor
void listar_professor()
{
    FILE *arq_professor = fopen("professor.bin", "rb");

	if(arq_professor == NULL)
	{
		printf("\nFalha ao abrir arquivo(s) ou ");
		printf("Nenhum professor cadastrado.\n");
		printf("\nPressione <Enter> para continuar...");
		scanf("%*c");
 
		fseek(stdin, 0, SEEK_END);
		return;
	}
 
	int encontrou_professor = 0;
	t_professor professor;
 
	printf("\nListando todos os professores...\n");
	while(1)
	{
		size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);

		if(result == 0)
			break;
 
		// atualiza a variável indicando que encontrou
		// pelo menos um professor
		encontrou_professor = 1;
 
		// mostra os dados do professor
		printf("\nID do professor: %d\n", professor.id);
		printf("Nome do professor: %s\n", professor.nome);
		printf("Rg do professor: %s\n", professor.rg);
		printf("Cpf do professor: %s\n", professor.cpf);
		printf("endereco do professor: %s\n", professor.endereco);
		printf("salario do professor: %s\n", professor.salario);
		printf("data de nascimento do professor: %s\n", professor.data_nasc);
	}

	if(encontrou_professor == 0)
		printf("\nNenhum professor cadastrado.\n");
 
	fclose(arq_professor);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

// função listar professor em ordem alfabética
void listar_professor_oa(){

    char nome_aux[MAX];
    FILE *arq_professor = fopen("professor.bin", "rb");


    if(arq_professor == NULL)
    {
        printf("\nFalha ao abrir arquivo(s) ou ");
        printf("Nenhum professor cadastrado.\n");
        printf("\nPressione <Enter> para continuar...");
        scanf("%*c");

        fseek(stdin, 0, SEEK_END);
        return;
    }

    int encontrou_professor = 0;
    t_professor professor;

    strcpy(nome_aux, professor.nome);

    printf("\nListando todos os professores...\n");
    while(1)
    {
        size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);
        qsort(&professor, 2 , sizeof(t_professor), comparar_nome);

        if(result == 0)
            break;

        // atualiza a variável indicando que encontrou
        // pelo menos um professor
        encontrou_professor = 1;

            // mostra os dados do professor
            printf("Nome do professor: %s\n", professor.nome);


    }

    if(encontrou_professor == 0)
        printf("\nNenhum professor cadastrado.\n");

    fclose(arq_professor);

    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    // uma forma de "limpar" o buffer de entrada
    fseek(stdin, 0, SEEK_END); // não recomendável o uso
}


// função que obtém um professor pelo ID
t_professor *obter_professor(FILE *arq_professor, int id_professor)
{
	// vai para o início do arquivo
	rewind(arq_professor);
 
	t_professor *professor;
 
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	// aloca espaço mesmo sem saber se o professor existe
	professor = (t_professor *)malloc(sizeof(t_professor));
	while(1)
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(professor, sizeof(t_professor), 1, arq_professor);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
		{
			free(professor); // libera a mémoria, pois o professor não foi encontrado
			return NULL;
		}
		// verifica se os ID's são iguais
		if(professor->id == id_professor)
			break;
	}
	return professor;

}

//função pesquisar professor
void pesquisar_professor(){

char nome[MAX];

int encontrou_professor = 0;
 
	// rb+ => abre para leitura somente
	FILE *arq_professor = fopen("professor.bin", "rb");
 
	// testa a abertura do arquivo
	if(arq_professor == NULL)
	{
		printf("\nFalha ao abrir arquivo(s)!\n");
		exit(1); // aborta o programa
	}
 
	printf("\nDigite o nome do professor: ");
	scanf("%99[^\n]%*c", nome);
 
	printf("\n Professores com o nome \"%s\":\n\n", nome);
	// loop para percorrer o arquivo
	t_professor professor;
	while(1)
	{
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		char nome_aux[MAX];
		// faz uma cópia para não alterar professor->nome
		strcpy(nome_aux, professor.nome);        
 
		// verifica se é igual
		if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
		{
			// mostra os dados do professor
			printf("ID do professor: %d\n\n", professor.id);
			printf("Rg do professor: %s\n", professor.rg);
			printf("Cpf do professor: %s\n", professor.cpf);
			printf("endereco do professor: %s\n", professor.endereco);
			printf("salario do professor: %s\n", professor.salario);
			printf("data de nascimento do professor: %s\n", professor.data_nasc);
			encontrou_professor = 1;
		}
	}
 
	if(encontrou_professor == 0)
		printf("Nenhum professor encontrado.\n\n");
 
	// fecha o arquivo
	fclose(arq_professor);
	printf("Pressione <Enter> para continuar...");
	scanf("%*c");
 
	// uma forma de "limpar" o buffer de entrada
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

// função para atualizar um professor
// recebe o ponteiro para o arquivo e o professor
void atualizar_professor(FILE *arq_professor, t_professor *arqp_atualizar)
{
	// vai para o início do arquivo
	rewind(arq_professor);
 
	t_professor professor;
	while(1) // loop para percorrer o arquivo
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// verifica se os ID's são iguais
		if(professor.id == arqp_atualizar->id)
		{
			// fseek posiciona o arquivo
			fseek(arq_professor, sizeof(t_professor), SEEK_CUR);
			// atualiza o professor
			fwrite(arqp_atualizar, sizeof(t_professor), 1, arq_professor);
			break; // sai do loop
		}
	}
}
//função alterar professor
void alterar_professor()
{
	char str_id_professor[10];
	int id_professor;
 
	// rb+ abre para leitura/atualização
	FILE *arq_professor = fopen("professor.bin", "rb+");
 
	// se não conseguiu abrir, então cria o arquivo
	// wb+ abre para escrita/atualização (cria o arquivo se ele NÃO existir)
	if(arq_professor == NULL)
	{
		arq_professor = fopen("professor.bin", "wb+");
		if(arq_professor == NULL)
		{
			printf("\nFalha ao criar arquivo(s)!\n");
			exit(1); // aborta o programa
		}
	}
 
	// obtém o ID do professor
	printf("\nDigite o ID do professor: ");
	scanf("%10s%*c", str_id_professor);
 
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
 
	if(str_somente_numeros(str_id_professor) == 1)
	{
		// se caiu aqui é porque o ID possui somente números, então
		// preenche a variável "id_professor" com o valor de "str_id_professor"
		sscanf(str_id_professor, "%d", &id_professor);
 
		// verifica se o ID do professor existe
		if(existe_professor(arq_professor, id_professor))
		{

			if(str_somente_numeros(str_id_professor) == 1)
			{
				// se chegou aqui é porque o ID do professor é válido
				sscanf(str_id_professor, "%d", &id_professor);
 
				// obtém o professor pelo ID
				t_professor *professor = obter_professor(arq_professor, id_professor);
 
				// testa se o professor existe...
				if(professor != NULL)
				{		printf("\nDigite o novo nome do professor: ");
						gets(professor->nome);
						printf("\nDigite o novo rg do professor: ");
						scanf("%99[^\n]%*c", professor->rg);
						printf("\nDigite o novo cpf do professor: ");
						scanf("%10s%*c", professor->cpf);
						printf("\nDigite o novo endereco do professor: ");
						scanf("%99[^\n]%*c", professor->endereco);
						printf("\nDigite o salario do professor: ");
						scanf("%99[^\n]%*c", professor->salario);
						printf("\nDigite a data de nascimento do professor: ");
						scanf("%10s%*c", professor->data_nasc);
						atualizar_professor(arq_professor, professor); // atualiza o professor no arquivo
					
					free(professor); // libera o professor alocado
				}
				else
					printf("\nNao existe professor com o ID \"%d\".\n", id_professor);
			}
			else
				printf("\nO ID so pode conter numeros!\n");
		}
	}
	// fecha os arquivos
	fclose(arq_professor);
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
 
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

//função que verifica se o professor existe
int existe_professor(FILE *arq_professor, int id_professor)
{
	// vai para o início do arquivo, pois não sabemos a posição do ponteiro no arquivo
	rewind(arq_professor);
 
	t_professor professor;
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	while(1)
	{
 
		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);
 
		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;
 
		// verifica se o ID é igual
		if(professor.id == id_professor)
			return 1;
	}
 
	// se chegou aqui é porque NÃO existe o professor, então retorna 0
	return 0;
}
//função excluir professor
void excluir_professor(){

char str_id_professor[10];
int id_professor;

	printf("\nDigite o ID do professor: ");
	scanf("%10s%*c", str_id_professor);
 
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
 
	// verifica se str_id_professor só contém números
	if(str_somente_numeros(str_id_professor) == 1)
	{
		// se chegou aqui é porque o ID do professor é válido
		sscanf(str_id_professor, "%d", &id_professor);
 
		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_professor = fopen("professor.bin", "rb");
 
		if(arq_professor == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); // aborta o programa
		}
 
		// verifica se o professor existe
		if(existe_professor(arq_professor, id_professor) == 1)
		{
			char nome_professor[MAX];
			// abre um novo arquivo temporário
			FILE *arq_temp = fopen("temp_professor.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_professor);
				exit(1); // aborta o programa
			}
			rewind(arq_professor); // vai para o início do arquivo
 
			t_professor professor;
			while(1) // loop para percorrer o arquivo
			{
 
				// fread retorna o número de elementos lidos com sucesso
				size_t result = fread(&professor, sizeof(t_professor), 1, arq_professor);
 
				// se for 0, é porque não há mais elemento, então sai do loop
				if(result == 0)
					break;
 
				// só copia pro novo arquivo se for diferente
				if(professor.id != id_professor)
				{
					// escreve no arquivo temporário
					fwrite(&professor, sizeof(t_professor), 1, arq_temp);
				}
				else
					strcpy(nome_professor, professor.nome);
			}
 
			// antes de fazer operações de remover arquivo e renomear,
			// é preciso fechar os dois arquivos
			fclose(arq_professor);
			fclose(arq_temp);
 
			// depois de fechar o arquivo, então tentamos remover
			if(remove("professor.bin") != 0)
				printf("\nErro ao deletar o arquivo \"professor.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_professor.bin", "professor.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_professor.bin\" e renomeie manualmente para \"filmes.bin\"\n");
				}
				else
					printf("\nProfessor \"%s\" removido com sucesso!\n", nome_professor);
			}
		}
		else
		{
			fclose(arq_professor);
			printf("\nNao existe professor com o ID \"%d\".\n", id_professor);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");
 
	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

//lista todas as disciplinas lecionadas por um professor
void disciplinas_de_um_professor(){

    char str_id_professor[10];
    int id_professor;

        FILE *arq_disciplina = fopen("disciplina.bin", "rb");
        FILE *arq_professor = fopen("professor.bin", "rb");

        if(arq_disciplina == NULL)
        {
            arq_disciplina = fopen("disciplina.bin", "wb+");
            if(arq_disciplina == NULL)
            {
                printf("\nFalha ao criar arquivo(s)!\n");
                exit(1); // aborta o programa
            }
        }

        if(arq_professor == NULL)
        {
            arq_professor = fopen("professor.bin", "wb+");
            if(arq_professor == NULL)
            {
                printf("\nFalha ao criar arquivo(s)!\n");
                exit(1); // aborta o programa
            }
        }

        // obtém o ID do professor
        printf("\nDigite o ID do professor: ");
        scanf("%10s%*c", str_id_professor);

    if(str_somente_numeros(str_id_professor) == 1)
    {
        // se caiu aqui é porque o ID possui somente números, então
        // preenche a variável "id_professor" com o valor de "str_id_professor"
        sscanf(str_id_professor, "%d", &id_professor);

        // verifica se o ID do professor existe
        if(existe_professor(arq_professor, id_professor))
        {

            int encontrou_disciplina = 0;
            t_disciplina disciplina;

            printf("\nListando todas as disciplinas de um professor...\n");
            while(1)
            {
                size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

                if(result == 0)
                    break;

                // atualiza a variável indicando que encontrou
                // pelo menos uma disciplina
                encontrou_disciplina= 1;

                // mostra os dados da disciplina
                printf("\nID da disciplina: %d\n", disciplina.id);
                printf("Nome da disciplina: %s\n", disciplina.nome);
                printf("Descricao da disciplina: %s\n", disciplina.descricao);
                printf("Numero de creditos da disciplina %s\n", disciplina.num_creditos);
                printf("Carga horaria da disciplina: %s\n", disciplina.carga_hora);
            }
            if(encontrou_disciplina == 0)
                printf("\nNenhuma disciplina cadastrada.\n");
        }else
                printf("\nO ID so pode conter numeros!\n");
        }else
            printf("\nNao existe professor com o ID \"%d\".\n", id_professor);


    // fecha os arquivos
    fclose(arq_professor);
    fclose(arq_disciplina);

    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    fseek(stdin, 0, SEEK_END); // não recomendável o uso
}