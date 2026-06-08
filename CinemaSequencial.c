#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define LINHAS 7
#define COLS 5

//IDs das salas
#define ID_SALA_VIP -1
#define ID_SALA_NORMAL 1

//matriz para representar o mapa de cada sala de cinema
bool salaVIP[LINHAS][COLS] = {false};
bool salaNormal[LINHAS][COLS] = {false};

//contadores globais de assentos ocupados
int ocupadosVIP = 0;
int ocupadosNormal = 0;
int capacidadeTotal = LINHAS * COLS;

//imprime o estado atual de ocupação de uma sala usando o ID numérico
void mostrarMapaSala(int idSala) {
    bool (*sala)[COLS];
    int ocupados;
    
    // Substituído o operador ternário por estrutura if-else
    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        ocupados = ocupadosVIP;
    } else {
        sala = salaNormal;
        ocupados = ocupadosNormal;
    }
    
    printf("\n--- Mapa Atual da Sala (ID: %d) ---\n", idSala);
    for (int i = 0; i < LINHAS; i++) {
        printf("Fileira %d: ", i + 1);
        for (int j = 0; j < COLS; j++) {
            printf("[%d] ", sala[i][j]); 
        }
        printf("\n");
    }
    printf("Total Ocupados: %d/%d\n", ocupados, capacidadeTotal);
    printf("-----------------------------------\n");
}

//reserva poltrona aleatoria
void reservarAssentoAleatorio(int idSala, int cliente_id) {
    bool (*sala)[COLS];
    int *contadorOcupados;

    // Substituído o operador ternário por estrutura if-else
    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }

    //sorteia fileira (0 a 6) e coluna (0 a 4)
    int l = rand() % LINHAS;
    int c = rand() % COLS;

    if (*contadorOcupados < capacidadeTotal) {
        if (sala[l][c] == false) {
            sala[l][c] = true;
            (*contadorOcupados)++;
            printf("Cliente %d RESERVOU assento [%d, %d] na Sala %d\n", cliente_id, l + 1, c + 1, idSala);
        } else {
            printf("Cliente %d tentou assento [%d, %d] na Sala %d, mas ja estava ocupado\n", cliente_id, l + 1, c + 1, idSala);
        }
    } else {
        printf("Sala %d cheia! Cliente %d nao conseguiu vaga\n", idSala, cliente_id);
    }
}

//libera uma poltrona aleatória que estava ocupada
void cancelarReservaAleatoria(int idSala, int cliente_id) {
    bool (*sala)[COLS];
    int *contadorOcupados;

    // Substituído o operador ternário por estrutura if-else
    if (idSala == ID_SALA_VIP) {
        sala = salaVIP;
        contadorOcupados = &ocupadosVIP;
    } else {
        sala = salaNormal;
        contadorOcupados = &ocupadosNormal;
    }

    int l = rand() % LINHAS;
    int c = rand() % COLS;

    if (sala[l][c] == true) {
        sala[l][c] = false;
        (*contadorOcupados)--;
        printf("Cliente %d CANCELOU assento [%d, %d] na Sala %d\n", cliente_id, l + 1, c + 1, idSala);
    } else {
        printf("Cliente %d tentou cancelar assento [%d, %d] na Sala %d, mas ja estava livre\n", cliente_id, l + 1, c + 1, idSala);
    }
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

void acaoClienteSimulado(int cliente_id) {
    int idSala;
    
    // Substituído o operador ternário por estrutura if-else
    if (rand() % 2 == 0) {
        idSala = ID_SALA_VIP;
    } else {
        idSala = ID_SALA_NORMAL;
    }
    
    int operacao = rand() % 2; // 0 para reservar, 1 para cancelar

    if (operacao == 0) {
        reservarAssentoAleatorio(idSala, cliente_id);
    } else {
        cancelarReservaAleatoria(idSala, cliente_id);
    }
}
int main() {
    //gerador de sementes fisicas
    srand(42);

    int totalClientesParaTestar = 6;

    printf("=== INICIANDO SIMULAÇÃO SEQUENCIAL DE BILHETERIA ===\n");
    printf("Simulando %d ações de clientes no sistema...\n\n", totalClientesParaTestar);

    // Loop que simula vários clientes entrando no sistema um após o outro
    for (int i = 1; i <= totalClientesParaTestar; i++) {
        printf("-----------------------------------\n");
        printf("Ação número: %d\n", i);
        
        // Passa o 'i' como ID do cliente para sabermos quem está agindo
        acaoClienteSimulado(i);
    }

    printf("\n===================================================\n");
    printf("===       FIM DA SIMULAÇÃO SEQUENCIAL       ===\n");
    printf("===================================================\n");

    //mostra mapas
    mostrarMapaSala(ID_SALA_VIP);
    mostrarMapaSala(ID_SALA_NORMAL);

    
    verifica_integridade();

    return 0;
}