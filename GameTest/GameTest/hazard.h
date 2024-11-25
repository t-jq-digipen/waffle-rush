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
#include "platform.h"
#define MAX_TRAPS 50
#define SQUARE_SIZE 10
#define SQUARE_SPEED 200
#define MAX_SQUARES 10

//Enum for the hazard types
typedef enum hazardstype {
	spike,
	crusher,
	shooter
}hazardstype;

typedef struct hazard {
	int x[3], y[3];
	int ori;
	hazardstype type;
}hazard;

// Structure for a 2D point
typedef struct Point {
	float x;
	float y;
} Point;

// Structure to represent a square
typedef struct {
    float x, y;
    bool active;
} Square;

Square squares[MAX_SQUARES]; // Array to store squares

CP_Image fire_sprite;
void Draw_Fire(CP_Image fire_sprite, hazard traps);


hazard traps[MAX_TRAPS];

int numberoftraps;
int crusherstate;
float bulletx, bullety;

bool is_point_in_square(Point p, Point squareTopLeft, float width, float height);
int orientation(Point a, Point b, Point c);
bool lines_intersect(Point p1, Point q1, Point p2, Point q2);
bool check_square_triangle_collision(Point squareTopLeft, float width, float height,
	Point A, Point B, Point C);

void reset_hazards(hazard* traps);