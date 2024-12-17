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
        perror("Erro ao alocar memória para o livro");
        exit(EXIT_FAILURE);
    }

    l->nome = strdup(nome);  // Aloca e copia o título
    l->genero = strdup(genero); // Aloca e copia o gênero
    l->autor = strdup(autor);  // Aloca e copia o autor
    l->qtd = 1;                // Inicializa a quantidade como 1
    l->status = false;        // Inicializa como não alugado

    return l;
}


// Função para inicializar um nó da árvore binária
No* no_init(Livro *livro) {
    if (!livro) {
        printf("Erro: Livro inválido.\n");
        return NULL;
    }

    // Aloca memória para o nó
    No *novo_no = (No *)malloc(sizeof(No));
    if (!novo_no) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }

    // Inicializa os campos do nó
    novo_no->livro = livro;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;

    return novo_no;
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
No* add_aux(No *atual, Livro *livro) {
    if (!atual) {
        // Cria um novo nó se o local estiver vazio
        No *novo_no = no_init(livro);
        if (!novo_no) {
            perror("Erro ao alocar memória para o nó");
            exit(EXIT_FAILURE);
        }
        return novo_no;
    }

    // Compara os livros pelo título e autor
    int cmp_nome = strcmp(livro->nome, atual->livro->nome);
    int cmp_autor = strcmp(livro->autor, atual->livro->autor);

    if (cmp_nome < 0) {
        // Adiciona à subárvore esquerda
        atual->esquerda = add_aux(atual->esquerda, livro);
    } else if (cmp_nome > 0 || cmp_autor != 0) {
        // Adiciona à subárvore direita
        atual->direita = add_aux(atual->direita, livro);
    } else {
        // Incrementa a quantidade se o livro já existir
        atual->livro->qtd++;
    }

    return atual;
}

// Função principal para adicionar um livro ao catálogo
Livro* add_livro(Catalogo *c, Livro *l) {
    if (!c || !l) {
        fprintf(stderr, "Erro: Catálogo ou livro inválido.\n");
        return NULL;
    }

    // Adiciona o livro à árvore binária
    c->raiz = add_aux(c->raiz, l);

    // Incrementa o número total de livros no catálogo
    c->nl++;

    return l;
}

// Função auxiliar para listar os livros em ordem (in-order traversal)
void listar_aux(No *no) {
    if (!no) return;

    // Percorre a subárvore esquerda
    listar_aux(no->esquerda);

    // Imprime os dados do livro no nó atual
    Livro *livro = no->livro;
    printf("Título: %s | Autor: %s | Gênero: %s | Quantidade: %d \n",
        livro->nome,
        livro->autor,
        livro->genero,
        livro->qtd);

    // Percorre a subárvore direita
    listar_aux(no->direita);
}

// Função principal para listar todos os livros no catálogo
void listar_livros(Catalogo *c) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio.\n");
        return;
    }

    // Chama a função auxiliar para percorrer a árvore
    listar_aux(c->raiz);
}

No* edita_aux(No *atual, const char *nome_errado, const char *autor, const char *novo_nome) {
    if (!atual) return NULL;

    // Compara o título e o autor do livro
    int cmp_nome = strcmp(nome_errado, atual->livro->nome);
    int cmp_autor = strcmp(autor, atual->livro->autor);

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        atual->esquerda = edita_aux(atual->esquerda, nome_errado, autor, novo_nome);
    } else if (cmp_nome > 0 || cmp_autor != 0) {
        // Procura na subárvore direita
        atual->direita = edita_aux(atual->direita, nome_errado, autor, novo_nome);
    } else if (cmp_autor == 0) {
        // Livro encontrado: renomeia o título
        atual->livro->nome = strdup(novo_nome); // Aloca e copia o novo nome
        printf("Livro renomeado com sucesso! Novo título: '%s'\n", atual->livro->nome);
    }

    return atual;
}

// Função auxiliar para remover um livro da árvore binária
No* remove_aux(No *atual, const char *nome, const char *autor) {
    if (!atual) return NULL;

    // Compara o título do livro
    int cmp_nome = strcmp(nome, atual->livro->nome);
    int cmp_autor = autor ? strcmp(autor, atual->livro->autor) : 0;

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        atual->esquerda = remove_aux(atual->esquerda, nome, autor);
    } else if (cmp_nome > 0) {
        // Procura na subárvore direita
        atual->direita = remove_aux(atual->direita, nome, autor);
    } else if (cmp_autor == 0) {
        // Livro encontrado
        if (atual->livro->qtd > 1) {
            // Decrementa a quantidade se for maior que 1
            atual->livro->qtd--;
            printf("Quantidade do livro '%s' diminuída para %d.\n", nome, atual->livro->qtd);
            return atual;
        }

        // Caso sem filhos
        if (!atual->esquerda && !atual->direita) {
            free(atual->livro->nome);
            free(atual->livro->autor);
            free(atual->livro->genero);
            free(atual->livro);
            free(atual);
            return NULL;
        }

        // Caso com um filho
        if (!atual->esquerda) {
            No *temp = atual->direita;
            free(atual->livro->nome);
            free(atual->livro->autor);
            free(atual->livro->genero);
            free(atual->livro);
            free(atual);
            return temp;
        }

        if (!atual->direita) {
            No *temp = atual->esquerda;
            free(atual->livro->nome);
            free(atual->livro->autor);
            free(atual->livro->genero);
            free(atual->livro);
            free(atual);
            return temp;
        }

        // Caso com dois filhos: encontrar o sucessor
        No *sucessor = atual->direita;
        while (sucessor && sucessor->esquerda != NULL) {
            sucessor = sucessor->esquerda;
        }

        // Substituir os dados do nó atual pelos do sucessor
        free(atual->livro->nome);
        free(atual->livro->autor);
        free(atual->livro->genero);

        atual->livro->nome = strdup(sucessor->livro->nome);
        atual->livro->autor = strdup(sucessor->livro->autor);
        atual->livro->genero = strdup(sucessor->livro->genero);
        atual->livro->qtd = sucessor->livro->qtd;

        // Remove o sucessor recursivamente
        atual->direita = remove_aux(atual->direita, sucessor->livro->nome, sucessor->livro->autor);
    }

    return atual;
}


Livro* editar_livro(Catalogo *c, const char *nome_errado, const char *novo_nome, const char *autor) {
    if (!c || !c->raiz || !nome_errado || !novo_nome || !autor) {
        printf("Erro: Catálogo vazio ou dados inválidos.\n");
        return NULL;
    }

    // Edita o livro usando a função auxiliar
    No *editado = edita_aux(c->raiz, nome_errado, autor, novo_nome);

    if (!editado) {
        printf("Erro: Livro não encontrado no catálogo.\n");
        return NULL;
    }

    // Verifica se a posição do nó ainda é válida após a edição
    int qtd = editado->livro->qtd; // Salva a quantidade de exemplares
    editado->livro->qtd = 1;      // Temporariamente ajusta para 1 para remoção
    c->raiz = remove_aux(c->raiz, novo_nome, autor); // Remove o nó editado

    Livro *novo_livro = livro_init(novo_nome,editado->livro->genero,autor);
    novo_livro->qtd = qtd;       // Restaura a quantidade original
    novo_livro->status = editado->livro->status;

    c->raiz = add_aux(c->raiz, novo_livro); // Reinsere o nó com os dados atualizados

    printf("Livro '%s' atualizado com sucesso no catálogo.\n", novo_livro->nome);
    return novo_livro;
}

// Função principal para remover um livro do catálogo
void remover_livro(Catalogo *c, const char *nome, const char *autor) {
    if (!c || !c->raiz) {
        printf("Erro: O catálogo está vazio ou inválido.\n");
        return;
    }

    // Remove o livro usando a função auxiliar
    c->raiz = remove_aux(c->raiz, nome,autor);

    if (c) { 
        c -> nl--;
        printf("Livro removido com sucesso!\n");
    } else printf("Erro ao tentar remover'%s'\n", nome); 
}

// Função para verificar o status de um livro no catálogo
void verificar_status(Catalogo *c, const char *nome, const char *autor) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio ou inválido.\n");
        return;
    }

    // Normaliza o nome do livro
    char nome_normalizado[256];
    strcpy(nome_normalizado, remover_acentos(to_lowercase(nome)));

    No *atual = c->raiz;

    while (atual) {
        // Compara o título e o autor do livro
        int cmp_nome = strcmp(nome_normalizado, atual->livro->nome);
        int cmp_autor = strcmp(autor, atual->livro->autor);

        if (cmp_nome < 0) {
            atual = atual->esquerda; // Procura na subárvore esquerda
        } else if (cmp_nome > 0) {
            atual = atual->direita; // Procura na subárvore direita
        } else if (cmp_autor == 0) {
            // Livro encontrado
            printf("Título: %s | Autor: %s | Gênero: %s | Quantidade: %d \n",
            atual->livro->nome,
            atual->livro->autor,
            atual->livro->genero,
            atual->livro->qtd);

            // Verifica e exibe o status do livro
            if (atual->livro->qtd >= 1) {
                printf("Status: Disponível\n");
                atual->livro->status = false; // Atualiza o status para disponível
            } else {
                printf("Status: Indisponível\n");
                atual->livro->status = true; // Atualiza o status para status
            }
            return;
        } else {
            // Se os títulos coincidirem, mas os autores não, continua na subárvore direita
            atual = atual->direita;
        }
    }

    // Caso o livro não seja encontrado
    printf("Livro '%s' do autor '%s' não encontrado no catálogo.\n", nome, autor);
}

// Função auxiliar para devolver um livro na árvore binária
int devolve_livro_aux(No *atual, Livro *l) {
    if (!atual || !l->nome) return 2;

    // Compara o título e o autor do livro
    int cmp_nome = strcmp(l->nome, atual->livro->nome);
    int cmp_autor = strcmp(l->autor, atual->livro->autor);

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        atual->esquerda = add_aux(atual->esquerda, l);
    } else if (cmp_nome > 0) {
        // Procura na subárvore direita
        atual->direita = add_aux(atual->direita, l);
    } else if (cmp_nome == 0) {
        if (cmp_autor == 0) {
            // Livro encontrado: incrementa a quantidade e atualiza o status
            atual->livro->qtd++;
            atual->livro->status = false; // Atualiza o status para disponível
            printf("Obrigado pela devolução do livro '%s'!\n", l->nome);
            return 1;
        } else {
            // Continua na subárvore direita se os autores forem diferentes
            atual->direita = add_aux(atual->direita, l);
        }
    }
    return 2; // Caso o livro não seja encontrado, será tratado como doado
}

// Função principal para devolver um livro ao catálogo
int devolve_livro(Catalogo *c, char *nome, char *autor) {
    if (!c || !nome || !autor) {
        printf("Erro: Parâmetros inválidos para devolução.\n");
        return -1; // Retorna -1 em caso de erro
    }

    // Cria um livro temporário com os dados fornecidos
    Livro *temp = livro_init(nome, "", autor);

    // Tenta devolver o livro usando a função auxiliar
    int resultado = devolve_livro_aux(c->raiz, temp);

    if (resultado == 2) {
        // Adiciona o livro ao catálogo se ele não existir
        add_livro(c, temp);
        printf("Obrigado pela doação do livro '%s'!\n", nome);
    }

    // Libera a memória do livro temporário
    free(temp->nome);
    free(temp->genero);
    free(temp->autor);
    free(temp);

    return resultado;
}

// Função auxiliar para alugar um livro na árvore binária
int empresta_livro_aux(No *atual, Livro *l) {
    if (!atual || !l || !l->nome) return -1; // Retorna -1 em caso de erro

    // Compara o título e o autor do livro
    int cmp_nome = strcmp(l->nome, atual->livro->nome);
    int cmp_autor = strcmp(l->autor, atual->livro->autor);

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        return empresta_livro_aux(atual->esquerda, l);
    } else if (cmp_nome > 0) {
        // Procura na subárvore direita
        return empresta_livro_aux(atual->direita, l);
    } else if (cmp_autor == 0) { 
        // Livro encontrado
        if (atual->livro->qtd > 0) {
            atual->livro->qtd--; // Decrementa a quantidade
            if (atual->livro->qtd == 0)
                atual->livro->status = true; // Marca como indisponível se a quantidade for 0
            printf("Livro '%s' emprestado com sucesso.\n", l->nome);
            return 1; // Retorna 1 indicando sucesso no empréstimo
        } else {
            printf("Livro '%s' não está disponível para aluguel.\n", l->nome);
            return 0; // Retorna 0 indicando que o livro não está disponível
        }
    }

    return -1; // Retorna -1 caso ocorra algum erro inesperado
}

// Função principal para alugar um livro do catálogo
int emprestar_livro(Catalogo *c, char *nome, char *autor) {
    if (!c || !c->raiz || !nome || !autor) {
        printf("Erro: Parâmetros inválidos ou catálogo vazio.\n");
        return -1; // Retorna -1 em caso de erro
    }

    // Cria um livro temporário com os dados fornecidos
    Livro *temp = livro_init(nome, "", autor);

    // Tenta alugar o livro usando a função auxiliar
    int resultado = empresta_livro_aux(c->raiz, temp);

    // Libera a memória do livro temporário
    free(temp->nome);
    free(temp->genero);
    free(temp->autor);
    free(temp);

    return resultado; // Retorna o resultado da operação (1, 0 ou -1)
}

// Função auxiliar para buscar todos os livros com o mesmo nome
void buscar_aux(No *atual, const char *nome_normalizado, int *encontrados) {
    if (!atual || !nome_normalizado) return;

    // Compara o título do livro
    int cmp_nome = strcmp(nome_normalizado, atual->livro->nome);

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        buscar_aux(atual->esquerda, nome_normalizado, encontrados);
    } else if (cmp_nome > 0) {
        // Procura na subárvore direita
        buscar_aux(atual->direita, nome_normalizado, encontrados);
    } else {
        // Livro encontrado: exibe as informações

        printf("Título: %s\n", atual->livro->nome);
        printf("Autor: %s\n", atual->livro->autor);
        printf("Gênero: %s\n", atual->livro->genero);
        printf("Quantidade: %d\n", atual->livro->qtd);
        printf("Status: %s\n", atual->livro->status ? "Emprestado" : "Disponível");
        printf("###########\n");

        (*encontrados)++; // Incrementa o contador de livros encontrados

        // Continua buscando na subárvore esquerda e direita para encontrar outros livros com o mesmo nome
        buscar_aux(atual->esquerda, nome_normalizado, encontrados);
        buscar_aux(atual->direita, nome_normalizado, encontrados);
    }
}

// Função principal para buscar um livro pelo nome no catálogo
int buscar_por_nome(Catalogo *c, const char *nome) {
    if (!c || !nome) {
        printf("Erro: Catálogo ou nome inválido.\n");
        return 0;
    }

    int encontrados = 0;
    contar_livros(c->raiz, nome, &encontrados);

    if (encontrados == 0) {
        printf("Nenhum livro com o título '%s' foi encontrado no catálogo.\n", nome);
    }

    return encontrados;
}




// Libera todos os nós da árvore recursivamente
void free_livros(No *atual) {
    if (!atual) return;

    // Libera os nós da subárvore esquerda e direita
    free_livros(atual->esquerda);
    free_livros(atual->direita);

    // Libera os campos do livro
    if (atual->livro) {
        free(atual->livro->nome);
        free(atual->livro->autor);
        free(atual->livro->genero);
        free(atual->livro);
    }

    // Libera o nó atual
    free(atual);
}




// Libera toda a memória associada ao catálogo e seus livros
void free_catalogo(Catalogo *c) {
    if (!c) return;

    // Libera todos os nós da árvore binária
    free_livros(c->raiz);

    // Libera a estrutura do catálogo
    free(c);
}
