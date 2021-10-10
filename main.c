#include <stdio.h>
#include <stdlib.h> //necessário para usar as funções malloc() e free()
//#include <conio.h>
#include <time.h>
#include <string.h>

/*Criando a struct */
typedef struct Pacientes
{
  int id;
  int tempoFilaRecep;
  int tempoRecep;
  int tempoFilaMedico;
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

pacientes *aloca(int identificacao_paciente, int tempo_fila_rec, int tempo_rec, int tempo_fila_med)
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
    novo->tempoFilaRecep = tempo_fila_rec;
    novo->tempoRecep = tempo_rec;
    novo->tempoFilaMedico = tempo_fila_med;
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

void push(pacientes *FILA, int identificacao_paciente, int tempo_fila_rec, int tempo_rec, int tempo_fila_med)
{
  pacientes *novo = aloca(identificacao_paciente, tempo_fila_rec, tempo_rec, tempo_fila_med);

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
    printf("pop null\n");
    return NULL;
  }
  else
  {
    pacientes *tmp = FILA->prox;
    FILA->prox = tmp->prox;
    return tmp;
  }
}

void display(pacientes *Lista, char *nome)
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
  destino->tempoFilaRecep = origem->tempoFilaRecep;
  destino->tempoRecep = origem->tempoRecep;
  destino->tempoMedico = origem->tempoMedico;
  destino->prox = NULL; //como não é uma fila, remove o próximo (restante da fila (ele fica sozinho na recepcao))
}

int ajustaTempo(int tempo_prox_fila_rec)
{
  if (tempo_prox_fila_rec >= 60)
  {                                                        //existe um momento em que a soma passa dos 60 seg
    return tempo_prox_fila_rec = tempo_prox_fila_rec - 60; //então é necessário subtrair esse valor
  }
  return tempo_prox_fila_rec;
}

int main(void)
{
  pacientes *fila_rec = (pacientes *)malloc(sizeof(pacientes)); //fila da recepcao
  pacientes *recepcao = NULL;                                   // recepcao
  pacientes *fila_med = NULL;                                   // fila de medico
  pacientes *medico = NULL;                                     // recepcao

  int identificacao_paciente = 1, num_pacientes;
  //tempo em segundos que corre no while principal
  int tempo_atual;
  //declara tempo p/ o proximo na fila da rec e tempo de entrada na fila da rec de cada usuario
  int tempo_prox_fila_rec, tempo_fila_rec = gerarNumeroAleatorio(3, 21);
  //tempo de controle utilizado na recepcao e no registro do usuario
  int tempo_rec, tempo_prox_rec;
  //declara tempo de entrada na fila do medico de cada usuario
  int tempo_fila_med;
  //tempo de controle utilizado no medico e no registro do usuario
  int tempo_med, tempo_prox_med;

  //aux temporaria
  int aux = 0;

  srand(time(NULL));

  printf("Informe a quantidade de pacientes para a simualcao: \n");
  fflush(stdout);
  scanf("%i", &num_pacientes);

  system("cls");

  /** gera o primeiro paciente na fila da recepção */
  fila_rec = aloca(identificacao_paciente, tempo_fila_rec, 0, 0);

  tempo_prox_fila_rec = tempo_fila_rec + geraTempo();

  ajustaTempo(tempo_prox_fila_rec);

  tempo_atual = geraTempo();
  fflush(stdout);
  printf("\ntempo_atual -> %d\n", tempo_atual);
  fflush(stdout);
  printf("tempo_fila_rec -> %d\n", tempo_fila_rec);
  fflush(stdout);
  printf("tempo_prox_fila_rec -> %d\n", tempo_prox_fila_rec);

  //utilizei a "identificacao_paciente" como controle, ja que ele ele icrementa a partir de 1
  //num_pacientes != identificacao_paciente &&
  while (aux != num_pacientes)
  {
    tempo_atual = geraTempo(); //fica gerando o segundo atual

    if (tempo_prox_fila_rec == tempo_atual)
    {
      identificacao_paciente++;

      if (fila_rec == NULL)
      {
        fila_rec = aloca(identificacao_paciente, tempo_fila_rec, 0, 0); //se a lista estiver vazia, adiciona
      }
      else
      {
        push(fila_rec, identificacao_paciente, tempo_fila_rec, 0, 0); //push um novo na fila
      }
      tempo_fila_rec = gerarNumeroAleatorio(3, 21);
      tempo_prox_fila_rec = tempo_fila_rec + tempo_atual;
      printf("%i %i %i\n", tempo_prox_fila_rec, tempo_fila_rec, tempo_atual);

      tempo_prox_fila_rec = ajustaTempo(tempo_prox_fila_rec);

      fflush(stdout);
      system("cls");
      display(fila_rec, "fila_rec");

      fflush(stdout);
      printf("\ntempo_atual -> %d\n", tempo_atual);
      fflush(stdout);
      printf("tempo_fila_rec -> %d\n", tempo_fila_rec);
      fflush(stdout);
      printf("tempo_prox_fila_rec -> %d\n", tempo_prox_fila_rec);
      fflush(stdout);
      printf("\ntempo_prox_rec -> %d\n", tempo_prox_rec);
      fflush(stdout);
      printf("\ntempo_prox_med -> %d\n", tempo_prox_med);
    }

    if (tempo_prox_rec == tempo_atual || recepcao == NULL)
    {
      if (recepcao != NULL) //quando o primeiro a chegar a "recepcao" vai estar NULL
      {
        if (fila_med == NULL)
        {
          fila_med = (pacientes *)malloc(sizeof(pacientes));
          copiarFila(fila_med, recepcao);
        }
        else
        {
          push(
              fila_med,
              recepcao->id,
              recepcao->tempoFilaRecep,
              recepcao->tempoRecep,
              recepcao->tempoFilaMedico); //push um novo na fila
        }
      }

      if (fila_rec != NULL)
      {
        recepcao = NULL;
        recepcao = (pacientes *)malloc(sizeof(pacientes));

        copiarFila(recepcao, fila_rec); //adiciona o primeiro da fila na recepcao

        fila_rec = pop(fila_rec); //remove da fila da recepcao o primeiro já que ele já chegou na recepcao

        display(recepcao, "rec");

        tempo_rec = gerarNumeroAleatorio(7, 12);

        tempo_prox_rec = tempo_rec + tempo_atual;
        tempo_prox_rec = ajustaTempo(tempo_prox_rec);

        recepcao->tempoRecep = tempo_rec;           //tempo que vai ficar na recepcao
        recepcao->tempoFilaMedico = tempo_prox_rec; //aqui é adicionardo tempo que o proximo vai entrar na recepcao ou seja
                                                    //tempo que o atual vai sair, após isso, no consultorio medico, vai ser feito
                                                    //tempo que saiu da recepcao menos o tempo atual, para o ter o tempo em que ficou
                                                    //na fila do consultorio medico

        fflush(stdout);
        printf("tempo_prox_rec -> %d\n", tempo_prox_rec);
      }

      if (tempo_prox_med == tempo_atual || medico == NULL)
      {
        if (fila_med != NULL)
        {
          medico = NULL;
          medico = (pacientes *)malloc(sizeof(pacientes));;

          copiarFila(medico, fila_med); //adiciona o primeiro da fila no consultorio medico

          fila_med = pop(fila_med); //remove da fila do medico o primeiro já que ele já chegou no medico

          display(medico, "medico");

          tempo_med = gerarNumeroAleatorio(15, 25); //ficam de 15 a 25 no consultorio

          medico->tempoFilaMedico = medico->tempoFilaMedico - tempo_atual; //esse é o calculo do tempo de espera na fila do medico
          medico->tempoMedico = tempo_med;

          tempo_prox_med = tempo_med + tempo_atual;
          tempo_prox_med = ajustaTempo(tempo_prox_med);

          fflush(stdout);
          printf("tempo_prox_med -> %d\n", tempo_prox_med);

          aux++;
        }
      }
    }
  }

  printf("num_pacientes -> %i\nid -> %i\naux -> %i\n", num_pacientes, identificacao_paciente, aux);

  free(fila_rec);

  //getch();

  return 0;
}