#include "project2.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------- Funções de Leitura --------------------------- */

/* dedicada a contar argumentos com casos especiais e especificos */
int conta_argumentos_funcao_l(char input[]) {
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
  if (has_double_quotes(input)) {
    count = count - 2;
    if (count == 5) {
      count = count - 2;
    }

    return count;
  }
  count = count - 1;
  return count;
}

/* função dedicada a identificar casos em que o nome tenha aspas */
int has_double_quotes(char str[]) {
  int i, t;
  t = strlen(str);
  for (i = 0; i < t; i++) {
    if (str[i] == '"') {
      return 1;
    }
  }
  return 0;
}

/* função dedicada a verificar se já existe uma paragem com o mesmo nome */
int verifica_existe_paragem(char *nome, Paragem **paragens,
                            int *numero_paragens) {
  int i, tam;
  tam = *numero_paragens;
  for (i = 0; i < tam; i++) {
    if (strcmp(nome, (*paragens)[i].name) == 0) {
      return 1;
    }
  }
  return 0;
}

/* função dedicada a verificar se já existe uma carreira com o mesmo nome */
int verifica_existe_carreira(char *nome, Carreiras **carreiras,
                             int *numero_carreiras) {
  int i;

  if (*numero_carreiras == 0) {
    return 0;
  }
  for (i = 0; i < *numero_carreiras; i++) {
    if (strcmp(nome, (*carreiras)[i].name) == 0) {
      return 1;
    }
  }
  return 0;
}

/* função dedicada a contar argumentos */
int conta_argumentos(char comando[]) {

  int i, t, res = 1, aspas;
  aspas = has_double_quotes(comando);
  t = strlen(comando);
  for (i = 0; i < t; i++) {
    if (comando[i] == ' ' && comando[i + 1] != ' ') {
      res++;
    }
  }
  if (aspas) {
    return res - 1;
  } else {
    return res;
  }
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

/* ----------------------- Funções de Obtenção ----------------------------- */

/* função dedicada a obter o index da paragem atraves do nome */
int get_paragem_index(char *nome, Paragem **paragens, int *numero_paragens) {
  int i, tam;
  tam = *numero_paragens;
  for (i = 0; i < tam; i++) {
    if (strcmp(nome, (*paragens)[i].name) == 0) {
      return i;
    }
  }
  return -1; /* paragem não encontrada */
 
}


/* função dedicada a obter o index da carreira atraves do nome */
int get_carreira_index(char name[], Carreiras **carreiras, int *num_carreiras) {
  int i, tam;
  tam = *num_carreiras;
  for (i = 0; i < tam; i++) {
    if (strcmp(name, (*carreiras)[i].name) == 0) {
      return i;
    }
  }
  return -1; /* carreira não encontrada */
}


/* função dedicada a verificar se uma paragem está presente na carreira */
int verifica_presente_carreira(Carreiras **carreiras, Paragem **paragens,
                               int *numero_carreiras, int *numero_paragens,
                               char nome_paragem[], char nome_carreira[]) {

  int i, j;
  int index_paragem, index_paragem_atual;
  int index_carreira, index_carreira_atual;

  index_carreira =
      get_carreira_index(nome_carreira, carreiras, numero_carreiras);
  index_paragem = get_paragem_index(nome_paragem, paragens, numero_paragens);

  for (i = 0; i < *numero_carreiras; i++) {
    index_carreira_atual =
        get_carreira_index((*carreiras)[i].name, carreiras, numero_carreiras);

    if (index_carreira == index_carreira_atual) {

      for (j = 0; j < (*carreiras)[index_carreira_atual].num_de_paragens; j++) {
        index_paragem_atual =
            (*carreiras)[index_carreira_atual].paragens_carreira[j];

        if (index_paragem == index_paragem_atual) {

          return 1;
        }
      }
    }
  }
  return 0;
}


/*-------------------------------------------------------------------------- */
/*-------------------------------------------------------------------------- */
/* ------------------funções do primeiro projeto---------------------------- */
/*-------------------------------------------------------------------------- */
/*-------------------------------------------------------------------------- */



/*---------------- Funções associadas comando P ---------------------------- */


/* função dedicada a listar as paragens pela ordem inserida */
void lista_paragens(Paragem *paragens, int *numero_paragens) {
  int i, tam;
  tam = *numero_paragens;
  for (i = 0; i < tam; i++) {
    printf("%s:", paragens[i].name);
    printf(" %16.12f %16.12f %d\n", paragens[i].latitude, paragens[i].longitude,
           paragens[i].intersecoes);
  }
}


/* função dedicada a listar as coordenadas de uma paragem existente */
void lista_paragem_existente(Paragem *paragens, int *numero_paragens,
                             char nome[]) {
  
  int i, tam;
  tam = *numero_paragens;
  for (i = 0; i < tam; i++) {
    if (strcmp(nome, paragens[i].name) == 0) {
      printf("%16.12f %16.12f\n", paragens[i].latitude, paragens[i].longitude);
    }
  }
}


/* função dedicada a criar uma nova paragem */
void cria_paragem(Carreiras **carreiras, Paragem **paragens,
                  int *numero_paragens, int *numero_carreiras, char nome[],
                  double lat, double lon) {

  *paragens =
      (Paragem *)realloc(*paragens, (*numero_paragens + 1) * sizeof(Paragem));
  
  /* caso não haja memória */
  if (*paragens == NULL) {
    printf("No memory.");
    free(*paragens);
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras, numero_paragens);
    exit(0);
  }
  
  /* alocar o memória para o tamanho do nome */
  (*paragens)[*numero_paragens].name = (char *)malloc(strlen(nome) + 1);

  /* caso não haja memória */
  if ((*paragens)[*numero_paragens].name == NULL) {
    printf("No memory.");
    free((*paragens)[*numero_paragens].name);
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras, numero_paragens);
    exit(0);
  }

  /* inicializar os valores da paragem criada */
  strcpy((*paragens)[*numero_paragens].name, nome);
  (*paragens)[*numero_paragens].latitude = lat;
  (*paragens)[*numero_paragens].longitude = lon;
  (*paragens)[*numero_paragens].intersecoes = 0;
  (*paragens)[*numero_paragens].decrementada = 0;
  (*numero_paragens)++;
}


/* função principal do comando p*/
int funcao_gerencia_paragens(char comando[], Paragem **paragens,
                             Carreiras **carreiras, int *numero_paragens,
                             int *numero_carreiras) {

  int aspas, existe, args;

  char comando2[buff], nome_paragem[buff] = "";
  char lati[buff] = "", longi[buff] = "";

  double lat, lon;

  /* verificar se o nome da paragem tem aspas */
  aspas = has_double_quotes(comando);

  /* os diferentes casos de leitura de nome */
  if (aspas == 1) {
    args = sscanf(comando, "%c \"%[^\"]\" %s %s", comando2, nome_paragem, lati,
                  longi);
  } else {
    args =
        sscanf(comando, "%c %s %s %[^\n]", comando2, nome_paragem, lati, longi);
  }

  lat = atof(lati), lon = atof(longi);

  /* verificação se a paragem existe */
  existe = verifica_existe_paragem(nome_paragem, paragens, numero_paragens);

  /* caso o input seja apenas 'p' */
  if (args == 1) {
    lista_paragens(*paragens, numero_paragens);
    return 0;
  }

  /* caso o argumento passado seja 'p' seguido de um nome de uma paragem */
  if (args == 2) {
    if (existe == 1) {
      lista_paragem_existente(*paragens, numero_paragens, nome_paragem);
    }
    /* caso o argumento passado seja 'p' com um nome não existente */
    if (existe != 1) {
      printf("%s: no such stop.\n", nome_paragem);
    }
  }

  /* condição para se criar uma nova paragem */
  if (args == 4) {
    /* caso se tente criar uma paragem com um nome existente */
    if (existe == 1) {
      printf("%s: stop already exists.\n", nome_paragem);
      return 0;
    }
    cria_paragem(carreiras, paragens, numero_paragens, numero_carreiras,
                 nome_paragem, lat, lon);
  }
  return 0;
}


/*-------------------- Funções associadas comando C -----------------------*/


/* função dedicada a listar as carreiras pela ordem inserida */
void lista_carreiras(Carreiras *carreiras, int *numero_carreiras) {
  int i, tam;
  tam = *numero_carreiras;
  for (i = 0; i < tam; i++) {

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


/* dedicada a listar as paragens de uma carreira pela ordem inserida */
void lista_paragens_carreira(int index, Carreiras **carreiras,
                             Paragem **paragens) {

  int k;
  for (k = 0; k < (*carreiras)[index].num_de_paragens; k++) {
    printf("%s", (*paragens)[(*carreiras)[index].paragens_carreira[k]].name);
    if (k != (*carreiras)[index].num_de_paragens - 1) {
      printf(", ");
    }
  }
  printf("\n");
}


/* dedicada a listar as paragens de uma carreira pela ordem inversa */
void lista_paragens_carreira_inverso(int index, Carreiras **carreiras,
                                     Paragem **paragens) {

  int j;
  for (j = (*carreiras)[index].num_de_paragens - 1; j >= 0; j--) {
    printf("%s", (*paragens)[(*carreiras)[index].paragens_carreira[j]].name);
    if (j != 0) {
      printf(", ");
    }
  }
  printf("\n");
}


/* função dedicada a criar uma carreira */
void cria_carreira(Carreiras **carreiras, Paragem **paragens,
                   int *numero_carreiras, int *numero_paragens, char nome[]) {

  /* alocar tamanho para a carreira a ser criada */
  *carreiras = (Carreiras *)realloc(*carreiras, (*numero_carreiras + 1) *
                                                    sizeof(Carreiras));

  /* caso não haja memória */
  if (*carreiras == NULL) {
    printf("No memory.");
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras, numero_paragens);
    exit(0);
  }

  /* alocar tamanho para o novo nome */
  (*carreiras)[*numero_carreiras].name =
      (char *)malloc(sizeof(char) * (strlen(nome) + 1));
  
  /* alocar tamanho para o novo nome da origem */
  (*carreiras)[*numero_carreiras].origem.name =
      (char *)malloc(1 * sizeof(char));
  
  /* alocar tamanho para o novo nome do destino */
  (*carreiras)[*numero_carreiras].destino.name =
      (char *)malloc(1 * sizeof(char));

  /* caso não haja memória para algum dos nomes */
  if ((*carreiras)[*numero_carreiras].name == NULL ||
      (*carreiras)[*numero_carreiras].origem.name == NULL ||
      (*carreiras)[*numero_carreiras].destino.name == NULL) {
    printf("No memory.");
    funcao_apaga_sistema(carreiras, paragens, numero_carreiras, numero_paragens);
    exit(0);
  }

  /* inicializar a null */
  (*carreiras)[*numero_carreiras].paragens_carreira = NULL;
  (*carreiras)[*numero_carreiras].custo_paragens = NULL;
  (*carreiras)[*numero_carreiras].duracao_paragens = NULL;

  /* atualizar os novos nomes */
  strcpy((*carreiras)[*numero_carreiras].name, nome);
  strcpy((*carreiras)[*numero_carreiras].origem.name, "");
  strcpy((*carreiras)[*numero_carreiras].destino.name, "");

  /* inicializar as variaveis da carreira */
  (*carreiras)[*numero_carreiras].num_de_paragens = 0;
  (*carreiras)[*numero_carreiras].custo = 0.0;
  (*carreiras)[*numero_carreiras].duracao = 0.0;

  /*atualizar o numero de carreiras */
  (*numero_carreiras)++;
}


/* função dedicada a identificar se tem variações de inverso */
int tem_inverso(char palavra[]) {
  return (strcmp(palavra, "inverso") == 0) ||
         (strcmp(palavra, "invers") == 0) || (strcmp(palavra, "inver") == 0) ||
         (strcmp(palavra, "inve") == 0) || (strcmp(palavra, "inv") == 0);
}


/* função principal do comando 'c' */
int funcao_gerencia_carreiras(char comando[], Carreiras **carreiras,
                              Paragem **paragens, int *numero_carreiras,
                              int *numero_paragens) {

  char nome_carreira[buff] = "", comando2[buff] = "", palavra[buff] = "";
  int index_carreira = 0, num_argumentos = 0, existe = 0;

  num_argumentos =
      sscanf(comando, "%c %s %s", comando2, nome_carreira, palavra);

  /* verificar se a carreira existe */
  existe = verifica_existe_carreira(nome_carreira, carreiras, numero_carreiras);

  /* caso o argumento seja apenas 'c' listar todas as carreiras */
  if (num_argumentos == 1 && numero_carreiras != 0) {
    lista_carreiras(*carreiras, numero_carreiras);
  }

  if (num_argumentos == 2 && existe == 1) {

    /* ir buscar o index da carreira */
    index_carreira =
        get_carreira_index(nome_carreira, carreiras, numero_carreiras);

    lista_paragens_carreira(index_carreira, carreiras, paragens);
  }

  if (num_argumentos == 3 && existe == 1) {
    index_carreira =
        get_carreira_index(nome_carreira, carreiras, numero_carreiras);

    if (tem_inverso(palavra) != 0) {
      lista_paragens_carreira_inverso(index_carreira, carreiras, paragens);
    } else {
      /* caso a palavra incluida não seja uma variação de inverso */
      printf("incorrect sort option.\n");
      return 0;
    }
  }

  /* caso não exista cria-se */
  if (num_argumentos == 2 && existe != 1) {
    cria_carreira(carreiras, paragens, numero_carreiras, numero_paragens,
                  nome_carreira);
    return 0;
  }

  return 0;
}

/*------------------- Funções associadas comando L ------------------------- */


/* função dedicada a mover todos os elementos de um vector para a direita */
void shift_right(int vector[], int size) {
  int i, ultimo;
  
  /*guardar o ultimo elemento do vector na variavel ultimo */
  ultimo = vector[size - 1]; 

  for (i = size - 1; i > 0; i--) {
    /* atribuir o valor anterior ao valor atual */
    vector[i] = vector[i - 1]; 
  }
  vector[0] = ultimo;
}


/* função dedicada a mover todos os elementos de um vector para a direita */
void shift_right_float(double vector[], int size) {

  int i;
  double ultimo;

  ultimo = vector[size - 1]; 
  for (i = size - 1; i > 0; i--) {
    vector[i] = vector[i - 1];
  }
  vector[0] = ultimo;
}


/* função dedicada a criar uma nova ligação de raiz */
void cria_ligacao(int index_carreira, int origem, int destino, char custo[buff],
                  char duracao[buff], Carreiras **carreiras,
                  Paragem **paragens) {

  char *nome_origem, *nome_destino;
  int tamanho;

  nome_origem = (*paragens)[origem].name;
  nome_destino = (*paragens)[destino].name;

  /* alocar o espaço para as paragens presentes na carreira */
  (*carreiras)[index_carreira].paragens_carreira = (int *)realloc(
      (*carreiras)[index_carreira].paragens_carreira, sizeof(int) * 2);

  /* alocar espaço para o tamanho o novo nome de origem */
  tamanho = strlen(nome_origem);

  (*carreiras)[index_carreira].origem.name = (char *)realloc(
      (*carreiras)[index_carreira].origem.name, sizeof(char) * tamanho + 1);

  /* alocar espaço para o tamanho o novo nome de destino */
  tamanho = strlen(nome_destino);

  (*carreiras)[index_carreira].destino.name = (char *)realloc(
      (*carreiras)[index_carreira].destino.name, sizeof(char) * tamanho + 1);

  /* alocar espaço para guardar os novos custos e durações */
  (*carreiras)[index_carreira].custo_paragens = (double *)realloc(
      (*carreiras)[index_carreira].custo_paragens, sizeof(double) * 1 + 1);

  (*carreiras)[index_carreira].duracao_paragens = (double *)realloc(
      (*carreiras)[index_carreira].duracao_paragens, sizeof(double) * 1 + 1);

  
  /* adicionar ao vetor de paragens da cadeira*/
  (*carreiras)[index_carreira].paragens_carreira[0] = origem;
  
  /* atualizar o nome da origem */
  strcpy((*carreiras)[index_carreira].origem.name, (*paragens)[origem].name);
  
  /* incrementar o numero de paragens na carreira */
  (*carreiras)[index_carreira].num_de_paragens++;

  /* adicionar ao vetor de paragens da cadeira */
  (*carreiras)[index_carreira].paragens_carreira[1] = destino;
  
  /* atualizar o nome do destino */
  strcpy((*carreiras)[index_carreira].destino.name, (*paragens)[destino].name);
  
  /*incrementar o numero de paragens na carreira */
  (*carreiras)[index_carreira].num_de_paragens++;

  
  /* atribuir o custo e duração inicial */
  (*carreiras)[index_carreira].custo = atof(custo);
  (*carreiras)[index_carreira].duracao = atof(duracao);

  /* colocar o valor do custo no array dos custos */
  (*carreiras)[index_carreira].custo_paragens[0] = atof(custo);
  (*carreiras)[index_carreira].duracao_paragens[0] = atof(duracao);


  /* caso a ligação a criar seja logo uma circular */
  if (strcmp(nome_origem, nome_destino) == 0) {
    (*paragens)[origem].intersecoes++;
  } else {
    (*paragens)[origem].intersecoes++;
    (*paragens)[destino].intersecoes++;
  }
}

/* função dedicada a adicionar um novo destino a uma carreira */
void adiciona_destino(int index_carreira, int destino, char custo[buff],
                      char duracao[buff], Carreiras **carreiras,
                      Paragem **paragens, int *numero_carreiras,
                      int *numero_paragens) {

  double custo_atual, duracao_atual;
  int numero_de_paragens;
  char *nome_carreira;
  char *nome_paragem;
  int numero_custo;

  /* obter os nomes */
  nome_paragem = (*paragens)[destino].name;
  nome_carreira = (*carreiras)[index_carreira].name;

  /* verificar se está presente na carreira */
  if (verifica_presente_carreira(carreiras, paragens, numero_carreiras,
                                 numero_paragens, nome_paragem,
                                 nome_carreira) == 0) {
    /* aumentar o contador de carreiras de cada paragem individual */
    (*paragens)[destino].intersecoes++;
  }

  /* obter o numero de paragens na carreira */
  numero_de_paragens = (*carreiras)[index_carreira].num_de_paragens;

  /* devido à origem e destino original serem 2 ex no caderno */
  numero_custo = numero_de_paragens - 1;

  /* alocar espaço no vetor de paragens na carreira */
  (*carreiras)[index_carreira].paragens_carreira =
      (int *)realloc((*carreiras)[index_carreira].paragens_carreira,
                     sizeof(int) * (numero_de_paragens + 1));

  /* realloc do custo */
  (*carreiras)[index_carreira].custo_paragens =
      (double *)realloc((*carreiras)[index_carreira].custo_paragens,
                        sizeof(double) * (numero_custo + 1));

  /* realloc da duração */
  (*carreiras)[index_carreira].duracao_paragens =
      (double *)realloc((*carreiras)[index_carreira].duracao_paragens,
                        sizeof(double) * (numero_custo + 1));

  /* realloc do nome de destino */
  (*carreiras)[index_carreira].destino.name =
      realloc((*carreiras)[index_carreira].destino.name,
              sizeof(char) * strlen(nome_paragem) + 1);

  /* atualizar o novo nome de destino */
  strcpy((*carreiras)[index_carreira].destino.name, (*paragens)[destino].name);

  /* atualizar o vetor da carreira */
  (*carreiras)[index_carreira].paragens_carreira[numero_de_paragens] = destino;

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  /* atualizar o vetor de custos */
  (*carreiras)[index_carreira].custo_paragens[numero_custo] = custo_atual;
  (*carreiras)[index_carreira].duracao_paragens[numero_custo] = duracao_atual;

  /* atualizar o custo e duração global da carreira */
  (*carreiras)[index_carreira].custo += custo_atual;
  (*carreiras)[index_carreira].duracao += duracao_atual;

  /* aumentar o contador de paragens da carreira alterada */
  (*carreiras)[index_carreira].num_de_paragens++;
}

/* função dedicada a adicionar uma nova origem a uma carreira */
void adiciona_origem(int index_carreira, int origem, char custo[buff],
                     char duracao[buff], Carreiras **carreiras,
                     Paragem **paragens, int *numero_carreiras,
                     int *numero_paragens) {

  double custo_atual, duracao_atual;
  int numero_de_paragens_presentes;
  char *nome_paragem, *nome_carreira;

  nome_paragem = (*paragens)[origem].name;
  nome_carreira = (*carreiras)[index_carreira].name;

  /* verificar se já está presente na carreira */
  if ((verifica_presente_carreira(carreiras, paragens, numero_carreiras,
                                  numero_paragens, nome_paragem,
                                  nome_carreira)) == 0) {
    (*paragens)[origem].intersecoes++;
  }

  numero_de_paragens_presentes = (*carreiras)[index_carreira].num_de_paragens;

  /* dar realloc das paragens presentes na carreira */
  (*carreiras)[index_carreira].paragens_carreira =
      (int *)realloc((*carreiras)[index_carreira].paragens_carreira,
                     sizeof(int) * (numero_de_paragens_presentes) + 1);

  /* dar realloc com o numero de paragens presentes */
  (*carreiras)[index_carreira].custo_paragens =
      (double *)realloc((*carreiras)[index_carreira].custo_paragens,
                        sizeof(double) * (numero_de_paragens_presentes));
  (*carreiras)[index_carreira].duracao_paragens =
      (double *)realloc((*carreiras)[index_carreira].duracao_paragens,
                        sizeof(double) * (numero_de_paragens_presentes));


  /* mover todos os elementos do vetor com os custos */
  shift_right_float((*carreiras)[index_carreira].custo_paragens,
                    numero_de_paragens_presentes);
  shift_right_float((*carreiras)[index_carreira].duracao_paragens,
                    numero_de_paragens_presentes);

  /* atualizar o novo nome */
  strcpy((*carreiras)[index_carreira].origem.name,
         (*paragens)[origem].name); /* atualizar a destino da carreira */

  (*carreiras)[index_carreira].paragens_carreira[0] =
      origem; /* atualizar o vector da carreira */

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  /* colocar o valor novo aqui */
  (*carreiras)[index_carreira].custo_paragens[0] = custo_atual;
  (*carreiras)[index_carreira].duracao_paragens[0] = custo_atual;

  (*carreiras)[index_carreira].custo += custo_atual; /* atualizar o custo    */
  (*carreiras)[index_carreira].duracao +=
      duracao_atual; /* atualizar a duração  */

  (*carreiras)[index_carreira].num_de_paragens++;
}


/* função dedicada ao caso particular de carreira circular colocando o destino
 * com a mesma paragem da origem */
void ligacao_circular(int index_carreira, int destino, char custo[buff],
                      char duracao[buff], Carreiras **carreiras,
                      Paragem **paragens) {

  double custo_atual, duracao_atual;
  int numero_de_paragens;
  numero_de_paragens = (*carreiras)[index_carreira].num_de_paragens;


  /* realocar o o tamanho para adicionar mais uma paragem à carreira */
  (*carreiras)[index_carreira].paragens_carreira =
      (int *)realloc((*carreiras)[index_carreira].paragens_carreira,
                     sizeof(int) * (numero_de_paragens + 1));

  /* atualizar o nome */
  strcpy((*carreiras)[index_carreira].destino.name,(*paragens)[destino].name); 
  
  /* atualizar o vector das paragens com o novo destino */
  (*carreiras)[index_carreira].paragens_carreira[numero_de_paragens] =destino; 

  custo_atual = atof(custo);
  duracao_atual = atof(duracao);

  (*carreiras)[index_carreira].custo += custo_atual; /* atualizar o custo    */
  (*carreiras)[index_carreira].duracao +=
      duracao_atual; /* atualizar a duração  */

  (*carreiras)[index_carreira].num_de_paragens++;
}

int funcao_gerencia_ligacoes(char comando[], Carreiras **carreiras, Paragem **paragens,
             int *numero_carreiras, int *numero_paragens) {

  char nome_carreira[buff] = "", nome_origem[buff] = "",
       nome_destino[buff] = "";

  int pos_aspas, arg;
  char comando2[buff];

  char custo[buff] = "", duracao[buff] = "";
  int index_carreira, index_origem, index_destino;
  int paragens_atual; /* numero de paragens pertencentes a uma carreira */

  arg = conta_argumentos_funcao_l(comando);
  if ((arg == 4 || arg == 3 || arg == 2 || arg == 1)) {
    return 0;
  }

  if (arg == 7) { /* caso de terem os dois aspas */
    sscanf(comando, "%c %s \"%[^\"]\" \"%[^\"]\" %s %s", comando2,
           nome_carreira, nome_origem, nome_destino, custo, duracao);
  }
  if (arg != 7) {
    pos_aspas = conta_espacos_ate_aspas(comando);

    if (pos_aspas == 3) { /* caso do nome do destino ter aspas */
      sscanf(comando, "%c %s %s \"%[^\"]\" %s %s", comando2, nome_carreira,
             nome_origem, nome_destino, custo, duracao);
    }
    if (pos_aspas == 2) { /* caso do nome de origem ter aspas */
      sscanf(comando, "%c %s \"%[^\"]\" %s %s %s", comando2, nome_carreira,
             nome_origem, nome_destino, custo, duracao);
    }
    if (pos_aspas == 5) { /* caso de nenhum ter aspas */
      sscanf(comando, "%c %s %s %s %s %s", comando2, nome_carreira, nome_origem,
             nome_destino, custo, duracao);
    }
  }

  if (verifica_existe_carreira(nome_carreira, carreiras, numero_carreiras) ==
      0) {
    printf("%s: no such line.\n", nome_carreira);
    return 0;
  }
  if (verifica_existe_paragem(nome_origem, paragens, numero_paragens) == 0) {
    printf("%s: no such stop.\n", nome_origem);
    return 0;
  }
  if (verifica_existe_paragem(nome_destino, paragens, numero_paragens) == 0) {
    printf("%s: no such stop.\n", nome_destino);
    return 0;
  }
  if (atof(custo) < 0 || atof(duracao) < 0) {
    printf("negative cost or duration.\n");
    return 0;
  }

  /* obter os indexes que vão ser utilizados posteriormente */
  index_carreira =
      get_carreira_index(nome_carreira, carreiras, numero_carreiras);
  index_origem = get_paragem_index(nome_origem, paragens, numero_paragens);
  index_destino = get_paragem_index(nome_destino, paragens, numero_paragens);

  /* numero de paragens pertencente a uma carreira */
  paragens_atual = (*carreiras)[index_carreira].num_de_paragens;
  /* index ultima paragem registada */
  paragens_atual--;

  /* situacao de carreira vazia, adicionar de imediato à origem e ao destino */
  if ((*carreiras)[index_carreira].num_de_paragens == 0) {
    cria_ligacao(index_carreira, index_origem, index_destino, custo, duracao,
                 carreiras, paragens);
    return 0;
  }

  /* verificar se pelo menos um dos extremos dados está presente */
  if (strcmp(
          (*paragens)[(*carreiras)[index_carreira].paragens_carreira[0]].name,
          nome_destino) != 0 &&
      strcmp((*paragens)[(*carreiras)[index_carreira]
                .paragens_carreira[paragens_atual]].name, nome_origem) != 0) {
    printf("link cannot be associated with bus line.\n");
    return 0;
  }

  /* condicao de carreira ser circular */
  if ((strcmp((*paragens)[(*carreiras)[index_carreira]
                              .paragens_carreira[paragens_atual]]
                  .name,
              nome_origem) == 0) &&
      (strcmp(
           (*paragens)[(*carreiras)[index_carreira].paragens_carreira[0]].name,
           nome_destino) == 0)) {
    ligacao_circular(index_carreira, index_destino, custo, duracao, carreiras,
                     paragens);
    return 0;
  }

  /* condição para adicionar novo destino */
  if (strcmp((*paragens)[(*carreiras)[index_carreira]
                             .paragens_carreira[paragens_atual]]
                 .name,
             nome_origem) == 0) {
    adiciona_destino(index_carreira, index_destino, custo, duracao, carreiras,
                     paragens, numero_carreiras, numero_paragens);
    return 0;
  }

  /* condição para adicionar nova origem */
  if (strcmp(
          (*paragens)[(*carreiras)[index_carreira].paragens_carreira[0]].name,
          nome_destino) == 0) {

    /* mover o vector todo uma posição para a direita */
    shift_right((*carreiras)[index_carreira].paragens_carreira,
                (*carreiras)[index_carreira].num_de_paragens + 1);

    adiciona_origem(index_carreira, index_origem, custo, duracao, carreiras,
                    paragens, numero_carreiras, numero_paragens);
    return 0;
  }

  return 0;
}

/*--------------------- Funções associadas comando I ----------------------- */

void bubbleSort(char *vector[], int size) {
  int i, j;
  for (i = 0; i < size - 1; i++) {
    for (j = 0; j < size - i - 1; j++) {
      if (strcmp(vector[j], vector[j + 1]) > 0) {
        char *temp = vector[j];
        vector[j] = vector[j + 1];
        vector[j + 1] = temp;
      }
    }
  }
}

/* função principal do comando 'i' */
int funcao_imprime_intersecoes(Carreiras **carreiras, Paragem **paragens, int *numero_carreiras,
             int *numero_paragens) {
  
  int i, j, k, l, count;

  /* alocar memoria para o array que vai guardar os nomes das carreiras */
  char **carreiras_para_organizar = (char **)malloc( 
    (*numero_carreiras) *sizeof(char *)); 

  for (i = 0; i < *numero_paragens; i++) {
    /* condição de ter mais do que uma carreira associada */
    if ((*paragens)[i].intersecoes > 1) { 
      printf("%s %d:", (*paragens)[i].name, (*paragens)[i].intersecoes);

      count = 0;

      /* iterar sobre as carreiras e verificar as paragens incluidas em cada */
      for (j = 0; j < *numero_carreiras; j++) { 
                     
        for (k = 0; k < (*carreiras)[j].num_de_paragens; k++) {
          if (strcmp(((*paragens)[i].name),
                (*paragens)[(*carreiras)[j].paragens_carreira[k]].name) == 0) {
            
          /* enviar para o array que vai ser organizado e contabilizar a contagem*/
          carreiras_para_organizar[count] = (*carreiras)[j].name; 
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

  /* dar free do array alocado */
  free(carreiras_para_organizar);
  return 0;
}


/* função principal do programa */
int main() {

  int numero_paragens = 0;
  Paragem *paragens = NULL;

  int numero_carreiras = 0;
  Carreiras *carreiras = NULL;

  char funcoes[buff];
  while (fgets(funcoes, buff, stdin) != NULL) {
    if (funcoes[0] == 'q') {
      funcao_apaga_sistema(&carreiras, &paragens, &numero_carreiras,
                           &numero_paragens);
      break;
    } else if (funcoes[0] == 'p') {
      funcao_gerencia_paragens(funcoes, &paragens, &carreiras, &numero_paragens,
                               &numero_carreiras);
    } else if (funcoes[0] == 'e') {
      funcao_remove_paragem(funcoes, &paragens, &carreiras, &numero_paragens,
                            &numero_carreiras);
    } else if (funcoes[0] == 'c') {
      funcao_gerencia_carreiras(funcoes, &carreiras, &paragens,
                                &numero_carreiras, &numero_paragens);
    } else if (funcoes[0] == 'r') {
      funcao_remove_carreiras(funcoes, &carreiras, &paragens, &numero_carreiras,
                              &numero_paragens);
    } else if (funcoes[0] == 'l') {
      funcao_gerencia_ligacoes(funcoes, &carreiras, &paragens,
                               &numero_carreiras, &numero_paragens);
    } else if (funcoes[0] == 'a') {
      funcao_apaga_sistema(&carreiras, &paragens, &numero_carreiras,
                           &numero_paragens);
    } else if (funcoes[0] == 'i') {
      funcao_imprime_intersecoes(&carreiras, &paragens, &numero_carreiras,
                                 &numero_paragens);
    }
  }

  return 0;
}