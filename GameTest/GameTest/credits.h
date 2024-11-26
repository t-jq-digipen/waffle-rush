//---------------------------------------------------------
// file:	credits.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Declarations of game stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "mainmenu.h"
#include "globals.h"

#define name_length 20

typedef struct credits {
	char name[name_length];
} credits;

void Credits_Init(void);
void Credits_Update(void);
void Credits_Exit(void);