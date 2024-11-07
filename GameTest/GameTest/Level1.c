//---------------------------------------------------------
// file:	Level1.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Level 1 stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "Level1.h"
#include "LevelSelect.h"

#define num_levels 5
level levels[num_levels];
float timeElapsed;

void L1_Init(void)
{
	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
	// load font
	text = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load player sprite
	player_sprite = CP_Image_Load("Assets/Player_Sprites.png");
#pragma endregion

	// time elapsed (for level complete screen)
	timeElapsed = 0.0f;

	// MAP INIT
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region MAP
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
	FILE* file1 = fopen("Assets/Level1hazard.txt", "r");
	fscanf_s(file1, "%d", &numberoftraps);

	for (int i = 0; i < numberoftraps; ++i) {
		for (int j = 0; j < 3; ++j) {
			fscanf_s(file1, "%d %d", &traps[i].x[j], &traps[i].y[j]);
		}
		fscanf_s(file1, "%d", &traps[i].type);
	}

	fclose(file1);
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
	// transit variables
	player.transition_time = 1.0f;
	// health
	player.stamina = 100.0f;
	// color
	player.color = CP_Color_Create(150, 150, 200, 255);

	// initialize deaths
	deaths = 0;
#pragma endregion
	// Game states, counters
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region States and counters
	// initialize movestate
	movestate = OnPlat;

	// timer
	timer = (Timer){ 0,0,0 };

	// game pause
	isPaused = 0;

	// check if level complete
	isCleared = 0;

	// initialize level number
	levelNumber = 1;

	// score
	levelScore = 0;
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

	// get mouse coords
	mouseX = CP_Input_GetMouseX();
	mouseY = CP_Input_GetMouseY();


	// pause/resume game with Esc
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && isPaused == 0) {
		isPaused = 1;

	}
	else if (CP_Input_KeyTriggered(KEY_ESCAPE) && isPaused == 1) {
		isPaused = 0;
	}

	// Timer update
	if (isPaused == 0 && isCleared == 0) {
		timer.seconds += dt;
		if (timer.seconds >= 60) {
			timer.seconds = 0;
			timer.minutes++;
		}
		timer.millis = (int)(timer.seconds * 100) % 100;
	}

	// MOVEMENT 
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Movement
	// pause check
	if (isPaused == 0 && isCleared == 0) {
		// change movestate
		// OnPlat - on platform
		// InPlat - inside platform
		// EnterPlat - from OnPlat to InPlat
		// ExitPlat - from InPlat to OnPlat
		if (CP_Input_KeyTriggered(KEY_E) && (movestate == OnPlat || movestate == InPlat)) {
			movestate = start_transition(&player, movestate);
		}
		// Transiting movestate
		movestate = transition(&player, movestate, dt);

		// movement logics
		movement(&player, movestate, speed, dt);
		printf("movex %f, movey %f\n", player.movex, player.movey);
		// reset checks for jumping and entering
		player.onplatform = 0;
		player.withinx = 0;
		player.leaveplatform = 0;

		// collision against platforms
		if (movestate == OnPlat) {
			player.stamina += 40.0f * dt;
			for (int plat_index = 0; plat_index < numplats; plat_index++) {
				CollOnPlatforms(plat_index, Platform[plat_index], &player, player);
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

		// reset with R or touch ground while movestate OnPlat
<<<<<<< Updated upstream
		if (CP_Input_KeyTriggered(KEY_R) || ((player.y >= height - player.height) && movestate == OnPlat) || (player.stamina == 0)) {
			player.x = spawnx;
			player.y = spawny;
			movestate = OnPlat;
			player.movey = 0;
			deaths++;
=======
		if ((CP_Input_KeyTriggered(KEY_R) || ((player.y >= height - player.height) && movestate == OnPlat) || (player.stamina == 0)) && isCleared == 0) {
			Reset_Player(&player, &movestate, &deaths);
>>>>>>> Stashed changes
		}

		// animation timer update
		player.anim += 10 * dt;
		if (player.anim >= 20) {
			player.anim = 0;
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
	CP_Settings_Fill(green);
	CP_Graphics_DrawRect(goalx, goaly, 50, 50);

	FILE* file2 = fopen("Assets/LevelSelect.txt", "r");
	for (int i = 0; i < num_levels; i++) {
		fscanf_s(file2, "%d %d %d %d %02d:%02d:%02d\n", &levels[i].level_no, &levels[i].isUnlocked, &levels[i].isCleared, &levels[i].score, &levels[i].time.minutes, &levels[i].time.seconds, &levels[i].time.millis);
	}
	fclose(file2);

	if (AreSquaresIntersecting(player.x - 15,player.y - 15,player.height + 30,player.width + 30,goalx,goaly,50,50)) {
		// calculate score and mark the level as cleared
		AddScore(&levelScore, 10000 + (10000 - ((int)timer.seconds * 50) - ((int)timer.minutes * 3000)) - deaths * 100, &isCleared);
		isCleared = 1;
		// if cleared for the first time, unlock the next level
		if (levels[levelNumber-1].isCleared == 0) {
			levels[levelNumber-1].isCleared = 1;
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
		FILE* file2= fopen("Assets/LevelSelect.txt", "w");
		for (int i = 0; i < num_levels; i++) {
			fprintf_s(file2, "%d %d %d %d %02d:%02d:%02d\n", levels[i].level_no, levels[i].isUnlocked, levels[i].isCleared, levels[i].score, levels[i].time.minutes, levels[i].time.seconds, levels[i].time.millis);
		}
		fclose(file2);
	}

	// HAZARD DRAWING
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	for (int i = 0; i < numberoftraps; ++i) {
		CP_Graphics_DrawTriangle(traps[i].x[0], traps[i].y[0], traps[i].x[1], traps[i].y[1], traps[i].x[2], traps[i].y[2]);
	}

	// Player
	Draw_Player(player_sprite, player, &sprite_x, &sprite_y);

	// Stamina Bar
	CP_Settings_Fill(stamina);
	if (player.stamina <= 35.0f) {
		CP_Settings_Fill(red);
	}
	DrawStatBar(player.x - 15, player.y - 30, player.x + 35, player.y - 23, player.stamina, 100);

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
	sprintf_s(score, 20, "HIGH SCORE: %d", levels[levelNumber-1].score);
	CP_Font_DrawText(score, 1200, 50);

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

	// LEVEL COMPLETE PAGE
	if (isCleared == 1) {
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
#pragma endregion

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };
}

void L1_Exit(void)
{

	CP_Image_Free(player_sprite);

}