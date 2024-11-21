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
#include "playerbuff.h"
#include "platform.h"

//int buffstate = 0b000;  //init here, logic here, application in level2.c
//init collision =0;


/*might have to do init for each element here, preferably at buff.h*/
/*collision logic
if // collision 
	buffstate = buffstate | power[i];speed
*/

//checking for collision
int is_colliding(float x, float y, float height, float width) {
	float playerRight = player.x + player.width;
	float playerBase = player.y + player.height;
	
	if (playerRight > x && player.y < y+height && player.x < x + width && playerBase > y) {
		return 1;
	}
	else
		return 0;
}


