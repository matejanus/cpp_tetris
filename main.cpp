#include <iostream>
#include "includes/defines.hpp"
#include <array>
#include <curses.h>
#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include <random>
#include <cstdio>
#include <cwchar>
#include <stdio.h> 
#include <wchar.h>

using namespace std::chrono_literals;

std::array<std::string, 7> tetromino; 

const int nFieldHeight = {18};
const int nFieldWidth = {12};

const int nScreenWidth = {80};
const int nScreenHeight = {20};

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

bool doesPieceFit(int nTetromino, int nRotaion, int nPosX, int nPosY, const std::array<unsigned char, (nFieldWidth * nFieldHeight)>& gameField)
{
    for (int x = 0; x < 4; x++)
        for (int y=0; y<4; y++)
        {
            int piece = rotate(x, y, nRotaion);

            int field = (nPosY + y) *nFieldWidth + (nPosX + x);

            if (nPosX + x >= 0 && nPosX +x < nFieldWidth)
            {
                if (nPosY + y >= 0 && nPosY + y <nFieldHeight)
                {
                    if(tetromino[nTetromino][piece] == 'X' && gameField[field] !=0)
                        return false;
                }
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

     std::array<unsigned char, (nFieldWidth * nFieldHeight)> pField = {0};

    for(int x=0; x<nFieldWidth; x++)
    {
        for(int y=0; y<nFieldHeight; y++)
        {
            pField[y*nFieldWidth + x] = (x == 0 || x==nFieldWidth-1 || y==nFieldHeight -1) ? 9 : 0;
        }
    }


    std::array<char, (nScreenWidth * nScreenHeight)> screen = {0};

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

   
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth /2;
    int nCurrentY = 0;

    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = 0;

    int nPieceCount = 0;
    int nScore = 0;
    std::vector<int> vLines;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> gen(0,6);
    int nCurrentPiece = gen(rng);
    while (!gameOver)
    {
        //  Game timitng
        std::this_thread::sleep_for(50ms);
        nSpeedCounter++;
        bForceDown = (nSpeedCounter==nSpeed);
        
        // input
        bool bKey[4] = {false};

        int ch;
        ch = wgetch(win);
        switch(ch)
        {
            case KEY_RIGHT:
                bKey[0] = true;
                break;
            case KEY_LEFT:
                bKey[1] = true;
                break;
            case KEY_DOWN:
                bKey[2] = true;
                break;
            case 122:
                bKey[3] = true;
                break;
            default:
                break;
        }
        //  game logic

        nCurrentX += (bKey[0] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY, pField)) ? 1:0;
        nCurrentX -= (bKey[1] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY, pField)) ? 1:0;
        nCurrentY += (bKey[2] && doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1, pField)) ? 1:0;

        if(bKey[3])
        {
            nCurrentRotation  += (!bRotateHold && doesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY, pField)) ? 1:0;
            bRotateHold = true;
        }
        else
        {
            bRotateHold =false;
        }

        if(bForceDown)
        {
            if(doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1, pField))
            {
                nCurrentY++;
            }
            else
            {
                //lock te current piece
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        if(tetromino[nCurrentPiece][rotate(px,py,nCurrentRotation)] != '.')
                        {
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                        }
                nPieceCount++;
                if(nPieceCount % 10 == 0)
                    if(nSpeed >=10 )
                        nSpeed --;
                //check for horizontal lines
                for(int py = 0; py<4; py++)
                    if(nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for(int px = 1; px < nFieldWidth - 1; px++)
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) !=0;

                        if(bLine)
                        {
                            for(int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            
                            vLines.push_back(nCurrentY + py);   
                        }
                    }

                nScore +=25;
                if(!vLines.empty())
                {
                    nScore += (1 << vLines.size()) *100;
                }
                //choose next piece
                nCurrentX = nFieldWidth/2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = gen(rng);

                //if piece does nof fit
                gameOver = !doesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY, pField);
            }
            nSpeedCounter = 0;
        }

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
        
        snprintf(&screen[(nScreenHeight-1) * nScreenWidth + nFieldWidth + 6],16, "SCORE: %8d", nScore);// TODO: find another way of appending to screen
        
        if(!vLines.empty())
        {
            wprintw(win, screen.data());
            wmove(win, 0, 0);
            wrefresh(win);
            std::this_thread::sleep_for(400ms);

            for(auto &v: vLines)
            {
                for(int px = 1; px < nFieldWidth - 1; px++)
                {
                    for(int py = v; py > 0; py--)
                    {
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    }
                    pField[px] = 0;
                }
            }
            vLines.clear();
    }
        
        // display frame
        wprintw(win, screen.data());
        wmove(win, 0, 0);
        wrefresh(win);

    }
    
    endwin();
    std::cout<<"Game over! Score: " <<nScore << std::endl;
    getchar();
    return 0;
}