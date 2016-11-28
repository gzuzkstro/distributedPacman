#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>

#define PAC_PORT 12345
#define NUM_CONN 5
#define PAC_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

using namespace std;

class socketHelper {
	public:
		bool type;
		struct sockaddr_in server;
		struct sockaddr_in client;
		struct ip_mreq mreq;
		int socket_desc;
		int new_socket;
		int *new_sock;
		int c;
		int nbytes;
		unsigned int addrlen;
		char msgbuf[MSGBUFSIZE];
		const char *message;
		
		socketHelper(bool protocol);
		socketHelper(const char *ipaddress, bool protocol);
		
		int sh_bind();
		void sh_listen();
		int sh_accept();
		int sh_connect();
		int sh_setGroup();
		
		/* test */
		int sh_udpLoop();
};
 
void *connection_handler(void *);

#endif
