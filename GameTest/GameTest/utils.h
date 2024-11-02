//---------------------------------------------------------
// file:	utils.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Declaration of utility functions
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Utility globals
float mouseX, mouseY;


// check if position is within a rectangle
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int IsAreaCornerClicked(float area_corner_x, float area_corner_y, float area_width, float area_height, float click_x, float click_y);

// check if position is within a circle
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

// check if two circles intersect
int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

// check if two squares intersect
int AreSquaresIntersecting(float s1_x, float s1_y, float s1_w, float s1_h, float s2_x, float s2_y, float s2_w, float s2_h);

// check if position is within a triangle
int IsTriangleClicked(float tri_x1, float tri_y1, float tri_x2, float tri_y2, float tri_x3, float tri_y3, float click_x, float click_y);

// create a red box with text within
void TextBoxCreate(float pos_x, float pos_y, float size_x, float size_y, const char* text, CP_Color color);

// draw full hp bar
void DrawStatBar(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float hp, float maxhp);

// draw segmented hp bar
void DrawHPBar2(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float hp, float maxhp);

// check if square collides with triangle
int SquareTriangleCollide(float squarex, float squarey, float squarewidth, float squareheight, float tri1x, float tri1y, float tri2x, float tri2y, float tri3x, float tri3y);

// update score
void AddScore(int *totalScore, int addScore, int *isCleared);
