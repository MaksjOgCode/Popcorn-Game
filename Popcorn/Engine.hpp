#pragma once

#include <Windows.h>
#include <cmath>
#include "Game_Core.hpp"

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

enum EBrickType;

//----------------------------------------------------------------------------------------------------
void Init();

void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset);

void DrawPlatform(HDC hdc, int x, int y);

void DrawLevel(HDC hdc);

void DrawFrame(HDC hdc);
//----------------------------------------------------------------------------------------------------