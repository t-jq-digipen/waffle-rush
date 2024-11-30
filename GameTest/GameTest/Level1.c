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


void L1_Init(void)
{
	// VOLUME
	// load options file (music volume)
	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	// load music
	music = CP_Sound_LoadMusic("Assets/tutorial.wav");
	CP_Sound_PlayAdvanced(music, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_0);

	// load sfx
	sfx = CP_Sound_Load("Assets/WaffleMachine.wav");
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
	FILE* CheckpointFile = fopen("Assets/Level_1/Checkpoint.txt", "r");

	init_checkpoints(CheckpointFile);

	fclose(CheckpointFile);


	// initialize spawn location
	spawn.x = checkpoint.coordinates[0].x - 10;
	spawn.y = checkpoint.coordinates[0].y - 60;
	// initialize goal
	goalx = 2800;
	goaly = 1300;

	// initialize powerups location
	buffstate = 0b000;
	Buff[jumpBuff].canPickup = 1; //if active,
	Buff[jumpBuff].X = 1400; //draw
	Buff[jumpBuff].Y = 1325; //draw
	Buff[jumpBuff].width = 20; //draw
	Buff[jumpBuff].height = 20; //draw
	Buff[jumpBuff].buffduration = 0; //seconds?
	Buff[jumpBuff].jumpIncrease = 800;
	Buff[jumpBuff].resetTimer = 3;
	jumpalpha = 50;


	Buff[invulBuff].canPickup = 1; //if active,
	Buff[invulBuff].X = -750; //draw
	Buff[invulBuff].Y = -675; //draw
	Buff[invulBuff].width = 20; //draw
	Buff[invulBuff].height = 20; //draw
	Buff[invulBuff].buffduration = 0; //seconds?
	Buff[invulBuff].invulActive = 0;
	Buff[invulBuff].resetTimer = 3;
	shieldalpha = 50;

	// PLATFORMS INITIALIZATION
	/*---------------------------------------------------------------------------------------------------------------------*/
#pragma region Initialize Platform

// Platforms

// Platforms
	// resets all platforms to 0
	reset_platforms();

	FILE* PlatformFile = fopen("Assets/Level_1/Platform.txt", "r");

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

	FILE* WallFile = fopen("Assets/Level_1/Wall.txt", "r");

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

	FILE* HazardFile = fopen("Assets/Level_1/Hazard.txt", "r");
	fscanf_s(HazardFile, "%d", &numberoftraps);
	init_hazards(HazardFile, numberoftraps);
	fclose(HazardFile);
	spawnTimer = 0.0f;     // Timer to control spawning frequency
	spawnInterval = 1.f;  // Spawn a square every 1 second
	bulletx = 1750;
	bullety = 550;
	firetimer = 0;

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

	// initialize level number
	levelNumber = 1;

	// score
	levelScore = 0;

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

	// Options
	FILE* OptionsFile = fopen("Assets/Options.txt", "r");
	fscanf_s(OptionsFile, "%f %f", &current_bgm_vol, &current_se_vol);
	fclose(OptionsFile);

	reset_init		= L1_Init;
	reset_update	= L1_Update;
	reset_exit		= L1_Exit;

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

void L1_Update(void)
{
	// background
	CP_Graphics_ClearBackground(bg);
	
	// set volumes
	AudioPitch();
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_bgm_vol / 100.0f);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, current_se_vol / 100.0f);

	// getdt
	dt = CP_System_GetDt();
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
			jump(&player, movestate, buffstate);

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
			if ((player.y >= 1800) && movestate == OnPlat && player.deathcause != Fall) {
				Kill_Player(&player, Fall, &deaths, &buffstate);
			}
		}

#pragma endregion


		// Powerups
		/*---------------------------------------------------------------------------------------------------*/
#pragma region Powerups Update



		//checking for collision between players and buff
		//buff_pickup(Buff[jumpBuff].X+camera.worldshiftx, Buff[jumpBuff].Y+camera.worldshifty, Buff[jumpBuff].width, Buff[jumpBuff].height);
		if (buff_pickup(Buff[jumpBuff].X, Buff[jumpBuff].Y, Buff[jumpBuff].height, Buff[jumpBuff].width) && Buff[jumpBuff].canPickup == 1) {
			buffstate = buffstate | jumpBuff;
			Buff[jumpBuff].canPickup = 0; //availability of the buff on the map
			Buff[jumpBuff].buffduration = 3;  //this resets the duration so that you can pick up the same buff while its still active

		}
		if (buff_pickup(Buff[invulBuff].X, Buff[invulBuff].Y, Buff[invulBuff].height, Buff[invulBuff].width) && Buff[invulBuff].canPickup == 1) {
			buffstate = buffstate | invulBuff;
			Buff[invulBuff].canPickup = 0; //availability of the buff on the map
			Buff[invulBuff].buffduration = 3;  //this resets the duration so that you can pick up the same buff while its still active
		}

		//Reset the spawn of the buffs
		if (Buff[jumpBuff].canPickup == 0 && Buff[jumpBuff].resetTimer > 0) {
			Buff[jumpBuff].resetTimer -= 1 * dt;
			//printf("jump reset timer: %f\n", Buff[jumpBuff].resetTimer);
			if (Buff[jumpBuff].resetTimer <= 0) {
				Buff[jumpBuff].canPickup = 1;
				Buff[jumpBuff].resetTimer = 3;
			}
		}
		if (Buff[invulBuff].canPickup == 0 && Buff[invulBuff].resetTimer > 0) {
			Buff[invulBuff].resetTimer -= 1 * dt;
			//printf("invul reset timer: %f\n", Buff[invulBuff].resetTimer);
			if (Buff[invulBuff].resetTimer <= 0) {
				Buff[invulBuff].canPickup = 1;
				Buff[invulBuff].resetTimer = 3;
			}
		}

		//Set the stats for jumpboost below the first if statement

		jumpalpha = 50;
		if ((buffstate & 0b010) && Buff[jumpBuff].buffduration > 0.00f) { //here jumpbuff is active
			Buff[jumpBuff].buffduration -= 1 * dt;
			jumpalpha = 255;
			printf("JUMP BUFF ACTIVE!\n");
			//printf("buff: %d\n", buffstate); 
			//printf("jump increase duration: %f\n", Buff[jumpBuff].buffduration);
		}
		else if ((buffstate & 0b010) && Buff[jumpBuff].buffduration <= 0.00f) {
			buffstate &= ~0b010;
			Buff[jumpBuff].buffduration = 3; //maybe can functionise these two to reset
		}


		//Set the stats for invulnerability below the first if statement

		shieldalpha = 50;
		if ((buffstate & 0b100) && Buff[invulBuff].buffduration > 0.00f) { //here invulbuff is active
			Buff[invulBuff].buffduration -= 1 * dt;
			shieldalpha = 255;
			printf("INVUL BUFF ACTIVE!\n");
			//printf("buff: %d\n", buffstate);
			//printf("invul duration: %f\n", Buff[invulBuff].buffduration);
		}
		else if ((buffstate & 0b100) && Buff[invulBuff].buffduration <= 0.00f) {
			buffstate &= ~0b100;
			Buff[invulBuff].buffduration = 3; //maybe can functionise these two to reset
		}

#pragma endregion

	// Hazard
	/*---------------------------------------------------------------------------------------------------*/
#pragma region Hazard stuff

		// SHOOTER LOGIC
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
				bullets[i].x -= BULLET_SPEED * CP_System_GetDt();     // Move square left
				if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_SIZE)) {
					CauseOfDeath pewpew = (movestate == OnPlat) ? Burn : Fall;
					Kill_Player(&player, pewpew, &deaths, &buffstate);
				}
				if (bullets[i].x + BULLET_SIZE < 1400 + BULLET_SIZE) {   // Deactivate square if off-screen
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

	// render ALL THE THINGS
	RENDER_ALL();

		// LEVEL COMPLETE PAGE
		if (levelstate == Cleared) {
			if (clear_timer >= 5) {
				CP_Settings_RectMode(CP_POSITION_CENTER);
				CP_Settings_Fill(pausebg);
				CP_Graphics_DrawRect(width / 2.0f, height / 2.0f, width, height);
				CP_Settings_Fill(wall);
				CP_Graphics_DrawRect(width / 2.0f, height / 2.0f, 1200, 700);
				CP_Settings_Fill(black);
				CP_Settings_TextSize(70);
				CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
				CP_Font_DrawText("LEVEL COMPLETE", width / 2, 200);

				CP_Settings_TextSize(40);
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

				CP_Settings_TextSize(30);
				// draw next level button
				TextBoxCreate(500, 720, 250, 100, "NEXT LEVEL", red);
				int NextLevelClick = IsAreaClicked(500, 720, 250, 100, mouseX, mouseY); //if mouse is over button
				if (NextLevelClick) {
					CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
					CP_Graphics_DrawRect(500, 720, 250, 100);
					if (CP_Input_MouseClicked()) { //if LMB1 pressed
						SetNext(L2_Init, L2_Update, L2_Exit);
					}
				}

				// draw replay button
				TextBoxCreate(800, 720, 250, 100, "REPLAY", red);
				int ReplayClick = IsAreaClicked(800, 720, 250, 100, mouseX, mouseY); //if mouse is over button
				if (ReplayClick) {
					CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
					CP_Graphics_DrawRect(800, 720, 250, 100);
					if (CP_Input_MouseClicked()) { //if LMB1 pressed
						CP_Engine_SetNextGameStateForced(L1_Init, L1_Update, L1_Exit);
					}
				}

				// draw quit button
				TextBoxCreate(1100, 720, 250, 100, "EXIT", red);
				int QuitClick = IsAreaClicked(1100, 720, 250, 100, mouseX, mouseY); //if mouse is over button
				if (QuitClick) {
					CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
					CP_Graphics_DrawRect(1100, 720, 250, 100);
					if (CP_Input_MouseClicked()) { //if LMB1 pressed
						SetNext(LSelect_Init, LSelect_Update, LSelect_Exit);
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

		//LEVEL TRANSITION EFFECTS
		Level_Transition(next_init, next_update, next_exit);

#pragma endregion

}


void L1_Exit(void)
{
	// Frees images :D say no to memory leak
	Free_Images();

	CP_Sound_Free(&music);
	CP_Sound_Free(&sfx);
}