#ifndef _CATALOGO_H
#define _CATALOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Definições para representar o status de empréstimo: 1 para emprestado, 0 para disponível
#define bool int
#define true 1
#define false 0

// Estrutura para armazenar informações sobre o livro
typedef struct Livro {
    char *nome;           // Título do livro (alocado dinamicamente)
    char *nome_norma;
    char *autor;            // Nome do autor (alocado dinamicamente)
    char *autor_norma;
    char *genero;           // Gênero literário (alocado dinamicamente)
    char *genero_norma;
    int qtd;                // Quantidade de exemplares
    bool status;           // Status: true se o livro está alugado, false caso contrário
} Livro;

// Estrutura do nó da árvore binária
typedef struct NoArvore {
    Livro *livro;           // Ponteiro para o livro armazenado no nó
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} No;

// Estrutura para representar o catálogo de livros
typedef struct catalogo {
    No *raiz;               // Raiz da árvore binária de livros
    int nl;                 // Número de livros no catálogo
} Catalogo;

// Funções principais do catálogo
Livro* livro_init(const char *nome, const char *genero, const char *autor);
Catalogo* catalogo_init();
void editar_livro(Catalogo *c, const char *nome_errado, const char *novo_nome, const char *autor,const char *genero);
Livro* add_livro(Catalogo *c, Livro *l);
void remover_livro(Catalogo *c, const char *nome, const char *autor);
void listar_livros(Catalogo *c);
int devolve_livro(Catalogo *c, Livro *l); // Retorna 1 se devolvido, 2 se doado
int emprestar_livro(Catalogo *c, Livro *l); // Retorna 1 se alugado, 0 se indisponível, -1 em caso de erro
int buscar_por_nome(Catalogo *c, const char *nome); // Busca todos os livros com o mesmo título
void verificar_status(Catalogo *c, const char *nome, const char *autor);
void free_catalogo(Catalogo *c);
void salvar_catalogo(Catalogo *c, const char *nome_arquivo);
Catalogo* carregar_catalogo(const char *nome_arquivo);
void free_livro(Livro *livro);
void free_no(No *atual);

#endif // _CATALOGO_H
