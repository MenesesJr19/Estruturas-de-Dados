#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include "Pessoa.h"
#include "Livro.h"
typedef struct no
{
    PESSOA    *Info;
    struct no *Prox;
}NO;

typedef struct
{
    int NEL;
    NO *Inicio;
}LISTA;

//--------------------------------------------------
LISTA *CriarLista();
//--------------------------------------------------
void AddInicio(LISTA *L, PESSOA *X);
//--------------------------------------------------
void ShowListaPessoa(LISTA *L);
//--------------------------------------------------
void ShowListaLivro(LISTA *L);
//--------------------------------------------------
void DestruirLista(LISTA *L,void(*DestruirElemento)(void*));//A FUNÇÃO ACEITA UM PONTEIRO PARA UMA FUNÇÃO DE DESTRUIÇÃO ESPECIFICA,
//QUE SERA CHAMADA PARA CADA ELEMENTO DA LISTA
//--------------------------------------------------
PESSOA *PesquisarLista(LISTA *L, char *_nome);
//--------------------------------------------------

#endif // LISTA_H_INCLUDED
