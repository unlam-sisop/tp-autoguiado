C = gcc
CXX = g++
CXXFLAGS = -std=c++20

all: fork threads semaforos shm sockets

fork: 1-fork.c
	$(C) $(CFLAGS) -o 1-fork 1-fork.c

threads: 2-threads-a.cpp
	$(CXX) $(CXXFLAGS) -o 2-threads 2-threads-a.cpp

semaforos: 3-semaforos.cpp
	$(CXX) $(CXXFLAGS) -o 3-semaforos 3-semaforos.cpp

shm: 4-shm-productor.c 4-shm-consumidor1.c 4-shm-consumidor2.c
	$(C) $(CFLAGS) -o 4-shm-productor 4-shm-productor.c
	$(C) $(CFLAGS) -o 4-shm-consumidor1 4-shm-consumidor1.c
	$(C) $(CFLAGS) -o 4-shm-consumidor2 4-shm-consumidor2.c

sockets: 5-chat-cliente-a.cpp 5-chat-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-cliente 5-chat-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-servidor 5-chat-servidor-a.cpp

clean:
	rm 1-fork
	rm 2-threads
	rm 3-semaforos
	rm 4-shm-productor
	rm 4-shm-consumidor1
	rm 4-shm-consumidor2
	rm 5-cliente
	rm 5-servidor
