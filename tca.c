#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gea.h"

#define MAX_USERS 6
#define MAX_CLUBES 20
#define MAX_RODADAS 38

struct clube
{
	char nome[30];
	char cidade[30];
	char sigla[5];
	char estadio[50];

}typedef TClube;

struct rodada
{
	TClube jogos[10][2];
	int resultado[10][2];
	int cartoes[10][4];
	//varaivel que marca se uma tabela esta ou não sendo preenchida
	//1 = sendo preenchida
	//2 = totalmente preenchida
	//0 = vazia
	int preenchida;
}typedef TRodada;

struct ranking
{
	char nome[30];
	int pontos;
	int jogos;
	int vitorias;
	int derrotas;
	int empates;
	int GP;
	int GC;
	int SG;
	float mGP;
	float mGC;
	float aproveitamento;
	int movimentacao;
	int vermelhos;
	int amarelos;
}typedef TRanking;

struct user
{
	char nome[30];
	char login[12];
	char senha[12];
	int admin;

}typedef TUsuario;



int menuAdmin(int posicao_user, TUsuario *usuarios);
int menuUser(int posicao_user, TUsuario *usuarios, TClube *clubes, TRodada *rodadas, TRanking *ranking);

int login(TUsuario *usuarios);
int searchuser(char *log, TUsuario *usuarios);

void cadastro(int admin, TUsuario *usuarios);
void remover(TUsuario *usuarios);
void incia_user(TClube *clubes, int op);
void modificar(int posicao_user, TUsuario *usuarios);
void listar(TUsuario *usuarios);

void continuar_cadastro_rodada(TRodada *rodadas, TClube *clubes);
void listar_clubes(TClube *clubes);
void organiza_vetor_times_livres(TClube *clubes, TClube *vet_clubes_usados, TClube *clubes_livres, TRodada *rodadas);
int busca_clubes(TClube *clubes, char *nome);
void listar_rodadas(TRodada *rodadas, int op_rodada);

void leitura_usuario(TUsuario *usuarios);
void leitura_clubes(TClube *clubes);
void leitura_rodadas(TRodada *rodadas);
void leitura_ranking(TRanking *ranking);

void salvar_tudo(TUsuario *usuarios, TClube *clubes, TRodada *rodadas, TRanking *ranking);

void organiza_ranking(TRodada *rodadas, TRanking *ranking, TClube *clubes);
void lista_ranking(TRanking *ranking);

void zerar_rodadas(TRodada *rodadas);
void gerar_rodada_aleatoria(TRodada *rodadas, TClube *clubes);
void gerar_torneio_aleatorio(TRodada *rodadas, TClube *clubes);


int main()
{
	FILE *user;
	char a;
	int adm = 1;
	int op;
	int retorno_login;
	int fechar_prog = 0;

	TUsuario usuarios[MAX_USERS];
	TClube clubes[MAX_CLUBES];
	TRodada rodadas[MAX_RODADAS];
	TRanking ranking[MAX_CLUBES];

	//UTILIZADO PARA GERAÇÃO DE VALORES ALEATÓRIOS
	srand(time(NULL));

	user = fopen("usuarios.txt", "a+");
	
	if(!user)
	{
		COR_VERMELHO
		printf("- Erro na abertura do arquivo! -");
 		exit(0); 
	}

	//inicializa o vetor de usuarios com valores padrão para cada tipo de variavel
	//para que elas não armazenem lixo de memória
	for(int i =0; i<MAX_USERS; i++){
		strcpy(usuarios[i].nome, "\0");
		strcpy(usuarios[i].login, "\0");
		strcpy(usuarios[i].senha, "\0");
		usuarios[i].admin = 0;
	}

	for(int i = 0; i<MAX_CLUBES; i++){
		strcpy(clubes[i].nome, "\0");
		strcpy(clubes[i].cidade, "\0");
		strcpy(clubes[i].sigla, "\0");
		strcpy(clubes[i].estadio, "\0");

	}

	for(int i = 0; i<MAX_RODADAS; i++){
		for(int j = 0; j<10; j++){
			for(int k = 0; k<2; k++){
				strcpy(rodadas[i].jogos[j][k].nome, "\0");
				strcpy(rodadas[i].jogos[j][k].cidade, "\0");
				strcpy(rodadas[i].jogos[j][k].sigla, "\0");
				strcpy(rodadas[i].jogos[j][k].estadio, "\0");
				rodadas[i].resultado[j][k] = 0;
				rodadas[i].preenchida = 0;

			}
			rodadas[i].cartoes[j][0] = 0;
			rodadas[i].cartoes[j][1] = 0;
			rodadas[i].cartoes[j][2] = 0;
			rodadas[i].cartoes[j][3] = 0;
		}
	}
	
	for(int i =0; i<MAX_CLUBES; i++){
		strcpy(ranking[i].nome, clubes[i].nome);
		ranking[i].pontos = 0;
		ranking[i].jogos = 0;
		ranking[i].vitorias = 0;
		ranking[i].derrotas = 0;
		ranking[i].empates = 0;
		ranking[i].GP = 0;
		ranking[i].GC = 0;
		ranking[i].SG = 0;
		ranking[i].mGP = 0;
		ranking[i].mGC = 0;
		ranking[i].aproveitamento = 0; 
		ranking[i].movimentacao = 0;
	}

	//CARREGA DADOS
	leitura_usuario(usuarios);
	leitura_clubes(clubes);
	leitura_rodadas(rodadas);
	leitura_ranking(ranking);

	do{
		while((a = getc(user) ) != EOF)
		{
			adm = 0;
		}
		//verifica se há adm cadastrado para entrar no menu
		if(adm == 0)
		{
			COR_AGUA
			printf("\t-----------------\n");
			COR_AGUA
			printf("\t- EFETUAR LOGIN -\n");
			COR_AGUA
			printf("\t-----------------\n\n");
			retorno_login = login(usuarios);
			//login irá retornar a posição no vetor do usuário logado

			if(retorno_login != -1)
			{
				if(usuarios[retorno_login].admin == 1)
				{
					fechar_prog = menuAdmin(retorno_login, usuarios);
				}
				else if(usuarios[retorno_login].admin == 0)
				{
					fechar_prog = menuUser(retorno_login, usuarios, clubes, rodadas, ranking);
				}	
			}
			//aqui, retorno_login verifica se o user é adm ou n para entrar em seu respectivo menu
			//se adm = 0, entra no menu adm, se for 1, entra no menu user
			//se for -1, quer dizer que deu erro no login
		}
		else
		{
			cadastro(adm, usuarios);
			salvar_tudo(usuarios, clubes, rodadas, ranking);
			fechar_prog = menuAdmin(retorno_login, usuarios);
		}
		//senão, ele entra na aba de cadastro

	}while(fechar_prog == 0);

	//SALVA DADOS
	salvar_tudo(usuarios, clubes, rodadas, ranking);
}

int menuAdmin(int posicao_user, TUsuario *usuarios)
{
	int opcao;
	do{
		system("clear");
		COR_VERDE
		printf("- Olá, seja bem-vindo: %s\n", usuarios[0].nome);
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------ 1 ➜ Cadastrar usuário -------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------ 2 ➜ Visualizar usuário ------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------- 3 ➜ Remover usuário --------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------- 4 ➜ Efetuar Logoff ---------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------------ 5 ➜ Sair --------------\n");
		COR_VERDE
		printf("------------------------------------\n");
		scanf("%d", &opcao);

		if(opcao == 1)
		{
			cadastro(0, usuarios);
		}
		else if(opcao == 2)
		{
			listar(usuarios);
		}
		else if(opcao == 3)
		{
			remover(usuarios);

		}
		else if(opcao == 4)
		{
			return 0;
			//pois se fechar_prog == 0, ele irá deslogar
		}
		else if(opcao == 5)
		{
			return 1;
		}
		else
		{
			COR_VERMELHO
			printf("- ERRO! Opção inválida! -\n");
		}
	}while(opcao != 5);
}

int menuUser(int posicao_user, TUsuario *usuarios, TClube *clubes, TRodada *rodadas, TRanking *ranking)
{
	int opcao;
	char continuar;
	int flag = 0;
	int op_rodada;

	do{
		system("clear");
		COR_VERDE
		printf("- Olá, seja bem-vindo: %s\n", usuarios[posicao_user].nome);
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------ 1 ➜ Iniciar o Usuário -------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("----------- 2 ➜ Continuar ----------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------- 3 ➜ Modificar Dados --------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("--------- 4 ➜ Configurações ---------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("----------- 5 ➜ Ranking ------------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------- 6 ➜ Efetuar Logoff ---------\n");
		COR_VERDE
		printf("------------------------------------\n");
		COR_VERDE
		printf("------------ 7 ➜ Sair --------------\n");
		COR_VERDE
		printf("------------------------------------\n");

		scanf("%d", &opcao);

		if(opcao == 1)
		{
			if(strlen(clubes[0].nome) != 0){
				do{
					system("clear");
					COR_VERMELHO
					printf("\n- Esta ação removerá todos os times já cadastrados -\n");
					printf("\n- Tem certeza que deseja fazer isso? -\n");

					COR_ROXO
					printf("\n- Sim = s/S\tNão = n/N -\n");
					scanf("%c", &continuar);

					if(continuar == 'n' || continuar == 'N')
					{
						system("clear");
						COR_AGUA
						printf("Cadastro de clubes cancelado.\n");
						sleep(1);
						flag = 1;
					}
					else if(continuar == 's' || continuar == 'S')
					{
						incia_user(clubes, opcao);
						flag = 1;
					}
					// else
					// {
					// 	COR_VERMELHO
					// 	system("clear");
					// 	printf("[ERRO] Opção inválida.\n");
					// 	sleep(1);
					// }
				}while(flag == 0);
				flag = 0;
			}
			else
			{
				incia_user(clubes, opcao);
			}
		}
		else if(opcao == 2)
		{
			incia_user(clubes, opcao);
			//mesma função, pois ele pergunta se quer cadastrar mais a cada término de cadastro
		}
		else if(opcao == 3)
		{
			modificar(posicao_user, usuarios); 
		}
		else if(opcao == 4)
		{
			do{
				system("clear");
				COR_VERDE
				printf("------------------------------------\n");
				printf("---- 1 ➜ Continuar cadastrando -----\n");
				printf("------------------------------------\n");
				printf("- 2 ➜ Gerar rodadas aleatóriamente -\n");
				printf("------------------------------------\n");
				printf("-------- 3 ➜ Zerar rodadas ---------\n");
				printf("------------------------------------\n");
				printf("--------- 4 ➜ Ver rodadas ----------\n");
				printf("------------------------------------\n");
				printf("------------ 5 ➜ Voltar ------------\n");
				printf("------------------------------------\n");
				COR_ROXO
				scanf("%d", &opcao);

				if(opcao == 1)
				{
					continuar_cadastro_rodada(rodadas, clubes);
					organiza_ranking(rodadas, ranking, clubes);
				}
				else if(opcao == 2)
				{
					do{
						COR_VERDE
						printf("------------------------------------\n");
						printf("------- 1 ➜ Gerar uma rodada -------\n");
						printf("------------------------------------\n");
						printf("---- 2 ➜ Gerar todas as rodadas ----\n");
						printf("------------------------------------\n");
						printf("------------ 3 ➜ Voltar ------------\n");
						printf("------------------------------------\n");
						scanf("%d", &opcao);

						if(opcao == 1)
						{
							gerar_rodada_aleatoria(rodadas, clubes);
						}
						else if(opcao == 2)
						{
							do{
								printf("Esta ação irá remover todas as rodadas já cadastradas!\n");
								printf("Tem certeza que deseja continuar?\n");
								COR_ROXO
								printf("\n- Sim = s/S\tNão = n/N -\n");
								__fpurge(stdin);
								scanf("%c", &continuar);

								if(continuar == 'n' || continuar == 'N')
								{
									system("clear");
									COR_AGUA
									printf("Ação cancelada.\n");
									sleep(1);
								}
								else if(continuar == 's' || continuar == 'S')
								{
									if(strlen(clubes[MAX_CLUBES].nome) != 0)
									{
										gerar_torneio_aleatorio(rodadas, clubes);
									}
									else
									{
										system("clear");
										COR_VERMELHO
										printf("[ERRO] Todos os clubes devem estar cadastrados!\n");
										sleep(1);
									}
								}
								else
								{	
									system("clear");
									COR_VERMELHO
									printf("[ERRO] Opção inválida!\n");
									sleep(1);
								}
							}while(continuar != 's' && continuar == 'S' && continuar != 'n' && continuar == 'N');
							
						}
						else if(opcao == 3)
						{
							//recebe -1 para sair da repetição sem interferir a repetição anterior
							opcao = -1;
						}
						else
						{
							system("clear");
							COR_VERMELHO
							printf("[ERRO] Opção inválida!\n");
							sleep(1);
						}

					}while(opcao != -1);
				}
				else if(opcao == 3)
				{
					zerar_rodadas(rodadas);
				}
				else if(opcao == 4)
				{
					do{
						COR_VERDE
						system("clear");
						printf("Informe a rodada desejada\n(Digite 0 para listar todas):\n");
						scanf("%d", &op_rodada);

						if(op_rodada <0 && op_rodada > 38){
							system("clear");
							COR_VERMELHO
							printf("[ERRO] Opção inválida!\n");
							sleep(1);
						}

					}while(op_rodada <-1 && op_rodada > 38);
					listar_rodadas(rodadas, op_rodada);
				}
				else if(opcao == 5)
				{
					//recebe -2 para sair da repetição sem interferir a repetição anterior
					opcao = -2;
				}
				else
				{
					system("clear");
					COR_VERMELHO
					printf("[ERRO] Opção inválida!\n");
					sleep(1);
				}

			}while(opcao != -2);
			
		}
		else if(opcao == 5)
		{
			if(rodadas[0].preenchida != 0)
			{
				organiza_ranking(rodadas, ranking, clubes);
				lista_ranking(ranking);
			}
			else
			{
				system("clear");
				COR_VERMELHO
				printf("[ERRO] Nenhuma rodada cadastrada!\n");
				sleep(1);
			}
			
		}
		else if(opcao == 6)
		{
			return 0;
		}
		else if(opcao == 7)
		{
			return 1;
		}
		else
		{
			COR_VERMELHO
			printf("- ERRO! Opção inválida! -\n");
		}

	}while(opcao != 7);
}

void cadastro(int admin, TUsuario *usuarios)
{
	char senha1[12];
	int flag = 0;
	int posicao_atual = 0;

	TUsuario usuario;
	
	for(int i = 0; i < MAX_USERS; i++){
		if(strlen(usuarios[i].nome) != 0)
		{
			posicao_atual++;
		}
		//verifica se, se sim, incrementa a quantidade de cadastros
		//o strlen verifica se a posição do vetor está vaga
	}

	system("clear");
	COR_ROXO
	printf("Quantidade atual de cadastros: %d\n", posicao_atual);

	if(posicao_atual < 6)
	{
		COR_VERDE
		printf("\n- Insira o seu nome:\n\n");

		//NOME
		do{
			__fpurge(stdin);
			gets(usuarios[posicao_atual].nome);

			if(strlen(usuarios[posicao_atual].nome) < 3){
				COR_VERMELHO
				printf("[Nome muito curto]\n");
				COR_VERDE
			}

		}while(strlen(usuarios[posicao_atual].nome) < 3);

		//LOGIN
		do{
			COR_VERDE
			printf("\n- Insira o seu login:\n");
			__fpurge(stdin);
			gets(usuarios[posicao_atual].login);
			__fpurge(stdin);

			if(searchuser(usuarios[posicao_atual].login, usuarios) == 1)
			{
				flag = 1;
			}
			else if(strlen(usuarios[posicao_atual].login) < 3)
			{
				COR_VERMELHO
				printf("[Login muito curto]\n");
				COR_VERDE
			}
			else
			{
				COR_VERMELHO
				printf("- Desculpe, esse login já está sendo utilizado no momento! -\n");
				flag = 0;
			}

		}while(flag == 0 && strlen(usuarios[posicao_atual].login) < 3);
		//se o retorno for 0, irá pedir para o usuário digitar outro login

		//SENHA
		do
		{
			COR_VERDE
			printf("- Digite sua senha:\n");
			__fpurge(stdin);
			gets(usuarios[posicao_atual].senha);
			printf("\n- Confirme a senha:\n");
			__fpurge(stdin);
			gets(senha1);

		}while(strcmp(senha1, usuarios[posicao_atual].senha) != 0);
		//confere se a senha digitada está correta

		if(admin == 1)
		{
			usuarios[posicao_atual].admin = 1;
		}
		else
		{
			usuarios[posicao_atual].admin = 0;
		}
		//o cadastro no arquivo que possuir '1', é do admin

		COR_VERDE
		printf("[Cadastro concluido com sucesso]\n");
		sleep(1);
	}
	else
	{
		COR_VERMELHO
		sleep(1);
		printf("[ERRO] Limite de usuários atingido!\n");

	}
}

int login(TUsuario *usuarios)
{
	char login1[12], senha1[12];
	char c, var[100];
	int i = 0, CB = 0, flag = 0;
	int posicao_user = 0;

	TUsuario usuario;

	COR_VERDE
	printf("- Digite o seu login:\n");
	__fpurge(stdin);
	gets(login1);

	COR_VERDE
	printf("\n- Digite sua senha:\n");
	__fpurge(stdin);
	gets(senha1);

	for(int i = 0; i < MAX_USERS; i++)
	{
		if(strcmp(login1, usuarios[i].login) == 0 && strcmp(senha1, usuarios[i].senha) == 0)
		{
			flag = 1;
			posicao_user = i;
		}
		//verifica se o login e senha digitados pertencem a algum dos salvos 
	}

	if(flag == 1)
	{
		system("clear");
		COR_ROXO
		printf("Entrando...\n");
		sleep(1);
		system("clear");
		return posicao_user;
	}
	else
	{
		system("clear");
		COR_VERMELHO
		printf("- ERRO! -\n- Login não encontrado! -\n");
		sleep(1);
		system("clear");
		return -1;
	}
}

/////////////////////////////////////
//////FUNÇÕES REFERENTES A CLUBES////
/////////////////////////////////////

void leitura_clubes(TClube *clubes)
{
	FILE *fclubes;
	fclubes = fopen("clubes.txt", "a+");
	char c, var[100];
	int i = 0, CB = 0, cont_linha = 0;

	while(!feof(fclubes))
	{
		c = fgetc(fclubes);

		if(c != '|' && c != '\n')
		{
			var[i] = c;
			i++;
		}
		else if(c == '|')
		{
			if(CB == 0)
			{
				var[i] = '\0';
				strcpy(clubes[cont_linha].nome, var);
				CB++;
				i = 0;
			}
			else if(CB == 1)
			{
				var[i] = '\0';
				strcpy(clubes[cont_linha].cidade, var);
				CB++;
				i = 0;
			}
			else if(CB == 2)
			{
				var[i] = '\0';
				strcpy(clubes[cont_linha].sigla, var);
				CB++;
				i = 0;
			}
			else if(CB == 3)
			{
				var[i] = '\0';
				strcpy(clubes[cont_linha].estadio, var);
				CB++;
				i = 0;
			}
			//basicamente, a cada barra, quer dizer que é um novo atributo sendo escrito
			//até a próxima barra
		}
		else if(c == '\n')
		{
			i = 0;
			CB = 0;
			cont_linha++;
		}
		//se for uma quebra de linha, ele incrimenda o contador de linhas cadastradas
		//por isso retorna esse número
	}

	fclose(fclubes);
}

//a variavel OP define se o usuário deseja continuar os cadastros ou iniciar uma nova lista de cadastros
void incia_user(TClube *clubes, int op)
{	
	char continuar;
	int qtd_cadastro = 0;
	int posicao_atual = 0;
	char nome[50];

	//se op for 1 (inicia usuário) então o vetor de clubes é zerado
	if(op == 1){
		for(int i = 0; i<MAX_CLUBES; i++)
		{
			strcpy(clubes[i].nome, "\0");
			strcpy(clubes[i].cidade, "\0");
			strcpy(clubes[i].sigla, "\0");
			strcpy(clubes[i].estadio, "\0");
		}
	}
	//se op for 2 (continuar cadastros) então o buscamos a ultima posção utilizada para continuar os cadastros
	else if(op == 2){

		for(int i = 0; i < MAX_CLUBES; i++)
		{
			if(strlen(clubes[i].nome) != 0)
			{
				qtd_cadastro++;
			}
			//o strlen verifica se a posição do vetor está vaga
			//por isso incrementa 1 cadastro se não for vaga
		}
	}
		
	system("clear");
	printf("Quantidade atual de cadastros: %d\n", qtd_cadastro);

	if(qtd_cadastro < 20)
	{
		for(int i = qtd_cadastro; i < MAX_CLUBES; i++){

			COR_AGUA
			printf("\t---------------------------\n");
			printf("\t----- CADASTRAR CLUBE -----\n");
			printf("\t---------------------------\n");

			
			
			//verifica se não tem clubes com o mesmo nome
			do{
				COR_VERDE
				printf("- Insira o nome do clube:\n");
				__fpurge(stdin);
				gets(nome);
				
				if(busca_clubes(clubes, nome) == 1){
					COR_VERMELHO
					printf("- ERRO! -\n- Nome de clube ja utilizado! -\n");
				}
			}while(busca_clubes(clubes, nome) != 0);

			strcpy(clubes[i].nome, nome);


			printf("- Insira a cidade do clube:\n");
			__fpurge(stdin);
			gets(clubes[i].cidade);

			do{
				printf("- Insira o sigla do estado do clube:\n");
				__fpurge(stdin);
				gets(clubes[i].sigla);

			}while(strlen(clubes[i].sigla) > 2);
			
			
			printf("- Insira o nome do estádio clube:\n");
			__fpurge(stdin);
			gets(clubes[i].estadio);

			do{
				COR_ROXO
				printf("\n- Deseja cadastrar mais um time? -\n");
				printf("\n- Sim = s/S\tNão = n/N -\n");
				scanf("%c", &continuar);

				if(continuar == 'n' || continuar == 'N')
				{
					i = MAX_CLUBES + 1;
					//quebra a repetição se o user n quiser cadastrar mais
				}

			}while(continuar == 'n' || continuar == 'N');
		}
	}
	else{
		COR_VERMELHO
		printf("[ERRO] Limite de clubes atingido!\n");
		sleep(2);

	}
}

int busca_clubes(TClube *clubes, char *nome){

	for(int i = 0; i < MAX_CLUBES; i++)
	{
		if(strcmp(nome, clubes[i].nome) == 0)
		{
			return 1;
		}
	}
	return 0;
}

///////////////////////////////////////
//////FUNÇÕES REFERENTES A USUARIOS////
///////////////////////////////////////

int searchuser(char *log, TUsuario *usuarios)
{

	for(int i =0; i < MAX_USERS; i++)
	{
		if(strcmp(log, usuarios[i].login) == 0)
		{
			return 1;
		}
		//verifica se não há usuário com o mesmo login
	}
	return 0;
	//retorna 0 se o login já estiver sendo utilizado
}

void listar(TUsuario *usuarios)
{
	char sair;
	
	do{
		system("clear");
		COR_AGUA
		printf("\t---------------------------------\n");
		COR_AGUA
		printf("\t- LISTA DE USUÁRIOS CADASTRADOS -\n");
		COR_AGUA
		printf("\t---------------------------------\n");

		for(int i = 0; i < MAX_USERS; i++)
		{
			if(strlen(usuarios[i].nome) != 0)
			{
				COR_VERDE
				printf("-------------------------------------------------\n");
				printf("|- Nome do usuário: %s\t\t\t\t|\n", usuarios[i].nome);
				printf("|- Login do usuário: %s\t\t\t|\n", usuarios[i].login);
				printf("|- Senha do usuário: %s\t\t\t|\n", usuarios[i].senha);

				if(i == 0)
				{
					printf("|- Cadastro de: administrador -\t\t\t|\n");
				}
				else
				{
					printf("|- Cadastro de: usuário -\t\t\t|\n");
				}
				printf("-------------------------------------------------\n\n");
				//o strlen verifica se a posição do vetor está vaga
				//por isso ele lista todas as posições não nulas do vetor
			}	
		}

	
		COR_ROXO
		printf("\n- Deseja voltar ao menu? -\n");
		COR_ROXO
		printf("\n- Sim = s/S\tNão = n/N -\n");
		__fpurge(stdin);
		scanf("%c", &sair);

	}while(sair == 'n' || sair == 'N');
}

void modificar(int posicao_user, TUsuario *usuarios)
{
	char novoNome[30];
	char novaSenha[12];
	int qtd_cadastro = 0;
	int posicao_atual = 0;

	__fpurge(stdin);
	printf("Insira o novo nome do usuário:\n");
	gets(novoNome);

	__fpurge(stdin);
	printf("Insira sua nova senha:\n");
	gets(novaSenha);

	strcpy(usuarios[posicao_user].nome, novoNome);
	strcpy(usuarios[posicao_user].senha, novaSenha);

}

void remover(TUsuario *usuarios){
	int flag, j = 0;
	char login[12];
	TUsuario aux[6];

	for(int i =0; i<MAX_USERS; i++)
	{
		strcpy(aux[i].nome, "\0");
		strcpy(aux[i].login, "\0");
		strcpy(aux[i].senha, "\0");
		aux[i].admin = 0;
	}

	printf("Informe o login do usuário:\n");
	__fpurge(stdin);
	gets(login);

	system("clear");
	if(searchuser(login, usuarios) != 0 && strcmp(login, usuarios[0].nome) != 0){
		for(int i = 0; i < MAX_USERS; i++)
		{
			if(strcmp(usuarios[i].login, login) != 0)
			{
				strcpy(aux[j].nome, usuarios[i].nome);
				strcpy(aux[j].login, usuarios[i].login);
				strcpy(aux[j].senha, usuarios[i].senha);
				aux[j].admin = usuarios[i].admin;

				strcpy(usuarios[j].nome, aux[j].nome);
				strcpy(usuarios[j].login, aux[j].login);
				strcpy(usuarios[j].senha, aux[j].senha);
				usuarios[j].admin = aux[j].admin;
				j++;
			}
			else{
				strcpy(usuarios[i].nome, "\0");
				strcpy(usuarios[i].login, "\0");
				strcpy(usuarios[i].senha, "\0");
				usuarios[i].admin = 0;
			}
		}

		COR_VERDE
		system("clear");
		printf("\n[SUCESSO] Usuário removido.\n");
		sleep(2);
		COR_AGUA
	}
	else if(strcmp(login, usuarios[0].nome) == 0)
	{
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Impossível remover o admin!\n");
		sleep(2);
		COR_AGUA
	}
	else
	{
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Login não encontrado!\n");
		sleep(2);
		COR_AGUA
	}
}

//SALVA TODOS OS DADOS (AO SAIR DO PROGRAMA CORRETAMENTE)
void salvar_tudo(TUsuario *usuarios, TClube *clubes, TRodada *rodadas, TRanking *ranking){
	FILE *fusers = fopen("usuarios.txt", "w");
	FILE *fclubes = fopen("clubes.txt", "w");
	FILE *frodadas = fopen("rodadas.txt", "w");
	FILE *franking = fopen("ranking.txt", "w");

	for(int i = 0; i<MAX_USERS; i++){
		if(strlen(usuarios[i].nome) != 0){
			fprintf(fusers,"%s|%s|%s|%d|\n", usuarios[i].nome, usuarios[i].login, usuarios[i].senha, usuarios[i].admin);
		}
	}

	for(int i = 0; i<MAX_CLUBES; i++){
		if(strlen(clubes[i].nome) != 0){
			fprintf(fclubes ,"%s|%s|%s|%s|\n", clubes[i].nome, clubes[i].cidade, clubes[i].sigla, clubes[i].estadio);
		}
	}
	for(int i = 0; i<MAX_RODADAS; i++)
	{
		for(int j = 0; j <10; j++)
		{
			if(strlen(rodadas[i].jogos[j][0].nome) != 0)
			{
				fprintf(frodadas ,"%d|%s|%s|%s|%s|%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|\n", rodadas[i].preenchida, rodadas[i].jogos[j][0].nome, rodadas[i].jogos[j][0].cidade, rodadas[i].jogos[j][0].sigla, rodadas[i].jogos[j][0].estadio, rodadas[i].jogos[j][1].nome, rodadas[i].jogos[j][1].cidade, rodadas[i].jogos[j][1].sigla, rodadas[i].jogos[j][1].estadio, rodadas[i].resultado[j][0], rodadas[i].resultado[j][1], rodadas[i].cartoes[j][0], rodadas[i].cartoes[j][1], rodadas[i].cartoes[j][2], rodadas[i].cartoes[j][3]);
			}
		}
	}

	for(int i = 0; i < MAX_CLUBES; i++)
	{
		if(strlen(ranking[i].nome) != 0)
		{
			fprintf(franking, "%s|%d|%d|%d|%d|%d|%d|%d|%d|%.2f|%.2f|%.2f|%d|\n", ranking[i].nome, ranking[i].pontos, ranking[i].jogos, ranking[i].vitorias, ranking[i].empates, ranking[i].derrotas, ranking[i].GP, ranking[i].GC, ranking[i].SG, ranking[i].mGP, ranking[i].mGC, ranking[i].aproveitamento, ranking[i].movimentacao);
		}
	}

	fclose(franking);
	fclose(frodadas);
	fclose(fusers);
	fclose(fclubes);
}

void leitura_rodadas(TRodada *rodadas){
	FILE *frodadas;
	frodadas = fopen("rodadas.txt", "a+");
	char c, var[300];
	int i = 0, CB = 0, cont_linha = 0, cont_rodadas = 0, cont_jogos = 0;

	while(!feof(frodadas))
	{
		c = fgetc(frodadas);

		if(c != '|' && c != '\n')
		{
			var[i] = c;
			i++;
		}
		else if(c == '|')
		{
			if(CB == 0){
				rodadas[cont_rodadas].preenchida = atoi(var);
				var[i] = '\0';
				CB++;
				i = 0;
			}
			else if(CB == 1)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][0].nome, var);
				CB++;
				i = 0;
			}
			else if(CB == 2)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][0].cidade, var);
				CB++;
				i = 0;
			}
			else if(CB == 3)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][0].sigla, var);
				CB++;
				i = 0;
			}
			else if(CB == 4)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][0].estadio, var);
				CB++;
				i = 0;
			}
			else if(CB == 5)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][1].nome, var);
				CB++;
				i = 0;
			}
			else if(CB == 6)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][1].cidade, var);
				CB++;
				i = 0;
			}
			else if(CB == 7)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][1].sigla, var);
				CB++;
				i = 0;
			}
			else if(CB == 8)
			{
				var[i] = '\0';
				strcpy(rodadas[cont_rodadas].jogos[cont_jogos][1].estadio, var);
				CB++;
				i = 0;
			}
			else if(CB == 9)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].resultado[cont_jogos][0] = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 10)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].resultado[cont_jogos][1] = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 11)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].cartoes[cont_jogos][0] = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 12)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].cartoes[cont_jogos][1] = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 13)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].cartoes[cont_jogos][2] = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 14)
			{
				var[i] = '\0';
				rodadas[cont_rodadas].cartoes[cont_jogos][3] = atoi(var);
				CB++;
				i = 0;
				cont_jogos++;
			}
			//basicamente, a cada barra, quer dizer que é um novo atributo sendo escrito
			//até a próxima barra
		}
		else if(c == '\n')
		{
			i = 0;
			CB = 0;
			cont_linha++;
			strcpy(var, "\0");

			if(cont_jogos % 10 == 0){
				cont_rodadas++;
				cont_jogos = 0;
			}
		}
		//a cada 10 jogos, inicia uma nova rodada
	}	
}

void leitura_ranking(TRanking *ranking)
{
	FILE *franking;
	franking = fopen("ranking.txt", "a+");
	char c, var[100];
	int i = 0, CB = 0, cont_linha = 0;

	while(!feof(franking))
	{
		c = fgetc(franking);

		if(c != '|' && c != '\n')
		{
			var[i] = c;
			i++;
		}
		else if(c == '|')
		{
			if(CB == 0)
			{
				var[i] = '\0';
				strcpy(ranking[cont_linha].nome, var);
				CB++;
				i = 0;
			}
			else if(CB == 1)
			{
				var[i] = '\0';
				ranking[cont_linha].pontos = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 2)
			{
				var[i] = '\0';
				ranking[cont_linha].jogos = atoi(var);;
				CB++;
				i = 0;
			}
			else if(CB == 3)
			{
				var[i] = '\0';
				ranking[cont_linha].vitorias = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 4)
			{
				var[i] = '\0';
				ranking[cont_linha].empates = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 5)
			{
				var[i] = '\0';
				ranking[cont_linha].derrotas = atoi(var);;
				CB++;
				i = 0;
			}
			else if(CB == 6)
			{
				var[i] = '\0';
				ranking[cont_linha].GP = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 7)
			{
				var[i] = '\0';
				ranking[cont_linha].GC = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 8)
			{
				var[i] = '\0';
				ranking[cont_linha].SG = atoi(var);
				CB++;
				i = 0;
			}
			else if(CB == 9)
			{
				var[i] = '\0';
				ranking[cont_linha].mGP = atof(var);
				CB++;
				i = 0;
			}
			else if(CB == 10)
			{
				var[i] = '\0';
				ranking[cont_linha].mGC = atof(var);
				CB++;
				i = 0;
			}
			else if(CB == 11)
			{
				var[i] = '\0';
				ranking[cont_linha].aproveitamento = atof(var);
				CB++;
				i = 0;
			}
			else if(CB == 12)
			{
				var[i] = '\0';
				ranking[cont_linha].movimentacao = atoi(var);
				CB++;
				i = 0;
			}
			//basicamente, a cada barra, quer dizer que é um novo atributo sendo escrito
			//até a próxima barra
		}
		else if(c == '\n')
		{
			i = 0;
			CB = 0;
			cont_linha++;
		}
		//se for uma quebra de linha, ele incrimenda o contador de linhas cadastradas
		//por isso retorna esse número
	}

	fclose(franking);
}

void leitura_usuario(TUsuario *usuarios)
{
	FILE *user;
	user = fopen("usuarios.txt", "a+");
	char c, var[100];
	int i = 0, CB = 0, cont_linha = 0;

	TUsuario usuario;

	while(!feof(user))
	{
		c = fgetc(user);

		if(c != '|' && c != '\n')
		{
			var[i] = c;
			i++;
		}
		else if(c == '|')
		{
			if(CB == 0)
			{
				var[i] = '\0';
				strcpy(usuario.nome, var);
				CB++;
				i = 0;
			}
			else if(CB == 1)
			{
				var[i] = '\0';
				strcpy(usuario.login, var);
				CB++;
				i = 0;
			}
			else if(CB == 2)
			{
				var[i] = '\0';
				strcpy(usuario.senha, var);
				CB++;
				i = 0;
			}
			else if(CB == 3)
			{
				var[i] = '\0';
				usuario.admin = atoi(var);
				CB++;
				i = 0;
			}
			//basicamente, a cada barra, quer dizer que é um novo atributo sendo escrito
			//até a próxima barra
		}
		else if(c == '\n')
		{
			i = 0;
			CB = 0;

			strcpy(usuarios[cont_linha].nome, usuario.nome);
			strcpy(usuarios[cont_linha].login, usuario.login);
			strcpy(usuarios[cont_linha].senha, usuario.senha);

			usuarios[cont_linha].admin = usuario.admin;
			cont_linha++;
		}
		//se for uma quebra de linha, ele incrimenda o contador de linhas cadastradas
		//por isso retorna esse número
	}

	fclose(user);
}


/////////////////////////////////////
/////FUNÇÕES REFERENTES A RODADAS////
/////////////////////////////////////

void continuar_cadastro_rodada(TRodada *rodadas, TClube *clubes)
{
	TClube clubes_livres[MAX_CLUBES], vet_clubes_usados[MAX_CLUBES];
	int qtd_jogos = 0, qtd_rodadas = 0, flag = 0, op_clube, placar, placar2, amarelos = 0, vermelhos = 0;
	char continuar;

	if(rodadas[MAX_RODADAS-19].preenchida != 2){
		do{
			do{
				system("clear");
				COR_ROXO
				organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);
				listar_clubes(clubes_livres);
				COR_VERDE
				printf("Selecione o clube mandante:\n");
				scanf("%d", &op_clube);
				printf("Informe o placar do time mandante:\n");
				scanf("%d", &placar);
				printf("Informe a quantidade de cartões amarelos do time mandante:\n");
				scanf("%d", &amarelos);
				printf("Informe a quantidade de cartões vermelhos do time mandante:\n");
				scanf("%d", &vermelhos);

				if(strlen(clubes_livres[op_clube].nome) == 0){
					COR_VERMELHO
					system("clear");
					printf("[ERRO] Opção de clube inválida\n");
					sleep(2);
				}
				else{
					for(int i = 0; i<MAX_RODADAS; i++){
						if(rodadas[i].preenchida != 2){
							for(int j = 0; j < 10; j++){
								for(int k = 0; k < 2; k++){
									if(strlen(rodadas[i].jogos[j][k].nome) == 0){
										strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
										strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
										strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
										strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
										rodadas[i].resultado[j][k] = placar;
										rodadas[i].cartoes[j][0] = amarelos;
										rodadas[i].cartoes[j][1] = vermelhos;
										rodadas[i].preenchida = 1;
										i = MAX_RODADAS+1;
									}
								}
							}
						}
					}			
				}
			}while(strlen(clubes_livres[op_clube].nome) == 0);

			do{
				system("clear");
				COR_ROXO
				organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);
				listar_clubes(clubes_livres);
				COR_VERDE
				printf("Selecione o clube visitante:\n");
				scanf("%d", &op_clube);
				printf("Informe o placar do time visitante:\n");
				scanf("%d", &placar);
				printf("Informe a quantidade de cartões amarelos do time visitante:\n");
				scanf("%d", &amarelos);
				printf("Informe a quantidade de cartões vermelhos do time visitante:\n");
				scanf("%d", &vermelhos);

				if(strlen(clubes_livres[op_clube].nome) == 0){
					COR_VERMELHO
					system("clear");
					printf("[ERRO] Opção de clube inválida\n");
					sleep(2);
				}
				else{
					for(int i = 0; i<MAX_RODADAS; i++){
						if(rodadas[i].preenchida != 2){
							for(int j = 0; j < 10; j++){
								for(int k = 0; k < 2; k++){
									if(strlen(rodadas[i].jogos[j][k].nome) == 0){
										strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
										strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
										strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
										strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
										rodadas[i].resultado[j][k] = placar;
										rodadas[i].cartoes[j][2] = amarelos;
										rodadas[i].cartoes[j][3] = vermelhos;
										qtd_jogos = j;
										i = MAX_RODADAS+1;
	
									}
									if(j == 9){
										rodadas[i].preenchida = 2;
									}
								}
							}
						}
					}			
				}
			}while(strlen(clubes_livres[op_clube].nome) == 0);
			
			//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
			//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
			if(qtd_jogos != 9 && strlen(clubes_livres[1].nome) != 0){
				do{
					COR_ROXO
					printf("\n- Deseja cadastrar mais um jogo? -\n");
					printf("\n- Sim = s/S\tNão = n/N -\n");
					__fpurge(stdin);
					scanf("%c", &continuar);
				}while(continuar != 'n' && continuar != 'N' && continuar != 's' && continuar != 'S');
			}
			else{
				continuar = 'n';
			}
		}while(continuar != 'n' && continuar != 'N');
	}
	else if(rodadas[MAX_RODADAS-19].preenchida == 2 && rodadas[MAX_RODADAS-1].preenchida != 2){
		do{
			for(int i = 0; i<MAX_RODADAS; i++){
				if(rodadas[i].preenchida != 2){
					for(int j = 0; j < 10; j++){
						if(strlen(rodadas[i].jogos[j][0].nome) == 0){
							strcpy(rodadas[i].jogos[j][0].nome, rodadas[i-19].jogos[j][1].nome);
							strcpy(rodadas[i].jogos[j][0].cidade, rodadas[i-19].jogos[j][1].cidade);
							strcpy(rodadas[i].jogos[j][0].sigla, rodadas[i-19].jogos[j][1].sigla);
							strcpy(rodadas[i].jogos[j][0].estadio, rodadas[i-19].jogos[j][1].estadio);
							printf("Informe o placar do clube mandante (%s):\n", rodadas[i].jogos[j][0].nome);
							scanf("%d", &rodadas[i].resultado[j][0]);
							printf("Informe a quantidade de cartões amarelos do time mandante:\n");
							scanf("%d", &rodadas[i].cartoes[j][0]);
							printf("Informe a quantidade de cartões vermelhos do time mandante:\n");
							scanf("%d", &rodadas[i].cartoes[j][1]);

							strcpy(rodadas[i].jogos[j][1].nome, rodadas[i-19].jogos[j][0].nome);
							strcpy(rodadas[i].jogos[j][1].cidade, rodadas[i-19].jogos[j][0].cidade);
							strcpy(rodadas[i].jogos[j][1].sigla, rodadas[i-19].jogos[j][0].sigla);
							strcpy(rodadas[i].jogos[j][1].estadio, rodadas[i-19].jogos[j][0].estadio);
							printf("Informe o placar do clube visitante (%s):\n", rodadas[i].jogos[j][1].nome);
							scanf("%d", &rodadas[i].resultado[j][1]);
							printf("Informe a quantidade de cartões amarelos do time visitante:\n");
							scanf("%d", &rodadas[i].cartoes[j][2]);
							printf("Informe a quantidade de cartões vermelhos do time visitante:\n");
							scanf("%d", &rodadas[i].cartoes[j][3]);
							qtd_jogos = j;
							rodadas[i].preenchida = 1;

							//VERIFICA SE OS 10 JOGOS JA FORAM PREENCHIDOS
							//SE ELES JA TIVEREM SIDO PREENCHIDOS ENTÃO A RODADA É MARCADA COMO PREENCHIDA
							if(qtd_jogos == 9){
								rodadas[i].preenchida = 2;
							}
							//QUEBRA REPETIÇÃO
							i = MAX_RODADAS+1;
							j = 10;
						}	
					}
				}
			}			

			//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
			//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
			if(qtd_jogos != 9 || strlen(clubes_livres[1].nome) != 0){
				do{
					COR_ROXO
					printf("\n- Deseja cadastrar mais um jogo? -\n");
					printf("\n- Sim = s/S\tNão = n/N -\n");
					__fpurge(stdin);
					scanf("%c", &continuar);
				}while(continuar != 'n' && continuar != 'N' && continuar != 's' && continuar != 'S');
			}
			else{
				continuar = 'n';
			}
		}while(continuar != 'n' && continuar != 'N');
	}
	else if(rodadas[MAX_RODADAS-1].preenchida == 2){
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Todas as rodadas já foram preenchidas\n");
		sleep(2);
	}
}

void listar_rodadas(TRodada *rodadas, int op_rodada){
	char voltar;


	if(op_rodada != 0 && rodadas[op_rodada-1].preenchida != 0){
		do{
			system("clear");
			COR_AGUA
			printf("\n\nRodada n°%d\n", op_rodada);
			for(int i = 0; i<10; i++){
				printf("%s \t %d x %d\t %s\t%s\t%s\n", rodadas[op_rodada-1].jogos[i][0].nome, rodadas[op_rodada-1].resultado[i][0], rodadas[op_rodada-1].resultado[i][1], rodadas[op_rodada-1].jogos[i][1].nome, rodadas[op_rodada-1].jogos[i][0].estadio, rodadas[op_rodada-1].jogos[i][0].cidade);
			}
			do{
				COR_ROXO
				printf("\n- Deseja voltar ao menu? -\n");
				printf("\n- Sim = s/S\tNão = n/N -\n");
				__fpurge(stdin);
				scanf("%s", &voltar);
				if(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S'){
					COR_VERMELHO
					system("clear");
					printf("[ERRO] Opção inválida\n");
					sleep(2);
					voltar = 'n';
				}
			}while(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S');

		}while(voltar != 's' && voltar != 'S');
	}
	else if(op_rodada == 0 && rodadas[0].preenchida != 0){

		do{
			system("clear");
			COR_AGUA
			for(int i = 0; i<MAX_RODADAS; i++){
				if(rodadas[i].preenchida != 0){
					printf("\n\nRodada n°%d\n", i+1);
					for(int j = 0; j<10; j++){						
						printf("%s \t %d x %d\t %s\t%s\t%s\n", rodadas[i].jogos[j][0].nome, rodadas[i].resultado[j][0], rodadas[i].resultado[j][1], rodadas[i].jogos[j][1].nome, rodadas[i].jogos[j][0].estadio, rodadas[i].jogos[j][0].cidade);
					}
				}
			}

			do{
				COR_ROXO
				printf("\n- Deseja voltar ao menu? -\n");
				printf("\n- Sim = s/S\tNão = n/N -\n");
				__fpurge(stdin);
				scanf("%s", &voltar);
				if(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S'){
					COR_VERMELHO
					system("clear");
					printf("[ERRO] Opção inválida\n");
					sleep(2);
					voltar = 'n';
				}
			}while(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S');
		}while(voltar != 's' && voltar != 'S');
		
	}
	else if(op_rodada != 0 && rodadas[op_rodada-1].preenchida == 0){
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Rodada não preenchida!\n");
		sleep(2);
	}
	else{
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Nenhuma rodada preenchida!\n");
		sleep(2);
	}

}

void listar_clubes(TClube *clubes)
{
	char sair;

	for(int i =0; i<MAX_CLUBES; i++){
		if(strlen(clubes[i].nome) != 0)
		{
			COR_VERDE
			printf("---------------------------------------\n");
			printf("|- Clube n° %d\t\t\t\t\t\t\n", i);
			printf("|- Nome do clube: %s\t\t\t\t\t\n", clubes[i].nome);
			printf("|- Cidade do clube: %s\t\t\t\t\t\n", clubes[i].cidade);
			printf("|- Estado do clube: %s\t\t\t\t\t\n", clubes[i].sigla);
			printf("|- Estadio do clube: %s\t\t\t\t\n", clubes[i].estadio);
			printf("----------------------------------------\n\n");
			//o strlen verifica se a posição do vetor está vaga
			//por isso ele lista todas as posições não nulas do vetor
		}
	}
}

void organiza_vetor_times_livres(TClube *clubes, TClube *vet_clubes_usados, TClube *clubes_livres, TRodada *rodadas)
{
	int cont = 0, posicao_rodada = 0, posicao_times = 0, flag = 0;

	for(int i = 0; i<MAX_CLUBES; i++)
	{
		strcpy(vet_clubes_usados[i].nome, "\0");
		strcpy(vet_clubes_usados[i].cidade, "\0");
		strcpy(vet_clubes_usados[i].sigla, "\0");
		strcpy(vet_clubes_usados[i].estadio, "\0");

		strcpy(clubes_livres[i].nome, "\0");
		strcpy(clubes_livres[i].cidade, "\0");
		strcpy(clubes_livres[i].sigla, "\0");
		strcpy(clubes_livres[i].estadio, "\0");
	}
	
	for(int i = 0; i<MAX_RODADAS; i++){
		if(rodadas[i].preenchida == 1){
			posicao_rodada = i;
			for(int j = 0; j < 10; j++){
				for(int k = 0; k < 2; k++){
					if(strlen(rodadas[posicao_rodada].jogos[j][k].nome) != 0){
						//POSIÇÃO J+J+K PQ O VALOR DESSE CALCULO SEMPRE SERA MENOR QUE 20 NESSA REPETIÇÃO
						//POR ISSO IRÁ CABER NO vet_clubes_usados
						//PARANDO PRA PENSAR NÃO SEI PORQUE FIZEMOS ISSO POIS PODERIA SÓ SER UM CONTADOR...
						strcpy(vet_clubes_usados[j+j+k].nome, rodadas[posicao_rodada].jogos[j][k].nome);
						strcpy(vet_clubes_usados[j+j+k].cidade, rodadas[posicao_rodada].jogos[j][k].cidade);
						strcpy(vet_clubes_usados[j+j+k].sigla, rodadas[posicao_rodada].jogos[j][k].sigla);
						strcpy(vet_clubes_usados[j+j+k].estadio, rodadas[posicao_rodada].jogos[j][k].estadio);
						posicao_times = j;
					}
				}
			}
		}
	}

	for(int i = 0; i < MAX_CLUBES; i++){
		for(int j = 0; j < MAX_CLUBES; j++){
			if(strcmp(clubes[i].nome, vet_clubes_usados[j].nome) == 0){
				//MARCA SE O TIME ESTA SENDO UTILIZADO OU NÃO
				flag = 1;
			}	
		}
		if(flag == 0){
			strcpy(clubes_livres[cont].nome, clubes[i].nome);
			strcpy(clubes_livres[cont].cidade, clubes[i].cidade);
			strcpy(clubes_livres[cont].sigla, clubes[i].sigla);
			strcpy(clubes_livres[cont].estadio, clubes[i].estadio);
			//CONT SERVE PARA QUE O VETOR SEJA ORGANIZADO NA ORDEM CORRETA
			cont++;
		}
		//REDEFINE FLAG PARA 0 PARA A PRÓXIMA POSIÇÃO
		flag = 0;
	}
}

///////////////////////////////////
///FUNÇÕES REFERENTES AO RANKING///
///////////////////////////////////

void organiza_ranking(TRodada *rodadas, TRanking *ranking, TClube *clubes){
	TRanking rodada_anterior[MAX_CLUBES], aux;
	int flag = 0, rand1, rand2;

	for(int i = 0; i<MAX_CLUBES; i++){
		strcpy(ranking[i].nome, clubes[i].nome);
		ranking[i].pontos = 0;
		ranking[i].jogos = 0;
		ranking[i].vitorias = 0;
		ranking[i].derrotas = 0;
		ranking[i].empates = 0;
		ranking[i].GP = 0;
		ranking[i].GC = 0;
		ranking[i].SG = 0;
		ranking[i].mGP = 0;
		ranking[i].mGC = 0;
		ranking[i].aproveitamento = 0; 
		ranking[i].movimentacao = 0;
		ranking[i].vermelhos = 0;
		ranking[i].amarelos = 0;
	}

	for(int i = 0; i<MAX_CLUBES; i++)
	{
		for(int j = 0; j<MAX_RODADAS; j++)
		{
			for(int k = 0; k<10; k++)
			{
				if(strcmp(rodadas[j].jogos[k][0].nome, ranking[i].nome) == 0 && strlen(rodadas[j].jogos[k][0].nome) != 0)
				{
					if(rodadas[j].resultado[k][0] > rodadas[j].resultado[k][1])
					{
						ranking[i].vitorias++;

						ranking[i].pontos = ranking[i].pontos + 3;
					}
					else if(rodadas[j].resultado[k][1] > rodadas[j].resultado[k][0])
					{
						ranking[i].derrotas++;
					}
					else
					{
						ranking[i].empates++;
						ranking[i].pontos = ranking[i].pontos + 1;
					}
					ranking[i].jogos++;
					ranking[i].GP = rodadas[j].resultado[k][0] + ranking[i].GP;
					ranking[i].GC = rodadas[j].resultado[k][1] + ranking[i].GC;
					ranking[i].SG = ranking[i].GP - ranking[i].GC;
					ranking[i].mGP = ranking[i].GP/ranking[i].jogos;
					ranking[i].mGC = ranking[i].GC/ranking[i].jogos;
					ranking[i].aproveitamento = (float)(ranking[i].pontos*100)/(ranking[i].jogos*3);
					ranking[i].amarelos = rodadas[j].cartoes[k][0] + ranking[i].amarelos;
					ranking[i].vermelhos = rodadas[j].cartoes[k][1] + ranking[i].vermelhos;
					ranking[i].movimentacao = i - j;
				}
				else if(strcmp(rodadas[j].jogos[k][1].nome, ranking[i].nome) == 0 && strlen(rodadas[j].jogos[k][0].nome) != 0)
				{
					if(rodadas[j].resultado[k][1] > rodadas[j].resultado[k][0])
					{
						ranking[i].vitorias++;
						ranking[i].pontos = ranking[i].pontos + 3;
					}
					else if(rodadas[j].resultado[k][0] > rodadas[j].resultado[k][1])
					{
						ranking[i].derrotas++;
					}
					else
					{
						ranking[i].empates++;
						ranking[i].pontos = ranking[i].pontos + 1;
					}
					ranking[i].jogos++;
					ranking[i].GP = rodadas[j].resultado[k][1] + ranking[i].GP;
					ranking[i].GC = rodadas[j].resultado[k][0] + ranking[i].GC;
					ranking[i].SG = ranking[i].GP - ranking[i].GC;
					ranking[i].mGP = ranking[i].GP/ranking[i].jogos;
					ranking[i].mGC = ranking[i].GC/ranking[i].jogos;
					ranking[i].aproveitamento = (float)(ranking[i].pontos*100)/(ranking[i].jogos*3);
					ranking[i].amarelos = rodadas[j].cartoes[k][2] + ranking[i].amarelos;
					ranking[i].vermelhos = rodadas[j].cartoes[k][3] + ranking[i].vermelhos;
					ranking[i].movimentacao = i - j;
				}	
			}
		}
		//faz a organização do ranking
		//até que o time com menor pontuação seja o último

	}

	for(int i = 0; i<MAX_CLUBES; i++)
	{
		for(int j =0; j<MAX_CLUBES; j++)
		{
			if(j != i && strlen(ranking[j].nome) != 0 && ranking[i].jogos != 0 && ranking[j].jogos != 0)
			{
				if(ranking[i].pontos > ranking[j].pontos){
					flag = 1;
				}
				else if(ranking[i].pontos == ranking[j].pontos)
				{
					if(ranking[i].vitorias > ranking[j].vitorias)
					{
						flag = 1;
					}
					else if(ranking[i].vitorias == ranking[j].vitorias)
					{
						if(ranking[i].SG > ranking[j].SG)
						{
							flag = 1;
						}
						else if(ranking[i].SG == ranking[j].SG)
						{
							if(ranking[i].GP > ranking[j].GP)
							{
								flag = 1;
							}
							else if(ranking[i].GP == ranking[j].GP)
							{
								if(ranking[i].vermelhos < ranking[j].vermelhos)
								{
									flag = 1;
								}
								else if(ranking[i].vermelhos == ranking[j].vermelhos)
								{
									if(ranking[i].amarelos < ranking[j].amarelos)
									{
										flag = 1;
									}
									else if(ranking[i].amarelos == ranking[j].amarelos)
									{
										//GERA NUMERO DE 0 A 100
										//SE RAND FOR MAIR QUE RAND2 ENTÃO O TIME DA POSIÇÃO I É TROCADO DE LUGAR
										do{
											rand1 = rand() % 100;
											rand2 = rand() % 100;
										}while(rand1 == rand2);
										if(rand1 > rand2)
										{
											flag = 1;
										}
									}
								}
							}
						}
					}
				}
			}

			if(flag == 1)
			{
				strcpy(aux.nome, ranking[j].nome);
				aux.pontos = ranking[j].pontos;
				aux.jogos = ranking[j].jogos;
				aux.vitorias = ranking[j].vitorias;
				aux.derrotas = ranking[j].derrotas;
				aux.empates = ranking[j].empates;
				aux.GP = ranking[j].GP;
				aux.GC = ranking[j].GC;
				aux.SG = ranking[j].SG;
				aux.mGP = ranking[j].mGP;
				aux.mGC = ranking[j].mGC;
				aux.aproveitamento = ranking[j].aproveitamento;
				aux.movimentacao = ranking[j].movimentacao;
				aux.vermelhos = ranking[j].vermelhos;
				aux.amarelos = ranking[j].amarelos;

				strcpy(ranking[j].nome, ranking[i].nome);
				ranking[j].pontos = ranking[i].pontos;
				ranking[j].jogos = ranking[i].jogos;
				ranking[j].vitorias = ranking[i].vitorias;
				ranking[j].derrotas = ranking[i].derrotas;
				ranking[j].empates = ranking[i].empates;
				ranking[j].GP = ranking[i].GP;
				ranking[j].GC = ranking[i].GC;
				ranking[j].SG = ranking[i].SG;
				ranking[j].mGP = ranking[i].mGP;
				ranking[j].mGC = ranking[i].mGC;
				ranking[j].aproveitamento = ranking[i].aproveitamento;
				//ranking[j].movimentacao = ranking[i].movimentacao;
				ranking[j].vermelhos = ranking[i].vermelhos;
				ranking[j].amarelos = ranking[i].amarelos;
				ranking[j].movimentacao = ranking[j].movimentacao +1;

				strcpy(ranking[i].nome, aux.nome);
				ranking[i].pontos = aux.pontos;
				ranking[i].jogos = aux.jogos;
				ranking[i].vitorias = aux.vitorias;
				ranking[i].derrotas = aux.derrotas;
				ranking[i].empates = aux.empates;
				ranking[i].GP = aux.GP;
				ranking[i].GC = aux.GC;
				ranking[i].SG = aux.SG;
				ranking[i].mGP = aux.mGP;
				ranking[i].mGC = aux.mGC;
				ranking[i].aproveitamento = aux.aproveitamento;
				//ranking[i].movimentacao = aux.movimentacao;
				ranking[i].vermelhos = aux.vermelhos;
				ranking[i].amarelos = aux.amarelos;
				ranking[i].movimentacao = ranking[i].movimentacao -1;

				flag = 0;
			}
		}
	}
}

void lista_ranking(TRanking *ranking)
{
	char voltar;

		do{
			system("clear");
			printf("     NOME\tPONTOS\tJOGOS\tV\tE\tD\tGP\tGC\tSG\tMGP\tMGC\t%\tM\n");
			for(int i = 0; i<MAX_CLUBES; i++){
				COR_VERDE
				if(strlen(ranking[i].nome) != 0){
					printf("%d° -> %s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%.2f\t\n", (i+1), ranking[i].nome, ranking[i].pontos, ranking[i].jogos, ranking[i].vitorias, ranking[i].empates, ranking[i].derrotas, ranking[i].GP, ranking[i].GC, ranking[i].SG, ranking[i].mGP, ranking[i].mGC, ranking[i].aproveitamento);

				}
				//printf("%d° -> %s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%.2f\t%d\n", (i+1), ranking[i].nome, ranking[i].pontos, ranking[i].jogos, ranking[i].vitorias, ranking[i].empates, ranking[i].derrotas, ranking[i].GP, ranking[i].GC, ranking[i].SG, ranking[i].mGP, ranking[i].mGC, ranking[i].aproveitamento, ranking[i].movimentacao);

			}

			do{
				COR_ROXO
				printf("\n- Deseja voltar ao menu? -\n");
				printf("\n- Sim = s/S\tNão = n/N -\n");
				__fpurge(stdin);
				scanf("%s", &voltar);

				if(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S'){
					COR_VERMELHO
					system("clear");
					printf("[ERRO] Opção inválida\n");
					sleep(2);
					voltar = 'n';
				}
			}while(voltar != 'n' && voltar != 'N' && voltar != 's' && voltar != 'S');
		}while(voltar != 's' && voltar != 'S');
}

void zerar_rodadas(TRodada *rodadas)
{
	for(int i = 0; i<MAX_RODADAS; i++){
		for(int j = 0; j<10; j++){
			for(int k = 0; k<2; k++){
				strcpy(rodadas[i].jogos[j][k].nome, "\0");
				strcpy(rodadas[i].jogos[j][k].cidade, "\0");
				strcpy(rodadas[i].jogos[j][k].sigla, "\0");
				strcpy(rodadas[i].jogos[j][k].estadio, "\0");
				rodadas[i].resultado[j][k] = 0;
				rodadas[i].preenchida = 0;

			}
			rodadas[i].cartoes[j][0] = 0;
			rodadas[i].cartoes[j][1] = 0;
			rodadas[i].cartoes[j][2] = 0;
			rodadas[i].cartoes[j][3] = 0;
		}
	}
}

void gerar_rodada_aleatoria(TRodada *rodadas, TClube *clubes)
{
	TClube clubes_livres[MAX_CLUBES], vet_clubes_usados[MAX_CLUBES];
	int qtd_jogos = 0, op_clube;
	char continuar;

	if(rodadas[MAX_RODADAS-19].preenchida != 2){
		do{
			system("clear");
			organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);
			do{
				op_clube = rand() % 20;
			}while(strlen(clubes_livres[op_clube].nome) == 0);
			//verifica os clubes livres para por rodada

				for(int i = 0; i<MAX_RODADAS; i++){
					if(rodadas[i].preenchida != 2){
						for(int j = 0; j < 10; j++){
							for(int k = 0; k < 2; k++){
								if(strlen(rodadas[i].jogos[j][k].nome) == 0){
									strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
									strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
									strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
									strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
									rodadas[i].resultado[j][k] = rand() % 8;
									rodadas[i].cartoes[j][0] = rand() % 8;
									rodadas[i].cartoes[j][1] = rand() % 3;
									rodadas[i].preenchida = 1;
									i = MAX_RODADAS+1;
								}
							}
						}
					}
				}			

				organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);

				do{
					op_clube = rand() % 20;
				}while(strlen(clubes_livres[op_clube].nome) == 0);
				//verifica os clubes livres para por rodada
				
					for(int i = 0; i<MAX_RODADAS; i++){
						if(rodadas[i].preenchida != 2){
							for(int j = 0; j < 10; j++){
								for(int k = 0; k < 2; k++){
									if(strlen(rodadas[i].jogos[j][k].nome) == 0){
										strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
										strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
										strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
										strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
										rodadas[i].resultado[j][k] = rand() % 8;
										rodadas[i].cartoes[j][2] = rand() % 8;
										rodadas[i].cartoes[j][3] = rand() % 3;
										qtd_jogos = j;
										i = MAX_RODADAS+1;
	
									}
									if(j == 9){
										rodadas[i].preenchida = 2;
									}
								}
							}
						}
					}			
			//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
			//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
			if(qtd_jogos != 9 && strlen(clubes_livres[1].nome) != 0){
				continuar = 'n';
			}
		}while(continuar != 'n' && continuar != 'N');
	}
	else if(rodadas[MAX_RODADAS-20].preenchida == 2 && rodadas[MAX_RODADAS].preenchida != 2){
		do{
			for(int i = 0; i<MAX_RODADAS; i++){
				if(rodadas[i].preenchida != 2){
					for(int j = 0; j < 10; j++){
						if(strlen(rodadas[i].jogos[j][0].nome) == 0){
							strcpy(rodadas[i].jogos[j][0].nome, rodadas[i-19].jogos[j][1].nome);
							strcpy(rodadas[i].jogos[j][0].cidade, rodadas[i-19].jogos[j][1].cidade);
							strcpy(rodadas[i].jogos[j][0].sigla, rodadas[i-19].jogos[j][1].sigla);
							strcpy(rodadas[i].jogos[j][0].estadio, rodadas[i-19].jogos[j][1].estadio);
							rodadas[i].resultado[j][0] = rand() % 8;
							rodadas[i].cartoes[j][0] = rand() % 8;
							rodadas[i].cartoes[j][1] = rand() % 3;

							strcpy(rodadas[i].jogos[j][1].nome, rodadas[i-19].jogos[j][0].nome);
							strcpy(rodadas[i].jogos[j][1].cidade, rodadas[i-19].jogos[j][0].cidade);
							strcpy(rodadas[i].jogos[j][1].sigla, rodadas[i-19].jogos[j][0].sigla);
							strcpy(rodadas[i].jogos[j][1].estadio, rodadas[i-19].jogos[j][0].estadio);
							rodadas[i].resultado[j][1] = rand() % 8;
							rodadas[i].cartoes[j][2] = rand() % 8;
							rodadas[i].cartoes[j][3] = rand() % 3;
							qtd_jogos = j;
							rodadas[i].preenchida = 1;

							//VERIFICA SE OS 10 JOGOS JA FORAM PREENCHIDOS
							//SE ELES JA TIVEREM SIDO PREENCHIDOS ENTÃO A RODADA É MARCADA COMO PREENCHIDA
							if(qtd_jogos == 9){
								rodadas[i].preenchida = 2;
							}
							//QUEBRA REPETIÇÃO
							i = MAX_RODADAS+1;
							j = 10;
						}	
					}
				}
			}			

			//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
			//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
			if(qtd_jogos == 9 || strlen(clubes_livres[1].nome) == 0){
				continuar = 'n';
			}
		}while(continuar != 'n' && continuar != 'N');
	}
	else if(rodadas[MAX_RODADAS-1].preenchida == 2){
		COR_VERMELHO
		system("clear");
		printf("[ERRO] Todas as rodadas já foram preenchidas\n");
		sleep(2);
	}
}

void gerar_torneio_aleatorio(TRodada *rodadas, TClube *clubes)
{
	TClube clubes_livres[MAX_CLUBES], vet_clubes_usados[MAX_CLUBES];
	int qtd_jogos = 0, op_clube;
	char continuar;

	for(int i = 0; i<MAX_RODADAS; i++){
		for(int j = 0; j<10; j++){
			for(int k = 0; k<2; k++){
				strcpy(rodadas[i].jogos[j][k].nome, "\0");
				strcpy(rodadas[i].jogos[j][k].cidade, "\0");
				strcpy(rodadas[i].jogos[j][k].sigla, "\0");
				strcpy(rodadas[i].jogos[j][k].estadio, "\0");
				rodadas[i].resultado[j][k] = 0;
				rodadas[i].preenchida = 0;

			}
			rodadas[i].cartoes[j][0] = 0;
			rodadas[i].cartoes[j][1] = 0;
			rodadas[i].cartoes[j][2] = 0;
			rodadas[i].cartoes[j][3] = 0;
		}
	}

	for(int h = 0; h<MAX_RODADAS * 10; h++){
		if(rodadas[MAX_RODADAS-20].preenchida != 2){
			do{
				// system("clear");
				organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);
				do{
					op_clube = rand() % 20;
				}while(strlen(clubes_livres[op_clube].nome) == 0);

					for(int i = 0; i<MAX_RODADAS; i++){
						if(rodadas[i].preenchida != 2){
							for(int j = 0; j < 10; j++){
								for(int k = 0; k < 2; k++){
									if(strlen(rodadas[i].jogos[j][k].nome) == 0){
										strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
										strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
										strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
										strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
										rodadas[i].resultado[j][k] = rand() % 8;
										rodadas[i].cartoes[j][0] = rand() % 8;
										rodadas[i].cartoes[j][1] = rand() % 3;
										rodadas[i].preenchida = 1;
										i = MAX_RODADAS+1;
									}
								}
							}
						}
					}			

					organiza_vetor_times_livres(clubes, vet_clubes_usados, clubes_livres, rodadas);

					do{
						op_clube = rand() % 20;
					}while(strlen(clubes_livres[op_clube].nome) == 0);
						for(int i = 0; i<MAX_RODADAS; i++){
							if(rodadas[i].preenchida != 2){
								for(int j = 0; j < 10; j++){
									for(int k = 0; k < 2; k++){
										if(strlen(rodadas[i].jogos[j][k].nome) == 0){
											strcpy(rodadas[i].jogos[j][k].nome, clubes_livres[op_clube].nome);
											strcpy(rodadas[i].jogos[j][k].cidade, clubes_livres[op_clube].cidade);
											strcpy(rodadas[i].jogos[j][k].sigla, clubes_livres[op_clube].sigla);
											strcpy(rodadas[i].jogos[j][k].estadio, clubes_livres[op_clube].estadio);
											rodadas[i].resultado[j][k] = rand() % 8;
											rodadas[i].cartoes[j][2] = rand() % 8;
											rodadas[i].cartoes[j][3] = rand() % 3;
											qtd_jogos = j;
											i = MAX_RODADAS+1;
										}
										if(j == 9){
											rodadas[i].preenchida = 2;
										}
									}
								}
							}
						}			
				//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
				//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
				if(qtd_jogos == 9 || strlen(clubes_livres[1].nome) == 0){
					continuar = 'n';
				}
			}while(continuar != 'n' && continuar != 'N');
		}
		else if(rodadas[MAX_RODADAS-20].preenchida != 0 && rodadas[MAX_RODADAS].preenchida != 2){
			do{
				for(int i = 0; i<MAX_RODADAS; i++){
					if(rodadas[i].preenchida != 2){
						for(int j = 0; j < 10; j++){
							if(strlen(rodadas[i].jogos[j][0].nome) == 0){
								strcpy(rodadas[i].jogos[j][0].nome, rodadas[i-19].jogos[j][1].nome);
								strcpy(rodadas[i].jogos[j][0].cidade, rodadas[i-19].jogos[j][1].cidade);
								strcpy(rodadas[i].jogos[j][0].sigla, rodadas[i-19].jogos[j][1].sigla);
								strcpy(rodadas[i].jogos[j][0].estadio, rodadas[i-19].jogos[j][1].estadio);
								rodadas[i].resultado[j][0] = rand() % 8;
								rodadas[i].cartoes[j][0] = rand() % 8;
								rodadas[i].cartoes[j][1] = rand() % 3;

								strcpy(rodadas[i].jogos[j][1].nome, rodadas[i-19].jogos[j][0].nome);
								strcpy(rodadas[i].jogos[j][1].cidade, rodadas[i-19].jogos[j][0].cidade);
								strcpy(rodadas[i].jogos[j][1].sigla, rodadas[i-19].jogos[j][0].sigla);
								strcpy(rodadas[i].jogos[j][1].estadio, rodadas[i-19].jogos[j][0].estadio);
								rodadas[i].resultado[j][1] = rand() % 8;
								rodadas[i].cartoes[j][2] = rand() % 8;
								rodadas[i].cartoes[j][3] = rand() % 3;
								qtd_jogos = j;
								rodadas[i].preenchida = 1;

								//VERIFICA SE OS 10 JOGOS JA FORAM PREENCHIDOS
								//SE ELES JA TIVEREM SIDO PREENCHIDOS ENTÃO A RODADA É MARCADA COMO PREENCHIDA
								if(qtd_jogos == 9){
									rodadas[i].preenchida = 2;
								}
								//QUEBRA REPETIÇÃO
								i = MAX_RODADAS+1;
								j = 10;
							}	
						}
					}
				}			
				//QTD JOGOS MARCA SE A RODADA NÃO FOI COMPLETAMENTE PREENCHIDA
				//O STRLEN VERIFICA SE AINDA EXISTEM DOIS TIMES LIVRES PARA QUE POSSA SER FEITO UM NOVO JOGO
				if(qtd_jogos == 9 || strlen(clubes_livres[1].nome) == 0){
					continuar = 'n';
				}
				
			}while(continuar != 'n' && continuar != 'N');
		}
	}
}