#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#define MAXACUM 10
#define X_INFO 40
#define Y_INFO 5
#define NUM_CONN 5
#define MAX_CONN 5

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

#define POS_Y_PACMAN 16
#define POS_X_PACMAN 15
#define POS_Y_BLINKY 14
#define POS_X_BLINKY 13
#define POS_Y_PINKY 14
#define POS_X_PINKY 14
#define POS_Y_INKY 14
#define POS_X_INKY 15
#define POS_Y_CLYDE 14
#define POS_X_CLYDE 16

#define PACS_MAP 244
#define MAX_X 30
#define MAX_Y 30
#define PACPTS 10

#define P1 30
#define P2 10

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
        int *getOldDir(int pos);
        void setOldDir(int pos, int val);
        int *getSync(int pos);
        void setSync(int pos, int val);
        int getNumPlayers();
        void setNumPlayers(int val);
        void calcularMov(int mov[5][2]);
        void asignarPos(int mov[5][2]);
        int *getPosArray(int player);
        void compararPos();
        void resetPos();
        void endGame(bool s);
        void pacmanWin();
        int mov_y(int i);
        int mov_x(int i);
        char (*getMap(void))[35];
        void resetPosFantasma(int i);
        bool poderActivo(int i);
        void contadorPoder();
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



        int pts;    // Puntaje del juego
        int vidas;  // vidas de pacman
        int acum;   // pacdocts acumulados para activar el poder
        int pacts;  // pacdots consumidos
        int old_dirs[MAX_CONN] = {KEY_RIGHT,KEY_UP,KEY_UP,KEY_UP, KEY_UP}; // penultima tecla presionada por el usuario
        int dirs[MAX_CONN] = {KEY_RIGHT,KEY_UP,KEY_UP,KEY_UP, KEY_UP};   // arreglo de la ultima tecla precionada por cada jugador
        int sync[NUM_CONN];   // eso era para hacer el manejo de las cuestinoes del tiempo
        int num_players;      // Num de jugadores
        char char_players[MAX_CONN] = {PACMAN,BLINKY,PINKY,INKY,CLYDE};
        int pos_players[MAX_CONN][2]  = {{POS_Y_PACMAN,POS_X_PACMAN}, // Posiciones de los jugadores
                                        {POS_Y_BLINKY,POS_X_BLINKY},
                                        {POS_Y_PINKY,POS_X_PINKY},
                                        {POS_Y_INKY,POS_X_INKY},
                                        {POS_Y_CLYDE,POS_X_CLYDE}};
        // 0: indica que el juego continua | 1: indica que pacman gana | -1: indica que pacman pierde
        int game_status;
        int poderes[2];
        int delay_fantasma[MAX_CONN];
};

#endif // GAME_LOGIC_H
