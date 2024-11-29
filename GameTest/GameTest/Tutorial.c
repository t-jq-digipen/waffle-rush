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
#include "globals.h"

void Tutorial_Init(void)
{
	// VOLUME
	// load options file (music volume)
	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	// load music
	music = CP_Sound_LoadMusic("Assets/TutorialMusic.wav");
	CP_Sound_PlayAdvanced(music, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_0);

	// load sfx
	sfx = CP_Sound_Load("Assets/PlayerSpawn.wav");
	CP_Sound_PlayAdvanced(sfx, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_1);

	CP_Sound_ResumeGroup(CP_SOUND_GROUP_1);

	// ASSET LOADS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Assets
	// load font
	text				= CP_Font_Load("Assets/Exo2-Regular.ttf");

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
	FILE* CheckpointFile = fopen("Assets/Level_Tutorial/Checkpoint.txt", "r");

	init_checkpoints(CheckpointFile);

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

// Platforms
	// resets all platforms to 0
	reset_platforms();

	FILE* PlatformFile = fopen("Assets/Level_Tutorial/Platform.txt", "r");

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

	FILE* WallFile = fopen("Assets/Level_Tutorial/Wall.txt", "r");

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

	FILE* HazardFile = fopen("Assets/Level_Tutorial/Hazard.txt", "r");
	fscanf_s(HazardFile, "%d", &numberoftraps);
	init_hazards(HazardFile, numberoftraps);
	fclose(HazardFile);
#pragma endregion
#pragma endregion
	// PLAYER INITS
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Player related
	Init_Player(&speed, &player, &deaths, spawn);
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

	// clear anim
	clear_timer = 0;

#pragma endregion
	// Misc settings
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Misc inits and settings
	// Window size
	width = (float)CP_System_GetWindowWidth();
	height = (float)CP_System_GetWindowHeight();

	// Initialize colors
	pausebg		= CP_Color_Create(0, 0, 0, 100);
	bg			= CP_Color_Create(135, 200, 220, 255); //background
	black		= CP_Color_Create(0, 0, 0, 255); //black
	red			= CP_Color_Create(200, 50, 50, 255); //red
	green		= CP_Color_Create(50, 200, 50, 255); //green
	plat		= CP_Color_Create(255, 200, 80, 255); //platform colour
	wall		= CP_Color_Create(200, 200, 200, 255); //platform colour
	stamina		= CP_Color_Create(255, 255, 40, 255); //stamina colour

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
#pragma region Camera Init + Transition Init

	Cam_Init(&camera, width, height, spawn);

	playerLastX = player.x - camera.worldshiftx;
	playerLastY = player.y - camera.worldshifty;
	levelEnded = start;
	scaler1 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler2 = (spawn.x + player.width / 2) - camera.worldshiftx;
	scaler3 = (spawn.y + player.height / 2) - camera.worldshifty;
	scaler4 = (spawn.y + player.height / 2) - camera.worldshifty;

#pragma endregion
}

void Tutorial_Update(void)
{
	// background
	CP_Graphics_ClearBackground(bg);

	// set volumes
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_bgm_vol / 100.0f);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, current_se_vol / 100.0f);

	// getdt
	float dt = CP_System_GetDt();
	dt = CP_Math_ClampFloat(dt, 0.0f, 0.05f); //prevent excess lagspike clipping

	// get mouse coords
	mouseX = CP_Input_GetMouseX();
	mouseY = CP_Input_GetMouseY();

	// pause/resume game with Esc
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && levelstate == NotPaused) {
		levelstate = Paused;
		CP_Sound_PauseGroup(CP_SOUND_GROUP_1);
	}
	else if (CP_Input_KeyTriggered(KEY_ESCAPE) && levelstate == Paused) {
		levelstate = NotPaused;
		CP_Sound_ResumeGroup(CP_SOUND_GROUP_1);
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
			if (CP_Input_KeyTriggered(KEY_K) && !(player.state == Dead || player.state == Spawning) && levelstate != Cleared) {
				CauseOfDeath reset = (movestate == InPlat) ? Fall : Burn;
				Kill_Player(&player, reset, &deaths, &buffstate);
			}
			if ((player.y >= height - player.height) && movestate == OnPlat && player.deathcause != Fall && levelstate != Cleared) {
				Kill_Player(&player, Fall, &deaths, &buffstate);
			}
		}

#pragma endregion
		// Hazard
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Hazard stuff

		spawnTimer += CP_System_GetDt();
		if (spawnTimer >= spawnInterval) {
			for (int i = 0; i < MAX_BULLETS; ++i) {
				if (!bullets[i].active) {
					bullets[i].x = bulletx; // Spawn at the defined starting point
					bullets[i].y = bullety; // Align along the defined line
					bullets[i].active = true;
					break;
				}
			}
			spawnTimer = 0.0f;
		}

		for (int i = 0; i < MAX_BULLETS; ++i) {
			if (bullets[i].active) {
				bullets[i].x -= BULLET_SPEED * CP_System_GetDt(); // Move square left
				if (bullets[i].x + BULLET_SIZE < 0) {            // Deactivate square if off-screen
					bullets[i].active = false;
				}
			}
		}

		hazard_update();

#pragma endregion

		// CAMERA UPDATE
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Camera

		CamShift(&camera, mouseX, mouseY, width, height);
		if (!(player.state == Dead)) {
			playerCamColl(&player, &camera);
		}
	}

#pragma endregion

	// RENDER
	/*---------------------------------------------------------------------------------------------------*/
#pragma region RENDER

		// TOOLTIPS
		CP_Settings_TextSize(25);
		CP_Settings_Fill(CP_Color_Create(50, 50, 50, 255));
		CP_Font_DrawTextBox("Move your mouse to the edges of the screen to look around,\nR to reset the camera\n\nA and D keys to move left and right.\nSPACE to jump\n\nK to KILL the waffle", 50 - camera.worldshiftx, 50 - camera.worldshifty, 300);
		CP_Font_DrawTextBox("E key to ENTER the side of syrup-covered platforms\n\nWASD to crawl around >:)\nNOTE: if you run out of stamina, you DIE", 650 - camera.worldshiftx, 250 - camera.worldshifty, 250);
		CP_Font_DrawTextBox("Move into a wall to slide, jump while sliding against a wall (holding movement input) to perform a walljump!", 1050 - camera.worldshiftx, 150 - camera.worldshifty, 250);
		CP_Font_DrawTextBox("Touch a closed waffle press to set your new checkpoint!\n\nTry it by\nKILLING the waffle (K)", 1500 - camera.worldshiftx, 550 - camera.worldshifty, 250);
		CP_Font_DrawTextBox("Land on the golden plate to complete the level :D", 2350 - camera.worldshiftx, 700 - camera.worldshifty, 250);

		// render ALL THE THINGS
		RENDER_ALL();

	// LEVEL COMPLETE PAGE
	if (levelstate == Cleared) {
		if (clear_timer >= 5) {
			CP_Settings_RectMode(CP_POSITION_CENTER);
			CP_Settings_Fill(pausebg);
			CP_Graphics_DrawRect(width / 2, height / 2, width, height);
			CP_Settings_Fill(wall);
			CP_Graphics_DrawRect(width / 2, 2 * height / 5, 1200, 500);
			CP_Settings_Fill(black);
			CP_Settings_TextSize(100);
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Font_DrawText("TUTORIAL COMPLETE", width / 2, 200);


			CP_Settings_TextSize(50);
			// draw resume button
			TextBoxCreate(600, 520, 250, 100, "LEVELS", red);
			int ResumeClick = IsAreaClicked(600, 520, 250, 100, mouseX, mouseY); //if mouse is over button
			if (ResumeClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
				CP_Graphics_DrawRect(600, 520, 250, 100);
				if (CP_Input_MouseClicked()) { //if LMB1 pressed
					SetNext(&LSelect_Init, &LSelect_Update, &LSelect_Exit);
					levelEnded = end;
					levelstate = EndLevel;
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
	}

	// PAUSE MENU
	if (levelstate == Paused) {
		PauseMenu();
	}
	// OPTIONS CLICKED
	else if (levelstate == InOptions) {
		OptionsMenu();
	}
	// QUIT CLICKED
	else if (levelstate == InQuit) {
		QuitMenu();
	}


	// LEVEL TRANSITION EFFECTS
	Level_Transition(next_init, next_update, next_exit);

#pragma endregion

}

void Tutorial_Exit(void)
{
	// Frees images :D say no to memory leak
	Free_Images();

	CP_Sound_Free(&music);
	CP_Sound_Free(&sfx);
}