#include "socket_helper.h"

//Constructor
socketHelper::socketHelper(int port, bool protocol){
	
	type = protocol;
	
	//Creates socket based on type parameter
	// true: TCP, false: UDP
	if(type){
		socket_desc = socket(AF_INET, SOCK_STREAM , 0);
	} else {
		socket_desc = socket(AF_INET, SOCK_DGRAM , 0);
	}
	
	if (socket_desc == -1)
    {
        cout << "Could not create socket!";
    }
     
    //Prepares the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
}

int socketHelper::sh_bind(){
	
	//Binds socket to port
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << "bind failed";
        return 1;
    }
    cout << "bind done";
    
    return 0;
}

void socketHelper::sh_listen(){
	//Listen
    listen(socket_desc , 3);
}

int socketHelper::sh_accept(){
	//Accept and incoming connection
    cout << "Waiting for incoming connections...";
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        cout << "Connection accepted";
         
        //Reply to the client
        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        write(new_socket , message , MSGBUFSIZE);
         
        pthread_t sniffer_thread;
        new_sock = (int *)malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            cout << "could not create thread";
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        cout << "Handler assigned";
    }
     
    if (new_socket<0)
    {
        cout << "accept failed";
        return 1;
    }
    
    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
     
    char const *message;
     
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , MSGBUFSIZE);
     
    message = "Its my duty to communicate with you";
    write(sock , message , MSGBUFSIZE);
     
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}
