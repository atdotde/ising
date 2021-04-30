/* Simple 2D Ising simulation
   Written by Robert C. Helling <helling@atdotde.de>
   
   Published under GPL 2.0

   Compile as clang -O4 ising.c -lm -o ising
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define GRIDSIZE 600
#define PLOTSIZE 80

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <windows.h>

/* Turns the cursor on/off */

void show_console_cursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

/*Sets cursor to the top left position to overwrite terminal output*/

void home() {
	/*Initialise objects for cursor manipulation*/
	HANDLE hStdout;
	COORD destCoord;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	/*Sets coordinates of cursor to (0,0) */
	destCoord.X = 0;
	destCoord.Y = 0;
	SetConsoleCursorPosition(hStdout, destCoord);

	show_console_cursor(false);
}

void fullscreen(bool bl) {
	if (bl) {
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	}
}


int random() {
	return rand();
}

void srandom(unsigned seed){
	srand(seed);
}

void system_init() {
	fullscreen(true);
	show_console_cursor(false);
}

#else

void system_init() {
}

void home() {
	printf("\033[1;1H");
}
#endif

int grid[GRIDSIZE][GRIDSIZE];

int torus(int a) {
	return a < 0 ? a + GRIDSIZE : (a >= GRIDSIZE ? a - GRIDSIZE : a);
}

void init(){
	int x,y;
	FILE *mag;

	system_init();
	mag = fopen("magnetization","w");
	fclose(mag);

	for (x=0; x<GRIDSIZE; x++)
		for (y=0; y<GRIDSIZE; y++)
			grid[x][y] = random()/(float)RAND_MAX < 0.3  ? -1 : 1;

	srandom((int) time(NULL) % (1 << 31));
}

void output(){
	int x,y;
	int m=0;
	FILE *mag;

    /* Cursor home */
	home();
	for (x=0; x<PLOTSIZE; x++){
		for (y=0; y<PLOTSIZE*2; y++){
			if (grid[x][y] == 1)
				putchar('*');
			else
				putchar('.');
		}
		putchar('\n');
	}

	for (x=0; x<GRIDSIZE; x++)
		for (y=0; y<GRIDSIZE; y++)
			m += grid[x][y];
	
	printf("m=%f\n",m/(float)(GRIDSIZE * GRIDSIZE));
	mag = fopen("magnetization","a");
	fprintf(mag, "%f\n",m/(float)(GRIDSIZE * GRIDSIZE));
	fclose(mag);
}

void update(float J, float H){
	int x = (int)(random() * GRIDSIZE/RAND_MAX);
	int y = (int)(random() * GRIDSIZE/RAND_MAX);

	if (x >= GRIDSIZE || y >= GRIDSIZE)
		return;

	int sum = grid[x][torus(y+1)] + grid[x][torus(y - 1)] + grid[torus(x + 1)][y] + grid[torus(x - 1)][y];

	double E = 2.0 * grid[x][y] * (J * sum + H);

	if ((E < 0.0) || (exp(-E) > (random()/(double)RAND_MAX)))
		grid[x][y] *= -1;
}
		

int main(){

	float T = 1.0;
    float J = 0.44;
	float H = 0.001;
	
	long t;

	init();

	for (t=0L; 1; t++){
		update(J / T, H / T);
        if(!(t % 100000)){
			output();
		}
	}
	return(0);
}
