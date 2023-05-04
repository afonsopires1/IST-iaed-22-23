#define buff 65531

/* ----------------- Estruturas de dados -------------- */

typedef struct Paragem {
  char *name;
  double latitude;
  double longitude;
  int intersecoes;
  int decrementada;
} Paragem;

typedef struct Carreiras {
  char *name;
  Paragem origem;
  Paragem destino;
  int num_de_paragens;
  int *paragens_carreira;
  double *custo_paragens;   /* dedicado a guardar os custos de ligações */
  double *duracao_paragens; /* dedicado a guardar as durações de ligações */
  double custo;
  double duracao;
} Carreiras;

int funcao_gerencia_paragens(char comando[], Paragem **paragens,
                             Carreiras **carreiras, int *numero_paragens,
                             int *numero_carreiras);
int funcao_gerencia_carreiras(char comando[], Carreiras **carreiras,
                              Paragem **paragens, int *numero_carreiras,
                              int *numero_paragens);
int funcao_gerencia_ligacoes(char comando[], Carreiras **carreiras,
                             Paragem **paragens, int *numero_carreiras,
                             int *numero_paragens);
int funcao_imprime_intersecoes(Carreiras **carreiras, Paragem **paragens,
                               int *numero_carreiras, int *numero_paragens);
int funcao_remove_paragem(char comando[], Paragem **paragens,
                          Carreiras **carreiras, int *numero_paragens,
                          int *numero_carreiras);
int funcao_remove_carreiras(char comando[], Carreiras **carreiras,
                            Paragem **paragens, int *numero_carreiras,
                            int *numero_paragens);
int funcao_apaga_sistema(Carreiras **carreiras, Paragem **paragens,
                         int *numero_carreiras, int *numero_paragens);

int has_double_quotes(char str[]);
int verifica_existe_carreira(char *nome, Carreiras **carreiras,
                             int *numero_carreiras);
int verifica_existe_paragem(char *nome, Paragem **paragens,
                            int *numero_paragens);
int conta_argumentos_funcao_l(char input[]);
int get_carreira_index(char name[], Carreiras **carreiras, int *num_carreiras);
int get_paragem_index(char *nome, Paragem **paragens, int *numero_paragens);
int verifica_presente_carreira(Carreiras **carreiras, Paragem **paragens,
                               int *numero_carreiras, int *numero_paragens,
                               char nome_paragem[], char nome_carreira[]);
int conta_argumentos(char comando[]);
int conta_espacos_ate_aspas(char comando[]);

int has_double_quotes();
