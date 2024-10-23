#pragma once
#include "platform.h"


// Coordinates (NOTE: to be changed to read from text file)
void platform_coordinates(int plat_index, int coord_index, float x, float y) {
	Platform[plat_index].platcoords[coord_index] = (coords){ x,y };
}

// MOVEMENT AND MOVESTATE LOGIC
#pragma region Movement and movestate
int movestate_set(Player *p, int movestate) {
	// 3 conditions to allow phasing into platform
	// movestate == 0 : platformer
	// pply->onplatform == 1 : on platform
	// pply->withinx == 1 : within x of platform
	if (movestate == 0 && p->onplatform == 1 && p->withinx == 1) {
		p->y += 80;
		return 1;
	}
	// 3 conditions to allow exiting platform
	// movestate == 1 : topdown
	// pply->y > 200 : player will not bust above the window of screen
	// pply->leaveplatform == 1 : player is touching top edge of the platform
	else if (movestate == 1 && p->y > 200 && p->leaveplatform == 1) {
		p->y -= 80;
		return 0;
	}
	return movestate;
}

int start_transition(Player* ply, int movestate) {
	if (movestate == 0 && ply->onplatform == 1 && ply->withinx == 1) {
		movestate = 2;
		ply->movex = 0;
	}
	else if (movestate == 1 && ply->y > 200 && ply->leaveplatform == 1) {
		movestate = 3;
		ply->movex = 0;
	}
	return movestate;
}

//int end_transition(Player* ply, float dt, int movestate) {
//	if (switchtime < 1) {
//		if ((movestate == 2 && switchtime < 0.5) || (movestate == 3 && switchtime >= 0.5)) {
//			ply->movey += 800 * dt;
//		}
//		if ((movestate == 3 && switchtime < 0.5) || (movestate == 2 && switchtime >= 0.5)) {
//			ply->movey -= 800 * dt;
//		}
//		switchtime += 2 * dt;
//	}
//	if (switchtime >= 1) {
//		if (movestate == 2) {
//			movestate = 1;
//		}
//		if (movestate == 3) {
//			movestate = 0;
//		}
//	}
//	return movestate;
//}

void movement(Player* p, int movestate, float speed) {
	if (movestate == 0) { // platformer
		p->movex = 0; p->movey += 10;
		//if (CP_Input_KeyDown(KEY_W)) { pply->movey = -speed; }; //temp fly
		if (CP_Input_KeyTriggered(KEY_SPACE) && p->onplatform == 1) { p->movey = -speed; } // jump!
		if (CP_Input_KeyDown(KEY_A)) { p->movex = -speed; };
		if (CP_Input_KeyDown(KEY_D)) { p->movex = speed; };
		
	}
	if (movestate == 1) { // top down/inside platform
		p->movex = 0; p->movey = 0;
		if (CP_Input_KeyDown(KEY_W)) { p->movey = -speed; };
		if (CP_Input_KeyDown(KEY_A)) { p->movex = -speed; };
		if (CP_Input_KeyDown(KEY_S)) { p->movey = speed; };
		if (CP_Input_KeyDown(KEY_D)) { p->movex = speed; };
		
	}
}

#pragma endregion


// HITBOX LOGIC
/*---------------------------------------------------------------------------------------------------*/
#pragma region Hitboxes

// assigns platform horizontal hitboxes, used in hitbox function below
void horizontalhitbox(int coord_index, platform* plt) {
	if (plt->platcoords[coord_index + 1].x > plt->platcoords[coord_index].x) {		// if next coordx > current coordx
		plt->blocks[coord_index] = (platblocks){
			plt->platcoords[coord_index].x + 10,
			plt->platcoords[coord_index + 1].x - 10,
			plt->platcoords[coord_index].y - 10,
			plt->platcoords[coord_index].y + 10,
			(plt->platcoords[coord_index + 1].x - 10) - (plt->platcoords[coord_index].x + 10),
			20
		};
	}
	else {																			// if next coordx < current coordx
		plt->blocks[coord_index] = (platblocks){		
			plt->platcoords[coord_index + 1].x + 10,
			plt->platcoords[coord_index].x - 10,
			plt->platcoords[coord_index].y - 10,
			plt->platcoords[coord_index].y + 10,
			(plt->platcoords[coord_index].x - 10) - (plt->platcoords[coord_index + 1].x + 10),
			20
		};
	}
}

// assigns platform vertical hitboxes, used in hitbox function below
void verticalhitbox(int coord_index, platform* plt) {
	if (coord_index != plt->numcoords - 1) {										// if not last coord
		if (plt->platcoords[coord_index+1].y > plt->platcoords[coord_index].y) {	// if next coordy > current coordy
			plt->blocks[coord_index] = (platblocks){
				plt->platcoords[coord_index].x - 10,
				plt->platcoords[coord_index].x + 10,
				plt->platcoords[coord_index].y - 10,
				plt->platcoords[coord_index + 1].y + 10,
				20,
				(plt->platcoords[coord_index + 1].y + 10) - (plt->platcoords[coord_index].y - 10)
			};
		}
		else {																		// if next coordy < current coordy 
			plt->blocks[coord_index] = (platblocks){
				plt->platcoords[coord_index].x - 10,
				plt->platcoords[coord_index].x + 10,
				plt->platcoords[coord_index + 1].y - 10,
				plt->platcoords[coord_index].y + 10,
				20,
				(plt->platcoords[coord_index].y + 10) - (plt->platcoords[coord_index+1].y - 10)
			};

		}
	}
	else {																			// if last coord
			plt->blocks[coord_index] = (platblocks){
			plt->platcoords[0].x - 10,
			plt->platcoords[coord_index].x + 10,
			plt->platcoords[0].y - 10,
			plt->platcoords[coord_index].y + 10,
			20,
			(plt->platcoords[coord_index].y + 10) - (plt->platcoords[0].y - 10)
		};
	}
}

//---------------------------------------------------------
// determines all hitboxes of platforms, use in a loop with number of total platforms per level
// e.g :
//		for (int plat_index = 0; plat_index < numplats; plat_index++) {
//			hitbox(plat_index, Platform[plat_index].numcoords, &Platform[plat_index]);
//		}
//---------------------------------------------------------
void hitbox(int plat_index, int numplatcoords, platform* plt) {

	for (int coord_index = 0; coord_index < Platform[plat_index].numcoords; coord_index++) {
		if (coord_index % 2 == 0 && coord_index < Platform[plat_index].numcoords - 1) { //horizontal & not last coord
			horizontalhitbox(coord_index, &Platform[plat_index]);
		}
		else { //vertical
			verticalhitbox(coord_index, &Platform[plat_index]);
		}
	}
}
#pragma endregion


// COLLISION LOGIC
/*---------------------------------------------------------------------------------------------------*/
#pragma region Collision and repositions

// am i colliding
int collision(platblocks plt, Player p) {
	float pyh = p.y + p.height;
	float pxw = p.x + p.width;
	float pltx2 = plt.x1 + plt.w;
	float plty2 = plt.y1 + plt.h;
	return (pxw >= plt.x1 && p.x <= pltx2 && pyh >= plt.y1 && p.y <= plty2);
}

// checks if collision is vertical or horizontal
int collision_direction(platblocks plt, Player p) {

	// assign player x2 and y2 coordinates
	float px2 = (p.x + p.width);
	float py2 = (p.y + p.height);
	// how much the player goes into the platform
	float leftcoll = px2 - plt.x1;
	float rightcoll = plt.x2 - p.x;
	float topcoll = py2 - plt.y1;
	float botcoll = plt.y2 - p.y;

	// return lower value for horizontal/vertical
	float horizontal = (leftcoll < rightcoll) ? leftcoll : rightcoll;
	float vertical = (topcoll < botcoll) ? topcoll : botcoll;

	// horizontal = 1, vertical = 2
	return (horizontal < vertical) ? 1 : 2;
}

// fix x position
float fix_collide_x(platblocks plt, Player p) {
	if (p.x + p.width / 2 < plt.x1 + plt.w / 2) { // is player left of platform
		return plt.x1 - p.width; // throw left of platform
	}
	return plt.x1 + plt.w; // throw right of platform
}

// fix y position
float fix_collide_y(platblocks plt, Player p) {
	if (p.y + p.height / 2 < plt.y1) { // is player above platform
		return plt.y1 - p.height; // throw up of platform
	}
	return plt.y2; // throw bottom of platform
}

//---------------------------------------------------------
// fixes player position in relation to platform hitboxes
// e.g:
//		for (int plat_index = 0; plat_index < numplats; plat_index++) {
//			CollPlatforms(plat_index, Platform[plat_index].numcoords, Platform[plat_index], &pply, pply);
//		}
//---------------------------------------------------------
void CollPlatforms(int plat_index, int numplatcoords, platform plt, Player* pply, Player ply) {
	// iterate through number of coordinates
	for (int coord_index = 0; coord_index < Platform[plat_index].numcoords; coord_index++) {
		// check for collision
		if (collision(Platform[plat_index].blocks[coord_index], ply)) {
			// check if collision is from vertical and not horizontal
			if (collision_direction(Platform[plat_index].blocks[coord_index], ply) == 2) { // if colliding vertically
				if (ply.x < Platform[plat_index].blocks[coord_index].x2 && ply.x + ply.width > Platform[plat_index].blocks[coord_index].x1) {  // if player within x coords of platform 

					pply->y = fix_collide_y(Platform[plat_index].blocks[coord_index], ply);

					// check player within X corners of platform
					if (ply.x >= Platform[plat_index].blocks[coord_index].x1 && ply.x + ply.width <= Platform[plat_index].blocks[coord_index].x2) {
						pply->withinx = 1;
						// check player touching top surface of platform to exit
						if (pply->y == Platform[plat_index].blocks[coord_index].y2 && pply->y <= Platform[plat_index].blocks[coord_index].y2 + 50) {
							pply->leaveplatform = 1;
						}
					}

					if ((pply->y + ply.height) == Platform[plat_index].blocks[coord_index].y1) {
						pply->onplatform = 1; // on platform!
					}
					//---------------------------------------------------------
					// two conditions to set player vertical velocity 0:
					// 1) if player is on top of platform and travelling downwards
					// 2) if player bumps bottom of platform and travelling upwards
					//---------------------------------------------------------
					if (ply.y < Platform[plat_index].blocks[coord_index].y1 && ply.movey > 0 || ply.y > Platform[plat_index].blocks[coord_index].y1 && ply.movey < 0) { pply->movey = 0; }

					// end if player within x coords of platform
				}
				// end if colliding vertically
			}
			// if player is outside x coords of platform
			// aka player is colliding from the sides
			else pply->x = fix_collide_x(Platform[plat_index].blocks[coord_index], ply);
		}// end if collision check
	}
}
#pragma endregion

void DrawPlatform(int plat_index) {
	CP_Graphics_BeginShape();
	for (int i = 0; i <= Platform[plat_index].numcoords - 1; i++) {
		CP_Graphics_AddVertex(Platform[plat_index].platcoords[i].x, Platform[plat_index].platcoords[i].y);
	}
	CP_Graphics_EndShape();
}