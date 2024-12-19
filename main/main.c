#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../include/catalogo.h"
#include "../include/hash.h"
#include "../include/aux.h"


// Definindo função para lidar com o sleep() em diferentes SOs:

#ifdef _WIN32
    #include <windows.h> // Para Sleep() no Windows
#else
    #include <unistd.h>  // Para sleep() no Linux/Unix
#endif

// Função dormir:
void dormir(int seg) {
    #ifdef _WIN32
        Sleep(seg * 1000); // Windows: converte segundos para milissegundos
    #else
        sleep(seg);        // Linux/Unix: usa diretamente segundos
    #endif
}



// Função para exibir o menu de opções
void exibir_menu() {
    printf("\n=======================================================");
    printf("\nDigite um número para selecionar uma ação:\n");
    printf("1 - Adicionar livro\n");
    printf("2 - Remover livro\n");
    printf("3 - Editar livro\n");
    printf("4 - Listar livros\n");
    printf("5 - Buscar livro por nome\n");
    printf("6 - Buscar livro por autor\n");
    printf("7 - Buscar livro por gênero\n");
    printf("8 - Alugar livro\n");
    printf("9 - Devolver livro\n");
    printf("10 - Verificar status de um livro\n");
    printf("11 - Sair\n");
    printf("=======================================================\n");
    printf("Escolha: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para suportar caracteres acentuados

    // Carregando das estruturas de um arquivo binário externo:
    printf("\nCarregando as configurações iniciais. Por favor, aguarde!\n");
    dormir(2);
    Catalogo *catalogo = carregar_catalogo("./savedata/data_c.bin"); 
    HashTable *generoh = carregar_hash("./savedata/data_g.bin");
    HashTable *autorh = carregar_hash("./savedata/data_a.bin");
    printf("Configurações inicializadas com sucesso!\n");

    printf("\nCarregando o menu de opções...\n");
    dormir(2);

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
                    // Solicita o nome do livro
                    printf("Digite o nome do livro: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    char *autor_str = ler_string_dinamica();

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    char *genero_str = ler_string_dinamica();

                    // Inicializa um novo livro (aloca memória dinamicamente para os campos)
                    Livro *novo_livro = livro_init(nome, genero_str, autor_str);

                    dormir(1);

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

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor_str);
                    free(genero_str);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();

                    break;
                
                }


                case 2: { // Remover ou decrementar livro
                    // Solicita o nome do livro a ser removido ou decrementado
                    printf("Digite o nome do livro a ser removido ou decrementado: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro
                    printf("Digite o autor do livro a ser removido: ");
                    char *autor = ler_string_dinamica();

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro a ser removido: ");
                    char *genero = ler_string_dinamica();

                    dormir(1);

                    // Realiza as operações de remoção
                    remover_livro(catalogo, nome, autor);
                    deletar(autorh, nome, autor);
                    deletar(generoh, nome, genero);

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor);
                    free(genero);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 3: { // Editar livro
                    // Solicita o nome do livro a ser renomeado
                    printf("Digite o nome do livro a ser renomeado: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro a ser editado
                    printf("Digite o autor do livro a ser editado: ");
                    char *autor = ler_string_dinamica();

                    // Solicita o gênero do livro a ser editado
                    printf("Digite o gênero do livro a ser editado: ");
                    char *genero = ler_string_dinamica();

                    // Solicita o novo nome do livro
                    printf("Digite o novo nome do livro: ");
                    char *novo_nome = ler_string_dinamica();

                    Livro* editado = editar_livro(catalogo, nome, novo_nome, autor);
                    edita_livro(autorh, nome, novo_nome, autor);
                    edita_livro(generoh, nome, novo_nome, genero);

                    dormir(1);
                    
                    if (editado) {
                        printf("Livro editado com sucesso!\n");
                    } else {
                        printf("Erro ao editar o livro.\n");
                    }

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor);
                    free(genero);
                    free(novo_nome);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 4: { // Listar livros

                    dormir(1);
                    listar_livros(catalogo);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 5: { // Buscar livro por nome
                    // Solicita o nome do livro a ser buscado
                    printf("Digite o nome do livro a ser buscado: ");
                    char *nome = ler_string_dinamica();

                    dormir(1);
                    int encontrados = buscar_por_nome(catalogo, nome);
                    if (encontrados == 0) {
                        printf("Livro não encontrado.\n");
                    }

                    // Libera a string alocada dinamicamente
                    free(nome);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 6: { // Buscar por autor (usando tabela hash)
                    // Solicita o autor do livro a ser buscado
                    printf("Digite o autor que quer buscar: ");
                    char *autor = ler_string_dinamica();

                    dormir(1);
                    buscar_por_no(autorh, autor); // Busca na tabela hash de autores

                    // Libera a string alocada dinamicamente
                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 7: { // Buscar por gênero (usando tabela hash)
                    // Solicita o gênero a ser buscado
                    printf("Digite o gênero a ser buscado: ");
                    char *genero = ler_string_dinamica();

                    dormir(1);
                    buscar_por_no(generoh, genero); // Busca na tabela hash de gêneros

                    // Libera a string alocada dinamicamente
                    free(genero);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 8: { // Alugar livro
                    // Solicita o nome do livro a ser alugado
                    printf("Digite o nome do livro a ser alugado: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro
                    printf("Digite o autor do livro a ser alugado: ");
                    char *autor = ler_string_dinamica();

                    int resultado = emprestar_livro(catalogo, nome, autor);

                    dormir(1);
                    if (resultado == 1) {
                        printf("Livro alugado com sucesso!\n");
                    } else if (resultado == 0) {
                        printf("Livro indisponível para aluguel.\n");
                    } else {
                        printf("Erro ao tentar alugar o livro.\n");
                    }

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 9: { // Devolver livro
                    // Solicita o nome do livro a ser devolvido
                    printf("Digite o nome do livro a ser devolvido: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro a ser devolvido
                    printf("Digite o autor do livro a ser devolvido: ");
                    char *autor = ler_string_dinamica();

                    int resultado = devolve_livro(catalogo, nome, autor);

                    dormir(1);
                    if (resultado == 1) {
                        printf("Livro devolvido com sucesso!\n");
                    } else if (resultado == 2) {
                        printf("Livro adicionado como uma nova doação!\n");
                    } else {
                        printf("Erro ao tentar devolver o livro.\n");
                    }

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }


                case 10: { // Verificar status de um livro
                    // Solicita o nome do livro a ser consultado
                    printf("Digite o nome do livro a ser consultado: ");
                    char *nome = ler_string_dinamica();

                    // Solicita o autor do livro a ser consultado
                    printf("Digite o autor do livro a ser consultado: ");
                    char *autor = ler_string_dinamica();

                    verificar_status(catalogo, nome, autor);

                    // Libera as strings alocadas dinamicamente
                    free(nome);
                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 11: { // Sair
                    printf("Encerrando o programa...\n");
                    dormir(1);

                    // Salvando as structs nos arquivos binários:

                    printf("\nSalvando os arquivos de configurações...\n");
                    dormir(1);

                    salvar_catalogo(catalogo,"./savedata/data_c.bin");
                    salvar_hash(generoh, "./savedata/data_g.bin");
                    salvar_hash(autorh, "./savedata/data_a.bin");
                    printf("Arquivos salvos com sucesso!\n");

                    // Limpando as structs temporárias:

                    printf("\nLimpando dados temporários...\n");

                    hash_free(generoh);         
                    hash_free(autorh);             
                    free_catalogo(catalogo);      

                    printf("\nPrograma encerrado!\n \nPressione [Enter] para fechar a execução...\n");
                    getchar();
                    exit(1);
                }

                default:
                    printf("Opção inválida! Tente novamente.\n");
                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
            }
        }
    } while (opcao != 11);

    return 0;
}
