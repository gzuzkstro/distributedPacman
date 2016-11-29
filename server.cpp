#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(int argc, char** argv){
	
	int num_players;
	
	//If there were no arguments, it's set to default value
	if(argc >= 2){
		num_players = atoi(argv[1]);
		
		//If an invalid number is given, it's set to default value
		if(num_players < 2 || num_players > 5){
			num_players = NUM_CONN;
		}
	} else {
		num_players = NUM_CONN;
	}
	
	cout << "El servidor está esperando por " << num_players << " jugardores" << endl;
	cout << endl;
	
	//The server waits for connections
	socketHelper TCPserver(true);
	TCPserver.sh_setNumPlayers(num_players);
	TCPserver.sh_bind();
	TCPserver.sh_listen();
	TCPserver.sh_acceptLoop();
	
	//After all connections are set up, the UPD multi-cast starts
    socketHelper UDPserver(false);
    UDPserver.sh_sendStateLoop();
    
    sleep(100);
	return 0;
}
