#ifndef HASH_TABLE_OPEN_H
#define HASH_TABLE_OPEN_H

#define TABLE_SIZE 100

#include "../include/catalogo.h"

// Estrutura para um nó na hash table
typedef struct NoHash {
    char *nome;             // Nome do gênero ou autor (chave, alocado dinamicamente)
    Livro **livros;         // Vetor dinâmico de ponteiros para livros
    int quantidade;         // Quantidade atual de livros no vetor
    int capacidade;         // Capacidade máxima atual do vetor (para redimensionamento)
    struct NoHash *proximo; // Próximo nó em caso de colisão na hash table
} NoHash;

// Estrutura da tabela hash
typedef struct HashTable {
    NoHash **tabela;        // Vetor de ponteiros para nós da tabela hash
    int tamanho;            // Tamanho da tabela hash (número de buckets)
} HashTable;

// Funções principais da tabela hash
HashTable *hash_table_init(int tamanho);                        //Inicia a tabela hash
int funcaohash_string(const char *nome);                      // Calcula o índice com base em uma string
int inserir_livro(HashTable *t, Livro *l, const char *nome_no); // Insere um livro na tabela hash
void deletar(HashTable *t, const char *nome, const char *nome_no); // Deleta um livro da tabela hash
void edita_livro(HashTable *t, const char *nome_errado, const char *nome_correto, const char *nome_no); // Edita os dados de um livro existente
void buscar_por_no(HashTable *t, const char *nome_no);        // Busca todos os livros associados a um nó específico
void hash_free(HashTable *t);                                 // Libera toda a memória associada à tabela hash

#endif
