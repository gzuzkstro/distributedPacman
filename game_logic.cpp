#include "game_logic.h"

game_logic::game_logic()
{
	pts = 0;
    vidas = 3;
    acum = 0;
    pacts = 0;
    game_status = 0;

}

game_logic::~game_logic()
{
    endwin();
}

void initNcurses()
{
    // Inicializa ncurses
    initscr();
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, FALSE);
	keypad(stdscr, TRUE);
	cbreak();

	// COLOR
	if (has_colors()) start_color();

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // Color para pacman, pactdocs, powerpiles
	init_pair(2, COLOR_BLUE, COLOR_BLUE);      // Color para las paredes
	init_pair(3, COLOR_RED, COLOR_BLACK);      // Fantasma 1
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);  // Fantasma 2
	init_pair(5, COLOR_CYAN, COLOR_BLACK);     // Fantasma 3
	init_pair(6, COLOR_GREEN, COLOR_BLACK);    // Fantasma 4
	init_pair(7, COLOR_BLUE, COLOR_BLACK);     // Fantasma comestibles
	init_pair(8, COLOR_WHITE, COLOR_BLACK);    // Fantasmas comestibles y pared donde salen los fantasmas de la caja
	init_pair(9, COLOR_BLACK, COLOR_BLACK);    // Es pacio vacio

    clear();
}


void game_logic::print_map(char c,int x,int y)
{
    switch(c)
{
	case PACDOT:
        attron(COLOR_PAIR(1));
		break;
	case PACMAN:
        attron(COLOR_PAIR(1));
		break;
	case CELL_W:
        attron(COLOR_PAIR(2));
		break;
	case CELL_E:
        attron(COLOR_PAIR(9));
		break;
	case CELL_B:
        attron(COLOR_PAIR(8));
		break;
    case POWERP:
        attron(COLOR_PAIR(8));
		break;
    case PINKY:
        attron(COLOR_PAIR(3));
		break;
    case INKY:
        attron(COLOR_PAIR(4));
		break;
    case BLINKY:
        attron(COLOR_PAIR(5));
		break;
    case CLYDE:
        attron(COLOR_PAIR(6));
		break;
}

mvprintw(x, y, "%c",c);

}

char * game_logic::acum_str(int n)
{
    char str[11];
    int i;

    for ( i = 0; i < n && i < 10 ; i++)
        str[i] = '.';
    str[i] = '\0';

    return str;
}

void game_logic::print_info()
{
    attron(COLOR_PAIR(1));
    mvprintw(Y_INFO, X_INFO, "%s: %d","score", pts);
    mvprintw(Y_INFO + 2, X_INFO, "%s: %d","lives", vidas);
    if (acum >= 10)
        mvprintw(Y_INFO + 4, X_INFO, "activar poder en \"p\": %s", acum_str(acum));
    else
        mvprintw(Y_INFO + 4, X_INFO, "pacts acumulados: %s", acum_str(acum));

    mvprintw(Y_INFO + 6, X_INFO, "%s: %d","pacts", pacts);

    // Imprime si pacman gana o pierde segun el estatus del juego
    if( game_status == 1)
        mvprintw(Y_INFO + 8, X_INFO, "%s","PACMAN GANA !!!!");
    else if (game_status == -1)
        mvprintw(Y_INFO + 8, X_INFO, "%s","PACMAN PIERDE :( , Mejor suerte la proxima >:-P");

}

void game_logic::draw()
{
    for (int i = 0 ; i < 30 ; i++ )
    {
        for (int j = 0 ; j < 30 ; j++ )
        {
            this->print_map(mapa[i][j], i, j);
        }
    }
    for (int i = NUM_CONN-1; i >= 0  ; i--)
    {
        this->print_map(char_players[i], pos_players[i][0], pos_players[i][1]);
    }
    print_info();
}


void game_logic::calcularMov(int mov[5][2])
{
// En las asignaciones el 0 representa "y" y el 1 representa "x"

    for(int i = 0; i < MAX_CONN ; i++)
    {
        switch(dirs[i])
        {
            case KEY_UP:
                mov[i][0] =-1;
                mov[i][1] =0;
                break;
            case KEY_DOWN:
                mov[i][0] =1;
                mov[i][1] =0;
                break;
            case KEY_LEFT:
                mov[i][0] =0;
                mov[i][1] =-1;
                break;
            case KEY_RIGHT:
                mov[i][0] =0;
                mov[i][1] =1;
                break;
        }
    }
}

void game_logic::asignarPos(int mov[5][2])
{
    for (int i = 0 ; i < NUM_CONN ; i++)
    {
        int *y = &(pos_players[i][0]);
        int *x = &(pos_players[i][1]);
        int ny = *y + mov[i][0];
        int nx = *x + mov[i][1];

        if (nx < 0) {
            *x = 29;
            continue;
            }

        if (nx > 29) {
            *x = 0;
            continue;
            }

        if (i == 0) {
            switch(mapa[ny][nx])
            {
                case CELL_W:
                case CELL_B:
                    continue;
                    break;
                case PACDOT:
                    mapa[ny][nx] = CELL_E;
                    pts += PACPTS;
                    acum++;
                    pacts++;
                    break;
                case POWERP:
                    mapa[ny][nx] = CELL_E;
                    pts += PACPTS*2;
                    pacts++;
                    break;
            }
        } else {
            if (mapa[ny][nx] == CELL_W)
                continue;
        }
        // Se actualiza la posicion
        *y = ny;
        *x = nx;
    }
}

void game_logic::resetPos()
{
    pos_players[0][0] = POS_Y_PACMAN; pos_players[0][1] = POS_X_PACMAN;
    pos_players[1][0] = POS_Y_BLINKY; pos_players[1][1] = POS_X_BLINKY;
    pos_players[2][0] = POS_Y_PINKY; pos_players[2][1] = POS_X_PINKY;
    pos_players[3][0] = POS_Y_INKY; pos_players[3][1] = POS_X_INKY;
    pos_players[4][0] = POS_Y_CLYDE; pos_players[4][1] = POS_X_CLYDE;

}

void game_logic::endGame(bool s)
{
    attron(COLOR_PAIR(1));
    if  (s)
    {
        game_status = 1;
    } else
    {
        game_status = -1;
    }
}

void game_logic::compararPos()
{
    for (int i = 1 ; i < NUM_CONN ; i++) {
        if (pos_players[i][0] == pos_players[0][0] && pos_players[i][1] == pos_players[0][1]) {
            vidas--;
            if ( vidas > 0) {
                resetPos();
            } else {
                endGame(FALSE);
            }

        }
    }
}

// Calculo del estado del juego
void game_logic::nextState()
{
    int mov[5][2];
    calcularMov(mov);
    asignarPos(mov);
    compararPos();

}


// Sets y gets
void game_logic::setAcum(int n)
{
    this->acum = n;
}

int game_logic::getAcum()
{
    return this->acum;
}


int game_logic::getPts()
{
    return this->pts;
}

void game_logic::setPts(int n)
{
    this->pts = n;
}

int game_logic::getVidas()
{
    return this->vidas;
}

void game_logic::setVidas(int n)
{
    this->vidas = n;
}

int game_logic::getPacts()
{
    return this->pacts;
}

void game_logic::setPacts(int n)
{
    this->pacts = n;
}

void game_logic::setCell(int x, int y, char c)
{
    this->mapa[x][y] = c;
}

char game_logic::getCell(int x, int y)
{
    return this->mapa[x][y];
}

int *game_logic::getDir(int pos)
{
    return &(this->dirs[pos]);
}

void game_logic::setDir(int pos, int val)
{
    this->dirs[pos] = val;
}

int *game_logic::getSync(int pos)
{
    return &(this->sync[pos]);
}

void game_logic::setSync(int pos, int val)
{
    this->sync[pos] = val;
}

int game_logic::getNumPlayers()
{
    return this->num_players;
}

void game_logic::setNumPlayers(int val)
{
    this->num_players = val;
}
