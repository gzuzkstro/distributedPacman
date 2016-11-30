.PHONY: all

all: client server

server: socket_helper.o game_logic.o server.cpp
	g++ -Wall -pthread -g -o server server.cpp socket_helper.o game_logic.o -lncurses -std=gnu++11

client: socket_helper.o game_logic.o client.cpp
	g++ -Wall -pthread -g -o client client.cpp socket_helper.o game_logic.o -lncurses -std=gnu++11

socket_helper.o: socket_helper.h socket_helper.cpp game_logic.o
	g++ -Wall -pthread -g -c -o socket_helper.o socket_helper.cpp game_logic.o -lncurses -std=gnu++11

game_logic.o: game_logic.h game_logic.cpp
	g++ -Wall -g -c -o game_logic.o game_logic.cpp -lncurses -std=gnu++11 	