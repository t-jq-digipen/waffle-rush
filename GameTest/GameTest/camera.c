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
#include "camera.h"

void playerCamColl(Player *player, Camera* camera) {
	float playerR = player->x + player->width;
	float playerB = player->y + player->height;

	if (playerR > camera->limitrightx) {
		printf("too far right");
		camera->worldshiftx += playerR - camera->limitrightx; //this is the distance between player and border
		camera->limitleftx += playerR - camera->limitrightx;
		camera->limitrightx += playerR -camera->limitrightx;

	}
	else if (player->x < camera->limitleftx) {
		printf("too far left");
		camera->worldshiftx -= camera->limitleftx - player->x ; //this is the distance between player and border
		camera->limitrightx -= camera->limitleftx - player->x;
		camera->limitleftx -= camera->limitleftx - player->x;
	}

	if (player->y < camera->limittop) {
		printf("too far up");
		camera->worldshifty -=  camera->limittop - player->y ; //this is the distance between playery and bordertop
		camera->limitbase -= camera->limittop - player->y;
		camera->limittop -= camera->limittop - player->y;
		//camera->limittop -= camera->limittop - player->y;

	}
	else if (playerB > camera->limitbase) {
		printf("too far down");
		camera->worldshifty += playerB - camera->limitbase ; //this is the distance between player and border
		camera->limittop += playerB - camera->limitbase;
		camera->limitbase += playerB - camera->limitbase;
	}
}



void reset_camera(Camera* camera) {
	camera->limitleftx = 0;
	camera->limitrightx = 0;
	camera->limittop = 0;
	camera->limitbase = 0;
	camera->width = 0;
	camera->height = 0;
	camera->worldshiftx = 0;
	camera->worldshifty = 0;
}

