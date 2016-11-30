#include "game_logic.h"

game_logic::game_logic()
{
	_x= 0;
	_y= 0;
	pts = 0;
    vidas = 3;
    acum = 0;
    pacts = 0;


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
	case '.':
        attron(COLOR_PAIR(1));
		break;
	case 'P':
        attron(COLOR_PAIR(1));
		break;
	case '_':
        attron(COLOR_PAIR(2));
		break;
	case ' ':
        attron(COLOR_PAIR(9));
		break;
	case '-':
        attron(COLOR_PAIR(8));
		break;
    case 'o':
        attron(COLOR_PAIR(8));
		break;
    case 'F':
        attron(COLOR_PAIR(3));
		break;
    case 'G':
        attron(COLOR_PAIR(4));
		break;
    case 'M':
        attron(COLOR_PAIR(5));
		break;
    case 'N':
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
    print_info();
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
