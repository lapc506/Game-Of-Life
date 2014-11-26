#ifndef LIFESYSTEM_H
#define LIFESYSTEM_H
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbgim.h>

class LifeSystem
{
    private:
        static const int ALIVE = 1;
        static const int DEAD = 0;
        static const int GRIDSIZE = 70;
        int SCREENSIZE;
        int radius;
        int colorRED;
        int colorGREEN;
        int colorBLUE;
        int xMouse;
        int yMouse;
        int (**lifeMatrix); // int* S;
        bool direccionOndaTriangularColor;

        void beginLife(){
            unsigned long semilla_num_random = ( time(NULL) % 1000) % 13;
            for(int i = 0; i < GRIDSIZE; i++) {
                for(int j = 0; j < GRIDSIZE; j++) {
                    semilla_num_random = (214013 * semilla_num_random + 2531011);
                    lifeMatrix[i][j] = ((semilla_num_random >> 16 & 0x7FFF) % 8) / 7;
                            // Reducing the probability of starting alive to 1/8 of chances
                }
            }
        }
        void addLifeAtMousePosition() {
            lifeMatrix[xMouse/radius/2][yMouse/radius/2] = 1;
        }   // int i = xMouse/radius/2; int j = yMouse/radius/2; *(S + j * GRIDSIZE + i) = 1;

        void removeLifeAtMousePosition() {
            lifeMatrix[xMouse/radius/2][yMouse/radius/2] = 0;
        }   // int i = xMouse/radius/2; int j = yMouse/radius/2; *(S + j * GRIDSIZE + i) = 0;

        void refreshMatrix(){
            // Initialize a new integer matrix for calculating the new "living beings".
            int** tempLifeMatrix = new int*[GRIDSIZE];
            for(int i = 0; i<GRIDSIZE; i++){
                for(int j = 0; j<GRIDSIZE; j++){
                    tempLifeMatrix[i] = new int[GRIDSIZE];
                    tempLifeMatrix[i][j] = DEAD;
                }
            }

            // For every "living being" in the life matrix.
            for(int i = 0; i<GRIDSIZE; i++)
                for(int j = 0; j<GRIDSIZE; j++){

                    int neighbors = 0;

                    // Here the Life rules are applied.
                    for(int p = i-1; p <= i+1; p++)
                        for(int q = j-1; q <= j+1; q++)
                            if(p >= 0 && p < GRIDSIZE)
                                neighbors += lifeMatrix[(p+GRIDSIZE)%GRIDSIZE][(q+GRIDSIZE)%GRIDSIZE];
                                // neighbors += lifeMatrix[p][q];
                                // s += *(lifeMatrix + (p+GRIDSIZE)%GRIDSIZE * GRIDSIZE + (q+GRIDSIZE) % GRIDSIZE);

                    if(neighbors - lifeMatrix[i][j] == 3 || neighbors == 3)
                        tempLifeMatrix[i][j] = ALIVE;
                    else
                        tempLifeMatrix[i][j] = DEAD;
                }

            // Replace old "living being"s with the new ones.
            for(int i = 0; i < GRIDSIZE; i++){
                for(int j = 0; j < GRIDSIZE; j++){
                    lifeMatrix[i][j] = tempLifeMatrix[i][j];
                }
            }
            delete tempLifeMatrix;
        }
        void drawMatrix()
        {
            for(int i = 0; i < GRIDSIZE; i++){
                for(int j = 0; j < GRIDSIZE; j++){
                    if(lifeMatrix[i][j] == 1){
                        setcolor(COLOR(colorRED, colorGREEN, colorBLUE));
                        circle((2*j+1)*radius, (2*i+1)*radius, radius);
                    } else {
                        setcolor(0); // Black color is the default background color.
                        circle((2*j+1)*radius, (2*i+1)*radius, radius);
                    }
                }
                if (direccionOndaTriangularColor == true) {
                    colorGREEN +=1;
                    colorRED   +=6;
                    colorBLUE  +=8;
                    if (colorGREEN > 255) direccionOndaTriangularColor = false;
                }
                if (direccionOndaTriangularColor == false) {
                    colorGREEN -=1;
                    colorRED   -=3;
                    colorBLUE  -=4;
                    if (colorGREEN < 1)   direccionOndaTriangularColor = true;
                }
                // delay(10);
            }
        }
        bool canContinue(){
            char control = getch();
            if(control == ' ') delay(120);
            return (control != 'x');
        }
    public:
        LifeSystem() {
            // GRIDSIZE = 70;
            SCREENSIZE = 900;
            radius = SCREENSIZE/GRIDSIZE/2;
            colorRED = 10;
            colorGREEN = 20;
            colorBLUE = 5;
            direccionOndaTriangularColor = true; // true sube, false baja.
            lifeMatrix = new int*[GRIDSIZE];
            for(int i = 0; i<GRIDSIZE; i++){
                for(int j = 0; j<GRIDSIZE; j++){
                    lifeMatrix[i] = new int[GRIDSIZE];
                    lifeMatrix[i][j] = DEAD;
                }
            }
            char option;
            std::cout << "Press 'x' whenever you want to exit the game." << std::endl;
            std::cout << "Do you wish to previously fill the matrix? y/n" << std::endl;
            std::cin >> option;
            if (option == 'y') beginLife();
        }
        void run(){
            initwindow(SCREENSIZE + radius, SCREENSIZE + radius);
            // Radius is added to allow some little more space at both
            // inferior and right margins for thw circles to get drawn.
            do
            {
                refreshMatrix();
                drawMatrix();
                while(!kbhit()){
                    getmouseclick(WM_LBUTTONDOWN, yMouse, xMouse); if (xMouse != -1) addLifeAtMousePosition();
                    getmouseclick(WM_RBUTTONDOWN, yMouse, xMouse); if (xMouse != -1) removeLifeAtMousePosition();
                    xMouse = -1;
                    drawMatrix();
                }
            } while(canContinue());
        }
        ~LifeSystem() {
            delete lifeMatrix;
        }
};

#endif // LIFESYSTEM_H
