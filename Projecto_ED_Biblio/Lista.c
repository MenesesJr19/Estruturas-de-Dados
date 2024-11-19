
#include "Lista.h"

//--------------------------------------------------
/** \brief Permite alocar memória para uma lista
 *
 * \return LISTA* : Retorna a lista alocada
 * \date 10/04/2024
 * \author Docentes de ED
 */
LISTA *CriarLista()
{
    LISTA *L = (LISTA *)malloc(sizeof(LISTA));
    L->NEL = 0;
    L->Inicio = NULL;
    return L;
}

//--------------------------------------------------
void AddInicio(LISTA *L, PESSOA *X)
{
    if (!L) return;
    NO *aux = (NO *)malloc(sizeof(NO));
    aux->Info = X;
    aux->Prox = L->Inicio;
    L->Inicio = aux;
    L->NEL++;
}

//--------------------------------------------------
void ShowListaLivro(LISTA *L)
{
    if (!L) return;
    //printf("NEL = %d\n", L->NEL);
    NO *P = L->Inicio;
    while (P != NULL)
    {

        MostrarLivro(P->Info);
        P = P->Prox;
    }
    printf("\n");
}

void ShowListaPessoa(LISTA *L)
{
    if (!L) return;
    //printf("NEL = %d\n", L->NEL);
    NO *P = L->Inicio;
    while (P != NULL)
    {
        MostrarPessoa(P->Info);
        P = P->Prox;

    }
}

//--------------------------------------------------
void DestruirListaPessoa(LISTA *L)
{
    //printf("Implementar <%s>\n", __FUNCTION__);
    if (!L) return;
    NO *Martim;
    NO *Helder = L->Inicio;
    while (Helder)
    {
        Martim = Helder->Prox;
        DestruirPessoa(Helder->Info);
        free (Helder);
        Helder = Martim;
    }
    free(L);
}

//REFORMULAÇÃO DA DESTRUIÇÃO DE LISTA, A FUNÇÃO AGORA ACEITA UM PONTEIRO PARA UMA FUNÇÃO DE DESTRUIÇÃO ESPECIFICA,
//QUE SERA CHAMADA PARA CADA ELEMENTO DA LISTA

void DestruirLista(LISTA *L, void (*DestruirElemento)(void *)) {
    if (!L) return;
    NO *current = L->Inicio;
    while (current) {
        NO *next = current->Prox;
        DestruirElemento(current->Info);
        free(current);
        current = next;
    }
    free(L);
}



//--------------------------------------------------
PESSOA *PesquisarLista(LISTA *L, char *_nome)
{
    printf("Implementar <%s>\n", __FUNCTION__);
    if (L == NULL) return NULL;
    NO *P = L->Inicio;
    while (P)
    {
        if (stricmp(P->Info->NOME, _nome) == 0)
            return P->Info;
        P = P->Prox;
    }
    return NULL;
}

//--------------------------------------------------
