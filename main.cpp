#include <iostream>
#include "includes/defines.hpp"
#include <array>
#include <curses.h>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

std::array<std::string, 7> tetromino; 

int nFieldHeight = {18};
int nFieldWidth = {12};

int nScreenWidth = {80};
int nScreenHeight = {20};
unsigned char *pField = nullptr;

int rotate(int px, int py, int r)
{
    switch (r % 4)
    {
        case 0: 
            return py * 4 + px; //0deg
        case 1: 
            return 12 + py - (px * 4); //90deg
        case 2: 
            return 15 - (py * 4) - px; //180deg
        case 3: 
            return 3 - py + (px * 4); //270deg
    }
    return 0;
}

bool doesPieceFit(int nTetromino, int nRotaion, int nPosX, int nPosY)
{
    for (int x = 0; x < 4; x++)
        for (int y=0; y<4; y++)
        {
            int piece = rotate(x, y, nRotaion);

            int field = (nPosY + y) *nFieldWidth + (nPosX + x);

            if (nPosY + y >= 0 && nPosY + y <nFieldWidth)
            {
                if(tetromino[nTetromino][piece] == 'X' && pField[field] !=0)
                    return false;
            }
        }


    return true;
}

int main()
{
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");

    tetromino[1].append("..X.");
    tetromino[1].append(".XX.");
    tetromino[1].append(".X..");
    tetromino[1].append("....");

    tetromino[2].append(".X..");
    tetromino[2].append(".XX.");
    tetromino[2].append("..X.");
    tetromino[2].append("....");

    tetromino[3].append("....");
    tetromino[3].append(".XX.");
    tetromino[3].append(".XX.");
    tetromino[3].append("....");

    tetromino[4].append("..X.");
    tetromino[4].append(".XX.");
    tetromino[4].append("..X.");
    tetromino[4].append("....");

    tetromino[5].append("....");
    tetromino[5].append(".XX.");
    tetromino[5].append("..X.");
    tetromino[5].append("..X.");

    tetromino[6].append("....");
    tetromino[6].append("..XX");
    tetromino[6].append("..X.");
    tetromino[6].append("..X.");

    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for(int x=0; x<nFieldWidth; x++)
    {
        for(int y=0; y<nFieldHeight; y++)
        {
            pField[y*nFieldWidth + x] = (x == 0 || x==nFieldWidth-1 || y==nFieldHeight -1) ? 9 : 0;
        }
    }


    char *screen = new char[nScreenWidth*nScreenHeight];
    for (int i=0; i<nScreenWidth*nScreenHeight; i++)
    {
        screen[i] = L' ';
    }

    initscr();


    WINDOW * win = newwin(nScreenHeight, nScreenWidth, 0, 0);
    wrefresh(win);
    wmove(win, 0, 0);
    curs_set(0);
    keypad(win, TRUE);
    cbreak();
    noecho();
    nodelay(win, TRUE);

    bool gameOver = false;

    int nCurrentPiece = 1;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth /2;
    int nCurrentY = 0;

    while (!gameOver)
    {
        //  Game timitng
        std::this_thread::sleep_for(50ms);

        // input

        //  game logic

        // render output

        //draw field 
        for (int x=0; x< nFieldWidth; x++)
            for(int y=0; y<nFieldHeight; y++)
                screen[(y+2) * nScreenWidth + (x+2)] = " ABCDEFG=#"[pField[y* nFieldWidth + x]];
        
        
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)] == 'X')
                {
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px +2)] = nCurrentPiece + 65;
                }
        
        // display frame
        wprintw(win, screen);
        wmove(win, 0, 0);
        wrefresh(win);
    }
    
    endwin();

    return 0;
}