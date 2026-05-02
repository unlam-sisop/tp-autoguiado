CXX = g++
CXXFLAGS = -std=c++20

threads: 2-threads-a.cpp
	$(CXX) $(CXXFLAGS) -o 2-threads 2-threads-a.cpp

sockets: 5-chat-cliente-a.cpp 5-chat-servidor-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-cliente 5-chat-cliente-a.cpp
	$(CXX) $(CXXFLAGS) -o 5-servidor 5-chat-servidor-a.cpp

clean:
	rm 2-threads
	rm 5-cliente
	rm 5-servidor
