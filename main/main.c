#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "../include/catalogo.h"
#include "../include/hash.h"

int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para suportar caracteres acentuados

    // Inicializa o catálogo e as tabelas hash
    Catalogo *catalogo = catalogo_init();
    HashTable *generoh = hash_table_init(10); // Tabela hash para gêneros
    HashTable *autorh = hash_table_init(10);  // Tabela hash para autores

    // Adiciona alguns livros para teste
    Livro *livro1 = livro_init("O Senhor dos Anéis", "Fantasia", "J.R.R. Tolkien");
    Livro *livro2 = livro_init("1984", "Distopia", "George Orwell");
    Livro *livro3 = livro_init("Dom Casmurro", "Romance", "Machado de Assis");

    printf("\nAdicionando livros...\n");

    // Adiciona os livros ao catálogo e às tabelas hash
    add_livro(catalogo, livro1);
    inserir_livro(generoh, livro1, livro1->genero);
    inserir_livro(autorh, livro1, livro1->autor);

    add_livro(catalogo, livro2);
    inserir_livro(generoh, livro2, livro2->genero);
    inserir_livro(autorh, livro2, livro2->autor);

    add_livro(catalogo, livro3);
    inserir_livro(generoh, livro3, livro3->genero);
    inserir_livro(autorh, livro3, livro3->autor);

    // Imprime as tabelas hash de gêneros e autores
    printf("\nTabela Hash de Gêneros:\n");
    imprimir_tabela_hash(generoh);

    printf("\nTabela Hash de Autores:\n");
    imprimir_tabela_hash(autorh);

    printf("\nCatálogo:\n");
    listar_livros(catalogo);

    // Teste: Renomear um livro
    printf("\nRenomeando '1984' para 'Mil Novecentos e Oitenta e Quatro'...\n");
    editar_livro(catalogo, "1984", "Mil Novecentos e Oitenta e Quatro", "George Orwell");
    edita_livro(generoh, "1984", "Mil Novecentos e Oitenta e Quatro", "Distopia");
    edita_livro(autorh, "1984", "Mil Novecentos e Oitenta e Quatro", "George Orwell");

    printf("\nTabela Hash de Gêneros após renomear:\n");
    imprimir_tabela_hash(generoh);

    printf("\nTabela Hash de Autores após renomear:\n");
    imprimir_tabela_hash(autorh);

    printf("\nCatálogo após renomear:\n");
    listar_livros(catalogo);

    // Teste: Alugar um livro
    printf("\nAlugando 'O Senhor dos Anéis'...\n");
    emprestar_livro(catalogo, "O Senhor dos Anéis", "J.R.R. Tolkien");

    printf("\nCatálogo após aluguel:\n");
    listar_livros(catalogo);

    // Teste: Devolver um livro
    printf("\nDevolvendo 'O Senhor dos Anéis'...\n");
    devolve_livro(catalogo, "O Senhor dos Anéis", "J.R.R. Tolkien");

    printf("\nCatálogo após devolução:\n");
    listar_livros(catalogo);

    // Teste: Remover um livro
    printf("\nRemovendo 'Dom Casmurro'...\n");
    remover_livro(catalogo, "Dom Casmurro");
    deletar(generoh, "Dom Casmurro", "Romance");
    deletar(autorh, "Dom Casmurro", "Machado de Assis");

    printf("\nTabela Hash de Gêneros após remoção:\n");
    imprimir_tabela_hash(generoh);

    printf("\nTabela Hash de Autores após remoção:\n");
    imprimir_tabela_hash(autorh);

    printf("\nCatálogo após remoção:\n");
    listar_livros(catalogo);

    // Teste: Adicionar um mesmo livro novamente
    printf("\nAdicionando novamente 'Dom Casmurro'...\n");
    add_livro(catalogo, livro3);
    inserir_livro(generoh, livro3, livro3->genero);
    inserir_livro(autorh, livro3, livro3->autor);

    printf("\nTabela Hash de Gêneros após re-adicionar:\n");
    imprimir_tabela_hash(generoh);

    printf("\nTabela Hash de Autores após re-adicionar:\n");
    imprimir_tabela_hash(autorh);

    printf("\nCatálogo após re-adicionar:\n");
    listar_livros(catalogo);

    
   // Teste: Buscar status inválido ou inexistente 

   ///Liberação 
}
