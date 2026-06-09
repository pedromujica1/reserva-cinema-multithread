#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define LINHAS 7
#define COLS 5

//IDs das salas
#define ID_SALA_VIP -1
#define ID_SALA_NORMAL 1

typedef struct {
    int id;
    int salaReservada;
    int linha;
    int coluna;
    bool possuiReserva;
} Cinefilo;

//matrizes para mapear salas fisicas do cinema
int salaVIP[LINHAS][COLS] = {0};
int salaNormal[LINHAS][COLS] = {0};

//contadores globais de assentos ocupados
int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

//imprime o estado atual de ocupação de uma sala usando o ID numérico
void mostrarMapaSala(int idSala) {
    int (*sala)[COLS];
    int ocupados;
   
    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        ocupados = ocupadosVIP;
    } else {
        sala = salaNormal;
        ocupados = ocupadosNormal;
    }
    printf("=========================================\n");
    if (idSala == ID_SALA_VIP)
        printf("SALA VIP\n");
    else
        printf("SALA NORMAL\n");

    for (int i = 0; i < LINHAS; i++) {

        printf("F%02d | ", i + 1);

        for (int j = 0; j < COLS; j++) {

            if (sala[i][j] == 0)
                printf("[--] ");
            else
                printf("[%02d] ", sala[i][j]);
        }

        printf("\n");
    }
    printf("\nOcupação total: %d/%d\n",ocupados,capacidadeTotal);

    printf("=========================================\n");
}

//reserva poltrona aleatoria
void reservarAssentoAleatorio(int idSala, Cinefilo *cinefilo) {

    //se cinefilo, já tem reserva, acaba execução
    if (cinefilo->possuiReserva){
        printf("Cinéfilo %d já possui uma reserva .\n",cinefilo->id);
        return;
    }
    int (*sala)[COLS];
    int *contadorOcupados;

    //define as matrizes da sala e o contador de ocupados
    if (idSala == ID_SALA_VIP){
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }
    //verifica se sala está cheia
    if(*contadorOcupados >= capacidadeTotal){
        printf("Sala %d cheia!\n",idSala);
        return;
    }

    //tentativas maximas para cenario de falha
    int tentativas = 0;
    while (tentativas < 50){
        //definindo assentos aleatorios aka posições da matriz
        int l = rand() % LINHAS;
        int c = rand() % COLS;

        //se assento ocupado é igual a 0, significa q o assento está livre
        if(sala[l][c] == 0){

            //atualização de matriz como valor do id do cliente
            sala[l][c] = cinefilo->id;
            //atualização do struct do cinefilo
            cinefilo->possuiReserva = true;
            cinefilo->salaReservada = idSala;
            cinefilo->linha = l;
            cinefilo->coluna = c;

            //atualização de contador de ocupados
            (*contadorOcupados)++;

            printf("[RESERVA]\n" "Cliente : %d\n" "Sala    : %d\n" "Assento : [%d,%d]\n" "Status  : SUCESSO\n\n", cinefilo->id,idSala,l + 1,c + 1);
            return;
        }
        else{
            printf("Cliente %d não encontrou assento livre.\n",cinefilo->id);
        }
        tentativas++;
    }
}

void cancelarReserva(Cinefilo *cinefilo) {
    //se cinefilo, não possui reserva, acaba execução
    if (!cinefilo->possuiReserva){
        printf("Cinéfilo %d não possui uma reserva .\n",cinefilo->id);
        return;
    }

    int (*sala)[COLS];
    int *contadorOcupados;

    // Substituído o operador ternário por estrutura if-else
    if (cinefilo->salaReservada == ID_SALA_VIP) {
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }

    //troca valor da matriz por 0
    sala[cinefilo->linha][cinefilo->coluna] = 0;

    //atualiza contador
    (*contadorOcupados)--;

    printf("[CANCELAMENTO]\n" "Cliente : %d\n" "Sala    : %d\n" "Assento : [%d,%d]\n" "Status  : SUCESSO\n\n",cinefilo->id,cinefilo->salaReservada, cinefilo->linha + 1,cinefilo->coluna + 1);

    
    cinefilo->possuiReserva = false;
}

// VERIFICA INTEGRIDADE: Valida a consistência matemática comparando os contadores com as matrizes
void verifica_integridade() {
    int somaVIP = 0;
    int somaNormal = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (salaVIP[i][j]) somaVIP++;
            if (salaNormal[i][j]) somaNormal++;
        }
    }

    printf("\n[AUDITORIA DE INTEGRIDADE]\n");
    printf("Sala VIP (ID: %d)    -> Contador: %d | Fisico na Matriz: %d -> %s\n", ID_SALA_VIP, ocupadosVIP, somaVIP, (ocupadosVIP == somaVIP) ? "OK" : "CORROMPIDO");
    printf("Sala Normal (ID: %d) -> Contador: %d | Fisico na Matriz: %d -> %s\n", ID_SALA_NORMAL, ocupadosNormal, somaNormal, (ocupadosNormal == somaNormal) ? "OK" : "CORROMPIDO");
}

void acaoClienteSimulado(Cinefilo *cinefilo) {
    int idSala;

    if (rand() % 2 == 0)
        idSala = ID_SALA_VIP;
    else
        idSala = ID_SALA_NORMAL;

    int operacao = rand() % 2;

    if (operacao == 0)
        reservarAssentoAleatorio(idSala, cinefilo);
    else
        cancelarReserva(cinefilo);
}

int main() {

    srand(42);

    int totalClientesParaTestar = 10;

    Cinefilo clientes[100];

    for (int i = 0; i < 100; i++) {

        clientes[i].id = i + 1;
        clientes[i].possuiReserva = false;
        clientes[i].linha = -1;
        clientes[i].coluna = -1;
        clientes[i].salaReservada = 0;
    }

    printf("=========================================\n");
    printf(" SISTEMA DE BILHETERIA - MODO SEQUENCIAL\n");
    printf("=========================================\n");

    for (int i = 0; i < totalClientesParaTestar; i++) {

        printf("\n-----------------------------------------\n");
        printf("AÇÃO %d\n", i + 1);
        printf("-----------------------------------------\n");

        acaoClienteSimulado(&clientes[i]);
    }

    printf("\n\n=========================================\n");
    printf("     FIM DA SIMULAÇÃO SEQUENCIAL\n");
    printf("=========================================\n");

    mostrarMapaSala(ID_SALA_VIP);
    mostrarMapaSala(ID_SALA_NORMAL);

    verifica_integridade();

    return 0;
}