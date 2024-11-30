//---------------------------------------------------------
// file:	mainmenu.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Main menu stage with a play button and an exit
//          button
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "mainmenu.h"

CP_Image logo;
CP_Font exo;
CP_Sound music;
CP_Color button_color;
bool inMenu = 0;
bool inStart = 1;
float start_timer, logo_alpha, logo2_alpha = 0.0f;

void Main_Menu_Init(void)
{
	//load font
	exo = CP_Font_Load("Assets/Better VCR 6.1.ttf");
	CP_Font_Set(exo);
	CP_Settings_TextSize(150);

	// load logo
	logo = CP_Image_Load("Assets/DigiPen_Singapore_WEB_WHITE.png");
	CP_Settings_ImageMode(CP_POSITION_CENTER);
	player_sprite = CP_Image_Load("Assets/Player_Sprites.png");


	// load background color
	bg = CP_Color_Create(135, 200, 220, 255);

	// load options file (music volume)
	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSFX Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void Main_Menu_Update(void)
{
	//assign window width and height
	int width = CP_System_GetWindowWidth();
	int height = CP_System_GetWindowHeight();

	//track pointer
	mouseX = CP_Input_GetMouseX();
	mouseY = CP_Input_GetMouseY();

	//button color
	button_color = CP_Color_Create(200, 60, 60, 255);

	//logo
	float dt = CP_System_GetDt();
	dt = CP_Math_ClampFloat(dt, 0.0f, 1.0f); //prevent excess lagspike clipping

	start_timer += dt;

	// skip intro (for testing purposes)
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		start_timer += 10.0f;
	}

	// draw logo (before main menu)
	if (start_timer <= 3.0f) {
		logo_alpha += 5.0f;
		if (logo_alpha > 255.0f) {
			logo_alpha = 255.0f;
		}
	}
	else if (start_timer <= 4.0f) {
		logo_alpha -= 5.0f;
		if (logo_alpha < 0.0f) {
			logo_alpha = 0.0f;
		}
	}
	if (start_timer > 4.0f && start_timer <= 7.0f) {
		logo2_alpha += 5.0f;
		if (logo2_alpha > 255.0f) {
			logo2_alpha = 255.0f;
		}
	}
	else {
		logo2_alpha -= 5.0f;
		if (logo2_alpha < 0.0f) {
			logo2_alpha = 0.0f;
		}
	}

	if (start_timer > 8.0f) {
		inStart = 0;
	}

	if (inStart == 1) {
		CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
		CP_Image_Draw(logo, width / 2, height / 2, CP_Image_GetWidth(logo), CP_Image_GetHeight(logo), logo_alpha);

		CP_Settings_Fill(CP_Color_Create(255, 255, 255, logo_alpha));
		CP_Settings_TextSize(20);
		CP_Font_DrawText("All content \xC2\xA9 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", width / 2, height - 50);

		CP_Image_Draw(player_sprite, width / 2, height / 2, 1000, 600, logo2_alpha);
	}
	else if (inStart == 0) {
		// load music
		if (inMenu == 0) {
			music = CP_Sound_LoadMusic("Assets/Dumba.wav");
			CP_Sound_PlayAdvanced(music, 1.0f, 1.0f, TRUE, CP_SOUND_GROUP_0);
			inMenu = 1;
		}

		//background
		CP_Graphics_ClearBackground(CP_Color_Create(200, 200, 200, 255));
		CP_Graphics_ClearBackground(bg);

		// set volumes
		CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_bgm_vol / 100.0f);

		// set text size
		// game title
		CP_Settings_TextSize(150);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("WAFFLE RUSH", width / 2, 100);


		// set text size
		CP_Settings_TextSize(80);


		////draw test play button (FOR ASH)
		//TextBoxCreate(width / 2, height / 9, 500, 150, "ASH", button_color);
		//int AshClick = IsAreaClicked(width / 2, height / 9, 500, 150, mouseX, mouseY); //if mouse is over button
		//if (AshClick) {
		//	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		//	CP_Graphics_DrawRect(width / 2, height / 9, 500, 150);
		//	if (CP_Input_MouseClicked()) { //if LMB1 pressed
		//		CP_Engine_SetNextGameState(MainMenu1_Init, MainMenu1_Update, MainMenu1_Exit); //go to game
		//	}
		//}
		//draw play button
		TextBoxCreate(width / 2, height / 3, 500, 150, "PLAY", button_color);
		int PlayClick = IsAreaClicked(width / 2, height / 3, 500, 150, mouseX, mouseY); //if mouse is over button
		if (PlayClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 2, height / 3, 500, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit); //go to game
				inMenu = 0;
				CP_Sound_Free(&music);
			}
		}

		////test level button
		//TextBoxCreate(3 * width / 4, height / 3, 150, 150, "T", button_color);
		//int TestClick = IsAreaClicked(3 * width / 4, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
		//if (TestClick) {
		//	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		//	CP_Graphics_DrawRect(3 * width / 4, height / 3, 150, 150);
		//	if (CP_Input_MouseClicked()) { //if LMB1 pressed
		//		CP_Engine_SetNextGameState(Tutorial_Init, Tutorial_Update, Tutorial_Exit); //go to test stage
		//	}
		//}

		//options button
		TextBoxCreate(2 * width / 3, 5 * height / 9, 500, 150, "OPTIONS", button_color);
		int OptionsClick = IsAreaClicked(2 * width / 3, 5 * height / 9, 500, 150, mouseX, mouseY); //if mouse is over button
		if (OptionsClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(2 * width / 3, 5 * height / 9, 500, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(Options_Init, Options_Update, Options_Exit); //go to test stage
			}
		}

		//credits button
		TextBoxCreate(width / 3, 5 * height / 9, 500, 150, "CREDITS", button_color);
		int CreditsClick = IsAreaClicked(width / 3, 5 * height / 9, 500, 150, mouseX, mouseY); //if mouse is over button
		if (CreditsClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 3, 5 * height / 9, 500, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit); //go to test stage
			}
		}

		//draw exit button
		TextBoxCreate(width / 2, 7 * height / 9, 500, 150, "EXIT", button_color);
		int ExitClick = IsAreaClicked(width / 2, 7 * height / 9, 500, 150, mouseX, mouseY); //if mouse is over button
		if (ExitClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 2, 7 * height / 9, 500, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_Terminate(); //end game
			}
		}
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
	}

}

void Main_Menu_Exit(void)
{
	//CP_Sound_Free(&music);
}