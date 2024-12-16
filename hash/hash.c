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
}

Livro autores() {
    Livro a;
    fgets(a.autor, 49, stdin);
    return a;
}

Livro generos() {
    Livro g;
    fgets(g.genero, 49, stdin);
    return g;
}

void inserir_livro(HashTable *t, const char *nome) {
    if (t->autor) {
        Livro a = autores();
        int id = funcaohash_string(a.autor);
        
        //Resolver conflitos:
        while(t->autor[id] != NULL) { 
            id = funcaohash(id + 1);
        }

        //Inserção do autor:
        t->autor[id] = (Livro *)malloc(sizeof(Livro));
        *(t->autor[id]) = a;
    } 
    else if (t->genero) {
        Livro g = generos();
        int id = funcaohash_string(g.genero);

        //Resolver conflitos:
        while(t->genero[id] != NULL) { 
            id = funcaohash(id + 1);
        }

        //Inserção do gênero:
        t->genero[id] = (Livro *)malloc(sizeof(Livro));
        *(t->genero[id]) = g;
    }
}

Livro* busca_livro(HashTable *t, const char *nome) {
    if (t->autor != NULL) {
        int id = funcaohash_string(nome);
        int cond = id;
    
        //Conferir existência de algum livro:
        while(t->autor[id] != NULL) {
            if(strcmp(t->autor[id]->autor, nome) == 0) {
                return t->autor[id];
            }
            id = funcaohash(id + 1);
            if (id == cond) break;
        }
        printf("\nAutor não encontrado!\n");
        return NULL;
    }
    if(t->genero != NULL) {
        int id = funcaohash_string(nome);
        int cond = id;

        //Conferir existência de algum livro:
        while(t->genero[id] != NULL) {
            if(strcmp(t->genero[id]->genero, nome) == 0) {
                return t->genero[id];
            } 
            id = funcaohash(id + 1);
            if (id == cond) break;
        }
        printf("\nGênero não encontrado!\n");
        return NULL;
    }
    printf("\nNão há nada inserido!\n");
    return NULL;  // Caso ambas as tabelas sejam NULL
}

void imprimir(HashTable *t){
    if(t->autor != NULL) {
        printf("\nAutores:\n");
        int ja = 0;

        for(int i=0; i<TABLE_SIZE; i++){
            if (t->autor[i] != NULL) {
                printf("\n%s", t->autor[i]->autor);
            }
            ja = 1;
        } 
        if(ja == 0){
            printf("\nAutor não encontrado!\n");
        }
    }
    if(t->genero != NULL) {
        printf("\nGêneros:\n");
        int ja = 0;
        
        for(int i=0; i<TABLE_SIZE; i++){
            if (t->genero[i] != NULL) {
                printf("\n%s", t->genero[i]->genero);
            }
            ja = 1;
        }
        if(ja == 0){
            printf("\nGênero não encontrado!\n");
        }
    }
}

void deletar(HashTable *t, const char *nome) {
    if(t->autor != NULL) {
        int id = funcaohash_string(nome);

        while (t->autor[id] != NULL) {
            if (strcmp(t->autor[id]->autor, nome) == 0) {
                free(t->autor[id]);
                t->autor[id] = NULL;
                printf("Autor deletado: '%s'\n", nome);
                return;
            }
            id = funcaohash(id + 1); 
        }
        printf("Autor '%s' não encontrado!\n", nome);
    }
    if(t->genero != NULL) {
        int id = funcaohash_string(nome);

        while (t->genero[id] != NULL) {
            if (strcmp(t->genero[id]->genero, nome) == 0) {
                free(t->genero[id]);
                t->genero[id] = NULL;
                printf("Gênero deletado: '%s'\n", nome);
                return;
            }
            id = funcaohash(id + 1);
        }
        printf("Gênero '%s' não encontrado!\n", nome);
    }
}

void hash_free(HashTable *t) {
    if(t->autor != NULL) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (t->autor[i] != NULL ) {
                free(t->autor[i]);
                t->autor[i] = NULL;
            }
        }
        printf("Limpeza concluída para autores!\n");
    }
    if(t->genero != NULL) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (t->genero[i] != NULL ) {
                free(t->genero[i]);
                t->genero[i] = NULL;
            }
        }
        printf("Limpeza concluída para generos!\n");
    }
}

