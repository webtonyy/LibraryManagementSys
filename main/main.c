#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/catalogo.h" // Inclua o cabeçalho apropriado

// Função para exibir o menu
void exibir_menu() {
    printf("\nDigite um número para selecionar uma ação:\n");
    printf("1 - Adicionar livro\n");
    printf("2 - Remover livro\n");
    printf("3 - Editar livro\n");
    printf("4 - Listar livros\n");
    printf("5 - Buscar livro por nome\n");
    printf("6 - Sair\n");
    printf("Escolha: ");
}

int main() {
    Catalogo *catalogo = catalogo_init(); // Inicializa o catálogo
    int opcao;

    do {
        exibir_menu();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1: { // Adicionar livro
                char nome[100], autor[100], genero[50];

                printf("Digite o nome do livro: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                printf("Digite o autor do livro: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';

                printf("Digite o gênero do livro: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';

                Livro *novo_livro = livro_init(nome, genero, autor);

                if (novo_livro) { // Only add the book if it passed validation
                    add_livro(catalogo, novo_livro);
                    printf("Livro adicionado com sucesso!\n");
                } else {
                    printf("Erro ao adicionar livro. Certifique-se de que os campos contêm apenas caracteres ASCII.\n");
                }

                break;
            }

            case 2: { // Remover livro
                char nome[100], autor[100];
                printf("Digite o nome do livro a ser removido: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                Livro *rem = remover_livro(catalogo,nome);
                rem ? printf("Livro removido com sucesso!"): printf("Falha ao remover livro.");

                break;
            }

            case 3: { // Editar livro
                char nome[100],novo_nome[100], novo_genero[50], novo_autor[100];

                printf("Digite o nome do livro a ser editado: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite o novo nome do livro: ");
                fgets(novo_nome, sizeof(novo_nome), stdin);
                novo_nome[strcspn(novo_nome, "\n")] = '\0';

                printf("Digite o novo gênero do livro: ");
                fgets(novo_genero, sizeof(novo_genero), stdin);
                novo_genero[strcspn(novo_genero, "\n")] = '\0';

                printf("Digite o novo autor do livro: ");
                fgets(novo_autor, sizeof(novo_autor), stdin);
                novo_autor[strcspn(novo_autor, "\n")] = '\0';

                Livro temp_livro = {
                    .nome = strdup(nome),
                    .genero = strdup(novo_genero),
                    .autor = strdup(novo_autor),
                    .esq = NULL,
                    .dir = NULL,
                    .qtd = 1
                };

                Livro *editado = editar_livro(catalogo, nome, &temp_livro);

                if (editado) {
                    printf("Livro editado com sucesso!\n");
                } else {
                    printf("Edição falhou. Livro não encontrado.\n");
                }

                free(temp_livro.nome); // Libera strings temporárias
                free(temp_livro.genero);
                free(temp_livro.autor);

                break;
}           

            case 4: { // Listar livros
                if (catalogo->raiz) {
                    listar_livros(catalogo);
                } else {
                    printf("Nenhum livro no catálogo.\n");
                }
                break;
            }

            case 5: { // Buscar livro por nome
                if (catalogo->raiz) {
                    char nome[100];
                    printf("Digite o nome do livro a ser buscado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    if (!verifica_livro(catalogo, nome)) {
                        printf("Livro não encontrado.\n");
                    }
                } else {
                    printf("Nenhum livro no catálogo.\n");
                }
                break;
            }

            case 6: { // Sair
                printf("Encerrando o programa...\n");
                break;
            }

            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);

    free_catalogo(catalogo); // Libera a memória alocada para o catálogo
    return 0;
}
