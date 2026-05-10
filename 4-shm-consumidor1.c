#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_POSIX_NAME "/shm_posix_ejemplo"

typedef struct {
    int contador;
} shared_data_t;

int main() {
    int fd = shm_open(SHM_POSIX_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    shared_data_t *ptr = mmap(NULL, sizeof(shared_data_t),
                              PROT_READ, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Consumidor POSIX iniciado\n");

    for (int i = 0; i < 60; i++) {
        printf("[POSIX] contador = %d\n", ptr->contador);
        sleep(1);
    }

    munmap(ptr, sizeof(shared_data_t));
    close(fd);

    printf("Consumidor POSIX finalizado\n");
    return 0;
}
