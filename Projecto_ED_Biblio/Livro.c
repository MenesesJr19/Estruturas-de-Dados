
#include "Livro.h"

LIVRO *CriarLivro(int _id, char *_nome, char *_autor, char *_area)
{
    LIVRO *P = (LIVRO *)malloc(sizeof(LIVRO));
    P->NOME = (char *)malloc((strlen(_nome) + 1)*sizeof(char));
    strcpy(P->NOME, _nome);
    P->AUTOR = (char *)malloc((strlen(_autor) + 1)*sizeof(char));
    strcpy(P->AUTOR, _autor);
    P->AREA = (char *)malloc((strlen(_area) + 1)*sizeof(char));
    strcpy(P->AREA, _area);
    P->ID = _id;
    P->disponivel = 1; //Assim que o livro é criado ele fica automaticamente disponivel
    return P;
}
void MostrarLivro(LIVRO *P)
{
    printf("\Livro: ID: %d [%s] [%s] [%s] [%s]\n",
           P->ID, P->NOME,P->AUTOR, P->AREA,P->disponivel? "Disponivel" : "Requisitado");
}
void DestruirLivro(LIVRO *P)
{
    if (P != NULL)
    {
        free(P->NOME);
        free(P->AUTOR);
        free(P->AREA);
        free(P);
    }
}
