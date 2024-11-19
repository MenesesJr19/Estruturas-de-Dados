#ifndef REQUISICAO_H_INCLUDED
#define REQUISICAO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pessoa.h"
#include "Livro.h"


#include <time.h>
typedef struct
{

    int ID;
    PESSOA *Ptr_Req;
    LIVRO *Ptr_Livro;
    time_t data_requisicao;
    time_t data_devolucao;

}REQUISICAO;

REQUISICAO *CriarRequisicao(int _id, PESSOA *P, LIVRO *L);
void MostrarRequisicao(REQUISICAO *P);
void DestruirRequisicao(REQUISICAO *P);

void FormatarDataHora(time_t tempo, char *buffer, size_t bufferSize);

#endif // REQUISICAO_H_INCLUDED
