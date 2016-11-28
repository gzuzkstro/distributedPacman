#include <iostream>
#include "socket_helper.h"

using namespace std;

int main(){
	cout << "Hello world!!!";
	socketHelper prueba(12345, true);
	prueba.sh_bind();
	prueba.sh_listen();
	prueba.sh_accept();
	return 0;
}
