#pragma once
#include "platform.h"


// MOVEMENT AND MOVESTATE LOGIC
#pragma region Movement and movestate
// 
int start_transition(Player* ply, MoveState movestate) {
	if (movestate == OnPlat && ply->onplatform == 1 && ply->withinx == 1) {
		movestate = EnterPlat;
		ply->movex = 0;
	}
	else if (movestate == InPlat && ply->y > 200 && ply->leaveplatform == 1) {
		movestate = ExitPlat;
		ply->movex = 0;
	}
	return movestate;
}

void movement(Player* p, MoveState movestate, float speed) {
	if (movestate == OnPlat) { // platformer
		p->movex = 0; p->movey += 10;
		//if (CP_Input_KeyDown(KEY_W)) { pply->movey = -speed; }; //temp fly
		if (CP_Input_KeyTriggered(KEY_SPACE) && p->onplatform == 1) { p->movey = -speed; } // jump!
		if (CP_Input_KeyDown(KEY_A)) { p->movex = -speed; };
		if (CP_Input_KeyDown(KEY_D)) { p->movex = speed; };
		
	}
	if (movestate == InPlat) { // inside platform
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
	//---------------------------------------------------------
	// Platforms are constructed as a series of "walls" named "hitboxes" that construct the platform, 
	// they are hollow to facilitate traversal within the platform
	// Construction logic is as follows:
	// Coordinates start from a top left corner, and go in a clockwise direction
	// The first hitbox is always horizontal
	// For horizontal hitboxes:
	//	Offset -15 for both left and right
	//	Offset +15 for both top and bottom
	// For vertical hitboxes:
	//	Offset +15 for both left and right
	//	Offset +15 for both top and bottom
	// This ensures the collision logic will not have conflicts when corners are touched by the player
	//---------------------------------------------------------

	// Initializes platform coordinates
	void platform_coordinates(int plat_index, int coord_index, float x, float y) {
		Platform[plat_index].platcoords[coord_index] = (coords){ x,y };
	}

	// assigns platform horizontal hitboxes, used in hitbox function below
	void horizontalhitbox(int coord_index, platform* plt) {
		// Left to right
		if (plt->platcoords[coord_index + 1].x > plt->platcoords[coord_index].x) {		// if next coordx > current coordx
			plt->blocks[coord_index] = (platblocks){
				plt->platcoords[coord_index].x + 15,
				plt->platcoords[coord_index + 1].x - 15,
				plt->platcoords[coord_index].y - 15,
				plt->platcoords[coord_index].y + 15,
				(plt->platcoords[coord_index + 1].x - 15) - (plt->platcoords[coord_index].x + 15),
				30
			};
		}
		// Right to left
		else {																			// if next coordx < current coordx
			plt->blocks[coord_index] = (platblocks){
				plt->platcoords[coord_index + 1].x + 15,
				plt->platcoords[coord_index].x - 15,
				plt->platcoords[coord_index].y - 15,
				plt->platcoords[coord_index].y + 15,
				(plt->platcoords[coord_index].x - 15) - (plt->platcoords[coord_index + 1].x + 15),
				30
			};
		}
	}

	// assigns platform vertical hitboxes, used in hitbox function below
	void verticalhitbox(int coord_index, platform* plt) {
		// Top to bottom
		if (coord_index != plt->numcoords - 1) {
			if (plt->platcoords[coord_index + 1].y > plt->platcoords[coord_index].y) {	
				plt->blocks[coord_index] = (platblocks){
					plt->platcoords[coord_index].x - 15,
					plt->platcoords[coord_index].x + 15,
					plt->platcoords[coord_index].y - 15,
					plt->platcoords[coord_index + 1].y + 15,
					30,
					(plt->platcoords[coord_index + 1].y + 15) - (plt->platcoords[coord_index].y - 15)
				};
			}
		// Bottom to top
			else {
				plt->blocks[coord_index] = (platblocks){
					plt->platcoords[coord_index].x - 15,
					plt->platcoords[coord_index].x + 15,
					plt->platcoords[coord_index + 1].y - 15,
					plt->platcoords[coord_index].y + 15,
					30,
					(plt->platcoords[coord_index].y + 15) - (plt->platcoords[coord_index + 1].y - 15)
				};

			}
		}
		// Last coordinate
		else {
			plt->blocks[coord_index] = (platblocks){
			plt->platcoords[0].x - 15,
			plt->platcoords[coord_index].x + 15,
			plt->platcoords[0].y - 15,
			plt->platcoords[coord_index].y + 15,
			30,
			(plt->platcoords[coord_index].y + 15) - (plt->platcoords[0].y - 15)
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
		// Go through all coordinates in a platform struct, and initialize all hitbox coordinates
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

		// assign player center xy coordinates
		float px = (p.x + p.width/2);
		float py = (p.y + p.height/2);
		// how much the player goes into the platform
		float leftcoll = (px - plt.x1);
		float rightcoll = (plt.x2 - px);
		float topcoll = (py - plt.y1);
		float botcoll = (plt.y2 - py);

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
							if (pply->y == Platform[plat_index].blocks[coord_index].y2) {
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