#include "../include/catalogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

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

//Inicializa um livro
Livro* livro_init(const char *nome, const char *genero, const char *autor) {
    if (!is_valid_string(nome) || !is_valid_string(genero) || !is_valid_string(autor)) {
        printf("Erro: Nome, gênero e/ou autor inválido.\n");
        return NULL;
    }

    Livro *l = malloc(sizeof(Livro));
    if (!l) {
        perror("Erro ao alocar memória para Livro");
        exit(EXIT_FAILURE);
    }

    l->real_nome = strdup(nome); // Nome original com acentos
    l->nome = strdup(remover_acentos(to_lowercase(nome))); // Nome normalizado
    l->real_genero = strdup(genero);
    l->genero = strdup(remover_acentos(to_lowercase(genero)));
    l->real_autor = strdup(autor);
    l->autor = strdup(remover_acentos(to_lowercase(autor)));

    if (!l->nome || !l->real_nome || !l->genero || !l->autor || !l->real_autor || !l->real_genero) {
        free(l->nome);
        free(l->real_nome);
        free(l->genero);
        free(l->real_autor);
        free(l->real_genero);
        free(l->autor);
        free(l);
        perror("Erro ao alocar memória para strings do Livro");
        exit(EXIT_FAILURE);
    }

    l->esq = l->dir = NULL;
    l->qtd = 1;

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


Livro* min_aux(Livro* l) {
    
    while (l && l->esq) {
        l = l->esq;
    }
    return l;
}


// Atualiza os dados de um livro
void livro_set(Livro *l, const char *nome, const char *genero, const char *autor) {
    if (!l) return;

    free(l->nome);
    free(l->real_nome);
    free(l->genero);
    free(l->real_autor);
    free(l->real_genero);
    free(l->autor);

    if (!is_valid_string(nome) || !is_valid_string(genero) || !is_valid_string(autor)) {
        printf("Erro: Nome, gênero e autor devem conter apenas letras e espaços.\n");
        return;
    }

    l->real_nome = strdup(nome); // Nome original com acentos
    l->nome = strdup(remover_acentos(to_lowercase(nome))); // Nome normalizado
    l->real_genero = strdup(genero);
    l->genero = strdup(remover_acentos(to_lowercase(genero)));
    l->real_autor = strdup(autor);
    l->autor = strdup(remover_acentos(to_lowercase(autor)));

    if (!l->nome || !l->real_nome || !l->genero || !l->autor || !l->real_autor || !l->real_genero) {
        free(l->nome);
        free(l->real_nome);
        free(l->genero);
        free(l->real_autor);
        free(l->real_genero);
        free(l->autor);
        free(l);
        perror("Erro ao alocar memória para atualizar os dados do Livro");
        exit(EXIT_FAILURE);
    }
}

// Função auxiliar para adicionar um nó à árvore binária
Livro* add_aux(Livro *atual, Livro *novo) {
    if (!atual) return novo;

    int cmp_nome = strcmp(novo->nome, atual->nome);
    int cmp_autor = strcmp(novo->autor, atual->autor);

    if (cmp_nome < 0) {
        atual->esq = add_aux(atual->esq, novo);
    } else if (cmp_nome > 0) {
        atual->dir = add_aux(atual->dir, novo);
    } else if (cmp_autor == 0) { // Mesmo nome e mesmo autor
        atual->qtd++;
    } else { // Mesmos nomes, mas autores diferentes
        atual->dir = add_aux(atual->dir, novo); // Inserimos no lado direito
    }

    return atual;
}

// Adiciona um livro ao catálogo
Livro* add_livro(Catalogo *c, Livro *l) {
    if (!c || !l) return NULL;

    c->raiz = add_aux(c->raiz, l);

    printf("O livro %s foi adicionado com sucesso\n", l->real_nome);
    return l;
}

// Lista todos os livros usando in-order traversal
void listar_aux(Livro *l) {
    if (!l) return;

    listar_aux(l->esq);

    printf("Nome: %s | Autor: %s | Gênero: %s | Quantidade: %d\n", 
        l->real_nome, l->real_autor, l->real_genero, l->qtd);

    listar_aux(l->dir);
}

void listar_livros(Catalogo *c) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio.\n");
        return;
    }

    listar_aux(c->raiz);
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


Livro* edita_aux(Livro *atual, const char *nome, const char *autor, Livro *novo_dados) {
    if (!atual) return NULL;

    int cmp_nome = strcmp(nome, atual->nome);
    int cmp_autor = autor ? strcmp(autor, atual->autor) : 0;

    if (cmp_nome < 0) {
        atual->esq = edita_aux(atual->esq, nome, autor, novo_dados);
    } else if (cmp_nome > 0) {
        atual->dir = edita_aux(atual->dir, nome, autor, novo_dados);
    } else if (cmp_autor == 0) { // Found the book
        free(atual->nome);
        free(atual->real_nome);
        free(atual->genero);
        free(atual->real_autor);
        free(atual->real_genero);
        free(atual->autor);


        livro_set(atual, novo_dados->real_nome, novo_dados->real_genero, novo_dados->real_autor);

        if (!atual) {
            perror("Erro ao alocar memória para os novos dados do livro");
            exit(EXIT_FAILURE);
        }

        printf("Livro '%s' do autor '%s' editado com sucesso!\n", atual->real_nome, atual->autor);
    }

    return atual;
}


Livro* editar_livro(Catalogo *c, const char *nome, Livro *novo_dados) {
    if (!c || !c->raiz || !nome || !novo_dados) {
        printf("Erro: Catálogo vazio ou dados inválidos.\n");
        return NULL;
    }

    int count = 0;
    contar_livros(c->raiz, nome, &count);

    char autor[100] = {0};
    if (count > 1) {
        printf("Existem %d livros com o nome '%s'. Por favor, especifique o autor para editar: ", count, nome);
        fgets(autor, sizeof(autor), stdin);
        autor[strcspn(autor, "\n")] = '\0'; 
    }

    c->raiz = edita_aux(c->raiz, nome, count > 1 ? autor : NULL, novo_dados);

    return c->raiz;
}



Livro* remove_aux(Livro *atual, const char *nome, const char *autor, Livro **removido) {
    if (!atual) return NULL;

    int cmp_nome = strcmp(nome, atual->nome);
    int cmp_autor = autor ? strcmp(autor, atual->autor) : 0;

    if (cmp_nome < 0) {
        atual->esq = remove_aux(atual->esq, nome, autor, removido);
    } else if (cmp_nome > 0) {
        atual->dir = remove_aux(atual->dir, nome, autor, removido);
    } else { 
        *removido = atual;

        if (atual->qtd > 1) {
            atual->qtd--;
            return atual;
        } else { 
            Livro *temp;
            if (!atual->esq && !atual->dir) {//sem filhos
                temp = NULL;
            } else if (!atual->esq) { //apenas filho direito
                temp = atual->dir;
            } else if (!atual->dir) { // apenas filho esquerdo
                temp = atual->esq;
            } else { // dois filhos
                Livro *sucessor = min_aux(atual->dir);

                free(atual->nome);
                free(atual->genero);
                free(atual->autor);

                atual->nome = strdup(sucessor->nome);
                atual->genero = strdup(sucessor->genero);
                atual->autor = strdup(sucessor->autor);

                atual->dir = remove_aux(atual->dir, sucessor->nome, sucessor->autor, &sucessor);
                return atual;
            }

            free(atual->nome);
            free(atual->genero);
            free(atual->autor);
            free(atual);

            return temp;
        }
    }

    return atual;
}


Livro* remover_livro(Catalogo *c, const char *nome) {
    if (!c || !c->raiz) return NULL;

    Livro *removido = NULL;
    int count = 0;
    contar_livros(c->raiz, nome, &count);

    char autor[100] = {0};
    if (count > 1) {
        printf("Existem %d livros com o nome '%s'. Por favor, especifique o autor para remover: ", count, nome);
        fgets(autor, sizeof(autor), stdin);
        autor[strcspn(autor, "\n")] = '\0'; 
    }

    c->raiz = remove_aux(c->raiz, remover_acentos(to_lowercase(nome)), count > 1 ? remover_acentos(to_lowercase(autor)) : NULL, &removido);

    if (removido)
        printf("Livro '%s' removido com sucesso!\n", removido->real_nome);
    else
        printf("Erro: Livro '%s' não encontrado no catálogo.\n", nome);

    return removido;
}

void devolve_livro_aux(Livro *atual, Livro *l){
    if (!atual || !l->nome) return;

    int cmp_nome = strcmp(l->nome, atual->nome);

    if (cmp_nome < 0)
        return devolve_livro_aux(atual->esq, l);
    
    if (cmp_nome > 0)
        return devolve_livro_aux(atual->dir, l);
    
    atual->qtd++;
}

void devolve_livro(Catalogo *c, char *nome, char *autor){
    Livro *temp = livro_init(nome,"",autor);
    devolve_livro_aux(c->raiz,temp);
}

void empresta_livro_aux(Livro *atual, Livro *l){
    if (!atual || !l->nome) return;

    int cmp_nome = strcmp(l->nome, atual->nome);

    if (cmp_nome < 0)
        return empresta_livro_aux(atual->esq, l);
    
    if (cmp_nome > 0)
        return empresta_livro_aux(atual->dir, l);
    
    atual->qtd--;
}

void emprestar_livro(Catalogo *c, char *nome, char *autor){
    Livro *temp = livro_init(nome,"",autor);
    empresta_livro_aux(c->raiz,temp);
}

// Verifica se um livro está no catálogo pelo nome
int verifica_aux(Livro *atual, const char* nome_normalizado) {
    if (!atual || !nome_normalizado) return 0;

    int cmp_nome = strcmp(nome_normalizado, atual->nome);

    if (cmp_nome < 0)
        return verifica_aux(atual->esq, nome_normalizado);
    
    if (cmp_nome > 0)
        return verifica_aux(atual->dir, nome_normalizado);

    printf("O livro %s está no catálogo\n", atual->real_nome); // Exibe o nome original com acentos
    return 1;
}

int verifica_livro(Catalogo *c, const char *nome) {
    if (!c || !nome) return 0;

    char nome_normalizado[256];

    strcpy(nome_normalizado, remover_acentos(to_lowercase(nome)));

    return verifica_aux(c->raiz, nome_normalizado);
}

// Libera todos os nós da árvore recursivamente
void free_livros(Livro *atual) {
    if (!atual) return;

    free_livros(atual->esq);
    free_livros(atual->dir);

    free(atual->nome);
    free(atual->real_nome);
    free(atual->genero);
    free(atual->autor);
    free(atual);
}

// Libera toda a memória associada ao catálogo e seus livros
void free_catalogo(Catalogo *c) {
    if (!c) return;

    free_livros(c->raiz);
    free(c);
}
