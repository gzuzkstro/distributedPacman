#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(){
	
	socketHelper prueba(12345,"127.0.0.1",true);
	prueba.sh_connect();
	
	//Send some data
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
     
    return 0;
}
