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
#include <semaphore>
#include <vector>
#include <signal.h>

using namespace std;

vector<int> socketsClientes;
bool requestToFinish = false;

void cleanSockets(int signalId)
{
    requestToFinish = true;
}

void retransmitirMensaje(char *mensaje, int sourceSocketId)
{
    for (int clientSocketId : socketsClientes)
    {
        cout << "Retransmitiendo mensaje..." << mensaje << endl;
        write(clientSocketId, mensaje, strlen(mensaje));
    }
}

void atenderCliente(int socketId)
{
    while (!requestToFinish)
    {
        char buffer[2000];
        int bytesRecibidos = 0;
        while (!requestToFinish && (bytesRecibidos = read(socketId, buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[bytesRecibidos] = 0;
            printf("Un cliente dice \"%s\"\n", buffer);
            retransmitirMensaje(buffer, socketId);
        }
    }

    close(socketId);
}

int main()
{
    struct sigaction action;
    action.sa_handler = cleanSockets;

    sigaction(SIGINT, &action, NULL);

    struct sockaddr_in serverConfig;
    memset(&serverConfig, '0', sizeof(serverConfig));

    serverConfig.sin_family = AF_INET;
    serverConfig.sin_addr.s_addr = htonl(INADDR_ANY);
    serverConfig.sin_port = htons(5000);

    int socketEscucha = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketEscucha, (struct sockaddr *)&serverConfig, sizeof(serverConfig));

    listen(socketEscucha, 10);

    while (!requestToFinish)
    {
        int socketComunicacion = accept(socketEscucha, (struct sockaddr *)NULL, NULL);

        socketsClientes.push_back(socketComunicacion);

        thread hiloCliente(atenderCliente, socketComunicacion);
        hiloCliente.detach();
    }

    close(socketEscucha);
    return EXIT_SUCCESS;
}
