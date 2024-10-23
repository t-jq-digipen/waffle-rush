//---------------------------------------------------------
// file:	game.c
// author:	TAN JUN QUAN
// email:	t.junquan@digipen.edu
//
// brief:   When the player goes into red or green circles,
//			they will lose and gain health respectively
//			Render health bar on top, with segmented health
//			bar every 10%
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "Level1.h"
#include "hazard.h"

CP_Color bg, red, green, hpcol; //colors
float speed; //speed
float width, height; //window width/height
float hpx1, hpx2, hpy1, hpy2; //positions of hp bar
CP_Font text;
CP_Image player_sprite;
hazard traps[100];
int numberoftraps;

int numplats1 = 4;

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
	red = CP_Color_Create(200, 50, 50, 255); //red
	green = CP_Color_Create(50, 200, 50, 255); //green
	hpcol = CP_Color_Create(255, 150, 150, 255); //hp colour

	// initialize base speed
	speed = 400;

	// initialize movestate
	movestate = OnPlat;

	// initialize switchtime
	switchtime = 1;

	// initialize player
	// size
	player.width = 20;
	player.height = 20;
	// position
	player.x = 150;
	player.y = 200;
	// movement
	player.movex = 0;
	player.movey = 0;
	// health
	player.hp = 100;
	// color
	player.color = CP_Color_Create(150, 150, 200, 255);

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platform

	FILE* file = fopen("Assets/Level1.txt", "r");
	// pindex - platform index
	// pcindex - platform coordinate index
	int pindex = 0; int pcindex = 0;
	while (pindex < numplats1) {
		// coordinate ints to read
		int x, y;
		// scan how many coordinates
		fscanf_s(file, "%d", &Platform[pindex].numcoords);
		// for how many coordinates in platform
		while (pcindex < Platform[pindex].numcoords) {
			// scan coordinates
			fscanf_s(file, "%d %d", &x, &y);
			// assign coordinates to array
			platform_coordinates(pindex, pcindex, x, y);
			pcindex++;
		}
		pcindex = 0;
		pindex++;
	}
	fclose(file);

	// initialize all platform hitboxes
	for (int plat_index = 0; plat_index < numplats1; plat_index++) {
		hitbox(plat_index, Platform[plat_index].numcoords, &Platform[plat_index]);
	}

#pragma endregion

	//HAZARDS
	/*-----------------------------------------------------------------------------------------------------------*/
#pragma region Hazard
	FILE* file2 = fopen("Assets/Level1hazard.txt", "r");
	fscanf_s(file2, "%d", &numberoftraps);

	for (int i = 0; i < numberoftraps; ++i) {
		int j=0;
		for (; j < 3; ++j) {
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
		if (movestate == EnterPlat || movestate == ExitPlat) {
			switchtime = 0;
		}
	}
	// Transition animation
	if (switchtime < 1) {
		if ((movestate == EnterPlat && switchtime < 0.5f) || (movestate == ExitPlat && switchtime >= 0.5f)) {
			player.movey += 800.0f / 60.0f;
		}
		if ((movestate == ExitPlat && switchtime < 0.5f) || (movestate == EnterPlat && switchtime >= 0.5f)) {
			player.movey -= 800.0f / 60.0f;
		}
		switchtime += 1.0f / 30.0f;
	}
	if (switchtime >= 1) {
		if (movestate == EnterPlat) {
			movestate = InPlat;
		}
		if (movestate == ExitPlat) {
			movestate = OnPlat;
		}
	}

	// different movement logics
	movement(&player, movestate, speed);

	// position update
	player.x += player.movex * dt;
	player.y += player.movey * dt;

	// reset checks for jumping and entering
	player.onplatform = 0;
	player.withinx = 0;
	player.leaveplatform = 0;
	// collision against platforms
	if (movestate == InPlat || movestate == OnPlat) {
		for (int plat_index = 0; plat_index < numplats1; plat_index++) {
			CollPlatforms(plat_index, Platform[plat_index].numcoords, Platform[plat_index], &player, player);
		}
	}
	// window clamps
	player.x = CP_Math_ClampFloat(player.x, 0, width - player.width);
	if (player.y + player.height > height || player.y < 0) {
		player.movey = 0;
	}
	player.y = CP_Math_ClampFloat(player.y, 0, height - player.height);

	// reset with R or touch ground while movestate OnPlat
	if (CP_Input_KeyDown(KEY_R)|| ((player.y >= height - player.height) && movestate == OnPlat)) {
		player.x = 150;
		player.y = 200;
		movestate = OnPlat;
		player.movey = 0;
	}
	
#pragma endregion

	// HP
	/*---------------------------------------------------------------------------------------------------*/
#pragma region HP

	// hp clamp
	if (player.hp < 0) { player.hp = 0; };
	if (player.hp > 100) { player.hp = 100; };
#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------*/
#pragma region RENDER

	// Platforms
	CP_Settings_Fill(hpcol);
	for (int i = 0; i < numplats1; i++) {
		DrawPlatform(i);
	}

	// Goal
	CP_Settings_Fill(green);
	CP_Graphics_DrawRect(1520, 250, 50, 50);

	if (AreSquaresIntersecting(player.x,player.y,player.height,player.width,1520,250,50,50)) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Font_DrawText("Yippie!", 1300, 50);
	}

	// HAZARD DRAWING
	for (int i = 0; i < numberoftraps; ++i) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
		CP_Graphics_DrawTriangle(traps[i].x[0], traps[i].y[0], traps[i].x[1], traps[i].y[1], traps[i].x[2], traps[i].y[2]);
	}

	// Player
	CP_Image_Draw(player_sprite, player.x - 15, player.y - 15, player.width + 30, player.height + 30, 255);

#pragma endregion

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };
}

void L1_Exit(void)
{

	CP_Image_Free(player_sprite);

}