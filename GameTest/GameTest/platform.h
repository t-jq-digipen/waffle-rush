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
#include "utils.h"
#include "hazard.h"
#include "camera.h"


#define max_coords 16
#define max_platforms 20
#define max_walls 20
#define max_checkpoints 4

// Platformer globals
typedef enum MoveState {
	OnPlat,
	InPlat,
	EnterPlat,
	ExitPlat,
} MoveState;

MoveState movestate;

typedef enum LevelState {
	NotPaused,
	Paused,
	InOptions,
	InQuit,
	Cleared,
	EndLevel
} LevelState;

LevelState levelstate;

//audio
typedef struct Audio {
	CP_Sound music;
	int playing;
	float duration;
}Audio;

Audio bgm1;
Audio bgm2;
Audio bgm3;
Audio deathsound;


void play_music(Audio *audio,float seconds);
//void stop_music(Audio* audio, CP_Sound songname);

// Level Variables
int numplats;
int numcheckpoints;
int deaths;
float speed; //speed
int width, height; //window width/height
float spawnx, spawny, goalx, goaly; // spawn and goal position
int isPaused; // check if game paused
int canMove;  //if player can move
int isCleared; // check if level cleared (player touches goal)
int levelScore; // level score
int highScore; // level high score
int levelNumber; // level number

// Graphics
CP_Image player_sprite, transition_sprite, spawn_sprite, burn_sprite, fall_sprite, crush_sprite, goal_sprite;

float playerLastX, playerLastY;   //last player coords recorded, mainly used in level transition sequence
float tutorialDataY, level1DataY, level2DataY, level3DataY; //display high score and time for each level
float player_sprite_x, player_sprite_y;
float transit_sprite_x, transit_sprite_y;
float death_sprite_x, death_sprite_y;
float clear_timer;

// Anim timer


CP_Font text;
CP_Color bg, pausebg, black, red, green, plat, wall, stamina; //colors


// STRUCTS --------------------------------------------
#pragma region Structs

	// Camera struct


	typedef struct Camera {
		float limitleftx;
		float limitrightx;
		float spawnX;
		float spawnY;
		float prespawnX, respawnX;
		float prespawnY, respawnY;
		float width;
		float height;
		float limittop;
		float limitbase;
		float worldshiftx;
		float worldshifty;
	} Camera;

	Camera camera;

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

		coords spawn;

		// Checkpoint
		typedef struct Checkpoint {
			coords coordinates[max_checkpoints];
			int active;
		} Checkpoint;

		Checkpoint checkpoint;


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

		int numwalls;
		platblocks walls[max_walls];


		// coordinate struct
	#pragma endregion Platform Structs


	// Player Struct
	#pragma region Player
		typedef enum PlayerState {
			Idle,
			Running,
			Jumping,
			Sliding,
			Crawling,
			Dead,
			Spawning,
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

		typedef enum CauseOfDeath {
			Burn,
			Fall,
			Crush,
			Reset,
		} CauseOfDeath;

		typedef struct Player {
			float x; //posx
			float y; //posy
			float movex; //movex
			float movey; //movey
			float stamina; //stamina for InPlat
			float width; //width
			float height; //height

			// Animation timers
			float transition_time;
			float death_time;
			float spawn_time;

			// Condition checks
			int onplatform;
			int walljump;
			int withinx;
			int leaveplatform;
			PlayerState state;
			PlayerDirection direction;
			CauseOfDeath deathcause;
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
		// jump >:)
		void jump(Player* player, MoveState movestate);
		#pragma endregion

	// HITBOX LOGIC (see platform.c)
	#pragma region Hitboxes
		// PLATFORMS
		// reset platform members
		void reset_platforms(void);
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

		// CHECKPOINTS
		// reset checkpoint members and set active checkpoint to 0th coordinates
		void reset_checkpoints(Checkpoint* checkpoint);
		// reset walls
		void reset_walls(platblocks* walls);
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

		void CollWalls(platblocks walls[], int num, Player* pply, Player ply, MoveState movestate);
	#pragma endregion 
	
	// Player States
	void Init_Player(float* speed, Player* player, int* deaths);
	void Kill_Player(Player* player, CauseOfDeath deathcause, int* deaths);
	void Spawn_Player(Player* player, coords spawn, Camera* camera, CP_Sound music);
	void Reset_Player(Player* player, MoveState* movestate);
	void Transition_Player(Player* player, MoveState* movestate);

	// Anim Updates
	void player_anim_updates(Player* player, float dt, MoveState* movestate, Camera* camera, coords spawn);

	// Draw Platforms
	void DrawPlatform(int plat_index);


	// Draw Goal

	// Draw Player
	void Draw_Player(CP_Image player_sprite, Player player, MoveState movestate);



#pragma endregion