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

// Definindo a função para lidar com a exclusão dos arquivos binários do catálogo:
// Retorna 1 em caso de sucesso da exclusão, 0 em caso de falha
int excluir_arquivo(const char* arquivo){

    FILE *arq = fopen(arquivo, "rb");

    if(arq){
        fclose(arq);

        if(remove(arquivo) == 0){

            return 1;
        }
        else{
            return 0;
        }
    }
    return 0;
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
    printf("11 - Excluir Catálogo\n");
    printf("12 - Sair\n");
    printf("=======================================================\n");
    printf("Escolha: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para suportar caracteres acentuados

    // Carregando das estruturas de um arquivo binário externo:
    printf("\nCarregando as configurações iniciais. Por favor, aguarde!\n");
    dormir(2);
    Catalogo *catalogo = carregar_catalogo("./savedata/data_c.bin"); 
    HashTable *autorh = hash_table_init(50);
    HashTable *generoh = hash_table_init(50);
    printf("Configurações inicializadas com sucesso!\n");

    printf("\nCarregando o menu de opções...\n");
    dormir(2);

    char buffer[10];
    int opcao;
    percorrer_arvore_e_inserir_na_hash_autor(catalogo->raiz,autorh);
    percorrer_arvore_e_inserir_na_hash_genero(catalogo->raiz,generoh);

    do {
        exibir_menu();
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) { // Caso o usuário encerre o terminal
            opcao = 12; // Atribui a opção 11 -> saída do programa
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

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();

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

                    dormir(1);

                    remover_livro(catalogo, nome, autor);
                    deletar(autorh,nome,autor);
                    deletar(generoh,nome,genero);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
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

                    dormir(1);
                    if (editado) {
                        printf("Livro editado com sucesso!\n");
                    } else {
                        printf("Erro ao editar o livro.\n");
                    }

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
                    char nome[100];
                    printf("Digite o nome do livro a ser buscado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    dormir(1);
                    int encontrados = buscar_por_nome(catalogo, nome);
                    if (encontrados == 0) {
                        printf("Livro não encontrado.\n");
                    }

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 6: { // Buscar por autor (usando tabela hash)
                    char autor[100];
                    printf("Digite o nome do autor a ser buscado: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    dormir(1);
                    buscar_por_no(autorh, autor); // Busca na tabela hash de autores

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 7: { // Buscar por gênero (usando tabela hash)
                    char genero[50];
                    printf("Digite o gênero a ser buscado: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    dormir(1);
                    buscar_por_no(generoh, genero); // Busca na tabela hash de gêneros

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 8: { // Alugar livro
                    char nome[100], autor[100], genero[50];
                    printf("Digite o nome do livro a ser alugado: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser alugado: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    printf("Digite o gênero do livro a ser alugado: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    Livro *novo_livro = livro_init(nome,genero,autor);

                    int resultado = emprestar_livro(catalogo, novo_livro);

                    dormir(1);
                    if (resultado == 1) {
                        decrementa_qtd(autorh,novo_livro,autor);
                        decrementa_qtd(generoh,novo_livro,genero);
                    } else if (resultado == 0) {
                        printf("Livro indisponível para aluguel.\n");
                    } else {
                        printf("Erro ao tentar alugar o livro.\n");
                    }


                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 9: { // Devolver livro
                    char nome[100], autor[100], genero[50];
                    printf("Digite o nome do livro a ser entregue: ");
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Digite o autor do livro a ser entregue: ");
                    fgets(autor, sizeof(autor), stdin);
                    autor[strcspn(autor, "\n")] = '\0';

                    printf("Digite o gênero do livro a ser entregue: ");
                    fgets(genero, sizeof(genero), stdin);
                    genero[strcspn(genero, "\n")] = '\0';

                    Livro *novo_livro = livro_init(nome,genero,autor);
                    int resultado = devolve_livro(catalogo, novo_livro);

                    dormir(1);
                    if (resultado == 1) {
                        inserir_livro(autorh,novo_livro,autor);
                        inserir_livro(generoh,novo_livro,genero);
                        printf("Livro devolvido com sucesso!\n");
                    } else if (resultado == 2) {
                        inserir_livro(autorh,novo_livro,autor);
                        inserir_livro(generoh,novo_livro,genero);
                        printf("Livro adicionado como uma nova doação!\n");
                    } else {
                        printf("Erro ao tentar devolver o livro.\n");
                    }

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
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

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 11:{ // Excluir o catálogo

                    printf("\nExcluindo os arquivos de salvamento do catálogo...\n");

                    dormir(2);

                    int stt;

                    if(excluir_arquivo("./savedata/data_c.bin")) stt = 1;
                    else stt = 0;

                    if(stt == 1)
                        printf("Arquivos excluídos com sucesso!\n");
                    else
                        printf("Falha ao excluir os arquivos.\n");

                    printf("\nPrograma encerrado!\n \nPressione [Enter] para fechar a execução...\n");
                    getchar();
                    exit(0);

                }
                case 12: { // Sair
                    printf("Encerrando o programa...\n");
                    dormir(1);

                    // Salvando as structs nos arquivos binários:

                    printf("\nSalvando os arquivos de configurações...\n");
                    dormir(1);

                    salvar_catalogo(catalogo,"./savedata/data_c.bin");
                    printf("Arquivos salvos com sucesso!\n");

                    // Limpando as structs temporárias:

                    printf("\nLimpando dados temporários...\n");

                    hash_free(generoh);         
                    hash_free(autorh);             
                    free_catalogo(catalogo);      

                    printf("\nPrograma encerrado!\n \nPressione [Enter] para fechar a execução...\n");
                    getchar();
                    exit(0);
                }

                default:
                    printf("Opção inválida! Tente novamente.\n");
                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
            }
        }
    } while (opcao != 12);

    return 0;
}