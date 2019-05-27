//
// Created by Tao Chiang on 5/17/2019.
//

#ifndef _MODEL_H_
#define _MODEL_H_

#include "consts.h"

void InitModel();

tetrimino generateTetrimino (int type, int direction);

void timerEventHandler (int timerID);

tetrimino tetriMaintainer_on_gravity (int time, tetrimino tetri);
tetrimino tetriRandom ();
bool check_collision (tetrimino tetri);
void Settle_Tetri(tetrimino tetri);
tetrimino HardDrop(tetrimino tetri);
void Settle(tetrimino tetri);


bool CheckTop ();
tetrimino Restart ();
void Pulse();

extern Checkerboard checkerboard;

extern tetrimino ctetri;

extern int Score;
extern bool is_game_over;
extern double globalSpeed;
extern tetrimino que[2];
extern tetrimino HoledTetri;

#endif //_MODEL_H_
