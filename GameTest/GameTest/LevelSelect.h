//---------------------------------------------------------
// file:	LevelSelect.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Declaration of various level stages and features
//
// Copyright � 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once

//#include "Level2.h"
#include "mainmenu.h"
#include "platform.h"

// leevl struct
typedef struct level {
	int level_no;
	int isUnlocked;
	int isCleared;
	int score;
	int time;
} level;

void LSelect_Init(void);
void LSelect_Update(void);
void LSelect_Exit(void);


void Game_Init(void);
void Game_Update(void);
void Game_Exit(void);


void L1_Init(void);
void L1_Update(void);
void L1_Exit(void);

void L2_Init(void);
void L2_Update(void);
void L2_Exit(void);