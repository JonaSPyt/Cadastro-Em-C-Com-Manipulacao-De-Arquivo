#ifdef __WIN32
#define limpar_tela "cls"
#else
#define limpar_tela "clear"
#endif

#include "professor.h"
#include "disciplina.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
// função que verifica se uma string contém somente números
int str_somente_numeros(char str[])
{

    int len_str = strlen(str);

    for(int i = 0; i < len_str; i++)
    {
        if(str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}


//função criar disciplina
void cadastrar_disciplina()
{
    FILE *arq_disciplina = fopen("disciplina.bin", "a+b");

    if(arq_disciplina == NULL)
    {
        printf("\nFalha ao abrir arquivo(s)!\n");
        exit(1);
    }
    t_disciplina disciplina;

    int cont_bytes = 0;

    fseek(arq_disciplina, 0, SEEK_END);

    cont_bytes = ftell(arq_disciplina);

    if(cont_bytes == 0)
    {
        disciplina.id = 1;
    }
    else
    {
        t_disciplina ultimo_disciplina;

        fseek(arq_disciplina, cont_bytes - sizeof(t_disciplina), SEEK_SET);

        fread(&ultimo_disciplina, sizeof(t_disciplina), 1, arq_disciplina);

        disciplina.id = ultimo_disciplina.id + 1;
    }

    printf("\nDigite o nome da disciplina: ");
    gets(disciplina.nome);
    printf("\nDigite a descricao da disciplina: ");
    scanf("%99[^\n]%*c", disciplina.descricao);
    printf("\nDigite o numero de creditos da disciplina: ");
    scanf("%10s%*c", disciplina.num_creditos);
    printf("\nDigite a carga horaria semanal da disciplina (2h, 4h,6h): ");
    scanf("%10s%*c", disciplina.carga_hora);
    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    fseek(stdin, 0, SEEK_END);
    disciplina.id_professor = -1;
    fseek(arq_disciplina, 0, SEEK_END);

    fwrite(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

    fclose(arq_disciplina);
    system(limpar_tela);

    printf("\nDisciplina \"%s\" cadastrado com sucesso!\n", disciplina.nome);
    printf("\nDescricao:\"%s\"\n", disciplina.descricao);
    printf("\nNumero de creditos:\"%s\"\n", disciplina.num_creditos);
    printf("\nCarga horaria:\"%s\"\n", disciplina.carga_hora);
    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    fseek(stdin, 0, SEEK_END);
}

//função listar disciplinas
void listar_disciplina(){

    // rb => abre para leitura somente, ponteiro para o início do arquivo
    FILE *arq_disciplina = fopen("disciplina.bin", "rb");
    FILE *arq_professor = fopen("professor.bin", "rb");

    // se o arquivo de disciplina não existe
    if(arq_disciplina == NULL)
    {
        printf("\nFalha ao abrir arquivo ou ");
        printf("Nenhuma disciplina cadastrada.\n");
        printf("\nPressione <Enter> para continuar...");
        scanf("%*c"); // pega o Enter e descarta

        // uma forma de "limpar" o buffer de entrada
        fseek(stdin, 0, SEEK_END); // não recomendável o uso
        return;
    }
    // variável que indica se encontrou pelo menos uma disciplina
    int encontrou_disciplina = 0;
    printf("\nListando todas as disciplinas...\n");
    // loop para percorrer o arquivo
    t_disciplina disciplina;
    while(1)
    {
        // fread retorna o número de elementos lidos com sucesso
        size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

        // se for 0, é porque não há mais elemento, então sai do loop
        if(result == 0){
            break;
        }

        // atualiza a variável indicando que encontrou
        // pelo menos uma disciplina
        encontrou_disciplina = 1;

        // mostra os dados da disciplina
        printf("\nDisciplina: \"%s\"", disciplina.nome);
        printf("\nDescricao:\"%s\"", disciplina.descricao);
        printf("\nNumero de creditos:\"%s\"", disciplina.num_creditos);
        printf("\nCarga horaria:\"%s\"", disciplina.carga_hora);

        // se id_professor for diferente de -1, então disciplina ja tem professor, pois
        // possui algum id válido do professor.
        // se id_professor for igual a -1, indica que a disciplina não foi ligada,
        // pois o id -1 NÃO é um id válido

        if(disciplina.id_professor != -1)
        {
            // se o arquivo professor não existir,
            // não pode ser realizado no início da função, pois pode acontecer de
            // todas as disciplinas não estarem ligadas
            if(arq_professor == NULL)
            {
                printf("\nFalha ao abrir arquivo!\n");
                fclose(arq_professor); // libera recursos
                exit(1); // aborta o programa
            }

            t_professor *professor = obter_professor(arq_professor, disciplina.id_professor);
            printf("Disciplina tem professor? Sim, professor: %s\n", professor->nome);
            free(professor); // evita vazamento de memória
        }else{
            printf("Disciplina tem professor? Nao\n");
        }
    }

    // verifica se encontrou pelo menos uma disciplina
    if(encontrou_disciplina == 0)
        printf("\nNenhuma disciplina cadastrada.\n");

    // fecha os arquivos em ordem inversa que foram abertos
    if(arq_professor != NULL)
        fclose(arq_professor); // apenas se foi aberto
        fclose(arq_disciplina);

    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    // uma forma de "limpar" o buffer de entrada
    fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

//função listar disciplina em ordem alfabética
void listar_disciplina_oa(){

}

// função que obtém disciplina pelo ID
t_disciplina *obter_disciplina(FILE *arq_disciplina, int id_disciplina)
{
    // vai para o início do arquivo
    rewind(arq_disciplina);

    // loop para percorrer o arquivo
    // busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
    t_disciplina *disciplina;

    // aloca espaço mesmo sem saber se a disciplina existe
    disciplina = (t_disciplina *)malloc(sizeof(t_disciplina));
    while(1)
    {

        // fread retorna o número de elementos lidos com sucesso
        size_t result = fread(disciplina, sizeof(t_disciplina), 1, arq_disciplina);

        // se for 0, é porque não há mais elemento, então sai do loop
        if(result == 0)
            break;

        // verifica se os ID's são iguais
        if(disciplina->id == id_disciplina)
            return disciplina;
    }
    free(disciplina); // libera recursos
    return NULL;
}
//função pesquisar disciplina
void pesquisar_disciplina()
{
    char nome[MAX];
    int encontrou_disciplina = 0;

    // rb => abre para leitura somente
    FILE *arq_disciplina = fopen("disciplina.bin", "rb");
    FILE *arq_professor = fopen("professor.bin", "rb");

    // testa a abertura do arquivo
    if(arq_disciplina == NULL)
    {
        printf("\nFalha ao abrir arquivo(s)!\n");
        exit(1); // aborta o programa
    }

    printf("\nDigite o nome da disciplina: ");
    scanf("%99[^\n]%*c", nome);

    printf("\nDisciplina com o nome \"%s\":\n\n", nome);
    // loop para percorrer o arquivo
    // busca linear, pois o campo nome não possui índice
    t_disciplina disciplina;
    while(1)
    {
        // fread retorna o número de elementos lidos com sucesso
        size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

        // se for 0, é porque não há mais elemento, então sai do loop
        if(result == 0)
            break;

        char nome_aux[MAX];
        // faz uma cópia para não alterar disciplina.nome
        strcpy(nome_aux, disciplina.nome);

        // verifica se é igual
        if(strcmp(strupr(nome_aux), strupr(nome)) == 0)
        {
            // mostra os dados da disciplina
            printf("\nDisciplina:\"%s\"", disciplina.nome);
            printf("\nDescricao:\"%s\"", disciplina.descricao);
            printf("\nNumero de creditos:\"%s\"", disciplina.num_creditos);
            printf("\nCarga horaria:\"%s\"", disciplina.carga_hora);

            if(disciplina.id_professor != -1)
            {
                // se o arquivo professor não existir
                if(arq_professor == NULL)
                {
                    printf("\nFalha ao abrir arquivo!\n");
                    fclose(arq_professor); // libera recursos
                    exit(1); // aborta o programa
                }
                t_professor *professor = obter_professor(arq_professor, disciplina.id_professor);
                printf("Disciplina tem professor? Sim, professor: %s\n", professor->nome);
                free(professor); // evita vazamento de memória
            }
            else{
                printf("Disciplina tem professor? Nao\n");
                encontrou_disciplina = 1;
                printf("\n");
            }
        }
    }

    if(encontrou_disciplina == 0)
        printf("Nenhuma disciplina encontrada.\n\n");

    fclose(arq_disciplina);

    printf("Pressione <Enter> para continuar...");
    scanf("%*c");

    // uma forma de "limpar" o buffer de entrada
    fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

// função para atualizar uma disciplina
// recebe o ponteiro para o arquivo e a disciplina
void atualizar_disciplina(FILE *arq_disciplina, t_disciplina *arq_atualizar)
{
	// vai para o início do arquivo
	rewind(arq_disciplina);

	t_disciplina disciplina;
	while(1) // loop para percorrer o arquivo
	{

		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;

		// verifica se os ID's são iguais
		if(disciplina.id == arq_atualizar->id)
		{
			// fseek posiciona o arquivo
			fseek(arq_disciplina, sizeof(t_disciplina), SEEK_CUR);
			// atualiza a disciplina
			fwrite(arq_atualizar, sizeof(t_disciplina), 1, arq_disciplina);
			break; // sai do loop
		}
	}
}
//função alterar disciplina
void alterar_disciplina()
{
	char str_id_professor[10];
	int id_professor;

	// rb+ abre para leitura/atualização
	FILE *arq_disciplina = fopen("disciplina.bin", "rb+");
	FILE *arq_professor = fopen("professor.bin", "rb+");

	// se não conseguiu abrir, então cria o arquivo
	// wb+ abre para escrita/atualização (cria o arquivo se ele NÃO existir)
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

	// obtém o ID do cliente
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
			char str_id_disciplina[10];
			int id_disciplina;

			printf("\nDigite o ID da disciplina: ");
			scanf("%10s%*c", str_id_disciplina);

			fseek(stdin, 0, SEEK_END); // não recomendável o uso

			if(str_somente_numeros(str_id_disciplina) == 1)
			{
				// se chegou aqui é porque o ID do professor é válido
				sscanf(str_id_disciplina, "%d", &id_disciplina);

				// obtém o disciplina pelo ID
				t_disciplina *disciplina = obter_disciplina(arq_disciplina, id_disciplina);

				// testa se a disciplina existe...
				if(disciplina != NULL)
				{		printf("\nDigite o novo nome da disciplina: ");
						scanf("%99[^\n]%*c", disciplina->nome);
						printf("\nDigite a nova descricao da disciplina: ");
						scanf("%99[^\n]%*c", disciplina->descricao);
						printf("\nDigite o novo numero de creditos da disciplina: ");
						scanf("%99[^\n]%*c", disciplina->num_creditos);
						printf("\nDigite a nova carga horaria da disciplina: ");
						scanf("%99[^\n]%*c", disciplina->carga_hora);
						atualizar_disciplina(arq_disciplina, disciplina); // atualiza a disciplina no arquivo

					free(disciplina); // libera a disciplina alocado
				}
				else
					printf("\nNao existe disciplina com o ID \"%d\".\n", id_disciplina);
			}
			else
				printf("\nO ID so pode conter numeros!\n");
		}
		else
			printf("\nNao existe professor com o ID \"%d\".\n", id_professor);
	}
	else
		printf("\nO ID so pode conter numeros!\n");

	// fecha os arquivos
	fclose(arq_professor);
	fclose(arq_disciplina);

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");

	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}



int existe_disciplina(FILE *arq_disciplina, int id_disciplina)
{
	// vai para o início do arquivo, pois não sabemos a posição do ponteiro no arquivo
	rewind(arq_disciplina);

	t_disciplina disciplina;
	// loop para percorrer o arquivo
	// busca linear O(n), como o ID é crescente é possível fazer uma busca binária O(log(n))
	while(1)
	{

		// fread retorna o número de elementos lidos com sucesso
		size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

		// se for 0, é porque não há mais elemento, então sai do loop
		if(result == 0)
			break;

		// verifica se o ID é igual
		if(disciplina.id == id_disciplina)
			return 1;
	}

	// se chegou aqui é porque NÃO existe a disciplina, então retorna 0
	return 0;
}

//função excluir disciplina
void excluir_disciplina(){

char str_id_disciplina[10];
int id_disciplina;

	printf("\nDigite o ID da disciplina: ");
	scanf("%10s%*c", str_id_disciplina);

	fseek(stdin, 0, SEEK_END); // não recomendável o uso

	// verifica se str_id_disciplina só contém números
	if(str_somente_numeros(str_id_disciplina) == 1)
	{
		// se chegou aqui é porque o ID do disciplina é válido
		sscanf(str_id_disciplina, "%d", &id_disciplina);

		// rb abre para leitura (o arquivo deve existir)
		FILE *arq_disciplina = fopen("disciplina.bin", "rb");

		if(arq_disciplina == NULL)
		{
			printf("\nFalha ao abrir arquivo(s)!\n");
			exit(1); // aborta o programa
		}

		// verifica se o disciplina existe
		if(existe_disciplina(arq_disciplina, id_disciplina) == 1)
		{
			char nome_disciplina[MAX];
			// abre um novo arquivo temporário
			FILE *arq_temp = fopen("temp_disciplina.bin", "a+b");
			if(arq_temp == NULL)
			{
				printf("\nFalha ao criar arquivo temporario!\n");
				fclose(arq_disciplina);
				exit(1); // aborta o programa
			}
			rewind(arq_disciplina); // vai para o início do arquivo

			t_disciplina disciplina;
			while(1) // loop para percorrer o arquivo
			{

				// fread retorna o número de elementos lidos com sucesso
				size_t result = fread(&disciplina, sizeof(t_disciplina), 1, arq_disciplina);

				// se for 0, é porque não há mais elemento, então sai do loop
				if(result == 0)
					break;

				// só copia pro novo arquivo se for diferente
				if(disciplina.id != id_disciplina)
				{
					// escreve no arquivo temporário
					fwrite(&disciplina, sizeof(t_disciplina), 1, arq_temp);
				}
				else
					strcpy(nome_disciplina, disciplina.nome);
			}

			// antes de fazer operações de remover arquivo e renomear,
			// é preciso fechar os dois arquivos
			fclose(arq_disciplina);
			fclose(arq_temp);

			// depois de fechar o arquivo, então tentamos remover
			if(remove("disciplina.bin") != 0)
				printf("\nErro ao deletar o arquivo \"disciplina.bin\"\n");
			else
			{
				// renomeia o arquivo
				int r = rename("temp_disciplina.bin", "disciplina.bin");
				if(r != 0)
				{
					printf("\nPermissao negada para renomear o arquivo!\n");
					printf("Feche esse programa bem como o arquivo \"temp_disciplinabin\" e renomeie manualmente para \"disciplina.bin\"\n");
				}
				else
					printf("\nDisciplina \"%s\" removido com sucesso!\n", nome_disciplina);
			}
		}
		else
		{
			fclose(arq_disciplina);
			printf("\nNao existe disciplina com o ID \"%d\".\n", id_disciplina);
		}
	}
	else
		printf("\nO ID so pode conter numeros!\n");

	printf("\nPressione <Enter> para continuar...");
	scanf("%*c");
	fseek(stdin, 0, SEEK_END); // não recomendável o uso
}

// função responsável pela ligacao entre professor e disciplina
void professor_da_disciplina()
{
    char str_id_professor[10];
    int id_professor;

    // rb+ abre para leitura/atualização
    FILE *arq_disciplina = fopen("disciplina.bin", "rb+");
    FILE *arq_professor = fopen("professor.bin", "rb+");

    // se não conseguiu abrir, então cria o arquivo
    // wb+ abre para escrita/atualização (cria o arquivo se ele NÃO existir)
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

    fseek(stdin, 0, SEEK_END); // não recomendável o uso

    if(str_somente_numeros(str_id_professor) == 1)
    {
        // se caiu aqui é porque o ID possui somente números, então
        // preenche a variável "id_professor" com o valor de "str_id_professor"
        sscanf(str_id_professor, "%d", &id_professor);

        // verifica se o ID do professor existe
        if(existe_professor(arq_professor, id_professor))
        {
            char str_id_disciplina[10];
            int id_disciplina;

            printf("\nDigite o ID da disciplina: ");
            scanf("%10s%*c", str_id_disciplina);

            fseek(stdin, 0, SEEK_END); // não recomendável o uso

            if(str_somente_numeros(str_id_disciplina) == 1)
            {
                // se chegou aqui é porque o ID da disciplina é válido
                sscanf(str_id_disciplina, "%d", &id_disciplina);

                // obtém o professor pelo ID
                t_disciplina *disciplina = obter_disciplina(arq_disciplina, id_disciplina);

                // testa se a disciplina existe...
                if(disciplina != NULL)
                {
                    // se chegou aqui é porque a disciplina existe
                    // testa se a disciplina ja tem professor
                    if(disciplina->id_professor != -1)
                        printf("\nA disciplina \"%s\" ja e lecionada\n", disciplina->nome);
                    else
                    {
                        // se a disciplina não tem professor, então seta o
                        // id_professor da disciplina para associar ao professor
                        // a disciplina
                        disciplina->id_professor = id_professor;
                        atualizar_disciplina(arq_disciplina, disciplina); // atualiza a disciplina no arquivo
                        printf("\nDisciplina \"%s\" associada com sucesso com professor %d\n", disciplina->nome, disciplina->id_professor);
                    }
                    free(disciplina); // libera a disciplina alocada
                }
                else
                    printf("\nNao existe disciplina com o ID \"%d\".\n", id_disciplina);
            }
            else
                printf("\nO ID so pode conter numeros!\n");
        }
        else
            printf("\nNao existe professor com o ID \"%d\".\n", id_professor);
    }
    else
        printf("\nO ID so pode conter numeros!\n");

    // fecha os arquivos
    fclose(arq_professor);
    fclose(arq_disciplina);

    printf("\nPressione <Enter> para continuar...");
    scanf("%*c");

    fseek(stdin, 0, SEEK_END); // não recomendável o uso
}