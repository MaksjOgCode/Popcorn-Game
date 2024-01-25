#include "Engine.hpp"

HBRUSH Brush_PaleBlue,  Brush_Blue, Brush_WoodyBlue;
HPEN   Brick_Pen_White_Outline, Platform_Pen_White_Outline;

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

   Brush_PaleBlue  = CreateSolidBrush(RGB(193, 217, 249));
   Brush_Blue      = CreateSolidBrush(RGB(90,  137, 185));
   Brush_WoodyBlue = CreateSolidBrush(RGB(46,  69,  83));

   Brick_Pen_White_Outline    = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
   Platform_Pen_White_Outline = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
}
//---------------------------------------------------------------------------------------------------
void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset)
{  // The function of the drawning game object: Brick

   SelectObject(hdc, Brick_Pen_White_Outline);
   switch(color_brick) {
   case Woody_Blue: {
      SelectObject(hdc, Brush_WoodyBlue);
   } break;

   case Pale_Blue: {
      SelectObject(hdc, Brush_PaleBlue);
   } break;

   case Blue: {
      SelectObject(hdc, Brush_Blue);
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
void DrawPlatform(HDC hdc, int coord_x_pos, int coord_y_pos)
{  // The function of the drawning game object: Platform

   SelectObject(hdc, Brush_PaleBlue);
   SelectObject(hdc, Platform_Pen_White_Outline);

   for (short i = 0, offset = 0; i < 2; ++i, offset += 21)     // Draw two cicrle (the first, and the second drawning with offset x_coord (21)) :
      Ellipse( hdc, (coord_x_pos + offset) * GRAPHIC_GLOBAL_SCALE,
                     coord_y_pos * GRAPHIC_GLOBAL_SCALE,
                    (coord_x_pos + GRAPHIC_CIRCLE_SIZE + offset) * GRAPHIC_GLOBAL_SCALE,
                    (coord_y_pos + GRAPHIC_CIRCLE_SIZE) * GRAPHIC_GLOBAL_SCALE);
   
   // Draw highlight on circle :
   Arc(hdc, (coord_x_pos + 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_y_pos + 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_x_pos + GRAPHIC_CIRCLE_SIZE - 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_y_pos + GRAPHIC_CIRCLE_SIZE - 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_x_pos + 1 + 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_y_pos + 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_x_pos + 1) * GRAPHIC_GLOBAL_SCALE,
            (coord_y_pos + 1 + 2) * GRAPHIC_GLOBAL_SCALE);


   SelectObject(hdc, Brush_Blue);
   RoundRect(hdc, (coord_x_pos + 4) * GRAPHIC_GLOBAL_SCALE,
                  (coord_y_pos + 1) * GRAPHIC_GLOBAL_SCALE,
                  (coord_x_pos + 4 + GRAPHIC_INNER_WIDTH - 1) * GRAPHIC_GLOBAL_SCALE,
                  (coord_y_pos + 1 + 5) * GRAPHIC_GLOBAL_SCALE,
                  3 * GRAPHIC_GLOBAL_SCALE, 3 * GRAPHIC_GLOBAL_SCALE);
}
//---------------------------------------------------------------------------------------------------
void DrawLevel(HDC hdc)
{  // The function of drawning the level of game :

   for (int i = 0; i < 14; ++i) {
      for (int j = 0; j < 12; ++j) {
         DrawBrick(  hdc, (EBrickType)Level_01[i][j],
            GRAPHIC_LEVEL_X_OFFSET + j * (GRAPHIC_BRICK_WIDTH + 1),
            GRAPHIC_LEVEL_Y_OFFSET + i * (GRAPHIC_BRICK_HEIGHT + 1)  );
      }
   }
}
//---------------------------------------------------------------------------------------------------
void DrawFrame(HDC hdc)
{  // The function of rendering the game frame :

   DrawLevel(hdc);

   DrawPlatform(hdc, 80, 190);
}
//---------------------------------------------------------------------------------------------------