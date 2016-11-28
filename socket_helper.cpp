#include "socket_helper.h"

//Constructor for a server socket
//type: true(TCP), false(UDP)
socketHelper::socketHelper(bool protocol){
	
	type = protocol;
	
	//Creates socket based on type parameter
	if(type){
		socket_desc = socket(AF_INET, SOCK_STREAM , 0);
	} else {
		socket_desc = socket(AF_INET, SOCK_DGRAM , 0);
	}
	
	if (socket_desc == -1)
    {
        cout << "Could not create socket!" << endl;
    }
    
    cout << "Created socket!" << endl;
    
    //Prepares the sockaddr_in structure
    server.sin_family = AF_INET;
    
    if(type){
		server.sin_addr.s_addr = INADDR_ANY;
	} else {
		server.sin_addr.s_addr = inet_addr(PAC_GROUP);
	}
	
    server.sin_port = htons(PAC_PORT);
}

//Constructor for a client socket
socketHelper::socketHelper(const char *ipaddress, bool protocol){
	
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
        cout << "Could not create socket!" << endl;
    }
    
    cout << "Created socket!" << endl;
    
    //Prepares the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ipaddress);
    server.sin_port = htons(PAC_PORT);
}

int socketHelper::sh_bind(){
	
	//Binds socket to port
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << "bind failed" << endl;
        return 1;
    }
    cout << "bind done" << endl;
    
    return 0;
}

void socketHelper::sh_listen(){
	
	//Listen
    listen(socket_desc , NUM_CONN);

}

int socketHelper::sh_accept(){
	//Accept and incoming connection
    cout << "Waiting for incoming connections..." << endl;
    c = sizeof(struct sockaddr_in);
    
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        cout << "Connection accepted" << endl;
       
        //Reply to the client
        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        write(new_socket , message , MSGBUFSIZE);
         
        pthread_t sniffer_thread;
        new_sock = (int *)malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            cout << "could not create thread" << endl;
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        cout << "Handler assigned" << endl;
    }
     
    if (new_socket<0)
    {
        cout << "accept failed" << endl;
        return 1;
    }
    
    return 0;
}

int socketHelper::sh_connect() {
	
	//Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cout << "connect error" << endl;
        return 1;
    }
     
    cout << "Connected" << endl;
    return 0;
}

int socketHelper::sh_setGroup(){
	
	mreq.imr_multiaddr.s_addr=inet_addr(PAC_GROUP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if (setsockopt(socket_desc,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		cout << "Error in setsockopt" << endl;
		exit(1);
	}
	
	return 0;
}

int socketHelper::sh_udpLoop(){
	/* now just enter a read-print loop */
     while (1) {
	  addrlen=sizeof(server);
	  if ((nbytes=recvfrom(socket_desc,msgbuf,MSGBUFSIZE,0,
			(struct sockaddr *) &server,&addrlen)) < 0) {
			cout << "recvfrom" << endl;
	       exit(1);
	  }
	  cout << message << endl;
     }
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
    
    cout << "Am I working?" << endl;
    return 0;
}
