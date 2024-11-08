//---------------------------------------------------------
// file:	platform.c
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Definition of functions used in platformer logic
//			- Movement
//			- Transitioning movestates
//			- Assigning platform properties
//			- Collision logic
//			- Render logic
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "platform.h"


// MOVEMENT AND MOVESTATE LOGIC
#pragma region Movement and movestate
// 
void start_transition(Player* player, MoveState* movestate) {
	if (*movestate == OnPlat && player->onplatform == 1 && player->withinx == 1) {
		*movestate = EnterPlat;
		player->movex = 0;
	}
	else if (*movestate == InPlat && player->y > 200 && player->leaveplatform == 1) {
		*movestate = ExitPlat;
		player->y -= 50;
		player->movex = 0;
	}
	if (*movestate == EnterPlat || *movestate == ExitPlat) {
		player->transition_time = 0;
	}
}

void end_transition(Player* player, MoveState* movestate) {
	switch (*movestate) {
	case EnterPlat:
		player->y += 50;
		*movestate = InPlat;
		player->state = Idle;
		player->direction = InDown;
		break;
	case ExitPlat:
		*movestate = OnPlat;
		player->state = Idle;
		player->direction = OnRight;
		player->onplatform = 1;
		break;
	case InPlat:
	case OnPlat:
		player->transition_time = 13;
		break;
	}
}

void movement(Player* ply, MoveState movestate, float speed, float dt) {
	float accel = 3000.0f * dt;
	float accel2 = 1500.0f * dt;
	float speed2 = speed/2;

	PlayerDirection tempdirV = Null;
	PlayerDirection tempdirH = Null;

	// OnPlat movement
	if (movestate == OnPlat) {
		tempdirH = Null;
		// Gravity and terminal velocity
		if (ply->movey < 1000.0f && ply->onplatform == 0) {
			ply->movey += 1800.0f * dt;
		}
		// jump!
		if (CP_Input_KeyTriggered(KEY_SPACE) && ply->onplatform == 1) {
			ply->movey = -speed;
		}
		// left
		if (CP_Input_KeyDown(KEY_A) && !(CP_Input_KeyDown(KEY_D)) && (ply->movex > -speed)) {
			ply->movex -= accel;
			tempdirH = OnLeft;
		}
		else if ((ply->movex < 100.f) && (!(CP_Input_KeyDown(KEY_A))|| ((CP_Input_KeyDown(KEY_A) && CP_Input_KeyDown(KEY_D))))) {
			ply->movex += accel;
		};
		// right
		if (CP_Input_KeyDown(KEY_D) && !(CP_Input_KeyDown(KEY_A)) && (ply->movex < speed)) {
			ply->movex += accel;
			tempdirH = OnRight;
		}
		else if ((ply->movex > 100.f) && (!(CP_Input_KeyDown(KEY_A)) || ((CP_Input_KeyDown(KEY_A) && CP_Input_KeyDown(KEY_D))))) {
			ply->movex -= accel;
		};
		if (((((CP_Input_KeyDown(KEY_A) && CP_Input_KeyDown(KEY_D))) || !((CP_Input_KeyDown(KEY_A)) || (CP_Input_KeyDown(KEY_D)))) && (ply->movex <= 100.f && ply->movex >= -100.f))) {
			ply->movex = 0;
		}

		// Speed clamp
		ply->movex = CP_Math_ClampFloat(ply->movex, -speed, speed);

		// anim state
		if (tempdirH != Null) {
			ply->direction = tempdirH;
		}

		if (ply->onplatform == 1) {
			ply->state = Idle;
			if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_D)) {
				ply->state = Running;
			}
		}
		if (ply->onplatform == 0) {
			ply->state = Jumping;
		}
	}
	// InPlat movement
	if (movestate == InPlat) {
		ply->state = Idle;

		// WASD keys
		// W - Up
		if (CP_Input_KeyDown(KEY_W) && !CP_Input_KeyDown(KEY_S)) {
			ply->state = Crawling;
			tempdirV = InUp;
			if (ply->movey > -speed2) {
				ply->movey -= accel2;
			}
		}
		// A - Left
		if (CP_Input_KeyDown(KEY_A) && !CP_Input_KeyDown(KEY_D)) {
			ply->state = Crawling;
			tempdirH = InLeft;
			if (ply->movex > -speed2) {
				ply->movex -= accel2;
			}
		}
		// S - Down
		if (CP_Input_KeyDown(KEY_S) && !CP_Input_KeyDown(KEY_W)) {
			ply->state = Crawling;
			tempdirV = InDown;
			if (ply->movey < speed2) {
				ply->movey += accel2;

			}
		}
		// D - Right
		if (CP_Input_KeyDown(KEY_D) && !CP_Input_KeyDown(KEY_A)) {
			ply->state = Crawling;
			tempdirH = InRight;
			if (ply->movex < speed2) {
				ply->movex += accel2;
			}
		}

		// Idle decel and brake
		if (tempdirH == Null) {
			if (ply->movex < 0) {
				ply->movex += accel2;
			}
			else {
				ply->movex -= accel2;
			}
			if (ply->movex <= 50.f && ply->movex >= -50.f) {
				ply->movex = 0;
			}
		}
		if (tempdirV == Null) {
			if (ply->movey < 50) {
				ply->movey += accel2;
			}
			else if (ply->movey > 50) {
				ply->movey -= accel2;
			}
			if (ply->movey <= 50.f && ply->movey >= -50.f) {
				ply->movey = 0;
			}
		}

		// Speed clamps
		ply->movex = CP_Math_ClampFloat(ply->movex, -speed2, speed2);
		ply->movey = CP_Math_ClampFloat(ply->movey, -speed2, speed2);

		// Direction
		// Diagonal check if there are both horizontal and vertical inputs
		if (tempdirH != Null && tempdirV != Null) {
			if (tempdirV == InUp) {
				if (tempdirH == InLeft) {
					ply->direction = InUpLeft;
				}
				else {
					ply->direction = InUpRight;
				}
			}
			else {
				if (tempdirH == InLeft) {
					ply->direction = InDownLeft;
				}
				else {
					ply->direction = InDownRight;
				}
			}
		}
		// If no inputs, or only one directional input
		if (tempdirV == Null && tempdirH == Null) {
			ply->direction = ply->direction;
		} 
		else if (tempdirV == Null) {
			ply->direction = tempdirH;
		}
		else if (tempdirH == Null) {
			ply->direction = tempdirV;
		}

	}

	// Update movement only if InPlat/OnPlat, ignore transition
	if (movestate == InPlat || movestate == OnPlat) {
		ply->x += ply->movex * dt;
		ply->y += ply->movey * dt;
	}

	// reset checks for jumping and entering
	ply->onplatform = 0;
	ply->withinx = 0;
	ply->leaveplatform = 0;
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

	void platform_onblocks(int plat_index, int block_index, float x1, float y1, float x2, float y2) {
		Platform[plat_index].onblocks[block_index] = (platblocks){ x1-15, x2+15, y1-15, y2+15, (x2+15) - (x1-15), (y2+15) - (y1-15) };
	}

	// assigns platform horizontal hitboxes, used in hitbox function below
	void horizontalhitbox(int coord_index, platform* plt) {
		// Left to right
		if (plt->platcoords[coord_index + 1].x > plt->platcoords[coord_index].x) {		// if next coordx > current coordx
			plt->inblocks[coord_index] = (platblocks){
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
			plt->inblocks[coord_index] = (platblocks){
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
				plt->inblocks[coord_index] = (platblocks){
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
				plt->inblocks[coord_index] = (platblocks){
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
			plt->inblocks[coord_index] = (platblocks){
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
		float px = (p.x + p.width / 2);
		float py = (p.y + p.height / 2);
		// how much the player goes into the platform
		float leftcoll = (abs)((px + p.width/2) - plt.x1);
		float rightcoll = (abs)(plt.x2 - (px - p.width/2));
		float topcoll = (abs)((py + p.height/2) - plt.y1);
		float botcoll = (abs)(plt.y2 - (py - p.height/2));

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
		if (p.y + p.height / 2 < plt.y1+plt.h/2) { // is player above platform
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
	void CollPlatforms(int plat_index, platform plt, Player* pply, Player ply) {
		// iterate through number of coordinates
		for (int coord_index = 0; coord_index < Platform[plat_index].numcoords; coord_index++) {
			// check for collision
			if (collision(Platform[plat_index].inblocks[coord_index], ply)) {
				// check if collision is from vertical and not horizontal
				if (collision_direction(Platform[plat_index].inblocks[coord_index], ply) == 2) { // if colliding vertically
					if (ply.x < Platform[plat_index].inblocks[coord_index].x2 && ply.x + ply.width > Platform[plat_index].inblocks[coord_index].x1) {  // if player within x coords of platform 

						pply->y = fix_collide_y(Platform[plat_index].inblocks[coord_index], ply);

						// check player within X corners of platform
						if (ply.x >= Platform[plat_index].inblocks[coord_index].x1 && ply.x + ply.width <= Platform[plat_index].inblocks[coord_index].x2) {
							pply->withinx = 1;
							// check player touching top surface of platform to exit
							if ((coord_index%2==0) && (pply->y == Platform[plat_index].inblocks[coord_index].y2)) {
								pply->leaveplatform = 1;
							}
						}

						if ((pply->y + ply.height) == Platform[plat_index].inblocks[coord_index].y1) {
							pply->onplatform = 1; // on platform!
						}
						//---------------------------------------------------------
						// two conditions to set player vertical velocity 0:
						// 1) if player is on top of platform and travelling downwards
						// 2) if player bumps bottom of platform and travelling upwards
						//---------------------------------------------------------
						if (ply.y < Platform[plat_index].inblocks[coord_index].y1 && ply.movey > 0 || ply.y > Platform[plat_index].inblocks[coord_index].y1 && ply.movey < 0) { pply->movey = 0; }

						// end if player within x coords of platform
					}
					// end if colliding vertically
				}
				// if player is outside x coords of platform
				// aka player is colliding from the sides
				else 
					pply->x = fix_collide_x(Platform[plat_index].inblocks[coord_index], ply);
				if (collision_direction(Platform[plat_index].inblocks[coord_index], ply) == 1) {
					pply->movex = 0;
				}
			}// end if collision check
		}
	}
	void CollOnPlatforms(int plat_index, platform plt, Player* pply, Player ply) {
		// iterate through number of coordinates
		for (int block_index = 0; block_index < Platform[plat_index].numblocks; block_index++) {
			// check for collision
			if (collision(Platform[plat_index].onblocks[block_index], ply)) {
				// check if collision is from vertical and not horizontal
				if (collision_direction(Platform[plat_index].onblocks[block_index], ply) == 2) { // if colliding vertically
					if (ply.x < Platform[plat_index].onblocks[block_index].x2 && ply.x + ply.width > Platform[plat_index].onblocks[block_index].x1) {  // if player within x coords of platform 

						pply->y = fix_collide_y(Platform[plat_index].onblocks[block_index], ply);

						// check player within X corners of platform
						if (ply.x >= Platform[plat_index].onblocks[block_index].x1+30 && ply.x + ply.width <= Platform[plat_index].onblocks[block_index].x2-30) {
							pply->withinx = 1;
						}

						if ((pply->y + ply.height) == Platform[plat_index].onblocks[block_index].y1) {
							pply->onplatform = 1; // on platform!
						}
						//---------------------------------------------------------
						// two conditions to set player vertical velocity 0:
						// 1) if player is on top of platform and travelling downwards
						// 2) if player bumps bottom of platform and travelling upwards
						//---------------------------------------------------------
						if (ply.y < Platform[plat_index].onblocks[block_index].y1 && ply.movey > 0 || ply.y > Platform[plat_index].onblocks[block_index].y1 && ply.movey < 0) { pply->movey = 0; }

						// end if player within x coords of platform
					}
					// end if colliding vertically
				}
				// if player is outside x coords of platform
				// aka player is colliding from the sides
				else pply->x = fix_collide_x(Platform[plat_index].onblocks[block_index], ply);
				if (collision_direction(Platform[plat_index].onblocks[block_index], ply) == 1) {
					pply->movex = 0;
				}
				
			}// end if collision check
		}
	}

	void Reset_Player(Player* player, MoveState* movestate, int* deaths) {
		player->x = spawnx;
		player->y = spawny;
		*movestate = OnPlat;
		player->state = Idle;
		player->direction = OnRight;
		player->movey = 0;
		*deaths += 1;
	}
#pragma endregion



// RENDER
/*---------------------------------------------------------------------------------------------------*/
#pragma region Render

void DrawPlatform(int plat_index) {
	CP_Graphics_BeginShape();
	for (int i = 0; i <= Platform[plat_index].numcoords - 1; i++) {
		CP_Graphics_AddVertex(Platform[plat_index].platcoords[i].x, Platform[plat_index].platcoords[i].y);
	}
	CP_Graphics_EndShape();
}

void Draw_Player(CP_Image player_sprite, Player player, MoveState movestate) {
	// Normal Player
	float sprite_x, sprite_y;
	float rot = 0;
	if (movestate == InPlat || movestate == OnPlat) {
		switch (player.direction) {
		case OnLeft:
			sprite_y = 0;
			break;
		case OnRight:
			sprite_y = 180;
			break;
		case InUp:
			sprite_y = 360;
			rot = 0;
			break;
		case InUpLeft:
			sprite_y = 360;
			rot = 315;
			break;
		case InLeft:
			sprite_y = 360;
			rot = 270;
			break;
		case InDownLeft:
			sprite_y = 360;
			rot = 225;
			break;
		case InDown:
			sprite_y = 360;
			rot = 180;
			break;
		case InDownRight:
			sprite_y = 360;
			rot = 135;
			break;
		case InRight:
			sprite_y = 360;
			rot = 90;
			break;
		case InUpRight:
			sprite_y = 360;
			rot = 45;
			break;
		}
		switch (player.state) {
		case Idle:
			sprite_x = (player.anim < 10) ? 0 : 180;
			break;
		case Running:
		case Crawling:
			sprite_x = ((int)player.anim % 2 == 0) ? 360 : 540;
			break;
		case Jumping:
			sprite_x = 720;
			break;
		}
		CP_Image_DrawSubImageAdvanced(player_sprite, player.x - 15, player.y - 15, player.width + 30, player.height + 30, sprite_x + 5, sprite_y + 5, sprite_x + 170, sprite_y + 170, 255, rot);
	}
	else 
	{
		sprite_y = (movestate == EnterPlat) ? 360 : 0;

		sprite_x = ((int)player.transition_time / 1) * 180;
		CP_Image_DrawSubImage(transition_sprite, player.x - 15, player.y - 15, player.width + 30, (player.height + 30) * 2, sprite_x + 5, sprite_y + 5, sprite_x + 170, sprite_y + 350, 255);
	}
}

void Draw_Player_Transitioning_Sprite(Player player) {
	float transit_sprite_x, transit_sprite_y;
	transit_sprite_y = (movestate == EnterPlat) ? 360 : 0;

	transit_sprite_x = ((int)player.transition_time / 1) * 180;
	CP_Image_DrawSubImage(transition_sprite, player.x - 15, player.y - 15, player.width + 30, (player.height + 30) * 2, transit_sprite_x + 5, transit_sprite_y + 5, transit_sprite_x + 170, transit_sprite_y + 350, 255);

}

#pragma endregion