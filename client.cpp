#include <iostream>
#include "socket_helper.h"
#include "game_logic.h"

using namespace std;

void UDPsettings () {

}
int main(int argc, char** argv){

	const char *ip_param;
	game_logic *gl = (game_logic *)malloc(sizeof(game_logic));


	//If there were no arguments, defaults to localhost
	if(argc >= 2){
		ip_param = argv[1];
	} else {
		ip_param = "127.0.0.1";
	}

	cout << "La direccion del servidor es " << ip_param << endl;

	//TCP socket settings
	socketHelper TCPclient(ip_param,true,gl);
	TCPclient.sh_connect();

    //UDP socket settings

    socketHelper UDPclient(ip_param,false,gl);
    UDPclient.sh_setMCGroup();
    UDPclient.sh_bind();
    UDPclient.sh_recvState();

    sleep(100);
    return 0;
}
