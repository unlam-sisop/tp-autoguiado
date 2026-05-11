#include <fcntl.h>
#include <iostream>
#include <limits>
#include <mutex>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

// ======================================================
// CONFIG
// ======================================================

const int ITER = 100000;

// ======================================================
// THREADS
// ======================================================

mutex mtx;
int contador_threads = 0;

// ======================================================
// PROCESOS
// ======================================================

sem_t* sem = nullptr;
int* contador_procesos = nullptr;

// ======================================================
// TRABAJO THREADS
// ======================================================

void trabajo_thread(int id, bool usar_mutex) {
    for (int i = 0; i < ITER; i++) {

        if (usar_mutex) {
            mtx.lock();
        }

        // 🔴 REGION CRITICA
        contador_threads++;

        if (usar_mutex) {
            mtx.unlock();
        }
    }

    cout << "Thread " << id << " finalizo" << endl;
}

// ======================================================
// TRABAJO PROCESOS
// ======================================================

void trabajo_proceso(const string& quien, bool usar_sem) {
    for (int i = 0; i < ITER; i++) {

        if (usar_sem) {
            sem_wait(sem);
        }

        // 🔴 REGION CRITICA
        (*contador_procesos)++;

        if (usar_sem) {
            sem_post(sem);
        }
    }

    cout << quien << " finalizo" << endl;
}

// ======================================================
// MODOS 1 Y 2
// ======================================================

void ejecutar_procesos(bool con_semaforo) {

    // Memoria compartida
    contador_procesos = (int*) mmap(
        NULL,
        sizeof(int),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    *contador_procesos = 0;

    // Semaforo POSIX
    if (con_semaforo) {

        sem_unlink("/sem_tp");

        sem = sem_open(
            "/sem_tp",
            O_CREAT,
            0644,
            1
        );

        if (sem == SEM_FAILED) {
            cerr << "Error creando semaforo" << endl;
            return;
        }
    }

    pid_t pid = fork();

    if (pid == 0) {

        trabajo_proceso("👶 Hijo", con_semaforo);
        _exit(0);
    }

    if (pid > 0) {

        trabajo_proceso("👨 Padre", con_semaforo);

        wait(nullptr);

        cout << endl;
        cout << "===================================" << endl;
        cout << "Valor final procesos: "
             << *contador_procesos << endl;
        cout << "Valor esperado: "
             << ITER * 2 << endl;
        cout << "===================================" << endl;
    }

    // ==================================================
    // PAUSA PARA INSPECCION
    // ==================================================

    if (con_semaforo) {

        cout << endl;
        cout << "🔍 El semaforo POSIX sigue creado." << endl;
        cout << "👉 Inspeccionar con:" << endl;
        cout << "   ls -l /dev/shm" << endl;
        cout << endl;
        cout << "Presione ENTER para liberar recursos...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    // Cleanup
    if (con_semaforo) {
        sem_close(sem);
        sem_unlink("/sem_tp");
    }

    munmap(contador_procesos, sizeof(int));
}

// ======================================================
// MODOS 3 Y 4
// ======================================================

void ejecutar_threads(bool con_mutex) {

    contador_threads = 0;

    vector<thread> threads;

    for (int i = 0; i < 3; i++) {
        threads.emplace_back(
            trabajo_thread,
            i,
            con_mutex
        );
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << endl;
    cout << "===================================" << endl;
    cout << "Valor final threads: "
         << contador_threads << endl;
    cout << "Valor esperado: "
         << ITER * 3 << endl;
    cout << "===================================" << endl;
}

// ======================================================
// MAIN
// ======================================================

int main() {

    int modo;

    cout << endl;
    cout << "===================================" << endl;
    cout << "SINCRONIZACION - SISTEMAS OPERATIVOS" << endl;
    cout << "===================================" << endl;
    cout << endl;

    cout << "1 - Procesos pesados SIN sincronizacion" << endl;
    cout << "2 - Procesos pesados CON semaforo POSIX" << endl;
    cout << "3 - Threads SIN sincronizacion" << endl;
    cout << "4 - Threads CON mutex" << endl;
    cout << endl;

    cout << "Seleccion: ";
    cin >> modo;

    cout << endl;

    switch (modo) {

        case 1:
            ejecutar_procesos(false);
            break;

        case 2:
            ejecutar_procesos(true);
            break;

        case 3:
            ejecutar_threads(false);
            break;

        case 4:
            ejecutar_threads(true);
            break;

        default:
            cout << "Modo invalido" << endl;
    }

    return 0;
}