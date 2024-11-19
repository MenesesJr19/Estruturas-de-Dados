#ifndef HASHING_H_INCLUDED
#define HASHING_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"
#include "Livro.h"





typedef struct no_chave
{
    char *KEY; // Assim nao temos tamanho fixo!, Bem melhor!, mas depois será necessário fazer o free!
    LISTA *DADOS;
    struct no_chave *Prox;
}NO_CHAVE;

typedef struct
{
    int NEL;
    NO_CHAVE *Inicio;
}LISTA_CHAVES;

typedef struct
{
    LISTA_CHAVES *LChaves;
}HASHING;




//--------------------------------------------------------
LISTA_CHAVES *CriarListaCHAVES();
//--------------------------------------------------------
NO_CHAVE *AddCHAVE(LISTA_CHAVES *L, char *key);
//--------------------------------------------------------
HASHING *CriarHashing();
//--------------------------------------------------------
//void DestruirHashing(HASHING *H);
void DestruirHashing(HASHING *H, void (*DestruirElemento)(void *));//A FUNÇÃO  ACEITA UM PONTEIRO PARA UMA FUNÇÃO DE DESTRUIÇÃO ESPECIFICA,
//QUE SERA CHAMADA PARA CADA ELEMENTO DA TABELA DE HASH

//--------------------------------------------------------
void AddHashing(HASHING *H, PESSOA *P);
//--------------------------------------------------------
void ShowHashingPessoa(HASHING *H);

void ShowHashingLivro(HASHING *H);
//--------------------------------------------------------
NO_CHAVE *FuncaoHashing(HASHING *H, PESSOA *X);
//--------------------------------------------------------
NO_CHAVE *FuncaoHashingLivros(HASHING *H, LIVRO *L);
//--------------------------------------------------------
NO_CHAVE *FuncaoHashingByID(HASHING *H, const char *key);
//--------------------------------------------------------
NO_CHAVE *FuncaoHashingByID(HASHING *H, const char *key);
//--------------------------------------------------------
//--------------------------------------------------------

//--------------------------------------------------------
#endif // HASHING_H_INCLUDED
