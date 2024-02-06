#include "Engine.hpp"

HBRUSH Brush_PaleBlue,  Brush_Blue, Brush_WoodyBlue, Brush_BG, Brush_White;
HPEN   Brick_Pen_White_Outline, Platform_Pen_White_Outline, Letter_Pen_Black, BG_Pen;

HWND hwnd;

const int Level_Width = 14;
const int Level_Height = 12;

int Platform_X_Pos = 0;
int Platform_Y_Pos = 185;
int Platform_X_Step = 1 * GRAPHIC_GLOBAL_SCALE * 2;
int Platform_Width = 28;

int Ball_X_Pos = 20, Ball_Y_Pos = 150;
int Ball_X_Offset = 1, Ball_Y_Offset = 1;

RECT Platform_Rect, Prev_Platform_Rect;
RECT BALL_Rect, Prev_Ball_Rect;
RECT Level_Rect;
RECT Ball_Rect;

char Level_01[Level_Width][Level_Height] =
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
void EngineInit(HWND &hWnd)
{  /* Setting up the game at startup : */

   hwnd = hWnd;

   Brush_PaleBlue  = CreateSolidBrush(RGB(193, 217, 249));
   Brush_Blue      = CreateSolidBrush(RGB(90,  137, 185));
   Brush_WoodyBlue = CreateSolidBrush(RGB(46,  69,  83));
   Brush_BG        = CreateSolidBrush(GRAPHIC_BG_COLOR);
   Brush_White     = CreateSolidBrush(RGB(255, 255, 255));

   Brick_Pen_White_Outline    = CreatePen(PS_DASHDOT, 1, RGB(255, 255, 255));
   Platform_Pen_White_Outline = CreatePen(PS_SOLID, 1, GRAPHIC_BG_COLOR);
   Letter_Pen_Black           = CreatePen(PS_DASHDOT, 4, RGB(10, 28, 52));
   BG_Pen                     = CreatePen(PS_SOLID, 3, GRAPHIC_BG_COLOR);

   Level_Rect.left = GRAPHIC_LEVEL_X_OFFSET * GRAPHIC_GLOBAL_SCALE;
   Level_Rect.top = GRAPHIC_LEVEL_X_OFFSET * GRAPHIC_GLOBAL_SCALE;
   Level_Rect.right = Level_Rect.left + GRAPHIC_CELL_WIDTH * Level_Width * GRAPHIC_GLOBAL_SCALE;
   Level_Rect.bottom = Level_Rect.top + GRAPHIC_BRICK_HEIGHT * Level_Height * GRAPHIC_GLOBAL_SCALE;

   RedrawPlatform();

   SetTimer(hWnd, WM_USER + 1, 50, 0);
}
//---------------------------------------------------------------------------------------------------
void DrawBrick(HDC hdc, EBrickType color_brick, int x_offset, int y_offset)
{  /* The function of the drawning game object: Brick */

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
void SetBrickLetterColor(bool is_front_side, EBrickType brick_type, HBRUSH &front_brush, HBRUSH &back_brush)
{  /* The function responsible for correctly displaying the color of the side of the brick during animation */

   if (is_front_side) {
      switch (brick_type)
      {
      case Pale_Blue: {
         front_brush = Brush_WoodyBlue;
         back_brush  = Brush_PaleBlue;
      } break;

      case Woody_Blue: {
         front_brush = Brush_PaleBlue;
         back_brush  = Brush_WoodyBlue;
      } break;

      case Blue: {
         front_brush = Brush_PaleBlue;
         back_brush  = Brush_Blue;
      } break;

      default: {
         return;
      } break;
      }
   } else {
      switch (brick_type)
      {
      case Pale_Blue: {
         front_brush = Brush_PaleBlue;
         back_brush  = Brush_WoodyBlue;
      } break;

      case Woody_Blue: {
         front_brush = Brush_WoodyBlue;
         back_brush  = Brush_PaleBlue;
      } break;

      case Blue: {
         front_brush = Brush_Blue;
         back_brush  = Brush_PaleBlue;
      } break;

      default: {
         return;
      } break;
      }
   }

}
//---------------------------------------------------------------------------------------------------
void DrawBrickLetter(HDC hdc, int coord_x_pos, int coord_y_pos, EBrickType brick_type, ELetterType letter_type, int rotation_step)
{  /* The function drawning animation on fall brick : */

	double offset;
   double rotation_angle;
	int brick_half_height = GRAPHIC_BRICK_HEIGHT * GRAPHIC_GLOBAL_SCALE / 2;
	int back_part_offset;
   HBRUSH front_brush, back_brush;
	XFORM xform, old_xform;

   /* Adjusting the rotation pitch and rotation angle */
   rotation_step = rotation_step % 16;

   /* Converting a step to a rotation angle */
   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step);

   /* Edge case processing */
   if (!(brick_type == Pale_Blue || brick_type == Blue || brick_type == Woody_Blue)) { return; }

   if (rotation_step > 4 && rotation_step <= 12)
      SetBrickLetterColor(true, brick_type, front_brush, back_brush);
   else
      SetBrickLetterColor(false, brick_type, front_brush, back_brush);

	if (rotation_step == 4 || rotation_step == 12)
	{
      /* Brick background */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, coord_x_pos,
                     coord_y_pos + brick_half_height - GRAPHIC_GLOBAL_SCALE,
                     coord_x_pos + GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                     coord_y_pos + brick_half_height);

      /* The front of the brick : */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, coord_x_pos,
                     coord_y_pos + brick_half_height,
                     coord_x_pos + GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                     coord_y_pos + brick_half_height + GRAPHIC_GLOBAL_SCALE - 1);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);

		/* The matrix of the letter reversal : */
		xform.eM11 = 1.0f;
		xform.eM12 = 0.0f;
		xform.eM21 = 0.0f;
		xform.eM22 = (float)cos(rotation_angle);
		xform.eDx = (float)coord_x_pos;
		xform.eDy = (float)coord_y_pos + (float)(brick_half_height);
		GetWorldTransform(hdc, &old_xform);
		SetWorldTransform(hdc, &xform);
	
		/* Brick background */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, back_brush);

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)GRAPHIC_GLOBAL_SCALE;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset,
                        GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                        brick_half_height - back_part_offset);

      /* The front of the brick : */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, 0, -brick_half_height,
                        GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                        brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         SelectObject(hdc, Letter_Pen_Black);

         switch (letter_type)
         {
         case ELT_O: {
            Ellipse(hdc, 0 + 5 * GRAPHIC_GLOBAL_SCALE,
                       (-5 * GRAPHIC_GLOBAL_SCALE) / 2,
                         0 + 10 * GRAPHIC_GLOBAL_SCALE,
                         5 * GRAPHIC_GLOBAL_SCALE / 2);
         } break;

         default: break;
         }
      }

		SetWorldTransform(hdc, &old_xform);
   }

}
//---------------------------------------------------------------------------------------------------
void DrawPlatform(HDC hdc, int coord_x_pos, int coord_y_pos)
{  /* The function of the drawning game object: Platform */

   SelectObject(hdc, Brush_BG);
   SelectObject(hdc, BG_Pen);

   Rectangle(hdc, Prev_Platform_Rect.left,   
                  Prev_Platform_Rect.top,
                  Prev_Platform_Rect.right,
                  Prev_Platform_Rect.bottom);

   SelectObject(hdc, Brush_PaleBlue);
   SelectObject(hdc, Platform_Pen_White_Outline);

   for (short i = 0, offset = 0; i < 2; ++i, offset += 21)     /* Draw two cicrle (the first, and the second drawning with offset x_coord (21)) :  */
      Ellipse( hdc, (coord_x_pos + offset) * GRAPHIC_GLOBAL_SCALE,
                     coord_y_pos * GRAPHIC_GLOBAL_SCALE,
                    (coord_x_pos + GRAPHIC_CIRCLE_SIZE + offset) * GRAPHIC_GLOBAL_SCALE,
                    (coord_y_pos + GRAPHIC_CIRCLE_SIZE) * GRAPHIC_GLOBAL_SCALE);
   
   /* Draw highlight on circle : */
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
void DrawBall(HDC hdc, RECT &paint_area)
{
   // 1 - Clear background
   SelectObject(hdc, Brush_BG);
   SelectObject(hdc, BG_Pen);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   // 2 - Draw game object ball
   Ball_Rect.left    = (Ball_X_Pos + GRAPHIC_LEVEL_X_OFFSET) * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.top     = (Ball_Y_Pos + GRAPHIC_LEVEL_Y_OFFSET) * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.right   = Ball_Rect.left + GRAPHIC_BALL_SIZE * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.bottom  = Ball_Rect.top + GRAPHIC_BALL_SIZE * GRAPHIC_GLOBAL_SCALE;

   SelectObject(hdc, Brush_White);
   SelectObject(hdc, BG_Pen);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

}
//---------------------------------------------------------------------------------------------------
void DrawLevel(HDC hdc)
{  /* The function of drawning the level of game : */

   for (int i = 0; i < 14; ++i) {
      for (int j = 0; j < 12; ++j) {
         DrawBrick(  hdc, (EBrickType)Level_01[i][j],
            GRAPHIC_LEVEL_X_OFFSET + j * (GRAPHIC_BRICK_WIDTH + 1),
            GRAPHIC_LEVEL_Y_OFFSET + i * (GRAPHIC_BRICK_HEIGHT + 1)  );
      }
   }
}
//---------------------------------------------------------------------------------------------------
void DrawFrame(HDC hdc, RECT &paint_area)
{  /* The function of rendering the game frame :   */

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
      DrawLevel(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
      DrawPlatform(hdc, (GRAPHIC_LEVEL_X_OFFSET + Platform_X_Pos), Platform_Y_Pos);

   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
      DrawBall(hdc, paint_area);

  /* for(int i = 0; i < 16; ++i) {
      DrawBrickLetter(hdc, 20 + i * 16 * GRAPHIC_GLOBAL_SCALE, 100, Woody_Blue, ELT_O, i);
      DrawBrickLetter(hdc, 20 + i * 16 * GRAPHIC_GLOBAL_SCALE, 130, Pale_Blue, ELT_O, i);
      DrawBrickLetter(hdc, 20 + i * 16 * GRAPHIC_GLOBAL_SCALE, 160, Blue, ELT_O, i);
   }*/
}
//---------------------------------------------------------------------------------------------------
void RedrawPlatform() 
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left   = (GRAPHIC_LEVEL_X_OFFSET + Platform_X_Pos) * GRAPHIC_GLOBAL_SCALE;
   Platform_Rect.top    = Platform_Y_Pos * GRAPHIC_GLOBAL_SCALE;
   Platform_Rect.right  = Platform_Rect.left + Platform_Width * GRAPHIC_GLOBAL_SCALE;
   Platform_Rect.bottom = Platform_Rect.top  + GRAPHIC_PLATFORM_HEIGHT * GRAPHIC_GLOBAL_SCALE;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//---------------------------------------------------------------------------------------------------
int OnKeyDown(EKeyType key)
{
   switch (key)
   {
   case EKT_Left: {
      Platform_X_Pos -= Platform_X_Step;
      RedrawPlatform();
   } break;

   case EKT_Right: {
      Platform_X_Pos += Platform_X_Step;
      RedrawPlatform();
   } break;

   case EKT_Space: {
   } break;

   default: {
   } break;
   }

   return 0;
}
//---------------------------------------------------------------------------------------------------
void MoveBall()
{
   Prev_Ball_Rect = Ball_Rect;

   Ball_X_Pos += Ball_X_Offset;
   Ball_Y_Pos -= Ball_Y_Offset;

   Ball_Rect.left   = (GRAPHIC_LEVEL_X_OFFSET + Ball_X_Pos) * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.top    = Ball_Y_Pos * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.right  = Ball_Rect.left + GRAPHIC_BALL_SIZE * GRAPHIC_GLOBAL_SCALE;
   Ball_Rect.bottom = Ball_Rect.top  + GRAPHIC_PLATFORM_HEIGHT * GRAPHIC_GLOBAL_SCALE;

   InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(hwnd, &Ball_Rect, FALSE);
}
//---------------------------------------------------------------------------------------------------
int OnTimer()
{
   MoveBall();
   return 0;
}
//---------------------------------------------------------------------------------------------------