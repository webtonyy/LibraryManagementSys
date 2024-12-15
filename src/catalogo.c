#include "../include/catalogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inicializa um novo livro
Livro* livro_init(const char *nome, const char *genero, const char *autor, Livro *pai) {
    Livro *l = malloc(sizeof(Livro));
    if (!l) {
        perror("Erro ao alocar memória para Livro");
        exit(EXIT_FAILURE);
    }
    l->nome = strdup(nome);
    l->genero = strdup(genero);
    l->autor = strdup(autor);
    l->esq = l->dir = pai;
    l->qtd = 1;

    if (!l->nome || !l->genero || !l->autor) {
        free(l->nome);
        free(l->genero);
        free(l->autor);
        free(l);
        perror("Erro ao alocar memória para strings do Livro");
        exit(EXIT_FAILURE);
    }

    return l;
}

// Inicializa um novo catálogo
Catalogo* catalogo_init() {
    Catalogo *c = malloc(sizeof(Catalogo));
    if (!c) {
        perror("Erro ao alocar memória para Catalogo");
        exit(EXIT_FAILURE);
    }

    c->raiz = NULL;
    c->nl = 0;
    return c;
}

// Atualiza os dados de um livro
void livro_set(Livro *l, const char *nome, const char *genero, const char *autor) {
    if (!l) return;

    free(l->nome);
    free(l->genero);
    free(l->autor);

    l->nome = strdup(nome);
    l->genero = strdup(genero);
    l->autor = strdup(autor);

    if (!l->nome || !l->genero || !l->autor) {
        perror("Erro ao alocar memória para atualizar os dados do livro");
        exit(EXIT_FAILURE);
    }
}

// Função auxiliar para adicionar um nó à árvore binária
Livro* add_aux(Livro *atual, Livro *novo) {
    if (!atual) return novo;

    if (strcmp(novo->nome, atual->nome) < 0)
        atual->esq = add_aux(atual->esq, novo);
    else if (strcmp(novo->nome, atual->nome) > 0)
        atual->dir = add_aux(atual->dir, novo);
    else
        atual->qtd++;

    return atual;
}

// Adiciona um livro ao catálogo
Livro* add_livro(Catalogo *c, Livro *l) {
    if (!c || !l) return NULL;

    c->raiz = add_aux(c->raiz, l);
    c->nl++;
    recalcular_ids(c); // Recalcula os IDs após adicionar o livro

    printf("O livro %s foi adicionado com sucesso\n", l->nome);
    return l;
}

// Função auxiliar para encontrar o menor nó na subárvore direita
Livro* min_aux(Livro* l) {
    while (l && l->esq)
        l = l->esq;
    return l;
}

// Função auxiliar para remover um nó da árvore binária
Livro* remove_aux(Livro *atual, const int id) {
    if (!atual) return NULL;

    if (id < atual->id)
        atual->esq = remove_aux(atual->esq, id);
    else if (id > atual->id)
        atual->dir = remove_aux(atual->dir, id);
    else { // Encontrou o nó a ser removido
        if (atual->qtd > 1) { // Reduz a quantidade se houver mais de uma cópia
            atual->qtd--;
            return atual;
        } else { // Remove o nó
            if (!atual->esq && !atual->dir) { // Caso 1: Nó sem filhos
                free(atual->nome);
                free(atual->genero);
                free(atual->autor);
                free(atual);
                return NULL;
            } else if (!atual->esq) { // Caso 2: Apenas filho direito
                Livro *temp = atual->dir;
                free(atual->nome);
                free(atual->genero);
                free(atual->autor);
                free(atual);
                return temp;
            } else if (!atual->dir) { // Caso 2: Apenas filho esquerdo
                Livro *temp = atual->esq;
                free(atual->nome);
                free(atual->genero);
                free(atual->autor);
                free(atual);
                return temp;
            } else { // Caso 3: Dois filhos
                Livro *sucessor = min_aux(atual->dir); // Encontra o sucessor in-order
                free(atual->nome);
                free(atual->genero);
                free(atual->autor);

                atual->nome = strdup(sucessor->nome);
                atual->genero = strdup(sucessor->genero);
                atual->autor = strdup(sucessor->autor);

                atual->dir = remove_aux(atual->dir, sucessor->id); // Remove o sucessor
            }
        }
    }

    return atual;
}

// Remove um livro do catálogo
Livro* remover_livro(Catalogo *c, const int id) {
    if (!c) return NULL;

    c->raiz = remove_aux(c->raiz, id);
    recalcular_ids(c); // Recalcula os IDs após remover o livro
    return c->raiz;
}

// Função auxiliar para recalcular os IDs em ordem alfabética
void recalcular_ids_aux(Livro *atual, int *id) {
    if (!atual) return;

    recalcular_ids_aux(atual->esq, id);
    atual->id = (*id)++;
    recalcular_ids_aux(atual->dir, id);
}

// Recalcula os IDs do catálogo
void recalcular_ids(Catalogo *c) {
    if (!c || !c->raiz) return;

    int id = 1; // Começa o ID em 1
    recalcular_ids_aux(c->raiz, &id);
}

// Lista todos os livros usando in-order traversal
void listar_aux(Livro *l) {
    if (!l) return;

    listar_aux(l->esq);

    printf("ID: %d | Nome: %s | Autor: %s | Gênero: %s | Quantidade: %d\n",
        l->id, l->nome, l->autor, l->genero, l->qtd);

    listar_aux(l->dir);
}

void listar_livros(Catalogo *c) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio.\n");
        return;
    }

    listar_aux(c->raiz);
}


int verifica_aux(Livro *atual,const char* nome){
    if(!atual || !nome) return 0;

    if (strcmp(nome, atual->nome) < 0)
        verifica_aux(atual->esq, nome);
    else if (strcmp(nome, atual->nome) > 0)
        verifica_aux(atual->dir, nome);
    else{
        printf("O livro %s está no catálogo", nome);
        return 1;
    }
    printf("O livro %s não foi encontrado no catálogo, verifique se digitou o nome corretamente", nome);
    return 0;
}
// Verifica se um livro está no catálogo pelo nome
int verifica_livro(Catalogo *c, const char *nome) {
    if(!c) return 0;
    Livro *atual = c -> raiz;
    verifica_aux(atual,nome);
    return 0;
}

// Função para editar um livro no catálogo
void editar_livro(Catalogo *c, Livro *novo_dados) {
    if (!c || !c->raiz || !novo_dados) {
        printf("Erro: Catálogo vazio ou dados inválidos.\n");
        return;
    }

    Livro *atual = c->raiz;
    while (atual) {
        int cmp = strcmp(novo_dados->nome, atual->nome);
        if (cmp == 0) { // Livro encontrado
            free(atual->genero);
            free(atual->autor);

            atual->genero = strdup(novo_dados->genero);
            atual->autor = strdup(novo_dados->autor);

            if (!atual->genero || !atual->autor) {
                perror("Erro ao alocar memória para os novos dados do livro");
                exit(EXIT_FAILURE);
            }

            printf("Livro '%s' editado com sucesso!\n", atual->nome);
            return;
        } else if (cmp < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    printf("Erro: Livro '%s' não encontrado no catálogo.\n", novo_dados->nome);
}
// Libera todos os nós da árvore recursivamente
void free_livros(Livro *atual) {
    if (!atual) return;

    // Libera os nós filhos
    free_livros(atual->esq);
    free_livros(atual->dir);

    // Libera os dados do nó atual
    free(atual->nome);
    free(atual->genero);
    free(atual->autor);
    free(atual);
}

// Libera toda a memória associada ao catálogo e seus livros
void free_catalogo(Catalogo *c) {
    if (!c) return;

    // Libera os livros da árvore
    free_livros(c->raiz);

    // Libera o próprio catálogo
    free(c);
}
