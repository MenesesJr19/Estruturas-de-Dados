#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char *NOME;
    int ID;
    char *CATEGORIA;
    char *DATA_NASC;
    int ID_FREGUESIA;
}PESSOA;

PESSOA *CriarPessoa(int _id, char *_nome, char *_categoria, char *data_nasc);
void MostrarPessoa(PESSOA *P);
void DestruirPessoa(PESSOA *P);

#endif // PESSOA_H_INCLUDED
