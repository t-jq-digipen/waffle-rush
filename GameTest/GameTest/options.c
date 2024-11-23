//---------------------------------------------------------
// file:	options.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Options menu
//			
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "options.h"

CP_Font exo;

void Options_Init(void) {

	//load font
	exo = CP_Font_Load("Assets/Exo2-Regular.ttf");
	CP_Font_Set(exo);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_RectMode(CP_POSITION_CENTER);

	// load options file

	FILE* file = fopen("Assets/Options.txt", "r");
	fscanf_s(file, "%f %f", &current_bgm_vol, &current_se_vol);
	printf("BGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	printf("---------------------------------------------------------------\n");
	fclose(file);

	// set max volumes for BGM and sound effects
	max_bgm_vol = 100.0f;
	max_se_vol = 100.0f;

	// set size for icons
	icon1_width = 10.0f;
	icon1_height = 10.0f;
	icon2_width = 20.0f;
	icon2_height = 70.0f;

	// set position for icons
	icon_bgm_x = CP_System_GetWindowWidth() / 2;
	icon_bgm_y = CP_System_GetWindowHeight() / 3;
	icon_se_x = CP_System_GetWindowWidth() / 2;
	icon_se_y = CP_System_GetWindowHeight() / 2;
}

void Options_Update(void) {
	//assign window width and height
	int width = CP_System_GetWindowWidth();
	int height = CP_System_GetWindowHeight();

	//track pointer
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	// back button color
	CP_Color button_color = CP_Color_Create(200, 60, 60, 255);

	//background
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));

	//header text
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(100);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("OPTIONS", width / 2, 100);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(70);
	CP_Font_DrawText("BGM Volume", width / 16, height / 3);
	CP_Font_DrawText("Sound Volume", width / 16, height / 2);

	// update BGM volume
	if (current_bgm_vol > max_bgm_vol) {
		current_bgm_vol = max_bgm_vol;
	}
	else if (current_bgm_vol < 0) {
		current_bgm_vol = 0.0f;
	}
	
	// lower BGM volume button
	TextBoxCreate(icon_bgm_x - 100, icon_bgm_y, 50, 50, "-", button_color);
	int LowerBGMClick = IsAreaClicked(icon_bgm_x - 100, icon_bgm_y, 50, 50, mouseX, mouseY); //if mouse is over button
	if (LowerBGMClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(icon_bgm_x - 100, icon_bgm_y, 50, 50);
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
			current_bgm_vol -= 1;
		}
	}

	// raise BGM volume button
	TextBoxCreate(width - 150, icon_bgm_y, 50, 50, "+", button_color);
	int RaiseBGMClick = IsAreaClicked(width - 150, icon_bgm_y, 50, 50, mouseX, mouseY); //if mouse is over button
	if (RaiseBGMClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(width - 150, icon_bgm_y, 50, 50);
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
			current_bgm_vol += 1;
		}
	}

	// update BGM volume bar
	if (current_bgm_vol > 0) {
		for (float i = 0.0f; i < max_bgm_vol; i += 5.0f) {
			CP_Settings_Fill(button_color);
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
			CP_Settings_Fill(button_color);
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
	TextBoxCreate(icon_se_x - 100, icon_se_y, 50, 50, "-", button_color);
	int LowerSEClick = IsAreaClicked(icon_se_x - 100, icon_se_y, 50, 50, mouseX, mouseY); //if mouse is over button
	if (LowerSEClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(icon_se_x - 100, icon_se_y, 50, 50);
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
			current_se_vol -= 1;
		}
	}

	// raise SE volume button
	TextBoxCreate(width - 150, icon_se_y, 50, 50, "+", button_color);
	int RaiseSEClick = IsAreaClicked(width - 150, icon_se_y, 50, 50, mouseX, mouseY); //if mouse is over button
	if (RaiseSEClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(width - 150, icon_se_y, 50, 50);
		if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT)) { //if LMB1 pressed
			current_se_vol += 1;
		}
	}

	// update SE volume bar
	if (current_se_vol > 0) {
		for (float i = 0.0f; i < max_se_vol; i += 5.0f) {
			CP_Settings_Fill(button_color);
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
			CP_Settings_Fill(button_color);
			CP_Graphics_DrawRect(icon_se_x + 30.0f * (i / 5.0f), icon_se_y, icon1_width, icon1_height);
		}
	}

	//save changes button
	TextBoxCreate(3 * width / 8, height - 100, 300, 150, "SAVE", button_color);
	int SaveClick = IsAreaClicked(3 * width / 8, height - 100, 300, 150, mouseX, mouseY); //if mouse is over button
	if (SaveClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(3 * width / 8, height - 100, 300, 150);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			SaveOptions(current_bgm_vol, current_se_vol);
		}
	}

	//discard changes and return to main menu button
	TextBoxCreate(5 * width / 8, height - 100, 300, 150, "BACK", button_color);
	int ExitClick = IsAreaClicked(5 * width / 8, height - 100, 300, 150, mouseX, mouseY); //if mouse is over button
	if (ExitClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(5 * width / 8, height - 100, 300, 150);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); //return to main menu
		}
	}

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };
}

void Options_Exit(void) {

}

// ------------------------------------------------------------------------------------------------------------------
// Functions for Pause Menu

// PAUSE MENU (When Levelstate == Paused)

void PauseMenu() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(pausebg);
	CP_Graphics_DrawRect(width / 2, height / 2, width, height);
	CP_Settings_Fill(wall);
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

// OPTIONS MENU (When Levelstate == InOptions)

void OptionsMenu() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(pausebg);
	CP_Graphics_DrawRect(width / 2, height / 2, width, height);
	CP_Settings_Fill(wall);
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
			SaveOptions(current_bgm_vol, current_se_vol);
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

// QUIT MENU (When Levelstate == InQuit)

void QuitMenu() {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(pausebg);
	CP_Graphics_DrawRect(width / 2, height / 2, width, height);
	CP_Settings_Fill(wall);
	CP_Graphics_DrawRect(width / 2, height / 2, 1200, 600);
	CP_Settings_Fill(black);
	CP_Settings_TextSize(100);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Are you sure you want to quit?", width / 2, 300);
	CP_Font_DrawText("Any progress will be lost!", width / 2, 400);

	CP_Settings_TextSize(100);
	// go back to pause menu
	TextBoxCreate(500, 2 * height / 3, 300, 200, "BACK", red);
	int GoBackClick = IsAreaClicked(500, 2 * height / 3, 300, 200, mouseX, mouseY); //if mouse is over button
	if (GoBackClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
		CP_Graphics_DrawRect(500, 2 * height / 3, 300, 200);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			levelstate = Paused;
		}
	}
	// quit the level and go back to main menu
	TextBoxCreate(1100, 2 * height / 3, 300, 200, "QUIT", red);
	int GoHomeClick = IsAreaClicked(1100, 2 * height / 3, 300, 200, mouseX, mouseY); //if mouse is over button
	if (GoHomeClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); // highlight button
		CP_Graphics_DrawRect(1100, 2 * height / 3, 300, 200);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit);
		}
	}

	CP_Settings_RectMode(CP_POSITION_CORNER);
}

// save changed options while in options
void SaveOptions(float current_bgm_vol, float current_se_vol) {
	FILE* OptionsFile = fopen("Assets/Options.txt", "w");
	fprintf_s(OptionsFile, "%lf %lf", current_bgm_vol, current_se_vol);
	printf("Changes saved successfully!\nBGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	fclose(OptionsFile);
}

// save changed options while in game
void SaveOptionsInGame(float current_bgm_vol, float current_se_vol, float *prev_bgm_vol, float *prev_se_vol) {
	FILE* OptionsFile = fopen("Assets/Options.txt", "w");
	fprintf_s(OptionsFile, "%lf %lf", current_bgm_vol, current_se_vol);
	printf("Changes saved successfully!\nBGM Volume: %lf\nSE Volume: %lf\n", current_bgm_vol, current_se_vol);
	*prev_bgm_vol = current_bgm_vol;
	*prev_se_vol = current_se_vol;
	fclose(OptionsFile);
}