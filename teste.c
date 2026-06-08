#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 3
#define COLS 4
#define RANDOM_MAX 100 // Limite de controle para sorteios, se necessário

// Cada matriz representa o mapa físico completo de uma sala distinta
bool salaVIP[LINHAS][COLS] = {false};
bool salaNormal[LINHAS][COLS] = {false};

// Contadores globais de assentos ocupados
int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

// Estrutura para controle de clientes se necessário mapear IDs futuros
typedef struct {
    int id;
} cliente;

// Imprime o estado atual de ocupação de uma sala
void mostrarMapaSala(bool sala[LINHAS][COLS], const char* nomeSala) {
    printf("\n--- Mapa Atual da Sala %s ---\n", nomeSala);
    for (int i = 0; i < LINHAS; i++) {
        printf("Fileira %d: ", i + 1);
        for (int j = 0; j < COLS; j++) {
            printf("[%d] ", sala[i][j]); 
        }
        printf("\n");
    }
    printf("Total Ocupados: %d/%d\n", 
           (strcmp(nomeSala, "VIP") == 0) ? ocupadosVIP : ocupadosNormal, 
           capacidadeTotal);
    printf("-------------------------------\n");
}

// RESERVAR (Equivalente ao Sacar): Tenta ocupar uma poltrona aleatória
void reservarAssentoAleatorio(bool sala[LINHAS][COLS], int *contadorOcupados, const char* nomeSala, int cliente_id, unsigned int *seed) {
    // Sorteia fileira e coluna baseado nas dimensões da matriz
    int l = rand_r(seed) % LINHAS;
    int c = rand_r(seed) % COLS;

    if (*contadorOcupados < capacidadeTotal) {
        if (sala[l][c] == false) {
            sala[l][c] = true;
            (*contadorOcupados)++;
            printf("Cliente %d RESERVOU assento [%d, %d] na Sala %s\n", cliente_id, l + 1, c + 1, nomeSala);
        } else {
            printf("Cliente %d tentou assento [%d, %d] na Sala %s, mas ja estava ocupado\n", cliente_id, l + 1, c + 1, nomeSala);
        }
    } else {
        printf("Sala %s cheia! Cliente %d nao conseguiu vaga\n", nomeSala, cliente_id);
    }
}

// CANCELAR RESERVA (Equivalente ao Depositar): Libera uma poltrona aleatória ocupada
void cancelarReservaAleatoria(bool sala[LINHAS][COLS], int *contadorOcupados, const char* nomeSala, int cliente_id, unsigned int *seed) {
    int l = rand_r(seed) % LINHAS;
    int c = rand_r(seed) % COLS;

    if (sala[l][c] == true) {
        sala[l][c] = false;
        (*contadorOcupados)--;
        printf("Cliente %d CANCELOU assento [%d, %d] na Sala %s\n", cliente_id, l + 1, c + 1, nomeSala);
    } else {
        printf("Cliente %d tentou cancelar assento [%d, %d] na Sala %s, mas ja estava livre\n", cliente_id, l + 1, c + 1, nomeSala);
    }
}

// TROCAR ASSENTO (Equivalente à Transação): Move uma reserva de um lugar para outro da mesma sala
void trocarAssentoAleatorio(bool sala[LINHAS][COLS], const char* nomeSala, unsigned int *seed) {
    int l_origem = rand_r(seed) % LINHAS;
    int c_origem = rand_r(seed) % COLS;
    
    int l_destino = rand_r(seed) % LINHAS;
    int c_destino = rand_r(seed) % COLS;

    // Impede a troca para o mesmo lugar
    if (l_origem == l_destino && c_origem == c_destino) return;

    // Se a origem estiver ocupada e o destino livre, realiza a troca
    if (sala[l_origem][c_origem] == true && sala[l_destino][c_destino] == false) {
        sala[l_origem][c_origem] = false;
        sala[l_destino][c_destino] = true;
        printf("Troca de assento na Sala %s de [%d, %d] para [%d, %d]\n", nomeSala, l_origem + 1, c_origem + 1, l_destino + 1, c_destino + 1);
    }
}

// VERIFICA INTEGRIDADE (Equivalente ao Verifica): Valida a consistência matemática dos contadores
void verifica_integridade() {
    int somaVIP = 0;
    int somaNormal = 0;

    // Varre fisicamente a matriz VIP
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (salaVIP[i][j]) somaVIP++;
        }
    }

    // Varre fisicamente a matriz Normal
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (salaNormal[i][j]) somaNormal++;
        }
    }

    printf("\n[AUDITORIA DE INTEGRIDADE]\n");
    printf("Sala VIP -> Contador: %d | Fisico na Matriz: %d -> %s\n", ocupadosVIP, somaVIP, (ocupadosVIP == somaVIP) ? "OK" : "CORROMPIDO");
    printf("Sala Normal -> Contador: %d | Fisico na Matriz: %d -> %s\n", ocupadosNormal, somaNormal, (ocupadosNormal == somaNormal) ? "OK" : "CORROMPIDO");
}

// AÇÃO (Equivalente ao Acao): Rotina lógica que alterna entre operações por iteração
void acaoClienteSimulado(int cliente_id, unsigned int *seed) {
    int r = rand_r(seed) % 2; // Sorteia entre tentar Sala VIP ou Normal
    int operacao = rand_r(seed) % 2; // Sorteia entre reservar e cancelar

    if (r == 0) {
        if (operacao == 0)
            reservarAssentoAleatorio(salaVIP, &ocupadosVIP, "VIP", cliente_id, seed);
        else
            cancelarReservaAleatoria(salaVIP, &ocupadosVIP, "VIP", cliente_id, seed);
    } else {
        if (operacao == 0)
            reservarAssentoAleatorio(salaNormal, &ocupadosNormal, "Normal", cliente_id, seed);
        else
            cancelarReservaAleatoria(salaNormal, &ocupadosNormal, "Normal", cliente_id, seed);
    }
}