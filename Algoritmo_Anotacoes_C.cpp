#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ANOTACOES 100
#define TAM_TEXTO 200
#define NOME_ARQUIVO "anotacoes.txt"

/* 
Aluno: João Anderson da Cruz Gonçalves dos Santos - SIF 3 - A

O tipo de fila utilizado no código foi uma fila linear simples (ou fila estática sequencial).
- Linear e sequencial ;
- Armazenada em um array fixo ;
- Insere no final (tras++) ;
- Remove deslocando os elementos para a esquerda.

Exemplo: 
[ Anotação1 ][ Anotação2 ][ Anotação3 ]
   frente          -         tras

*/

// Estrutura de uma anotação
typedef struct {
    char texto[TAM_TEXTO];
    char dataHora[20];
} Anotacao;

// Fila de anotações
typedef struct {
    Anotacao itens[MAX_ANOTACOES];
    int frente;
    int tras;
} FilaAnotacoes;

// Inicializa a fila
void inicializarFila(FilaAnotacoes *fila) {
    fila->frente = 0;
    fila->tras = 0;
}

// Verifica se a fila está cheia
int filaCheia(FilaAnotacoes *fila) {
    return fila->tras == MAX_ANOTACOES;
}

// Verifica se a fila está vazia
int filaVazia(FilaAnotacoes *fila) {
    return fila->frente == fila->tras;
}

// Pega data e hora atual
void pegarDataHora(char *dataHora) {
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(dataHora, 20, "%d/%m/%Y %H:%M", tm_info);
}

// Remove o '\n' do final da string
void tirarEnter(char *texto) {
    int len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    }
}

// Salva as anotações no arquivo
void salvarAnotacoes(FilaAnotacoes *fila) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar anotacoes!\n");
        return;
    }

    for (int i = fila->frente; i < fila->tras; i++) {
        fprintf(arquivo, "%s\n%s\n", fila->itens[i].dataHora, fila->itens[i].texto);
    }

    fclose(arquivo);
}

// Carrega anotações do arquivo
void carregarAnotacoes(FilaAnotacoes *fila) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) return; // Nenhum arquivo ainda

    char linhaData[20];
    char linhaTexto[TAM_TEXTO];

    while (fgets(linhaData, sizeof(linhaData), arquivo) != NULL &&
           fgets(linhaTexto, sizeof(linhaTexto), arquivo) != NULL) {

        tirarEnter(linhaData);
        tirarEnter(linhaTexto);

        strcpy(fila->itens[fila->tras].dataHora, linhaData);
        strcpy(fila->itens[fila->tras].texto, linhaTexto);
        fila->tras++;

        if (filaCheia(fila)) break;
    }

    fclose(arquivo);
}

// Adiciona anotação na fila
void novaAnotacao(FilaAnotacoes *fila) {
    if (filaCheia(fila)) {
        printf("Fila de anotacoes cheia!\n");
        return;
    }

    printf("Digite sua anotacao:\n> ");
    fgets(fila->itens[fila->tras].texto, TAM_TEXTO, stdin);
    tirarEnter(fila->itens[fila->tras].texto);

    pegarDataHora(fila->itens[fila->tras].dataHora);

    fila->tras++;
    printf("Anotacao salva!\n");

    salvarAnotacoes(fila);
}

// Mostra todas as anotações da fila
void verAnotacoes(FilaAnotacoes *fila) {
    if (filaVazia(fila)) {
        printf("Nenhuma anotacao encontrada.\n");
        return;
    }

    printf("\n=== Minhas Anotacoes ===\n");
    {
        int i;
        for (i = fila->frente; i < fila->tras; i++) {
            printf("%d - [%s] %s\n", i + 1, fila->itens[i].dataHora, fila->itens[i].texto);
        }
    }
    printf("=========================\n");
}

// Remove anotação por posição
void apagarAnotacao(FilaAnotacoes *fila) {
    if (filaVazia(fila)) {
        printf("Nenhuma anotacao para apagar.\n");
        return;
    }

    verAnotacoes(fila);

    {
        int apagar;
        printf("Digite o numero da anotacao que deseja apagar: ");
        scanf("%d", &apagar);
        getchar(); // limpar buffer

        apagar--; // ajusta para índice de array

        if (apagar < 0 || apagar >= fila->tras) {
            printf("Numero invalido!\n");
            return;
        }

        {
            int i;
            for (i = apagar; i < fila->tras - 1; i++) {
                fila->itens[i] = fila->itens[i + 1];
            }
        }
        fila->tras--;

        printf("Anotacao apagada com sucesso!\n");
        salvarAnotacoes(fila);
    }
}

int main() {
    FilaAnotacoes fila;
    int opcao;

    inicializarFila(&fila);
    carregarAnotacoes(&fila);

    do {
        printf("\n=== BLOCO DE ANOTACOES ===\n");
        printf("1 - Nova anotacao\n");
        printf("2 - Ver anotacoes\n");
        printf("3 - Apagar uma anotacao\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar enter

        if (opcao == 1) {
            novaAnotacao(&fila);
        } else if (opcao == 2) {
            verAnotacoes(&fila);
        } else if (opcao == 3) {
            apagarAnotacao(&fila);
        } else if (opcao == 4) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 4);

    return 0;
}

