#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define BOMB_RATIO 0.2

struct Cell
{
    int value;
    bool bomb;
    bool show;
}cell;

void ranBomb(int dim);
void values(int dim);
void reset(int dim);
void setGame(int dim);

void revealTitle(int i, int j, int dim);
void createBoard(int dim);
void playBoard(int dim);
void revealAll(int dim);

//quality of life functions

//If you click on a zero, it reveals all titles around it until it reaches a non-zero
void reveal0(int i, int j,int dim);

//flag 3

//not dying of rip 4
void fakeDeath(int i, int j, int dim);
bool checkAround(int x, int y, int i, int j);
//create icon and image for number and bomba 1

struct Cell titles[16][16];
bool firstTime;

int main(void) {
    InitWindow(800,800, "Minesweeper");
    SetTargetFPS(20);
    int dim = 16;

    //srand(time(NULL));

    srand(0);
    setGame(dim);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

        GuiSetIconScale(2);
        createBoard(dim);
        playBoard(dim);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        if (GuiButton((Rectangle){ 236, 28, 200, 30 }, "New Game")) {
            setGame(dim);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void playBoard(int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            revealTitle(i,j,dim);
        }
    }
}

void createBoard(int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (titles[i][j].show==0) {
                (titles[i][j]).show = GuiButton((Rectangle){ 80+i*40, 80+j*40, 40, 40 },"");
            }
        }
    }
}

void ranBomb(int dim)
{
    int num = (int)((float)dim*(float)dim * BOMB_RATIO);
    for(int i = 0; i < num; i++)
    {
        int x = rand() % dim;
        int y = rand() % dim;
        while(titles[x][y].bomb)
        {
            x = rand() % dim;
            y = rand() % dim;
        }
        titles[x][y].bomb = true;
    }
}

/*
 *This function has 4 for loops and 4 if statements
 *But it is only used to go through a 2d array and check if the surrounding titles are bombs or not
 *it will count how many bombs are surrounding the title and assign that number as the value of the title
 */
void values(int dim)
{
    int bc = 0;

    for(int i = 0; i < dim; i++)//first 2 for loops iterates through the entire 2d array
    {
        for(int j = 0; j < dim; j++)//they set a title as a "point" for the next 2 for loops
        {

            bc=0;
            for(int n=-1;n<2;n++)//next 2 for loops iterates around the 8 titles surrounding the "point" title
            {
                for(int m = -1; m<2;m++)
                {
                    if(n!=0||m!=0)//checks to see if they iterate on the "point" title and skips it
                    {
                        if(n+i!=-1&&m+j!=-1)//check to see if the for loops iterate outside the bounds (top and left)
                        {
                            if(n+i!=dim&&m+j!=dim)//check to see if the for loops iterate outside the bounds (bottom and right)
                            {
                                if(titles[i+n][j+m].bomb==true)//counts if the iterated title is a bomb or not
                                    {bc++;}
                            }
                        }
                    }
                }
            }
            titles[i][j].value = bc;//assigns value

        }
    }
}

void reset(int dim) {
    for(int i =0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            (titles[i][j]).value = 0;
            (titles[i][j]).bomb = false;
            (titles[i][j]).show = false;
        }
    }
}

void setGame(int dim) {
    reset(dim);
    ranBomb(dim);
    values(dim);
    firstTime = true;
}

void reveal0(int i, int j,int dim) {
    for(int n=-1;n<2;n++)
    {
        for(int m = -1; m<2;m++)
        {
            if(n!=0||m!=0)
            {
                if(n+i!=-1&&m+j!=-1)//check to see if the for loops iterate outside the bounds (top and left)
                {
                    if(n+i!=dim&&m+j!=dim)//check to see if the for loops iterate outside the bounds (bottom and right)
                    {
                        titles[i+n][j+m].show=true;
                    }
                }
            }
        }
    }
}

void revealTitle(int i, int j, int dim) {
    if (titles[i][j].show) {
        if (firstTime) {
            fakeDeath(i, j, dim);
            values(dim);
            firstTime = false;
        }
        if (titles[i][j].bomb) {
            GuiLabel((Rectangle){ 80+i*40, 80+j*40, 40, 40 },"#234#");
            revealAll(dim);
            GuiLabel((Rectangle){ 360, 720, 100, 50 },"Game Over");
        }
        else{
            char num[2];
            sprintf(num,"%d",titles[i][j].value);
            num[1]='\0';
            GuiLabel((Rectangle){ 80+i*40, 80+j*40, 40, 40 },num);
            if (titles[i][j].value==0) {
                reveal0(i,j,dim);
            }
        }
    }
}

void revealAll(int dim) {
    for(int i =0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            titles[i][j].show = true;
        }
    }
}

void fakeDeath(int i, int j, int dim) {
    int counter = 0;
    for(int n=-1;n<2;n++) {
        for(int m=-1;m<2;m++) {
            if(n+i!=-1 && m+j!=-1) {
                if (n+i!=dim && m+j!=dim) {
                    if (titles[n+i][m+j].bomb) {
                        titles[n+i][m+j].bomb = false;
                        counter++;
                    }
                }
            }
        }
    }

    for(int i = 0; i < counter; i++)
    {
        int x = rand() % dim;
        int y = rand() % dim;
        while(titles[x][y].bomb || checkAround(x, y, i, j))
        {
            x = rand() % dim;
            y = rand() % dim;
        }
        titles[x][y].bomb = true;
    }
}

bool checkAround(int x, int y, int i, int j) {
    for(int n=-1;n<2;n++) {
        if (i+n==x||j+n==y) {
            return true;
        }
    }
    return false;
}