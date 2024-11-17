//---------------------------------------------------------
// file:	Level1.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Level 1 stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "LevelSelect.h"

#define num_levels 5
level levels[num_levels];
float timeElapsed;

// Options settings ---

// BGM volume bar
float current_bgm_vol, max_bgm_vol, prev_bgm_vol;

// Sound effects volume bar
float current_se_vol, max_se_vol, prev_se_vol;

// icons for volume bars
float icon1_width, icon1_height, icon2_width, icon2_height;
float icon_bgm_x, icon_bgm_y;
float icon_se_x, icon_se_y;

void Tutorial_Init(void)
{
	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
// load font
	text = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load player sprite
	player_sprite		= CP_Image_Load("Assets/Player_Sprites.png");
	transition_sprite	= CP_Image_Load("Assets/Transition.png");
	spawn_sprite		= CP_Image_Load("Assets/Spawn_Sprite.png");
	burn_sprite			= CP_Image_Load("Assets/Burnt.png");
	fall_sprite			= CP_Image_Load("Assets/Fall_Sprite.png");
	crush_sprite		= CP_Image_Load("Assets/Crush_Sprite.png");
	goal_sprite			= CP_Image_Load("Assets/Plate.png");
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
	FILE* CheckpointFile = fopen("Assets/Level_Tutorial/Checkpoint.txt", "r");
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
	goalx = 2400;
	goaly = 700;

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platform

	reset_platforms();

	FILE* PlatformFile = fopen("Assets/Level_Tutorial/Platform.txt", "r");

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
	FILE* WallFile = fopen("Assets/Level_Tutorial/Wall.txt", "r");

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
	for (int i = 0; i < MAX_TRAPS; i++) {
		for (int j = 0; j < 3; j++) {
			traps[i].x[j] = 0;
			traps[i].y[j] = 0;
		}
		traps[i].type = 0;
	}

	FILE* HazardFile = fopen("Assets/Level_Tutorial/Hazard.txt", "r");
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
	// initialize base speed
	speed = 600;

	// initialize player struct
	// size
	player.width = 20;
	player.height = 20;
	// position
	player.x = spawn.x;
	player.y = spawn.y;
	// movement
	player.movex = 0;
	player.movey = 0;
	player.walljump = 0;
	// anim timers
	player.transition_time = 15;
	player.death_time = 17;
	player.spawn_time = 34;
	// health
	player.stamina = 100.0f;
	// anim state
	player.state = Idle;
	player.anim = 0;
	player.direction = OnRight;

	// initialize deaths
	deaths = 0;
#pragma endregion
	// Game states, counters
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region States and counters
	// initialize movestate
	movestate = OnPlat;

	// initialize levelstate
	levelstate = NotPaused;

	// timer
	timer = (Timer){ 0,0,0 };

	// time elapsed (for level complete screen)
	timeElapsed = 0.0f;

	// game pause
	// isPaused = 0;

	// check if level complete
	// isCleared = 0;

	//// initialize level number
	//levelNumber = 1;

	//// score
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
	plat = CP_Color_Create(200, 200, 200, 255); //platform colour
	stamina = CP_Color_Create(255, 255, 40, 255); //stamina colour

	// OPTIONS
	// BGM volume bar
	current_bgm_vol = 0.0f;
	prev_bgm_vol = 0.0f;
	max_bgm_vol = 100.0f;

	// Sound effects volume bar
	current_se_vol = 0.0f;
	prev_se_vol = 0.0f;
	max_se_vol = 100.0f;

	// icons for volume bars
	icon1_width = 10.0f;
	icon1_height = 10.0f;
	icon2_width = 20.0f;
	icon2_height = 70.0f;
	icon_bgm_x = 700.0f;
	icon_bgm_y = 350.0f;
	icon_se_x = 700.0f;
	icon_se_y = 500.0f;

	FILE* OptionsFile = fopen("Assets/Options.txt", "r");
	fscanf_s(OptionsFile, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(OptionsFile);

	// Corner drawing
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_ImageMode(CP_POSITION_CORNER);

	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);

#pragma endregion
	// CAMERA INIT
		/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Camera Init

	camera.limitleftx = camera.spawnX = width * 3.0f / 10.0f; //
	camera.limitrightx = width * 7.0f / 10.0f; //
	camera.limittop = camera.spawnY = height * 0.2f; //
	camera.limitbase = height * 0.8f; //  
	camera.width = camera.limitrightx - camera.limitleftx; //
	camera.height = camera.limitbase - camera.limittop; //since base is a bigger number than the top number.
	camera.worldshiftx = 0;
	camera.worldshifty = 0;

#pragma endregion
	//CP_System_SetFrameRate(30);

}

void Tutorial_Update(void)
{
	// background
	CP_Graphics_ClearBackground(bg);

	// getdt
	float dt = CP_System_GetDt();
	dt = CP_Math_ClampFloat(dt, 0.0f, 0.05f); //prevent excess lagspike clipping

	// get mouse coords
	mouseX = CP_Input_GetMouseX();
	mouseY = CP_Input_GetMouseY();

	// pause/resume game with Esc
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && levelstate == NotPaused) {
		levelstate = Paused;
	}
	else if (CP_Input_KeyTriggered(KEY_ESCAPE) && levelstate == Paused) {
		levelstate = NotPaused;
	}

	// Timer updates
	if (levelstate == NotPaused) {
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
	if (levelstate == NotPaused) {

		// Animation Timer
		player_anim_updates(&player, dt, &movestate, &camera, spawn);
		
		// Movement logics
		if (levelstate != Cleared) {
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
			jump(&player);

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
			if (CP_Input_KeyTriggered(KEY_E) && (movestate == OnPlat || movestate == InPlat) && levelstate != Cleared) {
				start_transition(&player, &movestate);
			}

			if (player.transition_time >= 13) {
				end_transition(&player, &movestate);
			}
			// reset with R or touch ground while movestate OnPlat
			if (CP_Input_KeyTriggered(KEY_R) && !(player.state == Dead || player.state == Spawning) && levelstate != Cleared) {
				CauseOfDeath reset = (movestate == InPlat) ? Fall : Burn;
				Kill_Player(&player, reset, &deaths);
			}
			if ((player.y >= height - player.height) && movestate == OnPlat && player.deathcause != Fall && levelstate != Cleared) {
				Kill_Player(&player, Fall, &deaths);
			}
		}

#pragma endregion
		// Hazard
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Hazard stuff
		////declaring top left of square for square triangle collision
		//Point squareTopLeft = { player.x, player.y };
		//for (int i = 0; i < 4; i++) {
		//	Point A = { traps[i].x[0], traps[i].y[0] };
		//	Point B = { traps[i].x[1], traps[i].y[1] };
		//	Point C = { traps[i].x[2], traps[i].y[2] };

		//	if (check_square_triangle_collision(squareTopLeft, player.width, player.height, A, B, C)) {
		//		Kill_Player(&player, Burn, &deaths);
		//	}
		//}
		////CRUSHER MOVEMENT
		//// crusherstate = 0 if going down and crusherstate = 1 when going up
		//float crushervely;
		//if (crusherstate == 0) {
		//	crushervely = 1000.f * dt;
		//	traps[4].y[2] += crushervely;
		//	if (traps[4].y[2] > 600) {
		//		traps[4].y[2] = 600;
		//		crusherstate = 1;
		//	}
		//	//if the player hits the crusher at the bottom, add death and reset
		//	if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[4].x[0], traps[4].y[0], traps[4].x[1] - traps[4].x[0], traps[4].y[2] - traps[4].y[0])) {
		//		if (crusher_direction(traps[4], player) == 1) {
		//			if (player.x >= ((traps[4].x[1] + traps[4].x[0]) / 2.f)) {
		//				player.x = 300;
		//				player.movex = 0;
		//			}
		//			else if (player.x <= ((traps[4].x[1] + traps[4].x[0]) / 2.f)) {
		//				player.x = 200 - player.width;
		//				player.movex = 0;
		//			}
		//		}
		//		else if (crusher_direction(traps[4], player) == 2) {
		//			player.y = traps[4].y[2];
		//			player.movey = 0;
		//			if ((player.y + player.height) >= 600) {
		//				player.y = (600 - player.height);
		//				Kill_Player(&player, Crush, &deaths);
		//				printf("active\n");
		//			}
		//		}

		//	}
		//}
		//if (crusherstate == 1) {
		//	crushervely = -0.005f * dt;
		//	traps[4].y[2] += crushervely;
		//	//traps[4].y[0] += crushervely;
		//	if (traps[4].y[2] < 450) {
		//		traps[4].y[2] = 450;
		//		crusherstate = 0;
		//	}
		//	//if the player hits the crusher as it goes up, no death but cant go in
		//	if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[4].x[0], traps[4].y[0], traps[4].x[1] - traps[4].x[0], traps[4].y[2] - traps[4].y[0])) {
		//		
		//		if (crusher_direction(traps[4], player) == 1) {
		//			if (player.x >= ((traps[4].x[1] + traps[4].x[0]) / 2.f)) {
		//				player.x = 300;
		//				player.movex = 0;
		//			}
		//			else if (player.x <= ((traps[4].x[1] + traps[4].x[0]) / 2.f)) {
		//				player.x = 200 - player.width;
		//				player.movex = 0;
		//			}
		//		}
		//		else if (crusher_direction(traps[4], player) == 2) {
		//			player.y = traps[4].y[2];
		//			player.movey = 0;
		//		}

		//	}
		//}


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
		CP_Graphics_DrawRect(camera.limitleftx - camera.worldshiftx, camera.limittop - camera.worldshifty, camera.width, camera.height); //code for rectangle
	}
#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------*/
#pragma region RENDER

	// Platforms
		CP_Settings_Fill(plat);
		CP_Settings_Stroke(CP_Color_Create(100, 100, 100, 255));
		for (int i = 0; i < numplats; i++) {
			DrawPlatform(i);
		}
		CP_Settings_Stroke(black);

		// Goal
		if (levelstate != Cleared) {
			CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, 0, 0, 32, 32, 255);
		}
		else {
			if (clear_timer <= 3) {
				clear_timer += 6 * dt;
			}
			int sprite_x = 32 + (32 * (int)clear_timer);
			CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, sprite_x, 0, sprite_x + 32, 32, 255);
		}

		if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, goalx, goaly, 50, 50)) {
			levelstate = Cleared;
		}


		// Wall
		CP_Settings_Fill(black);
		for (int i = 0; i < numwalls; i++) {
			CP_Graphics_DrawRect(walls[i].x1+15-camera.worldshiftx, walls[i].y1+15 - camera.worldshifty, walls[i].w-30, walls[i].h-30);
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

		// Checkpoint
		for (int checkpoint_index = 0; checkpoint_index < numcheckpoints; checkpoint_index++) {
			if (checkpoint_index != checkpoint.active) {
				CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, 1472, 0, 1536, 64, 255);
			}
			if (checkpoint_index == checkpoint.active) {
				float spawn_sprite_x = (int)player.spawn_time * 64;
				CP_Image_DrawSubImage(spawn_sprite, checkpoint.coordinates[checkpoint_index].x - 50 - camera.worldshiftx, checkpoint.coordinates[checkpoint_index].y - 100 - camera.worldshifty, 100, 100, spawn_sprite_x, 0, spawn_sprite_x+64, 64, 255);
			}
		}

		// Player
		if (levelstate != Cleared) {
			Draw_Player(player_sprite, player, movestate);
		}

		// Stamina Bar
		if (player.stamina < 100.0f) {
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

		// LEVEL COMPLETE PAGE
		if (levelstate == Cleared) {
			
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(pausebg);
			CP_Graphics_DrawRect(width / 2, height / 2, width, height);
			CP_Settings_Fill(plat);
			CP_Graphics_DrawRect(width / 2, height / 2, 1000, 450);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(100);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("TUTORIAL COMPLETE", width / 2, 350);


			CP_Settings_TextSize(50);
			// draw resume button
			TextBoxCreate(600, 520, 250, 100, "LEVELS", red);
			int ResumeClick = IsAreaClicked(600, 520, 250, 100, mouseX, mouseY); //if mouse is over button
			if (ResumeClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(600, 520, 250, 100);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit);
				}
			}

			// draw quit button
			TextBoxCreate(1000, 520, 250, 100, "QUIT", red);
			int QuitClick = IsAreaClicked(1000, 520, 250, 100, mouseX, mouseY); //if mouse is over button
			if (QuitClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(1000, 520, 250, 100);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
				}
			}
			CP_Settings_RectMode(CP_POSITION_CORNER);
		}

	// PAUSE PAGE
	if (levelstate == Paused) {
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(pausebg);
		CP_Graphics_DrawRect(width / 2, height / 2, width, height);
		CP_Settings_Fill(plat);
		CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(150);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", width / 2, 300);

		CP_Settings_TextSize(70);
		// draw resume button
		TextBoxCreate(400, 2 * height / 3, 300, 150, "RESUME", red);
		int ResumeClick = IsAreaClicked(400, 2 * height / 3, 300, 150, mouseX, mouseY); //if mouse is over button
		if (ResumeClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
			CP_Graphics_DrawRect(400, 2 * height / 3, 300, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				levelstate = NotPaused;
			}
		}

		// draw options button
		TextBoxCreate(800, 2 * height / 3, 300, 150, "OPTIONS", red);
		int OptionsClick = IsAreaClicked(800, 2 * height / 3, 300, 150, mouseX, mouseY); //if mouse is over button
		if (OptionsClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
			CP_Graphics_DrawRect(800, 2 * height / 3, 300, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				levelstate = InOptions;
				prev_bgm_vol = current_bgm_vol;
				prev_se_vol = current_se_vol;
			}
		}

		// draw quit button
		TextBoxCreate(1200, 2 * height / 3, 300, 150, "QUIT", red);
		int QuitClick = IsAreaClicked(1200, 2 * height / 3, 300, 150, mouseX, mouseY); //if mouse is over button
		if (QuitClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
			CP_Graphics_DrawRect(1200, 2 * height / 3, 300, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				levelstate = InQuit;
			}
		}
		CP_Settings_RectMode(CP_POSITION_CORNER);
	}

	// OPTIONS CLICKED
	else if (levelstate == InOptions) {
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(pausebg);
		CP_Graphics_DrawRect(width / 2, height / 2, width, height);
		CP_Settings_Fill(plat);
		CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(100);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("OPTIONS", width / 2, 200);

		// update BGM volume
		if (current_bgm_vol > max_bgm_vol) {
			current_bgm_vol = max_bgm_vol;
		}
		else if (current_bgm_vol < 0) {
			current_bgm_vol = 0.0f;
		}


		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_TextSize(70);
		CP_Font_DrawText("BGM Volume", 250, icon_bgm_y);
		CP_Font_DrawText("SE Volume", 250, icon_se_y);

		// lower BGM volume button
		TextBoxCreate(icon_bgm_x - 70, icon_bgm_y, 50, 50, "-", red);
		int LowerBGMClick = IsAreaClicked(icon_bgm_x - 70, icon_bgm_y, 50, 50, mouseX, mouseY); //if mouse is over button
		if (LowerBGMClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(icon_bgm_x - 70, icon_bgm_y, 50, 50);
			if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
				current_bgm_vol -= 1;
			}
		}

		// raise BGM volume button
		TextBoxCreate(1350, icon_bgm_y, 50, 50, "+", red);
		int RaiseBGMClick = IsAreaClicked(1350, icon_bgm_y, 50, 50, mouseX, mouseY); //if mouse is over button
		if (RaiseBGMClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(1350, icon_bgm_y, 50, 50);
			if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
				current_bgm_vol += 1;
			}
		}

		// update BGM volume bar
		if (current_bgm_vol > 0) {
			for (float i = 0.0f; i < max_bgm_vol; i += 5.0f) {
				CP_Settings_Fill(red);
				if (i < current_bgm_vol) {
					CP_Graphics_DrawRect(icon_bgm_x + 30.0f * (i / 5.0f), icon_bgm_y, icon2_width, icon2_height);
				}
				else {
					CP_Graphics_DrawRect(icon_bgm_x + 30.0f * (i / 5.0f), icon_bgm_y, icon1_width, icon1_height);
				}
			}
		}
		else {
			for (float i = 0.0f; i < max_bgm_vol; i += 5.0f) {
				CP_Settings_Fill(red);
				CP_Graphics_DrawRect(icon_bgm_x + 30.0f * (i / 5.0f), icon_bgm_y, icon1_width, icon1_height);
			}
		}

		// update SE volume
		if (current_se_vol > max_se_vol) {
			current_se_vol = max_se_vol;
		}
		else if (current_se_vol < 0) {
			current_se_vol = 0.0f;
		}

		// lower SE volume button
		TextBoxCreate(icon_se_x - 70, icon_se_y, 50, 50, "-", red);
		int LowerSEClick = IsAreaClicked(icon_se_x - 70, icon_se_y, 50, 50, mouseX, mouseY); //if mouse is over button
		if (LowerSEClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(icon_se_x - 70, icon_se_y, 50, 50);
			if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
				current_se_vol -= 1;
			}
		}

		// raise SE volume button
		TextBoxCreate(1350, icon_se_y, 50, 50, "+", red);
		int RaiseSEClick = IsAreaClicked(1350, icon_se_y, 50, 50, mouseX, mouseY); //if mouse is over button
		if (RaiseSEClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(1350, icon_se_y, 50, 50);
			if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
				current_se_vol += 1;
			}
		}

		// update SE volume bar
		if (current_se_vol > 0) {
			for (float i = 0.0f; i < max_se_vol; i += 5.0f) {
				CP_Settings_Fill(red);
				if (i < current_se_vol) {
					CP_Graphics_DrawRect(icon_se_x + 30.0f * (i / 5.0f), icon_se_y, icon2_width, icon2_height);
				}
				else {
					CP_Graphics_DrawRect(icon_se_x + 30.0f * (i / 5.0f), icon_se_y, icon1_width, icon1_height);
				}
			}
		}
		else {
			for (float i = 0.0f; i < max_se_vol; i += 5.0f) {
				CP_Settings_Fill(red);
				CP_Graphics_DrawRect(icon_se_x + 30.0f * (i / 5.0f), icon_se_y, icon1_width, icon1_height);
			}
		}

		//save changes button
		TextBoxCreate(3 * width / 8, height - 250, 200, 100, "SAVE", red);
		int SaveClick = IsAreaClicked(3 * width / 8, height - 250, 200, 100, mouseX, mouseY); //if mouse is over button
		if (SaveClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(3 * width / 8, height - 250, 200, 100);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				FILE* OptionsFile = fopen("Assets/Options.txt", "w");
				fprintf_s(OptionsFile, "%lf %lf", current_bgm_vol, current_se_vol);
				printf("Changes saved successfully!\nBGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
				prev_bgm_vol = current_bgm_vol;
				prev_se_vol = current_se_vol;
				fclose(OptionsFile);
			}
		}

		//discard changes and return to main menu button
		TextBoxCreate(5 * width / 8, height - 250, 200, 100, "BACK", red);
		int ExitClick = IsAreaClicked(5 * width / 8, height - 250, 200, 100, mouseX, mouseY); //if mouse is over button
		if (ExitClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(5 * width / 8, height - 250, 200, 100);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				levelstate = Paused;
				current_bgm_vol = prev_bgm_vol;
				current_se_vol = prev_se_vol;
			}
		}

		CP_Settings_RectMode(CP_POSITION_CORNER);
	}

	// QUIT CLICKED
		else if (levelstate == InQuit) {
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(pausebg);
			CP_Graphics_DrawRect(width / 2, height / 2, width, height);
			CP_Settings_Fill(plat);
			CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(100);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("Are you sure you want to quit?", width / 2, 300);
			CP_Font_DrawText("Any progress will be lost!", width / 2, 400);

			CP_Settings_TextSize(100);
			// go back to pause menu
			TextBoxCreate(400, 2 * height / 3, 200, 200, "no", red);
			int GoBackClick = IsAreaClicked(400, 2 * height / 3, 200, 200, mouseX, mouseY); //if mouse is over button
			if (GoBackClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(400, 2 * height / 3, 200, 200);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					levelstate = Paused;
				}
			}
			// quit the level and go back to main menu
			TextBoxCreate(1000, 2 * height / 3, 600, 200, "LET ME OUT", red);
			int GoHomeClick = IsAreaClicked(1000, 2 * height / 3, 600, 200, mouseX, mouseY); //if mouse is over button
			if (GoHomeClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(1000, 2 * height / 3, 600, 200);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
				}
			}

			CP_Settings_RectMode(CP_POSITION_CORNER);
		}
#pragma endregion

	// Back to main menu
	if (CP_Input_KeyTriggered(KEY_Q) && levelstate == NotPaused) {
		levelstate = InQuit;
	}
	else if (CP_Input_KeyTriggered(KEY_Q) && levelstate == InQuit) {
		levelstate = NotPaused;
	}
}

void Tutorial_Exit(void)
{
	CP_Image_Free(&player_sprite);
	CP_Image_Free(&transition_sprite);
	CP_Image_Free(&spawn_sprite);
	CP_Image_Free(&burn_sprite);
	CP_Image_Free(&fall_sprite);
	CP_Image_Free(&crush_sprite);
	CP_Image_Free(&goal_sprite);
}