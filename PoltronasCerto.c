#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TAM_CLASSE 5
#define TAM_TOTAL 10

void copiarPoltronas(bool primeiraClasse[], bool economica[], bool resultado[]);

void mostrarPoltronas(bool resultado[]);

void reservarPrimeiraClasse(bool primeiraClasse[], bool economica[], bool resultado[], int *index1);

void reservarClasseEconomica(bool primeiraClasse[], bool economica[], bool resultado[], int *index2);

int tratarPrimeiraClasseLotada(bool primeiraClasse[], bool economica[],bool resultado[],int *index2);

int tratarEconomicaLotada(bool primeiraClasse[],bool economica[],bool resultado[],int *index1);

void menu();

int main(){
    bool primeiraClasse[TAM_CLASSE] = {false};
    bool economica[TAM_CLASSE] = {false};
    bool resultado[TAM_TOTAL] = {false};

    int index1 = 0;
    int index2 = 0;

    int opcao;

    while (1){
        menu();
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            if (index1 < TAM_CLASSE)
            {
                reservarPrimeiraClasse(primeiraClasse,economica,resultado,&index1);
            }
            else{
                if (!tratarPrimeiraClasseLotada(primeiraClasse,economica,resultado,&index2)){
                    break;
                }
            }
        }
        else if (opcao == 2)
        {
            if (index2 < TAM_CLASSE){
                reservarClasseEconomica(primeiraClasse,economica,resultado,&index2);
            }
            else{
                if (!tratarEconomicaLotada(primeiraClasse,economica,resultado,&index1)){
                    break;
                }
            }
        }
    }
    return 0;
}

void menu(){
    printf("\n========================\n");
    printf("1 - Primeira Classe\n");
    printf("2 - Classe Economica\n");
    printf("========================\n");
    printf("Escolha: ");
}

void copiarPoltronas(bool primeiraClasse[],bool economica[],bool resultado[]){
    int i;

    for (i = 0; i < TAM_CLASSE; i++)
        resultado[i] = primeiraClasse[i];

    for (i = 0; i < TAM_CLASSE; i++)
        resultado[i + TAM_CLASSE] = economica[i];
}

void mostrarPoltronas(bool resultado[]){
    int i;

    printf("Poltronas: [");

    for (i = 0; i < TAM_TOTAL; i++){
        printf("%d", resultado[i]);

        if (i < TAM_TOTAL - 1)
            printf(", ");
    }
    printf("]\n");
}

void reservarPrimeiraClasse(bool primeiraClasse[],bool economica[],bool resultado[],int *index1){
    primeiraClasse[*index1] = true;

    (*index1)++;

    copiarPoltronas(primeiraClasse,economica,resultado);

    printf("\nVoce escolheu a Primeira Classe\n");
    printf("Poltrona atribuida: %d\n", *index1);

    mostrarPoltronas(resultado);
}

void reservarClasseEconomica(bool primeiraClasse[],bool economica[],bool resultado[],int *index2){
    economica[*index2] = true;

    (*index2)++;

    copiarPoltronas(primeiraClasse,economica,resultado);

    printf("\nVoce escolheu a Classe Economica\n");
    printf("Poltrona atribuida: %d\n",(*index2) + 5);
    mostrarPoltronas(resultado);
}

int tratarPrimeiraClasseLotada(bool primeiraClasse[],bool economica[],bool resultado[],int *index2){
    char resposta;

    printf("\nPrimeira Classe lotada!\n");
    printf("Deseja ir para Classe Economica? (s/n): ");

    scanf(" %c", &resposta);

    if (resposta == 's')
    {
        if (*index2 < TAM_CLASSE)
        {
            reservarClasseEconomica(
                primeiraClasse,
                economica,
                resultado,
                index2);
        }
        else{
            printf("Classe Economica tambem esta lotada.\n");
        }

        return 1;
    }

    printf("Proximo voo parte em 3 horas.\n");
    return 0;
}

int tratarEconomicaLotada(bool primeiraClasse[], bool economica[], bool resultado[],  int *index1){
    char resposta;

    printf("\nClasse Economica lotada!\n");
    printf("Deseja ir para Primeira Classe? (s/n): ");

    scanf(" %c", &resposta);

    if (resposta == 's')
    {
        if (*index1 < TAM_CLASSE){
            reservarPrimeiraClasse(primeiraClasse,economica,resultado,index1);
        }
        else{
            printf("Primeira Classe tambem esta lotada.\n");
        }

        return 1;
    }
    printf("Proximo voo parte em 3 horas.\n");
    return 0;
}