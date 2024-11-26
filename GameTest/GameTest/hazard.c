#pragma once
#include "globals.h"

// Helper function to check if a point is inside the square
bool is_point_in_square(Point p, Point squareTopLeft, float width, float height) {
    return (p.x >= squareTopLeft.x && p.x <= squareTopLeft.x + width &&
        p.y >= squareTopLeft.y && p.y <= squareTopLeft.y + height);
}

// Helper function to calculate orientation
int orientation(Point a, Point b, Point c) {
    float val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0) return 0;  // Collinear
    return (val > 0) ? 1 : 2; // Clockwise or counterclockwise
}

// Function to check if two line segments intersect
bool lines_intersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) return true;

    return false;
}

// Main collision detection function
bool check_square_triangle_collision(Point squareTopLeft, float width, float height,
    Point A, Point B, Point C) {
    // Check if any triangle vertex is inside the square
    if (is_point_in_square(A, squareTopLeft, width, height) ||
        is_point_in_square(B, squareTopLeft, width, height) ||
        is_point_in_square(C, squareTopLeft, width, height)) {
        return true;
    }

    // Check if any square edge intersects with any triangle edge
    Point squarePoints[4] = {
        squareTopLeft,
        {squareTopLeft.x + width, squareTopLeft.y},
        {squareTopLeft.x + width, squareTopLeft.y + height},
        {squareTopLeft.x, squareTopLeft.y + height}
    };

    // Define square edges
    Point squareEdges[4][2] = {
        {squarePoints[0], squarePoints[1]},
        {squarePoints[1], squarePoints[2]},
        {squarePoints[2], squarePoints[3]},
        {squarePoints[3], squarePoints[0]}
    };

    // Check all triangle edges with square edges
    Point triangleEdges[3][2] = { {A, B}, {B, C}, {C, A} };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (lines_intersect(squareEdges[i][0], squareEdges[i][1],
                triangleEdges[j][0], triangleEdges[j][1])) {
                return true;
            }
        }
    }

    return false;
}

int crusher_direction(hazard crusher, Player p) {

		// assign player center xy coordinates
		float px = (p.x + p.width / 2);
		float py = (p.y + p.height / 2);
		// how much the player goes into the platform
		float leftcoll = (fabs)((px + p.width/2) - crusher.x[0]);
		float rightcoll = (fabs)(crusher.x[1] - (px - p.width / 2));
		float topcoll = (fabs)((py + p.height/2) - crusher.y[0]);
		float botcoll = (fabs)(crusher.y[2] - (py - p.height / 2));

		// return lower value for horizontal/vertical
		float horizontal = (leftcoll < rightcoll) ? leftcoll : rightcoll;
		float vertical = (topcoll < botcoll) ? topcoll : botcoll;

		// horizontal = 1, vertical = 2
		return (horizontal < vertical) ? 1 : 2;
	}

void reset_hazards() {
    for (int i = 0; i < MAX_TRAPS; i++) {
        for (int j = 0; j < 3; j++) {
            traps[i].x[j] = 0;
            traps[i].y[j] = 0;
        }
        traps[i].type = 0;
    }
}

void init_hazards(FILE* HazardFile, int numberoftraps) {
    for (int hazard_index = 0; hazard_index < numberoftraps; ++hazard_index) {
        fscanf_s(HazardFile, "%d", &traps[hazard_index].type);
        switch (traps[hazard_index].type)
        {
        case spike:
        {
			for (int coordinate_index = 0; coordinate_index < 3; ++coordinate_index)
			{
				fscanf_s(HazardFile, "%f %f", &traps[hazard_index].x[coordinate_index], &traps[hazard_index].y[coordinate_index]);
			}
			fscanf_s(HazardFile, "%d", &traps[hazard_index].direction);
			break;
            
        }
        case crusher:
        {
            for (int coordinate_index = 0; coordinate_index < 3; ++coordinate_index)
            {
                fscanf_s(HazardFile, "%f %f", &traps[hazard_index].x[coordinate_index], &traps[hazard_index].y[coordinate_index]);
            }
            break;
        }
        case shooter:
        {
            for (int coordinate_index = 0; coordinate_index < 3; ++coordinate_index)
            {
                fscanf_s(HazardFile, "%f %f", &traps[hazard_index].x[coordinate_index], &traps[hazard_index].y[coordinate_index]);
            }
            break;
        }
        }
    }
}

void hazard_update() {
	//declaring top left of square for square triangle collision
	Point squareTopLeft = { player.x, player.y };
	for (int trap_index = 0; trap_index < numberoftraps; trap_index++) {
		switch (traps[trap_index].type) {
		case spike: {
			Point A = { traps[trap_index].x[0], traps[trap_index].y[0] };
			Point B = { traps[trap_index].x[1], traps[trap_index].y[1] };
			Point C = { traps[trap_index].x[2], traps[trap_index].y[2] };

			if (check_square_triangle_collision(squareTopLeft, player.width, player.height, A, B, C)) {
				Kill_Player(&player, Burn, &deaths);
			}
			break;
		}
		case crusher: {
			float crushervely;
			//CRUSHER MOVEMENT
			// crusherstate = 0 if going down and crusherstate = 1 when going up
			if (crusherstate == 0) {
				crushervely = 1000.f * dt;
				traps[trap_index].y[2] += crushervely;
				if (traps[trap_index].y[2] > traps[trap_index].y[1]) {
					traps[trap_index].y[2] = traps[trap_index].y[1];
					crusherstate = 1;
				}
				// if the player hits the crusher at the bottom, add death and reset
				if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[trap_index].x[0], traps[trap_index].y[0], traps[trap_index].x[1] - traps[trap_index].x[0], traps[trap_index].y[2] - traps[trap_index].y[0]) && !(player.state == Dead || player.state == Spawning)) {
					if (crusher_direction(traps[trap_index], player) == 1) {
						if (player.x >= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
							player.x = traps[trap_index].x[1];
							player.movex = 0;
						}
						else if (player.x <= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
							player.x = traps[trap_index].x[0] - player.width;
							player.movex = 0;
						}
					}
					else if (crusher_direction(traps[trap_index], player) == 2) {
						player.y = traps[trap_index].y[2];
						player.movey = 0;
						if ((player.y + player.height + 30) >= traps[trap_index].y[1]) {
							player.y = (traps[trap_index].y[1] - player.height - 15);
							Kill_Player(&player, Crush, &deaths);
						}
					}

				}
			}
			if (crusherstate == 1) {
				crushervely = -100.0f * dt;
				traps[trap_index].y[2] += crushervely;
				if (traps[trap_index].y[2] < traps[trap_index].y[0]) {
					traps[trap_index].y[2] = traps[trap_index].y[0];
					crusherstate = 0;
				}
				//if the player hits the crusher as it goes up, no death but cant go in
				if (AreSquaresIntersecting(player.x, player.y, player.width, player.height, traps[trap_index].x[0], traps[trap_index].y[0], traps[trap_index].x[1] - traps[trap_index].x[0], traps[trap_index].y[2] - traps[trap_index].y[0]) && !(player.state == Dead || player.state == Spawning)) {
					if (crusher_direction(traps[trap_index], player) == 1) {
						if (player.x >= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
							player.x = traps[trap_index].x[1];
							player.movex = 0;
						}
						else if (player.x <= ((traps[trap_index].x[1] + traps[trap_index].x[0]) / 2.f)) {
							player.x = traps[trap_index].x[0] - player.width;
							player.movex = 0;
						}
					}
					else if (crusher_direction(traps[trap_index], player) == 2) {
						player.y = traps[trap_index].y[2];
						player.movey = 0;
					}

				}
			}
			break;
		}
		default: {}
		}
	}
}