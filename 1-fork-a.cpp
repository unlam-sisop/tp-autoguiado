#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

using namespace std;

int main()
{
    cout << "Creación de procesos pesados" << endl;

    pid_t pidHijo = fork();

    if (pidHijo < 0)
    {
        cerr << "Error creando un nuevo proceso pesado" << endl;
    }
    else if (pidHijo > 0)
    {
        cout << "Proceso padre: " << getpid() << endl;
        wait(NULL);
    }
    else
    {
        cout << "Proceso hijo: " << getpid() << " hijo de " << getppid() << endl;
    }

    return EXIT_SUCCESS;
}
