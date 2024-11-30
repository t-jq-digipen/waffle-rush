//---------------------------------------------------------
// file:	credits.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Credits page
//			
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "credits.h"

#define num_names 5
credits names[num_names];

CP_Font exo;
CP_Sound sfx;

credit_screen screen_num;

void Credits_Init(void) {

	// VOLUME
	// load options file (music volume)
	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	// load font
	exo = CP_Font_Load("Assets/Better VCR 6.1.ttf");
	CP_Font_Set(exo);

	// load credits file
	// load background color
	bg = CP_Color_Create(135, 200, 220, 255);

	// load credits file
	FILE* file2 = fopen("Assets/Credits.txt", "r");
	for (int i = 0; i < num_names; i++) {
		fgets(names[i].name, sizeof(names[i].name), file2);
	}
	fclose(file2);

	// load sfx
	sfx = CP_Sound_Load("Assets/PlayerSpawn.wav");

	// set first screen when opening credits
	screen_num = FIRST;

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_RectMode(CP_POSITION_CENTER);
}

void Credits_Update(void) {
	//assign window width and height
	int width = CP_System_GetWindowWidth();
	int height = CP_System_GetWindowHeight();

	//track pointer
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	// button color
	CP_Color button_color = CP_Color_Create(200, 60, 60, 255);
	// disabled button color
	CP_Color disabled_color = CP_Color_Create(128, 128, 128, 255);

	//background
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));
	// set volumes
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_0, current_bgm_vol / 100.0f);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, current_se_vol / 100.0f);

	// background
	CP_Graphics_ClearBackground(bg);

	//header text
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(100);
	CP_Settings_TextSize(70);
	CP_Font_DrawText("CREDITS:", (float)width / 2.0f, (float)100);

	CP_Settings_TextSize(100);
	char credits[500] = { 0 };

	// funny easter egg
	int EasterEggClick = IsAreaClicked((float)width / 4.0f, (float)525.0f, 120.0f, 70.0f, mouseX, mouseY); //if mouse is over button
	if (EasterEggClick) {
		if (CP_Input_MouseClicked()) {
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Sound_PlayAdvanced(sfx, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_1);
		}
	}

	if (screen_num == FIRST) {
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("THE TEAM", (float)width / 4.0f, (float)200.0f);
		CP_Settings_TextSize(30);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		for (int i = 0; i < num_names; i++) {
			sprintf_s(credits, sizeof(credits), names[i].name);
			credits[strcspn(credits, "\n")] = '\0';
			if (i == 3 && EasterEggClick) {
				CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
			}
			CP_Font_DrawText(credits, (float)width / 4.0f, (float)(300 + (i * 75)));
			CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		}
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("INSTRUCTORS", (float)2 * width / 3.0f, (float)200.0f);
		CP_Settings_TextSize(30);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		CP_Font_DrawText("Ding Xiang Cheng", (float)2 * width / 3.0f, (float)300.0f);
		CP_Font_DrawText("Gerald Wong", (float)2 * width / 3.0f, (float)375.0f);
		CP_Font_DrawText("Soroor Malekmohmmadi Faradounbeh", (float)2 * width / 3.0f, (float)450.0f);
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("CREATED AT", (float)2 * width / 3.0f, (float)525.0f);
		CP_Settings_TextSize(30);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		CP_Font_DrawText("DigiPen Institute of Technology Singapore", (float)2 * width / 3.0f, (float)600.0f);

		CP_Settings_TextSize(20);
		CP_Font_DrawText("All content \xC2\xA9 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", (float)width / 2, (float)700.0f);
		CP_Settings_TextSize(100);
	}
	else if (screen_num == SECOND) {
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("PRESIDENT", (float)width / 2.0f, (float)200.0f);
		CP_Settings_TextSize(30);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		CP_Font_DrawText("Claude COMAIR", (float)width / 2.0f, (float)275.0f);
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(168, 122, 27, 255));
		CP_Font_DrawText("EXECUTIVES", (float)width / 2.0f, (float)375.0f);
		CP_Settings_TextSize(30);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		CP_Font_DrawText("CHU Jason Yeu Tat", (float)width / 4.0f, (float)450.0f);
		CP_Font_DrawText("Michele COMAIR", (float)width / 2.0f, (float)450.0f);
		CP_Font_DrawText("Christopher COMAIR", (float)3 * width / 4.0f, (float)450.0f);
		CP_Font_DrawText("Alexander COMAIR", (float)width / 4.0f, (float)525.0f);
		CP_Font_DrawText("Michael GATS", (float)width / 2.0f, (float)525.0f);
		CP_Font_DrawText("TAN Chek Ming", (float)3 * width / 4.0f, (float)525.0f);
		CP_Font_DrawText("Prasanna GHALI", (float)width / 4.0f, (float)600.0f);
		CP_Font_DrawText("Mandy WONG", (float)width / 2.0f, (float)600.0f);
		CP_Font_DrawText("Johnny DEEK", (float)3 * width / 4.0f, (float)600.0f);

		CP_Settings_TextSize(20);
		CP_Font_DrawText("All content \xC2\xA9 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", (float)width / 2, (float)700.0f);
		CP_Settings_TextSize(100);
	}
	else if (screen_num == THIRD) {
		CP_Settings_TextSize(50);
		CP_Settings_TextSize(70);
		CP_Settings_Fill(CP_Color_Create(204, 125, 71, 255));
		CP_Font_DrawText("INSERT REFERENCES HERE", (float)width / 2.0f, (float)height / 2.0f);

		CP_Settings_TextSize(20);
		CP_Font_DrawText("All content \xC2\xA9 2024 DigiPen Institute of Technology Singapore. All Rights Reserved.", (float)width / 2, (float)700.0f);
		CP_Settings_TextSize(100);
	}

	CP_Settings_TextSize(70);

	//toggle screen buttons
	if (screen_num != FIRST) {
		TextBoxCreate((float)(width / 4), (float)(height - 100), 100.0f, 100.0f, "<", button_color);
		int BackClick = IsAreaClicked((float)(width / 4), (float)(height - 100), 100.0f, 100.0f, mouseX, mouseY); //if mouse is over button
		if (BackClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect((float)(width / 4), (float)(height - 100), 100.0f, 100.0f);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				screen_num -= 1;
			}
		}
	}
	else {
		TextBoxCreate((float)(width / 4), (float)(height - 100), 100.0f, 100.0f, "<", disabled_color);
	}

	if (screen_num != THIRD) {
		TextBoxCreate((float)(3 * width / 4), (float)(height - 100), 100.0f, 100.0f, ">", button_color);
		int NextClick = IsAreaClicked((float)(3 * width / 4), (float)(height - 100), 100.0f, 100.0f, mouseX, mouseY); //if mouse is over button
		if (NextClick) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect((float)(3 * width / 4), (float)(height - 100), 100.0f, 100.0f);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				screen_num += 1;
			}
		}
	}
	else {
		TextBoxCreate((float)(3 * width / 4), (float)(height - 100), 100.0f, 100.0f, ">", disabled_color);
	}

	//back to main menu button
	TextBoxCreate((float)(width / 2), (float)(height - 100), 400.0f, 120.0f, "BACK", button_color);
	int ExitClick = IsAreaClicked((float)(width / 2), (float)(height - 100), 400.0f, 120.0f, mouseX, mouseY); //if mouse is over button
	if (ExitClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect((float)(width / 2), (float)(height - 100), 400.0f, 120.0f);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); //return to main menu
		}
	}



	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };

}

void Credits_Exit(void) {

	CP_Sound_Free(&sfx);
}

