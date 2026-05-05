#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <mutex>

using namespace std;

mutex mtxTerminal;

void recibirMensajes(int socketId)
{
    while (true)
    {
        char buffer[2000];
        int bytesRecibidos = 0;
        while ((bytesRecibidos = read(socketId, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRecibidos] = 0;
            mtxTerminal.lock();
            printf("[Alguien dice]: %s\n", buffer);
            mtxTerminal.unlock();
        }
    }
}

void escribirMensaje(int socketId)
{
    mtxTerminal.lock();
    cout << "Escriba los mensajes que quiera enviar." << endl;
    mtxTerminal.unlock();

    while (true)
    {
        string mensaje = "";
        getline(cin, mensaje);

        write(socketId, mensaje.c_str(), mensaje.length());
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_in socketConfig;
    memset(&socketConfig, '0', sizeof(socketConfig));

    socketConfig.sin_family = AF_INET;
    socketConfig.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &socketConfig.sin_addr);

    int socketComunicacion = socket(AF_INET, SOCK_STREAM, 0);
    int resultadoConexion = connect(socketComunicacion, (struct sockaddr *)&socketConfig, sizeof(socketConfig));

    if (resultadoConexion < 0)
    {
        cout << "Error en la conexión..." << endl;
        return EXIT_FAILURE;
    }

    thread thRecibirMensajes(recibirMensajes, socketComunicacion);
    thread thEscribirMensaje(escribirMensaje, socketComunicacion);

    thRecibirMensajes.join();
    thEscribirMensaje.join();

    return EXIT_SUCCESS;
}
