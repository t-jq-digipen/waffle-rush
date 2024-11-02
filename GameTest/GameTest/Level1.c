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

void L1_Init(void)
{
	// load font
	text = CP_Font_Load("Assets/Exo2-Regular.ttf");

	// load player sprite
	player_sprite = CP_Image_Load("Assets/Waffle_Player.png");

	// get window size
	width = CP_System_GetWindowWidth();
	height = CP_System_GetWindowHeight();

	// initialize colors
	bg = CP_Color_Create(100, 100, 100, 255); //background
	black = CP_Color_Create(0, 0, 0, 255); //black
	red = CP_Color_Create(200, 50, 50, 255); //red
	green = CP_Color_Create(50, 200, 50, 255); //green
	plat = CP_Color_Create(255, 150, 150, 255); //platform colour
	stamina = CP_Color_Create(255, 255, 40, 255); //stamina colour

	// initialize base speed
	speed = 600;

	// initialize movestate
	movestate = OnPlat;

	// initialize deaths
	deaths = 0;

	// initialize goal
	goalx = 1520;
	goaly = 250;

	// initialize player
	// size
	player.width = 20;
	player.height = 20;
	// position
	player.x = spawnx = 150;
	player.y = spawny = 200;
	// movement
	player.movex = 0;
	player.movey = 0;
	// transit variables
	player.transition_time = 1.0f;
	// health
	player.stamina = 100.0f;
	// color
	player.color = CP_Color_Create(150, 150, 200, 255);

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------*/
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
	/*-----------------------------------------------------------------------------------------------------------*/
#pragma region Hazard
	FILE* file2 = fopen("Assets/Level1hazard.txt", "r");
	fscanf_s(file2, "%d", &numberoftraps);

	for (int i = 0; i < numberoftraps; ++i) {
		for (int j = 0; j < 3; ++j) {
			fscanf_s(file2, "%d %d", &traps[i].x[j], &traps[i].y[j]);
		}
		fscanf_s(file2, "%d", &traps[i].type);
	}

	fclose(file2);
#pragma endregion
	// Corner drawing
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_ImageMode(CP_POSITION_CORNER);
}

void L1_Update(void)
{
	// background
	CP_Graphics_ClearBackground(bg);

	// getdt
	float dt = CP_System_GetDt();

	// MOVEMENT 
	/*---------------------------------------------------------------------------------------------------*/
#pragma region Movement
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
	if (CP_Input_KeyTriggered(KEY_R) || ((player.y >= height - player.height) && movestate == OnPlat) || (player.stamina == 0)) {
		player.x = spawnx;
		player.y = spawny;
		movestate = OnPlat;
		player.movey = 0;
		deaths++;
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

	if (AreSquaresIntersecting(player.x - 15,player.y - 15,player.height + 30,player.width + 30,goalx,goaly,50,50)) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Settings_TextSize(150);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("Yippie!", 1300, 50);
	}

	// HAZARD DRAWING
	for (int i = 0; i < numberoftraps; ++i) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Graphics_DrawTriangle(traps[i].x[0], traps[i].y[0], traps[i].x[1], traps[i].y[1], traps[i].x[2], traps[i].y[2]);
	}

	// Player
	CP_Image_Draw(player_sprite, player.x - 15, player.y - 15, player.width + 30, player.height + 30, 255);
	// Stamina Bar
	CP_Settings_Fill(stamina);
	if (player.stamina <= 25.0f) {
		CP_Settings_Fill(red);
	}
	DrawStatBar(player.x - 15, player.y - 25, player.x + 35, player.y - 20, player.stamina, 100);

	// DEATHS DRAWING
	CP_Settings_Fill(black);
	CP_Settings_TextSize(50);
	char deathcount[20] = { 0 };
	if (deaths < 100) {
		sprintf_s(deathcount, 20, "Deaths: %d", deaths);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText(deathcount, 50, 50);
	}
	else {
		CP_Font_DrawText("Bruh", 50, 50);
	}

#pragma endregion

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };
}

void L1_Exit(void)
{

	CP_Image_Free(player_sprite);

}