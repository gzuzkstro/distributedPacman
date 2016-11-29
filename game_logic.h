#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#define MAXACUM 10
#define X_INFO 40
#define Y_INFO 5

using namespace std;

class game_logic
{
    public:
        game_logic();
        virtual ~game_logic();
        void print_map(char c,int x,int y);
        void draw();
        void print_info();
        char* acum_str(int n);
        void initNcurses();

        void setAcum(int n);
        int getAcum();
    protected:
    private:
        // Mapa del juego
        char mapa[100][100] = {
            "______________________________",
            "__............__............__",
            "__.____._____.__._____.____.__",
            "__o____._____.__._____.____o__",
            "__.____._____.__._____.____.__",
            "__..........................__",
            "__.____.__.________.__.____.__",
            "__.____.__.________.__.____.__",
            "__......__....__....__......__",
            "_______._____ __ _____._______",
            "      _._____ __ _____._      ",
            "      _.__          __._      ",
            "      _.___ __--__ ___._      ",
            "_______.___ _    _ ___._______",
            "       .    _    _    .       ",
            "_______.___ ______ ___._______",
            "      _.__          __._      ",
            "      _.__ ________ __._      ",
            "_______.__ ________ __._______",
            "__............__............__",
            "__.____._____.__._____.____.__",
            "__.____._____.__._____.____.__",
            "__o..__................__..o__",
            "____.__.__.________.__.__.____",
            "____.__.__.________.__.__.____",
            "__......__....__....__......__",
            "__.__________.__.__________.__",
            "__.__________.__.__________.__",
            "__..........................__",
            "______________________________"};


        int max_x;  // por definir
        int max_y;  // por definir
        int _x;     // por definir
        int _y;     // por definir


        int pts;    // Puntaje del juego
        int vidas;  // vidas de pacman
        int acum;   // pacdocts acumulados para activar el poder
        int pacts;  // pacdots consumidos
};

#endif // GAME_LOGIC_H
