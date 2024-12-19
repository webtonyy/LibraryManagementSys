#include "../include/catalogo.h"
#include "../include/aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

// Inicializa um livro
Livro* livro_init(const char *nome, const char *genero, const char *autor) {

    Livro *l = malloc(sizeof(Livro));
    if (!l) {
        perror("Erro ao alocar memória para o livro");
        exit(EXIT_FAILURE);
    }

    // Aloca e copia os valores originais
    l->nome = strdup(nome);
    l->genero = strdup(genero);
    l->autor = strdup(autor);

    // Normaliza os valores e armazena nos campos correspondentes
    l->nome_norma = strdup(normalize_string(nome));   // Normaliza o título
    l->genero_norma = strdup(normalize_string(genero)); // Normaliza o gênero
    l->autor_norma = strdup(normalize_string(autor));  // Normaliza o autor

    // Inicializa os outros campos
    l->qtd = 1;                // Inicializa a quantidade como 1
    l->status = false;         // Inicializa como não alugado

    return l;
}


// Função para inicializar um nó da árvore binária
No* no_init(Livro *livro) {
    if (!livro) {
        fprintf(stderr, "Erro: Livro inválido.\n");
        return NULL;
    }

    No *novo_no = (No *)malloc(sizeof(No));
    if (!novo_no) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }

    // Associa o livro ao nó
    novo_no->livro = livro;

    // Inicializa os ponteiros para os filhos como NULL
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;

    return novo_no;
}

void free_livro(Livro *livro) {
    if (!livro) return;

    free(livro->nome);
    free(livro->nome_norma);
    free(livro->autor);
    free(livro->autor_norma);
    free(livro->genero);
    free(livro->genero_norma);
    free(livro);
}


// Libera todos os nós da árvore recursivamente
void free_no(No *atual) {
    if (!atual) return;

    // Libera os nós da subárvore esquerda e direita
    free_no(atual->esquerda);
    free_no(atual->direita);

    // Libera os campos do livro
    if (atual->livro) {
        free_livro(atual->livro);
    }

    // Libera o nó atual
    free(atual);
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
        No *novo_no = no_init(livro);
        return novo_no;
    }

    if (!livro || !livro->nome_norma || !atual || !atual->livro || !atual->livro->nome_norma) {
        fprintf(stderr, "Erro: Ponteiro nulo detectado em add_aux.\n");
        exit(EXIT_FAILURE);
    }

    // Comparações baseadas nos campos normalizados
    int cmp_nome = strcmp(livro->nome_norma, atual->livro->nome_norma);
    int cmp_autor = strcmp(livro->autor_norma, atual->livro->autor_norma);

    if (cmp_nome < 0) {
        atual->esquerda = add_aux(atual->esquerda, livro);
    } else if (cmp_nome > 0 || cmp_autor != 0) {
        atual->direita = add_aux(atual->direita, livro);
    } else {
        atual->livro->qtd+=livro->qtd;
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
    printf("Título: %s | Autor: %s | Gênero: %s | Quantidade: %d \n",
        no->livro->nome,   // Usa o nome original para exibição
        no->livro->autor,  // Usa o autor original para exibição
        no->livro->genero, // Usa o gênero original para exibição
        no->livro->qtd);

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

No* remove_aux(No *atual, const char *nome, const char *autor) {
    if (!atual) return NULL;

    int cmp_nome = strcmp(nome, atual->livro->nome_norma);
    int cmp_autor = strcmp(autor, atual->livro->autor_norma);

    if (cmp_nome < 0) {
        atual->esquerda = remove_aux(atual->esquerda, nome, autor);
    } else if (cmp_nome > 0) {
        atual->direita = remove_aux(atual->direita, nome, autor);
    } else if (cmp_autor == 0) {
        if (atual->livro->qtd > 1) {
            atual->livro->qtd--;
            return atual;
        }

        // Caso sem filhos
        if (!atual->esquerda && !atual->direita) {
            free_livro(atual->livro);
            free(atual);
            return NULL;
        }

        // Caso com um filho
        if (!atual->esquerda || !atual->direita) {
            No *temp = atual->esquerda ? atual->esquerda : atual->direita;
            free_livro(atual->livro);
            free(atual);
            return temp;
        }

        // Caso com dois filhos
        No *sucessor = atual->direita;
        No *sucessor_pai = atual;

        while (sucessor->esquerda) {
            sucessor_pai = sucessor;
            sucessor = sucessor->esquerda;
        }

        // Substituir os dados do nó atual pelos do sucessor
        Livro *livro_temp = sucessor->livro;
        sucessor->livro = NULL; // Remover a referência ao livro no sucessor
        free_livro(atual->livro);
        atual->livro = livro_temp;

        // Ajustar os ponteiros para desconectar o sucessor
        if (sucessor_pai != atual) {
            sucessor_pai->esquerda = sucessor->direita;
        } else {
            sucessor_pai->direita = sucessor->direita;
        }

        free(sucessor);
    }

    return atual;
}

// Função para renomear um livro no catálogo
void editar_livro(Catalogo *c, const char *nome_errado, const char *novo_nome, const char *autor, const char *genero) {
    if (!c || !c->raiz) {
        fprintf(stderr, "Erro: O catálogo está vazio ou inválido.\n");
        return;
    }

    // Normaliza os nomes para comparação
    char *nome_errado_norma = strdup(normalize_string(nome_errado));
    char *novo_nome_norma = strdup(normalize_string(novo_nome));
    char *autor_norma = strdup(normalize_string(autor));

    // Busca e remove o livro com o nome errado
    No *atual = c->raiz;
    while (atual) {
        int cmp_nome = strcmp(nome_errado_norma, atual->livro->nome_norma);
        int cmp_autor = strcmp(autor_norma, atual->livro->autor_norma);

        if (cmp_nome == 0 && cmp_autor == 0) {
            break;
        }
        if (cmp_nome < 0) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }

    if (!atual) {
        fprintf(stderr, "Erro: Livro '%s' não encontrado no catálogo.\n", nome_errado);
        free(nome_errado_norma);
        free(novo_nome_norma);
        free(autor_norma);
        return;
    }

    // Armazena a quantidade do livro encontrado
    int quantidade_antiga = atual->livro->qtd;

    atual->livro->qtd =1;
    // Remove o nó do livro antigo
    c->raiz = remove_aux(c->raiz, nome_errado_norma, autor_norma);

    // Cria um novo livro com o nome atualizado e a mesma quantidade
    Livro *livro_atualizado = livro_init(novo_nome, genero, autor);
    livro_atualizado->qtd = quantidade_antiga;

    // Insere novamente o livro atualizado na árvore
    add_livro(c, livro_atualizado);

    printf("Livro '%s' renomeado para '%s' com sucesso!\n", nome_errado, novo_nome);

    // Libera as strings normalizadas
    free(nome_errado_norma);
    free(novo_nome_norma);
    free(autor_norma);
}

// Função principal para remover um livro do catálogo
void remover_livro(Catalogo *c, const char *nome, const char *autor) {
    if (!c || !c->raiz) {
        printf("Erro: O catálogo está vazio ou inválido.\n");
        return;
    }

    // Normaliza os parâmetros de entrada
    char *nome_norma = strdup(normalize_string(nome));
    char *autor_norma = strdup(normalize_string(autor));

    // Remove o livro usando a função auxiliar
    No *nova_raiz = remove_aux(c->raiz, nome_norma, autor_norma);

    c->raiz = nova_raiz;
    c->nl--; // Decrementa o número de livros no catálogo
    printf("Livro '%s' removido com sucesso!\n", nome);
    

    // Libera a memória das strings normalizadas
    free(nome_norma);
    free(autor_norma);
}

// Função para verificar o status de um livro no catálogo
void verificar_status(Catalogo *c, const char *nome, const char *autor) {
    if (!c || !c->raiz) {
        printf("O catálogo está vazio ou inválido.\n");
        return;
    }

    // Normaliza o nome e o autor para comparação
    char *nome_normalizado = strdup(normalize_string(nome));
    char *autor_normalizado = strdup(normalize_string(autor));

    No *atual = c->raiz;

    while (atual) {
        // Compara o título e o autor do livro usando os campos normalizados
        int cmp_nome = strcmp(nome_normalizado, atual->livro->nome_norma);
        int cmp_autor = strcmp(autor_normalizado, atual->livro->autor_norma);

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
                atual->livro->status = true; // Atualiza o status para indisponível
            }

            // Libera as strings normalizadas antes de retornar
            free(nome_normalizado);
            free(autor_normalizado);
            return;
        } else {
            // Se os títulos coincidirem, mas os autores não, continua na subárvore direita
            atual = atual->direita;
        }
    }

    // Caso o livro não seja encontrado
    printf("Livro '%s' do autor '%s' não encontrado no catálogo.\n", nome, autor);

    // Libera as strings normalizadas
    free(nome_normalizado);
    free(autor_normalizado);
}

int devolve_livro_aux(No *atual, Livro *l) {
    if (!atual || !l->nome_norma) return 2; // Retorna 2 se o livro não for encontrado

    // Compara o título e o autor do livro usando os campos normalizados
    int cmp_nome = strcmp(l->nome_norma, atual->livro->nome_norma);
    int cmp_autor = strcmp(l->autor_norma, atual->livro->autor_norma);

    if (cmp_nome < 0) {
        // Procura na subárvore esquerda
        return devolve_livro_aux(atual->esquerda, l);
    } else if (cmp_nome > 0) {
        // Procura na subárvore direita
        return devolve_livro_aux(atual->direita, l);
    } else if (cmp_nome == 0) {
        if (cmp_autor == 0) {
            // Livro encontrado: incrementa a quantidade e atualiza o status
            atual->livro->qtd++;
            atual->livro->status = false; // Atualiza o status para disponível
            printf("Obrigado pela devolução do livro '%s'!\n", l->nome);
            return 1; // Retorna 1 indicando que o livro foi devolvido
        }
    }

    return 2; // Retorna 2 se o livro não for encontrado
}


int devolve_livro(Catalogo *c, Livro *l) {
    if (!c || !l) {
        printf("Erro: Parâmetros inválidos para devolução.\n");
        return -1; // Retorna -1 em caso de erro
    }

    // Tenta devolver o livro usando a função auxiliar
    int resultado = devolve_livro_aux(c->raiz, l);

    if (resultado == 2) {
        // Adiciona o livro ao catálogo se ele não existir
        add_livro(c, l); // Adiciona com quantidade inicial correta (1)
        printf("Obrigado pela devolução ou doação do livro '%s'!\n", l->nome);
    }

    return resultado;
}



// Função auxiliar para alugar um livro na árvore binária
int empresta_livro_aux(No *atual, Livro *l) {
    if (!atual || !l || !l->nome_norma) return -1; // Retorna -1 em caso de erro

    // Compara o título e o autor do livro usando os campos normalizados
    int cmp_nome = strcmp(l->nome_norma, atual->livro->nome_norma);
    int cmp_autor = strcmp(l->autor_norma, atual->livro->autor_norma);

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
int emprestar_livro(Catalogo *c, Livro *l) {
    if (!c || !c->raiz || !l) {
        printf("Erro: Parâmetros inválidos ou catálogo vazio.\n");
        return -1; // Retorna -1 em caso de erro
    }


    // Tenta alugar o livro usando a função auxiliar
    int resultado = empresta_livro_aux(c->raiz, l);

    return resultado; // Retorna o resultado da operação (1, 0 ou -1)
}

// Função auxiliar para buscar todos os livros com o mesmo nome normalizado
void buscar_aux(No *atual, const char *nome_normalizado, int *encontrados) {
    if (!atual || !nome_normalizado) return;

    // Compara o título do livro usando o campo normalizado
    int cmp_nome = strcmp(nome_normalizado, atual->livro->nome_norma);

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

        // Continua buscando na subárvore esquerda e direita para encontrar outros livros com o mesmo nome normalizado
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

    // Normaliza o nome para comparação
    char *nome_normalizado =strdup(normalize_string(nome));

    int encontrados = 0;
    buscar_aux(c->raiz, nome_normalizado, &encontrados);


    // Libera a memória do nome normalizado
    free(nome_normalizado);

    return encontrados;
}

// Libera toda a memória associada ao catálogo e seus livros
void free_catalogo(Catalogo *c) {
    if (!c) return;

    // Libera todos os nós da árvore binária
    free_no(c->raiz);

    // Libera a estrutura do catálogo
    free(c);
}


void salvar_aux(No *raiz, FILE *arquivo) {
    if (!raiz) {
        bool nulo = true;
        fwrite(&nulo, sizeof(bool), 1, arquivo); // Marca nó nulo
        return;
    }

    bool nulo = false;
    fwrite(&nulo, sizeof(bool), 1, arquivo); // Marca nó não-nulo

    // Salva os dados do livro
    Livro *livro = raiz->livro;
    int nome_len = strlen(livro->nome) + 1;
    int nome_norma_len = strlen(livro->nome_norma) + 1;
    int genero_len = strlen(livro->genero) + 1;
    int genero_norma_len = strlen(livro->genero_norma) + 1;
    int autor_len = strlen(livro->autor) + 1;
    int autor_norma_len = strlen(livro->autor_norma) + 1;

    // Salva os tamanhos e conteúdos dos campos originais e normalizados
    fwrite(&nome_len, sizeof(int), 1, arquivo);
    fwrite(livro->nome, sizeof(char), nome_len, arquivo);
    fwrite(&nome_norma_len, sizeof(int), 1, arquivo);
    fwrite(livro->nome_norma, sizeof(char), nome_norma_len, arquivo);

    fwrite(&genero_len, sizeof(int), 1, arquivo);
    fwrite(livro->genero, sizeof(char), genero_len, arquivo);
    fwrite(&genero_norma_len, sizeof(int), 1, arquivo);
    fwrite(livro->genero_norma, sizeof(char), genero_norma_len, arquivo);

    fwrite(&autor_len, sizeof(int), 1, arquivo);
    fwrite(livro->autor, sizeof(char), autor_len, arquivo);
    fwrite(&autor_norma_len, sizeof(int), 1, arquivo);
    fwrite(livro->autor_norma, sizeof(char), autor_norma_len, arquivo);

    // Salva os demais campos
    fwrite(&livro->qtd, sizeof(int), 1, arquivo);
    fwrite(&livro->status, sizeof(bool), 1, arquivo);

    // Salva recursivamente as subárvores esquerda e direita
    salvar_aux(raiz->esquerda, arquivo);
    salvar_aux(raiz->direita, arquivo);
}

void salvar_catalogo(Catalogo *c, const char *nome_arquivo) {
    if (!c || !nome_arquivo) return;

    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo de configurações.");
        return;
    }

    salvar_aux(c->raiz, arquivo); // Salva a árvore binária
    fclose(arquivo);
}


No* carregar_aux(FILE *arquivo) {
    bool nulo;
    fread(&nulo, sizeof(bool), 1, arquivo);

    if (nulo) return NULL;

    // Lê os dados do livro
    int nome_len, nome_norma_len, genero_len, genero_norma_len, autor_len, autor_norma_len;

    // Lê o tamanho e o conteúdo do nome original
    fread(&nome_len, sizeof(int), 1, arquivo);
    char *nome = malloc(nome_len);
    fread(nome, sizeof(char), nome_len, arquivo);

    // Lê o tamanho e o conteúdo do nome normalizado
    fread(&nome_norma_len, sizeof(int), 1, arquivo);
    char *nome_norma = malloc(nome_norma_len);
    fread(nome_norma, sizeof(char), nome_norma_len, arquivo);

    // Lê o tamanho e o conteúdo do gênero original
    fread(&genero_len, sizeof(int), 1, arquivo);
    char *genero = malloc(genero_len);
    fread(genero, sizeof(char), genero_len, arquivo);

    // Lê o tamanho e o conteúdo do gênero normalizado
    fread(&genero_norma_len, sizeof(int), 1, arquivo);
    char *genero_norma = malloc(genero_norma_len);
    fread(genero_norma, sizeof(char), genero_norma_len, arquivo);

    // Lê o tamanho e o conteúdo do autor original
    fread(&autor_len, sizeof(int), 1, arquivo);
    char *autor = malloc(autor_len);
    fread(autor, sizeof(char), autor_len, arquivo);

    // Lê o tamanho e o conteúdo do autor normalizado
    fread(&autor_norma_len, sizeof(int), 1, arquivo);
    char *autor_norma = malloc(autor_norma_len);
    fread(autor_norma, sizeof(char), autor_norma_len, arquivo);

    // Lê os demais campos
    int qtd;
    bool status;
    
    fread(&qtd, sizeof(int), 1, arquivo);
    fread(&status, sizeof(bool), 1, arquivo);

    // Inicializa o livro com os campos originais
    Livro *livro = malloc(sizeof(Livro));
    
    if (!livro) {
        perror("Erro ao alocar memória para o livro");
        free(nome); free(nome_norma);
        free(genero); free(genero_norma);
        free(autor); free(autor_norma);
        return NULL;
    }

    livro->nome = nome;
    livro->nome_norma = nome_norma;
    livro->genero = genero;
    livro->genero_norma = genero_norma;
    livro->autor = autor;
    livro->autor_norma = autor_norma;
    livro->qtd = qtd;
    livro->status = status;

    // Inicializa o nó com o livro carregado
    No *no = no_init(livro);

    // Carrega recursivamente as subárvores esquerda e direita
    no->esquerda = carregar_aux(arquivo);
    no->direita = carregar_aux(arquivo);

    return no;
}


Catalogo* carregar_catalogo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo,"rb");
    
    Catalogo *c = catalogo_init();
    
    if (!arquivo) {
        printf("\nArquivo de configurações não encontrado. Criando um novo.\n");
        arquivo = fopen(nome_arquivo, "wb");

        if (!arquivo) {
            perror("Erro ao criar o arquivo");
            return NULL;
        }

        fclose(arquivo);
        return c; // Retorna um catálogo vazio
    }

    c->raiz = carregar_aux(arquivo); // Carrega a árvore binária
    fclose(arquivo);

    return c;
}
