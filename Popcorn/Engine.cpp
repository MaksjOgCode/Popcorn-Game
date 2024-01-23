#include "Engine.hpp"

HBRUSH Brick_Brush_PaleBlue,  Brick_Brush_Blue, Brick_Brush_WoodyBlue;
HPEN   Brick_Pen_White_Outline;

enum EBrickType {
   Pale_Blue   = 101,   // RGB - (193, 217, 249);
   Blue        = 102,   // RGB - (90, 137, 185);
   Woody_Blue  = 103    // RGB - (46, 69, 83);
};

char Level_01[14][12] =
{
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
   102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
   103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
   101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101,
};

//---------------------------------------------------------------------------------------------------
void Init()
{  // Setting up the game at startup :

   Brick_Brush_PaleBlue  = CreateSolidBrush(RGB(193, 217, 249));
   Brick_Brush_Blue      = CreateSolidBrush(RGB(90,  137, 185));
   Brick_Brush_WoodyBlue = CreateSolidBrush(RGB(46,  69,  83));

   Brick_Pen_White_Outline = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
}
//---------------------------------------------------------------------------------------------------
void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset)
{  // The function of rendering a graphic object <brick> :

   SelectObject(hdc, Brick_Pen_White_Outline);
   switch(color_brick) {
   case Woody_Blue: {
      SelectObject(hdc, Brick_Brush_WoodyBlue);
   } break;

   case Pale_Blue: {
      SelectObject(hdc, Brick_Brush_PaleBlue);
   } break;

   case Blue: {
      SelectObject(hdc, Brick_Brush_Blue);
   } break;

   default: 
      return;
   }

   RoundRect(  hdc, x_offset * GRAPHIC_GLOBAL_SCALE,
               y_offset * GRAPHIC_GLOBAL_SCALE,
               (x_offset + GRAPHIC_BRICK_WIDTH)  * GRAPHIC_GLOBAL_SCALE, 
               (y_offset + GRAPHIC_BRICK_HEIGHT) * GRAPHIC_GLOBAL_SCALE,
               2 * GRAPHIC_GLOBAL_SCALE, 2 * GRAPHIC_GLOBAL_SCALE );
}
//---------------------------------------------------------------------------------------------------
void DrawFrame(HDC hdc)
{  // The function of rendering the game frame :

   for (int i = 0; i < 14; ++i) {
      for (int j = 0; j < 12; ++j) {
         DrawBrick(  hdc, (EBrickType)Level_01[i][j],
                     GRAPHIC_LEVEL_X_OFFSET + j * (GRAPHIC_BRICK_WIDTH + 1),
                     GRAPHIC_LEVEL_Y_OFFSET + i * (GRAPHIC_BRICK_HEIGHT + 1)  );
      }
   }
}
//---------------------------------------------------------------------------------------------------