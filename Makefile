CXX = g++
CXXFLAGS = -std=c++20

all: fork threads semaforos shm sockets

fork: 1-fork-a.cpp
	$(CXX) $(CXXFLAGS) -o 1-fork.out 1-fork-a.cpp

threads: 2-threads-a.cpp
	$(CXX) $(CXXFLAGS) -o 2-threads.out 2-threads-a.cpp

semaforos: 3-semaforos-a.cpp
	$(CXX) $(CXXFLAGS) -o 3-semaforos.out 3-semaforos-a.cpp

shm: 4-shm-cliente-a.cpp 4-shm-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 4-shm-cliente.out 4-shm-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 4-shm-servidor.out 4-shm-servidor-a.cpp

sockets: 5-chat-cliente-a.cpp 5-chat-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-cliente.out 5-chat-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-servidor.out 5-chat-servidor-a.cpp

clean:
	rm 1-fork.out
	rm 2-threads.out
	rm 3-semaforos.out
	rm 4-shm-cliente.out
	rm 4-shm-servidor.out
	rm 5-cliente.out
	rm 5-servidor.out
