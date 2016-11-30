#include "socket_helper.h"


//Constructor for a server socket
//type: true(TCP), false(UDP)
socketHelper::socketHelper(bool protocol, game_logic *g){

    this->gl = g;
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
socketHelper::socketHelper(const char *ipaddress, bool protocol, game_logic *g){
    this->gl = g;
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

		u_int yes=1;

		// Allow multiple sockets to use the same PORT number
		if (setsockopt(socket_desc,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
			cout << "Reusing ADDR failed" << endl;
			exit(1);
		}
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

    int conn_count = 0;

    struct threadParams *params;

    while(conn_count < num_players &&
    (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&addrlen)) )
    {
        cout << "Conexion aceptada:" << endl;

        //Prints connection data
		cout << "...sin_addr:" << inet_ntoa(client.sin_addr) << endl;
		cout << "...sin_port:" << ntohs(client.sin_port) << endl;
		cout << endl;

        //Reply to the client
        message = "Tu solicitud ha sido aceptada por el servidor, por favor enviar nombre";
        write(new_socket , message , MSGBUFSIZE);

        /* legacy code */
        pthread_t sniffer_thread;
        new_sock = (int *)malloc(1);
        *new_sock = new_socket;

        params = (struct threadParams *)malloc(sizeof(struct threadParams));
        params->dif = &sync[conn_count];
        params->sock = (int *)malloc(1);
        *(params->sock) = new_socket;

        //if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*)params) < 0)
        {
            cout << "No se pudo crear el hilo" << endl;
            return 1;
        }

        conn_count++;
        cout << "Manejador asignado a la conexion entrante" << endl;
        cout << "Faltan " << num_players - conn_count << " jugadores por conectarse..." << endl << endl;

		//cout << "Aqui" << sync[0] << "," << sync[1] << endl;

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

    char buffer[MSGBUFSIZE];

    //Server should be asking for a name
    read(socket_desc, buffer , MSGBUFSIZE);
    cout << buffer << endl;
    cin >> buffer;
    write(socket_desc, buffer, MSGBUFSIZE);

	//Time is sent for sync purpose
	time_t seconds;
	seconds = time(0);
	sprintf(buffer,"%ld", seconds);
	write(socket_desc, buffer, MSGBUFSIZE);

    initNcurses();

    pthread_t client_thread;
	//new_sock = (int *)malloc(1);
	//*new_sock = new_socket;

	if( pthread_create( &client_thread , NULL ,  connection_handler_client , (void*) &socket_desc) < 0)
	{
		cout << "No se pudo crear el hilo" << endl;
		return 1;
	}

	cout << "Se creo el hilo para manejar TCP en cliente" << endl;

    /*
    char buffer[MSGBUFSIZE];

    if( read(TCPclient.socket_desc, buffer , MSGBUFSIZE) < 0)
    {
        cout << "Fallo en READ" << endl;
    }

    //Server should be asking for a name
    cout << "Servidor:" << buffer << endl;
    cin >> buffer;

    if( write(TCPclient.socket_desc , buffer , MSGBUFSIZE) < 0)
    {
        cout << "Fallo en WRITE" << endl;
        return 1;
    }
    cout << "Mensaje enviado al servidor" << endl;
    */

    return 0;
}

int socketHelper::sh_setMCGroup(){

	mreq.imr_multiaddr.s_addr=inet_addr(PAC_GROUP);
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if (setsockopt(socket_desc,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		cout << "Error in setsockopt" << endl;
		exit(1);
	}

	return 0;
}

int socketHelper::sh_recvStateLoop(){
/*
	char buffer[MSGBUFSIZE];

     while (1) {
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(socket_desc,buffer,MSGBUFSIZE,0,
			(struct sockaddr *) &addr,&addrlen)) < 0) {
			cout << "recvfrom" << endl;
	       exit(1);
	  }
	  cout << buffer << endl;
     }*/
     //initNcurses();
     while (1) {
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(socket_desc,gl,sizeof(game_logic),0,
			(struct sockaddr *) &addr,&addrlen)) < 0) {
			cout << "recvfrom" << endl;
	       exit(1);
	  }
	  clear();
	  gl->draw();
	  refresh();
     }
}

int socketHelper::sh_sendStateLoop(){
	/*int i = 0;
	char buffer[MSGBUFSIZE];

     while (1) {
	  sprintf(buffer,"Multicast #%d",i);
	  cout << buffer << endl;
	  if (sendto(socket_desc,buffer,sizeof(buffer),0,(struct sockaddr *) &addr,
		     sizeof(addr)) < 0) {
	       cout << "Fallo en SENDTO" << endl;
	       exit(1);
	  }
	  i++;
	  sleep(2);
     }
     */
     while (1) {
	  if (sendto(socket_desc,gl,sizeof(game_logic),0,(struct sockaddr *) &addr,
		     sizeof(addr)) < 0) {
	       cout << "Fallo en SENDTO" << endl;
	       exit(1);
	  }
	  usleep(NAP_UDP);
     }

     return 0;
}

/*
 * This will handle TCP connection for each client
 * */
void *connection_handler(void *params)
{
    //Get the socket descriptor
    struct threadParams *context = (struct threadParams *)params;
	int sock = *(int*)context->sock;
	int *dif = context->dif;
    char buffer[MSGBUFSIZE];

    //User should be sending a name
    read(sock, buffer , MSGBUFSIZE);
    cout << "Username:" << buffer << endl;
    //save it?

    //Gets time from client
	time_t sec_server;
    read(sock, buffer , MSGBUFSIZE);
    sleep(1);
    sec_server = time(0);
    int sec_client = atoi(buffer);
	*dif = sec_server - sec_client;
    cout << "La diferencia es: " << *dif << endl;
    sleep(3);

    while(1){

		strcpy(buffer,"TCP server says bye!");
		write(sock, buffer, MSGBUFSIZE);

		read(sock, buffer,MSGBUFSIZE);
		cout << "CliTCP:" << buffer << endl;

		usleep(NAP_TCP);
	}

    //Free the socket pointer
    free(&sock);

    return 0;
}

/*
 * This will handle TCP connection on each client
 * */
void *connection_handler_client(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;


    char buffer[MSGBUFSIZE];
/*
    //Server should be asking for a name
    read(sock, buffer , MSGBUFSIZE);
    cout << buffer << endl;
    cin >> buffer;
    write(sock, buffer, MSGBUFSIZE);

	//Time is sent for sync purpose
	time_t seconds;
	seconds = time(0);
	sprintf(buffer,"%ld", seconds);
	write(sock, buffer, MSGBUFSIZE);
    */
    while(1){
		read(sock, buffer , MSGBUFSIZE);
		cout << "ServTCP:" << buffer << endl;


		/* Getting time to later sync */
		time_t seconds;
		seconds = time(0);
		sprintf(buffer,"TCP client says something! in %ld", seconds);
		write(sock, buffer, MSGBUFSIZE);

		//sleep(NAP_TCP)
	}

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
