//
// Created by liujunliang on 2019/5/13.
//
#include "graphics.h"
#include "consts.h"
#include "drawers.h"
#include <winuser.h>

void keyboardEventHandler (int key, int event)
{
    switch (event) {
        case KEY_DOWN:STATE.ifKeyEvent = TRUE;
            STATE.KeyEvent = key;
            switch (key) {
                case VK_LEFT:
                case VK_RIGHT:break;
                case VK_DOWN:STATE.isSoftDrop = TRUE;
                    break;
                case VK_UP :STATE.isTurn = TRUE;
                    break;
            }
            break;
        case KEY_UP:
            switch (key) {
                case VK_DOWN:STATE.isSoftDrop = FALSE;
                    break;
                case VK_UP :STATE.isTurn = FALSE;
            }
            break;
    }
}
void mouseEventHandler (int x, int y, int button, int event)
{

}
