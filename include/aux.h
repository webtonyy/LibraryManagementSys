#ifndef _AUX_H
#define _AUX_H
#include "../include/catalogo.h"


char* remover_acentos(const char* str);
char* to_lowercase(const char* str);
int is_valid_string(const char *str);
char* normalize_string(const char* str);
Livro* min_aux(Livro* l);

void contar_livros(Livro *atual, const char* nome, int *count);
#endif