#ifndef LIVRO_H_INCLUDED
#define LIVRO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    char *NOME;
    int ID;
    char *AREA;
    char *AUTOR;
    int disponivel;//1 - disponivel, 0 - requisitado
}LIVRO;

LIVRO *CriarLivro(int _id, char *_nome,char *_autor, char *_area);
void MostrarLivro(LIVRO *P);
void DestruirLivro(LIVRO *P);


#endif // LIVRO_H_INCLUDED
