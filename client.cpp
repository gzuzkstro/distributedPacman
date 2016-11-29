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
	
	
	socketHelper prueba(ip_param,true);
	prueba.sh_connect();
	
	//Send some data
    char server_reply[2000];
     
    if( recv(prueba.socket_desc, server_reply , 2000 , 0) < 0)
    {
        cout << "recv failed" << endl;
    }
    cout << "Reply received" << endl;
    cout << server_reply << endl;
    
    /*
    prueba.message = "GET / HTTP/1.1\r\n\r\n";
    if( send(prueba.socket_desc , prueba.message , MSGBUFSIZE , 0) < 0)
    {
        cout << "Send failed" << endl;
        return 1;
    }
    cout << "Data Send\n" << endl;
     
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
