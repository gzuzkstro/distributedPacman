#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(int argc, char** argv){
	
	int num_players;
	
	//If there were no arguments, defaults to 5
	if(argc >= 2){
		num_players = atoi(argv[1]);
	} else {
		num_players = NUM_CONN;
	}
	
	cout << "El servidor está esperando por " << num_players << " jugardores" <<endl;
	
	
	socketHelper prueba(true);
	prueba.sh_bind();
	prueba.sh_listen();
	
	//prueba.sh_accept();
	return 0;
}
