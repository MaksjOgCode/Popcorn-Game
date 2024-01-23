#pragma once

#include <Windows.h>
#include "Game_Core.hpp"

#define GRAPHIC_GLOBAL_SCALE     3
#define GRAPHIC_LEVEL_X_OFFSET   8
#define GRAPHIC_LEVEL_Y_OFFSET   6
#define GRAPHIC_BRICK_WIDTH      15
#define GRAPHIC_BRICK_HEIGHT     7

enum EBrickType;

void Init();
void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset);
void DrawFrame(HDC hdc);