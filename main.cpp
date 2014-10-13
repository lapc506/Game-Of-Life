#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define GRIDSIZE 70
#define SCREENSIZE 900
#include <windows.h>
#include <winbgim.h>
#include <string>


int radius = SCREENSIZE/GRIDSIZE/2;
int colorRED = 10;
int colorGREEN = 20;
int colorBLUE = 5;

bool direccion = true;

using namespace std;

void fill(int* S){
	time_t t;
	srand(time(&t));

	int i, j;
	for(i = 0; i<GRIDSIZE; i++)
		for(j = 0; j<GRIDSIZE; j++)
			*(S + i*GRIDSIZE + j) = rand()%8/7;//probablity of 1/8 of starting alive
}

void addLife (int* S, int x, int y) {
    int i = x/radius/2;
    int j = y/radius/2;
    *(S + j * GRIDSIZE + i) = 1;
}

void takeLife (int* S, int x, int y) {
    int i = x/radius/2;
    int j = y/radius/2;
    *(S + j * GRIDSIZE + i) = 0;
}

int * life(int* S){
	int i,j,p,q,s;
	int* S2 = (int*) malloc(GRIDSIZE*GRIDSIZE*sizeof(int));
	for(i = 0; i<GRIDSIZE; i++)
		for(j = 0; j<GRIDSIZE; j++){
			s = 0;

			for(p = i-1; p<=i+1; p++)
				for(q = j-1; q<=j+1; q++)
					if(p >= 0 && p<GRIDSIZE)
						s += *(S + (p+GRIDSIZE)%GRIDSIZE*GRIDSIZE + (q+GRIDSIZE)%GRIDSIZE);

			if(s - *(S + i*GRIDSIZE + j) == 3 || s == 3)
				*(S2 + i*GRIDSIZE + j) = 1;
			else
				*(S2 + i*GRIDSIZE + j) = 0;
		}
	return S2;
}



void print(int* S)
{

	int i, j;
	for(i = 0; i<GRIDSIZE; i++){
		for(j = 0; j<GRIDSIZE; j++){


			if(*(S + i*GRIDSIZE + j) == 1){ setcolor(COLOR(colorRED, colorGREEN, colorBLUE)); circle((2*j+1)*radius, (2*i+1)*radius, radius);}
			else { setcolor(0); circle((2*j+1)*radius, (2*i+1)*radius, radius);}
            }


            if (direccion == true) {
                colorGREEN+=1; colorRED+=6; colorBLUE+=8; if (colorGREEN > 255) direccion = false;
            }
            if (direccion == false) {
                colorGREEN-=1; colorRED-=3; colorBLUE-=4; if (colorGREEN < 1) direccion = true;
            }
            //outtextxy(0, i*8, filas.c_str());
		}
		//printf("\n")
        delay(10);
}

char control(){

    char control =getch();
    if(control == ' ') delay(120);
    return control;
}

int main(){

    initwindow(SCREENSIZE+radius,SCREENSIZE+radius);

    int xMouse;
    int yMouse;

	int* S = (int*) malloc(GRIDSIZE*GRIDSIZE*sizeof(int));
	int* S2;
	//fill(S);

	do
	{

		S2 = life(S);
		free(S);
		S = S2;
		print(S);

        while(!kbhit()){
            getmouseclick(WM_LBUTTONDOWN, xMouse, yMouse);
            if (xMouse!=-1) addLife(S, xMouse, yMouse);
            getmouseclick(WM_RBUTTONDOWN, xMouse, yMouse);
            if (xMouse!=-1) takeLife(S, xMouse, yMouse);

            xMouse= -1;
            print(S);
        }
		//Sleep();
	} while(control());

	return 0;

}
