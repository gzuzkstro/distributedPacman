server: socket_helper.o server.cpp
	g++ -Wall -pthread -g -o server server.cpp socket_helper.o

socket_helper.o: socket_helper.h socket_helper.cpp
	g++ -Wall -pthread -g -c -o socket_helper.o socket_helper.cpp
