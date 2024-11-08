//---------------------------------------------------------
// file:	platform.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Declaration of global variables and functions 
//			used in platformer logic
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "mainmenu.h"
#include "platform.h"
#include "Level2.h"


#define max_coords 16
#define max_platforms 20

// Platformer globals
typedef enum MoveState {
	OnPlat,
	InPlat,
	EnterPlat,
	ExitPlat,
} MoveState;

MoveState movestate;

// Level Variables
int numplats;
int deaths;
float speed; //speed
float width, height; //window width/height
float spawnx, spawny, goalx, goaly; // spawn and goal position
int isPaused; // check if game paused
int isCleared; // check if level cleared (player touches goal)
int levelScore; // level score
int highScore; // level high score
int levelNumber; // level number

// Graphics
CP_Image player_sprite, transition_sprite;

float player_sprite_x, player_sprite_y;
float transit_sprite_x, transit_sprite_y;

// Anim timer
float transit_anim;


CP_Font text;
CP_Color bg, pausebg, black, red, green, plat, stamina; //colors


// STRUCTS --------------------------------------------
#pragma region Structs

	// Time Struct
	#pragma region Timer
		typedef struct Timer {
			float minutes, seconds, millis;
		} Timer;

		Timer timer;
	#pragma endregion


	// Platform Struct
	#pragma region Platform Structs
		// InPlat
		// coordinate struct
		typedef struct coords {
			float x, y;
		} coords;

		// platform hitbox struct
		typedef struct platblocks {
			float x1, x2, y1, y2, w, h;
		} platblocks;

		// platform struct
			//---------------------------------------------------------
			// Platforms are constructed by two parts: InPlat and OnPlat "hitboxes"
			// InPlat: The hitboxes are constructed by walls based on coordinates
				// The first hitbox is always horizontal
				// For horizontal hitboxes:
				//	Offset -15 for both left and right
				//	Offset +15 for both top and bottom
				// For vertical hitboxes:
				//	Offset +15 for both left and right
				//	Offset +15 for both top and bottom
				// This ensures the collision logic will not have conflicts when corners are touched by the player
			// OnPlat: The hitboxes are constructed by larger blocks, this is to prevent the
			//		   higher speed in OnPlat movestate from making the player sink into the platform
				// The platforms are constructed as a series of vertical blocks
			// 
			// Text file format:
				// Number of platforms in a level (e.g 2)
					// Number of coordinates for platform (e.g 6)
						// Coordinate x and y values, starting from a top left point, going in clockwise manner
					// coord1_x coord1_y
					// coord2_x coord2_y
					// ...
					// coord6_x coord6_y
					// 
					// Number of blocks for platform (e.g 2)
						// Coordinates x1 y1 x2 y2, where x1 y1 is the top left, and x2 y2 is the bottom right
					// block1_x1 block1_y1 block1_x2 block1_y2
				// Repeat entering coordinates for all platforms
			//---------------------------------------------------------
		typedef struct platform {
			int numcoords; // Number of coordinates in a platform
			int numblocks; // Number of blocks that make up the platform
			coords platcoords[max_coords]; // Array of coords struct
			platblocks inblocks[max_coords]; // Wall hitboxes
			platblocks onblocks[max_coords]; // Big block hitboxes
		} platform;

		// platform array
		platform Platform[max_platforms];

		// coordinate struct
	#pragma endregion Platform Structs


	// Player Struct
	#pragma region Player
		typedef enum PlayerState {
			Idle,
			Running,
			Jumping,
			Crawling,
		} PlayerState;
		
		typedef enum PlayerDirection {
			OnLeft,
			OnRight,
			InUp,
			InDown,
			InLeft,
			InRight,
			InUpLeft,
			InUpRight,
			InDownLeft,
			InDownRight,
			Null,
		} PlayerDirection;

		typedef struct Player {
			float x; //posx
			float y; //posy
			float movex; //movex
			float movey; //movey
			float stamina; //stamina for InPlat
			float width; //width
			float height; //height
			float transition_time; //transition timer
			int onplatform;
			int withinx;
			int leaveplatform;
			CP_Color color; //color
			PlayerState state;
			PlayerDirection direction;
			float anim; //sprite animation timer
		} Player;

		Player player; //player 
	#pragma endregion Player Struct

#pragma endregion



// FUNCTIONS --------------------------------------------
#pragma region Functions

	// MOVEMENT AND MOVESTATE LOGIC
	#pragma region Movement and movestate
		// set movestate
		void start_transition(Player* player, MoveState* movestate);
		void end_transition(Player* player, MoveState* movestate);
		// platformer and in platform movements
		void movement(Player* p, MoveState movestate, float speed, float dt);
		#pragma endregion

	// HITBOX LOGIC (see platform.c)
	#pragma region Hitboxes
		// assign platform coordinates
		void platform_coordinates(int plat_index, int coord_index, float x, float y);
		// assign onblock coordinates
		void platform_onblocks(int plat_index, int block_index, float x1, float y1, float x2, float y2);
		// initialize horizontal hitbox
		void horizontalhitbox(int coord_index, platform* plt);
		// initialize vertical hitbox
		void verticalhitbox(int coord_index, platform* plt);
		// initialize all hitboxes for a platform
		void hitbox(int plat_index, int numplatcoords, platform* plt);
	#pragma endregion

	// COLLISION LOGIC (see platform.c)
	#pragma region Collision
		// if player and platform hitbox collides
		int collision(platblocks plt, Player p);

		int collision_direction(platblocks plt, Player p);

		float fix_collide_x(platblocks plt, Player p);
		float fix_collide_y(platblocks plt, Player p);

		void CollPlatforms(int plat_index, platform plt, Player* p, Player ply);

		void CollOnPlatforms(int plat_index, platform plt, Player* p, Player ply);
	#pragma endregion 
	
	// Reset Player
	void Reset_Player(Player* player, MoveState* movestate, int* deaths);

	// Draw Platforms
	void DrawPlatform(int plat_index);

	// Draw Player
	void Draw_Player(CP_Image player_sprite, Player player, MoveState movestate);

	void Draw_Player_Transitioning_Sprite(Player player);

#pragma endregion