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

<<<<<<< Updated upstream
int crusherstate = 0;
=======
// Options settings ---

// BGM volume bar
float current_bgm_vol, max_bgm_vol, prev_bgm_vol;

// Sound effects volume bar
float current_se_vol, max_se_vol, prev_se_vol;

// icons for volume bars
float icon1_width, icon1_height, icon2_width, icon2_height;
float icon_bgm_x, icon_bgm_y;
float icon_se_x, icon_se_y;
>>>>>>> Stashed changes

void L1_Init(void)
{
	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
// load font
	text = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load player sprite
	player_sprite = CP_Image_Load("Assets/Player_Sprites.png");
	transition_sprite = CP_Image_Load("Assets/transition.png");
	death_sprite = CP_Image_Load("Assets/Burnt.png");
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

	// initialize spawn location
	spawnx = 150;
	spawny = 200;
	// initialize test goal
	goalx = 1520;
	goaly = 250;

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platform
	FILE* file = fopen("Assets/Level1.txt", "r");

	if (file == NULL) {
		printf("Failed to open platform file! Check if file exists\n");
		CP_Engine_Terminate;
	}
	// read how many platforms in level
	fscanf_s(file, "%d", &numplats);

	// plat_index - platform index
	// plat_coord_index - platform coordinate index
	int plat_index = 0; int plat_coord_index = 0;
	while (plat_index < numplats) {
		// coordinate ints to read
		// scan how many coordinates
		fscanf_s(file, "%d", &Platform[plat_index].numcoords);
		// for how many coordinates in platform
		while (plat_coord_index < Platform[plat_index].numcoords) {
			int x, y;
			// scan coordinates
			fscanf_s(file, "%d %d", &x, &y);
			// assign coordinates to array
			platform_coordinates(plat_index, plat_coord_index, x, y);
			plat_coord_index++;
		}
		plat_coord_index = 0;
		fscanf_s(file, "%d", &Platform[plat_index].numblocks);
		while (plat_coord_index < Platform[plat_index].numblocks) {
			int x1, y1, x2, y2;
			fscanf_s(file, "%d %d %d %d", &x1, &y1, &x2, &y2);
			platform_onblocks(plat_index, plat_coord_index, x1, y1, x2, y2);
			plat_coord_index++;
		}
		plat_coord_index = 0;
		plat_index++;
	}
	fclose(file);

	// initialize all platform hitboxes
	for (int plat_index = 0; plat_index < numplats; plat_index++) {
		hitbox(plat_index, Platform[plat_index].numcoords, &Platform[plat_index]);
	}


#pragma endregion

	//HAZARDS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Hazard
	FILE* HazardFile = fopen("Assets/Level1hazard.txt", "r");
	fscanf_s(HazardFile, "%d", &numberoftraps);

	for (int i = 0; i < numberoftraps; ++i)
	{
		fscanf_s(HazardFile, "%d", &traps[i].type);
		switch (traps[i].type)
		{
		case 0:
		{
			int j = 0;
			for (; j < 3; ++j)
			{
				fscanf_s(HazardFile, "%d %d", &traps[i].x[j], &traps[i].y[j]);
			}
			break;
		}
		case 1:
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
	player.x = spawnx;
	player.y = spawny;
	// movement
	player.movex = 0;
	player.movey = 0;
	// anim timers
	player.transition_time = 15;
	player.death_time = 17;
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

	// initialize level number
	levelNumber = 1;

	// score
	levelScore = 0;

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
	bg = CP_Color_Create(100, 100, 100, 255); //background
	black = CP_Color_Create(0, 0, 0, 255); //black
	red = CP_Color_Create(200, 50, 50, 255); //red
	green = CP_Color_Create(50, 200, 50, 255); //green
	plat = CP_Color_Create(255, 150, 150, 255); //platform colour
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

#pragma endregion

}

void L1_Update(void)
{
	// background
	CP_Graphics_ClearBackground(bg);

	// getdt
	float dt = CP_System_GetDt();
	dt = CP_Math_ClampFloat(dt, 0, 0.05); //prevent excess lagspike clipping

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

<<<<<<< Updated upstream
	// Pause check
	if (isPaused == 0) {
=======
		// Pause check
	if (levelstate == NotPaused) {
>>>>>>> Stashed changes

		// Animation Timer
		player.anim += 10 * dt;
		if (player.anim >= 20) {
			player.anim = 0;
		}
		if (player.transition_time < 13) {
			player.transition_time += 20 * dt;
		}
		if (player.transition_time > 13) {
			player.transition_time = 13;
		}
		if (player.death_time < 17) {
			player.death_time += 17 * dt;
		}
		if (player.death_time >= 17) {
			if (player.state == Dead) {
				Reset_Player(&player, &movestate);
				player.movex = 0;
			}
			player.death_time = 17;
		}

		// Movement logics
<<<<<<< Updated upstream
		movement(&player, movestate, speed, dt);

		// Collision against platforms
		if (movestate == OnPlat) {
			player.stamina += 40.0f * dt;
			for (int plat_index = 0; plat_index < numplats; plat_index++) {
				CollOnPlatforms(plat_index, Platform[plat_index], &player, player);
=======
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
>>>>>>> Stashed changes
			}
		}

		else if (movestate == InPlat) {
			player.stamina -= 20.0f * dt;
			for (int plat_index = 0; plat_index < numplats; plat_index++) {
				CollPlatforms(plat_index, Platform[plat_index], &player, player);
			}
		}
		player.stamina = CP_Math_ClampFloat(player.stamina, 0, 100);

		// window clamps
		player.x = CP_Math_ClampFloat(player.x, 0, width - player.width);
		if (player.y + player.height > height || player.y < 0) {
			player.movey = 0;
		}
		player.y = CP_Math_ClampFloat(player.y, 0, height - player.height);

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
		if ((CP_Input_KeyTriggered(KEY_R) || ((player.stamina == 0) && player.state != Dead)) && isCleared == 0) {
			Kill_Player(&player, &deaths);
		}
		if ((player.y >= height - player.height) && movestate == OnPlat && isCleared == 0) {
			Reset_Player(&player, &movestate);
			deaths++;
		}
		
#pragma endregion
		// Hazard
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Hazard stuff
		if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[4].x[0], traps[4].y[0], traps[4].x[1] - traps[4].x[0], traps[4].y[2] - traps[4].y[0]) && isCleared == 0) {
			Kill_Player(&player, &deaths);
		}

		Point squareTopLeft = { player.x, player.y };
		for (int i = 0; i < 4; i++) {
			Point A = { traps[i].x[0], traps[i].y[0] };
			Point B = { traps[i].x[1], traps[i].y[1] };
			Point C = { traps[i].x[2], traps[i].y[2] };

			if (check_square_triangle_collision(squareTopLeft, player.width, player.height, A, B, C) && isCleared == 0) {
				Kill_Player(&player, &deaths);
			}
		}

#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------*/
#pragma region RENDER

	// Platforms
		CP_Settings_Fill(plat);
		for (int i = 0; i < numplats; i++) {
			DrawPlatform(i);
		}

		// Goal
<<<<<<< Updated upstream
		CP_Settings_Fill(green);
		CP_Graphics_DrawRect(goalx, goaly, 50, 50);

=======
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
>>>>>>> Stashed changes

		if (AreSquaresIntersecting(player.x - 15, player.y - 15, player.height + 30, player.width + 30, goalx, goaly, 50, 50)) {
			// calculate score and mark the level as cleared
			AddScore(&levelScore, 10000 + (10000 - ((int)timer.seconds * 50) - ((int)timer.minutes * 3000)) - deaths * 100, &levelstate);
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
		//CRUSHER MOVEMENT
		// crusherstate = 0 if going down and crusherstate = 1 when going up
		float crushervely;
		if (crusherstate == 0) {
			crushervely = 100.f * dt;
			traps[4].y[2] += crushervely;
			traps[4].y[0] += crushervely;
			if (traps[4].y[2] >= 600) {
				crusherstate = 1;
			}
		}
		if (crusherstate == 1) {
			crushervely = 10.f * dt;
			traps[4].y[2] -= crushervely;
			traps[4].y[0] -= crushervely;
			if (traps[4].y[0] <= 450) {
				crusherstate = 0;
			}
		}

		// HAZARD DRAWING
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		for (int i = 0; i < numberoftraps; ++i)
		{
			switch (traps[i].type)
			{
			case 0:
			{
				CP_Graphics_DrawTriangle(traps[i].x[0], traps[i].y[0], traps[i].x[1], traps[i].y[1], traps[i].x[2], traps[i].y[2]);
				break;
			}
			case 1:
			{
				CP_Graphics_DrawRect(traps[i].x[0], traps[i].y[0], traps[i].x[1] - traps[i].x[0], traps[i].y[2] - traps[i].y[0]);
				break;
			}

			}
		}


		// Player
<<<<<<< Updated upstream
		Draw_Player(player_sprite, player, movestate);
=======
		if (levelstate != Cleared) {
			Draw_Player(player_sprite, player, movestate);
		}
>>>>>>> Stashed changes

		// Stamina Bar
		if (player.stamina < 100.0f) {
			CP_Settings_Fill(stamina);
			if (player.stamina <= 35.0f) {
				CP_Settings_Fill(red);
			}
			DrawStatBar(player.x - 15, player.y - 30, player.x + 35, player.y - 23, player.stamina, 100);
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
		CP_Font_DrawText(time, 1200, 100);

		// SCORE DRAWING
		char score[20] = { 0 };
		sprintf_s(score, 20, "HIGH SCORE: %d", levels[levelNumber - 1].score);
		CP_Font_DrawText(score, 1200, 50);

		// LEVEL COMPLETE PAGE
		if (levelstate == Cleared) {
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(pausebg);
			CP_Graphics_DrawRect(width / 2, height / 2, width, height);
			CP_Settings_Fill(plat);
			CP_Graphics_DrawRect(width / 2, height / 2, 1200, 700);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(100);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("LEVEL COMPLETE", width / 2, 200);

			CP_Settings_TextSize(50);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);

			// Score labels (on the left side)
			CP_Font_DrawText("GOAL BONUS", 300, 300);
			CP_Font_DrawText("TIME BONUS", 300, 400);
			CP_Font_DrawText("DEATH PENALTIES", 300, 500);
			CP_Font_DrawText("TOTAL SCORE", 300, 600);

			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);

			// Score values (on the right side)
			// GOAL SCORE CALCULATION
			char scoreText[20] = { 0 };
			sprintf_s(scoreText, 20, "%d", IncrementScore(0, 10000, &timeElapsed, dt));
			CP_Font_DrawText(scoreText, 1200, 300);

			// TIMER DRAWING
			char time[20] = { 0 };
			sprintf_s(time, 20, "%d", IncrementScore(0, 10000 - (int)timer.seconds * 50 - (int)timer.minutes * 3000, &timeElapsed, dt));
			CP_Font_DrawText(time, 1200, 400);

			// DEATH DRAWING
			char deathcount[20] = { 0 };
			sprintf_s(deathcount, 20, "%d", IncrementScore(0, deaths * -100, &timeElapsed, dt));
			CP_Font_DrawText(deathcount, 1200, 500);

			// SCORE DRAWING
			char score[20] = { 0 };
			sprintf_s(score, 20, "%d", IncrementScore(0, levelScore, &timeElapsed, dt));
			CP_Font_DrawText(score, 1200, 600);

			CP_Settings_TextSize(50);
			// draw resume button
			TextBoxCreate(600, 720, 250, 100, "NEXT LEVEL", red);
			int ResumeClick = IsAreaClicked(600, 720, 250, 100, mouseX, mouseY); //if mouse is over button
			if (ResumeClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(600, 720, 250, 100);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit);
				}
			}

			// draw quit button
			TextBoxCreate(1000, 720, 250, 100, "QUIT", red);
			int QuitClick = IsAreaClicked(1000, 720, 250, 100, mouseX, mouseY); //if mouse is over button
			if (QuitClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(1000, 720, 250, 100);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
				}
			}

			CP_Settings_RectMode(CP_POSITION_CORNER);
		}
<<<<<<< Updated upstream
	}

	// PAUSE PAGE
	if (isPaused == 1 && isCleared == 0) {
		CP_Settings_RectMode(CP_POSITION_CENTER);
		CP_Settings_Fill(pausebg);
		CP_Graphics_DrawRect(width / 2, height / 2, width, height);
		CP_Settings_Fill(plat);
		CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
		CP_Settings_Fill(black);
		CP_Settings_TextSize(150);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("PAUSED", width / 2, 300);
=======
	
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
>>>>>>> Stashed changes

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

void L1_Exit(void)
{
	CP_Image_Free(player_sprite);
	CP_Image_Free(transition_sprite);
	CP_Image_Free(death_sprite);
}