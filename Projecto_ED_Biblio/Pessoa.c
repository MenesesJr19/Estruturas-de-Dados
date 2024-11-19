

#include "Pessoa.h"

/** \brief Permite Alocar e inicializar uma estrutura Pessoa
 *
 * \param _id int
 * \param _nome char*
 * \param _categoria char*
 * \return PESSOA*
 *
 */
PESSOA *CriarPessoa(int _id, char *_nome, char *_categoria, char *data_nascimento)
{

    PESSOA *P = (PESSOA *)malloc(sizeof(PESSOA));
    P->NOME = (char *)malloc((strlen(_nome) + 1)*sizeof(char));
    strcpy(P->NOME, _nome);
    P->CATEGORIA = (char *)malloc((strlen(_categoria) + 1)*sizeof(char));
    strcpy(P->CATEGORIA, _categoria);
    P->ID = _id;
    P->DATA_NASC = (char *)malloc((strlen(data_nascimento) + 1)*sizeof(char));
    strcpy(P->DATA_NASC, data_nascimento);
    P->ID_FREGUESIA = -1; // Inicialmente indefinido, será definido posteriormente
    return P;
}

void MostrarPessoa(PESSOA *P)
{
    printf("\tPESSOA: ID: %d [%s] [%s] [%s]\n", P->ID, P->NOME, P->CATEGORIA, P->DATA_NASC);
}
void DestruirPessoa(PESSOA *P)
{
    free (P->NOME);
    free (P->CATEGORIA);
    free (P->DATA_NASC);
    free (P);
}




