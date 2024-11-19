#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include "Lista.h"
#include "Hashing.h"
#include "Biblioteca.h"

#define REQUISITANTES_POR_PAGINA 450//requisitantes que vão ser listados por cada pagina

extern int LerInteiro(char *txt);
//-----------------------------------------


int MenuPrincipal() {
    printf("1- Livros\n");
    printf("2- Requisitantes\n");
    printf("3- Requisições\n");
    printf("4- Outros\n");
    printf("0- Sair\n");
    int op;
    op = LerInteiro("Qual a opcao? ");
    return op;
}

int MenuLivros() {
    printf("1- Adicionar Livro\n");
    printf("2- Remover Livro\n");
    printf("3- Pesquisar Livro\n");
    printf("4- Livro Mais Requisitado\n");
    printf("5- Área com mais livros\n");
    printf("6- Remover livros nunca requisitados e listar todos os livros\n");
    printf("7- Mostrar Livros\n");
    printf("0- Voltar\n");
    int op;
    op = LerInteiro("Qual a opcao? ");
    return op;
}

int MenuRequisitantes() {
    printf("1- Mostrar Requisitantes\n");
    printf("2- Adicionar Requisitante\n");
    printf("3- Pesquisar Requisitante\n");
    printf("4- Determinar idade máxima\n");
    printf("5- Determinar média das idades\n");
    printf("6- Contar pessoas com idade superior a um valor\n");
    printf("7- Determinar idade com mais requisitantes\n");
    printf("8- Determinar sobrenome mais usado\n");
    printf("9- Listar requisitantes nascidos em Domingo\n");
    printf("0- Voltar\n");
    int op;
    op = LerInteiro("Qual a opcao? ");
    return op;
}

int MenuRequisicoes() {
    printf("1- Requisitar Livro\n");
    printf("2- Devolução de Livro\n");
    printf("3- Listar Requisições\n");
    printf("4- Mostrar Requisição de Pessoa\n");
    printf("5- Mostrar Pessoas que nunca requisitaram\n");
    printf("6- Mostrar Pessoas com livros requisitados\n");
    printf("7- Área com mais requisições\n");
    printf("0- Voltar\n");
    int op;
    op = LerInteiro("Qual a opcao? ");
    return op;
}

int MenuOutros() {
    printf("1- Guardar Dados da biblioteca\n");
    printf("2- Mostrar Memória Ocupada\n");
    printf("3- Gravar Informações em XML\n");
    printf("0- Voltar\n");
    int op;
    op = LerInteiro("Qual a opcao? ");
    return op;
}


void limparEcra(){
    system("cls");
}


int main()
{
    setlocale(LC_ALL, "Portuguese");
    printf("Projecto-Biblioteca-Versao-Base!\n");
    BIBLIOTECA *Bib;
    Bib = CriarBiblioteca("Biblioteca-ESTGV", "log.txt");

    // Carregar todos os ficheiros automaticamente ao iniciar o programa
    LoadFicheiroBiblioteca(Bib);

    int OP, criterio, idadeMinima;
    do {
        OP = MenuPrincipal();
        limparEcra();
        switch (OP) {
            case 1:
                switch (MenuLivros()) {
                    case 1: {
                        int id = LerInteiro("ID do livro: ");
                        char nome[50], autor[50], area[20];
                        printf("Nome do livro: ");
                        scanf(" %[^\n]", nome);
                        printf("Nome do autor: ");
                        scanf(" %[^\n]", autor);
                        printf("Area do livro: ");
                        scanf(" %[^\n]", area);
                        LIVRO *newLivro = CriarLivro(id, nome, autor, area);
                        AddLivroBiblioteca(Bib, newLivro);
                        break;
                    }
                    case 2: {
                        char area[50];
                        int id = LerInteiro("ID do livro a remover: ");
                        printf("Area do livro a remover: ");
                        scanf(" %[^\n]", area);
                        RemoverLivroBiblioteca(Bib, id, area);
                        break;
                    }
                    case 3: {
                        char area[20];
                        int id = LerInteiro("Indique o ID do livro:");
                        printf("\nIndique a area do livro:");
                        scanf(" %[^\n]", area);
                        LIVRO *found = PesquisarLivro(Bib, id, area);
                        if (found) {
                            MostrarLivro(found);
                        } else {
                            printf("Livro nao encontrado\n");
                        }
                        break;
                    }
                    case 4: {
                        LIVRO *livroMaisRequisitado = LivroMaisRequisitadoBiblioteca(Bib);
                        if (livroMaisRequisitado) {
                            printf("Livro mais requisitado:\n");
                            MostrarLivro(livroMaisRequisitado);
                            printf("\n");
                        } else {
                            printf("Nenhum livro requisitado ainda.\n");
                        }
                        break;
                    }
                    case 5: {
                        AreaMaisComumBiblioteca(Bib);//area com mais livros
                        break;
                    }
                    case 6: {
                        RemoverLivrosNuncaRequisitados(Bib);
                        printf("Lista de Livros após Remoção de Livros Nunca Requisitados:\n");
                        ListarLivros(Bib);
                        break;
                    }
                    case 7:
                        {
                            ShowBiblioteca(Bib);
                            break;
                        }
                    default:
                        printf("Opcao nao implementada\n");
                        break;
                }
                break;
            case 2:
                switch (MenuRequisitantes()) {
                    case 1:
                        listarRequisitantesPorPagina(Bib, REQUISITANTES_POR_PAGINA);
                        break;
                    case 2: {
                        char nome[50], freguesia[50], data_nasc[11];

                        int id;
                        limparEcra();

                        // Gerar automaticamente um ID único para o requisitante
                        id = GerarIdRequisitante(Bib);

                        printf("ID do requisitante gerado automaticamente: %d\n", id);

                        printf("Nome do requisitante: ");
                        scanf(" %[^\n]", nome);

                        printf("Freguesia do requisitante: ");
                        scanf(" %[^\n]", freguesia);

                        if (!validar_id_freguesia(freguesia, Bib->HFreguesias)) {
                            printf("ID Freguesia inválido\n");
                            sleep(2);
                            break;
                        }
                        printf("Data de nascimento (dd-mm-aaaa): ");
                        scanf(" %[^\n]", data_nasc);
                        if (!validar_data_nasc(data_nasc)) {
                            printf("Data de nascimento inválida\n");
                            break;
                        }

                        PESSOA *newReq = CriarPessoa(id, nome, freguesia, data_nasc);

                        if (AddRequisitante(Bib, newReq) == EXIT_SUCCESS) {
                            printf("Requisitante adicionado com sucesso\n");
                            sleep(2); // Pausa por 2 segundos antes de continuar
                        } else {
                            printf("Erro ao adicionar requisitante\n");
                            sleep(2); // Pausa por 2 segundos antes de continuar
                        }

                        break;
                    }
                    case 3: {
                        int op;
                        printf("1 - Pesquisar ID || 0 - Pesquisar Nome\n");
                        scanf("%d", &op);
                        if (op == 1) {
                            int id = LerInteiro("ID do requisitante: ");
                            PESSOA *found = PesquisarRequisitante(Bib, id);
                            if (found) {
                                MostrarPessoa(found);
                                printf("1 - Ver requisicoes || 2 - Sair\n");
                                scanf("%d", &op);
                                if (op == 1) {
                                    MostrarLivrosRequisitadosPorPessoa(Bib, found);
                                }
                            } else {
                                printf("Requisitante nao encontrado\n");
                            }
                        } else {
                            char nome[50];
                            printf("Insira o nome:");
                            scanf(" %[^\n]", nome);

                            PESSOA *foundName = PesquisarRequisitanteNome(Bib, nome);
                            if (foundName) {
                                MostrarPessoa(foundName);
                                printf("1 - Ver requisicoes || 2 - Sair\n");
                                scanf("%d", &op);
                                if (op == 1) {
                                    MostrarLivrosRequisitadosPorPessoa(Bib, foundName);
                                }
                            } else {
                                printf("Requisitante não encontrado\n");
                            }
                        }
                        break;
                    }

                    case 4:
                        printf("Idade máxima: %d\n", DeterminarIdadeMaxima(Bib));
                        break;
                    case 5:
                        DeterminarMediaIdades(Bib);
                        break;
                    case 6:
                        printf("Insira a idade mínima: ");
                        scanf("%d", &idadeMinima);
                        printf("Número de pessoas com idade superior a %d: %d\n", idadeMinima, ContarPessoasIdadeSuperior(Bib, idadeMinima));
                        break;
                    case 7:
                        printf("Idade com mais requisitantes: %d\n", DeterminarIdadeComMaisRequisitantes(Bib));
                        break;
                    case 8:
                        DeterminarSobrenomeMaisUsado(Bib);
                        break;
                    case 9:
                        ListarRequisitantesNascidosEmDomingo(Bib);
                        break;

                    default:
                        printf("Opcao nao implementada\n");
                        break;
                }
                break;
            case 3:
                switch (MenuRequisicoes()) {
                    case 1: {
                        char area[20];
                        int idReq = LerInteiro("ID do requisitante: ");
                        int idLivro = LerInteiro("ID do livro: ");
                        printf("\nIndique a area do livro:");
                        scanf(" %[^\n]", area);
                        if (!CriarRequisicaoBiblioteca(Bib, idReq, idLivro, area)) {
                            printf("Nao foi possivel criar a requisicao\n");
                        }
                        break;
                    }
                    case 2: {
                        char area[20];
                        int idReq = LerInteiro("ID do requisitante: ");
                        int idLivro = LerInteiro("ID do livro: ");
                        printf("\nIndique a area do livro:");
                        scanf(" %[^\n]", area);
                        if (!RegistarDevolucao(Bib, idReq, idLivro, area)) {
                            printf("Nao foi possivel registrar a devolucao\n");
                        }
                        break;
                    }
                    case 3:
                        ListarRequisicoes(Bib);
                        printf("\n");
                        break;
                    case 4: {
                        int id;
                        printf("ID do requisitante: ");
                        scanf("%d", &id);
                        PESSOA *requisitante = PesquisarRequisitante(Bib, id);
                        if (requisitante != NULL) {
                            printf("Requisicoes de [%s]\n", requisitante->NOME);
                            MostrarLivrosRequisitadosPorPessoa(Bib, requisitante);
                        } else {
                            printf("Requisitante não encontrado.\n");
                        }
                        break;
                    }
                    case 5:
                        MostrarPessoasNuncaRequisitaram(Bib);
                        break;
                    case 6:
                        MostrarRequisitantesComLivrosRequisitados(Bib);
                        break;
                    case 7:
                        AreaMaisRequisitadaBiblioteca(Bib);
                        break;
                    default:
                        printf("Opcao nao implementada\n");
                        break;
                }
                break;
            case 4:
                switch (MenuOutros()) {
                    case 1:
                        SalvarBiblioteca(Bib);
                        break;
                    case 2: {
                        size_t memoriaTotal = CalcularMemoriaBiblioteca(Bib);
                        printf("Memória ocupada por toda a estrutura de dados: %zu bytes\n", memoriaTotal);
                        break;
                    }
                    case 3: {
                        char nomeArquivo[256];
                        printf("Digite o nome do arquivo XML para salvar as informações: ");
                        scanf("%255s", nomeArquivo);
                        GravarInformacaoXML(Bib, nomeArquivo);
                        printf("Informações da biblioteca gravadas em arquivo XML: %s\n", nomeArquivo);
                        break;
                    }
                    default:
                        printf("Opcao nao implementada\n");
                        break;
                }
                break;
            default:
                printf("Opcao nao implementada\n");
                break;
        }
    } while (OP != 0);
    printf("A guardar todos os ficheiros...\n");
    sleep(2);
    SalvarBiblioteca(Bib);
    printf("Ficheiros guardados com sucesso...\n");

    printf("Vou destruir a biblioteca\n");
    DestruirBiblioteca(Bib);
    return EXIT_SUCCESS;
}
