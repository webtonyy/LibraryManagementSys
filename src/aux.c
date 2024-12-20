#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include "../include/aux.h"
#include "../include/catalogo.h"
#include "../include/hash.h"


#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Função para remover acentos de uma string.
char* remover_acentos(const char* str) {
    if (!str) return NULL; // Verifica se a string é nula.

    char *buffer = malloc(strlen(str) + 1); // Aloca memória para a string resultante.
    if (!buffer) return NULL; // Verifica se a alocação foi bem-sucedida.

    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) { // Percorre a string original.
        unsigned char c = (unsigned char)str[i]; // Trata o caractere como unsigned char.
        // Substitui caracteres acentuados pelas versões sem acento.
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
            buffer[j++] = c; // Copia o caractere sem alterações.
        }
    }
    buffer[j] = '\0'; // Adiciona o terminador nulo à string.
    return buffer; // Retorna a string sem acentos.
}

// Função para converter uma string para minúsculas.
char* to_lowercase(const char* str) {
    if (!str) return NULL; // Verifica se a string é nula.

    char *buffer = malloc(strlen(str) + 1); // Aloca memória para a string resultante.
    if (!buffer) return NULL; // Verifica se a alocação foi bem-sucedida.

    int i = 0;
    while (str[i]) { // Converte cada caractere para minúscula.
        buffer[i] = tolower((unsigned char)str[i]);
        i++;
    }
    buffer[i] = '\0'; // Adiciona o terminador nulo à string.
    return buffer; // Retorna a string em letras minúsculas.
}

// Função para verificar se uma string é válida (contém apenas caracteres permitidos).
int is_valid_string(const char *str) {
    if (!str || str[0] == '\0') {
        return 0; // Retorna 0 se a string for nula ou vazia.
    }

    for (int i = 0; str[i] != '\0'; i++) { // Percorre cada caractere da string.
        char c = str[i];
        // Verifica se o caractere não é ASCII, espaço ou caractere válido com acento.
        if (!isascii(c) && c != ' ' && strchr("áàãâéêíóõôúçÁÀÃÂÉÊÍÓÕÔÚÇñöÖäÑÄ", c) == NULL) {
            return 0; // Retorna 0 se um caractere inválido for encontrado.
        }
    }
    return 1; // Retorna 1 se todos os caracteres forem válidos.
}

// Função para normalizar strings (remover acentos e converter para minúsculas).
char* normalize_string(const char* str) {
    if (!str) return NULL; // Verifica se a string é nula.

    char *lowercase = to_lowercase(str);       // Converte para letras minúsculas.
    if (!lowercase) return NULL;              // Verifica se a conversão foi bem-sucedida.

    char *normalized = remover_acentos(lowercase); // Remove acentos da string.
    free(lowercase);                             // Libera a memória da string intermediária.

    return normalized;                          // Retorna a string normalizada.
}

// Função para contar quantos livros existem com o mesmo nome no catálogo.
void contar_livros(No *no, const char *nome, int *count) {
    if (!no) return; // Retorna se o nó for nulo (base da recursão).

    contar_livros(no->esquerda, nome, count); // Percorre o lado esquerdo da árvore.
    if (strcmp(no->livro->nome, nome) == 0) (*count)++; // Incrementa o contador se o nome coincidir.
    contar_livros(no->direita, nome, count); // Percorre o lado direito da árvore.
}

// Função para percorrer a árvore e inserir livros na tabela hash usando o autor como chave.
void percorrer_arvore_e_inserir_na_hash_autor(No *no, HashTable *ht) {
    if (no == NULL) return; // Retorna se o nó for nulo (base da recursão).

    inserir_livro(ht, no->livro, no->livro->autor); // Insere o livro na hash com o autor como chave.

    percorrer_arvore_e_inserir_na_hash_autor(no->esquerda, ht); // Percorre o lado esquerdo da árvore.
    percorrer_arvore_e_inserir_na_hash_autor(no->direita, ht); // Percorre o lado direito da árvore.
}

// Função para percorrer a árvore e inserir livros na tabela hash usando o gênero como chave.
void percorrer_arvore_e_inserir_na_hash_genero(No *no, HashTable *ht) {
    if (no == NULL) return; // Retorna se o nó for nulo (base da recursão).

    inserir_livro(ht, no->livro, no->livro->genero); // Insere o livro na hash com o gênero como chave.

    percorrer_arvore_e_inserir_na_hash_genero(no->esquerda, ht); // Percorre o lado esquerdo da árvore.
    percorrer_arvore_e_inserir_na_hash_genero(no->direita, ht); // Percorre o lado direito da árvore.
}
