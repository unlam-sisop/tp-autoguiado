CXX = g++
CXXFLAGS = -std=c++20

fork: 1-fork-a.cpp
	$(CXX) $(CXXFLAGS) -o 1-fork 1-fork-a.cpp

threads: 2-threads-a.cpp
	$(CXX) $(CXXFLAGS) -o 2-threads 2-threads-a.cpp

semaforos: 3-semaforos-a.cpp
	$(CXX) $(CXXFLAGS) -o 3-semaforos 3-semaforos-a.cpp

shm: 4-shm-cliente-a.cpp 4-shm-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 4-shm-cliente 4-shm-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 4-shm-servidor 4-shm-servidor-a.cpp

sockets: 5-chat-cliente-a.cpp 5-chat-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-cliente 5-chat-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-servidor 5-chat-servidor-a.cpp

clean:
	rm 1-fork
	rm 2-threads
	rm 3-semaforos
	rm 4-shm-cliente
	rm 4-shm-servidor
	rm 5-cliente
	rm 5-servidor
