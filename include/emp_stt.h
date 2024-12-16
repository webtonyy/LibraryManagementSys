#ifndef emp_stt
#define emp_stt

// Declaração das estruturas de dados para o sistema de empréstimos
typedef struct No LivroL;

typedef struct catalogoL catalogoL;

// Definições para representar o status de empréstimo: 1 para emprestado, 0 para disponível
#define bool int
#define true 1
#define false 0

// Funções para inicializar e manipular livros
LivroL *livroL_init(char *nome, char *autor); // Inicializa um nó do livro com nome e autor
catalogoL *catalogoL_init(); // Inicializa um catálogo

// Funções para manipulação do catálogo de empréstimos
void empstt_append(catalogoL *l, LivroL* livro); // Adiciona um livro ao catálogo pelo nome
void empstt_pop(catalogoL *l, LivroL* livro); // Remove um livro do catálogo pelo nome
void empstt_clear(catalogoL *l); // Limpa todo o catálogo de empréstimos
int empstt_pegarlivro(catalogoL *l, LivroL *livro); // Pega um livro emprestado da biblioteca a partir de seu nome
int empstt_devolverlivro(catalogoL *l, LivroL *livro); // Devolve um livro emprestado a partir de seu nome
void empstt_status(catalogoL *l, LivroL *livro); // Verifica o status de empréstimo de um livro específico
void empstt_print(catalogoL *l); // verificar os elementos da lista de empréstimos

#endif