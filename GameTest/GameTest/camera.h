#pragma once
//---------------------------------------------------------
// file:	credits.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Declarations of game stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "mainmenu.h"
#include "utils.h"
#include "platform.h"



typedef struct camera {
	float limitleftx;
	float limitrightx;
	float width;
	float height;
	float limittop;
	float limitbase;
	float worldshiftx;
	float worldshifty;
} Camera;

Camera camera;