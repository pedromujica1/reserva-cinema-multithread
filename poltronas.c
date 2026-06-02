#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <unistd.h>

#define TAM 10

volatile int buffer[TAM];
volatile int count = 0;
sem_t semid;

//verSessoesDisponiveis(): Percorre pela lista de sesões e verifica se existem Poltronas que ainda podem ser compradas
//verificarPoltronas(): percorre pelo vetor para verificar se existem cadeiras disponiveis, se não tem, não mostra filme
//atualizarPoltronasDisponiveis(): apos a confirmação de que a poltrona foi confirmada, o vetor de poltronas disponiveis deve ser atualizado
//reservarPoltrona(): Atualiza vetor de cadeiras reservadas
//cancelarReserva()





void *produtor(void *arg) {
    int in = 0;

    for (int i = 0; i < 1000; i++) {

        while (count == TAM);

        //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        buffer[in] = in;
        in = (in + 1) % TAM;
        sem_wait(&semid);

        count++;
        sem_post(&semid);
        printf("Produtor -> count %d\n", count);
    }
    return NULL;
}

void *consumidor(void *arg) {
    int dado, out = 0;

    for (int i = 0; i < 1000; i++) {

        while (count == 0);


        dado = buffer[out];
        out = (out + 1) % TAM;
        sem_wait(&semid);
        count--;
        sem_post(&semid);

        printf("Consumidor -> count %d | dado %d\n", count, dado);
       
    }
    return NULL;
}

int main() {
    sem_init(&semid, 0, 1);

    pthread_t p1, p2;

    pthread_create(&p1, NULL, produtor, NULL);
    pthread_create(&p2, NULL, consumidor, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    sem_destroy(&semid);

    return 0;
}