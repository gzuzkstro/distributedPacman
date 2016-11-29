#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(int argc, char** argv){
	
	const char *ip_param;
	
	//If there were no arguments, defaults to localhost
	if(argc >= 2){
		ip_param = argv[1];
	} else {
		ip_param = "127.0.0.1";
	}
	
	cout << "La direccion del servidor es " << ip_param << endl;
	
	
	socketHelper client(ip_param,true);
	client.sh_connect();
	
	//Send some data
    char buffer[MSGBUFSIZE];
     
    if( read(client.socket_desc, buffer , MSGBUFSIZE) < 0)
    {
        cout << "Fallo en READ" << endl;
    }
    
    //Server should be asking for a name
    cout << "Servidor:" << buffer << endl;
    
    cin >> buffer;
    
    if( write(client.socket_desc , buffer , MSGBUFSIZE) < 0)
    {
        cout << "Fallo en WRITE" << endl;
        return 1;
    }
    cout << "Mensaje enviado al servidor" << endl;
    
    /*
     char server_reply[2000];
     
    //Receive a reply from the server
		if( recv(prueba.socket_desc, server_reply , 2000 , 0) < 0)
    {
        cout << "recv failed" << endl;
    }
    cout << "Reply received\n" << endl;
    cout << server_reply << endl;
    */
    return 0;
}
