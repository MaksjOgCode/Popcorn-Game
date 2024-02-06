#pragma once

#include <Windows.h>
#include <cmath>
#include "Game_Core.hpp"

//----------------------------------------------------------------------------------------------------
# define M_PI           3.14159265358979323846  /* pi */

//||:-GAME GLOBAL: MULTIPLY FOR SIZE GAME OBJECTS-:||
# define GRAPHIC_GLOBAL_SCALE     3

//||||||||||||:-WORLD TRANSFORM: OFFSET-:||||||||||||
# define GRAPHIC_LEVEL_X_OFFSET   8
# define GRAPHIC_LEVEL_Y_OFFSET   6

//||||||||||||||:-GAME OBJECT: BRICK-:|||||||||||||||
# define GRAPHIC_BRICK_WIDTH      15
# define GRAPHIC_BRICK_HEIGHT     7

//|||||||||||||:-GAME OBJECT: PLATFORM-:|||||||||||||
# define GRAPHIC_CIRCLE_SIZE		7
# define GRAPHIC_INNER_WIDTH		21
# define GRAPHIC_PLATFORM_HEIGHT 7

//|||||||||||||:-GAME OBJECT: BALL-:|||||||||||||
#define GRAPHIC_BALL_SIZE        5

//|||||||||||||:-GAME LEVEL: CONSTANTS-:|||||||||||||
# define GRAPHIC_CELL_WIDTH      16
# define GRAPHIC_CELL_HEIGHT     8
# define GRAPHIC_LEVEL_X_OFFSET  8
# define GRAPHIC_LEVEL_Y_OFFSET  6

//|||||||||:-GAME COLOR BG: COLOR - COBALT-:||||||||
# define GRAPHIC_BG_COLOR        RGB(10, 28, 52)
//----------------------------------------------------------------------------------------------------
enum EBrickType {
   Pale_Blue   = 101,   /* RGB - (193, 217, 249):  */
   Blue        = 102,   /* RGB - (90, 137, 185):   */
   Woody_Blue  = 103    /* RGB - (46, 69, 83):     */
};

enum ELetterType {
   ELT_NONE    = 900,   /* The absence of a letter */
   ELT_O       = 901    /* Letter "O"              */
};

enum EKeyType {
   EKT_Left    = 1001,   /* Key "Arrow Left":      */
   EKT_Right   = 1002,   /* Key "Arrow Right":     */
   EKT_Space   = 1003    /* Key "Space":           */
};
//----------------------------------------------------------------------------------------------------
void EngineInit(HWND &hWnd);

void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset);


void SetBrickLetterColor(bool is_front_side, EBrickType brick_type,
                                             HBRUSH &front_brush, HBRUSH &back_brush);


void DrawBrickLetter(HDC hdc, int coord_x_pos, int coord_y_pos,
                              EBrickType brick_type, ELetterType letter_type, int rotation_step);


void DrawPlatform(HDC hdc, int coord_x_pos, int coord_y_pos);

void DrawLevel(HDC hdc);

void DrawFrame(HDC hdc, RECT &paint_area);

void RedrawPlatform();

int  OnKeyDown(EKeyType key);

int OnTimer();
//----------------------------------------------------------------------------------------------------