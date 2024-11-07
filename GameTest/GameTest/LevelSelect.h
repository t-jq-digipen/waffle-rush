//---------------------------------------------------------
// file:	game.h
// author:	TAN JUN QUAN
// email:	t.junquan@digipen.edu
//
// brief:	Declarations of game stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "game.h"
#include "Level1.h"

// time struct
typedef struct time {
	int minutes;
	int seconds;
	int millis;
} time;

// leevl struct
typedef struct level {
	int level_no;
	int isUnlocked;
	int isCleared;
	int score;
	struct time time;
} level;

void LSelect_Init(void);
void LSelect_Update(void);
void LSelect_Exit(void);
