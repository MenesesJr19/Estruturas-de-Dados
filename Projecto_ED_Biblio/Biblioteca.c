

#include "Biblioteca.h"
#include <time.h>
#include <ctype.h>
#define CLEAR_SCREEN "cls"

void ConvertToUpperCase(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int is_digits(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Função para validar id_requisitante
int validar_id_requisitante(const char *id) {
    if (strlen(id) != 9 || !is_digits(id)) return 0;

    int soma = 0;
    for (int i = 0; i < 9; i++) {
        soma += id[i] - '0';
    }

    return (soma % 10 == 0);
}

// Função para validar data de nascimento
int validar_data_nasc(const char *data) {
    if (strlen(data) != 10) return 0;

    int dia, mes, ano;
    if (sscanf(data, "%2d-%2d-%4d", &dia, &mes, &ano) != 3) return 0;

    // Verificar validade da data
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 1900 || ano > 2100) return 0;

    // Checar dias por mês
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        if (dia > 30) return 0;
    } else if (mes == 2) {
        // Ano bissexto
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > 28 + bissexto) return 0;
    }

    return 1;
}
NO_CHAVE *FuncaoHashingPorID(HASHING *H, const char *id) {
    if (!H) return NULL;
    if (!H->LChaves) return NULL;

    NO_CHAVE *P = H->LChaves->Inicio;
    while (P) {
        if (strcmp(P->KEY, id) == 0)
            return P;
        P = P->Prox;
    }
    return NULL;
}


int validar_id_freguesia(const char *id, HASHING *HFreguesias) {
    if (strlen(id) != 6) return 0;

    NO_CHAVE *chave = FuncaoHashingPorID(HFreguesias, id);
    return (chave != NULL);
}




// Função para registrar erros no ficheiro log.txt
void registrar_erro(const char *linha, const char *erro) {
    FILE *log = fopen("log.txt", "a");
    if (log) {
        fprintf(log, "Erro: %s - Linha: %s\n", erro, linha);
        fclose(log);
    }
}

/** \brief Aloca Memoria para uma Biblioteca
 *
 * \param _nome char* : Nome da Biblioteca
 * \param _logs char* : Ficheiro dos Logs
 * \return BIBLIOTECA* : Retorna o ponteiro para a biblioteca
 * \author Docentes & Alunos
 * \date   11/04/2024
 *
 */


BIBLIOTECA *CriarBiblioteca(char *_nome, char *_logs)
{
    BIBLIOTECA *Bib = (BIBLIOTECA *)malloc(sizeof(BIBLIOTECA));
    Bib->NOME = (char *)malloc((strlen(_nome) + 1) * sizeof(char));
    strcpy(Bib->NOME, _nome);
    Bib->FICHEIRO_LOGS = (char *)malloc((strlen(_logs) + 1) * sizeof(char));
    strcpy(Bib->FICHEIRO_LOGS, _logs);
    Bib->HLivros = CriarHashing();
    Bib->HRequisitantes = CriarHashing();
    Bib->LRequisicoes = CriarLista();

     Bib->HDistritos = CriarHashing();
    Bib->HConcelhos = CriarHashing();
    Bib->HFreguesias = CriarHashing();

    return Bib;
}


/** \brief Funcao para Mostrar toda a Biblioteca
 *
 * \param B BIBLIOTECA* : Pnteiro para a Biblioteca
 * \return void
 * \author : Docentes e Alunos
 * \date   : 11/04/2024
 */
void ShowBiblioteca(BIBLIOTECA *B)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL) ;
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));
    printf("NOME BIBLIOTECA = [%s]\n", B->NOME);
    // Vosso Codigo.....
    ShowHashingLivro(B->HLivros);

    fclose(F_Logs);
}

void ShowRequisitantes(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));
    printf("NOME BIBLIOTECA = [%s]\n", B->NOME);

    // Usar a função de paginação
    ShowHashingPessoa(B->HRequisitantes);

    fclose(F_Logs);
}





void DestruirDistrito(DISTRITO *distrito) {
    if (distrito) {
        free(distrito->Nome);
        free(distrito);
    }
}

void DestruirConcelho(CONCELHO *concelho) {
    if (concelho) {
        free(concelho->Nome);
        free(concelho);
    }
}

void DestruirFreguesia(FREGUESIA *freguesia) {
    if (freguesia) {
        free(freguesia->ID);
        free(freguesia->Nome);
        free(freguesia);
    }
}

void DestruirBiblioteca(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    if (F_Logs == NULL) {
        perror("Erro ao abrir o arquivo de logs");
        return;
    }

    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    // Liberar memória para nome e ficheiro de logs
    if (B->NOME) free(B->NOME);
    if (B->FICHEIRO_LOGS) free(B->FICHEIRO_LOGS);

    // Destruir hashing de livros
    DestruirHashing(B->HLivros, DestruirLivro);
    // Destruir hashing de requisitantes
    DestruirHashing(B->HRequisitantes, DestruirPessoa);
    // Destruir hashing de distritos
    DestruirHashing(B->HDistritos, DestruirDistrito);
    // Destruir hashing de concelhos
    DestruirHashing(B->HConcelhos, DestruirConcelho);
    // Destruir hashing de freguesias
    DestruirHashing(B->HFreguesias, DestruirFreguesia);
    // Destruir todas as requisições
    NO *current = B->LRequisicoes->Inicio;
    while (current != NULL) {
        NO *next = current->Prox;
        DestruirRequisicao((REQUISICAO *)current->Info);
        free(current);
        current = next;
    }
    // Destruir a lista de requisições em si
    free(B->LRequisicoes);
    // Liberar a estrutura da biblioteca
    free(B);
    printf("Biblioteca destruída com sucesso\n");
    fprintf(F_Logs, "Biblioteca destruída com sucesso em %s\n", ctime(&now));
    fclose(F_Logs);
}



int LoadFicheiroBiblioteca(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    LoadDistritos(B, "distritos.txt");
    //printf("Carreguei Distritos\n");
    LoadConcelhos(B, "concelhos.txt");
    //printf("Carreguei Concelhos\n");
    LoadFreguesias(B, "freguesias.txt");
    //printf("Carreguei Freguesias\n");
    LoadRequisitantes(B, "Requisitantes.txt");
    //printf("Carreguei Requisitantes\n");
    LoadLivros(B, "livros.txt");
    //printf("Carreguei Livros\n");
    //ShowBiblioteca(B);
    LoadRequisicoes(B, "requisicoes.txt");
   // printf("Carreguei Requisicoes\n");

    fclose(F_Logs);
    return EXIT_SUCCESS;
}

int AddLivroBiblioteca(BIBLIOTECA *B, LIVRO *L)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    if (!B || !L) return EXIT_FAILURE;

    // Verificar se já existe um livro com o mesmo ID na mesma área
    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave != NULL) {
        if (strcmp(chave->KEY, L->AREA) == 0) {
            NO *atual = chave->DADOS->Inicio;
            while (atual != NULL) {
                LIVRO *livroExistente = (LIVRO *)atual->Info;
                if (livroExistente->ID == L->ID) {
                    // Livro com o mesmo ID já existe nesta área
                    fprintf(F_Logs, "Livro com ID %d já existe na área %s\n", L->ID, L->AREA);
                    fclose(F_Logs);
                    return EXIT_FAILURE;
                }
                atual = atual->Prox;
            }
        }
        chave = chave->Prox;
    }

    // Adicionar o livro se não existe um livro com o mesmo ID na mesma área
    NO_CHAVE *Key_colocar = FuncaoHashingLivros(B->HLivros, L);
    if (!Key_colocar)
    {
        Key_colocar = AddCHAVE(B->HLivros->LChaves, L->AREA);
    }
    AddInicio(Key_colocar->DADOS, L);

    fprintf(F_Logs, "Livro adicionado: ID=%d, Título=%s, Autor=%s, Área=%s\n", L->ID, L->NOME, L->AUTOR, L->AREA);

    fclose(F_Logs);
    return EXIT_SUCCESS;
}


void RemoverLivroBiblioteca(BIBLIOTECA *B, int id, const char *area) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        if (strcmp(chave->KEY, area) == 0) {
            NO *anterior = NULL;
            NO *atual = chave->DADOS->Inicio;
            while (atual) {
                LIVRO *livro = (LIVRO *)atual->Info;
                if (livro->ID == id) {
                    if (anterior == NULL) {
                        chave->DADOS->Inicio = atual->Prox;
                    } else {
                        anterior->Prox = atual->Prox;
                    }

                    free(livro);
                    free(atual);

                    printf("Livro com ID %d na área %s removido.\n", id, area);
                    fprintf(F_Logs, "Livro com ID %d na área %s removido.\n", id, area);
                    fclose(F_Logs);
                    return;
                }
                anterior = atual;
                atual = atual->Prox;
            }
        }
        chave = chave->Prox;
    }

    printf("Livro com ID %d na área %s não encontrado.\n", id, area);
    fprintf(F_Logs, "Livro com ID %d na área %s não encontrado.\n", id, area);
    fclose(F_Logs);
}



LIVRO *LivroMaisRequisitadoBiblioteca(BIBLIOTECA *B)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    LIVRO *maisRequisitado = NULL;
    int maxRequisicoes = 0;

    NO_CHAVE *P = B->HLivros->LChaves->Inicio;
    while (P) {
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            LIVRO *livro = (LIVRO *)livroAtual->Info;
            int requisicoes = 0;

            // Contar as requisições do livro atual considerando ID e Área
            NO *reqAtual = B->LRequisicoes->Inicio;
            while (reqAtual) {
                REQUISICAO *req = (REQUISICAO *)reqAtual->Info;
                if (req->Ptr_Livro->ID == livro->ID && strcmp(req->Ptr_Livro->AREA, livro->AREA) == 0) {
                    requisicoes++;
                }
                reqAtual = reqAtual->Prox;
            }

            // Verificar se o livro atual é o mais requisitado
            if (requisicoes > maxRequisicoes) {
                maxRequisicoes = requisicoes;
                maisRequisitado = livro;
            }

            livroAtual = livroAtual->Prox;
        }
        P = P->Prox;
    }

    if (maisRequisitado != NULL) {

        printf("Número de requisições: %d\n", maxRequisicoes);
        fprintf(F_Logs, "Livro mais requisitado: ID = %d, Nome = %s, Autor = %s, Área = %s\n",
                maisRequisitado->ID, maisRequisitado->NOME, maisRequisitado->AUTOR, maisRequisitado->AREA);
        fprintf(F_Logs, "Número de requisições: %d\n", maxRequisicoes);
    }

    fclose(F_Logs);
    return maisRequisitado;
}



char *ApelidoMaisComum(BIBLIOTECA *B)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL) ;
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    struct {
        char apelido[50];
        int frequencia;
    } apelidos[100];
    int totalApelidos = 0;

    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;
    while (P) {
        NO *pessoaAtual = P->DADOS->Inicio;
        while (pessoaAtual) {
            PESSOA *pessoa = (PESSOA *)pessoaAtual->Info;
            char *apelido = strrchr(pessoa->NOME, ' ');
            if (apelido) {
                apelido++;
                int encontrado = 0;
                for (int i = 0; i < totalApelidos; i++) {
                    if (strcmp(apelidos[i].apelido, apelido) == 0) {
                        apelidos[i].frequencia++;
                        encontrado = 1;
                        break;
                    }
                }
                if (!encontrado) {
                    strcpy(apelidos[totalApelidos].apelido, apelido);
                    apelidos[totalApelidos].frequencia = 1;
                    totalApelidos++;
                }
            }
            pessoaAtual = pessoaAtual->Prox;
        }
        P = P->Prox;
    }

    int maxFrequencia = 0;
    char *apelidoMaisComum = NULL;
    for (int i = 0; i < totalApelidos; i++) {
        if (apelidos[i].frequencia > maxFrequencia) {
            maxFrequencia = apelidos[i].frequencia;
            apelidoMaisComum = apelidos[i].apelido;
        }
    }

    fclose(F_Logs);
    return apelidoMaisComum;
}


char *AreaMaisComum(BIBLIOTECA *B)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL) ;
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

     struct {
        char area[50];
        int frequencia;
    } areas[100];
    int totalAreas = 0;

    NO_CHAVE *P = B->HLivros->LChaves->Inicio;
    while (P) {
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            LIVRO *livro = (LIVRO *)livroAtual->Info;
            int encontrado = 0;
            for (int i = 0; i < totalAreas; i++) {
                if (strcmp(areas[i].area, livro->AREA) == 0) {
                    areas[i].frequencia++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(areas[totalAreas].area, livro->AREA);
                areas[totalAreas].frequencia = 1;
                totalAreas++;
            }
            livroAtual = livroAtual->Prox;
        }
        P = P->Prox;
    }

    int maxFrequencia = 0;
    char *areaMaisComum = NULL;
    for (int i = 0; i < totalAreas; i++) {
        if (areas[i].frequencia > maxFrequencia) {
            maxFrequencia = areas[i].frequencia;
            areaMaisComum = areas[i].area;
        }
    }

    fclose(F_Logs);
    return areaMaisComum;
}

int IdExiste(BIBLIOTECA *B, int id) {
    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    //printf("Aqui vou fazer [%s]\n",__FUNCTION__);
    while (chave != NULL) {
            NO *atual = chave->DADOS->Inicio;
            //printf("[%d]\n",atual->Info->ID);//mensagem de controlo para verificar se esta a percorrer os Ids.:ESTÁ
        while(atual != NULL){
            PESSOA *idExistente = (PESSOA *)atual->Info;
            if (idExistente->ID == id) {
                    //printf("Id [%d] pertence a :[%s]\nIntroduza outro id\n",id,idExistente->NOME);//mensagem para verificar a quem pertence o ID
            return 1; // ID encontrado
        }
        atual = atual->Prox;
        }
            chave = chave->Prox;
    }
    return 0; // ID não encontrado
}


// Função para calcular o dígito de controle
int CalcularDigitoDeControlo(int *id_parcial) {
    int soma = 0;
    for (int i = 0; i < 8; i++) {
        soma += id_parcial[i];
    }
    return (10 - (soma % 10)) % 10;
}

// Função para gerar um ID de requisitante único
int GerarIdRequisitante(BIBLIOTECA *B) {
    int id_parcial[9];
    int id_final;

    do {
        // Gerar os primeiros 8 dígitos aleatórios
        for (int i = 0; i < 8; i++) {
            id_parcial[i] = rand() % 10;
        }

        // Calcular o dígito de controle
        id_parcial[8] = CalcularDigitoDeControlo(id_parcial);

        // Combinar os dígitos em um único número inteiro
        id_final = 0;
        for (int i = 0; i < 9; i++) {
            id_final = id_final * 10 + id_parcial[i];
        }
    } while (IdExiste(B, id_final)); // Verificar se o ID já existe

    return id_final;
}


int AddRequisitante(BIBLIOTECA *B, PESSOA *X)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL) ;
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

   AddHashing(B->HRequisitantes,X);

    fclose(F_Logs);
    return EXIT_SUCCESS;
}
PESSOA *PesquisarRequisitante(BIBLIOTECA *B, int cod)
{
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL) ;
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while(chave){
        NO *no= chave->DADOS->Inicio;
        while(no){
            if(no->Info->ID == cod){
                fclose(F_Logs);
                return no->Info;
            }
            no = no->Prox;
        }
        chave = chave->Prox;
    }


    fclose(F_Logs);
    return NULL;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

PESSOA *PesquisarRequisitanteNome(BIBLIOTECA *B, char *nome) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    char nome_lower[50];
    strcpy(nome_lower, nome);
    to_lowercase(nome_lower);

    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            char nome_requisitante_lower[50];
            strcpy(nome_requisitante_lower, no->Info->NOME);
            to_lowercase(nome_requisitante_lower);

            if (strcmp(nome_requisitante_lower, nome_lower) == 0) {
                fclose(F_Logs);
                return no->Info;
            }
            no = no->Prox;
        }
        chave = chave->Prox;
    }

    fclose(F_Logs);
    return NULL;
}

LIVRO *PesquisarLivro(BIBLIOTECA *B, int id, const char *area) {
    //printf("%s\n", __FUNCTION__);
    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;

    while (chave != NULL) {
        if (strcmp(chave->KEY, area) == 0) {
            NO *atual = chave->DADOS->Inicio;
            while (atual != NULL) {
                LIVRO *livro = (LIVRO *)atual->Info;
                if (livro->ID == id) {
                    return livro;
                }
                atual = atual->Prox;
            }
        }
        chave = chave->Prox;
    }
    printf("Livro com ID %d não encontrado na área %s\n", id, area);
    return NULL;
}



char* EncontrarFreguesiaH(const char *nome_freguesia, HASHING *HFreguesias)
{
    NO_CHAVE *chave = HFreguesias->LChaves->Inicio;

    while (chave != NULL) {
        NO *atual = chave->DADOS->Inicio;
        while (atual != NULL) {
            FREGUESIA *freguesia = (FREGUESIA *)atual->Info;
            if (strcmp(freguesia->Nome, nome_freguesia) == 0) {
                return freguesia->ID; // Retorna o ID da freguesia encontrada
            }
            atual = atual->Prox;
        }
        chave = chave->Prox;
    }

    return NULL; // Freguesia não encontrada
}


int LivroRequisitado(BIBLIOTECA *B, int livroID) {
    NO *no = B->LRequisicoes->Inicio;
    while (no) {
        REQUISICAO *requisicao = (REQUISICAO *)no->Info;
        if (requisicao->Ptr_Livro->ID == livroID) {
            return 1;
        }
        no = no->Prox;
    }
    return 0;
}

int RemoverLivrosNuncaRequisitados(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    if (!B) return EXIT_FAILURE;

    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        NO *anterior = NULL;
        NO *atual = chave->DADOS->Inicio;
        while (atual) {
            LIVRO *livro = (LIVRO *)atual->Info;
            if (!LivroRequisitado(B, livro->ID)) {
                if (anterior) {
                    anterior->Prox = atual->Prox;
                } else {
                    chave->DADOS->Inicio = atual->Prox;
                }
                DestruirLivro(livro);
                free(atual);
                fprintf(F_Logs, "Removido livro ID=%d, Título=%s\n", livro->ID, livro->NOME);
                // Continue no mesmo nó após remoção
                atual = (anterior) ? anterior->Prox : chave->DADOS->Inicio;
            } else {
                anterior = atual;
                atual = atual->Prox;
            }
        }
        chave = chave->Prox;
    }

    fclose(F_Logs);
    return EXIT_SUCCESS;
}

void ListarLivros(BIBLIOTECA *B) {
    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            LIVRO *livro = (LIVRO *)no->Info;
            printf("ID: %d, Título: %s, Area: %s, Disponível: %d\n",
                   livro->ID, livro->NOME, livro->AREA, livro->disponivel);
            no = no->Prox;
        }
        chave = chave->Prox;
    }
}

DISTRITO *CriarDistrito(int id, const char *nome) {
    DISTRITO *distrito = (DISTRITO *)malloc(sizeof(DISTRITO));
    if (!distrito) {
        printf("Erro ao alocar memória para o distrito.\n");
        exit(EXIT_FAILURE);
    }
    distrito->ID = id;
    distrito->Nome = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (!distrito->Nome) {
        printf("Erro ao alocar memória para o nome do distrito.\n");
        free(distrito);
        exit(EXIT_FAILURE);
    }
    strcpy(distrito->Nome, nome);
    return distrito;
}


CONCELHO *CriarConcelho(int id, const char *nome, int id_distrito) {
    CONCELHO *concelho = (CONCELHO *)malloc(sizeof(CONCELHO));
    if (!concelho) {
        printf("Erro ao alocar memória para o concelho.\n");
        exit(EXIT_FAILURE);
    }
    concelho->ID = id;
    concelho->ID_Distrito = id_distrito;
    concelho->Nome = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (!concelho->Nome) {
        printf("Erro ao alocar memória para o nome do concelho.\n");
        free(concelho);
        exit(EXIT_FAILURE);
    }
    strcpy(concelho->Nome, nome);
    return concelho;
}


FREGUESIA *CriarFreguesia(int id, const char *nome, int id_concelho) {
    FREGUESIA *freguesia = (FREGUESIA *)malloc(sizeof(FREGUESIA));
    if (!freguesia) {
        printf("Erro ao alocar memória para a freguesia.\n");
        exit(EXIT_FAILURE);
    }
    freguesia->ID = id;
    freguesia->ID_Concelho = id_concelho;
    freguesia->Nome = (char *)malloc((strlen(nome) + 1) * sizeof(char));
    if (!freguesia->Nome) {
        printf("Erro ao alocar memória para o nome da freguesia.\n");
        free(freguesia);
        exit(EXIT_FAILURE);
    }
    strcpy(freguesia->Nome, nome);
    return freguesia;
}



void LoadDistritos(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char nome[256];
        if (sscanf(line, "%d %[^\n]", &id, nome) != 2) {
            printf("Erro ao ler a linha do arquivo %s.\n", filename);
            continue;
        }

        DISTRITO *distrito = CriarDistrito(id, nome);
        AddDistritoHashing(B->HDistritos, distrito);
    }

    fclose(file);
}


void LoadConcelhos(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char nome[256];
        if (sscanf(line, "%d %[^\t\n]", &id, nome) != 2) {
            printf("Erro ao ler a linha do arquivo %s.\n", filename);
            continue;
        }

        // Extrair o ID do distrito a partir do ID do concelho
        int id_distrito = id / 100;  // Assumindo que os dois primeiros dígitos representam o distrito

        CONCELHO *concelho = CriarConcelho(id, nome, id_distrito);
        AddConcelhoHashing(B->HConcelhos, concelho);
    }

    fclose(file);
}


void LoadFreguesias(BIBLIOTECA *biblioteca, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", filename);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        int id;
        char nome[100];
        if (sscanf(linha, "%6d\t%99[^\n]", &id, nome) != 2) {
            continue;
        }

        FREGUESIA *freguesia = (FREGUESIA *)malloc(sizeof(FREGUESIA));
        freguesia->ID = (char *)malloc(7 * sizeof(char));
        snprintf(freguesia->ID, 7, "%06d", id);
        freguesia->Nome = strdup(nome);

        AddHashingFreguesia(biblioteca->HFreguesias, freguesia);
    }

    fclose(file);
}




void ListarDistritos(HASHING *HDistritos) {
    if (!HDistritos || !HDistritos->LChaves) return;
    NO_CHAVE *P = HDistritos->LChaves->Inicio;

    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            DISTRITO *distrito = (DISTRITO *)aux->Info;
            printf("ID: %02d, Nome: %s\n", distrito->ID, distrito->Nome);
            aux = aux->Prox;
        }
        P = P->Prox;
    }
}


void ListarConcelhos(HASHING *HConcelhos) {
    if (!HConcelhos || !HConcelhos->LChaves) return;
    NO_CHAVE *P = HConcelhos->LChaves->Inicio;

    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            CONCELHO *concelho = (CONCELHO *)aux->Info;
            printf("ID: %04d, Nome: %s, ID Distrito: %02d\n", concelho->ID, concelho->Nome, concelho->ID_Distrito);
            aux = aux->Prox;
        }
        P = P->Prox;
    }
}

void ListarFreguesias(HASHING *HFreguesias) {
    if (!HFreguesias || !HFreguesias->LChaves) return;
    NO_CHAVE *P = HFreguesias->LChaves->Inicio;

    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            FREGUESIA *freguesia = (FREGUESIA *)aux->Info;
            printf("ID: %06d, Nome: %s, ID Concelho: %04d\n", freguesia->ID, freguesia->Nome, freguesia->ID_Concelho);
            aux = aux->Prox;
        }
        P = P->Prox;
    }
}

void ListarTodosDistritos(BIBLIOTECA *B) {
    printf("Distritos:\n");
    ListarDistritos(B->HDistritos);
}

void ListarTodosConcelhos(BIBLIOTECA *B) {
    printf("Concelhos:\n");
    ListarConcelhos(B->HConcelhos);
}

void ListarTodasFreguesias(BIBLIOTECA *B) {
    printf("Freguesias:\n");
    ListarFreguesias(B->HFreguesias);
}





void LoadRequisitantes(BIBLIOTECA *biblioteca, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o ficheiro %s\n", filename);
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), file)) {
        char id[10], nome[100], data_nasc[11], id_freguesia[7];
        if (sscanf(linha, "%9s\t%99[^\t]\t%10s\t%6s", id, nome, data_nasc, id_freguesia) != 4) {
            registrar_erro(linha, "Formato inválido");
            continue;
        }

        if (!validar_id_requisitante(id)) {
            registrar_erro(linha, "ID Requisitante inválido");
            continue;
        }

        if (!validar_data_nasc(data_nasc)) {
            registrar_erro(linha, "Data de nascimento inválida");
            continue;
        }

        if (!validar_id_freguesia(id_freguesia, biblioteca->HFreguesias)) {
            registrar_erro(linha, "ID Freguesia inválido");
            continue;
        }

        // Criar e adicionar requisitante à biblioteca
        PESSOA *pessoa = (PESSOA *)malloc(sizeof(PESSOA));
        pessoa->ID = atoi(id);
        pessoa->NOME = strdup(nome);
        pessoa->DATA_NASC = strdup(data_nasc);
        pessoa->CATEGORIA = strdup(id_freguesia);

        AddHashing(biblioteca->HRequisitantes, pessoa);
    }

    fclose(file);
}


void LoadLivros(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", filename);
        return;
    }

    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    fprintf(F_Logs, "Carregando livros do ficheiro %s\n", filename);

    int id;
    char titulo[100];
    char autor[100];
    char area[50];
    int disponivel;

    while (fscanf(file, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%d\n", &id, titulo, autor, area, &disponivel) == 5) {
        LIVRO *livro = CriarLivro(id, titulo, autor, area);
        livro->disponivel = disponivel;
        AddLivroBiblioteca(B, livro);
        fprintf(F_Logs, "Livro lido: ID=%d, Título=%s, Autor=%s, Área=%s, Disponível=%d\n", id, titulo, autor, area, disponivel);
    }

    fclose(file);
    fclose(F_Logs);
}



int strptime_custom(const char *str, struct tm *tm_info) {
    return sscanf(str, "%d-%d-%d %d:%d:%d",
                  &tm_info->tm_mday, &tm_info->tm_mon, &tm_info->tm_year,
                  &tm_info->tm_hour, &tm_info->tm_min, &tm_info->tm_sec) == 6;
}

void ConverterTimeParaString(time_t time_value, char *buffer, size_t buffer_size) {
    struct tm *tm_info;
    tm_info = localtime(&time_value);
    strftime(buffer, buffer_size, "%d-%m-%Y %H:%M:%S", tm_info);
}
// Converte string no formato "dd-MM-yyyy HH:mm:ss" para time_t
time_t ConverterStringParaTime(const char *str) {
    struct tm tm_info;
    memset(&tm_info, 0, sizeof(struct tm));
    if (strptime_custom(str, &tm_info)) {
        tm_info.tm_year -= 1900; // Ajusta o ano para time_t
        tm_info.tm_mon -= 1; // Ajusta o mês para time_t
        tm_info.tm_isdst = -1; // Para determinar o horário de verão
        time_t t = mktime(&tm_info);
        char buffer[20];
        ConverterTimeParaString(t, buffer, sizeof(buffer));  // Converte para string para verificação
        //printf("ConverterStringParaTime - str: %s, result: %s\n", str, buffer);  // MENSAGEM DEPURAÇÃO
        return t;
    }
    return -1;
}




void LoadRequisicoes(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro %s\n", filename);
        return;
    }

    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    fprintf(F_Logs, "Carregando requisicoes do ficheiro %s\n", filename);

    int id, reqId, livroId;
    char bufferReq[20];
    char bufferDev[20];
    char area[50];

    while (fscanf(file, "%d\t%d\t%d\t%s\t%[^\t]\t%[^\n]", &id, &reqId, &livroId, area, bufferReq, bufferDev) == 6) {
        PESSOA *requisitante = PesquisarRequisitante(B, reqId);
        LIVRO *livro = PesquisarLivro(B, livroId, area);

        if (requisitante && livro) {
            REQUISICAO *requisicao = CriarRequisicao(id, requisitante, livro);

            requisicao->data_requisicao = ConverterStringParaTime(bufferReq);

            if (strcmp(bufferDev, "Não devolvido") != 0) {
                requisicao->data_devolucao = ConverterStringParaTime(bufferDev);
            } else {
                requisicao->data_devolucao = 0;
            }

            AddInicio(B->LRequisicoes, requisicao);
            fprintf(F_Logs, "Requisicao lida: ID=%d, ReqID=%d, LivroID=%d, Area=%s, DataReq=%s, DataDev=%s\n", id, reqId, livroId, area, bufferReq, bufferDev);
        } else {
            fprintf(F_Logs, "Erro: Requisitante ou livro não encontrado para a requisicao ID=%d\n", id);
        }
    }

    fclose(file);
    fclose(F_Logs);
}


void SalvarRequisicoes(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar requisicoes");
        return;
    }

    NO *atual = B->LRequisicoes->Inicio;
    while (atual) {
        REQUISICAO *req = (REQUISICAO *)atual->Info;
        char bufferReq[20];
        char bufferDev[20];

        ConverterTimeParaString(req->data_requisicao, bufferReq, sizeof(bufferReq));
        if (req->data_devolucao != 0) {
            ConverterTimeParaString(req->data_devolucao, bufferDev, sizeof(bufferDev));
        } else {
            strcpy(bufferDev, "Não devolvido");
        }

        fprintf(file, "%d\t%d\t%d\t%s\t%s\t%s\n", req->ID, req->Ptr_Req->ID, req->Ptr_Livro->ID, req->Ptr_Livro->AREA, bufferReq, bufferDev);
        atual = atual->Prox;
    }

    fclose(file);
}







void SalvarLivros(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar livros");
        return;
    }

    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            LIVRO *livro = (LIVRO *)no->Info;
            fprintf(file, "%d\t%s\t%s\t%s\t%d\n", livro->ID, livro->NOME, livro->AUTOR, livro->AREA, livro->disponivel);
            no = no->Prox;
        }
        chave = chave->Prox;
    }

    fclose(file);
}

void SalvarRequisitantes(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar requisitantes");
        return;
    }

    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            PESSOA *pessoa = no->Info;
            fprintf(file, "%d\t%s\t%s\t%s\n", pessoa->ID, pessoa->NOME, pessoa->DATA_NASC, pessoa->CATEGORIA);
            no = no->Prox;
        }
        chave = chave->Prox;
    }

    fclose(file);
}

void SalvarRequisitantesCSV(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar requisitantes em CSV");
        return;
    }

    // Cabeçalho do CSV
    fprintf(file, "ID,Nome,Data de Nascimento,Categoria\n");

    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            PESSOA *pessoa = no->Info;
            fprintf(file, "%d,%s,%s,%s\n", pessoa->ID, pessoa->NOME, pessoa->DATA_NASC, pessoa->CATEGORIA);
            no = no->Prox;
        }
        chave = chave->Prox;
    }

    fclose(file);
    printf("Requisitantes salvos em %s\n", filename);
}

void SalvarRequisicoesCSV(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar requisições em CSV");
        return;
    }

    // Cabeçalho do CSV
    fprintf(file, "ID,ID Requisitante,ID Livro,Área,Data Requisição,Data Devolução\n");

    NO *atual = B->LRequisicoes->Inicio;
    while (atual) {
        REQUISICAO *req = (REQUISICAO *)atual->Info;
        char bufferReq[20];
        char bufferDev[20];

        ConverterTimeParaString(req->data_requisicao, bufferReq, sizeof(bufferReq));
        if (req->data_devolucao != 0) {
            ConverterTimeParaString(req->data_devolucao, bufferDev, sizeof(bufferDev));
        } else {
            strcpy(bufferDev, "Não devolvido");
        }

        fprintf(file, "%d,%d,%d,%s,%s,%s\n", req->ID, req->Ptr_Req->ID, req->Ptr_Livro->ID, req->Ptr_Livro->AREA, bufferReq, bufferDev);
        atual = atual->Prox;
    }

    fclose(file);
    printf("Requisições salvas em %s\n", filename);
}

void SalvarLivrosCSV(BIBLIOTECA *B, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro para salvar livros em CSV");
        return;
    }

    // Cabeçalho do CSV
    fprintf(file, "ID,Nome,Autor,Área,Disponível\n");

    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            LIVRO *livro = (LIVRO *)no->Info;
            fprintf(file, "%d,%s,%s,%s,%d\n", livro->ID, livro->NOME, livro->AUTOR, livro->AREA, livro->disponivel);
            no = no->Prox;
        }
        chave = chave->Prox;
    }

    fclose(file);
    printf("Livros salvos em %s\n", filename);
}




void SalvarBiblioteca(BIBLIOTECA *B) {

    //Guardar em formato .txt
    SalvarLivros(B, "livros.txt");
    SalvarRequisitantes(B, "requisitantes.txt");
    SalvarRequisicoes(B, "requisicoes.txt");

    //Guardar os ficheiros no formato .csv
    SalvarLivrosCSV(B, "livros.csv");
    SalvarRequisitantesCSV(B,"requisitantes.csv");
    SalvarRequisicoesCSV(B, "requisicoes.csv");
}







void AdicionarRequisicao(BIBLIOTECA *B, REQUISICAO *req)
{
    NO *novoNo = (NO *)malloc(sizeof(NO));
    if (novoNo == NULL)
    {
        printf("Erro ao alocar memoria para nova requisicao\n");
        return;
    }

    novoNo->Info = (PESSOA *)req;  // Armazena a requisição em vez de uma pessoa
    novoNo->Prox = B->LRequisicoes->Inicio;
    B->LRequisicoes->Inicio = novoNo;
    B->LRequisicoes->NEL++;
}

LIVRO *PesquisarLivroPorArea(BIBLIOTECA *B, int id, const char *area) {
    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;

    while (chave != NULL) {
        if (strcmp(chave->KEY, area) == 0) {
            NO *atual = chave->DADOS->Inicio;
            while (atual != NULL) {
                LIVRO *livro = (LIVRO *)atual->Info;
                if (livro->ID == id) {
                    return livro;
                }
                atual = atual->Prox;
            }
        }
        chave = chave->Prox;
    }
    return NULL;
}


int CriarRequisicaoBiblioteca(BIBLIOTECA *B, int idReq, int idLivro, const char *area)
{
    PESSOA *req = PesquisarRequisitante(B, idReq);
    LIVRO *livro = PesquisarLivroPorArea(B, idLivro, area);

    if (req == NULL)
    {
        printf("Requisitante não encontrado\n");
        return 0;
    }
    if (livro == NULL)
    {
        printf("Livro não encontrado\n");
        return 0;
    }
    if (livro->disponivel == 0)
    {
        printf("Livro já está requisitado\n");
        return 0;
    }

    REQUISICAO *newReq = CriarRequisicao(rand(), req, livro);
    AdicionarRequisicao(B, newReq);
    livro->disponivel = 0; // Define o livro como indisponível

    return 1;
}


void ListarRequisicoes(BIBLIOTECA *B) {
    NO *atual = B->LRequisicoes->Inicio;
    while (atual != NULL) {
        REQUISICAO *requisicao = (REQUISICAO *)atual->Info;
        char *areaAtual = requisicao->Ptr_Livro->AREA;
        int encontrada = 0;

        // Verificar se a área já foi listada
        NO *tmp = B->LRequisicoes->Inicio;
        while (tmp != atual) {
            REQUISICAO *reqTmp = (REQUISICAO *)tmp->Info;
            if (strcmp(reqTmp->Ptr_Livro->AREA, areaAtual) == 0) {
                encontrada = 1;
                break;
            }
            tmp = tmp->Prox;
        }

        // Se a área não foi listada, listar as requisições dessa área
        if (!encontrada) {
            //printf("||--------------------------||--------------------------||\n");
            printf("||--------Área--------||--------%s--------||--------------------------||\n", areaAtual);
            NO *innerAtual = B->LRequisicoes->Inicio;
            while (innerAtual != NULL) {
                REQUISICAO *innerRequisicao = (REQUISICAO *)innerAtual->Info;
                if (strcmp(innerRequisicao->Ptr_Livro->AREA, areaAtual) == 0) {
                    MostrarRequisicao(innerRequisicao);
                }
                innerAtual = innerAtual->Prox;
            }
        }
        atual = atual->Prox;
    }
}

int RegistarDevolucao(BIBLIOTECA *B, int idReq, int idLivro, const char *area)
{
    NO *atual = B->LRequisicoes->Inicio;
    int encontrouRequisicao = 0;

    while (atual != NULL) {
        REQUISICAO *requisicao = (REQUISICAO *)atual->Info;
        if (requisicao->Ptr_Req->ID == idReq && requisicao->Ptr_Livro->ID == idLivro && strcmp(requisicao->Ptr_Livro->AREA, area) == 0) {
            encontrouRequisicao = 1;
            if (requisicao->data_devolucao == 0) {
                time(&requisicao->data_devolucao);
                requisicao->Ptr_Livro->disponivel = 1; // o livro volta a estar disponivel
                printf("Devolução registrada com sucesso.\n");
                return 1;
            }
        }
        atual = atual->Prox;
    }

    if (encontrouRequisicao) {
        printf("O livro já foi devolvido anteriormente.\n");
    } else {
        printf("Requisição não encontrada.\n");
    }
    return 0;
}


void MostrarLivrosRequisitadosPorPessoa(BIBLIOTECA *B, PESSOA *P) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    int encontrou = 0;
    NO *noReq = B->LRequisicoes->Inicio;
    while (noReq) {
        REQUISICAO *req = (REQUISICAO *)noReq->Info;
        if (req->Ptr_Req->ID == P->ID) {

            MostrarLivro(req->Ptr_Livro);
        // Mostrar data e hora de requisição
            char buffer[30];
            struct tm *tm_info;

            tm_info = localtime(&req->data_requisicao);
            strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
            printf("Data e hora de requisição: %s\n", buffer);

            // Mostrar data e hora de devolução, se houver
            if (req->data_devolucao != 0) {
                tm_info = localtime(&req->data_devolucao);
                strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
                printf("Data e hora de devolução: %s\n\n", buffer);
            } else {
                printf("Ainda não devolvido.\n\n");
            }
            encontrou = 1;
        }
        noReq = noReq->Prox;
    }

    if (!encontrou) {
        printf("Nenhum livro requisitado por %s.\n", P->NOME);
    }

    fclose(F_Logs);
}

void MostrarPessoasNuncaRequisitaram(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *noReq = chave->DADOS->Inicio;
        while (noReq) {
            PESSOA *pessoa = noReq->Info;
            int requisitou = 0;

            NO *noReqCheck = B->LRequisicoes->Inicio;
            while (noReqCheck) {
                REQUISICAO *req = (REQUISICAO *)noReqCheck->Info;
                if (req->Ptr_Req->ID == pessoa->ID) {
                    requisitou = 1;
                    break;
                }
                noReqCheck = noReqCheck->Prox;
            }

            if (!requisitou) {
                MostrarPessoa(pessoa);
            }

            noReq = noReq->Prox;
        }
        chave = chave->Prox;
    }

    fclose(F_Logs);
}

void MostrarRequisitantesComLivrosRequisitados(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO *noReq = B->LRequisicoes->Inicio;
    while (noReq) {
        REQUISICAO *req = (REQUISICAO *)noReq->Info;
        if (req->data_devolucao == 0) {
            MostrarPessoa(req->Ptr_Req);
        }
        noReq = noReq->Prox;
    }

    fclose(F_Logs);
}

void ListarRequisitantes(BIBLIOTECA *B) {
    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            PESSOA *requisitante = (PESSOA *)no->Info;
            char nomeUpper[50];
            strcpy(nomeUpper, requisitante->NOME);
            ConvertToUpperCase(nomeUpper);
            printf("ID: %d, Nome: %s, Data de Nascimento: %s, Freguesia: %s\n",
                   requisitante->ID, nomeUpper, requisitante->DATA_NASC, requisitante->CATEGORIA);
            no = no->Prox;
        }
        chave = chave->Prox;
    }
}


void AreaMaisComumBiblioteca(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO_CHAVE *P = B->HLivros->LChaves->Inicio;
    NO_CHAVE *areaMaisComum = NULL;
    int maxLivros = 0;

    // Contar o número de livros em cada área
    while (P) {
        int contagemLivros = 0;
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            contagemLivros++;
            livroAtual = livroAtual->Prox;
        }

        // Verificar se esta área tem mais livros do que a atual máxima
        if (contagemLivros > maxLivros) {
            maxLivros = contagemLivros;
            areaMaisComum = P;
        }
        P = P->Prox;
    }

    // Verificar outras áreas que têm o mesmo número de livros que o máximo
    P = B->HLivros->LChaves->Inicio;
    fprintf(F_Logs, "Áreas com o maior número de livros (%d):\n", maxLivros);
    printf("Áreas com o maior número de livros (%d):\n", maxLivros);
    while (P) {
        int contagemLivros = 0;
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            contagemLivros++;
            livroAtual = livroAtual->Prox;
        }

        // Se esta área tem o mesmo número de livros que o máximo, registrar
        if (contagemLivros == maxLivros) {
            fprintf(F_Logs, "Área: %s, Número de livros: %d\n", P->KEY, contagemLivros);
            printf("Área: [%s], Número de livros: [%d]\n", P->KEY, contagemLivros);
        }
        P = P->Prox;
    }

    fclose(F_Logs);
}

void AreaMaisRequisitadaBiblioteca(BIBLIOTECA *B) {
    FILE *F_Logs = fopen(B->FICHEIRO_LOGS, "a");
    time_t now = time(NULL);
    fprintf(F_Logs, "Entrei em %s na data %s\n", __FUNCTION__, ctime(&now));

    NO_CHAVE *P = B->HLivros->LChaves->Inicio;
    NO_CHAVE *areaMaisRequisitada = NULL;
    int maxRequisicoes = 0;

    // Contar o número de requisições em cada área
    while (P) {
        int contagemRequisicoes = 0;
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            LIVRO *livro = (LIVRO *)livroAtual->Info;
            NO *reqAtual = B->LRequisicoes->Inicio;
            while (reqAtual) {
                REQUISICAO *req = (REQUISICAO *)reqAtual->Info;
                if (strcmp(req->Ptr_Livro->AREA, livro->AREA) == 0 && req->Ptr_Livro->ID == livro->ID) {
                    contagemRequisicoes++;
                }
                reqAtual = reqAtual->Prox;
            }
            livroAtual = livroAtual->Prox;
        }

        // Verificar se esta área tem mais requisições do que a atual máxima
        if (contagemRequisicoes > maxRequisicoes) {
            maxRequisicoes = contagemRequisicoes;
            areaMaisRequisitada = P;
        }
        P = P->Prox;
    }

    // Verificar outras áreas que têm o mesmo número de requisições que o máximo
    P = B->HLivros->LChaves->Inicio;
    fprintf(F_Logs, "Áreas com o maior número de requisições (%d):\n", maxRequisicoes);
    printf("Áreas com o maior número de requisições (%d):\n", maxRequisicoes);
    while (P) {
        int contagemRequisicoes = 0;
        NO *livroAtual = P->DADOS->Inicio;
        while (livroAtual) {
            LIVRO *livro = (LIVRO *)livroAtual->Info;
            NO *reqAtual = B->LRequisicoes->Inicio;
            while (reqAtual) {
                REQUISICAO *req = (REQUISICAO *)reqAtual->Info;
                if (strcmp(req->Ptr_Livro->AREA, livro->AREA) == 0 && req->Ptr_Livro->ID == livro->ID) {
                    contagemRequisicoes++;
                }
                reqAtual = reqAtual->Prox;
            }
            livroAtual = livroAtual->Prox;
        }

        // Se esta área tem o mesmo número de requisições que o máximo, registrar
        if (contagemRequisicoes == maxRequisicoes) {
            fprintf(F_Logs, "Área: %s, Número de requisições: %d\n", P->KEY, contagemRequisicoes);
            printf("Área: %s, Número de requisições: %d\n", P->KEY, contagemRequisicoes);
        }
        P = P->Prox;
    }

    fclose(F_Logs);
}


// Função para contar o número total de páginas
int contarTotalPaginas(int numRequisitantes, int requisitantesPorPagina) {
    return (numRequisitantes + requisitantesPorPagina - 1) / requisitantesPorPagina;
}

// Função para listar os requisitantes por página
void listarRequisitantesPorPagina(BIBLIOTECA *biblioteca, int requisitantesPorPagina) {
    if (!biblioteca || !biblioteca->HRequisitantes || !biblioteca->HRequisitantes->LChaves) return;

    int numRequisitantes = 0;
    // Contando o número total de requisitantes
    NO_CHAVE *chaveAtual = biblioteca->HRequisitantes->LChaves->Inicio;
    while (chaveAtual) {
        numRequisitantes += chaveAtual->DADOS->NEL;
        chaveAtual = chaveAtual->Prox;
    }

    int totalPaginas = contarTotalPaginas(numRequisitantes, requisitantesPorPagina);
    int paginaAtual = 1;
    char opcao[10];

    while (1) {
        system(CLEAR_SCREEN);
        printf("Número de requisitantes: %d\n", numRequisitantes);
        printf("Total de páginas: %d\n", totalPaginas);
        printf("\nPágina %d de %d\n", paginaAtual, totalPaginas);
        printf("-------------------\n");

        int inicio = (paginaAtual - 1) * requisitantesPorPagina;
        int fim = inicio + requisitantesPorPagina;
        if (fim > numRequisitantes) {
            fim = numRequisitantes;
        }

        printf("Mostrando requisitantes de %d a %d\n", inicio + 1, fim);

        int contador = 0;
        chaveAtual = biblioteca->HRequisitantes->LChaves->Inicio;

        while (chaveAtual) {
            NO *noAtual = chaveAtual->DADOS->Inicio;
            while (noAtual) {
                if (contador >= inicio && contador < fim) {
                    PESSOA *requisitante = (PESSOA *)noAtual->Info;
                    char nomeUpper[50];
                    strcpy(nomeUpper, requisitante->NOME);
                    ConvertToUpperCase(nomeUpper);
                    printf("[ID: %d] ", requisitante->ID);
                    printf("[Nome: %s] ", nomeUpper);
                    printf("[Categoria: %s]\n", requisitante->CATEGORIA);
                    printf("--------------------------------------------\n");
                }
                contador++;
                noAtual = noAtual->Prox;
            }
            chaveAtual = chaveAtual->Prox;
        }

        printf("\n[|--------------------------------------------|]\n");
        printf("Número de requisitantes: %d\n", numRequisitantes);
        printf("Total de páginas: %d\n", totalPaginas);
        printf("\nPágina %d de %d\n", paginaAtual, totalPaginas);
        printf("\n[|--------------------------------------------|]\n");
        printf("n - próxima página, p - página anterior, q - sair, [1-%d] - ir para página: ", totalPaginas);
        scanf("%s", opcao);

        if (strcmp(opcao, "n") == 0) {
            if (paginaAtual < totalPaginas) {
                paginaAtual++;
            } else {
                printf("Você já está na última página.\n");
            }
        } else if (strcmp(opcao, "p") == 0) {
            if (paginaAtual > 1) {
                paginaAtual--;
            } else {
                printf("Você já está na primeira página.\n");
            }
        } else if (strcmp(opcao, "q") == 0) {
            printf("Saindo...\n");
            break;
        } else {
            int paginaEscolhida = atoi(opcao);
            if (paginaEscolhida >= 1 && paginaEscolhida <= totalPaginas) {
                paginaAtual = paginaEscolhida;
            } else {
                printf("Página inválida.\n");
            }
        }
    }
}



int CalcularIdade(char *data_nasc) {
    int dia, mes, ano;
    sscanf(data_nasc, "%d-%d-%d", &dia, &mes, &ano);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int idade = tm.tm_year + 1900 - ano;
    if (tm.tm_mon + 1 < mes || (tm.tm_mon + 1 == mes && tm.tm_mday < dia)) {
        idade--;
    }
    return idade;
}


int DeterminarIdadeMaxima(BIBLIOTECA *B) {
    int idadeMaxima = 0;
    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;
    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;
            int idade = CalcularIdade(requisitante->DATA_NASC);
            if (idade > idadeMaxima) {
                idadeMaxima = idade;
            }
            aux = aux->Prox;
        }
        P = P->Prox;
    }
    return idadeMaxima;
}





double DeterminarMediaIdades(BIBLIOTECA *B) {
    int totalIdade = 0;
    int contador = 0;
    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;




    while (P) {
        NO *aux = P->DADOS->Inicio;

        // Debugging: Print current node info
        /*
        printf("Percorrendo requisitantes...\n");
        printf("P->DADOS->Inicio: %p\n", (void *)P->DADOS->Inicio);
        */
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;

            // Debugging: Print person info
            //printf("Processando requisitante: %s\n", requisitante->NOME);

            int idade = CalcularIdade(requisitante->DATA_NASC);

            // Debugging: Print calculated age
            //printf("Idade calculada: %d\n", idade);

            totalIdade += idade;
            contador++;
            aux = aux->Prox;
        }
        P = P->Prox;
    }

    // Calculating the average
    double media = contador ? (double)totalIdade / contador : 0.0;

    // Debugging: Print total age and count
    printf("Total de idades: %d\n", totalIdade);
    printf("Número de requisitantes: %d\n", contador);
    printf("Média de idades: %.2f\n", media);

    return media;
}


int ContarPessoasIdadeSuperior(BIBLIOTECA *B, int idadeMinima) {
    int contador = 0;
    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;
    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;
            int idade = CalcularIdade(requisitante->DATA_NASC);
            if (idade > idadeMinima) {
                contador++;
            }
            aux = aux->Prox;
        }
        P = P->Prox;
    }
    return contador;
}



int DeterminarIdadeComMaisRequisitantes(BIBLIOTECA *B) {
    int frequencia[150] = {0}; // Assumindo que ninguém tem mais de 150 anos
    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;
    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;
            int idade = CalcularIdade(requisitante->DATA_NASC);
            frequencia[idade]++;
            aux = aux->Prox;
        }
        P = P->Prox;
    }

    int maxFrequencia = 0;
    int idadeComMaisRequisitantes = 0;
    for (int i = 0; i < 150; i++) {
        if (frequencia[i] > maxFrequencia) {
            maxFrequencia = frequencia[i];
            idadeComMaisRequisitantes = i;
        }
    }

    return idadeComMaisRequisitantes;
}


void DeterminarSobrenomeMaisUsado(BIBLIOTECA *B) {
    SOBRENOME_COUNT sobrenomes[1000];  // Presumindo um máximo de 1000 sobrenomes diferentes
    int totalSobrenomes = 0;

    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;
    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;
            char *sobrenome = strrchr(requisitante->NOME, ' ');
            if (sobrenome != NULL) {
                sobrenome++;
            } else {
                sobrenome = requisitante->NOME;
            }

            int found = 0;
            for (int i = 0; i < totalSobrenomes; i++) {
                if (strcmp(sobrenomes[i].sobrenome, sobrenome) == 0) {
                    sobrenomes[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(sobrenomes[totalSobrenomes].sobrenome, sobrenome);
                sobrenomes[totalSobrenomes].count = 1;
                totalSobrenomes++;
            }

            aux = aux->Prox;
        }
        P = P->Prox;
    }

    int maxCount = 0;
    char *sobrenomeMaisUsado = NULL;
    for (int i = 0; i < totalSobrenomes; i++) {
        if (sobrenomes[i].count > maxCount) {
            maxCount = sobrenomes[i].count;
            sobrenomeMaisUsado = sobrenomes[i].sobrenome;
        }
    }

    if (sobrenomeMaisUsado) {
        printf("Sobrenome mais usado: %s (usado por %d requisitantes)\n", sobrenomeMaisUsado, maxCount);
    } else {
        printf("Nenhum sobrenome encontrado.\n");
    }
}







void ListarRequisitantesNascidosEmDomingo(BIBLIOTECA *B) {
    NO_CHAVE *P = B->HRequisitantes->LChaves->Inicio;

    while (P) {
        NO *aux = P->DADOS->Inicio;
        while (aux) {
            PESSOA *requisitante = (PESSOA *)aux->Info;

            int dia, mes, ano;
            sscanf(requisitante->DATA_NASC, "%d-%d-%d", &dia, &mes, &ano);
            struct tm timeinfo = {0};
            timeinfo.tm_year = ano - 1900;
            timeinfo.tm_mon = mes - 1;
            timeinfo.tm_mday = dia;
            char nomeUpper[50];
            strcpy(nomeUpper, requisitante->NOME);
            ConvertToUpperCase(nomeUpper);
            if (mktime(&timeinfo) != -1 && timeinfo.tm_wday == 0) {  // 0 representa domingo
                printf("ID: %d, Nome: %s, Freguesia: %s, Data de Nascimento: %s\n",
                       requisitante->ID, nomeUpper, requisitante->CATEGORIA, requisitante->DATA_NASC);
            }

            aux = aux->Prox;
        }
        P = P->Prox;
    }
}




size_t CalcularMemoriaBiblioteca(BIBLIOTECA *B) {
    size_t memoria_total = sizeof(BIBLIOTECA);

    memoria_total += strlen(B->NOME) + 1;
    memoria_total += strlen(B->FICHEIRO_LOGS) + 1;

    // Função auxiliar para calcular a memória ocupada por uma lista
    size_t CalcularMemoriaLista(LISTA *L, size_t (*CalcularMemoriaElemento)(void *)) {
        size_t memoria = sizeof(LISTA);
        NO *atual = L->Inicio;
        while (atual) {
            memoria += sizeof(NO);
            memoria += CalcularMemoriaElemento(atual->Info);
            atual = atual->Prox;
        }
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por um hashing
    size_t CalcularMemoriaHashing(HASHING *H, size_t (*CalcularMemoriaElemento)(void *)) {
        size_t memoria = sizeof(HASHING);
        NO_CHAVE *atual_chave = H->LChaves->Inicio;
        while (atual_chave) {
            memoria += sizeof(NO_CHAVE);
            memoria += strlen(atual_chave->KEY) + 1;
            memoria += CalcularMemoriaLista(atual_chave->DADOS, CalcularMemoriaElemento);
            atual_chave = atual_chave->Prox;
        }
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por um livro
    size_t CalcularMemoriaLivro(void *elemento) {
        LIVRO *livro = (LIVRO *)elemento;
        size_t memoria = sizeof(LIVRO);
        memoria += strlen(livro->NOME) + 1;
        memoria += strlen(livro->AUTOR) + 1;
        memoria += strlen(livro->AREA) + 1;
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por uma pessoa
    size_t CalcularMemoriaPessoa(void *elemento) {
        PESSOA *pessoa = (PESSOA *)elemento;
        size_t memoria = sizeof(PESSOA);
        memoria += strlen(pessoa->NOME) + 1;
        memoria += strlen(pessoa->CATEGORIA) + 1;
        memoria += strlen(pessoa->DATA_NASC) + 1;
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por uma requisição
    size_t CalcularMemoriaRequisicao(void *elemento) {
        return sizeof(REQUISICAO);
    }

    // Função auxiliar para calcular a memória ocupada por um distrito
    size_t CalcularMemoriaDistrito(void *elemento) {
        DISTRITO *distrito = (DISTRITO *)elemento;
        size_t memoria = sizeof(DISTRITO);
        memoria += strlen(distrito->Nome) + 1;
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por um concelho
    size_t CalcularMemoriaConcelho(void *elemento) {
        CONCELHO *concelho = (CONCELHO *)elemento;
        size_t memoria = sizeof(CONCELHO);
        memoria += strlen(concelho->Nome) + 1;
        return memoria;
    }

    // Função auxiliar para calcular a memória ocupada por uma freguesia
    size_t CalcularMemoriaFreguesia(void *elemento) {
        FREGUESIA *freguesia = (FREGUESIA *)elemento;
        size_t memoria = sizeof(FREGUESIA);
        memoria += strlen(freguesia->ID) + 1;
        memoria += strlen(freguesia->Nome) + 1;
        return memoria;
    }

    // Calcular a memória ocupada pelos livros
    memoria_total += CalcularMemoriaHashing(B->HLivros, CalcularMemoriaLivro);

    // Calcular a memória ocupada pelos requisitantes
    memoria_total += CalcularMemoriaHashing(B->HRequisitantes, CalcularMemoriaPessoa);

    // Calcular a memória ocupada pelas requisições
    memoria_total += CalcularMemoriaLista(B->LRequisicoes, CalcularMemoriaRequisicao);

    // Calcular a memória ocupada pelos distritos
    memoria_total += CalcularMemoriaHashing(B->HDistritos, CalcularMemoriaDistrito);

    // Calcular a memória ocupada pelos concelhos
    memoria_total += CalcularMemoriaHashing(B->HConcelhos, CalcularMemoriaConcelho);

    // Calcular a memória ocupada pelas freguesias
    memoria_total += CalcularMemoriaHashing(B->HFreguesias, CalcularMemoriaFreguesia);

    return memoria_total;
}


void SalvarRequisicoesXML(BIBLIOTECA *B, FILE *file, int xml) {
    NO *atual = B->LRequisicoes->Inicio;
    while (atual) {
        REQUISICAO *req = (REQUISICAO *)atual->Info;
        char bufferReq[20];
        char bufferDev[20];

        ConverterTimeParaString(req->data_requisicao, bufferReq, sizeof(bufferReq));
        if (req->data_devolucao != 0) {
            ConverterTimeParaString(req->data_devolucao, bufferDev, sizeof(bufferDev));
        } else {
            strcpy(bufferDev, "Não devolvido");
        }

        if (xml) {
            fprintf(file, "    <Requisicao>\n");
            fprintf(file, "      <ID>%d</ID>\n", req->ID);
            fprintf(file, "      <ID_Requisitante>%d</ID_Requisitante>\n", req->Ptr_Req->ID);
            fprintf(file, "      <ID_Livro>%d</ID_Livro>\n", req->Ptr_Livro->ID);
            fprintf(file, "      <Area>%s</Area>\n", req->Ptr_Livro->AREA);
            fprintf(file, "      <DataRequisicao>%s</DataRequisicao>\n", bufferReq);
            fprintf(file, "      <DataDevolucao>%s</DataDevolucao>\n", bufferDev);
            fprintf(file, "    </Requisicao>\n");
        } else {
            fprintf(file, "%d\t%d\t%d\t%s\t%s\t%s\n", req->ID, req->Ptr_Req->ID, req->Ptr_Livro->ID, req->Ptr_Livro->AREA, bufferReq, bufferDev);
        }

        atual = atual->Prox;
    }
}


void SalvarLivrosXML(BIBLIOTECA *B, FILE *file, int xml) {
    NO_CHAVE *chave = B->HLivros->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            LIVRO *livro = (LIVRO *)no->Info;
            if (xml) {
                fprintf(file, "    <Livro>\n");
                fprintf(file, "      <ID>%d</ID>\n", livro->ID);
                fprintf(file, "      <Nome>%s</Nome>\n", livro->NOME);
                fprintf(file, "      <Autor>%s</Autor>\n", livro->AUTOR);
                fprintf(file, "      <Area>%s</Area>\n", livro->AREA);
                fprintf(file, "      <Disponivel>%d</Disponivel>\n", livro->disponivel);
                fprintf(file, "    </Livro>\n");
            } else {
                fprintf(file, "%d\t%s\t%s\t%s\t%d\n", livro->ID, livro->NOME, livro->AUTOR, livro->AREA, livro->disponivel);
            }
            no = no->Prox;
        }
        chave = chave->Prox;
    }
}



void SalvarRequisitantesXML(BIBLIOTECA *B, FILE *file, int xml) {
    NO_CHAVE *chave = B->HRequisitantes->LChaves->Inicio;
    while (chave) {
        NO *no = chave->DADOS->Inicio;
        while (no) {
            PESSOA *pessoa = (PESSOA *)no->Info;
            if (xml) {
                fprintf(file, "    <Requisitante>\n");
                fprintf(file, "      <ID>%d</ID>\n", pessoa->ID);
                fprintf(file, "      <Nome>%s</Nome>\n", pessoa->NOME);
                fprintf(file, "      <DataNascimento>%s</DataNascimento>\n", pessoa->DATA_NASC);
                fprintf(file, "      <Categoria>%s</Categoria>\n", pessoa->CATEGORIA);
                fprintf(file, "    </Requisitante>\n");
            } else {
                fprintf(file, "%d\t%s\t%s\t%s\n", pessoa->ID, pessoa->NOME, pessoa->DATA_NASC, pessoa->CATEGORIA);
            }
            no = no->Prox;
        }
        chave = chave->Prox;
    }
}


void GravarInformacaoXML(BIBLIOTECA *Bib, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "<Biblioteca>\n");
    fprintf(arquivo, "  <Nome>%s</Nome>\n", Bib->NOME);

    // Gravar Livros
    fprintf(arquivo, "  <Livros>\n");
    SalvarLivrosXML(Bib, arquivo, 1);
    fprintf(arquivo, "  </Livros>\n");

    // Gravar Requisitantes
    fprintf(arquivo, "  <Requisitantes>\n");
    SalvarRequisitantesXML(Bib, arquivo, 1);
    fprintf(arquivo, "  </Requisitantes>\n");

    // Gravar Requisições
    fprintf(arquivo, "  <Requisicoes>\n");
    SalvarRequisicoesXML(Bib, arquivo, 1);
    fprintf(arquivo, "  </Requisicoes>\n");


    fprintf(arquivo, "</Biblioteca>\n");
    fclose(arquivo);
}

