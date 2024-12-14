#include <stdio.h>
#include <stdlib.h>
#include "emp_stt.h"

int main() {
    // Inicializa o catálogo
    catalogo *cat = catalogo_init();

    // Adiciona livros ao catálogo
    Livro *livro1 = livro_init("Principia", "Isaac Newton", "Física");
    Livro *livro2 = livro_init("1984", "George Orwell", "Ficção");
    Livro *livro3 = livro_init("O Senhor dos Anéis", "J.R.R. Tolkien", "Fantasia");

    No_Livro *no1 = no_livro_init(livro1);
    No_Livro *no2 = no_livro_init(livro2);
    No_Livro *no3 = no_livro_init(livro3);

    empstt_append(cat, no1);
    empstt_append(cat, no2);
    empstt_append(cat, no3);

    // Imprime o catálogo completo
    printf("Catálogo inicial:\n");
    empstt_print(cat);

    // Testa a alteração de status de um livro
    printf("\nAlterando status do livro '1984' para emprestado:\n");
    empstt_changestatus(cat, 2, true);
    empstt_print(cat);

    // Verifica o status de um livro específico
    printf("\nVerificando status do livro '1984':\n");
    empstt_status(cat, livro2);

    // Tenta alterar o status de um livro que já está emprestado
    printf("\nTentando emprestar novamente o livro '1984':\n");
    empstt_changestatus(cat, 2, true);

    // Remove um livro do catálogo
    printf("\nRemovendo o livro 'Principia':\n");
    empstt_pop(cat, 1);
    empstt_print(cat);

    // Tenta remover um livro que não existe
    printf("\nTentando remover um livro inexistente (ID 10):\n");
    empstt_pop(cat, 10);

    // Limpa o catálogo
    printf("\nLimpando o catálogo:\n");
    empstt_clear(cat);
    
    // Tenta imprimir o catálogo vazio
    printf("\nTentando imprimir o catálogo após limpeza:\n");
    empstt_print(cat);

    // Libera a memória alocada para o catálogo
    free(cat);

    return 0;
}