#include "game_logic.h"

int main(int argc, char const *argv[])
{
    game_logic *gl = new game_logic();
    while(1){
        gl->draw();
        refresh();
    }

	return 0;
}
