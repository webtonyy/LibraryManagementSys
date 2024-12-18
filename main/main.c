#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../include/catalogo.h"
#include "../include/hash.h"

// Função para exibir o menu de opções
void exibir_menu() {
    printf("\nDigite um número para selecionar uma ação:\n");
    printf("1 - Adicionar livro\n");
    printf("2 - Remover  livro\n");
    printf("3 - Editar livro\n");
    printf("4 - Listar livros\n");
    printf("5 - Buscar livro por nome\n");
    printf("6 - Buscar livro por autor\n");
    printf("7 - Buscar livro por gênero\n");
    printf("8 - Alugar livro\n");
    printf("9 - Devolver livro\n");
    printf("10 - Verificar status de um livro\n");
    printf("11 - Sair\n");
    printf("Escolha: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para suportar caracteres acentuados

    Catalogo *catalogo = catalogo_init(); // Inicializa o catálogo
    HashTable *generoh = hash_table_init(100); // Inicializa a tabela hash para gêneros
    HashTable *autorh = hash_table_init(100);  // Inicializa a tabela hash para autores

    char buffer[10];
    int opcao;

    do {
        exibir_menu();
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) { // Caso o usuário encerre o terminal
            opcao = 11; // Atribui a opção 11 -> saída do programa
        } else {
            opcao = atoi(buffer); // Converte a entrada para int

            switch (opcao) {
                case 1: { // Adicionar livro
                    // Declaração de variáveis locais para armazenar os dados do livro
                    char nome[100], autor_str[100], genero_str[50];

                    // Solicita o nome do livro
                    printf("Digite o nome do livro: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    fgets(autor_str, sizeof(autor_str), stdin);
                    autor_str[strcspn(autor_str, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    fgets(genero_str, sizeof(genero_str), stdin);
                    genero_str[strcspn(genero_str, "\n")] = '\0'; // Remove o caractere '\n'

                    // Inicializa um novo livro (aloca memória dinamicamente para os campos)
                    Livro *novo_livro = livro_init(nome, genero_str, autor_str);

                    if (novo_livro) {
                        // Adiciona o livro ao catálogo (árvore binária)
                        add_livro(catalogo, novo_livro);

                        // Insere o livro nas tabelas hash (gêneros e autores)
                        inserir_livro(generoh, novo_livro, genero_str); // Usa apenas strings alocadas dinamicamente no hash
                        inserir_livro(autorh, novo_livro, autor_str);

                        printf("Livro adicionado com sucesso!\n");
                    } else {
                        printf("Erro ao criar o livro.\n");
                    }

                    break;
                }


                case 2: { // Remover ou decrementar livro
                    char nome[100],autor[100],genero[50];
                    printf("Digite o nome do livro a ser removido ou decrementado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser removido: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    printf("Digite o gênero do livro a ser removido: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    remover_livro(catalogo, nome, autor);
                    deletar(autorh,nome,autor);
                    deletar(generoh,nome,genero)
                    break;
                }

                case 3: { // Editar livro
                    char nome[100], novo_nome[100], autor[100], genero[50];
                    printf("Digite o nome do livro a ser renomeado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser editado: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    printf("Digite o gênero do livro a ser editado: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    printf("Digite o novo nome do livro: ");
                    fgets(novo_nome, sizeof(novo_nome), stdin);
                    novo_nome[strcspn(novo_nome, "\n")] = '\0';



                    Livro* editado =  editar_livro(catalogo, nome, novo_nome, autor);
                    edita_livro(autorh,nome,novo_nome,autor);
                    edita_livro(generoh,nome,novo_nome,genero);
                    if (editado) {
                        printf("Livro editado com sucesso!\n");
                    } else {
                        printf("Erro ao editar o livro.\n");
                    }
                    break;
                }

                case 4: { // Listar livros
                    listar_livros(catalogo);
                    break;
                }

                case 5: { // Buscar livro por nome
                    char nome[100];
                    printf("Digite o nome do livro a ser buscado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    int encontrados = buscar_por_nome(catalogo, nome);
                    if (encontrados == 0) {
                        printf("Livro não encontrado.\n");
                    }
                    break;
                }

                case 6: { // Buscar por autor (usando tabela hash)
                    char autor[100];
                    printf("Digite o nome do autor a ser buscado: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    buscar_por_no(autorh, autor); // Busca na tabela hash de autores
                    break;
                }

                case 7: { // Buscar por gênero (usando tabela hash)
                    char genero[50];
                    printf("Digite o gênero a ser buscado: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    buscar_por_no(generoh, genero); // Busca na tabela hash de gêneros
                    break;
                }

                case 8: { // Alugar livro
                    char nome[100], autor[100];
                    printf("Digite o nome do livro a ser status: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser status: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    int resultado = emprestar_livro(catalogo, nome, autor);
                    if (resultado == 1) {
                        printf("Livro status com sucesso!\n");
                    } else if (resultado == 0) {
                        printf("Livro indisponível para aluguel.\n");
                    } else {
                        printf("Erro ao tentar alugar o livro.\n");
                    }
                    break;
                }

                case 9: { // Devolver livro
                    char nome[100], autor[100];
                    printf("Digite o nome do livro a ser devolvido: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';
                    printf("Digite o autor do livro a ser devolvido: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';
                    int resultado = devolve_livro(catalogo, nome, autor);
                    if (resultado == 1) {
                        printf("Livro devolvido com sucesso!\n");
                    } else if (resultado == 2) {
                        printf("Livro adicionado como uma nova doação!\n");
                    } else {
                        printf("Erro ao tentar devolver o livro.\n");
                    }
                    break;
                }

                case 10: { // Verificar status de um livro
                    char nome[100], autor[100];
                    printf("Digite o nome do livro a ser consultado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser consultado: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    verificar_status(catalogo, nome, autor);
                    break;
                }

                case 11: { // Sair
                    hash_free(generoh);            // Libera memória da tabela hash de gêneros
                    hash_free(autorh);             // Libera memória da tabela hash de autores
                    free_catalogo(catalogo);      // Libera memória do catálogo
                    printf("Encerrando o programa...\n");
                    break;
                }

                default:
                    printf("Opção inválida! Tente novamente.\n");
            }
        }
    } while (opcao != 11);

    return 0;
}
