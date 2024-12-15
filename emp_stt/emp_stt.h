#ifndef emp_stt
#define emp_stt

// Declaração das estruturas de dados para o sistema de empréstimos
typedef struct No Livro;
typedef struct catalogo catalogo;

// Definições para representar o status de empréstimo: 1 para emprestado, 0 para disponível
#define bool int
#define true 1
#define false 0

// Funções para inicializar e manipular livros
Livro *livro_init(char *nome, char *autor); // Inicializa um nó do livro com nome e autor
catalogo *catalogo_init(); // Inicializa um catálogo

// Funções para manipulação do catálogo de empréstimos
void empstt_append(catalogo *l, Livro* livro); // Adiciona um livro ao catálogo pelo nome
void empstt_pop(catalogo *l, Livro* livro); // Remove um livro do catálogo pelo nome
void empstt_clear(catalogo *l); // Limpa todo o catálogo de empréstimos
void empstt_pegarlivro(catalogo *l, Livro *livro); // Pega um livro emprestado da biblioteca a partir de seu nome
void empstt_devolverlivro(catalogo *l, Livro *livro); // Devolve um livro emprestado a partir de seu nome
void empstt_status(catalogo *l, Livro *livro); // Verifica o status de empréstimo de um livro específico
void empstt_print(catalogo *l); // verificar os elementos da lista de empréstimos

#endif