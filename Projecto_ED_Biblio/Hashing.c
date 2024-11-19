
#include "Hashing.h"
#include "Biblioteca.h"


LISTA_CHAVES *CriarListaCHAVES()
{
    LISTA_CHAVES *L = (LISTA_CHAVES *)malloc(sizeof(LISTA_CHAVES));
    L->NEL = 0;
    L->Inicio = NULL;
    return L;
}

//--------------------------------------------------
NO_CHAVE *AddCHAVE(LISTA_CHAVES *L, char *key)
{
    if (!L) return NULL;
    NO_CHAVE *aux = (NO_CHAVE *)malloc(sizeof(NO_CHAVE));
    aux->KEY = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(aux->KEY, key);
    aux->DADOS = CriarLista();
    aux->Prox = L->Inicio;
    L->Inicio = aux;
    L->NEL++;
    return aux;
}
HASHING *CriarHashing()
{
    HASHING *Has = (HASHING *)malloc(sizeof(HASHING));
    Has->LChaves = CriarListaCHAVES();
    return Has;
}
/*
void DestruirHashing(HASHING *H)
{
    if (!H) return;
    NO_CHAVE *Seguinte;
    NO_CHAVE *P = H->LChaves->Inicio;
    while (P)
    {
        Seguinte = P->Prox;
        DestruirLista(P->DADOS);
        free (P->KEY);
        free (P);
        P = Seguinte;
    }
    free(H);
}*/


//REFORMULAÇÃO DA DESTRUIÇÃO DE HASHIING, A FUNÇÃO AGORA ACEITA UM PONTEIRO PARA UMA FUNÇÃO DE DESTRUIÇÃO ESPECIFICA,
//QUE SERA CHAMADA PARA CADA ELEMENTO DA TABELA DE HASH
void DestruirHashing(HASHING *H, void (*DestruirElemento)(void *)) {
    if (!H) return;
    NO_CHAVE *P = H->LChaves->Inicio;
    while (P) {
        NO_CHAVE *Seguinte = P->Prox;
        DestruirLista(P->DADOS, DestruirElemento);
        free(P->KEY);
        free(P);
        P = Seguinte;
    }
    free(H);
}



void AddHashing(HASHING *H, PESSOA *P)
{
    if (!H) return;
    if (!H->LChaves) return;
    NO_CHAVE *Key_colocar = FuncaoHashing(H, P);
    if (!Key_colocar)
    {
        Key_colocar = AddCHAVE(H->LChaves, P->CATEGORIA);
    }
    AddInicio(Key_colocar->DADOS, P);
}

void ShowHashingPessoa(HASHING *H)
{
    if (!H) return;
    if (!H->LChaves) return;
    NO_CHAVE *P = H->LChaves->Inicio;
    while (P)
    {
        printf("Key: [%s]\n", P->KEY);

        ShowListaPessoa(P->DADOS);
        P = P->Prox;

    }
}

void ShowHashingLivro(HASHING *H)
{
    if (!H) return;
    if (!H->LChaves) return;
    NO_CHAVE *P = H->LChaves->Inicio;
    while (P)
    {
        printf("____________________________________________________________________________\n\n");
        printf("                     Área      [%s]                               \n", P->KEY);
        //printf("____________________________________________________________________________\n");
        ShowListaLivro(P->DADOS);
        P = P->Prox;

    }
    printf("\n");
}

NO_CHAVE *FuncaoHashing(HASHING *H, PESSOA *X)
{
    if (!H) return NULL;
    if (!H->LChaves) return NULL;

    NO_CHAVE *P = H->LChaves->Inicio;
    while (P)
    {
        if (stricmp(P->KEY, X->CATEGORIA) == 0)
            return P;
        P = P->Prox;
    }
    return NULL;
}

NO_CHAVE *FuncaoHashingLivros(HASHING *H, LIVRO *L){

    if (!H) return NULL;
    if (!H->LChaves) return NULL;

    NO_CHAVE *P = H->LChaves->Inicio;
    while (P)
    {
        if (stricmp(P->KEY, L->AREA) == 0)
            return P;
        P = P->Prox;
    }
    return NULL;
}

NO_CHAVE *FuncaoHashingByID(HASHING *H, const char *key) {
    if (!H) return NULL;
    if (!H->LChaves) return NULL;

    NO_CHAVE *P = H->LChaves->Inicio;
    while (P) {
        if (strcmp(P->KEY, key) == 0)
            return P;
        P = P->Prox;
    }
    return NULL;
}

void AddDistritoHashing(HASHING *H, DISTRITO *distrito) {
    if (!H) return;

    char key[3]; // ID do distrito como chave (assumindo que é um número de 2 dígitos)
    snprintf(key, sizeof(key), "%02d", distrito->ID);

    NO_CHAVE *key_node = FuncaoHashingByID(H, key);
    if (!key_node) {
        key_node = AddCHAVE(H->LChaves, key);
    }

    AddInicio(key_node->DADOS, distrito);
}


void AddConcelhoHashing(HASHING *H, CONCELHO *concelho) {
    if (!H) return;

    char key[5]; // ID do concelho como chave (assumindo que é um número de 4 dígitos)
    snprintf(key, sizeof(key), "%04d", concelho->ID);

    NO_CHAVE *key_node = FuncaoHashingByID(H, key);
    if (!key_node) {
        key_node = AddCHAVE(H->LChaves, key);
    }

    AddInicio(key_node->DADOS, concelho);
}


NO_CHAVE *FuncaoHashingFreguesia(HASHING *H, FREGUESIA *X) {
    if (!H) return NULL;
    if (!H->LChaves) return NULL;

    NO_CHAVE *P = H->LChaves->Inicio;
    while (P) {
        if (strcmp(P->KEY, X->ID) == 0)
            return P;
        P = P->Prox;
    }
    return NULL;
}


void AddInicioFreguesia(LISTA *L, FREGUESIA *X) {
    if (!L) return;
    NO *aux = (NO *)malloc(sizeof(NO));
    aux->Info = X;
    aux->Prox = L->Inicio;
    L->Inicio = aux;
    L->NEL++;
}

void AddHashingFreguesia(HASHING *H, FREGUESIA *freguesia) {
    if (!H) return;
    if (!H->LChaves) return;

    NO_CHAVE *Key_colocar = FuncaoHashingFreguesia(H, freguesia);
    if (!Key_colocar) {
        Key_colocar = AddCHAVE(H->LChaves, freguesia->ID);
    }
    AddInicioFreguesia(Key_colocar->DADOS, freguesia);
}

