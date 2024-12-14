#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emp_stt.h"

int id = 0;

typedef struct Livro{

    char* nome;
    char* autor;
    char* genero;

}Livro;

typedef struct No {

Livro *valor;
struct No *prox;
int id;
bool status;

} No_Livro;

typedef struct catalogo{
No_Livro *cabeca;
} catalogo;

// Cria uma entidade da struct 'livro' fornecendo nome, autor e gênero
Livro *livro_init(char* nome, char* autor, char* genero){

    char *nome_f = malloc((strlen(nome)+1)*sizeof(char));
    char *autor_f = malloc((strlen(autor)+1)*sizeof(char));
    char *gen_f = malloc((strlen(genero)*1)*sizeof(char));

    strcpy(nome_f, nome);
    strcpy(autor_f, autor);
    strcpy(gen_f, genero);

    Livro *livro = malloc(sizeof(Livro));

    livro->nome = nome_f;
    livro->autor = autor_f;
    livro->genero = gen_f;

    return livro;
}

// Cria um nó para a lista de empréstimos fornecendo o livro
No_Livro *no_livro_init(Livro *valor){
    No_Livro *atual = malloc(sizeof(No_Livro));

    atual->valor = valor;
    atual->prox = NULL;
    atual->status = false;
    atual->id = -1;

    return atual;
}

// Cria o catálogo de empréstimos
catalogo *catalogo_init(){

    catalogo *cat = malloc(sizeof(catalogo));

    cat->cabeca = NULL;

    return cat;
}


// Adiciona um livro no catálogo de empréstimos
void empstt_append(catalogo *l, No_Livro* livro){

    id++;

    livro->id = id;

    if(l->cabeca == NULL){
        l->cabeca = livro;
    }
    else{
        No_Livro *temp = l->cabeca;

        while(temp->prox){
            temp = temp->prox;
        }
        temp->prox = livro;
    }

}

// Retira um livro do catálogo de empréstimos pelo seu id.
void empstt_pop(catalogo *l, int idt){

    No_Livro *temp = l->cabeca;

    No_Livro *aux;

    if(l->cabeca = NULL){
        printf("A lista está vazia!\n");
        return;
    }

    if(idt>id){
        printf("Livro Inválido. Verifique seu ID e tente novamente!\n");
        return;

    }


    if(temp->id == idt){
        l->cabeca = temp->prox;
        free(temp->valor);
        free(temp);
        printf("Remoção realizada com sucesso!\n");
        return;
    }

    while(temp->id != idt && temp != NULL){
        aux = temp;
        temp = temp->prox;
    }

    if(temp == NULL){
        printf("O livro solicitado não foi encontrado!\n");
        return;
    }

    aux->prox = temp->prox;
    free(temp->valor);
    free(temp);

    printf("Remoção realizada com sucesso!\n");
    return;

}

//Verifica o estado de empréstimo de um livro específico:
// true para emprestado, false para disponível


void empstt_status(catalogo *l, Livro *livro){

    No_Livro *temp = l->cabeca;

    if(temp == NULL){
        printf("O catálogo está vazio! Por favor, adicione um livro antes de realizar quaisquer operações\n");
        return;
    }

    while(temp->valor != livro && temp != NULL){

        temp = temp->prox;

    }

    if(temp == NULL){
        printf("O livro requisitado não está no catálogo, por favor, adicione-o antes de realizar quaisquer operações!\n");
        return;
    }

    else if(temp->valor == livro){

        char stt_empr[12];


        if(temp->status == false){
            strcpy(stt_empr, "Disponível");
        }
        else{
            strcpy(stt_empr, "Emprestado");
        }

        printf("O livro %s (ID = %d) está %s.\n", livro->nome, temp->id, stt_empr);
        
        return;

    }
}

// Altera o status do livro
void empstt_changestatus(catalogo *l, int idt, bool status){

    No_Livro *temp = l->cabeca;

    if(idt <= 0){
        printf("ID Inválido!\n");
        return;
    }


    for(int i = 1; i<idt; i++){
        temp = temp->prox;
    }

    if(temp == NULL){
        printf("Livro não encontrado! Por favor, confira o ID do livro e tente novamente.\n");
        return;
    }

    else{
        if(temp->status == status){
            printf("O livro: %s (ID = %d) já está com o status requisitado!\n", temp->valor->nome, temp->id);
            return;
        }

        temp->status = status;
        printf("Operação efetuada com sucesso!\n");
        return;
    }
}

// Mostra ao usuário as informações de empréstimo sobre todos os livros do catálogo
void empstt_print(catalogo *l){

    char stt_empr[12];
    No_Livro *temp = l->cabeca;

    if(l->cabeca == NULL){
        printf("O catálogo está vazio! Insira algum livro e tente novamente.\n");
        return;
    }
    printf("\n");
    printf("================================================================================\n");
    while(temp != NULL){
        
        if(temp->status == false){
            strcpy(stt_empr, "Disponível");
        }
        else{
            strcpy(stt_empr, "Emprestado");
        }

        printf("Nome do livro: %s | ID do Livro: %d | Status: %s\n", temp->valor->nome, temp->id, stt_empr);

        temp = temp->prox;
    }
    printf("================================================================================\n");
    printf("\n");
}

// Limpa toda a lista de empréstimos
void empstt_clear(catalogo *l){
    No_Livro *temp = l->cabeca;

    No_Livro *aux;

    while(temp != NULL){

        aux = temp->prox;
        free(temp->valor);
        free(temp);
        temp = aux;
    }

    printf("Lista de empréstimos apagada com sucesso!\n");

}