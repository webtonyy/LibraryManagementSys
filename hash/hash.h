#ifndef HASH_TABLE_OPEN_H
#define HASH_TABLE_OPEN_H
#define TABLE_SIZE 100
#define MAX_GEN 20


typedef struct {
    int id;
    int chave;
    char titulo[TABLE_SIZE];
    char autor[TABLE_SIZE]; 
    char genero[TABLE_SIZE];
} LivroH;

typedef struct {
    Livro *autor[TABLE_SIZE];
    char *genero[MAX_GEN];
    int num_gen;
} HashTable;


int funcaohash_string(const char *nome);
int funcaohash(int chave);
void table_init(HashTable *t);
void genero_possivel(HashTable *t);
int genero_valido(HashTable *t, const char *genero);
void inserir_autor(HashTable *t, const char *nome);
void inserir_genero(HashTable *t, const char *nome) 
void print_autores(HashTable *t);
void print_generos(HashTable *t);
void deletar_autor(HashTable *t, const char *nome);
void hash_free(HashTable *t);

#endif 