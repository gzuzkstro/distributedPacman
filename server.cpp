#include <iostream>
#include "socket_helper.h"
#include "game_logic.h"

using namespace std;

int main(int argc, char** argv){


    game_logic *gl = new game_logic();

	//If there were no arguments, it's set to default value
	if(argc >= 2){
		int num_players;
		num_players = atoi(argv[1]);

		//If an invalid number is given, it's set to default value
		if(num_players < 2 || num_players > 5){
			gl->setNumPlayers(NUM_CONN);
		} else {
            gl->setNumPlayers(num_players);
		}
	} else {
		 gl->setNumPlayers(NUM_CONN);
	}

	cout << "El servidor está esperando por " << gl->getNumPlayers() << " jugardores" << endl;
	cout << endl;

	//The server waits for connections
	socketHelper TCPserver(true, gl);
	TCPserver.sh_bind();
	TCPserver.sh_listen();
	TCPserver.sh_accept();

	cout << "Ya está listo para comenzar el otro socket" << endl;
	//After all connections are set up, the UPD multi-cast starts
    socketHelper UDPserver(false, gl);
    UDPserver.sh_sendState();

	return 0;
}
