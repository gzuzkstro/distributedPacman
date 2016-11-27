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

#define HELLO_PORT 12345
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

using namespace std;

class socketHelper {
	public:
		bool type;
		struct sockaddr_in server;
		struct sockaddr_in client;
		int socket_desc;
		int new_socket;
		int *new_sock;
		int c;
		char const *message;
		
		socketHelper(int port, bool type);
		
		int sh_bind();
		void sh_listen();
		int sh_accept();
};
 
void *connection_handler(void *);

#endif
