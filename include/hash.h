#ifndef HASH_TABLE_OPEN_H
#define HASH_TABLE_OPEN_H

#define TABLE_SIZE 50

#include "../include/catalogo.h"

typedef struct NoLivro {
    char nome[100];
    char autor[100];
    char genero[50];
    int qtd;
    char real_nome[100];
    struct NoLivro *prox;
} NoLivro;

typedef struct AutorH {
    char autor[100];
    char real_autor[100];
    NoLivro *livros;
} AutorH;

typedef struct GeneroH {
    char genero[50];
    char real_genero[50];
    NoLivro *livros;
} GeneroH;

typedef struct HashTable {
    GeneroH *genero[TABLE_SIZE];
    AutorH *autor[TABLE_SIZE];
} HashTable;

int funcaohash_string(const char *nome);
int funcaohash(int chave);
void table_init(HashTable *t);
void inserir_generos_iniciais(HashTable *t);
int inserir_livro(HashTable *t, const char *nome, const char *autor, const char *genero);
void buscar_por_genero(HashTable *t, const char *genero);
void buscar_por_autor(HashTable *t, const char *autor);
void imprimir(HashTable *t);
void deletar(HashTable *t, const char *nome, const char *autor, const char *genero);
void hash_free(HashTable *t);

#endif
