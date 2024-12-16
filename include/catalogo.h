#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Livro {
    char *nome;          // Nome do livro
    char *autor;         // Autor do livro
    char *genero;         // Gênero do livro
    char *real_nome;
    char *real_autor;
    char *real_genero;
    int qtd;                 // Quantidade de exemplares
    struct Livro *esq;       // Ponteiro para o nó esquerdo (árvore binária)
    struct Livro *dir;       // Ponteiro para o nó direito (árvore binária)
    struct Livro *prox;      // Ponteiro para o próximo elemento (hash table encadeada)
} Livro;


// Estrutura para representar o catálogo de livros
typedef struct catalogo {
    Livro *raiz;           // Raiz da árvore binária de livros
} Catalogo;

// Funções principais do catálogo
Livro* livro_init(const char *nome, const char *genero, const char *autor);
Catalogo* catalogo_init();
Livro* editar_livro(Catalogo *c, const char *nome, Livro *novo_dados);
Livro* add_livro(Catalogo *c, Livro *l);
Livro* remover_livro(Catalogo *c, const char *nome);
void listar_livros(Catalogo *c);
void devolve_livro(Catalogo *c, char *nome, char *autor);
void emprestar_livro(Catalogo *c, char *nome, char *autor);
int verifica_livro(Catalogo *c, const char *nome);
void verificar_status(Catalogo *c, const char *nome, const char *autor);
void free_catalogo(Catalogo *c);

// Funções auxiliares para manipulação de strings
char* remover_acentos(const char* str);
char* to_lowercase(const char* str);

#endif // _CATALOGO_H
