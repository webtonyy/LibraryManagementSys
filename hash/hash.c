#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hash.h"

int funcaohash_string(const char *nome) { //chave: mult o código da letra na tab ASCII pelo indice 
	int tam = strlen(nome);
    int unsigned ch = 0;
    
    for (int i=0; i<tam; i++) {
        ch += nome[i]*(i+1);
    }
    return ch % TABLE_SIZE;
}

int funcaohash(int chave) {
    return chave % TABLE_SIZE;
}

void table_init(HashTable *t){
    for (int i=0; i<TABLE_SIZE; i++) {
        t->autor[i] = NULL;
        t->genero[i] = NULL;
    }
    t->num_gen = 0;
}

void genero_possivel(HashTable *t) {
    const char *permitidos[] = {"Ficção", "Romance", "Fantasia", "Não-ficção", "Biografia"};
    t->num_gen = sizeof(permitidos) / sizeof(permitidos[0]);
    
    for (int i = 0; i < t->num_gen; i++) {
        t->generos[i] = strdup(permitidos[i]);
    }
}

int genero_valido(HashTable *t, const char *genero) {
    if (!genero_possivel(t)) return -1;
    
    for (int i = 0; i < t->num_gen; i++) {
        if (strcmp(t->generos[i], genero) == 0) {
            return 1;
        }
    }
    return 0;
}

void inserir_autor(HashTable *t, const char *nome) {
    int id = funcaohash_string(nome);
    while (t->autores[id] != NULL) {
        if (strcmp(t->autores[id]->nome, nome) == 0) {
            return;
        }
        //Resolver conflitos:
        while(t->autor[id] != NULL) { 
            id = funcaohash(id + 1);
        }
    }
    t->autores[id] = (Item *)malloc(sizeof(Item));
    strcpy(t->autores[id]->nome, nome);
}

void inserir_genero(HashTable *t, const char *nome) {
    if (!genero_valido(t, nome)) {
        printf("Gênero '%s' não é permitido.\n", nome);
        return;
    }
    
    int id = funcaohash_string(nome);
    while (t->generos[id] != NULL) {
        if (strcmp(t->generos[id]->nome, nome) == 0) {
            return; // Gênero já existe
        }
        //Resolver conflitos:
        while(t->genero[id] != NULL) { 
            id = funcaohash(id + 1);
        }
    }
    t->generos[id] = (Item *)malloc(sizeof(Item));
    strcpy(t->generos[id]->nome, nome);
}


void print_autores(HashTable *t) {
    printf("Autores existentes:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (t->autores[i] != NULL) {
            printf("- %s\n", t->autores[i]->nome);
        }
    }
}

void print_generos(HashTable *t) {
    printf("Gêneros existentes:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (t->generos[i] != NULL) {
            printf("- %s\n", t->generos[i]->nome);
        }
    }
}

void deletar_autor(HashTable *t, const char *nome) {
    int id = funcaohash_string(nome);
    int original_id = id;

    while (t->autores[id] != NULL) {
        if (strcmp(t->autores[id]->nome, nome) == 0) {
            free(t->autores[id]);
            t->autores[id] = NULL;
            printf("Autor deletado.\n", nome);
            return;
        }
        id = (id + 1) % TABLE_SIZE;
        if (id == original_id) break;
    }
    printf("Autor não encontrado.\n", nome);
}

void hash_free(HashTable *t) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (t->autores[i] != NULL) {
            free(t->autores[i]);
            t->autores[i] = NULL;
        }
        if (t->generos[i] != NULL) {
            free(t->generos[i]);
            t->generos[i] = NULL;
        }
    }
        printf("Limpeza concluída!\n");
}