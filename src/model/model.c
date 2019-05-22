//
// Created by Tao Chiang on 5/17/2019.
//
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "model.h"
#include "consts.h"
#include "drawers.h"

//static string TETRI_COLOR[8] = {
//    "White",//for null
////    "BLUE",
//    "Magenta",
////    "DarkBlue",
//    "Blue",
////    "Orange",
//    "Cyan",
//    "Yellow",
//    "Green",
//    "Cyan",
//    "Red"
//};


int block_color[14][25] = {0}; // store the colors of block, white as 0, (x,y),  extended space are for easier(lazier) check...
tetrimino ctetri;

tetrimino generateTetrimino (int type, int direction)
{
    tetrimino tetri;

    tetri.x = WIDTH / 2;
    tetri.y = HEIGHT;
    tetri.type = type;
    tetri.direction = direction;
    tetri.color = TETRI_COLOR[type];
    tetri.yVelocity = DEBUG_SLOW;

//    if (type) {
//        drawTetri (tetri);
//    }
    return tetri;
}

void timerEventHandler (int timerID)
{
    static int time = 0;

    if (!STATE.isFalling) {
        ctetri = tetriRandom ();
        ctetri = tetriMaintainer_on_gravity (time, ctetri);
        STATE.isFalling = TRUE;
    } else {
//        if (tetri.y < 0) {
//            STATE.isFalling = FALSE;
//        } // now not in use...
    }

    time = (time + 1) % ERA; // !!!
    Clean ();
    drawInit ();

    ctetri = tetriMaintainer_on_gravity (time, ctetri);
    drawTetri (ctetri);
    DrawGrid();
}

tetrimino tetriMaintainer_on_gravity (int time, tetrimino tetri)
{
    static int curTime = 0;
    static double dy = 0;
    int dt;
    tetrimino last = tetri;
    if (time > curTime) {
        dt = (time - curTime);
    } else {
        dt = (time + ERA - curTime);
    }

    dy += tetri.yVelocity * dt;
    if (dy >= 1) {
        tetri.y -= 1;
        dy = 0;
    } // printf("%d\n",dt);

    curTime = time;
    if (!check_collision (tetri))
    {
        return tetri;
    }
    else
    {
        Settle_Tetri (last);
        return last;
    }
}

tetrimino tetriRandom ()
{
    static int last = 0;
    int type;
    do {
        type = rand () % 7;
        if (type == 0) type = 7;
    } while (last == type);
    last = type;
    int direction = rand () % 4;
    return generateTetrimino (type, direction);
}

void InitState ()
{
    STATE.isFalling = FALSE;
}

void InitModel ()
{
    int i;
    for (i = 0; i < 20; i++)
        block_color[0][i] = block_color[13][i] = 1;
    for (i = 1; i < 13; i++)
        block_color[i][0] = 1; // block_color[i][19] =
    // rewrite the boundary as 1
    block_color[3][3] = 2;
    block_color[10][10] = 5;
}

bool check_collision (tetrimino tetri)
{

    switch (tetri.direction) {
        case 0:
            for (int i = 0; i < 4; i++) {
                if (block_color[tetri.x + typeInfo[tetri.type][i][0]-9][tetri.y + typeInfo[tetri.type][i][1]+1])
                    return TRUE;
            }
            return FALSE;
            break;
        case 1:
            for (int i = 0; i < 4; i++) {
                if (block_color[tetri.x - typeInfo[tetri.type][i][1]-9][tetri.y + typeInfo[tetri.type][i][0]+1])
                    return TRUE;
            }
            return FALSE;
            break;
        case 2:
            for (int i = 0; i < 4; i++) {
                if (block_color[tetri.x - typeInfo[tetri.type][i][0]-9][tetri.y - typeInfo[tetri.type][i][1]+1])
                    return TRUE;
            }
            return FALSE;
            break;
        case 3:
            for (int i = 0; i < 4; i++) {
                if (block_color[tetri.x + typeInfo[tetri.type][i][1]-9][tetri.y - typeInfo[tetri.type][i][0]+1])
                    return TRUE;
            }
            return FALSE;
            break;
    }
}

void Settle_Tetri (tetrimino tetri)
{
    switch (tetri.direction) {
        case 0:
            for (int i = 0; i < 4; i++) {
                 block_color[tetri.x + typeInfo[tetri.type][i][0]-9][tetri.y + typeInfo[tetri.type][i][1]+1] = tetri.type;
            }
            break;
        case 1:
            for (int i = 0; i < 4; i++) {
                block_color[tetri.x - typeInfo[tetri.type][i][1]-9][tetri.y + typeInfo[tetri.type][i][0]+1] = tetri.type;
            }
            break;
        case 2:
            for (int i = 0; i < 4; i++) {
                block_color[tetri.x - typeInfo[tetri.type][i][0]-9][tetri.y - typeInfo[tetri.type][i][1]+1] = tetri.type;
            }
            break;
        case 3:
            for (int i = 0; i < 4; i++) {
                block_color[tetri.x + typeInfo[tetri.type][i][1]-9][tetri.y - typeInfo[tetri.type][i][0]+1] = tetri.type;
            }
            break;
    }
    STATE.isFalling = FALSE;
}
