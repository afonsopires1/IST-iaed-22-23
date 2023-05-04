/* iaed-23 - ist1102803 - project2 */
#include "project2.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------ Funções associadas comando E -------------------------- */

/* função secundaria que imprime os custos */
void imprime_custos(Carreiras **carreiras, int index_carreira) {

  int i;

  for (i = 0; i < (*carreiras)[index_carreira].num_de_paragens - 1; i++) {
    printf("CUSTO numero: %d, total: %f\n", i,
           (*carreiras)[index_carreira].custo_paragens[i]);
  }
}

/* função secundaria que imprime as durações */
void imprime_duracao(Carreiras **carreiras, int index_carreira) {

  int i;

  for (i = 0; i < (*carreiras)[index_carreira].num_de_paragens - 1; i++) {
    printf("DURACAO numero: %d, total: %f\n", i,
           (*carreiras)[index_carreira].duracao_paragens[i]);
  }
}

/* função dedicada a modificar o nome do destino */
void modifica_destino(Carreiras **carreiras, Paragem **paragens, int i) {

  int tamanho, index_nome;
  char *nome;

  /* vai buscar a paragem imediatamente antes da ultima */
  tamanho = (*carreiras)[i].num_de_paragens - 1;

  if (tamanho == -1) {

    free((*carreiras)[i].duracao_paragens);
    free((*carreiras)[i].custo_paragens);
    free((*carreiras)[i].paragens_carreira);
    (*carreiras)[i].custo = 0.0;
    (*carreiras)[i].duracao = 0.0;
    return;
    ;
  }

  index_nome = (*carreiras)[i].paragens_carreira[tamanho];
  nome = (*paragens)[index_nome].name;
  strcpy((*carreiras)[i].destino.name, nome);
}

/* funcao dedicada a modificar o nome da origem */
void modifica_origem(Carreiras **carreiras, Paragem **paragens, int i) {

  int tamanho, index_nome;
  char *nome;

  /* definido a 1 pois vai buscar a paragem imediatamente a seguir */
  tamanho = 1;
  index_nome = (*carreiras)[i].paragens_carreira[tamanho];
  nome = (*paragens)[index_nome].name;
  strcpy((*carreiras)[i].origem.name, nome);
}

/* função dedicada a atualizar o custo da carreira após remover paragens */
void atualiza_custo(Carreiras **carreiras, int i, int index_custo,
                    int numero_paragens) {

  double custo_atual = 0.0, custo_remover = 0.0;
  int j;
  double custo_atualizado;

  /* obter o custo total */
  custo_atual = (*carreiras)[i].custo;
  /* obter o custo que vai ser removido */
  custo_remover = (*carreiras)[i].custo_paragens[index_custo];

  /* mover todos os elementos para a esquerda se for o caso */
  if (index_custo == 0) {
    for (j = 0; j < numero_paragens - 1; j++) {
      (*carreiras)[i].custo_paragens[j] = (*carreiras)[i].custo_paragens[j + 1];
    }
  }
  /* fazer a decrementação e atualizar o custo */
  custo_atualizado = custo_atual - custo_remover;
  (*carreiras)[i].custo = custo_atualizado;

  /* realocar a memoria para o tamanho necessario */
  (*carreiras)[i].custo_paragens = (double *)realloc(
      (*carreiras)[i].custo_paragens, (numero_paragens) * sizeof(double));
}

/* função dedicada a atualizar a duração da carreira após remover paragens */
void atualiza_duracao(Carreiras **carreiras, int i, int index_custo,
                      int numero_paragens) {
  double duracao_atual = 0, duracao_remover = 0;
  double duracao_atualizado;
  int j;

  /* obter a duração total */
  duracao_atual = (*carreiras)[i].duracao;
  /* obter a duração a remover */
  duracao_remover = (*carreiras)[i].duracao_paragens[index_custo];

  /* mover todos os elementos para a esquerda se for o caso */
  for (j = 0; j < numero_paragens - 1; j++) {
    (*carreiras)[i].duracao_paragens[j] =
        (*carreiras)[i].duracao_paragens[j + 1];
  }

  /* atualizar o valor da duração */
  duracao_atualizado = duracao_atual - duracao_remover;
  (*carreiras)[i].duracao = duracao_atualizado;

  /* realocar espaço para os custos atualizados */
  (*carreiras)[i].duracao_paragens = (double *)realloc(
      (*carreiras)[i].duracao_paragens, (numero_paragens) * sizeof(double));
}

/* função dedicada a apagar uma paragem por completo */
void apaga_paragem(char nome[], Paragem **paragens, Carreiras **carreiras,
                   int *numero_paragens, int *numero_carreiras) {
  int i, j, index_paragem_apagar;
  int index_paragem_atual;
  int numero_paragens_carreira = 0;
  int index_custo = 0;

  /* obter o index da paragem a apagar */
  index_paragem_apagar = get_paragem_index(nome, paragens, numero_paragens);

  for (i = 0; i < *numero_carreiras; i++) {
    /* definir sempre por cada iteraçao de modo a guardar temporariamente */
    int *paragens_atualizado = NULL;
    int numero_paragens_atualizado = 0;

    /* percorrer as paragens presentes na carreira i */
    for (j = 0; j < (*carreiras)[i].num_de_paragens; j++) {
      numero_paragens_carreira = ((*carreiras)[i].num_de_paragens) - 1;
      index_paragem_atual = (*carreiras)[i].paragens_carreira[j];

      /* caso a paragem seja uma origem ou um destino efetuar as alteracoes */
      if (index_paragem_atual == index_paragem_apagar) {
        if (index_paragem_apagar == (*carreiras)[i].paragens_carreira[0]) {
          index_custo = 0;
          atualiza_custo(carreiras, i, index_custo, numero_paragens_carreira);
          atualiza_duracao(carreiras, i, index_custo, numero_paragens_carreira);
        }
        if (index_paragem_apagar ==
            (*carreiras)[i].paragens_carreira[numero_paragens_carreira]) {
          index_custo = numero_paragens_carreira - 1;

          atualiza_custo(carreiras, i, index_custo, numero_paragens_carreira);
          atualiza_duracao(carreiras, i, index_custo, numero_paragens_carreira);
        }
      }

      if (index_paragem_atual != index_paragem_apagar) {
        /* contar as paragens dessa carreira que nao sao iguais À pretendida*/
        numero_paragens_atualizado++;
        paragens_atualizado = (int *)realloc(
            paragens_atualizado, numero_paragens_atualizado * sizeof(int));

        /* caso não haja memória */
        if (paragens_atualizado == NULL) {
          printf("No memory.");
          funcao_apaga_sistema(carreiras, paragens, numero_carreiras,
                               numero_paragens);
          exit(0);
        }

        /* se o index da paragem for superior à que apaguei mover um indice para
         * baixo */
        if (index_paragem_atual > index_paragem_apagar) {
          index_paragem_atual--;
        }

        paragens_atualizado[numero_paragens_atualizado - 1] =
            index_paragem_atual;
      }
    }

    free((*carreiras)[i].paragens_carreira);
    (*carreiras)[i].paragens_carreira = paragens_atualizado;
    (*carreiras)[i].num_de_paragens = numero_paragens_atualizado;

    /* caso de apenas restar uma paragem na carreira */
    if (numero_paragens_atualizado == 1) {

      for (j = 0; j < (*carreiras)[i].num_de_paragens; j++) {

        (*paragens)[(*carreiras)[i].paragens_carreira[j]].intersecoes--;
      }
      free((*carreiras)[i].custo_paragens);
      free((*carreiras)[i].duracao_paragens);
      free((*carreiras)[i].paragens_carreira);
      ((*carreiras)[i].custo = 0.0);
      ((*carreiras)[i].duracao = 0.0);
      ((*carreiras)[i].num_de_paragens = 0);
      break;
    }

    /* condicao para alterar o destino da carreira*/
    if (strcmp(((*paragens)[index_paragem_apagar].name),
               (*carreiras)[i].destino.name) == 0) {
      modifica_destino(carreiras, paragens, i);
    }
    /* condicao para alterar a origem da carreira */
    if (strcmp(((*paragens)[index_paragem_apagar].name),
               (*carreiras)[i].origem.name) == 0) {
      modifica_origem(carreiras, paragens, i);
    }
  }
  free((*paragens)[index_paragem_apagar].name);

  /* mover as paragens todas */
  for (i = index_paragem_apagar; i < *numero_paragens - 1; i++) {
    (*paragens)[i] = (*paragens)[i + 1];
  }

  /* decrementar o numero de paragens */
  (*numero_paragens)--;

  if (*numero_paragens == 0) {
    *paragens = NULL;
    return;
  }
  /* realocar o vetor de paragens */
  *paragens =
      (Paragem *)realloc(*paragens, (*numero_paragens) * sizeof(Paragem));

  /* caso não haja memória */
  if (*paragens == NULL) {
    printf("No memory.");
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras,
                         numero_paragens);
    exit(0);
    return;
  }
}

/* função principal do comando 'e' destinado a apagar paragens */
int funcao_remove_paragem(char comando[], Paragem **paragens,
                          Carreiras **carreiras, int *numero_paragens,
                          int *numero_carreiras) {

  char nome[buff] = "", comando2[buff] = "";
  int existe, aspas;

  aspas = has_double_quotes(comando);

  /* caso o nome da paragem tenha aspas */
  if (aspas == 1) {
    sscanf(comando, "%c \"%[^\"]\"", comando2, nome);
  }
  /* caso geral do nome da paragem */
  else {
    sscanf(comando, "%c %s", comando2, nome);
  }

  /* verificação da existência da paragem */
  existe = verifica_existe_paragem(nome, paragens, numero_paragens);

  if (!existe) {
    printf("%s: no such stop.\n", nome);
    return 0;
  }

  /* chamada da função dedicada a efetivamente apagar a paragem  */
  apaga_paragem(nome, paragens, carreiras, numero_paragens, numero_carreiras);

  return 0;
}

/*------------------ Funções associadas comando R ------------------------ */

void apaga_carreira(char *nome, Carreiras **carreiras, Paragem **paragens,
                    int *numero_carreiras, int *numero_paragens) {

  int index_carreira_apagar, index_carreira_atual, i;

  /* obter o index da carreira a apagar */
  index_carreira_apagar = get_carreira_index(nome, carreiras, numero_carreiras);

  /* decrementar a contagem de intersecoes de cada paragem */
  for (i = 0; i < (*carreiras)[index_carreira_apagar].num_de_paragens; i++) {
    if ((*paragens)[(*carreiras)[index_carreira_apagar].paragens_carreira[i]]
            .decrementada == 0) {

      (*paragens)[(*carreiras)[index_carreira_apagar].paragens_carreira[i]]
          .intersecoes--;
      (*paragens)[(*carreiras)[index_carreira_apagar].paragens_carreira[i]]
          .decrementada = 1;
    }
  }

  /* atualizar o identificador após decrementar o numero de intersecoes*/
  for (i = 0; i < (*carreiras)[index_carreira_apagar].num_de_paragens; i++) {
    (*paragens)[(*carreiras)[index_carreira_apagar].paragens_carreira[i]]
        .decrementada = 0;
  }

  /* dar free de todos os elementos alocados da carreira */
  free((*carreiras)[index_carreira_apagar].name);
  free((*carreiras)[index_carreira_apagar].origem.name);
  free((*carreiras)[index_carreira_apagar].destino.name);
  free((*carreiras)[index_carreira_apagar].paragens_carreira);
  free((*carreiras)[index_carreira_apagar].custo_paragens);
  free((*carreiras)[index_carreira_apagar].duracao_paragens);

  /* mover todas as carreiras de modo a deixar o espaco a apagar à direita */
  for (i = index_carreira_apagar; i < *numero_carreiras - 1; i++) {
    index_carreira_atual = i;
    /* mover manualmente cada elemento */
    (*carreiras)[i].name = (*carreiras)[i + 1].name;
    (*carreiras)[i].origem = (*carreiras)[i + 1].origem;
    (*carreiras)[i].destino = (*carreiras)[i + 1].destino;
    (*carreiras)[i].num_de_paragens = (*carreiras)[i + 1].num_de_paragens;
    (*carreiras)[i].paragens_carreira = (*carreiras)[i + 1].paragens_carreira;
    (*carreiras)[i].custo_paragens = (*carreiras)[i + 1].custo_paragens;
    (*carreiras)[i].duracao_paragens = (*carreiras)[i + 1].duracao_paragens;
    (*carreiras)[i].custo = (*carreiras)[i + 1].custo;
    (*carreiras)[i].duracao = (*carreiras)[i + 1].duracao;

    /* se o index for superior ao index da carreira a apagar, decrementamos */
    if (index_carreira_atual > index_carreira_apagar) {
      index_carreira_atual--;
    }
  }

  /* decrementar o numero de carreiras */
  (*numero_carreiras)--;

  /* caso fique sem carreiras em sistema */
  if (*numero_carreiras == 0) {
    *carreiras = NULL;
    return;
  }

  /* realocar a memoria novamente para apenas o necessario */
  *carreiras =
      (Carreiras *)realloc(*carreiras, (*numero_carreiras) * sizeof(Carreiras));

  if (*carreiras == NULL) {
    printf("No memory.");
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras,
                         numero_paragens);
    return;
  }
}

/* função principal do comando r*/
int funcao_remove_carreiras(char comando[], Carreiras **carreiras,
                            Paragem **paragens, int *numero_carreiras,
                            int *numero_paragens) {
  char nome[buff] = "", comando2[buff] = "";
  int existe;

  sscanf(comando, "%c %s", comando2, nome);
  /* verificação se a carreira existe */
  existe = verifica_existe_carreira(nome, carreiras, numero_carreiras);

  if (!existe) {
    printf("%s: no such line.\n", nome);
    return 0;
  }
  /* se efectivamente for o caso apagamos */
  apaga_carreira(nome, carreiras, paragens, numero_carreiras, numero_paragens);

  return 0;
}

/* função principal do comando 'a'*/
int funcao_apaga_sistema(Carreiras **carreiras, Paragem **paragens,
                         int *numero_carreiras, int *numero_paragens) {
  int i;

  /* percorrer os nomes e dar free dos mesmos */
  for (i = 0; i < *numero_paragens; i++) {
    free((*paragens)[i].name);
  }

  /* free do vector de paragens e dar reset ao numero */
  free(*paragens);
  *paragens = NULL;
  *numero_paragens = 0;

  /* percorrer o vector de carreiras e dar free dos elementos  */
  for (i = 0; i < *numero_carreiras; i++) {

    free((*carreiras)[i].name);
    if ((*carreiras)[i].num_de_paragens > 1) {
      free((*carreiras)[i].origem.name);
      free((*carreiras)[i].destino.name);
      free((*carreiras)[i].paragens_carreira);
      free((*carreiras)[i].custo_paragens);
      free((*carreiras)[i].duracao_paragens);
    }
  }
  /* free do vector e dar reset ao numero */
  free(*carreiras);
  *carreiras = NULL;
  *numero_carreiras = 0;
  return 0;
}
