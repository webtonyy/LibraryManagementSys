#ifndef emp_stt
#define emp_stt

typedef struct Livro Livro;

typedef struct No No_Livro;

typedef struct catalogo catalogo;

// 1 para emprestado, 0 para disponível
#define bool int
#define true 1
#define false 0


Livro *livro_init(char* nome, char* autor, char* genero);
No_Livro *no_livro_init(Livro *valor);
catalogo *catalogo_init();
void empstt_append(catalogo *l, No_Livro* livro);
void empstt_pop(catalogo *l, int idt);
void empstt_print(catalogo *l);
void empstt_clear(catalogo *l);
void empstt_changestatus(catalogo *l, int idt, bool status);
void empstt_status(catalogo *l, Livro *livro);



#endif