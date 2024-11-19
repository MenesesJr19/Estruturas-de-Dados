

#include "Requisicao.h"
void FormatarDataHora(time_t tempo, char *buffer, size_t bufferSize)
{
    struct tm *tm_info = localtime(&tempo);
    strftime(buffer, bufferSize, "%d-%m-%Y %H:%M:%S", tm_info);
}

REQUISICAO *CriarRequisicao(int _id, PESSOA *P, LIVRO *L)
{
        REQUISICAO *nova = (REQUISICAO *)malloc(sizeof(REQUISICAO));
    if (!nova) {
        perror("Erro ao alocar mem�ria para nova requisi��o");
        exit(EXIT_FAILURE);
    }

    nova->ID = _id;
    nova->Ptr_Req = P;
    nova->Ptr_Livro = L;
    time(&nova->data_requisicao);
    nova->data_devolucao=0;
    return nova;
}

void MostrarRequisicao(REQUISICAO *R)
{
    char buffer[20];

    printf("REQ ID = %d\n", R->ID);
    MostrarPessoa(R->Ptr_Req);
    MostrarLivro(R->Ptr_Livro);

    FormatarDataHora(R->data_requisicao, buffer, sizeof(buffer));
    printf("Data de Requisi��o: %s\n", buffer);

    if (R->data_devolucao != 0)
    {
        FormatarDataHora(R->data_devolucao, buffer, sizeof(buffer));
        printf("Data de Devolu��o: %s\n\n", buffer);
    }
    else
    {
        printf("Data de Devolu��o: N�o devolvido\n\n");
    }
}




void DestruirRequisicao(REQUISICAO *R)
{
    //free (R->Ptr_Livro); //OH P� se tirares o comentario vais ter problemas!!!!
    //free (R->Ptr_Req); //OH P� se tirares o comentario vais ter problemas!!!!
    free (R);
}



