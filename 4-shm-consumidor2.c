#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SYSV_KEY 0x1234

typedef struct {
    int contador;
} shared_data_t;

int main() {
    int shmid = shmget(SHM_SYSV_KEY, sizeof(shared_data_t), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shared_data_t *ptr = (shared_data_t *) shmat(shmid, NULL, 0);
    if (ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Consumidor SYSV iniciado\n");

    for (int i = 0; i < 60; i++) {
        printf("[SYSV] contador = %d\n", ptr->contador);
        sleep(1);
    }

    shmdt(ptr);

    printf("Consumidor SYSV finalizado\n");
    return 0;
}
