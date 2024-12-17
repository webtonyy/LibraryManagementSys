#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../include/catalogo.h"
#include "../include/hash.h"
#include "../include/aux.h"

// Função para exibir o menu de opções
void exibir_menu() {
    printf("\nDigite um número para selecionar uma ação:\n");
    printf("1 - Adicionar livro\n");
    printf("2 - Remover ou decrementar livro\n");
    printf("3 - Editar livro\n");
    printf("4 - Listar livros\n");
    printf("5 - Buscar livro por nome\n");
    printf("6 - Buscar livro por autor\n");
    printf("7 - Buscar livro por gênero\n");
    printf("8 - Alugar livros\n");
    printf("9 - Devolver livros\n");
    printf("10 - Verificar status de um livro\n");
    printf("11 - Sair\n");
    printf("Escolha: ");
}

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para suportar caracteres acentuados

    Catalogo *catalogo = catalogo_init(); // Inicializa o catálogo
    HashTable table;
    table_init(&table);
    inserir_generos_iniciais(&table);

    char buffer[10];
    int opcao;

    do {
        exibir_menu();
        
        if(fgets(buffer,sizeof(buffer),stdin) == NULL){ // Caso o usuário encerre o terminal
            opcao = 11; // Atribui a opção 11 -> saída do programa
            
        }
        else{
            opcao = atoi(buffer); // Convertendo a entrada para int p ser utilizada no switch case
        }

        switch (opcao) {
            case 1: { // Adicionar livro
                char nome[100], autor[100], genero[50];
                // Solicita os dados do livro
                printf("Digite o nome do livro: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere '\n'

                printf("Digite o autor do livro: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';

                printf("Digite o gênero do livro: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';

                // Verifica se o gênero existe na tabela hash
                if (inserir_livro(&table, nome, autor, genero) == 1) {
                    // Cria um novo livro
                    Livro *novo_livro = livro_init(nome, genero, autor);
                    if (novo_livro) {
                        // Adiciona o livro ao catálogo
                        add_livro(catalogo, novo_livro);
                        printf("Livro adicionado com sucesso!\n");
                    } else {
                        printf("Erro ao criar o livro. Certifique-se de que os campos contêm apenas letras e espaços.\n");
                    }
                } else {
                    printf("Erro: Gênero não encontrado na tabela. Verifique a ortografia ou adicione este gênero primeiro.\n");
                }
                break;
            }
            case 2: { // Remover ou decrementar livro
                char nome[100], autor[100];
                // Solicita os dados do livro
                printf("Digite o nome do livro: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite o autor do livro: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';

                // Normaliza as strings para comparação
                char *nome_normalizado = normalize_string(nome);
                char *autor_normalizado = normalize_string(autor);

                // Busca e remove ou decrementa no catálogo
                Livro *removido = remover_livro(catalogo, nome_normalizado);

                if (removido) {
                    if (removido->qtd == 0) {
                        // Remover completamente da tabela hash
                        deletar(&table, nome_normalizado, autor_normalizado, removido->genero);
                        printf("Livro '%s' removido completamente.\n", removido->real_nome);
                    } else {
                        printf("Quantidade do livro '%s' decrementada. Quantidade atual: %d\n", removido->real_nome, removido->qtd);
                    }
                    if (removido) free(removido);

                } else {
                    printf("Erro: Livro '%s' não encontrado no catálogo.\n", nome);
                }
                break;
            }
            case 3: { // Editar livro
                char nome[100], novo_nome[100], novo_autor[100];

                printf("Digite o nome do livro a ser editado: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Digite o novo nome do livro: ");
                fgets(novo_nome, sizeof(novo_nome), stdin);
                novo_nome[strcspn(novo_nome, "\n")] = '\0';

                printf("Digite o novo autor do livro: ");
                fgets(novo_autor, sizeof(novo_autor), stdin);
                novo_autor[strcspn(novo_autor, "\n")] = '\0';

                Livro temp_livro = {
                    .nome = strdup(novo_nome),
                    .autor = strdup(novo_autor),
                    .esq = NULL,
                    .dir = NULL,
                    .qtd = 1
                };

                Livro *editado = editar_livro(catalogo, nome, &temp_livro);

                if (editado) {
                    printf("Livro editado com sucesso!\n");

                    free(temp_livro.nome); // Libera strings temporárias
                    free(temp_livro.autor);

                    break;
                }
            }
            case 4: { // Listar livros
                listar_livros(catalogo); 
                break; 
            }
            
            case 5: { // Buscar livro por nome 
                char nome[100]; 
                printf("Digite o nome do livro a ser buscado: "); 
                fgets(nome, sizeof(nome), stdin); 
                nome[strcspn(nome,"\n")]='\0'; 

                if (!verifica_livro(catalogo,nome)) { 
                    printf ("Livro não encontrado.\n"); 
                } 
                break; 
            }
            
            case 6: { // Busca por autor 
                char autor[100]; 
                printf ("Digite o nome do autor a ser buscado: "); 
                fgets(autor,sizeof(autor),stdin); 
                autor[strcspn(autor,"\n")]='\0'; 

                buscar_por_autor(&table ,autor); 

                break; 
            }
            
            case 7:{//Busca por gênero 
                char genero[50]; 
                printf ("Digite o gênero a ser buscado: "); 
                fgets(genero,sizeof(genero),stdin); 
                genero[strcspn(genero,"\n")]='\0'; 

                buscar_por_genero(&table ,genero); 

                break; 
            }
            
            case 8:{//Alugar livro 
                char nome[100],autor[100];  
                printf ("Digite o nome do livro a ser alugado : "); 
                fgets(nome,sizeof(nome),stdin); 
                nome[strcspn(nome,"\n")]='\0';  
                printf ("Digite o nome do autor do livro a ser alugado : "); 
                fgets(autor,sizeof(autor),stdin);   
                autor[strcspn(autor,"\n")]='\0';    
                emprestar_livro(catalogo ,nome ,autor);     
                break; 

            }

            case 9:{//Devolver livro 

                char nome[100],autor[100]; 
                printf ("Digite o nome do livro a ser devolvido : "); 
                fgets(nome,sizeof(nome),stdin); 
                nome[strcspn(nome,"\n")]='\0'; 
                printf ("Digite o nome do autor do livro a ser devolvido : "); 
                fgets(autor,sizeof(autor),stdin); 
                autor[strcspn(autor,"\n ")]='\0'; 
                devolve_livro(catalogo ,nome ,autor); 
                break; 

            }

            case 10:{//Verificar status de um Livro

                char nome[100],autor[100];  
                printf ("Digite o nome do Livro a ser consultado : ");  
                fgets(nome,sizeof(nome),stdin); 
                nome[strcspn(nome,"\n ")]='\0';    
                printf ("Digite o Nome Do Autor Do Livro A Ser Consultado : "); 
                fgets(autor,sizeof(autor),stdin);   
                autor[strcspn(autor,"\n ")]='\0';  
                verificar_status(catalogo,nome ,autor); 
                break;

            }

            case 11:{//Sair

                hash_free(&table); // Libera a memória alocada para a tabela hash
                free_catalogo(catalogo); // Libera a memória alocada para o catálogo

                printf("Encerrando O Programa...\n ");
                break;

            }

            default:

                printf("Opção Inválida! Tente Novamente\n ");

        }

    } while (opcao !=11);

    return 0;

}
