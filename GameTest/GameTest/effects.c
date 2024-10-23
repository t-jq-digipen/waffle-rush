//---------------------------------------------------------
// file:	effects.c
// 
// brief:   When the player dies, kills an enemy, finishes a level, jumps,
//			it creates a animation or effect that represents these changes.
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "platform.h"

void movement_left_vfx(Player* p) {
	float distance_travelled = 150;
	float x = p->x + p->width;
	float y = p->y + p->height;
	float speed = distance_travelled * CP_System_GetDt();
	CP_Color grey = CP_Color_Create(128, 128, 128, 255);
	CP_Settings_Fill(grey);
	CP_Graphics_DrawLine(x, y, p->movex - 20 * speed, y - 5);
}

void movement_right_vfx(Player* p) {
	float distance_travelled = 150;
	float x = p->x + p->width;
	float y = p->y + p->height;
	float speed = distance_travelled * CP_System_GetDt();
	CP_Color grey = CP_Color_Create(128, 128, 128, 255);
	CP_Settings_Fill(grey);
	CP_Graphics_DrawLine(x, y, p->movex + 20 * speed, y - 5);
}