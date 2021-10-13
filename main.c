// Murilo Xavier de Oliveira
#include <stdio.h>
#include <stdlib.h> //necessário para usar as funções malloc() e free()
#include <conio.h>
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
  int tempoFilaRecepSaida;
  int tempoRecepSaida;
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

pacientes *aloca(int identificacao_paciente, int tempo_fila_rec, int tempo_rec, int tempo_fila_med, int tempo_med, int tempo_fila_rec_saida, int tempo_rec_saida)
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
    novo->tempoMedico = tempo_med;
    novo->tempoFilaRecepSaida = tempo_fila_rec_saida;
    novo->tempoRecepSaida = tempo_rec_saida;
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

void push(pacientes *FILA, pacientes *Origem)
{
  pacientes *novo = aloca(Origem->id,
                          Origem->tempoFilaRecep,
                          Origem->tempoRecep,
                          Origem->tempoFilaMedico,
                          Origem->tempoMedico,
                          Origem->tempoFilaRecepSaida,
                          Origem->tempoRecepSaida); //push um novo na fila

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
    return NULL;
  }
  else
  {
    pacientes *tmp = FILA->prox;
    FILA->prox = tmp->prox;
    return tmp;
  }
}

void display(pacientes *fila_rec, pacientes *recepcao, pacientes *fila_med, pacientes *medico, int tempo_atual, int tempo_prox_fila_rec, int tempo_prox_rec, int tempo_prox_med)
{
  pacientes *aux = fila_rec; //cria nó auxiliar
  int i = 0;
  char *nome_fila = "fila recepcao";

  while (i < 4)
  {
    printf("%s -> ", nome_fila);
    if (aux == NULL)
    {
      printf(" vazia"); //fila_rec vazia se for igual a NULL
    }

    for (aux; aux != NULL; aux = aux->prox)
    {
      printf("%i - ", aux->id);
    }
    i++;

    if (i == 1)
    {
      aux = recepcao;
      nome_fila = "recepcao";
    }
    else if (i == 2)
    {
      aux = fila_med;
      nome_fila = "fila medico";
    }
    else if (i == 3)
    {
      aux = medico;
      nome_fila = "medico";
    }
    printf("\n");
  }

  fflush(stdout);
  printf("\n\nTempo Real -> %d\n", tempo_atual);
  fflush(stdout);
  printf("\nProximo na Fila da Recepcao -> %d\n", tempo_prox_fila_rec);
  fflush(stdout);
  printf("\nProximo na Recepcao -> %d\n", tempo_prox_rec);
  fflush(stdout);
  printf("\nProximo no Medico -> %d\n", tempo_prox_med);
  printf("\n\n");
}

void copiarFila(pacientes *destino, pacientes *origem)
{
  destino->id = origem->id;
  destino->tempoFilaRecep = origem->tempoFilaRecep;
  destino->tempoRecep = origem->tempoRecep;
  destino->tempoFilaMedico = origem->tempoFilaMedico;
  destino->tempoMedico = origem->tempoMedico;
  destino->tempoFilaRecepSaida = origem->tempoFilaRecepSaida;
  destino->tempoRecepSaida = origem->tempoRecepSaida;
  destino->prox = NULL; //como não é uma fila, remove o próximo (restante da fila (ele fica sozinho na recepcao))
}

int ajustaTempo(int prox_tempo)
{
  if (prox_tempo >= 60)
  {                                      //existe um momento em que a soma passa dos 60 seg
    return prox_tempo = prox_tempo - 60; //então é necessário subtrair esse valor
  }
  return prox_tempo;
}

void displayDados(pacientes *Fila)
{
  pacientes *aux = Fila; //cria nó auxiliar

  system("cls");

  for (aux; aux != NULL; aux = aux->prox)
  {
    printf("Id -> %i\n", aux->id);
    printf("Fila Recepcao -> %i\n", aux->tempoFilaRecep);
    printf("Recepcao - > %i\n", aux->tempoRecep);
    printf("Fila Medico -> %i\n", aux->tempoFilaMedico);
    printf("Medico -> %i\n", aux->tempoMedico);
    printf("Fila Recepcao Saida -> %i\n", aux->tempoFilaRecepSaida);
    printf("Recepcao Saida -> %i\n\n", aux->tempoRecepSaida);
  }
}

void displayRelatorio(pacientes *Fila)
{
  pacientes *aux = Fila; //cria nó auxiliar
  int fila_recep_min = aux->tempoFilaRecep, fila_recep_max = aux->tempoFilaRecep;
  float fila_recep_med = 0;

  int recep_min = aux->tempoRecep, recep_max = aux->tempoRecep;
  float recep_med = 0;

  int fila_med_min = aux->tempoFilaMedico, fila_med_max = aux->tempoFilaMedico;
  float fila_med_med = 0;

  int med_min = aux->tempoMedico, med_max = aux->tempoMedico;
  float med_med = 0;

  int fila_recep_saida_min = aux->tempoFilaRecepSaida, fila_recep_saida_max = aux->tempoFilaRecepSaida;
  float fila_recep_saida_med = 0;

  int recep_saida_min = aux->tempoRecepSaida, recep_saida_max = aux->tempoRecepSaida;
  float recep_saida_med = 0;

  for (aux; aux != NULL; aux = aux->prox)
  {
    fila_recep_min = fila_recep_min > aux->tempoFilaRecep ? aux->tempoFilaRecep : fila_recep_min;
    fila_recep_med = aux->prox != NULL ? fila_recep_med + aux->tempoFilaRecep : (fila_recep_med + aux->tempoFilaRecep) / aux->id;
    fila_recep_max = fila_recep_max < aux->tempoFilaRecep ? aux->tempoFilaRecep : fila_recep_max;

    recep_min = recep_min > aux->tempoRecep ? aux->tempoRecep : recep_min;
    recep_med = aux->prox != NULL ? recep_med + aux->tempoRecep : (recep_med + aux->tempoRecep) / aux->id;
    recep_max = recep_max < aux->tempoRecep ? aux->tempoRecep : recep_max;

    fila_med_min = fila_med_min > aux->tempoFilaMedico ? aux->tempoFilaMedico : fila_med_min;
    fila_med_med = aux->prox != NULL ? fila_med_med + aux->tempoFilaMedico : (fila_med_med + aux->tempoFilaRecep) / aux->id;
    fila_med_max = fila_med_max < aux->tempoFilaMedico ? aux->tempoFilaMedico : fila_med_max;

    med_min = med_min > aux->tempoMedico ? aux->tempoMedico : med_min;
    med_med = aux->prox != NULL ? med_med + aux->tempoMedico : (med_med + aux->tempoMedico) / aux->id;
    med_max = med_max < aux->tempoMedico ? aux->tempoMedico : med_max;

    fila_recep_saida_min = fila_recep_saida_min > aux->tempoFilaRecepSaida ? aux->tempoFilaRecepSaida : fila_recep_saida_min;
    fila_recep_saida_med = aux->prox != NULL ? fila_recep_saida_med + aux->tempoFilaRecepSaida : (fila_recep_saida_med + aux->tempoFilaRecepSaida) / aux->id;
    fila_recep_saida_max = fila_recep_saida_max < aux->tempoFilaRecepSaida ? aux->tempoFilaRecepSaida : fila_recep_saida_max;

    recep_saida_min = recep_saida_min > aux->tempoRecepSaida ? aux->tempoRecepSaida : recep_saida_min;
    recep_saida_med = aux->prox != NULL ? recep_saida_med + aux->tempoRecepSaida : (recep_saida_med + aux->tempoFilaRecep) / aux->id;
    recep_saida_max = recep_saida_max < aux->tempoRecepSaida ? aux->tempoRecepSaida : recep_saida_max;
  }

  printf("Fila Recepcao Tempo Minimo -> %i\n", fila_recep_min);
  printf("Fila Recepcao Tempo Medio -> %.2f\n", fila_recep_med);
  printf("Fila Recepcao Tempo Maximo -> %i\n\n", fila_recep_max);

  printf("Recepcao Tempo Minimo -> %i\n", recep_min);
  printf("Recepcao Tempo Medio -> %.2f\n", recep_med);
  printf("Recepcao Tempo Maximo -> %i\n\n", recep_max);

  printf("Fila Medico Tempo Minimo -> %i\n", fila_med_min);
  printf("Fila Medico Tempo Medio -> %.2f\n", fila_med_med);
  printf("Fila Medico Tempo Maximo -> %i\n\n", fila_med_max);

  printf("Medico Tempo Minimo -> %i\n", med_min);
  printf("Medico Tempo Medio -> %.2f\n", med_med);
  printf("Medico Tempo Maximo -> %i\n\n", med_max);

  printf("Recepcao Tempo Saida Minimo -> %i\n", fila_recep_saida_min);
  printf("Recepcao Tempo Saida Medio -> %.2f\n", fila_recep_saida_med);
  printf("Recepcao Tempo Saida Maximo -> %i\n\n", fila_recep_saida_max);

  printf("Recepcao Saida Tempo Minimo -> %i\n", recep_saida_min);
  printf("Recepcao Saida Tempo Medio -> %.2f\n", recep_saida_med);
  printf("Recepcao Saida Tempo Maximo -> %i\n\n", recep_saida_max);
}

int main(void)
{
  pacientes *fila_rec = (pacientes *)malloc(sizeof(pacientes)); //fila da recepcao
  pacientes *recepcao = NULL;                                   // recepcao
  pacientes *fila_med = NULL;                                   // fila de medico
  pacientes *medico = NULL;                                     // recepcao
  pacientes *saida = NULL;                                      // recepcao
  pacientes *auxNovo = NULL;                                    // recepcao

  int identificacao_paciente = 1, num_pacientes;
  //tempo em segundos que corre no while principal
  int tempo_atual;
  //declara tempo p/ o proximo na fila da rec e tempo de entrada na fila da rec de cada usuario
  int tempo_prox_fila_rec = 0, tempo_fila_rec = gerarNumeroAleatorio(3, 5);
  //tempo de controle utilizado na recepcao e no registro do usuario
  int tempo_rec = 0, tempo_prox_rec = 0;
  //declara tempo de entrada na fila do medico de cada usuario
  int tempo_fila_med = 0;
  //tempo de controle utilizado no medico e no registro do usuario
  int tempo_med = 0, tempo_prox_med = 0;
  //variavel auxliar para decidir de paciente volta a recepcao remarcar // ajuste é utilizado no calculo de tempo na recepcao
  int auxVoltarRecep = 0, ajuste = 0;

  //aux temporaria
  int fim = 1;

  srand(time(NULL));

  printf("Informe a quantidade de pacientes para a simualcao: \n");
  fflush(stdout);
  scanf("%i", &num_pacientes);

  system("cls");

  /** gera o primeiro paciente na fila da recepção */
  fila_rec = aloca(identificacao_paciente, 0, 0, 0, 0, 0, 0);

  tempo_prox_fila_rec = tempo_fila_rec + geraTempo();

  tempo_prox_fila_rec = ajustaTempo(tempo_prox_fila_rec);

  tempo_atual = geraTempo();

  //num_pacientes != identificacao_paciente &&
  while (fim)
  {
    tempo_atual = geraTempo(); //fica gerando o segundo atual

    //utilizei a "identificacao_paciente" como controle, ja que ele ele icrementa a partir de 1
    if (tempo_prox_fila_rec == tempo_atual && identificacao_paciente < num_pacientes)
    {
      identificacao_paciente++;

      if (fila_rec == NULL)
      {
        fila_rec = aloca(identificacao_paciente, tempo_fila_rec, 0, 0, 0, 0, 0); //se a lista estiver vazia, adiciona
      }
      else
      {
        auxNovo = aloca(identificacao_paciente, tempo_fila_rec, 0, 0, 0, 0, 0);
        push(fila_rec, auxNovo); //push um novo na fila
      }
      tempo_fila_rec = gerarNumeroAleatorio(3, 5);
      tempo_prox_fila_rec = tempo_fila_rec + tempo_atual;

      tempo_prox_fila_rec = ajustaTempo(tempo_prox_fila_rec);

      fflush(stdout);
      system("cls");
      display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);
    }

    if (tempo_prox_rec == tempo_atual || recepcao == NULL && fila_rec != NULL)
    {
      if (recepcao != NULL) //quando o primeiro a chegar a "recepcao" vai estar NULL
      {
        if (recepcao->tempoMedico != 0)
        {
          if (saida == NULL) //caso o paciente só passou na recepcao pra remarcar, agora ele irá sair
          {
            saida = (pacientes *)malloc(sizeof(pacientes));
            copiarFila(saida, recepcao);
          }
          else
          {
            push(saida, recepcao); //push um novo na fila
          }
        }
        else
        {
          if (fila_med == NULL) //caso o paciente ainda não passou pelo o medico
          {
            fila_med = (pacientes *)malloc(sizeof(pacientes));
            copiarFila(fila_med, recepcao);
          }
          else
          {
            push(fila_med, recepcao); //push um novo na fila
          }
        }
        recepcao = NULL;
        system("cls");
        display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);
      }

      if (fila_rec != NULL)
      {
        recepcao = (pacientes *)malloc(sizeof(pacientes));

        copiarFila(recepcao, fila_rec); //adiciona o primeiro da fila na recepcao

        fila_rec = pop(fila_rec); //remove da fila da recepcao o primeiro já que ele já chegou na recepcao

        //se o tempoMedico for = 0 significa que ele n passo pelo medico ainda, caso contrario, está remarcando a consulta
        tempo_rec = recepcao->tempoMedico == 0 ? gerarNumeroAleatorio(7, 12) : gerarNumeroAleatorio(2, 4);

        tempo_prox_rec = tempo_rec + tempo_atual;
        tempo_prox_rec = ajustaTempo(tempo_prox_rec);

        system("cls");
        display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);

        if (recepcao->tempoMedico == 0)
        {
          recepcao->tempoRecep = tempo_rec;           //tempo que vai ficar na recepcao
          recepcao->tempoFilaMedico = tempo_prox_rec; //aqui é adicionardo tempo que o proximo vai entrar na recepcao ou seja
                                                      //tempo que o atual vai sair, após isso, no consultorio medico, vai ser feito
                                                      //tempo que saiu da recepcao menos o tempo atual, para o ter o tempo em que ficou
                                                      //na fila do consultorio medico
        }
        else
        {
          //caso paciente tenha entrado na fila no segundo 57 e foi atendido na rec no segundo 2 ou seja 57-(2+60) = 5 seg na fila
          ajuste = recepcao->tempoFilaRecepSaida > tempo_atual ? tempo_atual + 60 : 0;

          recepcao->tempoRecepSaida = tempo_rec;
          recepcao->tempoFilaRecepSaida = (tempo_atual + ajuste) - recepcao->tempoFilaRecepSaida;
        }
      }
    }

    if (tempo_prox_med == tempo_atual || medico == NULL && fila_med != NULL)
    {
      if (medico != NULL) //verifica se tem alguem no medico antes de chamar o proximo
      {
        auxVoltarRecep = gerarNumeroAleatorio(1, 100); //valor <= 50 vai embora > 50 volta pra recepcao remarcar

        if (auxVoltarRecep <= 50)
        {
          if (saida == NULL)
          {
            saida = (pacientes *)malloc(sizeof(pacientes));
            copiarFila(saida, medico);
          }
          else
          {
            push(saida, medico); //push um novo na fila
          }
        }
        else
        {
          if (fila_rec == NULL)
          {
            fila_rec = (pacientes *)malloc(sizeof(pacientes));
            copiarFila(fila_rec, medico);
          }
          else
          {
            push(fila_rec, medico); //push um novo na fila
          }
        }

        medico = NULL;
        system("cls");
        display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);
      }

      if (fila_med != NULL)
      {
        medico = (pacientes *)malloc(sizeof(pacientes));

        copiarFila(medico, fila_med); //adiciona o primeiro da fila no consultorio medico

        fila_med = pop(fila_med); //remove da fila do medico o primeiro já que ele já chegou no medico

        tempo_med = gerarNumeroAleatorio(10, 20); //ficam de 15 a 25 no consultorio

        ajuste = medico->tempoFilaMedico > tempo_atual ? tempo_atual + 60 : 0;
        medico->tempoFilaMedico = (tempo_atual + ajuste) - medico->tempoFilaMedico; //esse é o calculo do tempo de espera na fila do medico
        medico->tempoMedico = tempo_med;

        tempo_prox_med = tempo_med + tempo_atual;
        tempo_prox_med = ajustaTempo(tempo_prox_med);

        fflush(stdout);
        system("cls");
        display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);
      }
    }
    if (fila_rec == NULL && recepcao == NULL && fila_med == NULL && medico == NULL)
    {
      fim = 0;
    }
  }
  //display(fila_rec, recepcao, fila_med, medico, tempo_atual, tempo_prox_fila_rec, tempo_prox_rec, tempo_prox_med);

  system("cls");
  printf("\n\n");
  fflush(stdout);
  displayDados(saida);

  getch();
  printf("\n\n");
  fflush(stdout);
  displayRelatorio(saida);

  //free(fila_rec);

  return 0;
}