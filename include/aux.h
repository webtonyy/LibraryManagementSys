#ifndef _AUX_H
#define _AUX_H

#include "../include/catalogo.h"
#include "../include/hash.h"



// Função para remover acentos de uma string
char* remover_acentos(const char* str);

// Função para converter uma string para letras minúsculas
char* to_lowercase(const char* str);

// Função para verificar se uma string é válida
int is_valid_string(const char *str);

// Função para normalizar uma string (possivelmente removendo acentos e convertendo para minúsculas)
char* normalize_string(const char* str);

// Função para contar livros em uma estrutura de árvore
void contar_livros(No *no, const char *nome, int *count);

// Função para percorrer uma árvore e inserir informações de autores em uma tabela hash
void percorrer_arvore_e_inserir_na_hash_autor(No *no, HashTable *ht);

// Função para percorrer uma árvore e inserir informações de gêneros em uma tabela hash
void percorrer_arvore_e_inserir_na_hash_genero(No *no, HashTable *ht);

#endif
