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
    if (!nome) return -1; // Retorna -1 em caso de nome inválido

    // Normaliza a string
    char *normalized = strdup(normalize_string(nome));
    if (!normalized) return -1; // Retorna -1 se a normalização falhar

    int tam = strlen(normalized);
    unsigned int ch = 0;

    for (int i = 0; i < tam; i++) {
        ch += normalized[i] * (i + 1);
    }

    free(normalized); // Libera a memória alocada para a string normalizada
    return ch % TABLE_SIZE;
}

int inserir_livro(HashTable *t, Livro *l, const char *nome_no) {
    if (!t || !l || !nome_no) {
        fprintf(stderr, "Erro: Parâmetros inválidos para inserir_livro.\n");
        return 0;
    }

    // Normaliza o nome do nó (gênero ou autor)
    char *no_normalizado = strdup(normalize_string(nome_no));
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar nome do nó: %s\n", nome_no);
        return 0;
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
                if (strcmp(livro->nome_norma, l->nome_norma) == 0 && strcmp(livro->autor_norma, l->autor_norma) == 0) {
                    // Livro encontrado, incrementa a quantidade
                    livro->qtd+=l->qtd;
                    livro->status = 0;
                    free(no_normalizado); // Libera a memória da string normalizada
                    return 1;
                }
            }

            // Livro não encontrado no nó, adiciona à lista de livros
            if (atual->quantidade == atual->capacidade) {
                // Redimensiona o vetor de livros se necessário
                atual->capacidade *= 2;
                atual->livros = realloc(atual->livros, atual->capacidade * sizeof(Livro *));
                if (!atual->livros) {
                    fprintf(stderr, "Erro ao redimensionar vetor de livros.\n");
                    free(no_normalizado);
                    return 0;
                }
            }

            // Cria e adiciona um novo livro ao nó
            Livro *novo_livro = malloc(sizeof(Livro));
            novo_livro->nome = strdup(l->nome);
            novo_livro->nome_norma = strdup(l->nome_norma);
            novo_livro->autor = strdup(l->autor);
            novo_livro->autor_norma = strdup(l->autor_norma);
            novo_livro->genero = strdup(l->genero);
            novo_livro->genero_norma = strdup(l->genero_norma);
            novo_livro->qtd = 1;
            novo_livro->status = false;

            atual->livros[atual->quantidade++] = novo_livro;

            free_livro(novo_livro);

            free(no_normalizado); // Libera a memória da string normalizada
            return 1;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Nó correspondente não encontrado, cria um novo nó e adiciona o livro a ele
    NoHash *novo_no = malloc(sizeof(NoHash));
    if (!novo_no) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        free(no_normalizado);
        return 0;
    }

    novo_no->nome = strdup(no_normalizado); // Armazena o nome original do nó
    novo_no->quantidade = 1;
    novo_no->capacidade = 100; // Capacidade inicial do vetor de livros
    novo_no->livros = malloc(novo_no->capacidade * sizeof(Livro *));
    if (!novo_no->livros) {
        fprintf(stderr, "Erro ao alocar memória para o vetor de livros.\n");
        free(novo_no->nome);
        free(novo_no);
        free(no_normalizado);
        return 0;
    }

    // Cria e adiciona o primeiro livro ao novo nó
    Livro *novo_livro = livro_init(l->nome,l->genero, l->autor);

    novo_no->livros[0] = novo_livro;

    // Insere o novo nó na tabela hash
    novo_no->proximo = t->tabela[id_no];
    t->tabela[id_no] = novo_no;


    free(no_normalizado); // Libera a memória da string normalizada
    return 1;
}

void edita_livro(HashTable *t, const char *nome_errado, const char *nome_correto, const char *nome_no) {
    if (!t || !nome_errado || !nome_correto || !nome_no) {
        fprintf(stderr, "Erro: Parâmetros inválidos para editar o livro.\n");
        return;
    }

    // Normaliza o nome do nó (gênero ou autor)
    char *no_normalizado = strdup(normalize_string(nome_no));
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
                
                // Normaliza o nome do livro errado para comparação
                char *nome_errado_norma = strdup(normalize_string(nome_errado));
                if (!nome_errado_norma) {
                    fprintf(stderr, "Falha ao normalizar o nome do livro: %s\n", nome_errado);
                    free(no_normalizado);
                    return;
                }

                if (strcmp(livro->nome_norma, nome_errado_norma) == 0) {
                    // Livro encontrado: renomeia o título
                    free(livro->nome); // Libera a memória do nome antigo
                    free(livro->nome_norma); // Libera a memória do nome normalizado antigo

                    livro->nome = strdup(nome_correto); // Aloca e copia o novo nome
                    livro->nome_norma = strdup(normalize_string(nome_correto)); // Atualiza o nome normalizado

                    free(nome_errado_norma); // Libera a memória da string normalizada
                    free(no_normalizado);   // Libera a memória da string normalizada do nó
                    return;
                }

                free(nome_errado_norma); // Libera a memória da string normalizada
            }

            free(no_normalizado); // Libera a memória da string normalizada do nó
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    free(no_normalizado); // Libera a memória da string normalizada do nó
}

void deletar(HashTable *t, const char *nome, const char *nome_no) {
    if (!t || !nome || !nome_no) {
        fprintf(stderr, "Erro: Parâmetros inválidos para deletar.\n");
        return;
    }

    // Normaliza o nome do nó
    char *no_normalizado = strdup(normalize_string(nome_no));
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar o nome do nó: %s\n", nome_no);
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

                // Normaliza o nome do livro para comparação
                char *nome_normalizado = strdup(normalize_string(nome));
                if (!nome_normalizado) {
                    fprintf(stderr, "Falha ao normalizar o nome do livro: %s\n", nome);
                    free(no_normalizado);
                    return;
                }

                if (strcmp(livro->nome_norma, nome_normalizado) == 0) {
                    // Livro encontrado
                    if (livro->qtd > 1) { // Usando 'qtd' como exemplo de quantidade
                        livro->qtd--; // Diminui a quantidade
                        free(nome_normalizado); // Libera a memória da string normalizada
                        free(no_normalizado);   // Libera a memória da string normalizada do nó
                        return;
                    } else {
                        // Remove o livro completamente
                        free(livro->nome);
                        free(livro->nome_norma);
                        free(livro->autor);
                        free(livro->autor_norma);
                        free(livro->genero);
                        free(livro->genero_norma);
                        free(livro);

                        // Reorganiza o vetor de livros para preencher o espaço vazio
                        for (int j = i; j < atual->quantidade - 1; j++) {
                            atual->livros[j] = atual->livros[j + 1];
                        }
                        atual->quantidade--;

                        // Verifica se o nó ficou vazio e remove-o
                        if (atual->quantidade == 0) {
                            free(atual->livros);
                            free(atual->nome);

                            if (anterior == NULL) {
                                t->tabela[id_no] = atual->proximo;
                            } else {
                                anterior->proximo = atual->proximo;
                            }
                            free(atual);
                        }

                        free(nome_normalizado); // Libera a memória da string normalizada
                        free(no_normalizado);   // Libera a memória da string normalizada do nó
                        return;
                    }
                }

                free(nome_normalizado); // Libera a memória da string normalizada
            }
            free(no_normalizado); // Libera a memória da string normalizada do nó
            return;
        }

        anterior = atual;
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado); // Libera a memória da string normalizada do nó
}


void decrementa_qtd(HashTable *ht, Livro *l, char *nome_no) {
    if (!ht || !l || !nome_no) {
        fprintf(stderr, "Erro: Parâmetros inválidos.\n");
        return;
    }

    // Normaliza o nome do nó
    char *no_normalizado = strdup(normalize_string(nome_no));
    if (!no_normalizado) {
        fprintf(stderr, "Falha ao normalizar o nome do nó: %s\n", nome_no);
        return;
    }

    // Calcula o índice na tabela hash usando a função hash
    int id_no = funcaohash_string(no_normalizado);

    // Procura pelo nó na tabela hash
    NoHash *atual = ht->tabela[id_no];

    while (atual != NULL) {
        if (strcmp(atual->nome, no_normalizado) == 0) {
            // Nó encontrado, verifica se o livro está presente
            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro = atual->livros[i];

                // Verifica se o livro corresponde ao esperado
                if (strcmp(livro->nome_norma, l->nome_norma) == 0) {
                    // Livro encontrado
                    if (livro->qtd > 0) 
                        livro->qtd--; // Diminui a quantidade

                    if(livro->qtd == 0)
                        livro->status = 1;
                    else
                        livro->status = 0;

                    free(no_normalizado); // Libera a memória da string normalizada do nó
                    return;
                }
            }

            printf("Livro '%s' não encontrado no nó '%s'.\n", l->nome, nome_no);
            free(no_normalizado); // Libera a memória da string normalizada do nó
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado); // Libera a memória da string normalizada do nó
}


void buscar_por_no(HashTable *t, const char *nome_no) {
    if (!t || !nome_no) {
        fprintf(stderr, "Erro: Parâmetros inválidos para buscar_por_no.\n");
        return;
    }

    // Normaliza o nome do nó
    char *no_normalizado = strdup(normalize_string(nome_no));
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
            // Nó encontrado, imprime os livros associados
            printf("Livros associados ao nó '%s':\n", atual->nome);

            if (atual->quantidade > 0) {
                for (int i = 0; i < atual->quantidade; i++) {
                    Livro *livro = atual->livros[i];
                    printf("- %s (Autor: %s, Gênero: %s, Quantidade: %d, Status: %s)\n",
                        livro->nome,
                        livro->autor,
                        livro->genero,
                        livro->qtd,
                        livro->status ? "Indisponível" : "Disponível");
                }
            } else {
                printf("Nenhum livro encontrado neste nó.\n");
            }

            free(no_normalizado); // Libera a memória da string normalizada
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Caso o nó não seja encontrado
    printf("Nó '%s' não encontrado.\n", nome_no);
    free(no_normalizado); // Libera a memória da string normalizada
}





void imprimir_tabela_hash(HashTable *hashTable) {
    if (!hashTable) {
        fprintf(stderr, "Erro: Tabela hash inválida.\n");
        return;
    }

    printf("\n--- Tabela Hash ---\n");
    for (int i = 0; i < hashTable->tamanho; i++) {
        printf("Bucket %d:\n", i);
        NoHash *noAtual = hashTable->tabela[i];
        
        while (noAtual != NULL) {
            printf("  Gênero/Autor: %s\n", noAtual->nome); // Exibe o nome original do nó
            printf("  Livros:\n");
            for (int j = 0; j < noAtual->quantidade; j++) {
                Livro *livro = noAtual->livros[j];
                printf("    - Título: %s, Autor: %s, Gênero: %s, Quantidade: %d, Status: %s\n", 
                    livro->nome,
                    livro->autor,
                    livro->genero,
                    livro->qtd,
                    livro->status ? "Indisponível" : "Disponível"); // Exibe o status do livro
            }
            noAtual = noAtual->proximo;
        }
    }
    printf("--- Fim da Tabela Hash ---\n");
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
                    free(livro->nome);       // Libera o título do livro
                    free(livro->nome_norma); // Libera o título normalizado
                    free(livro->autor);      // Libera o autor do livro
                    free(livro->autor_norma);// Libera o autor normalizado
                    free(livro->genero);     // Libera o gênero do livro
                    free(livro->genero_norma);// Libera o gênero normalizado
                    free(livro);             // Libera a estrutura do livro
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

    // Zera os campos da estrutura para evitar acessos inválidos
    t->tabela = NULL;
    t->tamanho = 0;

    printf("Memória liberada com sucesso.\n");
}

