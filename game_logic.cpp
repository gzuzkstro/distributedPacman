#include "game_logic.h"

game_logic::game_logic()
{
    // Inicializa ncurses
    initscr();
	noecho();
	curs_set(FALSE);
	nodelay (stdscr, TRUE);
	getmaxyx(stdscr, max_y, max_x);
	_x= (max_x/2)-15;
	_y=  max_y/6;

	// COLOR
	if (has_colors()) start_color();

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // Color para pacman, pactdocs, powerpiles
	init_pair(2, COLOR_BLUE, COLOR_BLUE);      // Color para las paredes
	init_pair(3, COLOR_RED, COLOR_BLACK);      // Fantasma 1
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);  // Fantasma 2
	init_pair(5, COLOR_CYAN, COLOR_BLACK);     // Fantasma 3
	init_pair(6, COLOR_GREEN, COLOR_BLACK);    // Fantasma 4
	init_pair(7, COLOR_BLUE, COLOR_BLACK);     // Fantasma comestibles
	init_pair(8, COLOR_WHITE, COLOR_BLACK);

	// MAPA


    clear();
}

game_logic::~game_logic()
{

}

void game_logic::draw()
{
    attron(COLOR_PAIR(1)); mvprintw(_y, _x, "%c",'P');
    attron(COLOR_PAIR(2)); mvprintw(_y + 1, _x + 1, "%c",'_');
    attron(COLOR_PAIR(3)); mvprintw(_y + 2, _x + 2, "%c",'F');
    attron(COLOR_PAIR(4)); mvprintw(_y + 3, _x + 3, "%c",'F');
    attron(COLOR_PAIR(5)); mvprintw(_y + 4, _x + 4, "%c",'F');
    attron(COLOR_PAIR(6)); mvprintw(_y + 5, _x + 5, "%c",'F');
    attron(COLOR_PAIR(7)); mvprintw(_y + 6, _x + 6, "%c",'F');
    attron(COLOR_PAIR(8)); mvprintw(_y + 7, _x + 7, "%c",'F');


}
