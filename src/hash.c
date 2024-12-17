#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash.h"
#include "../include/aux.h"

int funcaohash_string(const char *nome) {
    char *normalized = normalize_string(nome);
    int tam = strlen(normalized);
    unsigned int ch = 0;

    for (int i = 0; i < tam; i++) {
        ch += normalized[i] * (i + 1);
    }
    return ch % TABLE_SIZE;
}

int funcaohash(int chave) {
    return chave % TABLE_SIZE;
}

void table_init(HashTable *t) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        t->genero[i] = NULL;
        t->autor[i] = NULL;
    }
}

void inserir_generos_iniciais(HashTable *t) {
    const char *generos[] = {
        "Fantasia", "Ficção Científica", "Romance", "Mistério e Crime",
        "Terror e Horror", "Aventura", "Ficção Histórica", "Ficção Literária",
        "Ficção Juvenil", "Ficção LGBTQIA+",
        "Biografias e Memórias", "Autoajuda", "Religião e Espiritualidade",
        "História e Ciências Sociais", "Ciência e Tecnologia", "True Crime"
    };
    int num_generos = sizeof(generos) / sizeof(generos[0]);

    for (int i = 0; i < num_generos; i++) {
        char *genero_normalizado = normalize_string(generos[i]);
        if (!genero_normalizado) {
            fprintf(stderr, "Falha ao normalizar gênero: %s\n", generos[i]);
            continue;
        }

        int id = funcaohash_string(genero_normalizado);

        while (t->genero[id] != NULL) {
            id = funcaohash(id + 1);
        }

        t->genero[id] = (GeneroH *)malloc(sizeof(GeneroH));
        if (t->genero[id] == NULL) {
            fprintf(stderr, "Falha de alocação de memória para gênero: %s\n", generos[i]);
            continue;
        }

        strncpy(t->genero[id]->genero, genero_normalizado, sizeof(t->genero[id]->genero) - 1);
        t->genero[id]->genero[sizeof(t->genero[id]->genero) - 1] = '\0';
        strncpy(t->genero[id]->real_genero, generos[i], sizeof(t->genero[id]->real_genero) - 1);
        t->genero[id]->real_genero[sizeof(t->genero[id]->real_genero) - 1] = '\0';
        
        t->genero[id]->livros = NULL;
        
        
    }
}



int inserir_livro(HashTable *t, const char *nome, const char *autor, const char *genero) {
    char *genero_normalizado = normalize_string(genero);
    if (!genero_normalizado) {
        fprintf(stderr, "Falha ao normalizar gênero: %s\n", genero);
        return 0;
    }

    int id_genero = funcaohash_string(genero_normalizado);
    int id_inicial = id_genero;

    while (t->genero[id_genero] != NULL) {
        if (strcmp(t->genero[id_genero]->genero, genero_normalizado) == 0) {
            NoLivro *atual = t->genero[id_genero]->livros;
            while (atual != NULL) {
                if (strcmp(atual->nome, nome) == 0 && strcmp(atual->autor, autor) == 0) {
                    atual->qtd++;
                    return 1;
                }
                atual = atual->prox;
            }

            NoLivro *novo_livro = (NoLivro *)malloc(sizeof(NoLivro));
            if (!novo_livro) {
                fprintf(stderr, "Falha de alocação de memória.\n");
                return 0;
            }

            strncpy(novo_livro->nome, nome, sizeof(novo_livro->nome) - 1);
            novo_livro->nome[sizeof(novo_livro->nome) - 1] = '\0';
            strncpy(novo_livro->real_nome, nome, sizeof(novo_livro->real_nome) - 1);
            novo_livro->real_nome[sizeof(novo_livro->real_nome) - 1] = '\0';
            novo_livro->qtd = 1;
            novo_livro->prox = t->genero[id_genero]->livros;
            t->genero[id_genero]->livros = novo_livro;

            return 1;
        }
        
        id_genero = funcaohash(id_genero + 1);
        if (id_genero == id_inicial) break; // Evita loop infinito
    }

    return 0; // Falha ao encontrar o gênero
}



void buscar_por_genero(HashTable *t, const char *genero) {
    char *genero_normalizado = normalize_string(genero);
    if (!genero_normalizado) {
        fprintf(stderr, "Falha ao normalizar gênero: %s\n", genero);
        return;
    }

    int id_genero = funcaohash_string(genero_normalizado);
    int id_inicial = id_genero;

    while (t->genero[id_genero] != NULL) {
        if (strcmp(t->genero[id_genero]->genero, genero_normalizado) == 0) {
            printf("Livros no gênero '%s':\n", t->genero[id_genero]->real_genero);
            NoLivro *atual = t->genero[id_genero]->livros;
            
            if (!atual) {
                printf("Nenhum livro encontrado neste gênero.\n");
                return;
            }
            
            while (atual != NULL) {
                printf("- %s\n", atual->real_nome);
                atual = atual->prox;
            }
            
            return;
        }
        
        id_genero = funcaohash(id_genero + 1);
        if (id_genero == id_inicial) break; // Evita loop infinito
    }

    printf("Gênero '%s' não encontrado.\n", genero);
}

void buscar_por_autor(HashTable *t, const char *autor) {
    char *autor_normalizado = normalize_string(autor);
    if (!autor_normalizado) {
        fprintf(stderr, "Falha ao normalizar autor: %s\n", autor);
        return;
    }

    int id_autor = funcaohash_string(autor_normalizado);
    int initial_id = id_autor;

    while (t->autor[id_autor] != NULL) {
        if (strcmp(t->autor[id_autor]->autor, autor_normalizado) == 0) {
            printf("Livros do autor '%s':\n", t->autor[id_autor]->real_autor);

            NoLivro *atual = t->autor[id_autor]->livros;

            if (!atual) {
                printf("Nenhum livro encontrado para este autor.\n");
                return;
            }

            while (atual != NULL) {
                printf("- %s\n", atual->real_nome);
                atual = atual->prox;
            }

            return;
        }
        
        id_autor = funcaohash(id_autor + 1);
        if (id_autor == initial_id) break; // Evita loop infinito
    }

    printf("Autor '%s' não encontrado.\n", autor);
}



void imprimir(HashTable *t) {
    printf("\nGêneros:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (t->genero[i] != NULL) {
            // Imprime o nome do gênero
            printf("- %s\n", t->genero[i]->real_genero);

            // Itera sobre a lista de livros associados ao gênero
            NoLivro *atual = t->genero[i]->livros;
            while (atual != NULL) {
                printf("  * %s\n", atual->real_nome);
                atual = atual->prox;
            }
        }
    }

    printf("\nAutores:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (t->autor[i] != NULL) {
            // Imprime o nome do autor
            printf("- %s\n", t->autor[i]->real_autor);

            // Itera sobre a lista de livros associados ao autor
            NoLivro *atual = t->autor[i]->livros;
            while (atual != NULL) {
                printf("  * %s\n", atual->real_nome);
                atual = atual->prox;
            }
        }
    }
}


void hash_free(HashTable *t) {
    for (int i = 0; i < TABLE_SIZE; i++) {

         // Liberação de memória para autores
        if (t -> autor[i] != NULL){

            NoLivro* atual_a= t -> autor[i] -> livros;

            while(atual_a != NULL){
                NoLivro* temp_a= atual_a;

                atual_a= atual_a -> prox;

                free(temp_a);

            }
            free(t -> autor[i]);
            t -> autor[i]= NULL;

        }


        // Liberação de memória para gêneros

        if (t -> genero[i] != NULL){

            NoLivro* atual_g= t -> genero[i] -> livros;

            while(atual_g != NULL){
                NoLivro* temp_g= atual_g;

                atual_g= atual_g -> prox;

                free(temp_g);

            }
            free(t -> genero[i]);
            t -> genero[i] = NULL;

        }
    }
    printf("Memória liberada com sucesso.\n");
}


void deletar(HashTable *t, const char *nome, const char *autor, const char *genero) {
    if (autor != NULL) {
        int id = funcaohash_string(autor);
        int id_inicial = id;

        while (t->autor[id] != NULL) {
            if (strcmp(t->autor[id]->autor, autor) == 0) {
                NoLivro **atual = &(t->autor[id]->livros);

                while (*atual != NULL) {
                    if (strcmp((*atual)->nome, nome) == 0) {
                        if ((*atual)->qtd > 1) {
                            (*atual)->qtd--;
                            printf("Quantidade do livro '%s' do autor '%s' diminuída.\n", nome, autor);
                        } else {
                            NoLivro *temp = *atual;
                            *atual = (*atual)->prox;
                            free(temp); // Libera a memória do nó
                            printf("Livro '%s' deletado do autor '%s'.\n", nome, autor);

                            // Verifica se a lista está vazia e libera o autor
                            if (t->autor[id]->livros == NULL) {
                                free(t->autor[id]);
                                t->autor[id] = NULL;
                            }
                        }
                        return;
                    }
                    atual = &((*atual)->prox);
                }
            }
            id = funcaohash(id + 1);
            if (id == id_inicial) break; // Evita loop infinito
        }
        
        printf("Livro '%s' não encontrado para o autor '%s'.\n", nome, autor);
    }

    if (genero != NULL) {
        int id = funcaohash_string(genero);
        int id_inicial = id;

        while (t->genero[id] != NULL) {
            if (strcmp(t->genero[id]->genero, genero) == 0) {
                NoLivro **atual = &(t->genero[id]->livros);

                while (*atual != NULL) {
                    if (strcmp((*atual)->nome, nome) == 0) {
                        if ((*atual)->qtd > 1) {
                            (*atual)->qtd--;
                            printf("Quantidade do livro '%s' no gênero '%s' diminuída.\n", nome, genero);
                        } else {
                            NoLivro *temp = *atual;
                            *atual = (*atual)->prox;
                            free(temp); // Libera a memória do nó
                            printf("Livro '%s' deletado do gênero '%s'.\n", nome, genero);

                            // Verifica se a lista está vazia e libera o gênero
                            if (t->genero[id]->livros == NULL) {
                                free(t->genero[id]);
                                t->genero[id] = NULL;
                            }
                        }
                        return;
                    }
                    atual = &((*atual)->prox);
                }
            }
            id = funcaohash(id + 1);
            if (id == id_inicial) break; // Evita loop infinito
        }

        printf("Livro '%s' não encontrado no gênero '%s'.\n", nome, genero); 
    }
}
