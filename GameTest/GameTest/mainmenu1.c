//---------------------------------------------------------
// file:	mainmenu1.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Tutorial stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "LevelSelect.h"
#include "globals.h"

EnterLevel Tutorial, level1, level2, level3, level4, level5;

void LSelect_Init(void)
{
	// VOLUME
	// load options file (music volume)
	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	// load music
	music = CP_Sound_LoadMusic("Assets/Levelselectthemeig.wav");
	CP_Sound_PlayAdvanced(music, 0.5f, 1.0f, TRUE, CP_SOUND_GROUP_0);

	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
	// load font
	text				= CP_Font_Load("Assets/Better VCR 6.1.ttf");

	// loads images (all original assets woooo lets goooo)
	Load_Images();
#pragma endregion

	// LEVEL INIT
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region LEVEL
	// Initialize level variables
	FILE* file2 = fopen("Assets/LevelSelect.txt", "r");
	for (int i = 0; i < num_levels; i++) {
		fscanf_s(file2, "%d %d %d %d %02d:%02d:%02d\n", &levels[i].level_no, &levels[i].isUnlocked, &levels[i].isCleared, &levels[i].score,
			&levels[i].time.minutes, &levels[i].time.seconds, &levels[i].time.millis);
	}
	fclose(file2);


	// Initialize checkpoints and first spawn
	reset_checkpoints(&checkpoint);
	FILE* CheckpointFile = fopen("Assets/Level_Select/Checkpoint.txt", "r");

	init_checkpoints(CheckpointFile);

	fclose(CheckpointFile);


	// initialize spawn location
	spawn.x = checkpoint.coordinates[0].x - 10;
	spawn.y = checkpoint.coordinates[0].y - 60;
	// initialize goal
	goalx = 1000;
	goaly = 350;

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platforms and walls

// Platforms
	// resets all platforms to 0
	reset_platforms();

	FILE* PlatformFile = fopen("Assets/Level_Select/Platform.txt", "r");

	if (PlatformFile == NULL) {
		printf("Failed to open platform file! Check if file exists\n");
		CP_Engine_Terminate();
	}

	init_platforms(PlatformFile);

	fclose(PlatformFile);

	// initialize all platform hitboxes
	for (int plat_index = 0; plat_index < numplats; plat_index++) {
		hitbox(plat_index, Platform[plat_index].numcoords, &Platform[plat_index]);
	}


// Walls
	// resets all walls to 0
	reset_walls();

	FILE* WallFile = fopen("Assets/Level_Select/Wall.txt", "r");

	if (WallFile == NULL) {
		printf("Failed to open wall file! Check if file exists\n");
		CP_Engine_Terminate();
	}

	fscanf_s(WallFile, "%d", &numwalls);

	init_walls(WallFile);

	fclose(WallFile);


#pragma endregion

	//HAZARDS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Hazard
	reset_hazards();

#pragma endregion
#pragma endregion
	// PLAYER INITS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Player related
	Init_Player(&speed, &player, &deaths, spawn);
	canMove = 1;
#pragma endregion
	// Game states, counters
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region States and counters
	// initialize movestate
	movestate = OnPlat;

	// timer
	timer = (Timer){ 0,0,0 };

	// time elapsed (for level complete screen)
	timeElapsed = 0.0f;

	// game pause
	isPaused = 0;

	// check if level complete
	isCleared = 0;
	clear_timer = 0;

	// initialize level number
	//levelNumber = 1;

	// score
	//levelScore = 0;

	// transition sprite coordinates
	transit_sprite_x = 0;
	transit_sprite_y = 0;
	player_sprite_x = 0;
	player_sprite_y = 0;

#pragma endregion
	// Misc settings
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Misc inits and settings
	// Window size
	width = (float)CP_System_GetWindowWidth();
	height = (float)CP_System_GetWindowHeight();

	// Initialize colors
	pausebg = CP_Color_Create(0, 0, 0, 100);
	bg = CP_Color_Create(135, 200, 220, 255); //background
	black = CP_Color_Create(0, 0, 0, 255); //black
	red = CP_Color_Create(200, 50, 50, 255); //red
	green = CP_Color_Create(50, 200, 50, 255); //green
	plat = CP_Color_Create(255, 200, 80, 255); //platform colour
	wall = CP_Color_Create(200, 200, 200, 255); //platform colour
	stamina = CP_Color_Create(255, 255, 40, 255); //stamina colour

	// Corner drawing
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_ImageMode(CP_POSITION_CORNER);

	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);

#pragma endregion
	// CAMERA INIT
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Camera Init
	Cam_Init(&camera, width, height, spawn);
#pragma endregion

#pragma region Level Transition Inits
	//scaler1-4 sets the level transition effect
	levelEnded = start;
	scaler1 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler2 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler3 = (spawn.y + player.height / 2) - camera.worldshifty;
	scaler4 = (spawn.y + player.height / 2) - camera.worldshifty;

	//sets coords to detect when player warps through level select platform
	Tutorial.posx = 0;
	Tutorial.posy = 725;
	level1.posx = 400;
	level1.posy = 725;
	level2.posx = 800;
	level2.posy = 725;
	level3.posx = 1200;
	level3.posy = 725;
	level4.posx = 1600;
	level4.posy = 725;
	level5.posx = 2000;
	level5.posy = 725;
	Tutorial.height = level1.height = level2.height = level3.height = level4.height = level5.height = 100;
	Tutorial.width = level1.width = level2.width = level3.width = level4.width = level5.width = 200;

	//data to display for each level
	tutorialDataY = Tutorial.height * 5 - camera.worldshifty + 300;
	level1DataY = level1.height * 5 - camera.worldshifty;
	level2DataY = level2.height * 5 - camera.worldshifty;
	level3DataY = level3.height * 5 - camera.worldshifty;
	level4DataY = level4.height * 5 - camera.worldshifty;
	level5DataY = level5.height * 5 - camera.worldshifty;
#pragma endregion
}

void LSelect_Update(void)
{
	//printf("camshiftx %f | camshifty %f | worldx %f | worldy %f\n", camera.camshiftx, camera.camshifty, camera.worldshiftx, camera.worldshifty);
	// background
	CP_Graphics_ClearBackground(bg);
	// set volumes

	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_bgm_vol / 100.0f);

	// getdt
	dt = CP_System_GetDt();
	dt = CP_Math_ClampFloat(dt, 0.0f, 0.05f); //prevent excess lagspike clipping

	// get mouse coords
	mouseX = CP_Input_GetMouseX();
	mouseY = CP_Input_GetMouseY();

	// pause/resume game with Esc
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && isPaused == 0 && isCleared == 0) {
		isPaused = 1;
	}
	else if (CP_Input_KeyTriggered(KEY_ESCAPE) && isPaused == 1 && isCleared == 0) {
		isPaused = 0;
	}

	// Timer updates
	if (isPaused == 0 && isCleared == 0) {
		timer.seconds += dt;
		if (timer.seconds >= 60) {
			timer.seconds = 0;
			timer.minutes++;
		}
		timer.millis = (int)(timer.seconds * 100) % 100;
	}


	// MOVEMENT 
	/*  Contents:
	- Pause check
		- Animation timer
		- Movement
		- Collision
		- Window clamps
		- Movestate transition
		- Reset
	*/
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Movement

	// Pause check
	if (isPaused == 0 && canMove == 1) {

		// Animation Timer
		player_anim_updates(&player, dt, &movestate, &camera, spawn);

		// Movement logics
		if (isCleared == 0) {
			movement(&player, movestate, speed, dt);
		}

		// Collision against platforms
		if (!(player.state == Dead || player.state == Spawning)) {
			if (movestate == OnPlat && player.deathcause != Fall) {
				player.stamina += 40.0f * dt;
				for (int plat_index = 0; plat_index < numplats; plat_index++) {
					CollOnPlatforms(plat_index, Platform[plat_index], &player, player);
				}
			}

			else if (movestate == InPlat && player.deathcause != Fall) {
				//player.stamina -= 20.0f * dt;
				for (int plat_index = 0; plat_index < numplats; plat_index++) {
					CollPlatforms(plat_index, Platform[plat_index], &player, player);
				}
				if (player.stamina <= 0) {
					Kill_Player(&player, Fall, &deaths, &buffstate);
				}
			}
			player.stamina = CP_Math_ClampFloat(player.stamina, 0, 100);

			// Collision against walls
			CollWalls(walls, numwalls, &player, player, movestate);

			// Update active checkpoint
			for (int cp_index = 0; cp_index < numcheckpoints; cp_index++) {
				if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, checkpoint.coordinates[cp_index].x - 50, checkpoint.coordinates[cp_index].y - 100, 100, 100)) {
					checkpoint.active = cp_index;
					spawn.x = checkpoint.coordinates[cp_index].x - 10;
					spawn.y = checkpoint.coordinates[cp_index].y - 60;
				}
			}

			// Jump
			jump(&player, movestate, Buff[jumpBuff].state);

			// change movestate
				/*
				OnPlat	  - on platform
				InPlat	  - inside platform
				EnterPlat - from OnPlat to InPlat
				ExitPlat  - from InPlat to OnPlat
				*/
			if (CP_Input_KeyTriggered(KEY_E) && (movestate == OnPlat || movestate == InPlat)) {
				start_transition(&player, &movestate);
			}

			if (player.transition_time >= 13) {
				end_transition(&player, &movestate);
			}
			// reset with R or touch ground while movestate OnPlat
			if (CP_Input_KeyTriggered(KEY_K) && !(player.state == Dead || player.state == Spawning) && !isCleared) {
				CauseOfDeath reset = (movestate == InPlat) ? Fall : Burn;
				Kill_Player(&player, reset, &deaths, &buffstate);
			}
			if ((player.y >= 1600) && movestate == OnPlat && player.deathcause != Fall) {
				Kill_Player(&player, Fall, &deaths, &buffstate);
			}
		}

#pragma endregion
		

// CAMERA UPDATE
/*---------------------------------------------------------------------------------------------------*/
#pragma region Camera

		if (!(player.state == Dead)) {
			playerCamColl(&player, &camera); 
		}
	}

#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region RENDER

	//Drawing Level Text
	CP_Settings_Fill(black);

	//this will be replaced with an actual logo ideally
	CP_Settings_TextSize(100);
	CP_Font_DrawText("WAFFLE RUSH WOOOOOOOO", -800.0f - camera.worldshiftx, 300.0f - camera.worldshifty);
	CP_Font_DrawText("-->", -800.0f - camera.worldshiftx, 400.0f - camera.worldshifty);
	CP_Settings_TextSize(50);
	CP_Font_DrawText("WRONG WAY", -1750.0f - camera.worldshiftx, 1500.0f - camera.worldshifty);
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("INSTRUCTIONS:", -600.0f - camera.worldshiftx, 400.0f - camera.worldshifty);
	CP_Font_DrawTextBox("WASD to move\nE to enter a level,\ncrawl all the way down to start!", -600.0f - camera.worldshiftx, 450.0f - camera.worldshifty, 500.0f);

	
	float displaysprite_x = ((int)player.anim % 2 == 0) ? 36.0f : 54.0f;
	CP_Image_DrawSubImage(player_sprite, 20.0f - camera.worldshiftx, 300.0f - camera.worldshifty, 100.0f, 100.0f, displaysprite_x, 18.0f, displaysprite_x+18.0f, 36.0f, 255);



	CP_Settings_TextSize(50.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("TUTORIAL", Tutorial.posx + Tutorial.width / 2.0f - camera.worldshiftx, Tutorial.height * 5.0f - camera.worldshifty);
	CP_Font_DrawText("Level 1", level1.posx + level1.width / 2.0f - camera.worldshiftx, level1.height * 5.0f - camera.worldshifty);
	CP_Font_DrawText("Level 2", level2.posx + level2.width / 2.0f - camera.worldshiftx, level2.height * 5.0f - camera.worldshifty);
	CP_Font_DrawText("Level 3", level3.posx + level3.width / 2.0f - camera.worldshiftx, level3.height * 5.0f - camera.worldshifty);
	CP_Font_DrawText("Level 4", level4.posx + level4.width / 2.0f - camera.worldshiftx, level4.height * 5.0f - camera.worldshifty);
	CP_Font_DrawText("Level 5", level5.posx + level5.width / 2.0f - camera.worldshiftx, level5.height * 5.0f - camera.worldshifty);


	// Text transition (when player is touching the platform before entering each level)
	// display stats when standing on each level platform
	char highScore[20] = { 0 };
	char bestTime[20] = { 0 };
	CP_Settings_TextSize(40);
	// TUTORIAL
	CP_Settings_Fill(black);
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, Tutorial.posx, Tutorial.posy - 30, Tutorial.width, Tutorial.height)) {
		tutorialDataY -= 20.0f;
		if (tutorialDataY < Tutorial.height * 5 - camera.worldshifty + 55) {
			tutorialDataY = Tutorial.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		tutorialDataY += 20.0f;
		if (tutorialDataY > Tutorial.height * 5 - camera.worldshifty + 300) {
			tutorialDataY = Tutorial.height * 5 - camera.worldshifty + 300;
		}
	}
	CP_Font_DrawText("JUST DO IT", Tutorial.posx + Tutorial.width / 2 - camera.worldshiftx, tutorialDataY);

	// LEVEL 1
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level1.posx, level1.posy - 30, level1.width, level1.height)) {
		level1DataY -= 20.0f;
		if (level1DataY < level1.height * 5 - camera.worldshifty + 55) {
			level1DataY = level1.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		level1DataY += 20.0f;
		if (level1DataY > level1.height * 5 - camera.worldshifty + 300) {
			level1DataY = level1.height * 5 - camera.worldshifty + 300;
		}
	}
	sprintf_s(highScore, 20, "HIGH SCORE: %d", levels[0].score);
	CP_Font_DrawText(highScore, level1.posx + level1.width / 2 - camera.worldshiftx, level1DataY);
	sprintf_s(bestTime, 20, "BEST TIME: %02d:%02d:%02d", levels[0].time.minutes, levels[0].time.seconds, levels[0].time.millis);
	CP_Font_DrawText(bestTime, level1.posx + level1.width / 2 - camera.worldshiftx, level1DataY + 40);

	// LEVEL 2
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level2.posx, level2.posy - 30, level2.width, level2.height)) {
		level2DataY -= 20.0f;
		if (level2DataY < level2.height * 5 - camera.worldshifty + 55) {
			level2DataY = level2.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		level2DataY += 20.0f;
		if (level2DataY > level2.height * 5 - camera.worldshifty + 300) {
			level2DataY = level2.height * 5 - camera.worldshifty + 300;
		}
	}
	sprintf_s(highScore, 20, "HIGH SCORE: %d", levels[1].score);
	CP_Font_DrawText(highScore, level2.posx + level2.width / 2 - camera.worldshiftx, level2DataY);
	sprintf_s(bestTime, 20, "BEST TIME: %02d:%02d:%02d", levels[1].time.minutes, levels[1].time.seconds, levels[1].time.millis);
	CP_Font_DrawText(bestTime, level2.posx + level2.width / 2 - camera.worldshiftx, level2DataY + 40);

	// LEVEL 3
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level3.posx, level3.posy - 30, level3.width, level3.height)) {
		level3DataY -= 20.0f;
		if (level3DataY < level3.height * 5 - camera.worldshifty + 55) {
			level3DataY = level3.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		level3DataY += 20.0f;
		if (level3DataY > level3.height * 5 - camera.worldshifty + 300) {
			level3DataY = level3.height * 5 - camera.worldshifty + 300;
		}
	}
	sprintf_s(highScore, 20, "HIGH SCORE: %d", levels[2].score);
	CP_Font_DrawText(highScore, level3.posx + level3.width / 2 - camera.worldshiftx, level3DataY);
	sprintf_s(bestTime, 20, "BEST TIME: %02d:%02d:%02d", levels[2].time.minutes, levels[2].time.seconds, levels[2].time.millis);
	CP_Font_DrawText(bestTime, level3.posx + level3.width / 2 - camera.worldshiftx, level3DataY + 40);

	// LEVEL 4
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level4.posx, level4.posy - 30, level4.width, level4.height)) {
		level4DataY -= 20.0f;
		if (level4DataY < level4.height * 5 - camera.worldshifty + 55) {
			level4DataY = level4.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		level4DataY += 20.0f;
		if (level4DataY > level4.height * 5 - camera.worldshifty + 300) {
			level4DataY = level4.height * 5 - camera.worldshifty + 300;
		}
	}
	sprintf_s(highScore, 20, "HIGH SCORE: %d", levels[3].score);
	CP_Font_DrawText(highScore, level4.posx + level4.width / 2 - camera.worldshiftx, level4DataY);
	sprintf_s(bestTime, 20, "BEST TIME: %02d:%02d:%02d", levels[3].time.minutes, levels[3].time.seconds, levels[3].time.millis);
	CP_Font_DrawText(bestTime, level4.posx + level4.width / 2 - camera.worldshiftx, level4DataY + 40);

	// LEVEL 5
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level5.posx, level5.posy - 30, level5.width, level5.height)) {
		level5DataY -= 20.0f;
		if (level5DataY < level5.height * 5 - camera.worldshifty + 55) {
			level5DataY = level5.height * 5 - camera.worldshifty + 55;
		}
	}
	else {
		level5DataY += 20.0f;
		if (level5DataY > level5.height * 5 - camera.worldshifty + 300) {
			level5DataY = level5.height * 5 - camera.worldshifty + 300;
		}
	}
	sprintf_s(highScore, 20, "HIGH SCORE: %d", levels[4].score);
	CP_Font_DrawText(highScore, level5.posx + level5.width / 2 - camera.worldshiftx, level5DataY);
	sprintf_s(bestTime, 20, "BEST TIME: %02d:%02d:%02d", levels[4].time.minutes, levels[4].time.seconds, levels[4].time.millis);
	CP_Font_DrawText(bestTime, level5.posx + level5.width / 2 - camera.worldshiftx, level5DataY + 40);


	// Platforms
	CP_Settings_Fill(plat);
	CP_Settings_Stroke(CP_Color_Create(100, 100, 100, 255));
	for (int i = 0; i < numplats; i++) {
		DrawPlatform(i);
	}

	// Wall
	CP_Settings_Fill(wall);
	for (int i = 0; i < numwalls; i++) {
		CP_Graphics_DrawRect(walls[i].x1 + 15 - camera.worldshiftx, walls[i].y1 + 15 - camera.worldshifty, walls[i].w - 30, walls[i].h - 30);
	}

	CP_Settings_Stroke(black);

	//Guide for InPlat
#pragma region InPlatGuide
	if (player.y + 15 > Tutorial.posy) {
		float goalpointspritex = (player.anim < 10.0f) ? 0.0f : 32.0f;
		float goalpointspritey = 96.0f;
		if (player.x >= Tutorial.posx && player.x <= Tutorial.posx + Tutorial.width) {
			CP_Image_DrawSubImage(goal_pointer, Tutorial.posx + Tutorial.width / 4 - camera.worldshiftx, Tutorial.posy - camera.worldshifty + Tutorial.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
		if (player.x >= level1.posx && player.x <= level1.posx + level1.width) {
			CP_Image_DrawSubImage(goal_pointer, level1.posx + level1.width / 4 - camera.worldshiftx, level1.posy - camera.worldshifty + level1.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
		if (player.x >= level2.posx && player.x <= level2.posx + level2.width) {
			CP_Image_DrawSubImage(goal_pointer, level2.posx + level2.width / 4 - camera.worldshiftx, level2.posy - camera.worldshifty + level2.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
		if (player.x >= level3.posx && player.x <= level3.posx + level3.width) {
			CP_Image_DrawSubImage(goal_pointer, level3.posx + level3.width / 4 - camera.worldshiftx, level3.posy - camera.worldshifty + level3.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
		if (player.x >= level4.posx && player.x <= level4.posx + level4.width) {
			CP_Image_DrawSubImage(goal_pointer, level4.posx + level4.width / 4 - camera.worldshiftx, level4.posy - camera.worldshifty + level4.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
		if (player.x >= level5.posx && player.x <= level5.posx + level5.width) {
			CP_Image_DrawSubImage(goal_pointer, level5.posx + level5.width / 4 - camera.worldshiftx, level5.posy - camera.worldshifty + level5.height + 50, 100, 100, goalpointspritex, goalpointspritey, goalpointspritex + 32, goalpointspritey + 32, 190);
		}
	}
#pragma endregion


	// Checkpoint
	for (int checkpoint_index = 0; checkpoint_index < numcheckpoints; checkpoint_index++) {
		if (checkpoint_index != checkpoint.active) {
			CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, 1472, 0, 1536, 64, 255);
		}
		if (checkpoint_index == checkpoint.active) {
			float spawn_sprite_x = (int)player.spawn_time * 64.0f;
			CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, spawn_sprite_x, 0, spawn_sprite_x + 64, 64, 255);
		}
	}



	// Player
	if (isCleared == 0) {
		Draw_Player(player, movestate);
	}

	// HAZARD DRAWING
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	for (int i = 0; i < numberoftraps; ++i)
	{
		switch (traps[i].type)
		{
		case spike:
		{
			CP_Graphics_DrawTriangle(traps[i].x[0] - camera.worldshiftx, traps[i].y[0] - camera.worldshifty, traps[i].x[1] - camera.worldshiftx, traps[i].y[1] - camera.worldshifty, traps[i].x[2] - camera.worldshiftx, traps[i].y[2] - camera.worldshifty);
			break;
		}
		case crusher:
		{
			CP_Graphics_DrawRect(traps[i].x[0] - camera.worldshiftx, traps[i].y[0] - camera.worldshifty, traps[i].x[1] - traps[i].x[0], traps[i].y[2] - traps[i].y[0]);
			break;
		}
		case shooter:
		{
			CP_Graphics_DrawRect(traps[i].x[0] - camera.worldshiftx, traps[i].y[0] - camera.worldshifty, traps[i].x[1] - traps[i].x[0], traps[i].y[2] - traps[i].y[0]);
			break;
		}

		}
	}

	// PAUSE PAGE
	if (isPaused == 1 && isCleared == 0) {
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(pausebg);
		CP_Graphics_DrawRect((float)(width / 2), height / 2, width, height);
		CP_Settings_Fill(wall);
		CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(100);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", width / 2, 300);

		CP_Settings_TextSize(70);
		// draw resume button
		TextBoxCreate(500, 2 * height / 3, 350, 200, "RESUME", red);
		int ResumeClick = IsAreaClicked(500, 2 * height / 3, 350, 200, mouseX, mouseY); //if mouse is over button
		if (ResumeClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
			CP_Graphics_DrawRect(500, 2 * height / 3, 350, 200);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				isPaused = 0;
			}
		}

		// draw quit button
		TextBoxCreate(1100, 2 * height / 3, 350, 200, "QUIT", red);
		int QuitClick = IsAreaClicked(1100, 2 * height / 3, 350, 200, mouseX, mouseY); //if mouse is over button
		if (QuitClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
			CP_Graphics_DrawRect(1100, 2 * height / 3, 350, 200);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
		}
		CP_Settings_RectMode(CP_POSITION_CORNER);
	}
#pragma endregion

#pragma region Level Transition
	//Level Transition
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, Tutorial.posx, Tutorial.posy + 270, Tutorial.width, Tutorial.height)) {
		levelEnded = end;
		SetNext(Tutorial_Init, Tutorial_Update, Tutorial_Exit);
	}
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level1.posx, level1.posy + 270, level1.width, level1.height)) {
		levelEnded = end;
		SetNext(L1_Init, L1_Update, L1_Exit);

	}
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level2.posx, level2.posy + 270, level2.width, level2.height)) {
		levelEnded = end;
		SetNext(L2_Init, L2_Update, L2_Exit);
	}

	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level3.posx, level3.posy + 270, level3.width, level3.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		SetNext(L3_Init, L3_Update, L3_Exit);
	}

	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level4.posx, level4.posy + 270, level4.width, level4.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		SetNext(L4_Init, L4_Update, L4_Exit);
	}

	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level5.posx, level5.posy + 270, level5.width, level5.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		SetNext(L5_Init, L5_Update, L5_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_1)) {
		SetNext(L1_Init, L1_Update, L1_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_2)) {
		SetNext(L2_Init, L2_Update, L2_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_3)) {
		SetNext(L3_Init, L3_Update, L3_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_4)) {
		SetNext(L4_Init, L4_Update, L4_Exit);
	}

	if (CP_Input_KeyTriggered(KEY_5)) {
		SetNext(L5_Init, L5_Update, L5_Exit);
	}

	Level_Transition(next_init, next_update, next_exit);

#pragma endregion

}

void LSelect_Exit(void)
{
	// Frees images :D say no to memory leak
	Free_Images();

	CP_Sound_Free(&music);
}