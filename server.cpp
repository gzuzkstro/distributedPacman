#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(){
	socketHelper prueba(true);
	prueba.sh_bind();
	prueba.sh_listen();
	prueba.sh_accept();
	return 0;
}
