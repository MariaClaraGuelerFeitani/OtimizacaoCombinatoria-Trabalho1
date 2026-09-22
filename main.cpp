#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define const 100


void gerar(int pos, int usado, int *padrao, float matriz[][2], int capacidade, int numCortes, int **&matrizPadroes, int &numLinhas){
    // Todos os tipos de corte já foram escolhidos
    if (pos == numCortes)
    {
        // Não adiciona o padrão vazio
        if (usado > 0)
        {
            matrizPadroes = (int**) realloc(
                matrizPadroes,
                (numLinhas + 1) * sizeof(int*)
            );

            matrizPadroes[numLinhas] = (int*) malloc(
                numCortes * sizeof(int)
            );

            // Copia o padrão para a matriz
            for (int i = 0; i < numCortes; i++)
            {
                matrizPadroes[numLinhas][i] = padrao[i];
            }

            numLinhas++;
        }

        return;
    }

    // Tamanho do corte atual
    int tamanho = (int) matriz[pos][0];

    // Quantas unidades desse corte ainda cabem
    int maximo = (capacidade - usado) / tamanho;

    for (int quantidade = 0; quantidade <= maximo; quantidade++)
    {
        padrao[pos] = quantidade;

        gerar(pos + 1, usado + quantidade * tamanho, padrao, matriz, capacidade, numCortes, matrizPadroes, numLinhas);
        }

    padrao[pos] = 0;
}





using namespace std;

int main()
{

    //LENDO AS INSTÂNCIAS

    FILE *arq = fopen("fiber06_5180.txt", "r");

    if (arq == NULL){
        printf("Falha ao abrir o arquivo!\n");
        return NULL;
    }

    float comprimentoObjeto;

    fscanf(arq, "%f", &comprimentoObjeto);

    int numCortes;

    fscanf(arq, "%d", &numCortes);

    float matriz[numCortes][2];

    for(int i=0; i<numCortes; i++){
        for(int j=0;j<2;j++){
            fscanf(arq, "%f", &matriz[i][j]);
        }
    }


    //conferindo as instâncias
    /*
    printf("Comprimento do objeto: %f\n", comprimentoObjeto);

    printf("Numero de cortes diferentes: %d\n", numCortes);

    for(int i=0;i<numCortes;i++){
        for(int j=0;j<2;j++){
            printf("%f ", matriz[i][j]);
        }
        printf("\n");
    }
    */




    //MONTANDO A MATRIZ DE PADRÕES

    int capacidade = (int)comprimentoObjeto;
    int **matrizPadroes = NULL;
    int numLinhas = 0;

    //calloc já aloca e armazena tudo com valor 0
    int *padrao = (int*) calloc(numCortes,sizeof(int));

    gerar(0, 0, padrao, matriz, capacidade, numCortes, matrizPadroes, numLinhas);


    for(int i=0;i<numLinhas;i++){
        for(int j=0;j<numCortes;j++){
            printf("%d ", matrizPadroes[i][j]);
        }
        printf("\n");
    }

    printf("%d \n", numLinhas);




    //MONTANDO O ARQUIVO

    float custoPorSetup;
    float custoPorObjeto;
    float totalPadroes = 0;

    custoPorSetup = const;
    custoPorObjeto = comprimentoObjeto;

    for(int i=0;i<numCortes;i++){
        totalPadroes += matriz[i][1];
    }


    FILE *arquivo = fopen ("arquivo.lp", "w");

    fprintf(arquivo, "Minimize\n");


    for(int i=0;i<numLinhas;i++){
        if(i == numLinhas-1){
            fprintf(arquivo, "%f ", custoPorObjeto);
            fprintf(arquivo, " x%d", i+1);
        }
        else{
            fprintf(arquivo, "%f ", custoPorObjeto);
            fprintf(arquivo, " x%d + ", i+1);
        }
    }


    fprintf(arquivo, " + ");


    for(int i=0;i<numLinhas;i++){
        if(i == numLinhas-1){
            fprintf(arquivo, " %f", custoPorSetup);
            fprintf(arquivo, " y%d", i+1);
        }
        else{
            fprintf(arquivo, " %f", custoPorSetup);
            fprintf(arquivo, " y%d + ", i+1);
        }
    }



    fprintf(arquivo, "\n");
    fprintf(arquivo, "\n");
    fprintf(arquivo, "Subject To\n");



    for(int i=0;i<numCortes;i++){
        for(int j=0;j<numLinhas;j++){
            if(j != numLinhas-1){
                fprintf(arquivo, " %d ", matrizPadroes[j][i]);
                fprintf(arquivo, "x%d + ", j+1);
            }
            else{
                fprintf(arquivo, " %d ", matrizPadroes[j][i]);
                fprintf(arquivo, "x%d ", j+1);
            }
        }
        fprintf(arquivo, ">=  %f", matriz[i][1]);
        fprintf(arquivo, "\n");
    }


    fprintf(arquivo, "\n");


    for(int i=0;i<numLinhas;i++){
        fprintf(arquivo, "c%d:", i+1);
        fprintf(arquivo, " x%d - ", i+1);
        fprintf(arquivo, " %f ", totalPadroes);
        fprintf(arquivo, " y%d <= 0\n", i+1);
    }


    fprintf(arquivo, "\n");
    fprintf(arquivo, "Bounds\n");

    for(int i=0;i<numLinhas;i++){
        fprintf(arquivo, "x%d >= 0", i+1);
        fprintf(arquivo, "\n");
    }

    fprintf(arquivo, "\n");
    fprintf(arquivo, "General\n");

    for(int i=0;i<numLinhas;i++){
        fprintf(arquivo, "x%d ", i+1);
    }

    fprintf(arquivo, "\n");
    fprintf(arquivo, "\n");
    fprintf(arquivo, "Binary\n");

    for(int i=0;i<numLinhas;i++){
        fprintf(arquivo, "y%d ", i+1);
    }


    fprintf(arquivo, "\n");
    fprintf(arquivo, "\n");
    fprintf(arquivo, "End\n");



    free(matrizPadroes);
    fclose(arq);

    return 0;
}






