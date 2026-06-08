#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define LINHAS 7
#define COLS 5

// IDs das salas
#define ID_SALA_VIP -1
#define ID_SALA_NORMAL 1

// Quantidade de clientes simulados
#define TOTAL_CLIENTES 50

// Matrizes para representar o mapa de cada sala
bool salaVIP[LINHAS][COLS] = {false};
bool salaNormal[LINHAS][COLS] = {false};

//Contadores globais de assentos ocupados
int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

// Mutexes para proteger cada sala separadamente
pthread_mutex_t mutexVIP = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexNormal = PTHREAD_MUTEX_INITIALIZER;

// Estrutura para passar dados para cada thread
typedef struct {
    int cliente_id;
} ClienteArgs;

// Função auxiliar para selecionar a sala, contador e mutex corretos
void selecionarSala(int idSala, bool (**sala)[COLS], int **contadorOcupados, pthread_mutex_t **mutexSala
) {
    if (idSala == ID_SALA_VIP) {
        *sala = salaVIP;
        *contadorOcupados = &ocupadosVIP;
        *mutexSala = &mutexVIP;
    } else {
        *sala = salaNormal;
        *contadorOcupados = &ocupadosNormal;
        *mutexSala = &mutexNormal;
    }
}

// Imprime o estado atual de ocupação de uma sala
void mostrarMapaSala(int idSala) {
    
    bool (*sala)[COLS];
    int ocupados;
    pthread_mutex_t *mutexSala;

    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        ocupados = ocupadosVIP;
        mutexSala = &mutexVIP;
    } else if (idSala == ID_SALA_NORMAL) {
        sala = salaNormal;
        ocupados = ocupadosNormal;
        mutexSala = &mutexNormal;
    } else {
        printf("ID de sala invalido: %d\n", idSala);
        return;
    }

    pthread_mutex_lock(mutexSala);

    printf("\n--- Mapa Atual da Sala (ID: %d) ---\n", idSala);

    for (int i = 0; i < LINHAS; i++) {
        printf("Fileira %d: ", i + 1);

        for (int j = 0; j < COLS; j++) {
            printf("[%c] ", sala[i][j] ? 'X' : ' ');
        }

        printf("\n");
    }

    printf("Total Ocupados: %d/%d\n", ocupados, capacidadeTotal);
    printf("-----------------------------------\n");

    pthread_mutex_unlock(mutexSala);
}

// RESERVAR: tenta ocupar uma poltrona aleatória disponível
void reservarAssentoAleatorio(int idSala, int cliente_id) {
    bool (*sala)[COLS];
    int *contadorOcupados;
    pthread_mutex_t *mutexSala;

    selecionarSala(idSala, &sala, &contadorOcupados, &mutexSala);

    pthread_mutex_lock(mutexSala);

    if (*contadorOcupados >= capacidadeTotal) {
        printf("Sala %d cheia! Cliente %d nao conseguiu vaga\n",
               idSala, cliente_id);

        pthread_mutex_unlock(mutexSala);
        return;
    }

    int l;
    int c;

    /*
        Enquanto houver vaga, sorteia até encontrar
        uma poltrona livre.
    */
    do {
        l = rand() % LINHAS;
        c = rand() % COLS;
    } while (sala[l][c] == true);

    sala[l][c] = true;
    (*contadorOcupados)++;

    printf("Cliente %d RESERVOU assento [%d, %d] na Sala %d\n",
           cliente_id, l + 1, c + 1, idSala);

    pthread_mutex_unlock(mutexSala);
}

// CANCELAR RESERVA: libera uma poltrona aleatória ocupada
void cancelarReservaAleatoria(int idSala, int cliente_id) {
    bool (*sala)[COLS];
    int *contadorOcupados;
    pthread_mutex_t *mutexSala;

    selecionarSala(idSala, &sala, &contadorOcupados, &mutexSala);

    pthread_mutex_lock(mutexSala);

    if (*contadorOcupados <= 0) {
        printf("Sala %d vazia! Cliente %d nao tinha reserva para cancelar\n",
               idSala, cliente_id);

        pthread_mutex_unlock(mutexSala);
        return;
    }

    int l;
    int c;

    /*
        Enquanto houver assento ocupado, sorteia até encontrar
        uma poltrona ocupada para cancelar.
    */
    do {
        l = rand() % LINHAS;
        c = rand() % COLS;
    } while (sala[l][c] == false);

    sala[l][c] = false;
    (*contadorOcupados)--;

    printf("Cliente %d CANCELOU assento [%d, %d] na Sala %d\n",
           cliente_id, l + 1, c + 1, idSala);

    pthread_mutex_unlock(mutexSala);
}

// AÇÃO: alterna as operações simuladas
void acaoClienteSimulado(int cliente_id) {
    int idSala;

    if (rand() % 2 == 0) {
        idSala = ID_SALA_VIP;
    } else {
        idSala = ID_SALA_NORMAL;
    }

    int operacao = rand() % 2; // 0 = reservar, 1 = cancelar

    if (operacao == 0) {
        reservarAssentoAleatorio(idSala, cliente_id);
    } else {
        cancelarReservaAleatoria(idSala, cliente_id);
    }
}

// Função executada por cada thread
void *threadCliente(void *arg) {
    ClienteArgs *dados = (ClienteArgs *) arg;

    acaoClienteSimulado(dados->cliente_id);

    pthread_exit(NULL);
}

// VERIFICA INTEGRIDADE: compara contadores com as matrizes
void verifica_integridade() {
    int somaVIP = 0;
    int somaNormal = 0;

    /*
        Travamos os dois mutexes para garantir que ninguém
        esteja alterando as salas durante a auditoria.
    */
    pthread_mutex_lock(&mutexVIP);
    pthread_mutex_lock(&mutexNormal);

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (salaVIP[i][j]) {
                somaVIP++;
            }

            if (salaNormal[i][j]) {
                somaNormal++;
            }
        }
    }

    printf("\n[AUDITORIA DE INTEGRIDADE]\n");

    printf("Sala VIP (ID: %d)    -> Contador: %d | Fisico na Matriz: %d -> %s\n",
           ID_SALA_VIP,
           ocupadosVIP,
           somaVIP,
           (ocupadosVIP == somaVIP) ? "OK" : "CORROMPIDO");

    printf("Sala Normal (ID: %d) -> Contador: %d | Fisico na Matriz: %d -> %s\n",
           ID_SALA_NORMAL,
           ocupadosNormal,
           somaNormal,
           (ocupadosNormal == somaNormal) ? "OK" : "CORROMPIDO");

    pthread_mutex_unlock(&mutexNormal);
    pthread_mutex_unlock(&mutexVIP);
}

int main() {
    srand(42);

    pthread_t threads[TOTAL_CLIENTES];
    ClienteArgs clientes[TOTAL_CLIENTES];

    printf("=== INICIANDO SIMULACAO MULTITHREAD DE BILHETERIA ===\n");
    printf("Simulando %d clientes acessando o sistema...\n\n", TOTAL_CLIENTES);

    // Cria as threads
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        clientes[i].cliente_id = i + 1;

        int resultado = pthread_create(
            &threads[i],
            NULL,
            threadCliente,
            &clientes[i]
        );

        if (resultado != 0) {
            printf("Erro ao criar thread do cliente %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < TOTAL_CLIENTES; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n===================================================\n");
    printf("===          FIM DA SIMULACAO MULTITHREAD       ===\n");
    printf("===================================================\n");

    mostrarMapaSala(ID_SALA_VIP);
    mostrarMapaSala(ID_SALA_NORMAL);

    verifica_integridade();

    pthread_mutex_destroy(&mutexVIP);
    pthread_mutex_destroy(&mutexNormal);

    return 0;
}