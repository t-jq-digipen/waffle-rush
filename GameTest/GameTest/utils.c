//---------------------------------------------------------
// file:	utils.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Compilation of utility functions used throughout
//			the assignment
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "utils.h"
#include "platform.h"

// check if a position is within a rectangle
//
// area_center_x - position x center of rectangle
// area_center_y - position y center of rectangle
// area_width - width of rectangle
// area_height - height of rectangle
// click_x - position x to be checked
// click_y - position y to be checked
//
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y) {
	if (click_x > (area_center_x - area_width / 2) && click_x < (area_center_x + area_width / 2) &&
		click_y >(area_center_y - area_height / 2) && click_y < (area_center_y + area_height / 2)) {
		return 1;
	}
	return 0;
}

int IsAreaCornerClicked(float area_corner_x, float area_corner_y, float area_width, float area_height, float click_x, float click_y) {
	if (click_x > (area_corner_x) && click_x < (area_corner_x + area_width) &&
		click_y >(area_corner_y) && click_y < (area_corner_y + area_height)) {
		return 1;
	}
	return 0;
}

//check if a position is within a circle
//
// circle_center_x - position x center of circle
// circle_center_y - position y center of circle
// diameter - width of rectangle
// click_x - position x to be checked
// click_y - position y to be checked
//
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	if ((click_y - circle_center_y) * (click_y - circle_center_y) + (click_x - circle_center_x) * (click_x - circle_center_x) < (diameter / 2) * (diameter / 2)) {
		return 1;
	}
	return 0;
}

//check if two circles are intersecting
// 
// c1_x, c1_y - circle 1 position x and y
// c2_x, c2_y - circle 2 position x and y
// c1_r, c2_r - radius of circle 1 and 2
//
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2)
{
	CP_Vector distance = CP_Vector_Set(c2_x - c1_x, c2_y - c1_y);
	if (CP_Vector_DotProduct(distance, distance) < (r1 + r2)*(r1+r2)) {
		return 1;
	}
	return 0;
}

// check if two squares are intersecting
//
// s1_x, s1_y, s1_w, s1_h - square 1 posx, posy, width, height
// s2_x, s2_y, s2_w, s2_h - square 2 posx, posy, width, height
//
int AreSquaresIntersecting(float s1_x, float s1_y, float s1_w, float s1_h, float s2_x, float s2_y, float s2_w, float s2_h) {
	float s1_cx = s1_x + s1_w / 2; // square 1 center x
	float s1_cy = s1_y + s1_h / 2; // square 1 center y
	float s2_cx = s2_x + s2_w / 2; // square 2 center x
	float s2_cy = s2_y + s2_h / 2; // square 2 center y
	float widthtouch = s1_w / 2 + s2_w / 2;
	float heighttouch = s1_h / 2 + s2_h / 2;
	if (abs(s2_cx - s1_cx) <= widthtouch && (abs(s2_cy - s1_cy) <= heighttouch)) {
		return 1;
	}
	return 0;
}

int IsTriangleClicked(float tri_x1, float tri_y1, float tri_x2, float tri_y2, float tri_x3, float tri_y3, float click_x, float click_y) {

}

// create red box with black text
// 
// pos_x - position x center of box
// pos_y - position y center of box
// size_x - width of box
// size_y - height of box
//
void TextBoxCreate(float pos_x, float pos_y, float size_x, float size_y, const char* text, CP_Color color) {
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_Fill(color);
	CP_Graphics_DrawRect(pos_x, pos_y, size_x, size_y);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText(text, pos_x, pos_y);
}

// draw full stat bar
// 
// pos_x1, pos_y1 - position of top left corner
// pos_x2, pos_y2 - position of bottom right corner
// stat - current health
// max_stat - max health
//
void DrawStatBar(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float stat, float max_stat) {
	// assign total width, height, and width of health bar
	float width, height, statwidth;
	//width and height of total bar
	width = pos_x2 - pos_x1;
	height = pos_y2 - pos_y1;
	//width of current stat
	statwidth = width * stat / max_stat;
	// draw the rectangle
	CP_Graphics_DrawRect(pos_x1, pos_y1, statwidth, height);
}
// draw segmented stat bar
// 
// pos_x1, pos_y1 - position of top left corner
// pos_x2, pos_y2 - position of bottom right corner
// stat - current health
// max_stat - max health
//
void DrawHPBar2(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float hp, float maxhp) {
	//array of stat in 10% chunks
	int hpbar[10] = {0, maxhp / 10, 2 * maxhp / 10, 3 * maxhp / 10, 4 * maxhp / 10, 5 * maxhp / 10, 6 * maxhp / 10, 7 * maxhp / 10, 8 * maxhp / 10 , 9 * maxhp / 10}; //array 10% blocks
	//size of full bar
	float width = pos_x2 - pos_x1;
	float height = pos_y2 - pos_y1;
	//segment width and interval
	float segmentwidth = width / 10;
	float segmentinterval = 15;
	//for loop to check stat 10% chunks
	for (int i = 0; i < 10; ++i) {
		if (hp > (hpbar[i])+maxhp/20) { //if stat>10% interval, draw 10% chunk
			CP_Graphics_DrawRect(pos_x1 + (i * (segmentwidth + segmentinterval/10)), pos_y1 + 100, segmentwidth - segmentinterval, height);
		}

	}
}

int SquareTriangleCollide (float squarex, float squarey, float squarewidth, float squareheight, float tri1x,float tri1y, float tri2x, float tri2y, float tri3x, float tri3y) {
	// is Triangle corner in square?
	if ((IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri1x, tri1y))|| (IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri2x, tri2y))|| (IsAreaCornerClicked(squarex, squarey, squarewidth, squareheight, tri3x, tri3y))) {
		return 1;
	}


	return 0;
}

void AddScore(int *totalScore, int addScore, LevelState *levelstate) {
	if (*levelstate != Cleared) {
		*totalScore += addScore;
	}
}

int IncrementScore(int startScore, int endScore, float *timeElapsed, float dt) {
	*timeElapsed += dt;
	float duration = 5.0f;
	int score = 0;
	if (*timeElapsed < duration) {
		startScore = (int)((*timeElapsed / duration) * endScore);
	}
	else {
		startScore = endScore;
	}
	score = startScore;
	return score;
}

int TransitionInBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4) {
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);

	CP_Graphics_DrawRect(boxX1, boxY1, scaler1, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX2, boxY1, -scaler2, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX1, boxY1, boxX2 - boxX1, scaler3);
	CP_Graphics_DrawRect(boxX1, boxY2, boxX2 - boxX1, -scaler4);
}

int TransitionOutBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4) {
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_RectMode(CP_POSITION_CORNER);

	CP_Graphics_DrawRect(boxX1, boxY1, -scaler1, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX2, boxY1, scaler2, boxY2 - boxY1);
	CP_Graphics_DrawRect(boxX1, boxY1, boxX2 - boxX1, -scaler3);
	CP_Graphics_DrawRect(boxX1, boxY2, boxX2 - boxX1, scaler4);
}