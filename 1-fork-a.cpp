#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3, nieto;

    printf("PID Padre: %d\n", getpid());

    /* ================= HIJO 1 (ZOMBIE) ================= */
    pid1 = fork();

    if (pid1 == 0) {
        /* Proceso hijo 1 */
        printf("Hijo 1 (PID %d) finaliza inmediatamente\n", getpid());
        exit(0);
    }

    /* El padre NO hace wait por pid1 → queda zombie */

    /* ================= HIJO 2 (CREA DEMONIO) ================= */
    pid2 = fork();

    if (pid2 == 0) {
        /* Proceso hijo 2 */
        printf("Hijo 2 (PID %d) creando nieto...\n", getpid());

        nieto = fork();

        if (nieto == 0) {
            /* Proceso nieto */
            printf("Nieto (PID %d) - demonio simple, PPID inicial %d\n",
                   getpid(), getppid());

            /* Simula demonio */
            sleep(30);

            printf("Nieto (PID %d) sigue vivo, PPID ahora %d\n",
                   getpid(), getppid());

            exit(0);
        }

        /* Hijo 2 finaliza, dejando al nieto huérfano */
        printf("Hijo 2 (PID %d) finaliza\n", getpid());
        exit(0);
    }

    /* El padre tampoco espera al hijo 2 */

    /* ================= HIJO 3 (TRABAJO 1 MINUTO) ================= */
    pid3 = fork();

    if (pid3 == 0) {
        /* Proceso hijo 3 */
        printf("Hijo 3 (PID %d) procesando durante 1 minuto...\n", getpid());

        for (int i = 0; i < 60; i++) {
            sleep(1);
        }

        printf("Hijo 3 (PID %d) finaliza\n", getpid());
        exit(0);
    }

    /* ================= PADRE ================= */
    printf("Padre esperando SOLO al hijo 3 (PID %d)\n", pid3);

    waitpid(pid3, NULL, 0);

    printf("Padre finaliza\n");
    return 0;
}
