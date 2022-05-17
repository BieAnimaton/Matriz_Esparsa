#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


typedef struct matriz {
    int dado;
    int matriz_linha, matriz_coluna;
    struct matriz *direita, *esquerda, *acima, *abaixo;
} Matriz;

typedef struct vetor {
    int posicao;
    struct matriz *matriz;
} Vetor;

Vetor vetor_linha[10], vetor_coluna[10];


void gotoxy (int x, int y) {
COORD coord;
coord.X = (short)x;
coord.Y = (short)y;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


int listar () {
    int i, j, x;
    Matriz *percorre;

    x = 1;

    for (j = 0; j < 81; j += 9) {
        gotoxy(j, 0);
        printf("[%d]", x); // imprimindo indicadores de coluna
        x++;
    }

    printf("\n");
    for (i = 1; i <= 9; i++) {
        percorre = vetor_linha[i].matriz;
        while (percorre != NULL) {
            gotoxy((percorre->matriz_coluna-1)*9, percorre->matriz_linha+7+i); // coordenadas do terminal para impressão
            printf("(%d)", percorre->dado);
            percorre = percorre->direita;
        }
    }
    printf("\n");
    getchar();

    return 0;;
}


int inicializar () {
    int i;
    for (i = 1; i <= 9; i++) {
        vetor_coluna[i].matriz = NULL; // Deixando a matriz da coluna como NULL
        vetor_linha[i].matriz = NULL; // Deixando a matriz da linha como NULL
        vetor_coluna[i].posicao = i; // Deixando a posição da coluna igual ao "i"
        vetor_linha[i].posicao = i; // Deixando a posição da linha igual ao "i"
    }

    return 0;
}


int verificar_celula (int linha, int coluna) {
    Matriz *percorre;

    percorre = vetor_linha[linha].matriz;

    while ((percorre != NULL) && ((percorre->matriz_coluna!=coluna) || (percorre->matriz_linha!=linha))) {
        percorre = percorre->direita;
    }

    if(percorre == NULL)
        return 0; //célula não usada
    else
        return 1; //célula usada
}


int inserir (int info, int linha, int coluna) {
    int erro;
    erro = verificar_celula(linha, coluna);

    if (erro == 1) return 1; // volta erro devido a célula já estar sendo usada

    Matriz *percorre, *anterior, *elemento;

    elemento = (Matriz*)malloc(sizeof(Matriz));
    elemento->dado = info;
    elemento->direita = NULL;
    elemento->esquerda = NULL;
    elemento->acima = NULL;
    elemento->abaixo = NULL;
    elemento->matriz_linha = linha;
    elemento->matriz_coluna = coluna;

    // Verificando linha
    if (vetor_linha[linha].matriz == NULL) { // linha vazia
        vetor_linha[linha].matriz = elemento;
    } else {
        percorre = vetor_linha[linha].matriz;
        anterior = NULL;

        while ((percorre != NULL) && (percorre->matriz_coluna < coluna)) {
            anterior = percorre;
            percorre = percorre->direita;
        }

        if (percorre == NULL) {
            anterior->direita = elemento; // inserindo no final
            elemento->esquerda = anterior;
        } else {
            if (anterior == NULL) {
                vetor_linha[linha].matriz = elemento; // inserindo no inicio
                elemento->direita = percorre;
                percorre->esquerda = elemento;
            } else {
                anterior->direita = elemento; // inserindo no meio
                elemento->esquerda = anterior;

                percorre->esquerda = elemento;
                elemento->direita = percorre;
            }
        }
    }


    // Verificando coluna
    if (vetor_coluna[coluna].matriz == NULL) { //coluna vaiz
        vetor_coluna[coluna].matriz = elemento;
    } else {
        percorre = vetor_coluna[coluna].matriz;
        anterior = NULL;

        while ((percorre != NULL) && (percorre->matriz_linha < linha)) {
            anterior = percorre;
            percorre = percorre->abaixo;
        }

        if (percorre == NULL) {
            anterior->abaixo = elemento; // inserindo no final
            elemento->acima = anterior;
        } else {
            if (anterior == NULL) {
                vetor_coluna[coluna].matriz = elemento; // inserindo no inicio
                elemento->abaixo = percorre;
                percorre->acima = elemento;
            } else {
                anterior->abaixo = elemento; // inserindo no meio
                elemento->acima = anterior;

                percorre->acima = elemento;
                elemento->abaixo = percorre;
            }
        }
    }
}

int remover (int linha, int coluna) {
    int erro;

    erro = verificar_celula(linha, coluna);
    if (erro == 0) {
        return 1; // Não existe elemento
    }

    Matriz *percorre, *aux;

    percorre = vetor_linha[linha].matriz;
    while ((percorre != NULL) && ((percorre->matriz_coluna!=coluna) || (percorre->matriz_linha!=linha))) {
        percorre = percorre->direita;
    }

    aux = percorre;

    percorre = aux->direita;
    vetor_linha[linha].matriz = percorre;
    free(aux);

    return 0;
}

int main()
{
    int info;

    inicializar();

    inserir(56, 7, 8);
    inserir(33, 3, 5);
    inserir(6, 6, 6);

    remover(3, 5);

    listar();

    return 0;
}
