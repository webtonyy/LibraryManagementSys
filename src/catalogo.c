#include "../include/catalogo.h"
#include "../include/aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

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
    l->real_nome = strdup(nome);
    l->nome = strdup(normalize_string(nome));
    l->real_genero = strdup(genero);
    l->genero = strdup(normalize_string(genero));
    l->real_autor = strdup(autor);
    l->autor = strdup(normalize_string(autor));
    l->esq = l->dir = NULL;
    l->qtd = 1;
    l->status = 0;
    return l;
}


// Inicializa um l catálogo
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


// Função auxiliar para adicionar um nó à árvore binária
Livro* add_aux(Livro *atual, Livro *l) {
    if (!atual) return l;

    int cmp_nome = strcmp(l->nome, atual->nome);
    int cmp_autor = strcmp(l->autor, atual->autor);

    if (cmp_nome < 0) {
        atual->esq = add_aux(atual->esq, l);
    } else if (cmp_nome > 0 || cmp_autor != 0) {
        atual->dir = add_aux(atual->dir, l);
    } else {
        atual->qtd++;
    }
    
    return atual;
}

Livro* add_livro(Catalogo *c, Livro *l) {
    if (!c || !l) return NULL;

    if (c->raiz == NULL) {
        c->raiz = l;
        return c->raiz;
    }

    c->raiz = add_aux(c->raiz, l);
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


Livro* edita_aux(Livro *atual, const char *nome, const char *autor, Livro *novo_dados) {
    if (!atual) return NULL;

    int cmp_nome = strcmp(nome, atual->nome);
    int cmp_autor = autor ? strcmp(autor, atual->autor) : 0;

    if (cmp_nome < 0) {
        atual->esq = edita_aux(atual->esq, nome, autor, novo_dados);
    } else if (cmp_nome > 0) {
        atual->dir = edita_aux(atual->dir, nome, autor, novo_dados);
    } else if (cmp_autor == 0) { // Found the book
        free(atual->real_nome);
        free(atual->real_autor);

        atual->real_nome = strdup(novo_dados->real_nome);
        atual->nome = strdup(nome);
        atual->autor = strdup(autor);
        atual->real_autor = strdup(novo_dados->real_autor);


        printf("Livro '%s' do autor '%s' editado com sucesso!\n", atual->real_nome, atual->autor);
        return atual;
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

    Livro *editado = edita_aux(c->raiz, nome, count > 1 ? autor : NULL, novo_dados);
    return editado;
}

Livro* remove_aux(Livro *atual, const char *nome, const char *autor, Livro **removido) {
    if (!atual) return NULL;
    
    int cmp_nome = strcmp(nome, atual->nome);
    int cmp_autor = autor ? strcmp(autor, atual->autor) : 0;

    if (cmp_nome < 0) {
        atual->esq = remove_aux(atual->esq, nome, autor, removido);
    } else if (cmp_nome > 0) {
        atual->dir = remove_aux(atual->dir, nome, autor, removido);
    } else if (cmp_autor == 0) {
        // Encontrou o nó
        if (atual->qtd > 1) {
            atual->qtd--;
            return atual;
        }
        
        *removido = atual;

        // Caso sem filhos
        if (!atual->esq && !atual->dir) {
            return NULL;
        }

        // Caso com um filho
        if (!atual->esq) {
            Livro *temp = atual->dir;
            return temp;
        }
        
        if (!atual->dir) {
            Livro *temp = atual->esq;
            return temp;
        }

        // Caso com dois filhos: encontrar sucessor
        Livro *sucessor = atual->dir;
        while (sucessor && sucessor->esq != NULL) {
            sucessor = sucessor->esq;
        }

        // Substituir os dados do nó atual pelos do sucessor
        if (atual->nome) free(atual->nome);
        if (atual->real_nome) free(atual->real_nome);
        if (atual->genero) free(atual->genero);
        if (atual->real_genero) free(atual->real_genero);
        if (atual->autor) free(atual->autor);
        if (atual->real_autor) free(atual->real_autor);

        atual->nome = strdup(sucessor->nome);
        atual->real_nome = strdup(sucessor->real_nome);
        atual->genero = strdup(sucessor->genero);
        atual->real_genero = strdup(sucessor->real_genero);
        atual->autor = strdup(sucessor->autor);
        atual->real_autor = strdup(sucessor->real_autor);

        // Remover o sucessor recursivamente
        atual -> dir = remove_aux(atual -> dir, sucessor -> nome, sucessor -> autor, &sucessor);

    }
    
    return atual;
}


Livro* remover_livro(Catalogo *c, const char *nome) {
    if (!c || !c->raiz) {
        printf("Erro: O catálogo está vazio ou inválido.\n");
        return NULL;
    }

    Livro *removido = NULL;
    int count = 0; // Contar livros com o mesmo nome
    contar_livros(c->raiz, nome, &count);

    char autor[100] = {0};
    if (count > 1) {
        printf("Existem %d livros com o nome '%s'. Por favor, especifique o autor para remover: ", count, nome);
        fgets(autor, sizeof(autor), stdin);
        autor[strcspn(autor, "\n")] = '\0'; // Remove o '\n'
    }

    c->raiz = remove_aux(c->raiz, remover_acentos(to_lowercase(nome)), count > 1 ? remover_acentos(to_lowercase(autor)) : NULL, &removido);

    if (removido) {
        printf("Livro '%s' removido com sucesso!\n", removido->real_nome);
        
        // Liberar memória do nó removido
        free(removido->nome);
        free(removido->real_nome);
        free(removido->genero);
        free(removido->real_genero);
        free(removido->autor);
        free(removido->real_autor);
        free(removido);
    } else {
        printf("Erro: Livro '%s' não encontrado no catálogo.\n", nome);
    }

    return removido;
}

void verificar_status(Catalogo *c, const char *nome, const char *autor) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio ou inválido.\n");
        return;
    }

    Livro *atual = c->raiz;
    char nome_normalizado[256];
    strcpy(nome_normalizado, remover_acentos(to_lowercase(nome)));

    while (atual) {
        int cmp_nome = strcmp(nome_normalizado, atual->nome);
        int cmp_autor = strcmp(autor, atual->autor);

        if (cmp_nome < 0) {
            atual = atual->esq;
        } else if (cmp_nome > 0) {
            atual = atual->dir;
        } else if (cmp_autor == 0) {
            // Found the book
            printf("Livro: %s\n", atual->real_nome);
            printf("Autor: %s\n", atual->real_autor);
            printf("Gênero: %s\n", atual->real_genero);
            printf("Quantidade: %d\n", atual->qtd);
            printf("Status: %s\n", (atual->status == 0) ? "Disponível" : "Indisponível");
            return;
        } else {
            // If names match but authors do not, continue search in right subtree
            atual = atual->dir;
        }
    }

    printf("Livro '%s' do autor '%s' não encontrado no catálogo.\n", nome, autor);
}

int devolve_livro_aux(Livro *atual, Livro *l){
    if (!atual || !l->nome) return 2;

    int cmp_nome = strcmp(l->nome, atual->nome);
    int cmp_autor = strcmp(l->autor, atual->autor);

    if (cmp_nome < 0) {
        atual->esq = add_aux(atual->esq, l);
    } else if (cmp_nome > 0) {
        atual->dir = add_aux(atual->dir, l);
    } else if (cmp_nome == 0) {
        if(cmp_autor == 0){
            atual->qtd++;
            atual->status = 0;
            printf("Obrigado pela devolução.");
            return 1;
        }
        else 
            atual->dir = add_aux(atual->dir, l);
    }
    return 1;
}

void devolve_livro(Catalogo *c, char *nome, char *autor){
    Livro *temp = livro_init(nome,"",autor);

    int try = devolve_livro_aux(c -> raiz,temp);

    // Se o livro foi adicionado com sucesso ou devolvido
    if(try == 1){
        add_livro(c,temp); 
        printf("Obrigado pela devolução ou doação!");
       // Libere o livro temporário após adicioná-lo ao catálogo
        free(temp -> nome); 
        free(temp -> real_nome); 
        free(temp -> genero); 
        free(temp -> real_genero); 
        free(temp -> autor); 
        free(temp -> real_autor); 
        free(temp); 
    }
}


void empresta_livro_aux(Livro *atual, Livro *l) {
    if (!atual || !l || !l->nome) return;

    int cmp_nome = strcmp(l->nome, atual->nome);
    int cmp_autor = strcmp(l->autor, atual->autor);

    if (cmp_nome < 0) {
        empresta_livro_aux(atual->esq, l);
    } else if (cmp_nome > 0) {
        empresta_livro_aux(atual->dir, l);
    } else if (cmp_autor == 0) { // Mesmo nome e mesmo autor
        if (atual->qtd > 0) {
            atual->qtd--;
            if (atual->qtd == 0)
                atual->status = 1; // Marca como indisponível
            printf("Livro emprestado com sucesso.\n");
            return;
        } else {
            printf("Livro não disponível para aluguel.\n");
        }
    }
}


void emprestar_livro(Catalogo *c, char *nome, char *autor){
    if (!c) {
        printf("O catálogo está vazio! Por favor, adicione um livro antes de realizar quaisquer operações\n");
        return;
    }
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