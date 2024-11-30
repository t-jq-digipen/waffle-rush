//---------------------------------------------------------
// file:	utils.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Compilation of utility functions used throughout
//			the assignment
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "globals.h"
#include "LevelSelect.h"

// check if a position is within a rectangle
//
// area_center_x - position x center of rectangle
// area_center_y - position y center of rectangle
// area_width - width of rectangle
// area_height - height of rectangle
// click_x - position x to be checked
// click_y - position y to be checked
//
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y) {
	if (click_x > (area_center_x - area_width / 2) && click_x < (area_center_x + area_width / 2) &&
		click_y >(area_center_y - area_height / 2) && click_y < (area_center_y + area_height / 2)) {
		return 1;
	}
	return 0;
}

int IsAreaCornerClicked(float area_corner_x, float area_corner_y, float area_width, float area_height, float click_x, float click_y) {
	if (click_x > (area_corner_x) && click_x < (area_corner_x + area_width) &&
		click_y >(area_corner_y) && click_y < (area_corner_y + area_height)) {
		return 1;
	}
	return 0;
}

//check if a position is within a circle
//
// circle_center_x - position x center of circle
// circle_center_y - position y center of circle
// diameter - width of rectangle
// click_x - position x to be checked
// click_y - position y to be checked
//
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	if ((click_y - circle_center_y) * (click_y - circle_center_y) + (click_x - circle_center_x) * (click_x - circle_center_x) < (diameter / 2) * (diameter / 2)) {
		return 1;
	}
	return 0;
}

//check if two circles are intersecting
// 
// c1_x, c1_y - circle 1 position x and y
// c2_x, c2_y - circle 2 position x and y
// c1_r, c2_r - radius of circle 1 and 2
//
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	CP_Vector distance = CP_Vector_Set(c2_x - c1_x, c2_y - c1_y);
	if (CP_Vector_DotProduct(distance, distance) < (r1 + r2)*(r1+r2)) {
		return 1;
	}
	return 0;
}

// check if two squares are intersecting
//
// s1_x, s1_y, s1_w, s1_h - square 1 posx, posy, width, height
// s2_x, s2_y, s2_w, s2_h - square 2 posx, posy, width, height
//
int AreSquaresIntersecting(float s1_x, float s1_y, float s1_w, float s1_h, float s2_x, float s2_y, float s2_w, float s2_h) {
	float s1_cx = s1_x + s1_w / 2; // square 1 center x
	float s1_cy = s1_y + s1_h / 2; // square 1 center y
	float s2_cx = s2_x + s2_w / 2; // square 2 center x
	float s2_cy = s2_y + s2_h / 2; // square 2 center y
	float widthtouch = s1_w / 2 + s2_w / 2;
	float heighttouch = s1_h / 2 + s2_h / 2;
	if (fabs(s2_cx - s1_cx) <= widthtouch && (fabs(s2_cy - s1_cy) <= heighttouch)) {
		return 1;
	}
	return 0;
}

// create red box with black text
// 
// pos_x - position x center of box
// pos_y - position y center of box
// size_x - width of box
// size_y - height of box
//
void TextBoxCreate(float pos_x, float pos_y, float size_x, float size_y, const char* text, CP_Color color) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(color);
	CP_Graphics_DrawRect(pos_x, pos_y, size_x, size_y);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(text, pos_x, pos_y);
}

// draw full stat bar
// 
// pos_x1, pos_y1 - position of top left corner
// pos_x2, pos_y2 - position of bottom right corner
// stat - current stat
// max_stat - max stat
//
void DrawStatBar(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float stat, float max_stat) {
	// assign total width, height, and width of health bar
	float width, height, statwidth;
	//width and height of total bar
	width = pos_x2 - pos_x1;
	height = pos_y2 - pos_y1;
	//width of current stat
	statwidth = width * stat / max_stat;
	// draw the rectangle
	CP_Graphics_DrawRect(pos_x1, pos_y1, statwidth, height);
}


int SquareTriangleCollide (float squarex, float squarey, float squarewidth, float squareheight, float tri1x,float tri1y, float tri2x, float tri2y, float tri3x, float tri3y) {
	// is Triangle corner in square?
	if ((IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri1x, tri1y))|| (IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri2x, tri2y))|| (IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri3x, tri3y))) {
		return 1;
	}


	return 0;
}

void AddScore(int *totalScore, int addScore, LevelState *levelstate) {
	if (*levelstate != Cleared) {
		*totalScore += addScore;
	}
}

int IncrementScore(int startScore, int endScore, float *timeElapsed, float dt) {
	*timeElapsed += dt;
	float duration = 5.0f;
	int score = 0;
	if (*timeElapsed < duration) {
		startScore = (int)((*timeElapsed / duration) * endScore);
	}
	else {
		startScore = endScore;
	}
	score = startScore;
	return score;
}

void TransitionInBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4) {
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);

	CP_Graphics_DrawRect(boxX1, boxY1, scaler1, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX2, boxY1, -scaler2, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX1, boxY1, boxX2 - boxX1, scaler3);
	CP_Graphics_DrawRect(boxX1, boxY2, boxX2 - boxX1, -scaler4);
}

void TransitionOutBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4) {
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);

	CP_Graphics_DrawRect(boxX1, boxY1, -scaler1, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX2, boxY1, scaler2, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX1, boxY1, boxX2 - boxX1, -scaler3);
	CP_Graphics_DrawRect(boxX1, boxY2, boxX2 - boxX1, scaler4);
}

void Load_Images() {
	// player sprites
	player_sprite		= CP_Image_Load("Assets/Player_Sprites.png");	// player
	boot_sprite			= CP_Image_Load("Assets/Boot_Sprites.png");		// jump buff overlay
	bubble_sprite		= CP_Image_Load("Assets/Bubble.png");			// shield overlay
	transition_sprite	= CP_Image_Load("Assets/Transition.png");		// transition animation
	spawn_sprite		= CP_Image_Load("Assets/Spawn_Sprite.png");		// checkpoint and spawning animation
	burn_sprite			= CP_Image_Load("Assets/Burnt.png");			// burn death animation
	fall_sprite			= CP_Image_Load("Assets/Fall_Sprite.png");		// fall death animation
	crush_sprite		= CP_Image_Load("Assets/Crush_Sprite.png");		// crusher death animation

	// goal stuff
	goal_sprite			= CP_Image_Load("Assets/Plate.png");			// plate goal
	goal_pointer		= CP_Image_Load("Assets/Arrow.png");			// goal pointer green arrow

	// skull
	skull				= CP_Image_Load("Assets/Skull.png");			// skull icon for death counter

	// buffs stuff
	buff_sprite			= CP_Image_Load("Assets/Buff_Icon.png");		// buff icons
	buff_bg				= CP_Image_Load("Assets/Buff_Background.png");	// background for buff UI

	// hazard stuff
	fire_sprite			= CP_Image_Load("Assets/Fire_Sprite.png");		// fire animation
}

void Free_Images() {
	CP_Image_Free(&player_sprite);
	CP_Image_Free(&boot_sprite);
	CP_Image_Free(&bubble_sprite);
	CP_Image_Free(&transition_sprite);
	CP_Image_Free(&spawn_sprite);
	CP_Image_Free(&burn_sprite);
	CP_Image_Free(&fall_sprite);
	CP_Image_Free(&crush_sprite);

	CP_Image_Free(&goal_sprite);
	CP_Image_Free(&goal_pointer);

	CP_Image_Free(&skull);

	CP_Image_Free(&buff_sprite);
	CP_Image_Free(&buff_bg);

	CP_Image_Free(&fire_sprite);
}

void AudioPitch() {
	if (levelstate == Paused || levelstate == Cleared) {
		CP_Sound_SetGroupPitch(CP_SOUND_GROUP_0, 0.85);
	}
	else
	{
		CP_Sound_SetGroupPitch(CP_SOUND_GROUP_0, 1);
	}
}

// LEVEL TRANSITION EFFECTS
void Level_Transition(FunctionPtr level_init, FunctionPtr level_update, FunctionPtr level_exit) {

	if (CP_Input_KeyTriggered(KEY_P)) {
		levelEnded = end;
	}
	if (CP_Input_KeyDown(KEY_O)) {
		levelEnded = start;
	}

	playerLastX = player.x - camera.worldshiftx;
	playerLastY = player.y - camera.worldshifty;

	if (levelEnded == end) {
		canMove = 0;
		scaler1 += (playerLastX + player.width / 2.0f) * dt;
		scaler2 += (width - playerLastX + player.width / 2.0f) * dt;
		scaler3 += (playerLastY + player.height / 2.0f) * dt;
		scaler4 += (height - playerLastY + player.height / 2.0f) * dt;
		TransitionInBoxEffect(playerLastX, playerLastY, 0.0f, width, 0.0f, height, scaler1, scaler2, scaler3, scaler4);

		if (scaler1 >= playerLastX) {
			levelEnded = start;
			CP_Engine_SetNextGameState(level_init, level_update, level_exit);
		}
	}

	if (levelEnded == start) {
		canMove = 1;
		scaler1 -= (playerLastX + player.width / 2) * dt;
		scaler2 -= (width - playerLastX + player.width / 2) * dt;
		scaler3 -= (playerLastY + player.height / 2) * dt;
		scaler4 -= (height - playerLastY + player.height / 2) * dt;
		TransitionInBoxEffect(playerLastX, playerLastY, 0.0f, width, 0.0f, height, scaler1, scaler2, scaler3, scaler4);

		if (scaler1 <= 0 && scaler2 <= 0 && scaler3 <= 0 && scaler4 <= 0) {
			levelEnded = reset;
		}
	}
	if (levelEnded == reset) {
		scaler1 = scaler2 = scaler3 = scaler4 = 0;
	}
}

void SetNext(FunctionPtr level_init, FunctionPtr level_update, FunctionPtr level_exit) {
	next_init = level_init;
	next_update = level_update;
	next_exit = level_exit;

	levelEnded = end;
	levelstate = EndLevel;
}

void RENDER_ALL() {
	// Platforms
	CP_Settings_Fill(plat);
	CP_Settings_Stroke(CP_Color_Create(100, 100, 100, 255));
	for (int i = 0; i < numplats; i++) {
		DrawPlatform(i);
	}

	// Wall
	CP_Settings_Fill(wall);
	for (int i = 0; i < numwalls; i++) {
		CP_Graphics_DrawRect(walls[i].x1 + 15.0f - camera.worldshiftx, walls[i].y1 + 15.0f - camera.worldshifty, walls[i].w - 30.0f, walls[i].h - 30.0f);
	}
	CP_Settings_Stroke(black);

	// Goal
	if (levelstate != Cleared) {
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100.0f, 100.0f, 0.0f, 0.0f, 32.0f, 32.0f, 255);
	}
	else {
		if (clear_timer <= 5.0f) {
			clear_timer += 6.0f * dt;
		}
		float sprite_x = ((int)clear_timer <= 3) ? 32.0f + (float)(32 * (int)clear_timer) : 32.0f * 4.0f;
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100.0f, 100.0f, sprite_x, 0.0f, sprite_x + 32.0f, 32.0f, 255);
	}

	if (AreSquaresIntersecting(player.x, player.y, player.height, player.width, goalx, goaly + 70.0f, 100.0f, 30.0f)) {
		// calculate score and mark the level as cleared
		AddScore(&levelScore, 10000 + (10000 - ((int)timer.seconds * 50) - ((int)timer.minutes * 3000)) - deaths * 100, &levelstate);
		if (levelstate != EndLevel) {
			levelstate = Cleared;
			// if cleared for the first time, unlock the next level
			if (levels[levelNumber - 1].isCleared == 0) {
				levels[levelNumber - 1].isCleared = 1;
				levels[levelNumber].isUnlocked = 1;
			}
			// update high score if current score is greater than previous high score
			if (levelScore > levels[levelNumber - 1].score) {
				levels[levelNumber - 1].score = levelScore;
			}
			// update best time if current time is less than previous best time
			if ((levels[levelNumber - 1].time.minutes == 0 && levels[levelNumber - 1].time.seconds == 0 &&
				levels[levelNumber - 1].time.millis == 0) ||
				(timer.minutes < levels[levelNumber - 1].time.minutes) ||
				(timer.minutes == levels[levelNumber - 1].time.minutes && timer.seconds < levels[levelNumber - 1].time.seconds) ||
				(timer.minutes == levels[levelNumber - 1].time.minutes && timer.seconds == levels[levelNumber - 1].time.seconds && timer.millis < levels[levelNumber - 1].time.millis)
				) {
				levels[levelNumber - 1].time.minutes = (int)timer.minutes;
				levels[levelNumber - 1].time.seconds = (int)timer.seconds;
				levels[levelNumber - 1].time.millis = (int)timer.millis;
			}
			// update the values in LevelSelect.txt
			FILE* LevelFile = fopen("Assets/LevelSelect.txt", "w");
			for (int i = 0; i < num_levels; i++) {
				fprintf_s(LevelFile, "%d %d %d %d %02d:%02d:%02d\n", levels[i].level_no, levels[i].isUnlocked, levels[i].isCleared, levels[i].score,
					levels[i].time.minutes, levels[i].time.seconds, levels[i].time.millis);
			}
			fclose(LevelFile);
		}
	}


	// Checkpoint
		for (int checkpoint_index = 0; checkpoint_index < numcheckpoints; checkpoint_index++) {
			if (checkpoint_index != checkpoint.active) {
				CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50.0f - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, 1472, 0, 1536, 64, 255);
			}
			if (checkpoint_index == checkpoint.active) {
				float spawn_sprite_x = (int)player.spawn_time * 64.0f;
				CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50.0f - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, spawn_sprite_x, 0, spawn_sprite_x + 64, 64, 255);
			}
		}

		// HAZARD DRAWING

		Draw_Hazards();

	// Buffs
	Draw_Buffs();

	// Player
	if (levelstate != Cleared) {
		Draw_Player(player, movestate);
	}

	// Stamina Bar
	if (player.stamina < 100.0f && !(player.state == Dead || player.state == Spawning)) {
		CP_Settings_Fill(stamina);
		if (player.stamina <= 35.0f) {
			CP_Settings_Fill(red);
		}
		DrawStatBar(player.x - 15 - camera.worldshiftx, player.y - 30 - camera.worldshifty, player.x + 35 - camera.worldshiftx, player.y - 23 - camera.worldshifty, player.stamina, 100);
	}


	// Guide arrow that points to goal
	Guide_Arrow();

	// Camera Shift Highlight
	CamHighLight(&camera, mouseX, mouseY, width, height);

	// DEATHS DRAWING
	CP_Settings_Fill(black);
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Image_Draw(skull, 50, 25, 50, 50, 255);
	char deathcount[20] = { 0 };
	if (deaths < 100) {
		sprintf_s(deathcount, 20, ": %d", deaths);
		CP_Font_DrawText(deathcount, 100, 50);
	}
	else {
		CP_Font_DrawText("Bruh", 50, 50);
	}

	// TIMER DRAWING
	char time[16] = { 0 };
	sprintf_s(time, 16, "TIME: %02d:%02d:%02d", (int)timer.minutes, (int)timer.seconds, (int)timer.millis);
	CP_Font_DrawText(time, 1200, 50);
}