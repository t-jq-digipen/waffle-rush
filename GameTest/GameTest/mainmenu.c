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

CP_Font exo;
CP_Color button_color;

void Main_Menu_Init(void)
{
	//load font
	exo = CP_Font_Load("Assets/Exo2-Regular.ttf");
	CP_Font_Set(exo);
	CP_Settings_TextSize(150);
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

	//background
	CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));


	//draw play button
	TextBoxCreate(width / 2, height / 3, 500, 150, "PLAY", button_color);
	int PlayClick = IsAreaClicked(width / 2, height / 3, 500, 150, mouseX, mouseY); //if mouse is over button
	if (PlayClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(width / 2, height / 3, 500, 150);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(LSelect_Init, LSelect_Update, LSelect_Exit); //go to game
		}
	}

	//test level button
	TextBoxCreate(3*width / 4, height / 3, 150, 150, "T", button_color);
	int TestClick = IsAreaClicked(3 * width / 4, height / 3, 150, 150, mouseX, mouseY); //if mouse is over button
	if (TestClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(3 * width / 4, height / 3, 150, 150);
		if (CP_Input_MouseClicked()) { //if LMB1 pressed
			CP_Engine_SetNextGameState(
				, Game_Update, Game_Exit); //go to test stage
		}
	}

	//credits button
	TextBoxCreate(width / 2, 5 * height / 9, 500, 150, "CREDITS", button_color);
	int CreditsClick = IsAreaClicked(width / 2, 5 * height / 9, 500, 150, mouseX, mouseY); //if mouse is over button
	if (CreditsClick) {
		CP_Settings_Fill(CP_Color_Create(255, 255, 255, 50)); //highlight button
		CP_Graphics_DrawRect(width / 2, 5 * height / 9, 500, 150);
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
}

void Main_Menu_Exit(void)
{

}
