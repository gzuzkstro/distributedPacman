#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY 50000

int main(int argc, char *argv[]) {


	char matriz [22][31];
	int max_x=0, max_y=0;
	int x = 0;
 	int y = 0;
 	int pac_x = 1;
 	int pac_y = 1;
 	int next_x = 0;
 	int pts = 0;
 	int key = 0;
	
	initscr();
	noecho();
	curs_set(FALSE);
	nodelay (stdscr, TRUE);
	getmaxyx(stdscr, max_y, max_x);
	x= (max_x/2)-15;
	y=  max_y/6; 

	//COLOR
	if (has_colors()) start_color();
	
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);


 clear();

  for (int c = 0; c < 22; ++c)
 {
 	for (int b = 0; b < 31; ++b)
 	{
 		matriz[c][b]=' ';
 	}
 }
 
 for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 31; ++j)
		{
			//PTS FILAS
			if ((((i==1)||(i==3)) || ((i==16)||(i==18))) || (i==20))
			{
				matriz [i][j]='.';			
			}

			//WALL 
			if ((i==2) || (i==4) || (i==8) || (i==13) || (i==15) || (i==17) || (i==19))
			{
				matriz [i][j]='_';			
			}

			//PTS COLUMNA
			if (((j==1)||(j==6)) || ((j==24)||(j==29))  || ((i==5) || (i==7)) || i==14)
			{
				matriz [i][j]='.';			
			}

			//WALL MARCO
			if ((j==0)||(j==30))
			{
				matriz [i][j]='|';
			}
			
			if ((i==0)||(i==21))
			{
				matriz [i][j]='=';			
			}
			
			//FILA 0 - 4
			if (((i==0)||(i==1)) || ((i==2)||(i==4)))
			{
				if (i!=4){
					
					if(j==15)				
					{
						matriz [i][j]=' ';
					}else{
						if ((j==14)||(j==16))
						{
							matriz [i][j]='|';
						}
					}
				}	
				if (((i==4) && ((j==8)||(j==22))) || ((i==2) && ((j==13)||(j==17))))
				{
					matriz[i][j]='.';
				}
							
			}

			//FILA 5 -7
			if (((i==5)||(i==6))||(i==7))
			{
				
				if ((i==5) && ((j>1 && j<6) || (j>24 && j<29)))
				{
					matriz[i][j]='_';
				}else{
					if ((i==5) && ((j==7)||(j==15)||(j==23)))
					{
						matriz[i][j]='|';
					}
				}

				if (i==6)
				 {
				 	if ((j>7 && j<13) || (j>17 && j<23))
				 	{
				 		matriz[i][j]='_';												
				 	}else{
				 		if ((j==7) || (j==15) || (j==23))
				 		{
				 			matriz[i][j]='|';
				 		}
				 	}
				 } 
				
				if (i==7)
				 {	
				 	if (j>7 && j<23)
				 	{
				 		matriz[i][j]=' ';												
				 	}else{
				 		if (j==7 || j==23)
				 		{
				 			matriz[i][j]='|';
				 		}
				 	}
				 } 
					
			}

			//FILA 8
			if (i==8)
			{
				if ((j>6 && j<10) || (j>12 && j<18) || (j>20 && j<24))
			 	{
			 		matriz[i][j]=' ';												
			 	}
			}
			

			//FILA 9 - 13
			if (i>8 && i<13)
			{
				if (j==1 || j==29)
				{
					matriz[i][j]=' ';
				}else{
					if (j==5 || j==10 || j==20 || j==25)
					{
						matriz[i][j]='|';
					}
				}
			}

			if (i>9 && i<14)
			{
				if (j==7 || j==23)
				{
					matriz[i][j]='|';
				}else{
					if (i==13){
						if (j>9 && j<21)
					 	{
					 		matriz[i][j]='=';												
					 	}else{
					 		if (j==8 || j==9 || j==21 || j==22)
						 	{
						 		matriz[i][j]=' ';												
						 	}
					 	}
					}
				}
			
			}

			if (i==13 && (j==5 || j==25))
		 	{
		 		matriz[i][j]='|';												
		 	}

			//FILA 14
			if ((i==14) && (j>7 && j<23))
		 	{
		 		matriz[i][j]=' ';												
		 	}
		 	
		 	//FILA 15
			if ((i==15) && (j==7 || j==23))
		 	{
		 		matriz[i][j]='.';												
		 	}

		 	if ((i==15) && (j==6 || j==24))
		 	{
		 		matriz[i][j]='_';												
		 	}

		 	//FILA 16 
			if ((i==16 || i==17) && (j==6 || j==24))
		 	{
		 		matriz[i][j]='|';												
		 	}

		 	//FILA 17 - 18 
			if ((i==17 || i==18) && (j==8 || j==22))
		 	{
		 		matriz[i][j]='|';												
		 	}

		 	if (i==18 && j==15)
		 	{
		 		matriz[i][j]='|';												
		 	}
			
		 	//FILA 17
		 	if (i==17){
			 	if (j==5 || j==7 || j==9 || j==25 || j==21 || j==23)
				{
					matriz[i][j]='.';
				}else{
					if (j==1 || j==29)
					{
						matriz[i][j]='_';
					}
				}
			}

			//FILA 19 
			if (i==19 && (j==14 || j==16))
		 	{
		 		matriz[i][j]='.';												
		 	}

		 	if (i==19 && (j==6 || j==24))
		 	{
		 		matriz[i][j]='_';												
		 	}
		}		
 }
while(1){
	//clear();

	next_x = pac_x+1;

	 if (matriz[pac_y][next_x] == ' ')
	 {
	 	matriz[pac_y][pac_x] = 'O';
	 	pac_x = next_x; 	
	 }else{
	 	if (matriz[pac_y][next_x] == '.')
		 {
		 	matriz[pac_y][pac_x] = 'N';
		 	pac_x = next_x;		 	
		 	pts ++;
		 }
	 }

	matriz[pac_y][pac_x]='K';

	for (int i = 0; i < 22; ++i)
	{
		for (int j = 0; j < 31; ++j)
		{			
			char caracter= matriz[i][j];
			if (caracter=='.')
			{
				attron(COLOR_PAIR(1));
				mvprintw(y+i, x+j, "%c",matriz[i][j]);
			}else{
				if (caracter=='|' || caracter=='_' || caracter=='=')
				{
					attron(COLOR_PAIR(2));
					mvprintw(y+i, x+j, "%c",matriz[i][j]);
				}else{
					if (caracter=='K')
					{
						attron(COLOR_PAIR(1));
						mvprintw(y+i, x+j, "%c",matriz[i][j]);
						usleep(DELAY); // Shorter delay between movements
						mvprintw(0,0,"   ");
					}
				}
			}
		}
	}
	refresh();

	 if ((key = getch()) != ERR){
	 	attron(COLOR_PAIR(1));
		 mvprintw(0,0,"KEY");
	 }
	 
	}

//getch();
 endwin();
}
