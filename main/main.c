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
    setlocale(LC_ALL, "pt_BR.iso88591"); // Configura o locale para suportar caracteres acentuados

    // Carregando das estruturas de um arquivo binário externo:
    printf("\nCarregando as configurações iniciais. Por favor, aguarde!\n");
    dormir(2);
    Catalogo *catalogo = carregar_catalogo("./savedata/data_c.bin"); 
    HashTable *autorh = hash_table_init(101);
    HashTable *generoh = hash_table_init(101);
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
                    char *nome = NULL, *autor=NULL, *genero = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser adicionado: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    len = 0;
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1 || is_valid_string(genero) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(genero);
                        free(autor);
                        break;
                    }

                    // Inicializa um novo livro (aloca memória dinamicamente para os campos)
                    Livro *novo_livro = livro_init(nome, genero, autor);

                    dormir(1);

                    if (novo_livro) {
                        // Adiciona o livro ao catálogo (árvore binária)
                        add_livro(catalogo, novo_livro);

                        // Insere o livro nas tabelas hash (gêneros e autores)
                        inserir_livro(generoh, novo_livro, genero); // Usa apenas strings alocadas dinamicamente no hash
                        inserir_livro(autorh, novo_livro, autor);

                        printf("Livro adicionado com sucesso!\n");
                    } else {
                        printf("Erro ao criar o livro.\n");
                    }

                    free(nome);
                    free(genero);
                    free(autor);
                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();

                    break;
                }


                case 2: { // Remover ou decrementar livro
                    char *nome = NULL, *autor=NULL, *genero = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser removido: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    len = 0;
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1 || is_valid_string(genero) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(genero);
                        free(autor);
                        break;
                    }


                    dormir(1);

                    remover_livro(catalogo, nome, autor);
                    deletar(autorh,nome,autor);
                    deletar(generoh,nome,genero);

                    free(autor);
                    free(nome);
                    free(genero);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 3: { // Editar livro
                    char *nome = NULL, *autor=NULL, *genero = NULL, *novo_nome = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser editado: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    len = 0;
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o nome do livro
                    printf("Digite o novo nome do livro: ");
                    len=0;
                    getline(&novo_nome, &len, stdin);
                    novo_nome[strcspn(novo_nome, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1 || is_valid_string(genero) !=1 || is_valid_string(nome) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(genero);
                        free(autor);
                        free(novo_nome);
                        break;
                    }

                    editar_livro(catalogo, nome, novo_nome, autor,genero);
                    edita_livro(autorh,nome,novo_nome,autor);
                    edita_livro(generoh,nome,novo_nome,genero);
                    dormir(1);

                    free(nome);
                    free(genero);
                    free(autor);
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
                    char *nome = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser buscado: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        break;
                    }

                    dormir(1);
                    int encontrados = buscar_por_nome(catalogo, nome);
                    if (encontrados == 0) {
                        printf("Livro não encontrado.\n");
                    }

                    free(nome);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 6: { // Buscar por autor (usando tabela hash)
                    char *autor = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o autor do livro a ser buscado: ");
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(autor) !=1){
                        printf("Caracteres inválidos.");
                        free(autor);
                        break;
                    }

                    if(is_valid_string(autor) !=1){
                        printf("Caracteres inválidos.");
                        break;
                    }

                    dormir(1);
                    buscar_por_no(autorh, autor); // Busca na tabela hash de autores

                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 7: { // Buscar por gênero (usando tabela hash)
                    char *genero = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o gênero do livro a ser buscado: ");
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(genero) !=1){
                        printf("Caracteres inválidos.");
                        free(genero);
                        break;
                    }



                    dormir(1);
                    buscar_por_no(generoh, genero); // Busca na tabela hash de gêneros

                    free(genero);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 8: { // Alugar livro
                    // Declaração de variáveis locais para armazenar os dados do livro
                    char *nome = NULL, *autor=NULL, *genero = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser alugado: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    len = 0;
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1 || is_valid_string(genero) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(genero);
                        free(autor);
                        break;
                    }

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

                    free(nome);
                    free(genero);
                    free(autor);

                    free_livro(novo_livro);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 9: { // Devolver livro
                    // Declaração de variáveis locais para armazenar os dados do livro
                    char *nome = NULL, *autor=NULL, *genero = NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser devolvido: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o gênero do livro
                    printf("Digite o gênero do livro: ");
                    len = 0;
                    getline(&genero, &len, stdin);
                    genero[strcspn(genero, "\n")] = '\0'; // Remove o caractere '\n'

                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1 || is_valid_string(genero) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(genero);
                        free(autor);
                        break;
                    }
                    
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

                    free(nome);
                    free(genero);
                    free(autor);

                    printf("\nPressione [Enter] para retornar...\n");
                    getchar();
                    break;
                }

                case 10: { // Verificar status de um livro
                    // Declaração de variáveis locais para armazenar os dados do livro
                    char *nome = NULL, *autor=NULL;
                    size_t len = 0;

                    // Solicita o nome do livro
                    printf("Digite o nome do livro a ser consultado: ");
                    getline(&nome, &len, stdin);
                    nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                    // Solicita o autor do livro
                    printf("Digite o autor do livro: ");
                    len = 0;
                    getline(&autor, &len, stdin);
                    autor[strcspn(autor, "\n")] = '\0'; // Remove o caractere '\n'


                    if(is_valid_string(nome) !=1 || is_valid_string(autor) !=1){
                        printf("Caracteres inválidos.");
                        free(nome);
                        free(autor);
                        break;
                    }

                    verificar_status(catalogo, nome, autor);

                    free(nome);
                    free(autor);

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