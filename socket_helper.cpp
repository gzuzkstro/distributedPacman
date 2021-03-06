#include "socket_helper.h"

//Constructor for a server socket
//type: true(TCP), false(UDP)
socketHelper::socketHelper(bool protocol, game_logic *g){


    this->gl = g;
	type = protocol;

	for(int i = 0;i < gl->getNumPlayers();i++){
        threadReady[i] = false;
    }

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

    if( listen(socket_desc , gl->getNumPlayers()) < 0)
    {
		cout << "Fallo en LISTEN" << endl;
	}
    cout << "LISTEN listo" << endl;
}

//Starts accepting connections and assigns threads to handle
int socketHelper::sh_accept(){

    cout << "Esperando por conexiones entrantes..." << endl << endl;
    addrlen = sizeof(struct sockaddr_in);

    int conn_count = 0;

    struct threadParams *params;

    while(conn_count < gl->getNumPlayers() &&
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

        pthread_t sniffer_thread;

        params = (struct threadParams *)malloc(sizeof(struct threadParams));
        params->sock = (int *)malloc(1);
        *(params->sock) = new_socket;
        params->dir = gl->getDir(conn_count);
        params->gameStatus = gl->getGameStatus();
        params->old_dir = gl->getOldDir(conn_count);
        params->dif = gl->getSync(conn_count);
        params->id_player = conn_count;
        params->ready = getThreadReady(conn_count);
        params->poderes = gl->getPoderes();

        //params->mapa = (char *)[35] malloc(sizeof(char *)[35]);
        params->mapa = gl->getMap();
        params->pos_player = gl->getPosArray(conn_count);
        //if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*)params) < 0)
        {
            cout << "No se pudo crear el hilo" << endl;
            return 1;
        }

        conn_count++;
        cout << "Manejador asignado a la conexion entrante" << endl;
        cout << "Faltan " << gl->getNumPlayers() - conn_count << " jugadores por conectarse..." << endl << endl;

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

int socketHelper::sh_recvState(){

     while (1) {
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(socket_desc,gl,sizeof(game_logic),0,
			(struct sockaddr *) &addr,&addrlen)) < 0) {
            break;
	  }
	  clear();
	  gl->draw();
	  refresh();
	  if(*(gl->getGameStatus())!=0){
		  break;
		  }
     }

     close(socket_desc);
     clear();
}

int socketHelper::sh_sendState(){

    bool flag = TRUE;

     while (true) {

      if (flag) {
        gl->nextState();
        flag = FALSE;
      } else
        flag = TRUE;

	  if (sendto(socket_desc,gl,sizeof(game_logic),0,(struct sockaddr *) &addr,
		     sizeof(addr)) < 0) {
	       cout << "Fallo en SENDTO" << endl;
	       break;
	  }
	  
	  if (*(gl->getGameStatus())!=0){
		  cout << "Cerrado por el status" << endl;
		  break;
		  }
	  
	  usleep(NAP_UDP);
     }

     close(socket_desc);
     cout << "Cerrado el socket UDP" << endl;

     return 0;
}

bool socketHelper::allReady(){

    for(int i=0;i<gl->getNumPlayers();i++){
        if(threadReady[i]==false)
            return false;
    }
    return true;
}

bool *socketHelper::getThreadReady(int i){
    return &(threadReady[i]);
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
    int *dir = context->dir;
    bool *ready = context->ready;
    int *gameStatus = context->gameStatus;
    int *old_dir = context->old_dir;
    char (*mapa)[35] = context->mapa;
    int id_player = context->id_player;
    int *pos_player = context->pos_player;
    int *poderes = context->poderes;
    char buffer[MSGBUFSIZE];
    int _y, _x;

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
    //cout << "La diferencia es: " << *dif << endl;
    sleep(3);

    *ready = true;

    int aux;

    while(1){

        if(*gameStatus != 0){
            break;
        }

        if((read(sock,&aux,sizeof(aux)))<0){
            break;
        }
        
        //Si el fantasma intenta activar el poder se filtra
        if(id_player!=0 && aux=='1'){
			continue;
		}

        switch(aux)
        {
			//Activa el poder de invertir teclas
			case '1':
				poderes[1] = P2;
				continue;
            case KEY_UP:
                _y =-1;
                _x =0;
                break;
            case KEY_DOWN:
                _y =1;
                _x =0;
                break;
            case KEY_LEFT:
                _y =0;
                _x =-1;
                break;
            case KEY_RIGHT:
                _y =0;
                _x =1;
                break;
        }
        // La nueva direccion no hace que se quede quieto
        if (!(mapa[pos_player[0]+_y][pos_player[1]+_x] == CELL_W)){
			*old_dir = *dir;
		}

		*dir = aux;

        //cout << "El cliente " << id_player <<" presiono:" << *dir << endl;
        //cout << "La anterior fue :" << *old_dir << endl;
    }

    shutdown(sock,SHUT_RDWR);
    while(read(sock,dir,sizeof(*dir)));
    close(sock);
    cout << "Socket cerrado" << endl;
    return 0;
}

/*
 * This will handle TCP connection on each client
 * */
void *connection_handler_client(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int key;
    int key_old = KEY_RIGHT;

    while(1){
        key = getch();
        
        if  ( (key != key_old) && (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT || key == '1') ) {
            key_old = key;
            //cout << "tecla:" << key << endl;
            if((write(sock, &key , sizeof(key)))< 0){
                break;
            };
		}
	}

    shutdown(sock,SHUT_RDWR);
    while(read(sock,&key,sizeof(key)));
    close(sock);
    cout << "Socket cerrado" << endl;
    //Free the socket pointer
    //free(socket_desc);

    return 0;
}
