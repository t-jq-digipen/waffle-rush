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
#include <stdbool.h>
#define MAX_TRAPS 100

//
typedef enum hazardstype {
	spike,
	crusher,
}hazardstype;

typedef struct hazard {
	int x[3], y[3];
	hazardstype type;
}hazard;

// Structure for a 2D point
typedef struct Point {
	float x;
	float y;
} Point;

hazard traps[MAX_TRAPS];

int numberoftraps;

bool is_point_in_square(Point p, Point squareTopLeft, float width, float height);
int orientation(Point a, Point b, Point c);
bool lines_intersect(Point p1, Point q1, Point p2, Point q2);
bool check_square_triangle_collision(Point squareTopLeft, float width, float height,
	Point A, Point B, Point C);