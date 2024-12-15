#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emp_stt.h"

// Função auxiliar para criar e inicializar um livro
Livro* criar_livro(char *nome, char *autor) {
    return livro_init(nome, autor);
}

int main() {
    // Inicializando o catálogo
    catalogo *meuCatalogo = catalogo_init();

    // Criando 20 livros para testar
    Livro *livros[20];
    char *nomes[] = {
        "Livro A", "Livro B", "Livro C", "Livro D", "Livro E",
        "Livro F", "Livro G", "Livro H", "Livro I", "Livro J",
        "Livro K", "Livro L", "Livro M", "Livro N", "Livro O",
        "Livro P", "Livro Q", "Livro R", "Livro S", "Livro T"
    };
    char *autores[] = {
        "Autor 1", "Autor 2", "Autor 3", "Autor 4", "Autor 5",
        "Autor 6", "Autor 7", "Autor 8", "Autor 9", "Autor 10",
        "Autor 11", "Autor 12", "Autor 13", "Autor 14", "Autor 15",
        "Autor 16", "Autor 17", "Autor 18", "Autor 19", "Autor 20"
    };

    for (int i = 0; i < 20; i++) {
        livros[i] = criar_livro(nomes[i], autores[i]);
        empstt_append(meuCatalogo, livros[i]);  // Adicionando os livros ao catálogo
    }

    printf("\n--- Catálogo Inicializado com 20 Livros ---\n");
    empstt_print(meuCatalogo);

    // Testando remoção de um livro
    printf("\n--- Remoção de um Livro (Livro J) ---\n");
    empstt_pop(meuCatalogo, livros[9]);  // Removendo "Livro J"
    empstt_print(meuCatalogo);

    // Testando o status de um livro disponível
    printf("\n--- Verificando o Status de um Livro Disponível (Livro A) ---\n");
    empstt_status(meuCatalogo, livros[0]);

    // Testando empréstimo de um livro
    printf("\n--- Pegando um Livro Emprestado (Livro A) ---\n");
    empstt_pegarlivro(meuCatalogo, livros[0]);
    empstt_status(meuCatalogo, livros[0]);

    // Tentando pegar emprestado novamente no caso de estoque 0
    printf("\n--- Tentando Emprestar um Livro Sem Estoque Disponível (Livro A) ---\n");
    empstt_pegarlivro(meuCatalogo, livros[0]);

    // Testando devolução de um livro
    printf("\n--- Devolvendo um Livro Emprestado (Livro A) ---\n");
    empstt_devolverlivro(meuCatalogo, livros[0]);
    empstt_status(meuCatalogo, livros[0]);

    // Emprestando e devolvendo repetidamente
    for (int i = 0; i < 5; i++) {
        printf("\n--- Emprestando e Devolvendo Repetidamente (Livro B) - Iteração %d ---\n", i + 1);
        empstt_pegarlivro(meuCatalogo, livros[1]);
        empstt_status(meuCatalogo, livros[1]);
        empstt_devolverlivro(meuCatalogo, livros[1]);
        empstt_status(meuCatalogo, livros[1]);
    }

    // Verificando status de um livro inexistente
    printf("\n--- Verificando o Status de um Livro Não Existente (Livro Z) ---\n");
    Livro *livroNaoExistente = criar_livro("Livro Z", "Autor Z");
    empstt_status(meuCatalogo, livroNaoExistente);

    // Testando limpeza completa do catálogo
    printf("\n--- Limpando o Catálogo ---\n");
    empstt_clear(meuCatalogo);
    empstt_print(meuCatalogo);

    // Liberando memória alocada
    free(livroNaoExistente);
    free(meuCatalogo);

    printf("\n--- Testes Finalizados ---\n");

    return 0;
}
