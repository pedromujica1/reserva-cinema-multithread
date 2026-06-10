#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define LINHAS 7
#define COLS 5

// IDs das salas
#define ID_SALA_VIP -1
#define ID_SALA_NORMAL 1

#define MAX_CLIENTES 35
#define TOTAL_CLIENTES 10
#define ACOES_POR_CLIENTE 1

typedef struct {
    int id;
    int salaReservada;
    int linha;
    int coluna;
    bool possuiReserva;
} Cinefilo;

// Matrizes para mapear salas fisicas do cinema
int salaVIP[LINHAS][COLS] = {0};
int salaNormal[LINHAS][COLS] = {0};

int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

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

    printf("\nOcupacao total: %d/%d\n", ocupados, capacidadeTotal);
    printf("=========================================\n");
}

void reservarAssentoAleatorio(int idSala, Cinefilo *cinefilo) {
    if (cinefilo->possuiReserva) {
        printf("Cinefilo %d ja possui uma reserva.\n", cinefilo->id);
        return;
    }

    int (*sala)[COLS];
    int *contadorOcupados;

    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }


    if (*contadorOcupados >= capacidadeTotal) {
        printf("Sala %d cheia!\n", idSala);
        return;
    }

    int tentativas = 0;

    while (tentativas < 50) {
        int l = rand() % LINHAS;
        int c = rand() % COLS;

  
        if (sala[l][c] == 0) {

            usleep(1000);

            sala[l][c] = cinefilo->id;

            cinefilo->possuiReserva = true;
            cinefilo->salaReservada = idSala;
            cinefilo->linha = l;
            cinefilo->coluna = c;

            int valorAtual = *contadorOcupados;
            usleep(1000);
            *contadorOcupados = valorAtual + 1;

            printf("[RESERVA]\n"
                   "Cliente : %d\n"
                   "Sala    : %d\n"
                   "Assento : [%d,%d]\n"
                   "Status  : SUCESSO\n\n",
                   cinefilo->id,
                   idSala,
                   l + 1,
                   c + 1);

            return;
        }

        tentativas++;
    }

    printf("Cliente %d nao encontrou assento livre.\n", cinefilo->id);
}

void cancelarReserva(Cinefilo *cinefilo) {
    if (!cinefilo->possuiReserva) {
        printf("Cinefilo %d nao possui uma reserva.\n", cinefilo->id);
        return;
    }

    int (*sala)[COLS];
    int *contadorOcupados;

    if (cinefilo->salaReservada == ID_SALA_VIP) {
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }


    sala[cinefilo->linha][cinefilo->coluna] = 0;


    int valorAtual = *contadorOcupados;
    usleep(1000);
    *contadorOcupados = valorAtual - 1;

    printf("[CANCELAMENTO]\n"
           "Cliente : %d\n"
           "Sala    : %d\n"
           "Assento : [%d,%d]\n"
           "Status  : SUCESSO\n\n",
           cinefilo->id,
           cinefilo->salaReservada,
           cinefilo->linha + 1,
           cinefilo->coluna + 1);

    cinefilo->possuiReserva = false;
    cinefilo->salaReservada = 0;
    cinefilo->linha = -1;
    cinefilo->coluna = -1;
}

// Verifica integridade comparando contadores com as matrizes
void verifica_integridade() {
    int somaVIP = 0;
    int somaNormal = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (salaVIP[i][j] != 0)
                somaVIP++;

            if (salaNormal[i][j] != 0)
                somaNormal++;
        }
    }

    printf("\n[AUDITORIA DE INTEGRIDADE]\n");

    printf("Sala VIP    (ID:%d) -> Contador: %d | Fisico na Matriz: %d -> %s\n",
           ID_SALA_VIP,
           ocupadosVIP,
           somaVIP,
           (ocupadosVIP == somaVIP) ? "OK" : "CORROMPIDO");

    printf("Sala Normal (ID: %d) -> Contador: %d | Fisico na Matriz: %d -> %s\n",
           ID_SALA_NORMAL,
           ocupadosNormal,
           somaNormal,
           (ocupadosNormal == somaNormal) ? "OK" : "CORROMPIDO");
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


void *threadCliente(void *arg) {
    Cinefilo *cinefilo = (Cinefilo *) arg;

    for (int i = 0; i < ACOES_POR_CLIENTE; i++) {
        acaoClienteSimulado(cinefilo);

        
        usleep(1000);
    }

    return NULL;
}

int main() {
    srand(42);

    pthread_t threads[TOTAL_CLIENTES];
    Cinefilo clientes[MAX_CLIENTES];

    for (int i = 0; i < MAX_CLIENTES; i++) {
        clientes[i].id = i + 1;
        clientes[i].possuiReserva = false;
        clientes[i].linha = -1;
        clientes[i].coluna = -1;
        clientes[i].salaReservada = 0;
    }

    printf("=========================================\n");
    printf(" SISTEMA DE BILHETERIA - MULTITHREAD\n");
    printf("        SEM SINCRONIZACAO\n");
    printf("=========================================\n");

    clock_t inicio = clock();

    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        if (pthread_create(&threads[i], NULL, threadCliente, &clientes[i]) != 0) {
            printf("Erro ao criar thread do cliente %d\n", clientes[i].id);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t fim = clock();
    double tempoTotal = ((double)(fim-inicio))/CLOCKS_PER_SEC;

    printf("\n\n=========================================\n");
    printf("     FIM DA SIMULACAO MULTITHREAD\n");
    printf("        SEM SINCRONIZACAO\n");
    printf("=========================================\n");

    mostrarMapaSala(ID_SALA_VIP);
    mostrarMapaSala(ID_SALA_NORMAL);

    verifica_integridade();

    printf("Tempo de execução: %.6f segundos\n", tempoTotal);

    return 0;
}
