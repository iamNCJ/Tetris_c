//
// Created by 刘隽良 on 2019/5/15.
//
#ifndef _consts_h
#define _consts_h

#include <genlib.h>

#define BLOCKSIZE 0.5

#define WIDTH 32 //Width in inches
#define HEIGHT 18 //Height in inches

#define MAINTAINER 0

#define ERA 1000
//struct block{
//    int x,y;
//    string color;
//};

#define DEBUG_SLOW 0.06
#define SLOW 0.06
#define FAST 8

typedef struct tetriminoes {
    int x, y;
    int type; //对应一个二维数组表示形状  int shape[][],通过矩阵变换实现旋转 0 表示空对象
    string color;
    int direction;
    double yVelocity;
} tetrimino;

struct state {
    bool isFalling;
};

extern struct state STATE;

extern int typeInfo[][4][2];

extern string TETRI_COLOR[8];


#endif