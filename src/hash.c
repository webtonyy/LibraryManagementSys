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
int inserir_livro(HashTable *t, Livro *l, const char *nome_no) {
    // Normaliza o nome do nó (gênero ou autor)
    char *no_normalizado = normalize_string(nome_no);
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
                if (strcmp(livro->nome, l->nome) == 0 && strcmp(livro->autor, l->autor) == 0) {
                    // Livro encontrado, incrementa a quantidade
                    livro->qtd++;
                    printf("Quantidade do livro '%s' do autor '%s' incrementada.\n", l->nome, l->autor);
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
                    return 0;
                }
            }

            // Cria e adiciona um novo livro ao nó
            Livro *novo_livro = malloc(sizeof(Livro));
            novo_livro->nome = strdup(l->nome);
            novo_livro->autor = strdup(l->autor);
            novo_livro->genero = strdup(l->genero);
            novo_livro->qtd = 1;
            novo_livro->status = false;

            atual->livros[atual->quantidade++] = novo_livro;
            printf("Livro '%s' do autor '%s' adicionado ao nó existente '%s'.\n", l->nome, l->autor, no_normalizado);

            return 1;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Nó correspondente não encontrado, cria um novo nó e adiciona o livro a ele
    NoHash *novo_no = malloc(sizeof(NoHash));
    if (!novo_no) {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
        return 0;
    }

    novo_no->nome = strdup(no_normalizado);
    novo_no->quantidade = 1;
    novo_no->capacidade = 10; // Capacidade inicial do vetor de livros
    novo_no->livros = malloc(novo_no->capacidade * sizeof(Livro *));
    if (!novo_no->livros) {
        fprintf(stderr, "Erro ao alocar memória para o vetor de livros.\n");
        free(novo_no->nome);
        free(novo_no);
        return 0;
    }

    // Cria e adiciona o primeiro livro ao novo nó
    Livro *novo_livro = malloc(sizeof(Livro));
    novo_livro->nome = strdup(l->nome);
    novo_livro->autor = strdup(l->autor);
    novo_livro->genero = strdup(l->genero);
    novo_livro->qtd = 1;
    novo_livro->status = false;

    novo_no->livros[0] = novo_livro;

    // Insere o novo nó na tabela hash
    novo_no->proximo = t->tabela[id_no];
    t->tabela[id_no] = novo_no;

    printf("Nó '%s' criado e livro '%s' do autor '%s' adicionado.\n", no_normalizado, l->nome, l->autor);

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

                    return;
                }
            }

            printf("Livro '%s' não encontrado no nó '%s'.\n", nome_errado, nome_no);
            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
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

                    return;
                }
            }

            printf("Livro '%s' não encontrado no nó '%s'.\n", nome, nome_no);
            return;
        }

        anterior = atual;
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    printf("Nó '%s' não encontrado.\n", nome_no);
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
                return;
            }

            for (int i = 0; i < atual->quantidade; i++) {
                Livro *livro = atual->livros[i];
                printf("- %s (Autor: %s, Gênero: %s)\n",
                    livro->nome,
                    livro->autor,
                    livro->genero);
            }

            return;
        }
        atual = atual->proximo; // Avança para o próximo nó em caso de colisão
    }

    // Caso o nó não seja encontrado
    printf("Nó '%s' não encontrado.\n", nome_no);
}

void imprimir_tabela_hash(HashTable *hashTable) {
    printf("\n--- Tabela Hash ---\n");
    for (int i = 0; i < hashTable->tamanho; i++) {
        printf("Bucket %d:\n", i);
        NoHash *noAtual = hashTable->tabela[i];
        
        while (noAtual != NULL) {
            printf("  Gênero/Autor: %s\n", noAtual->nome);
            printf("  Livros:\n");
            for (int j = 0; j < noAtual->quantidade; j++) {
                Livro *livro = noAtual->livros[j];
                printf("    - Título: %s, Autor: %s, Gênero: %s\n", 
                    livro->nome, livro->autor, livro->genero);
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


void salvar_hash(HashTable *t, const char *nome_arquivo) {
    if (!t || !nome_arquivo) return;

    FILE *arquivo = fopen(nome_arquivo, "wb");
    if (!arquivo) {
        perror("Erro ao salvar o arquivo de configurações!");
        return;
    }

    // Salva o tamanho da tabela
    fwrite(&t->tamanho, sizeof(int), 1, arquivo);

    // Salva cada bucket da tabela hash
    for (int i = 0; i < t->tamanho; i++) {
        NoHash *atual = t->tabela[i];
        while (atual) {
            // Salva os dados do nó
            int nome_len = strlen(atual->nome) + 1;
            fwrite(&nome_len, sizeof(int), 1, arquivo);
            fwrite(atual->nome, sizeof(char), nome_len, arquivo);
            fwrite(&atual->quantidade, sizeof(int), 1, arquivo);

            // Salva os livros associados ao nó
            for (int j = 0; j < atual->quantidade; j++) {
                Livro *livro = atual->livros[j];
                int nome_livro_len = strlen(livro->nome) + 1;
                int autor_len = strlen(livro->autor) + 1;
                int genero_len = strlen(livro->genero) + 1;

                fwrite(&nome_livro_len, sizeof(int), 1, arquivo);
                fwrite(livro->nome, sizeof(char), nome_livro_len, arquivo);
                fwrite(&autor_len, sizeof(int), 1, arquivo);
                fwrite(livro->autor, sizeof(char), autor_len, arquivo);
                fwrite(&genero_len, sizeof(int), 1, arquivo);
                fwrite(livro->genero, sizeof(char), genero_len, arquivo);
                fwrite(&livro->qtd, sizeof(int), 1, arquivo);
                fwrite(&livro->status, sizeof(bool), 1, arquivo);
            }

            atual = atual->proximo;
        }
    }

    fclose(arquivo);
}


HashTable* carregar_hash(const char *nome_arquivo) {
    if (!nome_arquivo) return NULL;

    FILE *arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo) {
        // Caso o arquivo não exista, cria um novo
        arquivo = fopen(nome_arquivo, "wb");
        if (!arquivo) {
            perror("Erro ao criar o arquivo");
            return NULL;
        }
        fclose(arquivo);
        return hash_table_init(100); // Inicializa uma nova tabela hash com tamanho padrão
    }

    int tamanho;
    fread(&tamanho, sizeof(int), 1, arquivo);

    HashTable *t = hash_table_init(tamanho);

    for (int i = 0; i < tamanho; i++) {
        while (true) {
            int nome_len;
            if (fread(&nome_len, sizeof(int), 1, arquivo) != 1) break;

            char nome[nome_len];
            fread(nome, sizeof(char), nome_len, arquivo);

            int quantidade;
            fread(&quantidade, sizeof(int), 1, arquivo);

            NoHash *novo_no = malloc(sizeof(NoHash));
            novo_no->nome = strdup(nome);
            novo_no->quantidade = quantidade;
            novo_no->capacidade = quantidade;
            novo_no->livros = malloc(quantidade * sizeof(Livro*));
            novo_no->proximo = t->tabela[i];
            t->tabela[i] = novo_no;

            for (int j = 0; j < quantidade; j++) {
                Livro *livro = malloc(sizeof(Livro));

                int nome_livro_len;
                fread(&nome_livro_len, sizeof(int), 1, arquivo);
                livro->nome = malloc(nome_livro_len);
                fread(livro->nome, sizeof(char), nome_livro_len, arquivo);

                int autor_len;
                fread(&autor_len, sizeof(int), 1, arquivo);
                livro->autor = malloc(autor_len);
                fread(livro->autor, sizeof(char), autor_len, arquivo);

                int genero_len;
                fread(&genero_len, sizeof(int), 1, arquivo);
                livro->genero = malloc(genero_len);
                fread(livro->genero, sizeof(char), genero_len, arquivo);

                fread(&livro->qtd, sizeof(int), 1, arquivo);
                fread(&livro->status, sizeof(bool), 1, arquivo);

                novo_no->livros[j] = livro;
            }
        }
    }

    fclose(arquivo);
    return t;
}


