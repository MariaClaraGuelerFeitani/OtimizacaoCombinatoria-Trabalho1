#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gurobi_c++.h"
#define const 100

//FUNÇÃO QUE GERA TODAS AS POSSIBILIDADES DE PADRÕES POSSÍVEIS
//OBS: COMENTEI PORQUE ESTAVA GERANDO MUITOS PADRÕES E MEU COMPUTADOR NÃO ESTAVA CONSEGUINDO PROCESSAR

/*void gerar(int pos, int usado, int *padrao, float matriz[][2], int capacidade, int numCortes, int **&matrizPadroes, int &numLinhas){
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
*/





using namespace std;

int main()
{

    //LENDO AS INSTÂNCIAS

    FILE *arq = fopen("fiber29_5180.txt", "r");

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

    float vetAuxQuantidade[numCortes];

    for(int i=0;i<numCortes;i++){
        vetAuxQuantidade[i] = matriz[i][1];
    }


    float vetAuxTamPadroes[numCortes];

    for(int i=0;i<numCortes;i++){
        vetAuxTamPadroes[i] = matriz[i][0];
    }


    bool conferir = true;
    int x = numCortes;
    int quantidade;
    float sobra;
    int numLinhas = 0;
    int ** matrizPadroes = NULL;

    while (conferir){

        //criação dinâmica da matriz
        matrizPadroes = (int**) realloc (matrizPadroes, (numLinhas + 1) * sizeof(int*));
        matrizPadroes[numLinhas] = (int*) malloc(numCortes * (sizeof(int)));

        for(int i=0;i<numCortes;i++){
            matrizPadroes[numLinhas][i] = 0;
        }


        while(vetAuxQuantidade[x-1] == 0){
            x = x - 1;
        }

        quantidade = comprimentoObjeto / vetAuxTamPadroes[x-1];

        if(quantidade > vetAuxQuantidade[x-1]){
            quantidade = vetAuxQuantidade[x-1];
        }

        matrizPadroes[numLinhas][x-1] = quantidade;

        vetAuxQuantidade[x-1] = vetAuxQuantidade[x-1] - quantidade;

        sobra = comprimentoObjeto - (vetAuxTamPadroes[x-1] * quantidade);

        bool aproveitar = true;
        int contAproveitar = x - 2;
        int quantAproveitar;

        //aproveitar as sobras para cortar outros padrões menores
        while(aproveitar){
            aproveitar = false;
            for(int i=contAproveitar;i>=0;i--){
                if(sobra >= vetAuxTamPadroes[i]){
                    quantAproveitar = sobra / vetAuxTamPadroes[i];

                    if(quantAproveitar > vetAuxQuantidade[i]){
                        quantAproveitar = vetAuxQuantidade[i];
                    }


                    if(quantAproveitar > 0){
                        matrizPadroes[numLinhas][i] = quantAproveitar;

                        vetAuxQuantidade[i] = vetAuxQuantidade[i] - quantAproveitar;

                        sobra = sobra - (vetAuxTamPadroes[i] * quantAproveitar);

                        aproveitar = true;
                    }
                }
            }
        }


        numLinhas++;

        //condição de parada
        conferir = false;
        for(int i=0;i<numCortes;i++){
            if(vetAuxQuantidade[i] != 0){
                conferir = true;
            }
        }
    }


    /*for(int i=0;i<numLinhas;i++){
        for(int j=0;j<numCortes;j++){
            printf("%d ", matrizPadroes[i][j]);
        }
        printf("\n");
    }

    printf("%d \n", numLinhas);*/









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

    fclose(arquivo);

    //printf("Arquivo finalizado!!!");





    //MANDANDO PARA O GUROBI RESOLVER
    try {
        //licença do Gurobi de estudante - onde está
        setenv("GRB_LICENSE_FILE","/home/mariaclara/.gurobi/gurobi.lic",1);

        // Cria o ambiente do Gurobi
        GRBEnv env = GRBEnv(true);
        env.start();

        // Carrega a instancia .lp com o modelo a ser resolvido
        GRBModel model = GRBModel(env, "arquivo.lp");

        // Limite de tempo: 1 hora = 3600 segundos
        model.set(GRB_DoubleParam_TimeLimit, 3600.0);

        // Resolve o problema
        model.optimize();

        //Cria o arquivo que vai armazenar o resultado da execução
        FILE *arquivo = fopen("resultado.txt", "w");

        if (arquivo == NULL) {
            return 1;
        }


        if (model.get(GRB_IntAttr_SolCount) > 0) {
            double lb = model.get(GRB_DoubleAttr_ObjBound);

            double ub = model.get(GRB_DoubleAttr_ObjVal);

            double gap = model.get(GRB_DoubleAttr_MIPGap) * 100;

            double tempo = model.get(GRB_DoubleAttr_Runtime);

            double fo = model.get(GRB_DoubleAttr_ObjVal);

            fprintf(arquivo, "FO: %.6f\n", fo);

            fprintf(arquivo,"LB: %.6f\n", lb);

            fprintf(arquivo,"UB: %.6f\n", ub);

            fprintf(arquivo,"GAP (%%): %.6f\n", gap);

            fprintf(arquivo,"TEMPO (seg.): %.6f\n", tempo);


            fprintf(arquivo, "\n");
            fprintf(arquivo, "SOLUCAO DO PROBLEMA DE CORTE\n");


            int totalObjetos = 0;

            // Percorre todos os padrões de corte
            for (int i = 0; i < numLinhas; i++) {

                // Nome da variável x correspondente ao padrão
                char nomeVariavel[20];
                sprintf(nomeVariavel, "x%d", i + 1);

                // Pega o valor da variável x
                GRBVar x = model.getVarByName(nomeVariavel);

                double frequencia = x.get(GRB_DoubleAttr_X);

                // Só imprime padrões que foram utilizados
                    if (frequencia > 0.000001) {

                        // Soma a frequência do padrão ao total de objetos
                        totalObjetos += (int)frequencia;

                        fprintf(arquivo, "Padrao %d\n", i + 1);

                        fprintf(arquivo, "Itens: ");

                        // Imprime a composição do padrão
                        for (int j = 0; j < numCortes; j++) {
                            fprintf(arquivo, "%d ", matrizPadroes[i][j]);
                        }

                        fprintf(arquivo, "\n");

                        fprintf(arquivo, "Frequencia: %.0f\n", frequencia);

                        fprintf(arquivo, "\n");
                    }
            }
            fprintf(arquivo, "TOTAL DE OBJETOS: %d\n", totalObjetos);

        }

        else {
            fprintf(arquivo,"NAO FOI ENCONTRADA UMA SOLUCAO.\n");
        }


        fclose(arquivo);

    }

    catch (GRBException e) {

        FILE *erro = fopen("erro.txt", "w");

        if (erro != NULL) {

            fprintf(erro,"Erro no Gurobi: %d\n",e.getErrorCode());

            fprintf(erro,"%s\n",e.getMessage().c_str());

            fclose(erro);
        }

        return 1;
    }





    for(int i=0;i<numLinhas;i++){
        free(matrizPadroes[i]);
    }

    free(matrizPadroes);

    fclose(arq);

    return 0;
}



