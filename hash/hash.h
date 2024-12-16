#ifndef HASH_TABLE_OPEN_H
#define HASH_TABLE_OPEN_H
#define TABLE_SIZE 50


typedef struct Livro{
    int id;
    int chave;
    char autor[TABLE_SIZE]; 
    char genero[TABLE_SIZE];
} Livro;

typedef struct HashTable {
    Livro *autor[TABLE_SIZE];
    Livro *genero[TABLE_SIZE];
} HashTable;


int funcaohash_string(const char *nome);
int funcaohash(int chave);
void table_init(HashTable *t);
Livro autores();
Livro generos();
void inserir_livro(HashTable *t, const char *nome);
Livro* busca_livro(HashTable *t, const char *nome);
void imprimir(HashTable *t);
void deletar(HashTable *t, const char *nome);
void hash_free(HashTable *t);

#endif 