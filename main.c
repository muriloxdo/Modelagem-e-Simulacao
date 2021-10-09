#include <stdio.h>
#include <stdlib.h> //necessário para usar as funções malloc() e free()
#include <conio.h>
#include <time.h>

/*Criando a struct */
typedef struct Pacientes
{
	int id;
	int tempoEntrada;
	int tempoRecep;
	int tempoMedico;
	struct Pacientes *prox;
}pacientes;

//gera valor aleatorio entre lim_inf e lim_sup
int gerarNumeroAleatorio(int lim_inf, int lim_sup)
{
	// a função rand() gera um inteiro aleatório
	return (rand() % (lim_sup - lim_inf + 1)) + lim_inf;
}

int geraTempo()
{
	struct tm *data_hora_atual;
	//variável do tipo time_t para armazenar o tempo em segundos
	time_t segundos;

	//obtendo o tempo em segundos
	time(&segundos);

	//para converter de segundos para o tempo local utilizamos a função localtime
	data_hora_atual = localtime(&segundos);
	//printf("%d\n", data_hora_atual->tm_sec);
	return data_hora_atual->tm_sec;
}

pacientes *aloca(int identificacao_paciente, int tempo_fila_rec)
{
	pacientes *novo = (pacientes *)malloc(sizeof(pacientes));
	if (!novo)
	{
		printf("Sem memoria disponivel!\n");
		exit(1);
	}
	else
	{
		novo->id = identificacao_paciente;
		novo->tempoEntrada = tempo_fila_rec;
		novo->tempoRecep = 0;
		novo->tempoMedico = 0;
		novo->prox = NULL;
		return novo;
	}
}

int vazia(pacientes *FILA)
{
	if (FILA->prox == NULL)
		return 1;
	else
		return 0;
}

void insere(pacientes *FILA, int identificacao_paciente, int tempo_fila_rec)
{
	pacientes *novo = aloca(identificacao_paciente, tempo_fila_rec);

	if (vazia(FILA))
		FILA->prox = novo;
	else
	{
		pacientes *tmp = FILA->prox;

		while (tmp->prox != NULL)
			tmp = tmp->prox;

		tmp->prox = novo;
	}
}

void display(pacientes *Lista)
{
	pacientes *aux = Lista; //cria nó auxiliar

	if (aux == NULL)
	{
		printf("Lista vazia\n\n"); //lista vazia se for igual a NULL
	}

	for (aux = Lista; aux != NULL; aux = aux->prox)
	{
		printf("cod - %i\n", aux->id);
	}
}

int main(void)
{
	pacientes *fila_rec; //definindo o ponteiro do tipo pacientes
	int identificacao_paciente = 1, num_pacientes;
	int tempo_atual;
	//declara tempo p/ o proximo na fila da rec e tempo de entrada na fila da rec de cada um
	int tempo_prox_fila_rec, tempo_fila_rec = gerarNumeroAleatorio(3, 21);

	/** gera o primeiro paciente na fila da recepção */
	fila_rec = (pacientes *)malloc(sizeof(pacientes));

	fila_rec->id = identificacao_paciente;
	fila_rec->tempoEntrada = tempo_fila_rec;
	fila_rec->tempoRecep = 0;
	fila_rec->tempoMedico = 0;
	fila_rec->prox = NULL;
	/** gera o primeiro paciente na fila da recepção */

	tempo_prox_fila_rec = tempo_fila_rec + geraTempo();

	while (1)
	{
		tempo_atual = geraTempo(); //fica gerando o segundo atual

		if (tempo_prox_fila_rec == tempo_atual)
		{
			identificacao_paciente++;
			insere(fila_rec, identificacao_paciente, tempo_fila_rec); //insere um novo na fila

			display(fila_rec);

			tempo_fila_rec = gerarNumeroAleatorio(3, 21);
			tempo_prox_fila_rec = tempo_fila_rec + tempo_atual;

			if (tempo_prox_fila_rec >= 60)
			{																									//existe um momento em que a soma passa dos 60 seg
				tempo_prox_fila_rec = tempo_prox_fila_rec - 60; //então é necessário subtrair esse valor
			}

			printf("\n\ntempo_atual -> %d\n", tempo_atual);
			printf("tempo_fila_rec -> %d\n", tempo_fila_rec);
			printf("tempo_prox_fila_rec -> %d\n", tempo_prox_fila_rec);
		}
	}

	srand(time(NULL));

	free(fila_rec);

	//getch();

	return 0;
}