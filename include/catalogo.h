#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um livro
typedef struct livro {
    int id;                // ID do livro (opcional, pode ser usado para ordenação)
    int qtd;               // Quantidade de exemplares do livro
    char *nome;            // Nome do livro (sem acentos, usado para comparações)
    char *real_nome;       // Nome original com acentos (usado para exibição)
    char *real_genero;     // Gênero original com acentos (usado para exibição)
    char *real_autor;      // Autor original com acentos (usado para exibição)
    char *genero;          // Gênero do livro
    char *autor;           // Autor do livro
    struct livro *esq;     // Ponteiro para o nó esquerdo da árvore binária
    struct livro *dir;     // Ponteiro para o nó direito da árvore binária
} Livro;

// Estrutura para representar o catálogo de livros
typedef struct catalogo {
    int nl;                // Número total de livros no catálogo
    Livro *raiz;           // Raiz da árvore binária de livros
} Catalogo;

// Funções principais do catálogo
Livro* livro_init(const char *nome, const char *genero, const char *autor);
Catalogo* catalogo_init();
void livro_set(Livro *l, const char *nome, const char *genero, const char *autor);
Livro* add_livro(Catalogo *c, Livro *l);
Livro* remover_livro(Catalogo *c, const char *nome);
void listar_livros(Catalogo *c);
void devolve_livro(Catalogo *c, char *nome, char *autor);
void emprestar_livro(Catalogo *c, char *nome, char *autor);
int verifica_livro(Catalogo *c, const char *nome);
Livro* editar_livro(Catalogo *c, const char *nome, Livro *novo_dados);
void free_catalogo(Catalogo *c);

// Funções auxiliares para manipulação de strings
char* remover_acentos(const char* str);
char* to_lowercase(const char* str);

#endif // _CATALOGO_H
