#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#define DELAY 50000

using namespace std;

class game_logic
{
    public:
        game_logic();
        virtual ~game_logic();
        void draw();
    protected:
    private:
        char mapa[22][31];
        int max_x;
        int max_y;
        int _x;
        int _y;
        int next_x;
        int next_y;
        int pts;
};

#endif // GAME_LOGIC_H
