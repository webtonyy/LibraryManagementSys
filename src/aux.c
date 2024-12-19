#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "../include/aux.h"



#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Função para remover acentos
char* remover_acentos(const char* str) {
    if (!str) return NULL;

    char *buffer = malloc(strlen(str) + 1); // Aloca memória para a string normalizada
    if (!buffer) return NULL; // Verifica se a alocação foi bem-sucedida

    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        unsigned char c = (unsigned char)str[i];
        if (strchr("áàãâäÁÀÃÂÄ", c)) {
            buffer[j++] = 'a';
        } else if (strchr("éèêëÉÈÊË", c)) {
            buffer[j++] = 'e';
        } else if (strchr("íìîïÍÌÎÏ", c)) {
            buffer[j++] = 'i';
        } else if (strchr("óòõôöÓÒÕÔÖ", c)) {
            buffer[j++] = 'o';
        } else if (strchr("úùûüÚÙÛÜ", c)) {
            buffer[j++] = 'u';
        } else if (strchr("çÇ", c)) {
            buffer[j++] = 'c';
        } else {
            buffer[j++] = c; // Copia o caractere sem alteração
        }
    }
    buffer[j] = '\0'; // Certifique-se de terminar a string
    return buffer;
}

// Função para converter para minúsculas
char* to_lowercase(const char* str) {
    if (!str) return NULL;

    char *buffer = malloc(strlen(str) + 1); // Aloca memória para a string resultante
    if (!buffer) return NULL; // Verifica se a alocação foi bem-sucedida

    int i = 0;
    while (str[i]) {
        buffer[i] = tolower((unsigned char)str[i]);
        i++;
    }
    buffer[i] = '\0'; // Certifique-se de terminar a string
    return buffer;
}



int is_valid_string(const char *str) {
    if (!str || str[0] == '\0') {
        return 0; // String é nula ou vazia
    }

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (!isascii(c) && c != ' ' && strchr("áàãâéêíóõôúçÁÀÃÂÉÊÍÓÕÔÚÇñöÖäÑÄ", c) == NULL) {
            return 0; // Caractere inválido encontrado
        }
    }
    return 1; // Todos os caracteres são válidos e a string não está vazia
}


// Função para normalizar strings (remover acentos e converter para minúsculas)
char* normalize_string(const char* str) {
    if (!str) return NULL;

    char *lowercase = to_lowercase(str);       // Converte para minúsculas
    if (!lowercase) return NULL;              // Verifica se a conversão foi bem-sucedida

    char *normalized = remover_acentos(lowercase); // Remove os acentos
    free(lowercase);                             // Libera a memória intermediária

    return normalized;                          // Retorna a string normalizada
}




// Função para contar quantos livros existem com o mesmo nome no catálogo
void contar_livros(No *no, const char *nome, int *count) {
        if (!no) return;
        contar_livros(no->esquerda, nome, count);
        if (strcmp(no->livro->nome, nome) == 0) (*count)++;
        contar_livros(no->direita, nome, count);
    }


void percorrer_arvore_e_inserir_na_hash_autor(No *no, HashTable *ht) {
    if (no == NULL) return;

    // Insere o livro do nó atual na tabela hash
    inserir_livro(ht, no->livro, no->livro->autor);

    // Percorre a subárvore esquerda e direita recursivamente
    percorrer_arvore_e_inserir_na_hash_autor(no->esquerda, ht);
    percorrer_arvore_e_inserir_na_hash_autor(no->direita, ht);
}

void percorrer_arvore_e_inserir_na_hash_genero(No *no, HashTable *ht) {
    if (no == NULL) return;

    // Insere o livro do nó atual na tabela hash
    inserir_livro(ht, no->livro, no->livro->genero);

    // Percorre a subárvore esquerda e direita recursivamente
    percorrer_arvore_e_inserir_na_hash_genero(no->esquerda, ht);
    percorrer_arvore_e_inserir_na_hash_genero(no->direita, ht);
}