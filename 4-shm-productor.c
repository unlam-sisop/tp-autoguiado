#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

#define SHM_POSIX_NAME "/shm_posix_ejemplo"
#define SHM_SYSV_KEY 0x1234

typedef struct {
    int contador;
} shared_data_t;

shared_data_t *posix_ptr = NULL;
shared_data_t *sysv_ptr = NULL;

void sigint_handler(int sig) {
    printf("\nProductor finalizado (NO se liberan memorias compartidas)\n");
    exit(0);
}

int main() {
    signal(SIGINT, sigint_handler);

    /* ===== POSIX SHARED MEMORY ===== */
    int fd = shm_open(SHM_POSIX_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(fd, sizeof(shared_data_t));

    posix_ptr = mmap(NULL, sizeof(shared_data_t),
                     PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (posix_ptr == MAP_FAILED) {
        perror("mmap posix");
        exit(1);
    }

    posix_ptr->contador = 0;

    /* ===== SYSTEM V SHARED MEMORY ===== */
    int shmid = shmget(SHM_SYSV_KEY, sizeof(shared_data_t),
                       IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    sysv_ptr = (shared_data_t *) shmat(shmid, NULL, 0);
    if (sysv_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    sysv_ptr->contador = 0;

    /* ===== PRODUCCIÓN ===== */
    printf("Productor ejecutando... Ctrl-C para terminar\n");

    while (1) {
        posix_ptr->contador++;
        sysv_ptr->contador++;

        printf("Produciendo -> POSIX: %d | SYSV: %d\n",
               posix_ptr->contador, sysv_ptr->contador);

        sleep(1);
    }
}
