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
        cout << "No se pudo crear el socket!" << endl;
    }
    
    cout << "Creado socket servidor " << ((type)?"TCP":"UDP") << endl;
    
    //Prepares the sockaddr_in structure
    addr.sin_family = AF_INET;
    cout << "...sin_family:" << addr.sin_family << endl;
    
    if(type){
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		addr.sin_addr.s_addr = inet_addr(PAC_GROUP);
	}
	cout << "...sin_addr.s_addr:" << ntohs(addr.sin_addr.s_addr) << endl;
	
    addr.sin_port = htons(PAC_PORT);
    cout << "...sin_port:" << ntohs(addr.sin_port) << endl;
    cout << endl;
}

//Constructor for a client socket
//ipaddress: should be passed even for UDP
//type: true(TCP), false(UDP)
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
        cout << "No se pudo crear el socket!" << endl;
    }
    
    cout << "Creado socket cliente " << ((type)?"TCP":"UDP") << endl;
    
    //Prepares the sockaddr_in structure
    addr.sin_family = AF_INET;
    
    if(type){
		addr.sin_addr.s_addr = inet_addr(ipaddress);
	} else {
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	cout << "...sin_addr:" << inet_ntoa(addr.sin_addr) << endl;
	
    addr.sin_port = htons(PAC_PORT);
    cout << "...sin_port:" << ntohs(addr.sin_port) << endl;
    cout << endl;
}

//Setter for number of players
void socketHelper::sh_setNumPlayers(int num){
	num_players = num;
}

//Binds socket to port
int socketHelper::sh_bind(){
	
    if( bind(socket_desc,(struct sockaddr *)&addr , sizeof(addr)) < 0)
    {
        cout << "Fallo en bind" << endl;
        return 1;
    }
    cout << "BIND listo, ";
    return 0;
}

//Defines how maximum connection queue
void socketHelper::sh_listen(){

    if( listen(socket_desc , num_players) < 0)
    {
		cout << "Fallo en LISTEN" << endl;
	}
    cout << "LISTEN listo" << endl;
}

//Starts accepting connections and assigns threads to handle
int socketHelper::sh_acceptLoop(){
	
    cout << "Esperando por conexiones entrantes..." << endl << endl;
    addrlen = sizeof(struct sockaddr_in);
    
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&addrlen)) )
    {
        cout << "Conexion aceptada:" << endl;
        
        //Prints connection data
		cout << "...sin_addr:" << inet_ntoa(client.sin_addr) << endl;
		cout << "...sin_port:" << ntohs(client.sin_port) << endl;
		cout << endl;
		
        //Reply to the client
        message = "Tu solicitud ha sido aceptada por el servidor";
        write(new_socket , message , MSGBUFSIZE);
         
        pthread_t sniffer_thread;
        new_sock = (int *)malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            cout << "No se pudo crear el hilo" << endl;
            return 1;
        }
         
        //pthread_join( sniffer_thread , NULL);
        cout << "Manejador asignado a la conexion entrante" << endl << endl;
    }
     
    if (new_socket<0)
    {
        cout << "accept failed" << endl;
        return 1;
    }
    
    return 0;
}

//Connects to remote server
int socketHelper::sh_connect() {
	
    if (connect(socket_desc , (struct sockaddr *)&addr , sizeof(addr)) < 0)
    {
        cout << "Error en CONNECT" << endl;
        return 1;
    }
     
    cout << "CONNECT listo" << endl;
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
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(socket_desc,msgbuf,MSGBUFSIZE,0,
			(struct sockaddr *) &addr,&addrlen)) < 0) {
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
