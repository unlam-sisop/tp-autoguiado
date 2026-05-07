#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <semaphore.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

// ---------- Config ----------
const int ITER = 5;

// ---------- Mutex (threads) ----------
mutex mtx;

// ---------- Semaforo POSIX (procesos) ----------
sem_t* sem = nullptr;

// ---------- Trabajo generico ----------
void imprimir(const string& quien, bool usar_mutex, bool usar_sem) {
	for (int i = 0; i < ITER; i++) {
		if (usar_mutex) {
			mtx.lock();
		}

		if (usar_sem) {
			sem_wait(sem);
		}

		cout << quien << " PID: " << getpid() << " iter " << i << endl;
		usleep(100000);

		if (usar_sem) {
			sem_post(sem);
		}

		if (usar_mutex) {
			mtx.unlock();
		}

		usleep(50000);
	}
}

// ---------- Modo 1 y 2: procesos ----------
void ejecutar_procesos(bool con_semaforo) {
	if (con_semaforo) {
		sem = sem_open("/sem_tp", O_CREAT, 0644, 1);
		if (sem == SEM_FAILED) {
			cerr << "No se pudo crear el semaforo POSIX" << endl;
			return;
		}
	}

	pid_t pid = fork();
	if (pid == 0) {
		imprimir("Hijo", false, con_semaforo);
		_exit(0);
	}

	if (pid > 0) {
		imprimir("Padre", false, con_semaforo);
		wait(nullptr);
	}

	if (con_semaforo) {
		sem_close(sem);
		sem_unlink("/sem_tp");
	}
}

// ---------- Modo 3 y 4: threads ----------
void ejecutar_threads(bool con_mutex) {
	vector<thread> threads;

	for (int i = 0; i < 3; i++) {
		threads.emplace_back([i, con_mutex]() {
			string nombre = "Thread " + to_string(i);
			imprimir(nombre, con_mutex, false);
		});
	}

	for (auto& t : threads) {
		t.join();
	}
}

// ---------- MAIN ----------
int main() {
	int modo;

	cout << "Modo de ejecucion:\n";
	cout << "1 - Procesos pesados sin sincronización\n";
	cout << "2 - Procesos con semáforo POSIX\n";
	cout << "3 - Threads sin sincronización\n";
	cout << "4 - Threads con mutex\n";
	cout << "Seleccion: ";
	cin >> modo;

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
			cout << "Modo invalido\n";
	}

	return 0;
}
