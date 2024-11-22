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
	else if (*movestate == InPlat && player->leaveplatform == 1) {
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
	float speed2 = speed / 2;

	PlayerDirection tempdirV = Null;
	PlayerDirection tempdirH = Null;

	if (!(player.state == Dead || player.state == Spawning)) {
		// OnPlat movement
		if (movestate == OnPlat) {
			tempdirH = Null;
			// Gravity and terminal velocity
			if (ply->movey < 1000.0f && ply->onplatform == 0) {
				ply->movey += 1800.0f * dt;
			}

			// left
			if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT) && (!CP_Input_KeyDown(KEY_D) || !CP_Input_KeyDown(KEY_RIGHT)) && (ply->movex > -speed)) {
				ply->movex -= accel;
				tempdirH = OnLeft;
			}

			// right
			if (CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT) && (!CP_Input_KeyDown(KEY_A) || !CP_Input_KeyDown(KEY_LEFT)) && (ply->movex < speed)) {
				ply->movex += accel;
				tempdirH = OnRight;
			}
			if (tempdirH == Null) {
				if (ply->movex < -100) {
					ply->movex += accel;
				}
				else if (ply->movex > 100) {
					ply->movex -= accel;
				}
			}
			if (((((CP_Input_KeyDown(KEY_A) && CP_Input_KeyDown(KEY_D))) || ((CP_Input_KeyDown(KEY_LEFT) && CP_Input_KeyDown(KEY_RIGHT))) || tempdirH == Null) && (ply->movex <= 100.f && ply->movex >= -100.f))) {
				ply->movex = 0;
			}

			if (ply->walljump == 1) {
				ply->movex = CP_Math_ClampFloat(ply->movex, -speed, speed);
			}
			else {
				ply->movex = CP_Math_ClampFloat(ply->movex, -1.5f * speed, 1.5f * speed);
			}
			// anim state
			if (tempdirH != Null) {
				ply->direction = tempdirH;
			}

			if (ply->onplatform == 1) {
				ply->walljump = 1;
				ply->state = Idle;
				if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_LEFT) || CP_Input_KeyDown(KEY_RIGHT)) {
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
			if ((CP_Input_KeyDown(KEY_W) || CP_Input_KeyDown(KEY_UP)) && (!CP_Input_KeyDown(KEY_S) || !CP_Input_KeyDown(KEY_DOWN))) {
				ply->state = Crawling;
				tempdirV = InUp;
				if (ply->movey > -speed2) {
					ply->movey -= accel2;
				}
			}
			// A - Left
			if ((CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT)) && (!CP_Input_KeyDown(KEY_D) || !CP_Input_KeyDown(KEY_RIGHT))) {
				ply->state = Crawling;
				tempdirH = InLeft;
				if (ply->movex > -speed2) {
					ply->movex -= accel2;
				}
			}
			// S - Down
			if ((CP_Input_KeyDown(KEY_S) || CP_Input_KeyDown(KEY_DOWN)) && (!CP_Input_KeyDown(KEY_W) || !CP_Input_KeyDown(KEY_UP))) {
				ply->state = Crawling;
				tempdirV = InDown;
				if (ply->movey < speed2) {
					ply->movey += accel2;

				}
			}
			// D - Right
			if ((CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT)) && (!CP_Input_KeyDown(KEY_A) || !CP_Input_KeyDown(KEY_LEFT))) {
				ply->state = Crawling;
				tempdirH = InRight;
				if (ply->movex < speed2) {
					ply->movex += accel2;
				}
			}

			// Idle decel and brake
			if (tempdirH == Null) {
				if (ply->movex < -80) {
					ply->movex += accel2;
				}
				else if (ply->movex > 80){
					ply->movex -= accel2;
				}
				if (ply->movex <= 80.f && ply->movex >= -80.f) {
					ply->movex = 0;
				}
			}
			if (tempdirV == Null) {
				if (ply->movey < -80) {
					ply->movey += accel2;
				}
				else if (ply->movey > 80) {
					ply->movey -= accel2;
				}
				if (ply->movey <= 80.f && ply->movey >= -80.f) {
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
}



void play_music(Audio *audio, float seconds) {

	if (audio->playing == 0) {
		CP_Sound_Play(audio->music); 
		audio->playing = 1;       
		audio->duration = seconds;         
	}

	if (audio->playing == 1) {
		audio->duration -= 1 * CP_System_GetDt();
		printf("%lf\n", audio->duration);
	}
	if (audio->duration <= 0) {
		audio->playing = 0;
		audio->duration = seconds;
	}

}
//stop music after level;
//void stop_music(Audio* audio, CP_Sound songname) {
//	audio->playing = 0;
//	audio->duration = 7;
// CP_Sound_Free(songname);
//}

void jump(Player* player, MoveState movestate) {
	if (CP_Input_KeyTriggered(KEY_SPACE) && movestate == OnPlat) {
		if (((player->onplatform == 0) && player->state == Sliding && player->walljump == 1)) {
			player->walljump = 0;
			player->movey = -500;
			if (player->direction == OnLeft) {
				player->movex = 800;
			}
			else if (player->direction == OnRight) {
				player->movex = -800;
			}
		}
		if (player->onplatform) {
			player->movey = -600;
		}
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

	// Reset platform members
	void reset_platforms(void) {
		for (int plat_index = 0; plat_index < max_platforms; plat_index++) {
			Platform[plat_index].numcoords = 0;
			Platform[plat_index].numblocks = 0;
			for (int coord_index = 0; coord_index < max_coords; coord_index++) {
				Platform[plat_index].platcoords[coord_index] = (coords){ 0,0 };
				Platform[plat_index].inblocks[coord_index] = (platblocks){ 0,0,0,0 };
				Platform[plat_index].onblocks[coord_index] = (platblocks){ 0,0,0,0 };
			}
		}
	}

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

	// Checkpoints
	void reset_checkpoints(Checkpoint* checkpoint) {
		for (int checkpoint_index = 0; checkpoint_index < max_checkpoints; checkpoint_index++) {
			checkpoint->coordinates[checkpoint_index] = (coords){ 0,0 };
		}
		checkpoint->active = 0;
	}

	void reset_walls(platblocks* walls) {
		for (int wall_index = 0; wall_index < max_walls; wall_index++) {
			walls[wall_index] = (platblocks){ 0,0,0,0,0,0 };
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
				if (!(pply->onplatform) && pply->x == Platform[plat_index].onblocks[block_index].x2 && CP_Input_KeyDown(KEY_A) && !(CP_Input_KeyDown(KEY_D))) {
					pply->state = Sliding;
					if (pply->movey >= 400) {
						pply->movey = 400;
					}

				}
				else if (!(pply->onplatform) && pply->x + pply->width == Platform[plat_index].onblocks[block_index].x1 && CP_Input_KeyDown(KEY_D) && !(CP_Input_KeyDown(KEY_A))) {
					pply->state = Sliding;
					if (pply->movey >= 400) {
						pply->movey = 400;
					}

				}
				else if (!(pply->onplatform)){
					pply->state = Jumping;
				}
				else if (pply->onplatform && pply->state == Jumping) {
					pply->state = Idle;
				}
				
			}// end if collision check
		}
	}
	void CollWalls(platblocks walls[], int num, Player* pply, Player ply, MoveState movestate) {
		movestate = movestate;

		// iterate through number of coordinates
		for (int wall_index = 0; wall_index < numwalls; wall_index++) {
			// check for collision
			if (collision(walls[wall_index], ply)) 
			{
				// check if collision is from vertical and not horizontal
				if (collision_direction(walls[wall_index], ply) == 2) { // if colliding vertically
					if (ply.x < walls[wall_index].x2 && ply.x + ply.width > walls[wall_index].x1) {  // if player within x coords of platform 

						pply->y = fix_collide_y(walls[wall_index], ply);

						// check player within X corners of platform
						/*if (ply.x >= walls[wall_index].x1 + 30 && ply.x + ply.width <= walls[wall_index].x2 - 30) {
							pply->withinx = 1;
						}*/

						if ((pply->y + ply.height) == walls[wall_index].y1) {
							pply->onplatform = 1; // on platform!
						}
						//---------------------------------------------------------
						// two conditions to set player vertical velocity 0:
						// 1) if player is on top of platform and travelling downwards
						// 2) if player bumps bottom of platform and travelling upwards
						//---------------------------------------------------------
						if (ply.y < walls[wall_index].y1 && ply.movey > 0 || ply.y > walls[wall_index].y1 && ply.movey < 0) { pply->movey = 0; }

						// end if player within x coords of platform
					}
					// end if colliding vertically
				}
				// if player is outside x coords of platform
				// aka player is colliding from the sides
				else {
					pply->x = fix_collide_x(walls[wall_index], ply);
					if ((collision_direction(walls[wall_index], ply) == 1)) {
						pply->movex = 0;
					}
					if (movestate == OnPlat) {
						if (pply->x == walls[wall_index].x2 && CP_Input_KeyDown(KEY_A) && !(CP_Input_KeyDown(KEY_D))) {
							pply->state = Sliding;
							if (pply->movey >= 400) {
								pply->movey = 400;
							}

						}
						else if (pply->x + pply->width == walls[wall_index].x1 && CP_Input_KeyDown(KEY_D) && !(CP_Input_KeyDown(KEY_A))) {
							pply->state = Sliding;
							if (pply->movey >= 400) {
								pply->movey = 400;
							}

						}
						else if (!(pply->onplatform)) {
							pply->state = Jumping;
						}
					}
				}
			}// end if collision check
		}

		if (pply->onplatform && (pply->state == Jumping || pply->state == Sliding)) {
			pply->state = Idle;
		}
	}
#pragma endregion

// PLAYER STATES
/*---------------------------------------------------------------------------------------------------*/
#pragma region Player State Changes
	//---------------------------------------------------------
	// Init player
	// Kill player
	// Spawn player
	// Reset player
	// Transition player
	//---------------------------------------------------------
	void Init_Player(float* speed, Player* player, int* deaths) {
		// initialize base speed
		*speed = 600;

		// initialize player struct
		// size
		player->width = 20;
		player->height = 20;
		// position
		player->x = spawn.x;
		player->y = spawn.y;
		// movement
		player->movex = 0;
		player->movey = 0;
		player->walljump = 0;
		// anim timers
		player->transition_time = 15;
		player->death_time = 17;
		player->spawn_time = 0;
		// health
		player->stamina = 100.0f;
		// anim state
		player->state = Spawning;
		player->anim = 0;
		player->direction = OnRight;

		// initialize deaths
		*deaths = 0;
	}

	void Kill_Player(Player* player, CauseOfDeath deathcause, int* deaths) {
		if (player->state != Dead) {
			player->state = Dead;
			player->deathcause = deathcause;
			player->death_time = 0;
			player->spawn_time = 0;
			player->movey = (deathcause != Fall) ? 0 : -300;
			++*deaths;
			}
	}

	void Spawn_Player(Player* player, coords spawn,  Camera* camera, CP_Sound music) {
		// Player changes
		player->x = spawn.x;
		player->y = spawn.y;
		player->movex = 0;
		player->movey = 0;
		player->state = Spawning;
		// Camera changes
		camera->prespawnX = camera->limitleftx;
		camera->prespawnY = camera->limittop;
		camera->respawnX = spawn.x - camera->width / 2;
		camera->respawnY = spawn.y - 3 * camera->height / 4;
		play_music(&deathsound, 0.005);
	}

	void Reset_Player(Player* player, MoveState* movestate) {
		player->stamina = 100;
		player->movey = -100;
		*movestate = OnPlat;
		player->state = Idle;
		player->anim = 0;
		player->direction = OnRight;
		player->deathcause = Reset;
	}

	void Transition_Player(Player *player, MoveState *movestate) {
		if (movestate == OnPlat || movestate == InPlat) {
			start_transition(player, movestate);
		}

		if (player->transition_time >= 13) {
			end_transition(player, movestate);
		}
	}
#pragma endregion


// TIMER
/*---------------------------------------------------------------------------------------------------*/
#pragma region TIMERS
	void player_anim_updates(Player* player, float dt, MoveState* movestate, Camera* camera, coords spawn, CP_Sound music) {
		player->anim += 10 * dt;
		if (player->anim >= 20) {
			player->anim = 0;
		}
		if (player->transition_time < 13) {
			player->transition_time += 20 * dt;
			if (*movestate == EnterPlat) {
				camera->limitbase += 100 * dt;
				camera->limittop += 100 * dt;
			}
			if (*movestate == ExitPlat) {
				camera->limitbase -= 100 * dt;
				camera->limittop -= 100 * dt;
			}
		}
		if (player->transition_time > 13) {
			player->transition_time = 13;
		}
		if (player->death_time < 17) {
			if (player->deathcause != Fall) {
				player->death_time += 17 * dt;
			}
			else if (player->y <= camera->limitbase + 400) {
				player->movey += 1800.0f * dt;
				player->y += player->movey * dt;
			}
			else player->death_time = 17;
		}
		if (player->death_time >= 17) {
			if (player->state == Dead) {
				Spawn_Player(player, spawn, camera, music);
				player->movex = 0;
			}
			player->death_time = 17;
		}
		if (player->spawn_time < 34 && player->state == Spawning) {
			player->spawn_time += 34 * dt;
			camera->limitleftx = camera->prespawnX - (player->spawn_time * ((camera->prespawnX - camera->respawnX)/34));
			camera->limitrightx = camera->prespawnX + camera->width - (player->spawn_time * ((camera->prespawnX - camera->respawnX) / 34));
			camera->limittop = camera->prespawnY - (player->spawn_time * ((camera->prespawnY - camera->respawnY) / 34));
			camera->limitbase = camera->prespawnY + camera->height - (player->spawn_time * ((camera->prespawnY - camera->respawnY) / 34));
		}
		if (player->spawn_time > 34 && player->state == Spawning) {
			Reset_Player(player, movestate);
			player->spawn_time = 34;
		}
	}
#pragma endregion


// RENDER
/*---------------------------------------------------------------------------------------------------*/
#pragma region Render

void DrawPlatform(int plat_index) {
	CP_Graphics_BeginShape();
	for (int i = 0; i <= Platform[plat_index].numcoords - 1; i++) {
		CP_Graphics_AddVertex(Platform[plat_index].platcoords[i].x -camera.worldshiftx, Platform[plat_index].platcoords[i].y - camera.worldshifty);
	}
	CP_Graphics_EndShape();
}

void DrawGoal(CP_Image goal_sprite, float goalx, float goaly, Camera camera, float dt, float clear_timer) {
	if (!isCleared) {
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, 0, 0, 32, 32, 255);
	}
	else {
		if (clear_timer <= 3) {
			clear_timer += 6 * dt;
		}
		int sprite_x = 32 + (32 * (int)clear_timer);
		CP_Image_DrawSubImage(goal_sprite, goalx - camera.worldshiftx, goaly - camera.worldshifty, 100, 100, sprite_x, 0, sprite_x + 32, 32, 255);
	}
}

void Draw_Player(CP_Image player_sprite, Player player, MoveState movestate) {
	// Normal Player
	float draw_posx = player.x - 15 - camera.worldshiftx;
	float draw_posy = player.y - 15 - camera.worldshifty;
	float sprite_x = 0;
	float sprite_y = 0;
	float rot = 0;
	if (!(player.state == Dead || player.state == Spawning)) {
		if (movestate == InPlat || movestate == OnPlat) {
			switch (player.direction) {
			case OnLeft:
				sprite_y = 0;
				break;
			case OnRight:
				sprite_y = 18;
				break;
			case InUp:
				sprite_y = 36;
				rot = 0;
				break;
			case InUpLeft:
				sprite_y = 36;
				rot = 315;
				break;
			case InLeft:
				sprite_y = 36;
				rot = 270;
				break;
			case InDownLeft:
				sprite_y = 36;
				rot = 225;
				break;
			case InDown:
				sprite_y = 36;
				rot = 180;
				break;
			case InDownRight:
				sprite_y = 36;
				rot = 135;
				break;
			case InRight:
				sprite_y = 36;
				rot = 90;
				break;
			case InUpRight:
				sprite_y = 36;
				rot = 45;
				break;
			}
			switch (player.state) {
			case Idle:
				sprite_x = (player.anim < 10) ? 0 : 18;
				break;
			case Running:
			case Crawling:
				sprite_x = ((int)player.anim % 2 == 0) ? 36 : 54;
				break;
			case Jumping:
				sprite_x = 72;
				break;
			case Sliding:
				sprite_x = 90;
				break;
			}
			CP_Image_DrawSubImageAdvanced(player_sprite, draw_posx, draw_posy, player.width + 30, player.height + 30, sprite_x + 1, sprite_y + 1, sprite_x + 17, sprite_y + 17, 255, rot);
		}
		else
		{
			sprite_y = (movestate == EnterPlat) ? 36 : 0;

			sprite_x = ((int)player.transition_time / 1) * 18;
			CP_Image_DrawSubImage(transition_sprite, draw_posx, draw_posy, player.width + 30, (player.height + 30) * 2, sprite_x + 1, sprite_y + 1, sprite_x + 17, sprite_y + 35, 255);
		}
	}
	else if (player.state == Dead) {
		CP_Image death_sprite;
		switch (player.deathcause) {
		case Burn: {
			death_sprite = burn_sprite;

			switch (player.direction) {
			case OnLeft: sprite_y = 0;
				break;
			case OnRight: sprite_y = 36;
				break;
			default: sprite_y = 0;
			}
			sprite_x = ((int)player.death_time) * 18;
			if (!player.onplatform) {
				sprite_x += 306;
			}
			break;
		}
		case Fall: 
			death_sprite = fall_sprite;
			sprite_x = ((int)(player.anim) % 2 == 0) ? 0 : 18;
			sprite_y = 0;
			break;

		case Crush: 
			death_sprite = crush_sprite;
			sprite_x = ((int)player.death_time < 3) ? (int)player.death_time * 18 : 54;
			sprite_y = (player.direction == OnLeft) ? 36 : 0;
			break;
		
		default:
			death_sprite = burn_sprite;
		
		}

		CP_Image_DrawSubImage(death_sprite, draw_posx, draw_posy, player.width + 30, (player.height + 30) * 2, sprite_x + 1, sprite_y + 1, sprite_x + 17, sprite_y + 35, 255);
	}
}



#pragma endregion