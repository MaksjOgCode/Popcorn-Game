#include "Engine.hpp"

HBRUSH Brush_PaleBlue,  Brush_Blue, Brush_WoodyBlue;
HPEN   Brick_Pen_White_Outline, Platform_Pen_White_Outline;

enum EBrickType {
   Pale_Blue   = 101,   /* RGB - (193, 217, 249); */
   Blue        = 102,   /* RGB - (90, 137, 185);  */
   Woody_Blue  = 103    /* RGB - (46, 69, 83);    */
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
{  /* Setting up the game at startup : */

   Brush_PaleBlue  = CreateSolidBrush(RGB(193, 217, 249));
   Brush_Blue      = CreateSolidBrush(RGB(90,  137, 185));
   Brush_WoodyBlue = CreateSolidBrush(RGB(46,  69,  83));

   Brick_Pen_White_Outline    = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
   Platform_Pen_White_Outline = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
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
void DrawBrickLetter(HDC hdc, int coord_x_pos, int coord_y_pos, int rotation_step)
{  /* The function drawning animation on fall brick : */

	double offset;
	double rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;  /* Converting a step to a rotation angle */
	int brick_half_height = GRAPHIC_BRICK_HEIGHT * GRAPHIC_GLOBAL_SCALE / 2;
	int back_part_offset;
	XFORM xform, old_xform;

	if (rotation_step == 4 || rotation_step == 12)
	{
      /* Brick background */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, Brush_WoodyBlue);

		Rectangle(hdc, coord_x_pos,
                     coord_y_pos + brick_half_height - GRAPHIC_GLOBAL_SCALE,
                     coord_x_pos + GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                     coord_y_pos + brick_half_height);

      /* The front of the brick : */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, Brush_PaleBlue);

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
		SelectObject(hdc, Brush_WoodyBlue);

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)GRAPHIC_GLOBAL_SCALE;
		back_part_offset = (int)round(offset);
		Rectangle(hdc, 0, -brick_half_height - back_part_offset,
                        GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                        brick_half_height - back_part_offset);

      /* The front of the brick : */
		SelectObject(hdc, Brick_Pen_White_Outline);
		SelectObject(hdc, Brush_PaleBlue);

		Rectangle(hdc, 0, -brick_half_height,
                        GRAPHIC_BRICK_WIDTH * GRAPHIC_GLOBAL_SCALE,
                        brick_half_height);

		SetWorldTransform(hdc, &old_xform);
   }

}
//---------------------------------------------------------------------------------------------------
void DrawPlatform(HDC hdc, int coord_x_pos, int coord_y_pos)
{  /* The function of the drawning game object: Platform */

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
void DrawLevel(HDC hdc)
{  /* The function of drawning the level of game : */

   for (int i = 0; i < 14; ++i) {
      for (int j = 0; j < 12; ++j) {
         DrawBrick(  hdc, (EBrickType)Level_01[i][j],
            GRAPHIC_LEVEL_X_OFFSET + j * (GRAPHIC_BRICK_WIDTH + 1),
            GRAPHIC_LEVEL_Y_OFFSET + i * (GRAPHIC_BRICK_HEIGHT + 1)  );
      }
   }

   DrawPlatform(hdc, 80, 190);

}
//---------------------------------------------------------------------------------------------------
void DrawFrame(HDC hdc)
{  /* The function of rendering the game frame :   */

   //DrawLevel(hdc);

   for(int i = 0; i < 16; ++i)
      DrawBrickLetter(hdc, 20 + i * 16 * GRAPHIC_GLOBAL_SCALE, 100, i);
}
//---------------------------------------------------------------------------------------------------