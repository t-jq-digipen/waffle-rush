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

void Credits_Init(void) {

	//load font
	exo = CP_Font_Load("Assets/Exo2-Regular.ttf");
	CP_Font_Set(exo);

	// load credits file

	FILE* file = fopen("Assets/Credits.txt", "r");
	for (int i = 0; i < num_names; i++) {
		fgets(names[i].name, sizeof(names[i].name), file);
	}
	fclose(file);

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

	// back button color
	CP_Color button_color = CP_Color_Create(200, 60, 60, 255);

	//background
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));

	//header text
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(150);
	CP_Font_DrawText("CREDITS:", (float)width / 2.0f, (float)100);

	CP_Settings_TextSize(100);
	char credits[500] = { 0 };

	for (int i = 0; i < num_names; i++) {
		sprintf_s(credits, sizeof(credits), names[i].name);
		CP_Font_DrawText(credits, (float)width / 2.0f, (float)(250 + (i * 100)));
	}

	CP_Settings_TextSize(150);

	//back to main menu button
	TextBoxCreate((float)(width / 2), (float)(height - 100), 400.0f, 150.0f, "BACK", button_color);
	int ExitClick = IsAreaClicked((float)(width / 2), (float)(height - 100), 400.0f, 150.0f, mouseX, mouseY); //if mouse is over button
	if (ExitClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect((float)(width / 2), (float)(height - 100), 400.0f, 150.0f);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); //return to main menu
		}
	}

	// Back to main menu
	if (CP_Input_KeyDown(KEY_Q)) { CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit); };

}

void Credits_Exit(void) {

}