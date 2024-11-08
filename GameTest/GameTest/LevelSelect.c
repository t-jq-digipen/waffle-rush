//---------------------------------------------------------
// file:	LevelSelect.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Level selector menu
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "LevelSelect.h"

CP_Font exo;
CP_Color button_color, button_color_locked, button_color_cleared;

#define num_levels 5
level levels[num_levels];

void LSelect_Init(void)
{
	//load font
	exo = CP_Font_Load("Assets/Exo2-Regular.ttf");
	CP_Font_Set(exo);

	CP_Settings_TextSize(150);

	// load save file

	FILE* file = fopen("Assets/LevelSelect.txt", "r");
	for (int i = 0; i < num_levels; i++) {
		fscanf_s(file, "%d %d %d %d %d\n", &levels[i].level_no, &levels[i].isUnlocked, &levels[i].isCleared, &levels[i].score, &levels[i].time);
		printf("%d %d %d %d %d\n", levels[i].level_no, levels[i].isUnlocked, levels[i].isCleared, levels[i].score, levels[i].time);
	}
	fclose(file);
}

void LSelect_Update(void)
{
	//assign window width and height
	int width = CP_System_GetWindowWidth();
	int height = CP_System_GetWindowHeight();

	//track pointer
	float mouseX = CP_Input_GetMouseX();
	float mouseY = CP_Input_GetMouseY();

	//background
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));

	//button color
	button_color = CP_Color_Create(200, 60, 60, 255);
	button_color_locked = CP_Color_Create(80, 80, 80, 255);
	button_color_cleared = CP_Color_Create(57, 255, 20, 255);

	//header text
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_DrawText("LEVEL SELECT", width / 2, 100);

	//level 1 button
	if (levels[0].isUnlocked == 1 && levels[0].isCleared == 1) {
		TextBoxCreate(width / 4, height / 3, 150, 150, "1", button_color_cleared);
	}
	else if (levels[0].isUnlocked == 0) {
		TextBoxCreate(width / 4, height / 3, 150, 150, "1", button_color_locked);
	}
	else {
		TextBoxCreate(width / 4, height / 3, 150, 150, "1", button_color);
	}
	int L1Click = IsAreaClicked(width / 4, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (levels[0].isUnlocked == 1) {
		if (L1Click) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 4, height / 3, 150, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(L1_Init, L1_Update, L1_Exit); //go to game
			}
		}
	}

	//level 2 button
	if (levels[1].isUnlocked == 1 && levels[1].isCleared == 1) {
		TextBoxCreate(width / 4 + 200, height / 3, 150, 150, "2", button_color_cleared);
	}
	else if (levels[1].isUnlocked == 0) {
		TextBoxCreate(width / 4 + 200, height / 3, 150, 150, "2", button_color_locked);
	}
	else {
		TextBoxCreate(width / 4 + 200, height / 3, 150, 150, "2", button_color);
	}
	int L2Click = IsAreaClicked(width / 4 + 200, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (levels[1].isUnlocked == 1) {
		if (L2Click) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 4 + 200, height / 3, 150, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(L2_Init, L2_Update, L2_Exit); //go to game
			}
		}
	}

	//level 3 button
	if (levels[2].isUnlocked == 1 && levels[2].isCleared == 1) {
		TextBoxCreate(width / 2, height / 3, 150, 150, "3", button_color_cleared);
	}
	else if (levels[2].isUnlocked == 0) {
		TextBoxCreate(width / 2, height / 3, 150, 150, "3", button_color_locked);
	}
	else {
		TextBoxCreate(width / 2, height / 3, 150, 150, "3", button_color);
	}
	int L3Click = IsAreaClicked(width / 2, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (levels[2].isUnlocked == 1) {
		if (L3Click) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 2, height / 3, 150, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(L1_Init, L1_Update, L1_Exit); //go to game
			}
		}
	}

	//level 4 button
	if (levels[3].isUnlocked == 1 && levels[3].isCleared == 1) {
		TextBoxCreate(width / 2 + 200, height / 3, 150, 150, "4", button_color_cleared);
	}
	else if (levels[3].isUnlocked == 0) {
		TextBoxCreate(width / 2 + 200, height / 3, 150, 150, "4", button_color_locked);
	}
	else {
		TextBoxCreate(width / 2 + 200, height / 3, 150, 150, "4", button_color);
	}
	int L4Click = IsAreaClicked(width / 2 + 200, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (levels[3].isUnlocked == 1) {
		if (L4Click) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(width / 2 + 200, height / 3, 150, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(L1_Init, L1_Update, L1_Exit); //go to game
			}
		}
	}

	//level 5 button
	if (levels[4].isUnlocked == 1 && levels[4].isCleared == 1) {
		TextBoxCreate(3 * width / 4, height / 3, 150, 150, "5", button_color_cleared);
	}
	else if (levels[4].isUnlocked == 0) {
		TextBoxCreate(3 * width / 4, height / 3, 150, 150, "5", button_color_locked);
	}
	else {
		TextBoxCreate(3 * width / 4, height / 3, 150, 150, "5", button_color);
	}
	int L5Click = IsAreaClicked(3 * width / 4, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (levels[4].isUnlocked == 1) {
		if (L5Click) {
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
			CP_Graphics_DrawRect(3 * width / 4, height / 3, 150, 150);
			if (CP_Input_MouseClicked()) { //if LMB1 pressed
				CP_Engine_SetNextGameState(L1_Init, L1_Update, L1_Exit); //go to game
			}
		}
	}

	//back to main menu button
	TextBoxCreate(width / 2, 2 * height / 3, 500, 150, "BACK", button_color);
	int ExitClick = IsAreaClicked(width / 2, 2 * height / 3, 500, 150, mouseX, mouseY); //if mouse is over button
	if (ExitClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(width / 2, 2 * height / 3, 500, 150);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); //return to main menu
		}
	}
}

void LSelect_Exit(void)
{

}