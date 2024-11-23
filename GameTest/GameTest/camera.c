//---------------------------------------------------------
// file:	credits.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Camera code
//			
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "camera.h"

void playerCamColl(Player *player, Camera* camera) {
	float playerR = player->x + player->width;
	float playerB = player->y + player->height;
	if (player->state != Spawning) {
		if (playerR > camera->limitrightx) {
			camera->limitleftx += playerR - camera->limitrightx;
			camera->limitrightx += playerR - camera->limitrightx;

		}
		else if (player->x < camera->limitleftx) {
			camera->limitrightx -= camera->limitleftx - player->x;
			camera->limitleftx -= camera->limitleftx - player->x;
		}
		if (player->y < camera->limittop) {
			camera->limitbase -= camera->limittop - player->y;
			camera->limittop -= camera->limittop - player->y;
		}
		else if (playerB > camera->limitbase) {
			camera->limittop += playerB - camera->limitbase;
			camera->limitbase += playerB - camera->limitbase;
		}
	}
	camera->worldshiftx = camera->limitleftx - camera->spawnX + camera->camshiftx;
	camera->worldshifty = camera->limittop - camera->spawnY + camera->camshifty;
}


void CamShift(Camera* camera, float mouse_x, float mouse_y, float screen_width, float screen_height) {
	// left
	if (IsAreaCornerClicked(0, 0, 0.1f * screen_width, screen_height, mouse_x, mouse_y) && camera->camshiftx > -screen_width*0.6f) {
		camera->camshiftx -= 10;
	}
	// up
	if (IsAreaCornerClicked(0, 0, screen_width, 0.1f * screen_height, mouse_x, mouse_y) && camera->camshifty > -screen_height*0.6f) {
		camera->camshifty -= 10;
	}
	// right
	if (IsAreaCornerClicked(0.9f * screen_width, 0, 0.1f * screen_width, screen_height, mouse_x, mouse_y) && camera->camshiftx < screen_width*0.6f) {
		camera->camshiftx += 10;
	}
	// down
	if (IsAreaCornerClicked(0, 0.9f * screen_height, screen_width, 0.1f * screen_height, mouse_x, mouse_y) && camera->camshifty < screen_height*0.6f) {
		camera->camshifty += 10;
	}

	if (CP_Input_KeyTriggered(KEY_R)) {
		camera->camshiftx = 0;
		camera->camshifty = 0;
	}
}

void CamHighLight(Camera* camera, float mouse_x, float mouse_y, float screen_width, float screen_height) {
	CP_Color highlight = CP_Color_Create(255, 255, 255, 100);
	CP_Settings_Fill(highlight);
	CP_Settings_Stroke(CP_Color_Create(0,0,0,0));
	// left
	if (IsAreaCornerClicked(0, 0, 0.1f * screen_width, screen_height, mouse_x, mouse_y) && camera->camshiftx > -screen_width * 0.6f) {
		//CP_Graphics_DrawRect(0, 0, 0.1f * screen_width, screen_height);
		CP_Graphics_DrawTriangle(50, screen_height / 2, 100, screen_height / 2 - 25, 100, screen_height / 2 + 25);
	}
	// up
	if (IsAreaCornerClicked(0, 0, screen_width, 0.1f * screen_height, mouse_x, mouse_y) && camera->camshifty > -screen_height * 0.6f) {
		CP_Graphics_DrawTriangle(screen_width / 2, 50, screen_width / 2 - 25, 100, screen_width / 2 + 25, 100);
	}
	// right
	if (IsAreaCornerClicked(0.9f * screen_width, 0, 0.1f * screen_width, screen_height, mouse_x, mouse_y) && camera->camshiftx < screen_width * 0.6f) {
		CP_Graphics_DrawTriangle(screen_width - 50, screen_height / 2, screen_width - 100, screen_height / 2 - 25, screen_width - 100, screen_height / 2 + 25);
	}
	// down
	if (IsAreaCornerClicked(0, 0.9f * screen_height, screen_width, 0.1f * screen_height, mouse_x, mouse_y) && camera->camshifty < screen_height * 0.6f) {
		CP_Graphics_DrawTriangle(screen_width / 2, screen_height - 50, screen_width / 2 - 25, screen_height - 100, screen_width / 2 + 25, screen_height - 100);
	}
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
}


void Cam_Init(Camera* camera, float screen_width, float screen_height, coords spawn) {
	camera->camshiftx = camera->camshifty = 0;
	camera->spawnX = screen_width * 0.35f; //
	camera->spawnY = screen_height * 0.45f; //
	camera->width = 3.0f / 10.0f * screen_width; //
	camera->height = 0.1f * screen_height; //since base is a bigger number than the top number.
	camera->prespawnX = camera->respawnX = camera->limitleftx = spawn.x - (camera->width / 2.0f);
	camera->limitrightx = camera->limitleftx + camera->width;
	camera->prespawnY = camera->respawnY = camera->limittop = spawn.y - (camera->height * 3.0f / 4.0f);
	camera->limitbase = camera->limittop + camera->height;
	camera->worldshiftx = camera->limitleftx - camera->spawnX;
	camera->worldshifty = camera->limittop - camera->spawnY;
}