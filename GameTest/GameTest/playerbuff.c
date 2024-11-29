//---------------------------------------------------------
// file:	credits.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Credits page
//			
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "globals.h"

//int buffstate = 0b000;  //init here, logic here, application in level2.c
//init collision =0;


/*might have to do init for each element here, preferably at buff.h*/
/*collision logic
if // collision 
	buffstate = buffstate | power[i];speed
*/

//checking for collision
int buff_pickup(float x, float y, float height, float width) {
	float playerRight = player.x + player.width;
	float playerBase = player.y + player.height;
	
	if (playerRight > x-10.0f && player.y < y+height+10.0f && player.x < x+10.0f + width && playerBase > y-10.0f) {
		return 1;
	}
	else
		return 0;
}



void Draw_Buffs() {

	float buff_sprite_x = (player.anim < 10) ? 0.0f : 32.0f;
	if (Buff[jumpBuff].canPickup == 1) {
		CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
		//CP_Graphics_DrawRect(Buff[jumpBuff].X-camera.worldshiftx, Buff[jumpBuff].Y-camera.worldshifty, Buff[jumpBuff].width, Buff[jumpBuff].height);
		float jumpbuff_x = Buff[jumpBuff].X - camera.worldshiftx - 20.0f;
		float jumpbuff_y = Buff[jumpBuff].Y - camera.worldshifty - 20.0f;
		CP_Image_DrawSubImage(buff_sprite, jumpbuff_x, jumpbuff_y, Buff[jumpBuff].width + 40.0f, Buff[jumpBuff].height + 40.0f, buff_sprite_x, 0.0f, buff_sprite_x + 32.0f, 32.0f, 255);
	}
	if (Buff[invulBuff].canPickup == 1) {
		CP_Settings_Fill(CP_Color_Create(100, 100, 100, 255));
		//CP_Graphics_DrawRect(Buff[invulBuff].X - camera.worldshiftx, Buff[invulBuff].Y - camera.worldshifty, Buff[invulBuff].width, Buff[invulBuff].height);
		float invbuff_x = Buff[invulBuff].X - camera.worldshiftx - 20.0f;
		float invbuff_y = Buff[invulBuff].Y - camera.worldshifty - 20.0f;
		CP_Image_DrawSubImage(buff_sprite, invbuff_x, invbuff_y, Buff[jumpBuff].width + 40.0f, Buff[jumpBuff].height + 40.0f, buff_sprite_x, 32.0f, buff_sprite_x + 32.0f, 64.0f, 255);
	}


	// Stamina Bar
	if (player.stamina < 100.0f) {
		CP_Settings_Fill(stamina);
		if (player.stamina <= 35.0f) {
			CP_Settings_Fill(red);
	}
		DrawStatBar(player.x - 15 - camera.worldshiftx, player.y - 30 - camera.worldshifty, player.x + 35 - camera.worldshiftx, player.y - 23 - camera.worldshifty, player.stamina, 100);
}

	// Buff indicator
	CP_Settings_ImageMode(CP_POSITION_CENTER);

	CP_Settings_NoStroke();
	CP_Settings_Fill(stamina);

	// jump buff
	if (jumpalpha == 255.0f) {
		DrawStatBar(110.0f, 810.0f, 190.0f, 817.0f, Buff[jumpBuff].buffduration, 3.0f);
	}
	CP_Image_Draw(buff_bg, 150.0f, 750.0f, 100.0f, 100.0f, (int)jumpalpha);
	CP_Image_DrawSubImage(buff_sprite, 150.0f, 750.0f, 75.0f, 75.0f, 0.0f, 0.0f, 32.0f, 32.0f, (int)jumpalpha);


	// shield buff
	if (shieldalpha == 255.0f) {
		DrawStatBar(260.0f, 810.0f, 340.0f, 817.0f, Buff[invulBuff].buffduration, 3.0f);
	}
	CP_Image_Draw(buff_bg, 300.0f, 750.0f, 100.0f, 100.0f, (int)shieldalpha);
	CP_Image_DrawSubImage(buff_sprite, 300.0f, 750.0f, 70.0f, 70.0f, 0.0f, 32.0f, 32.0f, 64.0f, (int)shieldalpha);

	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
}

//jumpheight
#if 0

if (buffstate & jumpBuff){
	player.speed += 100 ; // or should i instead do speed *2? 


}

//speedboost/slowtime
if (/*buffstate & power[i] - box)*/) {

}

//invulnerable
if (/*buffstate & power[i] - box)*/) {

}
#endif