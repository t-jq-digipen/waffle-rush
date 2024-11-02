//---------------------------------------------------------
// file:	hazard.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Declaration of hazard 
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once

#define MAX_TRAPS 100

typedef enum hazardstype {
	spike,
}hazardstype;

typedef struct hazard {
	int x[3], y[3];
	hazardstype type;
}hazard;

hazard traps[MAX_TRAPS];

int numberoftraps;