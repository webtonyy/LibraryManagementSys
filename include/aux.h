#ifndef _AUX_H
#define _AUX_H
#include "../include/catalogo.h"
#include "../include/hash.h"


char* remover_acentos(const char* str);
char* to_lowercase(const char* str);
int is_valid_string(const char *str);
char* normalize_string(const char* str);
void contar_livros(No *no, const char *nome, int *count);
void percorrer_arvore_e_inserir_na_hash_autor(No *no, HashTable *ht);
void percorrer_arvore_e_inserir_na_hash_genero(No *no, HashTable *ht);
#endif