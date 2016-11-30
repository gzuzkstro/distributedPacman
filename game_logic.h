#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#define MAXACUM 10
#define X_INFO 40
#define Y_INFO 5
#define NUM_CONN 2

#define PACMAN 'C'
#define PACDOT '.'
#define CELL_W '_'
#define POWERP 'o'
#define CELL_E ' '
#define CELL_B '-'
#define PINKY 'P'
#define CLYDE 'K'
#define INKY 'I'
#define BLINKY 'B'

using namespace std;

void initNcurses();

class game_logic
{
    public:
        game_logic();
        virtual ~game_logic();
        void print_map(char c,int x,int y);
        void draw();
        void print_info();
        void nextState();
        char* acum_str(int n);
        void setAcum(int n);
        int getAcum();
        void setPts(int n);
        int getPts();
        void setVidas(int n);
        int getVidas();
        void setPacts(int n);
        int getPacts();
        void setCell(int x, int y, char c);
        char getCell(int x, int y);
        int *getDir(int pos);
        void setDir(int pos, int val);
        int *getSync(int pos);
        void setSync(int pos, int val);
        int getNumPlayers();
        void setNumPlayers(int val);
    protected:
    private:
        // Mapa del juego
        char mapa[35][35] = {
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


        int _x;
        int _y;


        int pts;    // Puntaje del juego
        int vidas;  // vidas de pacman
        int acum;   // pacdocts acumulados para activar el poder
        int pacts;  // pacdots consumidos
        int dirs[NUM_CONN];   // arreglo de la ultima tecla precionada por cada jugador
        int sync[NUM_CONN];   // eso era para hacer el manejo de las cuestinoes del tiempo
        int num_players;      // Num de jugadores

        int aux;
        int daux;
};

#endif // GAME_LOGIC_H
