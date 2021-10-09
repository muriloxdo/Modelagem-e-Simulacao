#include <stdio.h>
#include <stdlib.h> //necessário para usar as funções malloc() e free()
#include <conio.h>
#include <time.h>
#include <string.h>

/*Criando a struct */
typedef struct Pacientes
{
	int id;
	int tempoEntrada;
	int tempoRecep;
	int tempoMedico;
	struct Pacientes *prox;
} pacientes;

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

void push(pacientes *FILA, int identificacao_paciente, int tempo_fila_rec)
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

pacientes *pop(pacientes *FILA)
{
	if (FILA->prox == NULL)
	{
		printf("Fila ja esta vazia\n");
		//return NULL;
	}
	else
	{
		pacientes *tmp = FILA->prox;
		FILA->prox = tmp->prox;
		return tmp;
	}
}

void display(pacientes *Lista, char* nome)
{
	pacientes *aux = Lista; //cria nó auxiliar

	if (aux == NULL)
	{
		printf("Lista vazia\n"); //lista vazia se for igual a NULL
	}

	printf("%s\n", nome);
	for (aux = Lista; aux != NULL; aux = aux->prox)
	{
		printf("%i - ", aux->id);
	}
		printf("\n\n");
}

void copiarFila(pacientes *destino, pacientes *origem)
{
	destino->id = origem->id;
	destino->tempoEntrada = origem->tempoEntrada;
	destino->tempoRecep = origem->tempoRecep;
	destino->tempoMedico = origem->tempoMedico;
	destino->prox = NULL; //como não é uma fila, remove o próximo (restante da fila (ele fica sozinho na recepcao))
}

int main(void)
{
	pacientes *fila_rec; //definindo o ponteiro do tipo pacientes
	int identificacao_paciente = 1, num_pacientes;
	//tempo em segundos que corre no while principal
	int tempo_atual;
	//tempo de controle utilizado na recepcao e no registro do usuario
	int tempo_rec, tempo_prox_rec;
	//declara tempo p/ o proximo na fila da rec e tempo de entrada na fila da rec de cada usuario
	int tempo_prox_fila_rec, tempo_fila_rec = gerarNumeroAleatorio(3, 21);

	pacientes *recepcao = NULL;

	//aux temporaria
	int aux = 0;

	srand(time(NULL));

	printf("Informe a quantidade de pacientes para a simualcao: \n");
	scanf("%i", &num_pacientes);

	/** gera o primeiro paciente na fila da recepção */
	fila_rec = (pacientes *)malloc(sizeof(pacientes));

	fila_rec->id = identificacao_paciente;
	fila_rec->tempoEntrada = tempo_fila_rec;
	fila_rec->tempoRecep = 0;
	fila_rec->tempoMedico = 0;
	fila_rec->prox = NULL;
	/** gera o primeiro paciente na fila da recepção */

	tempo_prox_fila_rec = tempo_fila_rec + geraTempo();

	//utilizei a "identificacao_paciente" como controle, ja que ele ele icrementa a partir de 1
	while (num_pacientes != identificacao_paciente && aux != num_pacientes)
	{
		tempo_atual = geraTempo(); //fica gerando o segundo atual

		if (tempo_prox_fila_rec == tempo_atual)
		{
			identificacao_paciente++;

			if (fila_rec == NULL)
			{
				fila_rec = aloca(identificacao_paciente, tempo_fila_rec); //se a lista estiver vazia, adiciona
			}
			else
			{
				push(fila_rec, identificacao_paciente, tempo_fila_rec); //push um novo na fila

				tempo_fila_rec = gerarNumeroAleatorio(3, 21);
				tempo_prox_fila_rec = tempo_fila_rec + tempo_atual;

				if (tempo_prox_fila_rec >= 60)
				{																									//existe um momento em que a soma passa dos 60 seg
					tempo_prox_fila_rec = tempo_prox_fila_rec - 60; //então é necessário subtrair esse valor
				}
			}

			display(fila_rec, "fila_rec");

			printf("\ntempo_atual -> %d\n", tempo_atual);
			printf("tempo_fila_rec -> %d\n", tempo_fila_rec);
			printf("tempo_prox_fila_rec -> %d\n", tempo_prox_fila_rec);
			printf("\ntempo_prox_rec -> %d\n", tempo_prox_rec);
		}

		if (tempo_prox_rec == tempo_atual || recepcao == NULL)
		{
			if (fila_rec != NULL)
			{
				recepcao = NULL;
				recepcao = (pacientes *)malloc(sizeof(pacientes));

				copiarFila(recepcao, fila_rec); //adiciona o primeiro da fila na recepcao

				fila_rec = pop(fila_rec); //remove da fila da recepcao o primeiro já que ele já chegou na recepcao

				display(recepcao, "rec");

				tempo_rec = gerarNumeroAleatorio(7, 12);
				recepcao->tempoRecep = tempo_rec;

				tempo_prox_rec = tempo_rec + tempo_atual;

				if (tempo_prox_rec >= 60)
				{																				//existe um momento em que a soma passa dos 60 seg
					tempo_prox_rec = tempo_prox_rec - 60; //então é necessário subtrair esse valor
				}
				aux++;
				printf("aux -> %i\n",aux);
				printf("num_pacientes -> %i\n",num_pacientes);
			}
		}
	}

	free(fila_rec);

	//getch();

	return 0;
}