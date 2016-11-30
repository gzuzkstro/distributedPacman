#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include "game_logic.h"

#define PAC_PORT 12445
#define PAC_GROUP "225.0.0.38"
#define MSGBUFSIZE 256
#define NAP_TCP 50000
#define NAP_UDP 50000

using namespace std;

struct threadParams {
	int *dif;
	int *sock;
	int *dir;
	int id_player;
};

class socketHelper {
	public:
		bool type;
		struct sockaddr_in addr;
		struct sockaddr_in client;
		struct ip_mreq mreq;
		int num_players;
		int socket_desc;
		int new_socket; //suspect
		int *new_sock; 	//suspect
		int nbytes;	//suspect
		unsigned int addrlen;	//suspect
		char msgbuf[MSGBUFSIZE];	//should be 2D?
		const char *message;	//suspect
        // game logic
		game_logic *gl;

		socketHelper(bool protocol, game_logic *g);
		socketHelper(const char *ipaddress, bool protocol, game_logic *g);

		void sh_setNumPlayers(int num);
		int sh_bind();
		void sh_listen();
		int sh_acceptLoop();
		int sh_connect();
		int sh_setMCGroup();

		/* test */
		int sh_recvStateLoop();
		int sh_sendStateLoop();


};

void *connection_handler(void *);
void *connection_handler_client(void *);

#endif
