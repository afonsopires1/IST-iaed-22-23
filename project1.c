/* iaed-23 - ist1102803 - project1 */
#include "tes.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_Funcao 8193
#define MAX_CARREIRAS 201
#define MAX_PARAGENS 10001
#define MAX_NOME_PARAGEM 51
#define MAX_NOME_CARREIRA 21
#define max_double 100
#define MAX_LIGACOES 30001

/* ------------------------------ Estruturas de dados ---------------------- */


typedef struct Paragem {
  char name[MAX_NOME_PARAGEM];
  double latitude;
  double longitude;
  int intersecoes;
} Paragem;

typedef struct Carreiras {
  char name[MAX_NOME_CARREIRA];
  Paragem origem;
  Paragem destino;
  int num_de_paragens;
  int paragens_carreira[MAX_LIGACOES];
  double custo;
  double duracao;
} Carreiras;


/* ---------------------------- Variaveis Globais ---------------------------*/


int numero_paragens = 0;        /* numero de paragens introduzidas */
Paragem paragens[MAX_PARAGENS]; /* vector com todas as paragens */

int numero_carreiras = 0;           /* numero de carreiras em sistema */
Carreiras carreiras[MAX_CARREIRAS]; /* vector com todas as carreiras */


/* -------------------------- Funções de Leitura ----------------------------*/


/* função dedicada a identificar casos em que o nome tenha aspas */
int has_double_quotes(char str[]) {
  int count = 0;
  int len = MAX_NOME_PARAGEM + 2;
  int i;
  if (str[2] == '"') {
    for (i = len; i > 2; i--) {
      if (str[i] == '"') {
        count++;
        break;
      }
    }
  }
  return count;
}


/* função dedicada a contar argumentos com casos especiais e especificos */
int conta_argumentos(char input[]) {
  int count = 0;
  int i;
  for (i = 1; input[i] != '\0'; i++) {
    if (input[i] == ' ' && input[i - 1] != ' ') {
      count++;
    }
  }
  if (input[i - 1] != ' ') {
    count++;
  }

  return count;
}


/* função dedicada a contar o numero de espaços brancos até encontrar aspas */
int conta_espacos_ate_aspas(char comando[]) {
  int count = 0;
  int i = 0;
  while (comando[i] != '\0' && comando[i] != '\"') {
    if (comando[i] == ' ') {
      count++;
    }
    i++;
  }
  return count;
}


/*------------------------------ Funções de Verificação -------------------- */


/* função dedicada a verificar se já existe uma paragem com o mesmo nome */
int verifica_existe_paragem(char nome[]) {
  int i;
  for (i = 0; i < numero_paragens; i++) {
    if (strcmp(nome, paragens[i].name) == 0) {
      return 1;
    }
  }
  return 0;
}


/* função dedicada a verificar se já existe uma carreira com o mesmo nome */
int verifica_existe_carreira(char nome[]) {
  int i;
  for (i = 0; i < numero_carreiras; i++) {
    if (strcmp(carreiras[i].name, nome) == 0) {
      return 1;
    }
  }
  return 0;
}


/* função dedicada a verificar se a paragem já existe na carreira */
int verifica_existe_na_carreira(char nome[], Carreiras carreiras[],
                                int num_carreiras) {
  int i, j;
  for (i = 0; i < num_carreiras; i++) {
    if (strcmp(nome, carreiras[i].name) == 0) {
      for (j = 0; j < carreiras[i].num_de_paragens; j++) {
        if (strcmp(paragens[carreiras[i].paragens_carreira[j]].name, nome) ==
            0) {
          return 1;
        }
      }
    }
  }
  return 0;
}


/* função dedicada a identificar se uma certa palavra faz parte das possiveis
 variacoes de inverso que são aceites */
int tem_inverso(char palavra[]) {
  return (strcmp(palavra, "inverso") == 0) ||
         (strcmp(palavra, "invers") == 0) || (strcmp(palavra, "inver") == 0) ||
         (strcmp(palavra, "inve") == 0) || (strcmp(palavra, "inv") == 0);
}


/*------------------------- Funções de Obtenção ----------------------------*/


/* função dedicada a obter o index da carreira atraves do nome */
int get_carreira_index(char name[], Carreiras carreiras[], int num_carreiras) {
  int i;
  for (i = 0; i < num_carreiras; i++) {
    if (strcmp(name, carreiras[i].name) == 0) {
      return i;
    }
  }
  return -1; /*caso aleatorio, a carreira existe se esta funcao for chamada*/
}


/* função dedicada a obter o index da paragem atraves do nome */
int get_paragem_index(char name[], Paragem paragens[], int num_paragens) {
  int i;
  for (i = 0; i < num_paragens; i++) {
    if (strcmp(name, paragens[i].name) == 0) {
      return i;
    }
  }
  return -1; /* caso aleatorio, a paragem existe se esta funcao for chamada*/
}


/* função dedicada a encontrar uma paragem atraves do nome */
Paragem findParagemByName(char nome[], Paragem paragens[], int num_paragens) {
  int i;
  for (i = 0; i < num_paragens; i++) {
    if (strcmp(paragens[i].name, nome) == 0) {
      return paragens[i];
    }
  }

  return paragens[0]; /*caso aleatorio, paragem existe se a funcao for chamada*/
}


/* função dedicada a obter o numero de interseções de uma paragem pelo nome */
int get_numero_intersecoes(char nome[], Paragem paragens[], int num_paragens) {
  int i;
  for (i = 0; i < num_paragens; i++) {
    if (strcpy(paragens[i].name, nome) == 0) {
      return paragens[i].intersecoes;
    }
  }
  return -1; /* caso aleatorio, a paragem existe se esta funcao for chamada */
}


/*------------------- Funções associadas comando 'p' ------------------------*/


/* função dedicada a listar as paragens pela ordem inserida */
void lista_paragens() {
  int i;
  for (i = 0; i < numero_paragens; i++) {
    printf("%s:", paragens[i].name);
    printf(" %16.12f %16.12f %d\n", paragens[i].latitude, paragens[i].longitude,
           paragens[i].intersecoes);
  }
}


/* função dedicada a listar as coordenadas de uma paragem existente através do
 nome*/
void lista_paragem_existente(char nome[]) {
  int i;
  for (i = 0; i < numero_paragens; i++) {
    if (strcmp(nome, paragens[i].name) == 0) {
      printf("%16.12f %16.12f\n", paragens[i].latitude, paragens[i].longitude);
    }
  }
}


/* função dedicada a criar uma nova paragem colocando de imediato a mesma no
 vector de paragens */
void cria_paragem(char nome[], double lat, double lon) {
  strcpy(paragens[numero_paragens].name, nome);
  paragens[numero_paragens].latitude = lat;
  paragens[numero_paragens].longitude = lon;
  paragens[numero_paragens].intersecoes = 0;
  numero_paragens++;
}


/* função principal do comando 'p' */
int funcao_p(char comando[]) {
  char comando2[Max_Funcao];
  char nome[MAX_NOME_PARAGEM];
  char latitude[max_double];
  char longitude[max_double];

  int aspas;
  int existe;
  int num_argumentos;

  double lat;
  double lon;

  aspas = has_double_quotes(comando);

  if (aspas == 1) {
    /* caso do nome da paragem ter aspas*/
    num_argumentos = sscanf(comando, "%c \"%[^\"]\" %s %[^\n]", comando2, nome,
                            latitude, longitude);
  } else {
    /* caso do nome da paragem não ter aspas */
    num_argumentos =
        sscanf(comando, "%c %s %s %[^\n]", comando2, nome, latitude, longitude);
  }

  /* transformar o que foi lido em string em double */
  lat = atof(latitude);
  lon = atof(longitude);

  /* verificar se a paragem existe */
  existe = verifica_existe_paragem(nome);

  /* caso o argumento passado seja apenas 'p' */
  if (num_argumentos == 1) {
    lista_paragens();
  }
  /* caso o argumento passado seja 'p' seguido de um nome que não existe */
  if (num_argumentos == 2 && existe != 1) {
    printf("%s: no such stop.\n", nome);
  }
  /* caso o argumento passado seja 'p' seguido de um nome que existe */
  if (num_argumentos == 2 && existe == 1) {
    lista_paragem_existente(nome);
  }

  /* condição para se criar uma nova paragem */
  if (num_argumentos == 4) {
    /* caso se tente criar uma paragem com um nome existente */
    if (existe == 1) {
      printf("%s: stop already exists.\n", nome);
      return 0;
    }
    cria_paragem(nome, lat, lon);
  }
  return 0;
}


/*------------------- Funções associadas comando 'c' ------------------------*/


/* função dedicada a listar as carreiras pela ordem inserida */
void lista_carreiras() {
  int i;
  for (i = 0; i < numero_carreiras; i++) {
    printf("%s ", carreiras[i].name);
    if (carreiras[i].num_de_paragens > 0) {
      printf("%s ", carreiras[i].origem.name);
      printf("%s ", carreiras[i].destino.name);
    }
    printf("%d ", carreiras[i].num_de_paragens);
    printf("%.2f ", carreiras[i].custo);
    printf("%.2f\n", carreiras[i].duracao);
  }
}


/* função dedicada a listar as paragens de uma certa carreira pela ordem
 * inserida */
void lista_paragens_carreira(int i) {
  int k;
  for (k = 0; k < carreiras[i].num_de_paragens; k++) {
    printf("%s", paragens[carreiras[i].paragens_carreira[k]].name);
    if (k != carreiras[i].num_de_paragens - 1) {
      printf(", ");
    }
  }
  printf("\n");
}


/* função dedicada a listar as paragens de uma certa carreira pelo inverso da
 * ordem inserida */
void lista_paragens_carreira_inverso(int i) {
  int j;
  for (j = carreiras[i].num_de_paragens - 1; j >= 0; j--) {
    printf("%s", paragens[carreiras[i].paragens_carreira[j]].name);
    if (j != 0) {
      printf(", ");
    }
  }
  printf("\n");
}


/* função dedicada a criar uma carreira com um dado nome e os restantes dados a
 * zero/vazios*/
void cria_carreira(char nome[]) {
  strcpy(carreiras[numero_carreiras].name, nome);
  strcpy(carreiras[numero_carreiras].origem.name, "");
  strcpy(carreiras[numero_carreiras].destino.name, "");
  carreiras[numero_carreiras].num_de_paragens = 0;
  carreiras[numero_carreiras].custo = 0.0;
  carreiras[numero_carreiras].duracao = 0.0;
  numero_carreiras++;
}


/* função principal do comando 'c' */
int funcao_c(char comando[]) {
  int i;
  int num_argumentos;
  char comando2[Max_Funcao];
  char nome[MAX_NOME_CARREIRA];
  char palavra[8]; /*para guardar especificamente a palavra inverso ou inv */
  int existe = 0;

  num_argumentos = sscanf(comando, "%c %s %[^\n]", comando2, nome, palavra);

  /* verificação se a carreira existe */
  existe = verifica_existe_carreira(nome);

  /* caso o argumento seja apenas 'c' listar todas as carreiras */
  if (num_argumentos == 1) {
    lista_carreiras();
  }

  if (existe == 1) {
    /* ciclo que vai percorrer todas as carreiras até encontrar a carreira com o
     * nome correspondente */
    for (i = 0; i < MAX_CARREIRAS; i++) {
      if (strcmp(carreiras[i].name, nome) == 0) {

        /* caso a carreira em questão não tenha paragens */
        if (carreiras[i].num_de_paragens == 0) {
          return 0;
        }

        /* dar print das paragens da carreira ordem normal */
        if (num_argumentos == 2) {
          lista_paragens_carreira(i);
          return 0;
        }
        /* dar print das paragens da carreira ordem inversa */
        if (num_argumentos == 3 && tem_inverso(palavra) == 1) {
          lista_paragens_carreira_inverso(i);
          return 0;
        }
        /* levantar o erro de palavra errada */
        if (num_argumentos == 3 && tem_inverso(palavra) != 1) {
          printf("incorrect sort option.\n");
          return 0;
        }
        break;
      }
    }
  }

  /* caso não exista cria-se */
  if (num_argumentos == 2 && existe != 1) {
    cria_carreira(nome);
    return 0;
  }
  /* caso se procure por uma carreira que não exista */
  else if (num_argumentos > 1) {
    if (existe != 1) {
      return 0;
    }
  }
  return 0;
}


/*------------------- Funções associadas comando 'l' ------------------------*/


/* função dedicada a criar uma nova ligação de raiz */
void cria_ligacao(int index_carreira, int origem, int destino,
                  char custo[max_double], char duracao[max_double]) {

  /* adicionar ao vector de paragens da carreira*/
  carreiras[index_carreira].paragens_carreira[0] = origem;
  /* atualizar a origem da carreira*/
  strcpy(carreiras[index_carreira].origem.name, paragens[origem].name);
  carreiras[index_carreira].num_de_paragens++;

  /* adicionar ao vector de paragens da carreira*/
  carreiras[index_carreira].paragens_carreira[1] = destino;
  /* atualizar o destino da carreira*/
  strcpy(carreiras[index_carreira].destino.name, paragens[destino].name);
  carreiras[index_carreira].num_de_paragens++;

  carreiras[index_carreira].custo = atof(custo);
  carreiras[index_carreira].duracao = atof(duracao);

  paragens[origem].intersecoes++;
  paragens[destino].intersecoes++;
}


/* função dedicada ao caso particular de carreira circular colocando o destino
com a mesma paragem da origem */
void ligacao_circular(int index_carreira, int paragens_atual, int destino,
                      char custo[max_double], char duracao[max_double]) {

  double custo_atual;
  double duracao_atual;

  /* atualizar o nome do destino da carreira */
  strcpy(carreiras[index_carreira].destino.name, paragens[destino].name);
  /* atualizar o vector das paragens com o novo destino */
  carreiras[index_carreira].paragens_carreira[paragens_atual] = destino;

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  carreiras[index_carreira].custo += custo_atual;     /* atualizar o custo   */
  carreiras[index_carreira].duracao += duracao_atual; /* atualizar a duração */

  carreiras[index_carreira].num_de_paragens++;
}


/* função dedicada a adicionar um novo destino a uma carreira */
void adiciona_destino(int index_carreira, int paragens_atual, int destino,
                      char custo[max_double], char duracao[max_double]) {

  double custo_atual;
  double duracao_atual;

  /* atualizar o destino da carreira */
  strcpy(carreiras[index_carreira].destino.name, paragens[destino].name);
  /* atualizar o vector da carreira  */
  carreiras[index_carreira].paragens_carreira[paragens_atual] = destino;

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  carreiras[index_carreira].custo += custo_atual;     /* atualizar o custo   */
  carreiras[index_carreira].duracao += duracao_atual; /* atualizar a duração */

  /* aumentar o contador de paragens da carreira alterada */
  carreiras[index_carreira].num_de_paragens++;

  /* aumentar o contador de carreiras de cada  paragem individual */
  paragens[destino].intersecoes++;
}


/* função dedicada a adicionar uma nova origem a uma carreira */
void adiciona_origem(int index_carreira, int origem, char custo[max_double],
                     char duracao[max_double]) {

  double custo_atual;
  double duracao_atual;

  /* atualizar a origem da carreira */
  strcpy(carreiras[index_carreira].origem.name, paragens[origem].name);
  /* atualizar o vector da carreira */
  carreiras[index_carreira].paragens_carreira[0] = origem;

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  carreiras[index_carreira].custo += custo_atual;     /* atualizar o custo   */
  carreiras[index_carreira].duracao += duracao_atual; /* atualizar a duração */

  carreiras[index_carreira].num_de_paragens++;
  paragens[origem].intersecoes++;
}


/* função dedicada a mover todos os elementos de um vector um index para a
direita */
void shift_right(int vector[], int size) {
  int i, ultimo;
  ultimo =
      vector[size -
             1]; /*guardar o ultimo elemento do vector na variavel ultimo */

  for (i = size - 1; i > 0; i--) {
    vector[i] = vector[i - 1]; /* atribuir o valor anterior ao valor atual */
  }
  vector[0] = ultimo;
}


/* função principal do comando 'l' */
int funcao_l(char comando[]) {

  char nome[MAX_NOME_CARREIRA];
  char nome_origem[MAX_NOME_PARAGEM];
  char nome_destino[MAX_NOME_PARAGEM];
  char custo[max_double];
  char duracao[max_double];
  char comando2[Max_Funcao];

  int pos_aspas;
  int arg;

  int index_carreira;
  int paragens_atual; /* numero de paragens pertencentes a uma carreira */

  int index_origem;
  int index_destino;

  arg = conta_argumentos(comando);

  /* caso de terem os dois aspas */
  if (arg == 8) {
    sscanf(comando, "%c %s \"%[^\"]\" \"%[^\"]\" %s %[^\n]", comando2, nome,
           nome_origem, nome_destino, custo, duracao);
  }
  if (arg != 8) {
    pos_aspas = conta_espacos_ate_aspas(comando);

    /* caso do nome do destino ter aspas */
    if (pos_aspas == 3) {
      sscanf(comando, "%c %s %s \"%[^\"]\" %s %[^\n]", comando2, nome,
             nome_origem, nome_destino, custo, duracao);
    }

    /* caso do nome de origem ter aspas */
    if (pos_aspas == 2) {
      sscanf(comando, "%c %s \"%[^\"]\" %s %s %[^\n]", comando2, nome,
             nome_origem, nome_destino, custo, duracao);
    }

    /* caso de nenhum ter aspas */
    if (pos_aspas == 5) {
      sscanf(comando, "%c %s %s %s %s %[^\n]", comando2, nome, nome_origem,
             nome_destino, custo, duracao);
    }
  }

  /* verificações iniciais antes de se poder avançar */

  if (verifica_existe_carreira(nome) == 0) {
    printf("%s: no such line.\n", nome);
    return 0;
  }
  if (verifica_existe_paragem(nome_origem) == 0) {
    printf("%s: no such stop.\n", nome_origem);
    return 0;
  }
  if (verifica_existe_paragem(nome_destino) == 0) {
    printf("%s: no such stop.\n", nome_destino);
    return 0;
  }
  if (atof(custo) < 0 || atof(duracao) < 0) {
    printf("negative cost or duration.\n");
    return 0;
  }

  /* obter os indexes necessários para realizar as operações da função L */
  index_carreira = get_carreira_index(nome, carreiras, numero_carreiras);
  index_origem = get_paragem_index(nome_origem, paragens, numero_paragens);
  index_destino = get_paragem_index(nome_destino, paragens, numero_paragens);

  /* situacao de carreira vazia, adicionar de imediato à origem e ao destino */
  if (carreiras[index_carreira].num_de_paragens == 0) {
    cria_ligacao(index_carreira, index_origem, index_destino, custo, duracao);
    return 0;
  }

  /* numero de paragens pertencente a uma carreira */
  paragens_atual = carreiras[index_carreira].num_de_paragens;
  /* index ultima paragem registada */
  paragens_atual--;

  /* verificar se pelo menos um dos extremos dados está presente */
  if (strcmp(paragens[carreiras[index_carreira].paragens_carreira[0]].name,
             nome_destino) != 0 &&
      strcmp(
          paragens[carreiras[index_carreira].paragens_carreira[paragens_atual]]
              .name,
          nome_origem) != 0) {
    printf("link cannot be associated with bus line.\n");
    return 0;
  }

  /* condicao de carreira ser circular */
  if ((strcmp(
           paragens[carreiras[index_carreira].paragens_carreira[paragens_atual]]
               .name,
           nome_origem) == 0) &&
      (strcmp(paragens[carreiras[index_carreira].paragens_carreira[0]].name,
              nome_destino) == 0)) {

    /* somar para ficarmos no index vazio a ser alterado no vector de paragens
     */
    paragens_atual++;
    ligacao_circular(index_carreira, paragens_atual, index_destino, custo,
                     duracao);
    return 0;
  }

  /* condição para adicionar novo destino */
  if (strcmp(
          paragens[carreiras[index_carreira].paragens_carreira[paragens_atual]]
              .name,
          nome_origem) == 0) {
    paragens_atual++;
    adiciona_destino(index_carreira, paragens_atual, index_destino, custo,
                     duracao);
    return 0;
  }

  /* condição para adicionar nova origem */
  if (strcmp(paragens[carreiras[index_carreira].paragens_carreira[0]].name,
             nome_destino) == 0) {

    /* mover o vector todo uma posição para a direita */
    shift_right(carreiras[index_carreira].paragens_carreira,
                carreiras[index_carreira].num_de_paragens + 1);

    adiciona_origem(index_carreira, index_origem, custo, duracao);
    return 0;
  }
  return 0;
}


/*------------------- Funções associadas comando 'i' ------------------------*/


void bubbleSort(char vector[][MAX_NOME_CARREIRA], int size) {
  int i, j;
  for (i = 0; i < size - 1; i++) {
    for (j = 0; j < size - i - 1; j++) {
      if (strcmp(vector[j], vector[j + 1]) > 0) {
        char temp[MAX_NOME_CARREIRA];
        strcpy(temp, vector[j]);
        strcpy(vector[j], vector[j + 1]);
        strcpy(vector[j + 1], temp);
      }
    }
  }
}


/* função principal do comando 'i' */
int funcao_i() {
  int i, j, k, l, count;
  /* array a guardar os nomes das carreiras para organizar */
  char carreiras_para_organizar[MAX_CARREIRAS][MAX_NOME_CARREIRA];

  for (i = 0; i < numero_paragens; i++) {
    /* condição de ter mais do que uma carreira associada */
    if (paragens[i].intersecoes > 1) {
      printf("%s %d:", paragens[i].name, paragens[i].intersecoes);

      count = 0;

      /* iterar sobre as carreiras e verificar as paragens incluidas em cada */
      for (j = 0; j < numero_carreiras; j++) {
        for (k = 0; k < carreiras[j].num_de_paragens; k++) {
          if (strcmp(paragens[i].name,
                     paragens[carreiras[j].paragens_carreira[k]].name) == 0) {
            /*enviar para o array que vai ser organizado e contabilizar a
             * contagem*/
            strcpy(carreiras_para_organizar[count], carreiras[j].name);

            count++;
            break;
          }
        }
      }
      /* organizar todas as carreiras até à quantidade de carreiras */

      bubbleSort(carreiras_para_organizar, count);
      for (l = 0; l < count; l++) {
        printf(" %s", carreiras_para_organizar[l]);
      }
      printf("\n");
    }
  }
  return 0;
}


/* --------------------------Função principal--------------------------------*/


int main() {
  char funcoes[Max_Funcao];
  while (fgets(funcoes, Max_Funcao, stdin) != NULL) {
    if (funcoes[0] == 'q') {
      break;
    } else if (funcoes[0] == 'p') {
      funcao_p(funcoes);
    } else if (funcoes[0] == 'c') {
      funcao_c(funcoes);
    } else if (funcoes[0] == 'l') {
      funcao_l(funcoes);
    } else if (funcoes[0] == 'i') {
      funcao_i();
    }
  }

  return 0;
}
