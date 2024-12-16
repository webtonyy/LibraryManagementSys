#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "../include/aux.h"


// Função para remover acentos
char* remover_acentos(const char* str) {
    static char buffer[256];
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (strchr("áàãâÁÀÃÂ", c)) {
            buffer[j++] = 'a';
        } else if (strchr("éêÉÊ", c)) {
            buffer[j++] = 'e';
        } else if (strchr("íÍ", c)) {
            buffer[j++] = 'i';
        } else if (strchr("óõôÓÕÔ", c)) {
            buffer[j++] = 'o';
        } else if (strchr("úÚ", c)) {
            buffer[j++] = 'u';
        } else if (strchr("çÇ", c)) {
            buffer[j++] = 'c';
        } else {
            buffer[j++] = c;
        }
    }
    buffer[j] = '\0'; // Certifique-se de terminar a string
    return buffer;
}

// Função para converter para minúsculas
char* to_lowercase(const char* str) {
    static char buffer[256];
    int i = 0;
    while (str[i]) {
        buffer[i] = tolower((unsigned char)str[i]);
        i++;
    }
    buffer[i] = '\0'; // Certifique-se de terminar a string
    return buffer;
}

int is_valid_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (!isascii(c) && c != ' ' && strchr("áàãâéêíóõôúçÁÀÃÂÉÊÍÓÕÔÚÇ", c) == NULL) {
            return 0; // Caractere inválido encontrado
        }
    }
    return 1; // Todos os caracteres são válidos
}

char* normalize_string(const char* str) {
    return remover_acentos(to_lowercase(str));
}



Livro* min_aux(Livro* l) {
    
    while (l && l->esq) {
        l = l->esq;
    }
    return l;
}

// Function to count how many books with the same name exist in the catalog
void contar_livros(Livro *atual, const char* nome, int *count) {
    if (!atual) return; // Base case: If the current node is NULL, stop recursion

    // Recursively traverse the left subtree
    contar_livros(atual->esq, nome, count);

    // If the current book's name matches the target name, increment the count
    if (strcmp(nome, atual->nome) == 0) {
        (*count)++;
    }

    // Recursively traverse the right subtree
    contar_livros(atual->dir, nome, count);
}