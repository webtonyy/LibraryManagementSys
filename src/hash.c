#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/hash.h"
#include "../include/aux.h"


HashTable *hash_table_init(int tamanho){
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->tabela = (NoHash **)calloc(tamanho, sizeof(NoHash *));
    ht->tamanho = tamanho;
    return ht;
}


int funcaohash_string(const char *nome) {
    char *normalized = normalize_string(nome);
    int tam = strlen(normalized);
    unsigned int ch = 0;

    for (int i = 0; i < tam; i++) {
        ch += normalized[i] * (i + 1);
    }
    return ch % TABLE_SIZE;
}

// Função para inserir um livro na tabela hash
int inserir_livro(HashTable *t,Livro *l, const char *nome_no) {
    // Normaliza o nome do nó (implementação de normalize_string não fornecida)
    char *no_normalizado = normalize_string(nome_no);
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar nome do nó: %s\n", nome_no);
        return 0;
    }

    // Calcula o índice na tabela hash usando a função hash
    int id_no = funcaohash_string(no_normalizado);

    // Procura pelo nó na tabela
    NoHash *atual = t->tabela[id_no];
    while (atual != NULL) {
        if (strcmp(normalize_string(atual->nome), no_normalizado) == 0) {
            // Nó encontrado, verifica se o livro já existe
            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro_atual = atual->livros[i];
                if (strcmp(livro_atual->nome, l->nome) == 0) {
                    // Livro já existe, incrementa a quantidade
                    livro_atual->qtd++; // Incrementa a quantidade 
                    free(no_normalizado);
                    return 1;
                }
            }

            // Adiciona um novo livro ao vetor de livros do nó
            if (atual->quantidade == atual->capacidade) {
                // Redimensiona o vetor de livros se necessário
                atual->capacidade *= 2;
                atual->livros = realloc(atual->livros, atual->capacidade * sizeof(Livro *));
                if (!atual->livros) {
                    fprintf(stderr, "Falha ao redimensionar vetor de livros.\n");
                    free(no_normalizado);
                    return 0;
                }
            }

            // Cria e adiciona um novo livro
            Livro *novo_livro = (Livro *)malloc(sizeof(Livro));
            if (!novo_livro) {
                fprintf(stderr, "Falha de alocação de memória para o livro.\n");
                free(no_normalizado);
                return 0;
            }
            novo_livro->nome = strdup(l->nome);
            novo_livro->autor = strdup(l->autor); // Autor pode ser vazio ou definido posteriormente
            novo_livro->genero = strdup(l->genero); // O gênero ou categoria é baseado no nó
            novo_livro->qtd = 1; // Primeira cópia do livro
            novo_livro->status = false;

            atual->livros[atual->quantidade++] = novo_livro;

            free(no_normalizado);
            return 1;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Nó não encontrado, cria um novo nó na tabela hash
    NoHash *novo_no = (NoHash *)malloc(sizeof(NoHash));
    if (!novo_no) {
        fprintf(stderr, "Falha de alocação de memória para o nó.\n");
        free(no_normalizado);
        return 0;
    }
    novo_no->nome = strdup(no_normalizado);
    novo_no->capacidade = 10; // Capacidade inicial do vetor de livros
    novo_no->quantidade = 0;
    novo_no->livros = (Livro **)malloc(novo_no->capacidade * sizeof(Livro *));
    if (!novo_no->livros) {
        fprintf(stderr, "Falha de alocação de memória para o vetor de livros.\n");
        free(novo_no->nome);
        free(novo_no);
        free(no_normalizado);
        return 0;
    }

    // Cria e adiciona o primeiro livro ao novo nó
    Livro *novo_livro = (Livro *)malloc(sizeof(Livro));
    if (!novo_livro) {
        fprintf(stderr, "Falha de alocação de memória para o livro.\n");
        free(novo_no->livros);
        free(novo_no->nome);
        free(novo_no);
        free(no_normalizado);
        return 0;
    }
    novo_livro->nome = strdup(l->nome);
    novo_livro->autor = strdup(""); // Autor pode ser vazio ou definido posteriormente
    novo_livro->genero = strdup(nome_no); // O gênero ou categoria é baseado no nó
    novo_livro->qtd = 1; // Primeira cópia do livro
    novo_livro->status = false;

    novo_no->livros[novo_no->quantidade++] = novo_livro;

    // Insere o novo nó na tabela hash
    novo_no->proximo = t->tabela[id_no];
    t->tabela[id_no] = novo_no;

    free(no_normalizado);
    return 1;
}

// Função para editar os dados de um livro existente na tabela hash
void edita_livro(HashTable *t, const char *nome_errado, const char *nome_correto, const char *nome_no) {
    // Normaliza o nome do nó (gênero ou autor)
    char *no_normalizado = normalize_string(nome_no);
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar o nome do nó: %s\n", nome_no);
        return;
    }

    // Calcula o índice na tabela hash usando a função hash
    int id_no = funcaohash_string(no_normalizado);

    // Procura pelo nó na tabela hash
    NoHash *atual = t->tabela[id_no];
    while (atual != NULL) {
        if (strcmp(atual->nome, no_normalizado) == 0) {
            // Nó encontrado, verifica se o livro está presente
            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro = atual->livros[i];
                if (strcmp(livro->nome, nome_errado) == 0) {
                    // Livro encontrado: renomeia o título
                    free(livro->nome); // Libera a memória do nome antigo
                    livro->nome = strdup(nome_correto); // Aloca e copia o novo nome

                    printf("Livro renomeado com sucesso! Novo título: '%s'\n", livro->nome);

                    free(no_normalizado); // Libera memória alocada para o nome normalizado
                    return;
                }
            }

            printf("Livro '%s' não encontrado no nó '%s'.\n", nome_errado, nome_no);
            free(no_normalizado); // Libera memória alocada para o nome normalizado
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado); // Libera memória alocada para o nome normalizado
}



// Função para deletar um livro da tabela hash
void deletar(HashTable *t, const char *nome, const char *nome_no) {
    // Normaliza o nome do nó
    char *no_normalizado = normalize_string(nome_no);
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar nome do nó: %s\n", nome_no);
        return;
    }

    // Calcula o índice na tabela hash usando a função hash
    int id_no = funcaohash_string(no_normalizado);

    // Procura pelo nó na tabela hash
    NoHash *atual = t->tabela[id_no];
    NoHash *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome, no_normalizado) == 0) {
            // Nó encontrado, verifica se o livro está presente
            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro = atual->livros[i];
                if (strcmp(livro->nome, nome) == 0) {
                    // Livro encontrado
                    if (livro->qtd > 1) { // Usando 'qtd' como exemplo de quantidade
                        livro->qtd--; // Diminui a quantidade
                        printf("Quantidade do livro '%s' no nó '%s' diminuída.\n", nome, nome_no);
                    } else {
                        // Remove o livro completamente
                        printf("Livro '%s' removido do nó '%s'.\n", nome, nome_no);
                        free(livro->nome);
                        free(livro->autor);
                        free(livro->genero);
                        free(livro);

                        // Reorganiza o vetor de livros para preencher o espaço vazio
                        for (int j = i; j < atual->quantidade - 1; j++) {
                            atual->livros[j] = atual->livros[j + 1];
                        }
                        atual->quantidade--;

                        // Verifica se o nó ficou vazio e remove-o
                        if (atual->quantidade == 0) {
                            printf("Nó '%s' está vazio e será removido.\n", atual->nome);
                            free(atual->livros);
                            free(atual->nome);

                            if (anterior == NULL) {
                                t->tabela[id_no] = atual->proximo;
                            } else {
                                anterior->proximo = atual->proximo;
                            }
                            free(atual);
                        }
                    }

                    free(no_normalizado); // Libera memória alocada para o nome normalizado
                    return;
                }
            }

            printf("Livro '%s' não encontrado no nó '%s'.\n", nome, nome_no);
            free(no_normalizado); // Libera memória alocada para o nome normalizado
            return;
        }

        anterior = atual;
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado); // Libera memória alocada para o nome normalizado
}


// Função para buscar e listar todos os livros associados a um nó na tabela hash
void buscar_por_no(HashTable *t, const char *nome_no) {
    // Normaliza o nome do nó
    char *no_normalizado = normalize_string(nome_no);
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar nome do nó: %s\n", nome_no);
        return;
    }

    // Calcula o índice na tabela hash usando a função hash
    int id_no = funcaohash_string(no_normalizado);

    // Procura pelo nó na tabela hash
    NoHash *atual = t->tabela[id_no];
    while (atual != NULL) {
        if (strcmp(atual->nome, no_normalizado) == 0) {
            // Nó encontrado, imprime os livros associados
            printf("Livros associados a '%s':\n", atual->nome);

            if (atual->quantidade == 0) {
                printf("Nenhum livro encontrado neste nó.\n");
                free(no_normalizado);
                return;
            }

            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro = atual->livros[i];
                printf("- %s (Autor: %s, Gênero: %s)\n",
                    livro->nome,
                    livro->autor,
                    livro->genero);
            }

            free(no_normalizado);
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Caso o nó não seja encontrado
    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado);
}

// Função para liberar toda a memória associada à tabela hash
void hash_free(HashTable *t) {
    if (!t) return; // Verifica se a tabela é nula

    for (int i = 0; i < t->tamanho; i++) {
        NoHash *atual = t->tabela[i];

        // Percorre a lista encadeada em caso de colisões
        while (atual != NULL) {
            NoHash *temp = atual;

            // Libera todos os livros associados ao nó
            for (int j = 0; j < temp->quantidade; j++) {
                Livro *livro = temp->livros[j];
                if (livro) {
                    free(livro->nome);  // Libera o título do livro
                    free(livro->autor);   // Libera o autor do livro
                    free(livro->genero);  // Libera o gênero do livro
                    free(livro);          // Libera a estrutura do livro
                }
            }

            // Libera o vetor dinâmico de livros
            free(temp->livros);

            // Libera o nome do nó e o próprio nó
            free(temp->nome);
            atual = atual->proximo;
            free(temp);
        }
    }

    // Libera o vetor de ponteiros para os nós da tabela hash
    free(t->tabela);

    // Opcional: Zera os campos da estrutura para evitar acessos inválidos
    t->tabela = NULL;
    t->tamanho = 0;

    printf("Memória liberada com sucesso.\n");
}


