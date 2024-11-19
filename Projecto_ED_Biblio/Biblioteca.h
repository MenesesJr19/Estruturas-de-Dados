#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Livro.h"
#include "Lista.h"
#include "Hashing.h"
#include "Requisicao.h"

typedef struct {
    int ID;
    char *Nome;
} DISTRITO;

typedef struct {
    int ID;
    char *Nome;
    int ID_Distrito;
} CONCELHO;

typedef struct {
    char *ID;
    char *Nome;
    int ID_Concelho;
} FREGUESIA;


typedef struct
{
    char *NOME;
    char *FICHEIRO_LOGS;

    HASHING *HLivros;
    HASHING   *HRequisitantes;
    LISTA *LRequisicoes;
    HASHING *HDistritos;
    HASHING * HConcelhos;
    HASHING *HFreguesias;


}BIBLIOTECA;

typedef struct {
    char sobrenome[100];
    int count;
} SOBRENOME_COUNT;





BIBLIOTECA *CriarBiblioteca(char *_nome, char *_logs);
void ShowBiblioteca(BIBLIOTECA *B);//lista os livros
void ShowRequisitantes(BIBLIOTECA *B);
void ListarRequisitantes(BIBLIOTECA *B);//LISTA MAS EM MAIUSCULAS
void DestruirBiblioteca(BIBLIOTECA *B);
int LoadFicheiroBiblioteca(BIBLIOTECA *B);
int AddLivroBiblioteca(BIBLIOTECA *B, LIVRO *L);
void RemoverLivroBiblioteca(BIBLIOTECA *B, int id, const char *area);
LIVRO *LivroMaisRequisitadoBiblioteca(BIBLIOTECA *B);
char *ApelidoMaisComum(BIBLIOTECA *B);
char *AreaMaisComum(BIBLIOTECA *B);

//REQUISITANTE NOVO
int IdExiste(BIBLIOTECA *B, int id);
int CalcularDigitoDeControlo(int *id_parcial);
int GerarIdRequisitante(BIBLIOTECA *B);
int AddRequisitante(BIBLIOTECA *B, PESSOA *X);
//PESQUISAS
PESSOA *PesquisarRequisitante(BIBLIOTECA *B, int cod);
PESSOA *PesquisarRequisitanteNome(BIBLIOTECA *B, char *nome);
LIVRO *PesquisarLivro(BIBLIOTECA *B, int id, const char *area);
char* EncontrarFreguesiaH(const char *nome_freguesia, HASHING *HFreguesias);
int LivroRequisitado(BIBLIOTECA *B, int livroID);
int RemoverLivrosNuncaRequisitados(BIBLIOTECA *B);
void ListarLivros(BIBLIOTECA *B);

//criação distritos,concelhos,freguesias
DISTRITO *CriarDistrito(int id, const char *nome);
void AddDistritoHashing(HASHING *H, DISTRITO *distrito);
//--------------------------------------------------------
CONCELHO *CriarConcelho(int id, const char *nome, int id_distrito);
void AddConcelhoHashing(HASHING *H, CONCELHO *concelho);
//--------------------------------------------------------
FREGUESIA *CriarFreguesia(int id, const char *nome, int id_concelho);
NO_CHAVE *FuncaoHashingFreguesia(HASHING *H, FREGUESIA *X);
//--------------------------------------------------------
void AddInicioFreguesia(LISTA *L, FREGUESIA *X);
//--------------------------------------------------------
void AddHashingFreguesia(HASHING *H, FREGUESIA *freguesia);
void DestruirDistrito(DISTRITO *distrito);
void DestruirConcelho(CONCELHO *concelho);
void DestruirFreguesia(FREGUESIA *freguesia);

//carregar ficheiros
void LoadDistritos(BIBLIOTECA *B, const char *filename);
void LoadConcelhos(BIBLIOTECA *B, const char *filename);
void LoadFreguesias(BIBLIOTECA *B, const char *filename);
void LoadRequisitantes(BIBLIOTECA *B, const char *filename);
void LoadLivros(BIBLIOTECA *B, const char *filename);
void LoadRequisicoes(BIBLIOTECA *B, const char *filename);

//nao são necessárias, apenas para verificar se o carregamento  leitura estavam corretos
void ListarTodosDistritos(BIBLIOTECA *B);
void ListarTodosConcelhos(BIBLIOTECA *B);
void ListarTodasFreguesias(BIBLIOTECA *B);

//Salvar ficheiros formato .txt
void SalvarLivros(BIBLIOTECA *B, const char *filename);
void SalvarRequisitantes(BIBLIOTECA *B, const char *filename);
void SalvarRequisicoes(BIBLIOTECA *B, const char *filename);
//Salvar ficheiros formato .csv
void SalvarRequisitantesCSV(BIBLIOTECA *B, const char *filename);
void SalvarRequisicoesCSV(BIBLIOTECA *B, const char *filename);
void SalvarLivrosCSV(BIBLIOTECA *B, const char *filename);


//Salvar todos
void SalvarBiblioteca(BIBLIOTECA *B);
//gravarXML
void SalvarRequisicoesXML(BIBLIOTECA *B, FILE *file, int xml);
void SalvarLivrosXML(BIBLIOTECA *B, FILE *file, int xml);
void SalvarRequisitantesXML(BIBLIOTECA *B, FILE *file, int xml);
//gravar todos os xml
void GravarInformacaoXML(BIBLIOTECA *Bib, const char *nomeArquivo);

//requisições
void AdicionarRequisicao(BIBLIOTECA *B, REQUISICAO *req);
LIVRO *PesquisarLivroPorArea(BIBLIOTECA *B, int id, const char *area);
int CriarRequisicaoBiblioteca(BIBLIOTECA *B, int idReq, int idLivro, const char *area);
void ListarRequisicoes(BIBLIOTECA *B);
void MostrarLivrosRequisitadosPorPessoa(BIBLIOTECA *B, PESSOA *P);
void MostrarPessoasNuncaRequisitaram(BIBLIOTECA *B);
void MostrarRequisitantesComLivrosRequisitados(BIBLIOTECA *B);

//devoluções
int RegistarDevolucao(BIBLIOTECA *B, int idReq, int idLivro, const char *area);

void AreaMaisComumBiblioteca(BIBLIOTECA *B);
void AreaMaisRequisitadaBiblioteca(BIBLIOTECA *B);
void listarRequisitantesPorPagina(BIBLIOTECA *biblioteca, int requisitantesPorPagina);
int DeterminarIdadeMaxima(BIBLIOTECA *B);
double DeterminarMediaIdades(BIBLIOTECA *B);
int ContarPessoasIdadeSuperior(BIBLIOTECA *B, int idadeMinima);
int DeterminarIdadeComMaisRequisitantes(BIBLIOTECA *B);
void DeterminarSobrenomeMaisUsado(BIBLIOTECA *B);
void ListarRequisitantesNascidosEmDomingo(BIBLIOTECA *B);
size_t CalcularMemoriaBiblioteca(BIBLIOTECA *B);


//funções auxiliares
void ConvertToUpperCase(char *str);
int is_digits(const char *str);
int validar_id_requisitante(const char *id);
int validar_data_nasc(const char *data);
NO_CHAVE *FuncaoHashingPorID(HASHING *H, const char *id);
int validar_id_freguesia(const char *id, HASHING *HFreguesias);
void registrar_erro(const char *linha, const char *erro);
void to_lowercase(char *str);
int strptime_custom(const char *str, struct tm *tm_info);
void ConverterTimeParaString(time_t time_value, char *buffer, size_t buffer_size);
time_t ConverterStringParaTime(const char *str);
int contarTotalPaginas(int numRequisitantes, int requisitantesPorPagina);
int CalcularIdade(char *data_nasc);


#endif // BIBLIOTECA_H_INCLUDED
