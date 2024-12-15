#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emp_stt.h"



// Estrutura que representa um nó na lista encadeada de empréstimos
typedef struct No {
    char *nome;         // Nome do livro armazenado no nó
    char *autor;        // Nome do autor armazenado no nó
    struct No *prox;    // Ponteiro para o próximo nó na lista
    int qtd;            // Quantidade de livros no estoque
    bool status;        // Status do livro: true (emprestado), false (disponível)
} Livro;

// Estrutura que representa o catálogo de empréstimos como uma lista encadeada
typedef struct catalogo {
    Livro *cabeca;   // Ponteiro para o primeiro nó da lista (cabeça)
} catalogo;



// Cria um nó para a lista de empréstimos fornecendo um livro
Livro *livro_init(char *nome, char *autor) {
    Livro *atual = malloc(sizeof(Livro)); // Aloca memória para a estrutura Livro

    atual->nome = nome;     // Atribui o nome do livro ao nó
    atual->autor = autor;   // Atribui o autor do livro ao nó
    atual->prox = NULL;     // Define o próximo nó como NULL inicialmente (fim da lista)
    atual->status = false;  // Define status inicial como disponível (false)
    atual->qtd = 1;
    
    return atual; // Retorna o ponteiro para a estrutura Livro criada
}

// Cria e inicializa um catálogo de empréstimos vazio
catalogo *catalogo_init() {
    catalogo *cat = malloc(sizeof(catalogo)); // Aloca memória para a estrutura catalogo

    cat->cabeca = NULL; // Inicializa a cabeça da lista como NULL (lista vazia)

    return cat; // Retorna o ponteiro para a estrutura catalogo criada
}


// Função auxiliar para comparar se dois livros são iguais. Em caso positivo, retorna true, em caso contrário, false
bool compara_livros(Livro *l1, Livro *l2){

    if(strcmp(l1->nome, l2->nome) == 0 && strcmp(l1->autor, l2->autor) == 0){
        return true;
    }

    return false;
}

// Adiciona um livro no catálogo de empréstimos
void empstt_append(catalogo *l, Livro* livro) {


    // Verifica se a lista (catálogo) está vazia
    if (l->cabeca == NULL) {
        // Se estiver vazia, o novo livro se torna o primeiro elemento (cabeça) da lista
        l->cabeca = livro;
        return;
    }

    else {
        // Caso contrário, percorre a lista até encontrar o último nó
        Livro *temp = l->cabeca;

        // Continua até encontrar o último nó, onde 'prox' é NULL
        while (temp->prox) {
            temp = temp->prox;

            if(compara_livros(livro, temp)){ 
                // Achou um livro no catálogo com o mesmo nome e autor              
                temp->qtd++; // Adiciona mais um exemplar do livro
                return;// Retorna da função
            }
        }
        // Adiciona o novo livro ao final da lista
        temp->prox = livro; 
        return;
    }
}


// Retira um livro do catálogo de empréstimos pelo seu nome
void empstt_pop(catalogo *l, Livro *livro) {

    // Verifica se a lista está vazia
    if (l->cabeca == NULL) {
        printf("O catálogo está vazio!\n");
        return;
    }

    // Ponteiro temporário para percorrer a lista
    Livro *temp = l->cabeca;
    // Ponteiro auxiliar para manter referência ao nó anterior
    Livro *aux;

    // Verifica se o primeiro nó é o nó a ser removido
    if (compara_livros(livro, temp)) {
        // Atualiza a cabeça da lista para o próximo nó
        l->cabeca = temp->prox;
        // Libera a memória alocada para o livro e o nó
        free(temp);
        printf("Remoção realizada com sucesso!\n");
        return;
    }

    // Percorre a lista até encontrar o nó correspondente ou até o final da lista
    while (temp != NULL && (compara_livros(livro, temp) == false)) {
        aux = temp;      // Armazena o nó atual como anterior
        temp = temp->prox; // Avança para o próximo nó
    }

    // Verifica se o nó foi encontrado
    if (temp == NULL) {
        printf("O livro solicitado não foi encontrado!\n");
        return;
    }

    // Caso exista mais de uma unidade do livro, apenas retira um exemplar
    if(temp->qtd > 1){
        temp->qtd--;
        printf("Remoção de um exemplar do livro executada com sucesso!\n");
        return;
    }

    // Ajusta os ponteiros para remover o nó da lista
    aux->prox = temp->prox;
    // Libera a memória alocada para o livro
    free(temp);

    printf("Remoção realizada com sucesso!\n");
}

// Verifica o estado de empréstimo de um livro específico:
// true para emprestado, false para disponível
void empstt_status(catalogo *l, Livro *livro) {

    // Ponteiro temporário para percorrer a lista de livros no catálogo
    Livro *temp = l->cabeca;

    // Verifica se o catálogo está vazio
    if (temp == NULL) {
        printf("O catálogo está vazio! Por favor, adicione um livro antes de realizar quaisquer operações\n");
        return;
    }

    // Percorre a lista até encontrar o livro ou até o final da lista
    while (temp != NULL && (compara_livros(temp, livro) == false)) {
        temp = temp->prox; // Avança para o próximo nó
    }

    // Verifica se o livro não foi encontrado no catálogo
    if (temp == NULL) {
        printf("O livro requisitado não está no catálogo!\n");
        return;
    }

    // Caso o livro seja encontrado
    else if (compara_livros(temp, livro)) {
        char stt_empr[12]; // String para armazenar o status do livro

        if(temp->qtd == 0){
            temp->status = true; // Se no catálogo possuir 0 exemplares disponíveis, muda o status do livro para "emprestado aka true"
        }
        else{
            temp->status = false; // Caso contrário, livro disponível
        }

        // Define o status como "Disponível" ou "Emprestado" com base no valor de `status`
        if (temp->status == false) {
            strcpy(stt_empr, "Disponível");
        } else {
            strcpy(stt_empr, "Emprestado");
        }

        // Exibe o nome do livro e seu status atual
        printf("O livro %s está %s.\n", livro->nome, stt_empr);
        
        return;
    }
}

// Função para pegar um livro emprestado 
void empstt_pegarlivro(catalogo *l, Livro* livro){

    Livro *temp = l->cabeca;


    while(temp != NULL && (compara_livros(temp, livro) == false)){
        temp = temp->prox;
    }

    if(temp == NULL){
        printf("Livro não encontrado no catálogo! Confira o nome e tente novamente.\n");
        return;
    }
    else{
        if(temp->qtd >=1){ // Existem exemplares do livro no catálogo
            temp->qtd--;
            printf("Empréstimo efetuado com sucesso! Aproveite o livro. :)\n");
            return;
        }
        else{
            printf("Infelizmente todos os exemplares do livro estão emprestados. :(\n");
            return;
            
        }
       
    }
}
// Função para devolver um livro emprestado
void empstt_devolverlivro(catalogo *l, Livro *livro){

    Livro *temp = l->cabeca;


    while(temp != NULL && (compara_livros(temp, livro) == false)){
        temp = temp->prox;
    }

    if(temp == NULL){
        printf("Livro não encontrado no catálogo! Confira o nome e tente novamente.\n");
        return;
    }
    else{
        temp->qtd++;
        printf("Livro devolvido com sucesso!\n");
        return;
    }
       
}


// Limpa toda a lista de empréstimos
void empstt_clear(catalogo *l) {

    while (l->cabeca) {

        Livro *temp = l->cabeca;

        l->cabeca = l->cabeca->prox;

        free(temp);
    }

    // Imprime uma mensagem indicando que a lista foi limpa com sucesso
    printf("Lista de empréstimos apagada com sucesso!\n");
}

void empstt_print(catalogo *l){

    if(l == NULL){
        printf("Catalogo inexistente!\n");
        return;
    }

    if(l->cabeca == NULL){
        printf("Lista vazia! Adicione algum livro e tente novamente.\n");
        return;
    }

    Livro *temp = l->cabeca;



    while(temp){

        printf("\n| Nome: %s | Autor: %s | Status: %d | Exemplares Disponíveis: %d |\n ",
        temp->nome, temp->autor, temp->status, temp->qtd);

        temp = temp->prox;
    }
    return;
}