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
	
	socketHelper server(true);
	server.sh_setNumPlayers(num_players);
	server.sh_bind();
	server.sh_listen();
	server.sh_acceptLoop();
	
	return 0;
}
