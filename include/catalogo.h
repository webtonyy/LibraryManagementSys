#ifndef _ARVBB_H
#define _ARVBB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct livro {
    int id;
    int qtd;
    char *nome;
    char *genero;
    char *autor;
    struct livro *esq, *dir;
} Livro;

typedef struct catalogo {
    int nl;
    Livro *raiz;
} Catalogo;

// Funções principais do catálogo
Livro* livro_init(const char *nome, const char *genero, const char *autor, Livro *pai);
Catalogo* catalogo_init();
void livro_set(Livro *l, const char *nome, const char *genero, const char *autor);
Livro* add_livro(Catalogo *c, Livro *l);
Livro* remover_livro(Catalogo *c, const int id);
void listar_livros(Catalogo *c);
int verifica_livro(Catalogo *c, const char *nome);
void editar_livro(Catalogo *c, Livro *novo_dados);
void recalcular_ids(Catalogo *c);
void free_catalogo(Catalogo *c);

#endif
