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

// Função para inicializar um livro
Livro* livro_init(const char *nome, const char *genero, const char *autor);

// Função para inicializar um catálogo
Catalogo* catalogo_init();

// Função para editar informações de um livro no catálogo
void editar_livro(Catalogo *c, const char *nome_errado, const char *novo_nome, const char *autor,const char *genero);

// Função para adicionar um livro ao catálogo
Livro* add_livro(Catalogo *c, Livro *l);

// Função para remover um livro do catálogo
void remover_livro(Catalogo *c, const char *nome, const char *autor);

// Função para listar todos os livros no catálogo
void listar_livros(Catalogo *c);

// Função para devolver um livro. Retorna 1 se devolvido, 2 se doado
int devolve_livro(Catalogo *c, Livro *l);

// Função para emprestar um livro. Retorna 1 se alugado, 0 se indisponível, -1 em caso de erro
int emprestar_livro(Catalogo *c, Livro *l);

// Função para buscar livros por nome. Busca todos os livros com o mesmo título
int buscar_por_nome(Catalogo *c, const char *nome);

// Função para verificar o status de um livro específico
void verificar_status(Catalogo *c, const char *nome, const char *autor);

// Função para liberar a memória alocada para o catálogo
void free_catalogo(Catalogo *c);

// Função para salvar o catálogo em um arquivo
void salvar_catalogo(Catalogo *c, const char *nome_arquivo);

// Função para carregar um catálogo de um arquivo
Catalogo* carregar_catalogo(const char *nome_arquivo);

// Função para liberar a memória alocada para um livro
void free_livro(Livro *livro);

// Função para liberar a memória alocada para um nó da árvore
void free_no(No *atual);

#endif 
