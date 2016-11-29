#include "game_logic.h"
using namespace std;


int main(int argc, char const *argv[])
{
    game_logic *gl = new game_logic();
    gl->initNcurses();
    int i = 0;
    while(1){
        gl->draw();
        gl->setAcum(i++);
        sleep(1);
        refresh();
    }

	return 0;
}
