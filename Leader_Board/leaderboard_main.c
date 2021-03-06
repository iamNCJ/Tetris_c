/*
 * This file generates the leader board
 */

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "imgui.h"
#include "file_system_linked_list.h"

#define winwidth GetWindowWidth ()
#define winheight GetWindowHeight ()


#define TIMER_BLINK10  1     /*10ms定时器事件标志号*/
const int mseconds10 = 10;


static int enable_rotation = 1;   // 允许旋转
static int show_more_buttons = 0; // 显示更多按钮
userNode *head = NULL;

static char *const head_str = "Leader Board";
// 清屏函数，provided in libgraphics
void DisplayClear (void);

// 用户的显示函数
void display (void);

void DrawBasic ();
void RefreshDisplay ();
void DrawMenu ();

void Init_Rank_Data ();
void PrintList (userNode *head, double x, double y, int num);

void drawButtons ();

void DrawGrid (double x, double y, double width, double height,
               int columns, int rows);

void DrawBox (double x, double y, double width, double height);

// 用户的键盘事件响应函数
void KeyboardEventProcess (int key, int event)
{
    uiGetKeyboard (key, event); // GUI获取键盘
//    display ();
}

// 用户的鼠标事件响应函数
void MouseEventProcess (int x, int y, int button, int event)
{
    uiGetMouse (x, y, button, event); //GUI获取鼠标
//    display ();
}

void TimerEventProcess (int timerID)
{
    switch (timerID) {
        case TIMER_BLINK10: { /*10ms光标闪烁定时器*/
            display ();
            break;
        }
        default: break;
    }
}

// 用户主程序入口
// 仅初始化执行一次
void Main ()
{
    // 初始化窗口和图形系统
    SetWindowTitle (head_str);
    SetWindowSize (5, 5); // 单位 - 英寸
    //SetWindowSize(20, 10);
    //SetWindowSize(10, 20);  // 如果屏幕尺寸不够，则按比例缩小
    InitGraphics ();

    setMenuColors ("Black", "White", "Gray", "White", 1);

    // 注册时间响应函数
    registerKeyboardEvent (KeyboardEventProcess);// 键盘
    registerMouseEvent (MouseEventProcess);      // 鼠标
    registerTimerEvent (TimerEventProcess);

    // 打开控制台，方便输出变量信息，便于调试
//    InitConsole();

    Init_Rank_Data ();
    DrawBasic ();
    DrawMenu ();

    startTimer (TIMER_BLINK10, mseconds10);
}

// 菜单演示程序
void DrawMenu ()
{
    static char *menuListFile[] = {"File",
                                   "Refresh | Ctrl-R", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
//                                    "Close",
                                   "Exit       | Ctrl-E"};
//    static char *menuListTool[] = {"Tool",
//                                   "Triangle",
//                                   "Circle",
//                                   "Stop Rotation | Ctrl-T"};
    static char *menuListHelp[] = {"Help",
                                   "Show More  | Ctrl-M",
                                   "About"};
    static char *selectedLabel = NULL;

    double fH = GetFontHeight ();
    double x = 0; //fH/8;
    double y = winheight;
    double h = fH * 1.5; // 控件高度
//    double w = TextStringWidth (menuListHelp[0]) * 2; // 控件宽度
    double w = winwidth / 2;
    double wlist = TextStringWidth (menuListHelp[1]) * 1.2;
    double xindent = winheight / 20; // 缩进
    int selection;

    // File 菜单
    selection = menuList (GenUIID(0), x,
                          y - h, w, wlist, h, menuListFile, sizeof (menuListFile) / sizeof (menuListFile[0]));
    if (selection > 0) selectedLabel = menuListFile[selection];
    if (selection == 1)
        Init_Rank_Data ();
    if (selection == 2)
        exit (-1); // choose to exit

    // Tool 菜单
//    menuListTool[3] = enable_rotation ? "Stop Rotation | Ctrl-T" : "Start Rotation | Ctrl-T";
//    selection = menuList (GenUIID(0),
//                          x + w, y - h, w, wlist, h, menuListTool, sizeof (menuListTool) / sizeof (menuListTool[0]));
//    if (selection > 0) selectedLabel = menuListTool[selection];
//    if (selection == 3)
//        enable_rotation = !enable_rotation;

    // Help 菜单
    menuListHelp[1] = show_more_buttons ? "Show Less | Ctrl-M" : "Show More | Ctrl-M";
    selection = menuList (GenUIID(0),
                          x + w,
                          y - h, w, wlist, h, menuListHelp, sizeof (menuListHelp) / sizeof (menuListHelp[0]));
    if (selection > 0) selectedLabel = menuListHelp[selection];
    if (selection == 1)
        show_more_buttons = !show_more_buttons;
    if (selection == 2)
        MessageBoxA (NULL, "排行榜", "关于 | About", MB_ICONINFORMATION);;
}

void display ()
{
    // 清屏
    RefreshDisplay ();
    // 绘制和处理菜单
    DrawMenu ();
}

void RefreshDisplay ()
{
    DisplayClear ();
    DrawBasic ();
}

void DrawBasic ()
{
    SetFont ("微软雅黑");
    drawButtons ();
    SetPenColor ("Black");
    SetPointSize (32);
    MovePen (winwidth / 2 - TextStringWidth (head_str) / 2, winheight - 0.8);
    DrawTextString (head_str);
    SetPointSize (13);
//    DrawGrid (win_width/2 - 3, win_height/2 - 3, 2, 0.5, 3, 10);
    double x = 0.5;
    double y = 0.5;
    DrawBox (x, y, winwidth - 1, winheight - 1.5);
    PrintList (head, x, y + winheight - 1.5, ((show_more_buttons) ? 20 : 10));
}

void drawButtons ()
{
    double fH = GetFontHeight ();
    double h = fH * 2;  // 控件高度
    double x = winwidth / 2.5;
    double y = winheight / 2 - h;
    double w = winwidth / 5; // 控件宽度

    x = winwidth / 2 - 1;
    y = winheight / 2 - 0.6;
    setButtonColors ("Black", "White", "Gray", "White", 1);

}

void DrawBox (double x, double y, double width, double height)
{
    MovePen (x, y);
    DrawLine (0, height);
    DrawLine (width, 0);
    DrawLine (0, -height);
    DrawLine (-width, 0);
}

void DrawGrid (double x, double y, double width, double height,
               int columns, int rows)
{
    int i, j;

    for (i = 0; i < columns; i++) {
        for (j = 0; j < rows; j++) {
            DrawBox (x + i * width, y + j * height,
                     width, height);
        }
    }
}

void Init_Rank_Data ()
{
    head = Load_Rank ();
}

void PrintList (userNode *head, double x, double y, int num)
{
    char buffer[200];
    userNode *p;
    int cnt = 0;
    double d_h = (winheight - 1.5) / num;
    SetPenColor ("Black");
    SetPointSize (16);
    for (p = head; p && cnt < num; p = p->next) {
        cnt++;
        sprintf (buffer, "%10d %30s %10d", cnt, p->name, p->score);
        printf ("%s\n", buffer);
        MovePen (x, y - cnt * d_h);
        DrawTextString (buffer);
    }
}