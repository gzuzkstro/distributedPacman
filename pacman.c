#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>
#include <stdio.h>

#define DELAY 500000
#define FILA 22
#define COLUMNA 31
#define l -1
#define r 1
#define o 0
#define c 2
#define f 5
#define suspendido 20


void init() {

  setlocale(LC_ALL, "");
  initscr();
  noecho();
  curs_set(FALSE);
 // nodelay (stdscr, TRUE); //QUITALE EL COMENTARIO PARA LECT. TECLADO
  
  if (has_colors()) start_color();
  
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLUE);
}


void map(char matriz[][COLUMNA]) {

  for (int i = 0; i < FILA; ++i)
  {
    for (int j = 0; j < COLUMNA; ++j)
    {
       char caracter= matriz[i][j];
	if (caracter=='.')
	{
		attron(COLOR_PAIR(1));
		mvprintw(i,j, "%c",matriz[i][j]);
	}else{
		if (caracter=='|' || caracter=='_' || caracter=='=')
		{
			attron(COLOR_PAIR(2));
			mvprintw(i,j, "%c",matriz[i][j]);
		}else{			
			attron(COLOR_PAIR(1));
			mvprintw(i,j, "%c",matriz[i][j]);
		//	usleep(DELAY);			
		}
	}

		refresh();

    }
  }
 

//  getch();
}

int coordenadas(int pos_actual[][c], char pos_prox[][c], char matriz[][COLUMNA]) {
 int pos [f][c]; 
 int direccion, x, y, pts;
 int poder;
 char personaje;
 
for (int i = 0; i < f; ++i)
  {
    for (int j = 0; j < c; ++j)
    {
	switch(pos_prox [i][j])
        {
          case 'l':
             direccion = l;
             break;
          case 'r':
             direccion = r;
             break;
          case 'o':
             direccion = o;
             break;
        } 
      if(j==0){y= pos_actual [i][j] + direccion;} else {x= pos_actual [i][j] + direccion;}	 
    }


	switch(i)
        {
          case 0:
             personaje = 'K';
             break;
          case 1:
             personaje = 'm';
             break;
          case 2:
             personaje = 'r';
             break;
          case 3:
             personaje = 't';
             break;
          case 4:
             personaje = 's';
             break;
        } 
   attron(COLOR_PAIR(1));	mvprintw(30+i,1, "%c x %d y %d",personaje,x,y);
  if (matriz[y][x] == ' ')//-----------ESPACIO
 {
 	matriz[pos_actual[i][0]][pos_actual[i][1]] = ' ';
 	pos_actual[i][0] = y;
	pos_actual[i][1] = x;
	matriz[y][x] = personaje;	
 }else{
 	if (matriz[y][x] == '.')//-----------PTO
	 {
	 	if (i==0)
	 	{
	 		matriz[pos_actual[i][0]][pos_actual[i][1]] = ' ';
			pos_actual[i][0] = y;
			pos_actual[i][1] = x;
			pts ++;
			matriz[y][x] = personaje;
	 	}else{
	 		matriz[pos_actual[i][0]][pos_actual[i][1]] = ' ';
	 		pos_actual[i][0] = y;
			pos_actual[i][1] = x;
			matriz[y][x] = personaje;
	 	}
	
	 }else{
	 	if (matriz[y][x] == '*')//-----------PODER
	   	{	if (i==0)
		 	{
				pos_actual[i][0] = y;
				pos_actual[i][1] = x;
				poder = 1;
				matriz[y][x] = ' ';
		 	}else{
		 		pos_actual[i][0] = y;
				pos_actual[i][1] = x;
		 	}
	 	}else{//---------------------------PARED
	 		pos_actual[i][0] = pos_actual[i][0];
	    	pos_actual[i][1] = pos_actual[i][0];		
	 	}
	   
	}
 }}
 // getch();
}


int main(int argc, char *argv[]) {

 int pos_actual [f][c] = {
	{1,1},
	{9,11},
	{1,11},
	{10,14},
	{4,1}
	};

char pos_prox [f][c] = {
	{'o','r'}, //K
	{'l','o'}, //m
	{'r','o'}, //r
	{'o','r'}, //t
	{'r','o'} //s
	};


char  matriz[FILA][COLUMNA] = { 
  
{'=','=','=','=','=','=','=','=','=','=','=','=','=','=','=',' ','=','=','=','=','=','=','=','=','=','=','=','=','=','=','='},
{'|','K','.','.','.','.','.','.','.','.','.','r','.','.','|',' ','|','.','.','.','.','.','.','.','.','.','.','.','.','.','|'},
{'|','.','_','_','_','_','.','_','_','_','_','_','_','.','|',' ','|','.','_','_','_','_','_','_','.','_','_','_','_','.','|'},
{'|','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','|'},
{'|','s','_','_','_','_','.','|','.','_','_','_','_','_','_','_','_','_','_','_','_','_','.','|','.','_','_','_','_','.','|'},
{'|','.','_','_','_','_','.','|','.','.','.','.','.','.','.','|','.','.','.','.','.','.','.','|','.','_','_','_','_','.','|'},
{'|','.',' ',' ',' ',' ','.','|','.','_','_','_','_','_',' ',' ','|',' ',' ','_','_','_','_','_','|','.',' ',' ',' ','.','|'},
{'|','.','.','.','.','.','.','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|','.','.','.','.','.','.','|'},
{'=','=','=','=','=','=','.',' ',' ',' ','=','=','=',' ',' ',' ',' ',' ','=','=','=',' ',' ',' ','.','=','=','=','=','=','='},
{' ',' ',' ',' ',' ','|','.',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','.','|',' ',' ',' ',' ',' '},
{' ',' ',' ',' ',' ','|','.','|',' ',' ','|','m',' ',' ','t',' ',' ',' ',' ',' ','|',' ',' ','|','.','|',' ',' ',' ',' ',' '},
{' ',' ',' ',' ',' ','|','.','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|','.','|',' ',' ',' ',' ',' '},
{' ',' ',' ',' ',' ','|','.','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ','|','.','|',' ',' ',' ',' ',' '},
{'_','_','_','_','_','_','.','|',' ',' ','=','=','=','=','=','=','=','=','=','=','=',' ',' ','|','.','_','_','_','_','_','_'},
{'|','.','.','.','.','.','.','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','.','.','.','.','.','.','.','|'},
{'|','.','_','_','_','_','_','.','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','.','_','_','_','_','_','.','|'},
{'|','.','.','.','.','.','|','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','|','.','.','.','.','.','|'},
{'|','_','_','_','_','.','|','.','|','.','_','_','_','_','_','_','_','_','_','_','_','.','|','.','|','.','_','_','_','_','|'},
{'|','.','.','.','.','.','.','.','|','.','.','.','.','.','.','|','.','.','.','.','.','.','|','.','.','.','.','.','.','.','|'},
{'|','.','_','_','_','_','_','_','_','_','_','_','_','_','.','|','.','_','_','_','_','_','_','_','_','_','_','_','_','.','|'},
{'=','=','=','=','=','=','=','=','=','=','=','=','=','=','=',' ','=','=','=','=','=','=','=','=','=','=','=','=','=','=','='}
};  
 
  init();
  map(matriz);
  getch();
  coordenadas(pos_actual,pos_prox,matriz);
  map(matriz);
  getch();
  endwin();  

}
/* FUNCIÓN LECTURA DE TECLADO

if ((key = getch()) != ERR){
	 	attron(COLOR_PAIR(1));
		 mvprintw(0,0,"KEY");
	 }
 */