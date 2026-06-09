#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define LINHAS 7
#define COLS 5

// IDs das salas
#define ID_SALA_VIP -1
#define ID_SALA_NORMAL 1

#define TOTAL_CLIENTES 10
#define MAX_CLIENTES 100

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

// Contadores globais de assentos ocupados
int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

/*
    Semaforos usados como mutex.

    Valor inicial 1:
    - uma thread entra na secao critica
    - as outras esperam
*/
sem_t semVIP;
sem_t semNormal;

/*
    Semaforo para proteger o rand().

    Como varias threads podem chamar rand() ao mesmo tempo,
    usamos um semaforo separado para evitar acesso simultaneo.
*/
sem_t semRandom;

int numeroAleatorio(int limite) {
    int valor;

    sem_wait(&semRandom);
    valor = rand() % limite;
    sem_post(&semRandom);

    return valor;
}

// Imprime o estado atual de ocupacao de uma sala usando o ID numerico
void mostrarMapaSala(int idSala) {
    if (idSala == ID_SALA_VIP) {
        sem_wait(&semVIP);

        printf("=========================================\n");
        printf("SALA VIP\n");

        for (int i = 0; i < LINHAS; i++) {
            printf("F%02d | ", i + 1);

            for (int j = 0; j < COLS; j++) {
                if (salaVIP[i][j] == 0)
                    printf("[--] ");
                else
                    printf("[%02d] ", salaVIP[i][j]);
            }

            printf("\n");
        }

        printf("\nOcupacao total: %d/%d\n", ocupadosVIP, capacidadeTotal);
        printf("=========================================\n");

        sem_post(&semVIP);
    }

    else if (idSala == ID_SALA_NORMAL) {
        sem_wait(&semNormal);

        printf("=========================================\n");
        printf("SALA NORMAL\n");

        for (int i = 0; i < LINHAS; i++) {
            printf("F%02d | ", i + 1);

            for (int j = 0; j < COLS; j++) {
                if (salaNormal[i][j] == 0)
                    printf("[--] ");
                else
                    printf("[%02d] ", salaNormal[i][j]);
            }

            printf("\n");
        }

        printf("\nOcupacao total: %d/%d\n", ocupadosNormal, capacidadeTotal);
        printf("=========================================\n");

        sem_post(&semNormal);
    }

    else {
        printf("ID de sala invalido: %d\n", idSala);
    }
}

// Reserva poltrona aleatoria
void reservarAssentoAleatorio(int idSala, Cinefilo *cinefilo) {
    int tentativas = 0;

    /*
        Como cada cinefilo pertence a uma unica thread nesta simulacao,
        essa verificacao nao precisa de semaforo proprio.
    */
    if (cinefilo->possuiReserva) {
        printf("Cinefilo %d ja possui uma reserva.\n", cinefilo->id);
        return;
    }

    if (idSala == ID_SALA_VIP) {
        sem_wait(&semVIP);

        if (ocupadosVIP >= capacidadeTotal) {
            printf("Sala VIP cheia!\n");

            sem_post(&semVIP);
            return;
        }

        while (tentativas < 50) {
            int l = numeroAleatorio(LINHAS);
            int c = numeroAleatorio(COLS);

            if (salaVIP[l][c] == 0) {
                salaVIP[l][c] = cinefilo->id;

                cinefilo->possuiReserva = true;
                cinefilo->salaReservada = ID_SALA_VIP;
                cinefilo->linha = l;
                cinefilo->coluna = c;

                ocupadosVIP++;

                printf("[RESERVA]\n"
                       "Cliente : %d\n"
                       "Sala    : VIP (%d)\n"
                       "Assento : [%d,%d]\n"
                       "Status  : SUCESSO\n\n",
                       cinefilo->id,
                       ID_SALA_VIP,
                       l + 1,
                       c + 1);

                sem_post(&semVIP);
                return;
            }

            tentativas++;
        }

        printf("Cliente %d nao encontrou assento livre na Sala VIP.\n",
               cinefilo->id);

        sem_post(&semVIP);
    }

    else if (idSala == ID_SALA_NORMAL) {
        sem_wait(&semNormal);

        if (ocupadosNormal >= capacidadeTotal) {
            printf("Sala Normal cheia!\n");

            sem_post(&semNormal);
            return;
        }

        while (tentativas < 50) {
            int l = numeroAleatorio(LINHAS);
            int c = numeroAleatorio(COLS);

            if (salaNormal[l][c] == 0) {
                salaNormal[l][c] = cinefilo->id;

                cinefilo->possuiReserva = true;
                cinefilo->salaReservada = ID_SALA_NORMAL;
                cinefilo->linha = l;
                cinefilo->coluna = c;

                ocupadosNormal++;

                printf("[RESERVA]\n"
                       "Cliente : %d\n"
                       "Sala    : NORMAL (%d)\n"
                       "Assento : [%d,%d]\n"
                       "Status  : SUCESSO\n\n",
                       cinefilo->id,
                       ID_SALA_NORMAL,
                       l + 1,
                       c + 1);

                sem_post(&semNormal);
                return;
            }

            tentativas++;
        }

        printf("Cliente %d nao encontrou assento livre na Sala Normal.\n",
               cinefilo->id);

        sem_post(&semNormal);
    }

    else {
        printf("ID de sala invalido: %d\n", idSala);
    }
}

void cancelarReserva(Cinefilo *cinefilo) {
    if (!cinefilo->possuiReserva) {
        printf("Cinefilo %d nao possui uma reserva.\n", cinefilo->id);
        return;
    }

    if (cinefilo->salaReservada == ID_SALA_VIP) {
        sem_wait(&semVIP);

        salaVIP[cinefilo->linha][cinefilo->coluna] = 0;
        ocupadosVIP--;

        printf("[CANCELAMENTO]\n"
               "Cliente : %d\n"
               "Sala    : VIP (%d)\n"
               "Assento : [%d,%d]\n"
               "Status  : SUCESSO\n\n",
               cinefilo->id,
               ID_SALA_VIP,
               cinefilo->linha + 1,
               cinefilo->coluna + 1);

        cinefilo->possuiReserva = false;
        cinefilo->salaReservada = 0;
        cinefilo->linha = -1;
        cinefilo->coluna = -1;

        sem_post(&semVIP);
    }

    else if (cinefilo->salaReservada == ID_SALA_NORMAL) {
        sem_wait(&semNormal);

        salaNormal[cinefilo->linha][cinefilo->coluna] = 0;
        ocupadosNormal--;

        printf("[CANCELAMENTO]\n"
               "Cliente : %d\n"
               "Sala    : NORMAL (%d)\n"
               "Assento : [%d,%d]\n"
               "Status  : SUCESSO\n\n",
               cinefilo->id,
               ID_SALA_NORMAL,
               cinefilo->linha + 1,
               cinefilo->coluna + 1);

        cinefilo->possuiReserva = false;
        cinefilo->salaReservada = 0;
        cinefilo->linha = -1;
        cinefilo->coluna = -1;

        sem_post(&semNormal);
    }

    else {
        printf("Reserva invalida para o cinefilo %d.\n", cinefilo->id);
    }
}

// Verifica integridade comparando os contadores com as matrizes
void verifica_integridade() {
    int somaVIP = 0;
    int somaNormal = 0;

    /*
        Trava as duas salas para auditar um estado consistente.
    */
    sem_wait(&semVIP);
    sem_wait(&semNormal);

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

    sem_post(&semNormal);
    sem_post(&semVIP);
}

void acaoClienteSimulado(Cinefilo *cinefilo) {
    int idSala;
    int operacao;

    if (numeroAleatorio(2) == 0)
        idSala = ID_SALA_VIP;
    else
        idSala = ID_SALA_NORMAL;

    operacao = numeroAleatorio(2);

    if (operacao == 0)
        reservarAssentoAleatorio(idSala, cinefilo);
    else
        cancelarReserva(cinefilo);
}

/*
    Funcao executada por cada thread.
*/
void *threadCliente(void *arg) {
    Cinefilo *cinefilo = (Cinefilo *) arg;

    acaoClienteSimulado(cinefilo);

    return NULL;
}

int main() {
    srand(42);

    pthread_t threads[TOTAL_CLIENTES];
    Cinefilo clientes[MAX_CLIENTES];

    /*
        Inicializacao dos semaforos.

        O segundo parametro 0 indica uso entre threads do mesmo processo.
        O valor 1 faz o semaforo funcionar como mutex.
    */
    sem_init(&semVIP, 0, 1);
    sem_init(&semNormal, 0, 1);
    sem_init(&semRandom, 0, 1);

    for (int i = 0; i < MAX_CLIENTES; i++) {
        clientes[i].id = i + 1;
        clientes[i].possuiReserva = false;
        clientes[i].linha = -1;
        clientes[i].coluna = -1;
        clientes[i].salaReservada = 0;
    }

    printf("=========================================\n");
    printf(" SISTEMA DE BILHETERIA - MODO MULTITHREAD\n");
    printf("          SINCRONIZACAO COM SEMAFOROS\n");
    printf("=========================================\n");

    /*
        Cada cliente sera representado por uma thread.
    */
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        printf("\nCriando thread do cliente %d\n", clientes[i].id);

        if (pthread_create(&threads[i], NULL, threadCliente, &clientes[i]) != 0) {
            printf("Erro ao criar thread do cliente %d\n", clientes[i].id);
            exit(EXIT_FAILURE);
        }
    }

    /*
        Espera todas as threads terminarem.
    */
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\n=========================================\n");
    printf("     FIM DA SIMULACAO MULTITHREAD\n");
    printf("=========================================\n");

    mostrarMapaSala(ID_SALA_VIP);
    mostrarMapaSala(ID_SALA_NORMAL);

    verifica_integridade();

    sem_destroy(&semVIP);
    sem_destroy(&semNormal);
    sem_destroy(&semRandom);

    return 0;
}
