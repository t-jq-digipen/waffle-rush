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
#include "utils.h"

#define num_levels 5
level levels[num_levels];
float timeElapsed;

EnterLevel level1, level2, level3;
int levelEnded;

void MainMenu1_Init(void)
{
	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
// load font
	text = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load player sprite
	player_sprite = CP_Image_Load("Assets/Player_Sprites.png");
	transition_sprite = CP_Image_Load("Assets/Transition.png");
	spawn_sprite = CP_Image_Load("Assets/Spawn_Sprite.png");
	burn_sprite = CP_Image_Load("Assets/Burnt.png");
	fall_sprite = CP_Image_Load("Assets/Fall_Sprite.png");
	crush_sprite = CP_Image_Load("Assets/Crush_Sprite.png");
	goal_sprite = CP_Image_Load("Assets/Plate.png");
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
	fscanf_s(CheckpointFile, "%d", &numcheckpoints);
	for (int checkpoint_index = 0; checkpoint_index < numcheckpoints; checkpoint_index++) {
		int x = 0;
		int y = 0;
		fscanf_s(CheckpointFile, "%d %d", &x, &y);
		checkpoint.coordinates[checkpoint_index] = (coords){ x, y };
	}
	fclose(CheckpointFile);


	// initialize spawn location
	spawn.x = checkpoint.coordinates[0].x - 10;
	spawn.y = checkpoint.coordinates[0].y - 60;
	// initialize goal
	goalx = 1000;
	goaly = 350;

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platform

	reset_platforms();

	FILE* PlatformFile = fopen("Assets/Level_Select/Platform.txt", "r");

	if (PlatformFile == NULL) {
		printf("Failed to open platform file! Check if file exists\n");
		CP_Engine_Terminate();
	}
	// read how many platforms in level
	fscanf_s(PlatformFile, "%d", &numplats);

	// plat_index - platform index
	// plat_coord_index - platform coordinate index
	int plat_index = 0; int plat_coord_index = 0;
	while (plat_index < numplats) {
		// coordinate ints to read
		// scan how many coordinates

		fscanf_s(PlatformFile, "%d", &Platform[plat_index].numcoords);
		// for how many coordinates in platform
		while (plat_coord_index < Platform[plat_index].numcoords) {
			int x, y;
			// scan coordinates
			fscanf_s(PlatformFile, "%d %d", &x, &y);
			// assign coordinates to array
			platform_coordinates(plat_index, plat_coord_index, x, y);
			plat_coord_index++;
		}
		plat_coord_index = 0;
		fscanf_s(PlatformFile, "%d", &Platform[plat_index].numblocks);
		while (plat_coord_index < Platform[plat_index].numblocks) {
			int x1, y1, x2, y2;
			fscanf_s(PlatformFile, "%d %d %d %d", &x1, &y1, &x2, &y2);
			platform_onblocks(plat_index, plat_coord_index, x1, y1, x2, y2);
			plat_coord_index++;
		}
		plat_coord_index = 0;
		plat_index++;
	}
	fclose(PlatformFile);

	// initialize all platform hitboxes
	for (int plat_index = 0; plat_index < numplats; plat_index++) {
		hitbox(plat_index, Platform[plat_index].numcoords, &Platform[plat_index]);
	}


	// Walls
	reset_walls(&walls);
	FILE* WallFile = fopen("Assets/Level_Select/Wall.txt", "r");

	if (WallFile == NULL) {
		printf("Failed to open wall file! Check if file exists\n");
		CP_Engine_Terminate;
	}

	fscanf_s(WallFile, "%d", &numwalls);

	for (int wall_index = 0; wall_index < numwalls; wall_index++) {
		int x1, y1, x2, y2;
		fscanf_s(WallFile, "%d %d %d %d", &x1, &y1, &x2, &y2);
		walls[wall_index] = (platblocks){ x1 - 15, x2 + 15, y1 - 15, y2 + 15, (x2 + 15) - (x1 - 15), (y2 + 15) - (y1 - 15) };
	}

	fclose(WallFile);


#pragma endregion

	//HAZARDS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Hazard
	reset_hazards(&traps);

	FILE* HazardFile = fopen("Assets/Level_Select/Hazard.txt", "r");
	fscanf_s(HazardFile, "%d", &numberoftraps);

	for (int i = 0; i < numberoftraps; ++i)
	{
		fscanf_s(HazardFile, "%d", &traps[i].type);
		switch (traps[i].type)
		{
		case spike:
		{
			int j = 0;
			for (; j < 3; ++j)
			{
				fscanf_s(HazardFile, "%d %d", &traps[i].x[j], &traps[i].y[j]);
			}
			break;
		}
		case crusher:
		{
			int j = 0;
			for (; j < 3; ++j)
			{
				fscanf_s(HazardFile, "%d %d", &traps[i].x[j], &traps[i].y[j]);
			}
			break;
		}
		case shooter:
		{
			int j = 0;
			for (; j < 3; ++j)
			{
				fscanf_s(HazardFile, "%d %d", &traps[i].x[j], &traps[i].y[j]);
			}
			break;
		}
		}
	}
	fclose(HazardFile);
#pragma endregion
#pragma endregion
	// PLAYER INITS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Player related
	Init_Player(&speed, &player, &deaths);
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
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();

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

	camera.spawnX = width * 0.35f; //
	camera.spawnY = height * 0.45f; //
	camera.width = 3.0f / 10.0f * width; //
	camera.height = 0.1f * height; //since base is a bigger number than the top number.
	camera.prespawnX = camera.respawnX = camera.limitleftx = spawn.x - (camera.width / 2.0f);
	camera.limitrightx = camera.limitleftx + camera.width;
	camera.prespawnY = camera.respawnY = camera.limittop = spawn.y - (camera.height * 3.0f / 4.0f);
	camera.limitbase = camera.limittop + camera.height;
	camera.worldshiftx = camera.limitleftx - camera.spawnX;
	camera.worldshifty = camera.limittop - camera.spawnY;
#pragma endregion

#pragma region Level Transition Inits
	//scaler1-4 sets the level transition effect
	levelEnded = start;
	scaler1 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler2 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler3 = (spawn.y + player.height / 2) - camera.worldshifty;
	scaler4 = (spawn.y + player.height / 2) - camera.worldshifty;

	//sets coords to detect when player warps through level select platform
	level1.posx = 400;
	level1.posy = 1000;
	level2.posx = 800;
	level2.posy = 1000;
	level3.posx = 1200;
	level3.posy = 1000;
	level1.height = level2.height = level3.height = 100;
	level1.width = level2.width = level3.width = 200;
#pragma endregion
}

void MainMenu1_Update(void)
{
	//printf("camera.worldshiftx %f | camera.worldshifty %f\n", camera.worldshiftx, camera.worldshifty);
	// background
	CP_Graphics_ClearBackground(bg);

	// getdt
	float dt = CP_System_GetDt();
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
				player.stamina -= 20.0f * dt;
				for (int plat_index = 0; plat_index < numplats; plat_index++) {
					CollPlatforms(plat_index, Platform[plat_index], &player, player);
				}
				if (player.stamina <= 0) {
					Kill_Player(&player, Fall, &deaths);
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
			jump(&player, movestate);

			// window clamps (eventually World Clamps)
			/*player.x = CP_Math_ClampFloat(player.x, 0, width - player.width);
			if (player.y + player.height > height || player.y < 0) {
				player.movey = 0;
			}
			player.y = CP_Math_ClampFloat(player.y, 0, height + player.height);*/

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
			if (CP_Input_KeyTriggered(KEY_R) && !(player.state == Dead || player.state == Spawning) && !isCleared) {
				CauseOfDeath reset = (movestate == InPlat) ? Fall : Burn;
				Kill_Player(&player, reset, &deaths);
			}
			if ((player.y >= 1600) && movestate == OnPlat && player.deathcause != Fall) {
				Kill_Player(&player, Fall, &deaths);
			}
		}

#pragma endregion
		// Hazard
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Hazard stuff
		//declaring top left of square for square triangle collision
		Point squareTopLeft = { player.x, player.y };
		for (int trap_index = 0; trap_index < numberoftraps; trap_index++) {
			switch (traps[trap_index].type) {
			case spike: {
				Point A = { traps[trap_index].x[0], traps[trap_index].y[0] };
				Point B = { traps[trap_index].x[1], traps[trap_index].y[1] };
				Point C = { traps[trap_index].x[2], traps[trap_index].y[2] };

				if (check_square_triangle_collision(squareTopLeft, player.width, player.height, A, B, C)) {
					Kill_Player(&player, Burn, &deaths);
				}
				break;
			}
			case crusher: {
				float crushervely;
				//CRUSHER MOVEMENT
				// crusherstate = 0 if going down and crusherstate = 1 when going up
				if (crusherstate == 0) {
					crushervely = 1000.f * dt;
					traps[trap_index].y[2] += crushervely;
					if (traps[trap_index].y[2] > traps[trap_index].y[1]) {
						traps[trap_index].y[2] = traps[trap_index].y[1];
						crusherstate = 1;
					}
					// if the player hits the crusher at the bottom, add death and reset
					if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[4].x[0], traps[4].y[0], traps[4].x[1] - traps[4].x[0], traps[4].y[2] - traps[4].y[0]) && !(player.state == Dead || player.state == Spawning)) {
						if (crusher_direction(traps[trap_index], player) == 1) {
							if (player.x >= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
								player.x = traps[trap_index].x[1];
								player.movex = 0;
							}
							else if (player.x <= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
								player.x = traps[trap_index].x[0] - player.width;
								player.movex = 0;
							}
						}
						else if (crusher_direction(traps[trap_index], player) == 2) {
							player.y = traps[trap_index].y[2];
							player.movey = 0;
							if ((player.y + player.height + 30) >= traps[trap_index].y[1]) {
								player.y = (traps[trap_index].y[1] - player.height - 15);
								Kill_Player(&player, Crush, &deaths);
								printf("active\n");
							}
						}

					}
				}
				if (crusherstate == 1) {
					crushervely = -0.005f * dt;
					traps[trap_index].y[2] += crushervely;
					if (traps[trap_index].y[2] < traps[trap_index].y[0]) {
						traps[trap_index].y[2] = traps[trap_index].y[0];
						crusherstate = 0;
					}
					//if the player hits the crusher as it goes up, no death but cant go in
					if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[trap_index].x[0], traps[trap_index].y[0], traps[trap_index].x[1] - traps[trap_index].x[0], traps[trap_index].y[2] - traps[trap_index].y[0]) && !(player.state == Dead || player.state == Spawning)) {
						if (crusher_direction(traps[trap_index], player) == 1) {
							if (player.x >= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
								player.x = traps[trap_index].x[1];
								player.movex = 0;
							}
							else if (player.x <= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
								player.x = traps[trap_index].x[0] - player.width;
								player.movex = 0;
							}
						}
						else if (crusher_direction(traps[trap_index], player) == 2) {
							player.y = traps[trap_index].y[2];
							player.movey = 0;
						}

					}
				}
				break;
			}
			default: {}
			}
		}



#pragma endregion

		// CAMERA UPDATE
		/*---------------------------------------------------------------------------------------------------*/
		//implement the below logic later. do the main camera first.
		//camera.limitleftx =  camera.limitleftx  + offsetX;
		//camera.limitrightx = camera.limitrightx + offsetX;
#pragma region Camera
		if (!(player.state == Dead)) {
			playerCamColl(&player, &camera); //functionised my camera shift. 
			//where worldshiftx is now the modifyable for all objects
		}
		CP_Settings_NoFill();
		CP_Settings_RectMode(CP_POSITION_CORNER);
		//CP_Graphics_DrawRect(camera.limitleftx - camera.worldshiftx, camera.limittop - camera.worldshifty, camera.width, camera.height); //code for rectangle
	}

#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region RENDER

	// Platforms
	CP_Settings_Fill(plat);
	CP_Settings_Stroke(CP_Color_Create(100, 100, 100, 255));
	for (int i = 0; i < numplats; i++) {
		DrawPlatform(i);
	}
	CP_Settings_Stroke(black);

	// Goal
	if (!isCleared) {
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, 0, 0, 32, 32, 255);
	}
	else {
		if (clear_timer <= 3) {
			clear_timer += 6 * dt;
		}
		int sprite_x = 32 + (32 * (int)clear_timer);
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, sprite_x, 0, sprite_x + 32, 32, 255);
	}

	if (AreSquaresIntersecting(player.x, player.y, player.height, player.width, goalx, goaly + 70, 100, 30)) {
		//// calculate score and mark the level as cleared
		// AddScore(&levelScore, 10000 + (10000 - ((int)timer.seconds * 50) - ((int)timer.minutes * 3000)) - deaths * 100, &isCleared);
		isCleared = 1;
		// if cleared for the first time, unlock the next level
		//if (levels[levelNumber - 1].isCleared == 0) {
		//	levels[levelNumber - 1].isCleared = 1;
		//	levels[levelNumber].isUnlocked = 1;
		//}
		//// update high score if current score is greater than previous high score
		//if (levelScore > levels[levelNumber - 1].score) {
		//	levels[levelNumber - 1].score = levelScore;
		//}
		//// update best time if current time is less than previous best time
		//if ((levels[levelNumber - 1].time.minutes == 0 && levels[levelNumber - 1].time.seconds == 0 &&
		//	levels[levelNumber - 1].time.millis == 0) ||
		//	(timer.minutes < levels[levelNumber - 1].time.minutes) ||
		//	(timer.minutes == levels[levelNumber - 1].time.minutes && timer.seconds < levels[levelNumber - 1].time.seconds) ||
		//	(timer.minutes == levels[levelNumber - 1].time.minutes && timer.seconds == levels[levelNumber - 1].time.seconds && timer.millis < levels[levelNumber - 1].time.millis)
		//	) {
		//	levels[levelNumber - 1].time.minutes = (int)timer.minutes;
		//	levels[levelNumber - 1].time.seconds = (int)timer.seconds;
		//	levels[levelNumber - 1].time.millis = (int)timer.millis;
		//}
		//// update the values in LevelSelect.txt
		//FILE* LevelFile = fopen("Assets/LevelSelect.txt", "w");
		//for (int i = 0; i < num_levels; i++) {
		//	fprintf_s(LevelFile, "%d %d %d %d %02d:%02d:%02d\n", levels[i].level_no, levels[i].isUnlocked, levels[i].isCleared, levels[i].score,
		//		levels[i].time.minutes, levels[i].time.seconds, levels[i].time.millis);
		//}
		//fclose(LevelFile);
	}

	// Wall
	CP_Settings_Fill(wall);
	for (int i = 0; i < numwalls; i++) {
		CP_Graphics_DrawRect(walls[i].x1 + 15 - camera.worldshiftx, walls[i].y1 + 15 - camera.worldshifty, walls[i].w - 30, walls[i].h - 30);
	}

	// Checkpoint
	for (int checkpoint_index = 0; checkpoint_index < numcheckpoints; checkpoint_index++) {
		if (checkpoint_index != checkpoint.active) {
			CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, 1472, 0, 1536, 64, 255);
		}
		if (checkpoint_index == checkpoint.active) {
			float spawn_sprite_x = (int)player.spawn_time * 64;
			CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, spawn_sprite_x, 0, spawn_sprite_x + 64, 64, 255);
		}
	}

	//Drawing Level Text
	CP_Settings_Fill(black);
	CP_Settings_TextSize(70);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Level 1", level1.posx + level1.width / 2 - camera.worldshiftx, level1.height * 5 - camera.worldshifty);
	CP_Font_DrawText("Level 2", level2.posx + level2.width / 2 - camera.worldshiftx, level2.height * 5 - camera.worldshifty);
	CP_Font_DrawText("Level 3", level3.posx + level3.width / 2 - camera.worldshiftx, level3.height * 5 - camera.worldshifty);

	//this will be replaced with an actual logo ideally
	CP_Settings_TextSize(120);
	CP_Font_DrawText("WAFFLE RUSH LOGO WOOO", -500 - camera.worldshiftx, 300 - camera.worldshifty);
	CP_Settings_TextSize(50);
	CP_Font_DrawText("arrow pointing where player goes ->", -500 - camera.worldshiftx, 400 - camera.worldshifty);

	// Player
	if (isCleared == 0) {
		Draw_Player(player_sprite, player, movestate);
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



	// Stamina Bar
	if (player.stamina < 100.0f && !(player.state == Dead || player.state == Spawning)) {
		CP_Settings_Fill(stamina);
		if (player.stamina <= 35.0f) {
			CP_Settings_Fill(red);
		}
		DrawStatBar(player.x - 15 - camera.worldshiftx, player.y - 30 - camera.worldshifty, player.x + 35 - camera.worldshiftx, player.y - 23 - camera.worldshifty, player.stamina, 100);
	}

	// DEATHS DRAWING
	CP_Settings_Fill(black);
	CP_Settings_TextSize(50);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	char deathcount[20] = { 0 };
	if (deaths < 100) {
		sprintf_s(deathcount, 20, "Deaths: %d", deaths);
		CP_Font_DrawText(deathcount, 50, 50);
	}
	else {
		CP_Font_DrawText("Bruh", 50, 50);
	}

	// TIMER DRAWING
	char time[16] = { 0 };
	sprintf_s(time, 16, "TIME: %02d:%02d:%02d", (int)timer.minutes, (int)timer.seconds, (int)timer.millis);
	CP_Font_DrawText(time, 1200, 50);

	//// LEVEL COMPLETE PAGE
	//if (isCleared == 1) {
	//	CP_Settings_RectMode(CP_POSITION_CENTER);
	//	CP_Settings_Fill(pausebg);
	//	CP_Graphics_DrawRect(width / 2, height / 2, width, height);
	//	CP_Settings_Fill(wall);
	//	CP_Graphics_DrawRect(width / 2, height / 2, 1200, 700);
	//	CP_Settings_Fill(black);
	//	CP_Settings_TextSize(100);
	//	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	//	CP_Font_DrawText("LEVEL COMPLETE", width / 2, 200);

	//	CP_Settings_TextSize(50);
	//	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

	//	// Score labels (on the left side)
	//	CP_Font_DrawText("GOAL BONUS", 300, 300);
	//	CP_Font_DrawText("TIME BONUS", 300, 400);
	//	CP_Font_DrawText("DEATH PENALTIES", 300, 500);
	//	CP_Font_DrawText("TOTAL SCORE", 300, 600);

	//	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);

	//	// Score values (on the right side)
	//	// GOAL SCORE CALCULATION
	//	char scoreText[20] = { 0 };
	//	sprintf_s(scoreText, 20, "%d", IncrementScore(0, 10000, &timeElapsed, dt));
	//	CP_Font_DrawText(scoreText, 1200, 300);

	//	// TIMER DRAWING
	//	char time[20] = { 0 };
	//	sprintf_s(time, 20, "%d", IncrementScore(0, 10000 - (int)timer.seconds * 50 - (int)timer.minutes * 3000, &timeElapsed, dt));
	//	CP_Font_DrawText(time, 1200, 400);

	//	// DEATH DRAWING
	//	char deathcount[20] = { 0 };
	//	sprintf_s(deathcount, 20, "%d", IncrementScore(0, deaths * -100, &timeElapsed, dt));
	//	CP_Font_DrawText(deathcount, 1200, 500);

	//	// SCORE DRAWING
	//	char score[20] = { 0 };
	//	sprintf_s(score, 20, "%d", IncrementScore(0, levelScore, &timeElapsed, dt));
	//	CP_Font_DrawText(score, 1200, 600);

	//	CP_Settings_TextSize(50);
	//	// draw resume button
	//	TextBoxCreate(600, 720, 250, 100, "NEXT LEVEL", red);
	//	int ResumeClick = IsAreaClicked(600, 720, 250, 100, mouseX, mouseY); //if mouse is over button
	//	if (ResumeClick) {
	//		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
	//		CP_Graphics_DrawRect(600, 720, 250, 100);
	//		if (CP_Input_MouseClicked()) { //if LMB1 pressed
	//			CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit);
	//		}
	//	}

	//	// draw quit button
	//	TextBoxCreate(1000, 720, 250, 100, "QUIT", red);
	//	int QuitClick = IsAreaClicked(1000, 720, 250, 100, mouseX, mouseY); //if mouse is over button
	//	if (QuitClick) {
	//		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
	//		CP_Graphics_DrawRect(1000, 720, 250, 100);
	//		if (CP_Input_MouseClicked()) { //if LMB1 pressed
	//			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	//		}
	//	}

	//	CP_Settings_RectMode(CP_POSITION_CORNER);
	//}

	// PAUSE PAGE
	if (isPaused == 1 && isCleared == 0) {
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(pausebg);
		CP_Graphics_DrawRect(width / 2, height / 2, width, height);
		CP_Settings_Fill(wall);
		CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(150);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", width / 2, 300);

		CP_Settings_TextSize(100);
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
	//player lastX & Y store last location of player relative to screen, player is also forced to stop moving while level transition is ongoing to stop it from breaking
	//scaler1,2,3,4 , respectively, control the rects moving (Left,Right,Top,Bottom) L-R, R-L, T-B, B-T
	/*printf("scaler1: %lf | scaler2: %lf | scaler3: %lf | scaler4: %lf\n", scaler1, scaler2, scaler3, scaler4);
	printf("playerlastx: %lf\n", playerLastX);*/
	playerLastX = player.x - camera.worldshiftx;
	playerLastY = player.y - camera.worldshifty;
	//Level Transition
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level1.posx, level1.posy, level1.width, level1.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		if (scaler1 >= playerLastX - 20) {
			printf("swap to level1\n");
		}

		//CP_Engine_SetNextGameState(L1_Init, L1_Update, L1_Exit); //go to game
	}
	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level2.posx, level2.posy, level2.width, level2.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		if (scaler1 >= playerLastX - 20) {
			printf("swap to level2\n");
		}
		//CP_Engine_SetNextGameState(L2_Init, L2_Update, L2_Exit); //go to game
	}

	if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, level3.posx, level3.posy, level3.width, level3.height)) {
		levelEnded = end;
		player.stamina = 100.0f;
		if (scaler1 >= playerLastX - 20) {
			printf("swap to level3\n");
		}
		//CP_Engine_SetNextGameState(L3_Init, L3_Update, L3_Exit); //go to game
	}

	if (CP_Input_KeyTriggered(KEY_P)) {
		levelEnded = end;
	}
	if (CP_Input_KeyDown(KEY_O)) {
		levelEnded = start;
	}

	if (levelEnded == end) {
		canMove = 0;
		scaler1 += (playerLastX + player.width / 2) * dt;
		scaler2 += (width - playerLastX + player.width / 2) * dt;
		scaler3 += (playerLastY + player.height / 2) * dt;
		scaler4 += (height - playerLastY + player.height / 2) * dt;
		TransitionInBoxEffect(playerLastX, playerLastY, 0, width, 0, height, scaler1, scaler2, scaler3, scaler4);

		if (scaler1 >= playerLastX) {
			levelEnded = start;
		}
	}

	if (levelEnded == start) {
		canMove = 1;
		scaler1 -= (playerLastX + player.width / 2) * dt;
		scaler2 -= (width - playerLastX + player.width / 2) * dt;
		scaler3 -= (playerLastY + player.height / 2) * dt;
		scaler4 -= (height - playerLastY + player.height / 2) * dt;
		TransitionInBoxEffect(playerLastX, playerLastY, 0, width, 0, height, scaler1, scaler2, scaler3, scaler4);

		if (scaler1 <= 0 && scaler2 <= 0 && scaler3 <= 0 && scaler4 <= 0) {
			levelEnded = reset;
		}
	}
	if (levelEnded == reset) {
		scaler1 = scaler2 = scaler3 = scaler4 = 0;
	}
#pragma endregion

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void MainMenu1_Exit(void)
{
	CP_Image_Free(&player_sprite);
	CP_Image_Free(&transition_sprite);
	CP_Image_Free(&spawn_sprite);
	CP_Image_Free(&burn_sprite);
	CP_Image_Free(&fall_sprite);
	CP_Image_Free(&crush_sprite);
	CP_Image_Free(&goal_sprite);
}