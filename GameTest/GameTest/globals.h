//---------------------------------------------------------
// file:	globals.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:   Declaration of global variables and functions 
//			used in platformer logic
// 
//			Sections are broken down into:
//				- Globals
//					- Enums and Structs
//					- Variables
// 
//				- Functions
//					- Inits
//					- Updates
//					- Renders
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once
#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// GENERAL LEVEL GLOBALS
/*=====================================================================================================================*/
#pragma region 
#define num_levels 5
	// ENUMS AND STRUCTS
	/*
		Enums:
		- LevelState
	 
		Structs:
		- Camera
		- Timer
	*/
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		// LevelState enum
		typedef enum LevelState {
			NotPaused,
			Paused,
			InOptions,
			InQuit,
			Cleared,
			EndLevel
		} LevelState;


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
			float camshiftx;
			float camshifty;
		} Camera;

		// Timer struct
		typedef struct Timer {
			float seconds;
			int minutes, millis;
		} Timer;


		// time struct (for level)
		typedef struct time {
			int minutes;
			int seconds;
			int millis;
		} time;

		// Level struct
		typedef struct level {
			int level_no;
			int isUnlocked;
			int isCleared;
			int score;
			struct time time;
		} level;

		// Level Transition
		typedef struct EnterLevel
		{
			float posx, posy, height, width;
		} EnterLevel;

		typedef enum leveltrans {
			end = 1,
			start = 2,
			reset = 0
		}leveltrans;
	#pragma endregion Enums and Structs
	/*---------------------------------------------------------------------------------------------------------*/



	// VARIABLES
	//	
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		LevelState levelstate;

		Camera camera;

		Timer timer;

		// Level Variables
		int numplats;
		int numcheckpoints;

		int deaths;
		float speed; //speed

		float width, height; //window width/height

		float spawnx, spawny, goalx, goaly; // spawn and goal position

		int isPaused; // check if game paused
		int canMove;  //if player can move
		int isCleared; // check if level cleared (player touches goal)
		int levelScore; // level score
		int highScore; // level high score
		int levelNumber; // level number
		int frameCounter;  //counts frames between last touch of wall while sliding
		int walldir;		// direction for coyote time walljump
		bool wasSliding;  //checks is player was sliding
		int lastDir;  //saves last direction player was facing before wall jumping (for coyote time)
		int coyotetime;
		float timeElapsed;
		int levelEnded;
		int buffstate;

		float dt;

		// Text and colours
		CP_Font text;
		CP_Color bg, pausebg, black, red, green, plat, wall, stamina; //colors

		// Graphics
		float playerLastX, playerLastY;   //last player coords recorded, mainly used in level transition sequence
		float tutorialDataY, level1DataY, level2DataY, level3DataY, level4DataY, level5DataY; //display high score and time for each level
		float clear_timer;

		CP_Image goal_pointer, skull;

		// Mouse position
		float mouseX, mouseY;

		// Levels
		level levels[num_levels];

		// Level transition variables
		float scaler1, scaler2, scaler3, scaler4; // for transition effect
		FunctionPtr next_init, next_update, next_exit; // for next stage

	#pragma endregion VARIABLES
	/*---------------------------------------------------------------------------------------------------------*/
	
#pragma endregion GENERAL LEVEL GLOBALS
/*=====================================================================================================================*/


// PLAYER AND MOVEMENT RELATED
/*=====================================================================================================================*/
#pragma region 
	// ENUMS AND STRUCTS
	/*
		Enums:
		- PlayerState
		- PlayerDirection
		- CauseOfDeath
	 
		Structs:
		- Player
		- MoveState
	*/
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		// PlayerState updates what the player is doing, used to
		// determine animations and several conditions
		typedef enum PlayerState {
			Idle,
			Running,
			Jumping,
			Sliding,
			Crawling,
			Dead,
			Spawning,
		} PlayerState;

		// PlayerDirection updates animations
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

		// CauseOfDeath updates death animations
		typedef enum CauseOfDeath {
			Burn,
			Fall,
			Crush,
			Reset,
		} CauseOfDeath;

		// Player struct
		typedef struct Player {
			// position
			float x, y;
			// velocity
			float movex, movey;
			// stamina for InPlat
			float stamina; 
			// size of player
			float width, height;

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


		// MoveState determines how the player is moving or transitioning
		typedef enum MoveState {
			OnPlat,
			InPlat,
			EnterPlat,
			ExitPlat,
		} MoveState;

	#pragma endregion Enums and Structs
	/*---------------------------------------------------------------------------------------------------------*/



	// VARIABLES
	//	Death counter, speed, and animation sprites
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		int deaths;
		float speed;

		// Graphics and animation
		CP_Image player_sprite, boot_sprite, bubble_sprite, transition_sprite, spawn_sprite, burn_sprite, fall_sprite, crush_sprite, goal_sprite;

		float player_sprite_x, player_sprite_y;
		float transit_sprite_x, transit_sprite_y;
		float death_sprite_x, death_sprite_y;

		// Player
		Player player;

		// MoveState
		MoveState movestate;
		
	#pragma endregion VARIABLES
	/*---------------------------------------------------------------------------------------------------------*/

#pragma endregion PLAYER AND MOVEMENT RELATED
/*=====================================================================================================================*/


// BUFFS
/*=====================================================================================================================*/
#pragma region 
	// ENUMS AND STRUCTS
	/*
		Enums:
		- 
	 
		Structs:
		- 
	*/
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		// Buffs struct and enum
		typedef struct buffs {//we use this to get the quantitative amount of these buffs
			int state;
			int canPickup; //whether its there
			//int playerActive; //whether player has it
			float X; //for drawing
			float Y; //for drawing
			float width; //for drawing
			float height; //for drawing
			float timeslowed; //if its timeslow
			float jumpIncrease; //jumpIncrease
			float invulActive; //invulnerable
			float buffduration; //duration for each buff
			float resetTimer;
		}buffs;

		enum playerQualities  //we use this as a qualitative indicator of player powerup state 
		{
			baseStat = 0b000,
			JUMPYBuff = 0b001,
			jumpBuff = 0b010,
			invulBuff = 0b100
		}; //states/bool

	#pragma endregion Enums and Structs
	/*---------------------------------------------------------------------------------------------------------*/



	// VARIABLES
	//	
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		// Buffs
		buffs Buff[100];

		CP_Image buff_sprite, buff_bg;

		float jumpalpha, shieldalpha, jumpstatalpha, shieldstatalpha;
	#pragma endregion VARIABLES
	/*---------------------------------------------------------------------------------------------------------*/
	
#pragma endregion BUFFS
/*=====================================================================================================================*/


// PLATFORMS, WALLS, GENERAL COLLISION
/*=====================================================================================================================*/
#pragma region 

	#define max_coords 16
	#define max_platforms 20
	#define max_walls 20
	#define max_checkpoints 4
	// STRUCTS
	/*
		Structs:
		- coords
		- Checkpoint
		- platblocks
		- platform
	*/
	/*---------------------------------------------------------------------------------------------------------*/
#pragma region 
	// Platform Struct
	// InPlat
	// coordinate struct
		typedef struct coords {
			float x, y;
		} coords;

		// Checkpoint
		typedef struct Checkpoint {
			coords coordinates[max_checkpoints];
			int active;
		} Checkpoint;

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
#pragma endregion Structs
		/*---------------------------------------------------------------------------------------------------------*/



		// VARIABLES
		//	
		/*---------------------------------------------------------------------------------------------------------*/
#pragma region 
		int numplats;
		platform Platform[max_platforms];

		int numwalls;
		platblocks walls[max_walls];

		int numcheckpoints;
		Checkpoint checkpoint;

		coords spawn;
#pragma endregion VARIABLES
		/*---------------------------------------------------------------------------------------------------------*/

#pragma endregion PLATFORMS, WALLS, GENERAL COLLISION
/*=====================================================================================================================*/


// HAZARDS
/*=====================================================================================================================*/
#pragma region 
#define MAX_TRAPS 50
#define BULLET_SIZE 15
#define BULLET_SPEED 250
#define MAX_BULLETS 3
	// ENUMS AND STRUCTS
	/*
		Enums:
		- Hazard type (e.g spike/crusher/shooter)

		Structs:
		- Hazard
	*/
	/*---------------------------------------------------------------------------------------------------------*/
#pragma region 
	// Hazard type enum
	typedef enum hazardstype {
		spike,
		crusher,
		shooter
	}hazardstype;

	// Hazard struct
	typedef struct hazard {
		float x[3], y[3];
		int direction;
		hazardstype type;
	}hazard;

	// Point
	typedef struct Point {
		float x;
		float y;
	} Point;

	// Structure to represent a bullet
	typedef struct {
		float x, y;
		bool active;
	} Bullet;
#pragma endregion Enums and Structs
/*---------------------------------------------------------------------------------------------------------*/



// VARIABLES
//	
/*---------------------------------------------------------------------------------------------------------*/
#pragma region 
	hazard traps[MAX_TRAPS];
	Bullet bullets[MAX_BULLETS];
	CP_Image fire_sprite;
	float spawnTimer;
	float spawnInterval;
	float bulletx, bullety;
	int numberoftraps;
	int crusherstate;
	float firetimer;

#pragma endregion VARIABLES
/*---------------------------------------------------------------------------------------------------------*/

#pragma endregion HAZARDS
/*=====================================================================================================================*/

// OPTIONS
/*=====================================================================================================================*/
#pragma region 

// VARIABLES
//	
/*---------------------------------------------------------------------------------------------------------*/
#pragma region 
	// BGM volume bar
	float current_bgm_vol, max_bgm_vol, prev_bgm_vol;

	// Sound effects volume bar
	float current_se_vol, max_se_vol, prev_se_vol;

	// icons for volume bars
	float icon1_width, icon1_height, icon2_width, icon2_height;
	float icon_bgm_x, icon_bgm_y, icon_se_x, icon_se_y;

	// Sound files
	CP_Sound music;
	CP_Sound sfx;

	// Reset pointers
	FunctionPtr reset_init, reset_update, reset_exit;
	CP_Sound wafflemachine;
#pragma endregion VARIABLES
/*---------------------------------------------------------------------------------------------------------*/

#pragma endregion OPTIONS
/*=====================================================================================================================*/


	void AudioPitch();
// CAMERA FUNCTIONS
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init:
	/*-------------------------------------------------------------------------------------------------*/
	void Cam_Init(Camera* camera, float screen_width, float screen_height, coords spawn);
	/*-------------------------------------------------------------------------------------------------*/


	// Updates:
	/*-------------------------------------------------------------------------------------------------*/
	void playerCamColl(Player* player, Camera* camera);
	void CamShift(Camera* camera, float mouse_x, float mouse_y, float screen_width, float screen_height);
	/*-------------------------------------------------------------------------------------------------*/


	// Renders:
	/*-------------------------------------------------------------------------------------------------*/
	void CamHighLight(Camera* camera, float mouse_x, float mouse_y, float screen_width, float screen_height);
	void Guide_Arrow();
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion TEMPLATE FUNCTIONS


// PLAYER AND MOVEMENT FUNCTIONS (platform.c)
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init:
	/*-------------------------------------------------------------------------------------------------*/
		// Initialize player w set values such as size etc.
		void Init_Player(float* speed, Player* player, int* deaths, coords spawn);
		/*-------------------------------------------------------------------------------------------------*/


		// Updates:
		/*-------------------------------------------------------------------------------------------------*/
		// - MOVEMENT AND MOVESTATE
#pragma region 
		// set movestate
			void start_transition(Player* player, MoveState* movestate);
			void end_transition(Player* player, MoveState* movestate);
			// platformer and in platform movements
			void movement(Player* p, MoveState movestate, float speed, float dt);
			// jump >:)
			void jump(Player* player, MoveState movestate, int buffstate);
#pragma endregion Movement and movestate

		// - PLAYER
#pragma region 
		// Player States
			void Kill_Player(Player* player, CauseOfDeath deathcause, int* deaths, int *buffstate);
			void Spawn_Player(Player* player, coords spawn, Camera* camera);
			void Reset_Player(Player* player, MoveState* movestate);
			void Transition_Player(Player* player, MoveState* movestate);

			// Anim Updates
			void player_anim_updates(Player* player, float dt, MoveState* movestate, Camera* camera, coords spawn);
#pragma endregion Player
		/*-------------------------------------------------------------------------------------------------*/


		// Renders:
		/*-------------------------------------------------------------------------------------------------*/
			// Draw Player
			void Draw_Player(Player player, MoveState movestate);

			// draw stat bar
			void DrawStatBar(float pos_x1, float pos_y1, float pos_x2, float pos_y2, float stat, float maxstat);

		/*-------------------------------------------------------------------------------------------------*/
#pragma endregion PLAYER AND MOVEMENT FUNCTIONS


// BUFF FUNCTIONS
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init:
	/*-------------------------------------------------------------------------------------------------*/

	/*-------------------------------------------------------------------------------------------------*/


	// Updates:
	/*-------------------------------------------------------------------------------------------------*/
		int buff_pickup(float x, float y, float height, float width);
	/*-------------------------------------------------------------------------------------------------*/


	// Renders:
	/*-------------------------------------------------------------------------------------------------*/
		void Draw_Buffs();
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion TEMPLATE FUNCTIONS


// PLATFORM FUNCTIONS (platform.c)
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
		// Init:
		/*-------------------------------------------------------------------------------------------------*/
			// PLATFORMS
				// reset platform members to 0
				void reset_platforms(void);
				// init platforms from file
				void init_platforms(FILE* PlatformFile);
				// assign platform coordinates
				void platform_coordinates(int plat_index, int coord_index, int x, int y);
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
				// init checkpoints
				void init_checkpoints(FILE* CheckpointFile);

			// WALLS
				// reset walls to 0
				void reset_walls();
				// init walls from file
				void init_walls(FILE* WallFile);
		/*-------------------------------------------------------------------------------------------------*/


		// Updates:
		/*-------------------------------------------------------------------------------------------------*/
			// if player and platform hitbox collides
			int collision(platblocks plt, Player p);

			int collision_direction(platblocks plt, Player p);

			float fix_collide_x(platblocks plt, Player p);
			float fix_collide_y(platblocks plt, Player p);

			void CollPlatforms(int plat_index, platform plt, Player* p, Player ply);
			void CollOnPlatforms(int plat_index, platform plt, Player* p, Player ply);

			void CollWalls(platblocks walls[], int num, Player* pply, Player ply, MoveState movestate);
		/*-------------------------------------------------------------------------------------------------*/


		// Renders:
		/*-------------------------------------------------------------------------------------------------*/
			// Draw Platforms
			void DrawPlatform(int plat_index);
		/*-------------------------------------------------------------------------------------------------*/
	#pragma endregion PLATFORM FUNCTIONS


// HAZARD FUNCTIONS (hazards.c)
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
		// Init:
		/*-------------------------------------------------------------------------------------------------*/
			void reset_hazards();
			void init_hazards(FILE* HazardFile, int numberoftraps);
		/*-------------------------------------------------------------------------------------------------*/


		// Updates:
		/*-------------------------------------------------------------------------------------------------*/
			bool is_point_in_square(Point p, Point squareTopLeft, float width, float height);
			int orientation(Point a, Point b, Point c);
			bool lines_intersect(Point p1, Point q1, Point p2, Point q2);
			bool check_square_triangle_collision(Point squareTopLeft, float width, float height,
				Point A, Point B, Point C);

			int crusher_direction(hazard crusher, Player p);

			void hazard_update();
		/*-------------------------------------------------------------------------------------------------*/


		// Renders:
		/*-------------------------------------------------------------------------------------------------*/
			void Draw_Hazards();
		/*-------------------------------------------------------------------------------------------------*/
#pragma endregion HAZARD FUNCTIONS

// UTILITY FUNCTIONS (utils.c)
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init/Exit:
	/*-------------------------------------------------------------------------------------------------*/
		void Load_Images();
		void Free_Images();
	/*-------------------------------------------------------------------------------------------------*/


	// Updates:
	/*-------------------------------------------------------------------------------------------------*/
		// check if position is within a rectangle
		int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
		int IsAreaCornerClicked(float area_corner_x, float area_corner_y, float area_width, float area_height, float click_x, float click_y);

		// check if position is within a circle
		int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y);

		// check if two circles intersect
		int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

		// check if two squares intersect
		int AreSquaresIntersecting(float s1_x, float s1_y, float s1_w, float s1_h, float s2_x, float s2_y, float s2_w, float s2_h);

		// check if square collides with triangle
		int SquareTriangleCollide(float squarex, float squarey, float squarewidth, float squareheight, float tri1x, float tri1y, float tri2x, float tri2y, float tri3x, float tri3y);


		// update score
		void AddScore(int *totalScore, int addScore, LevelState* levelstate);

		// score increment animation
		int IncrementScore(int startScore, int endScore, float* timeElapsed, float dt);

		// Set next stage for transition
		void SetNext(FunctionPtr level_init, FunctionPtr level_update, FunctionPtr level_exit);
	/*-------------------------------------------------------------------------------------------------*/


	// Renders:
	/*-------------------------------------------------------------------------------------------------*/
		// create a text box
		void TextBoxCreate(float pos_x, float pos_y, float size_x, float size_y, const char* text, CP_Color color);

		// transition effect
		void Level_Transition(FunctionPtr level_init, FunctionPtr level_update, FunctionPtr level_exit);
		void TransitionInBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4);
		void TransitionOutBoxEffect(float playerX, float playerY, float boxX1, float boxX2, float boxY1, float boxY2, float scaler1, float scaler2, float scaler3, float scaler4);
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion UTILITY FUNCTIONS

// OPTIONS FUNCTIONS (options.c)
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	/*-------------------------------------------------------------------------------------------------*/
		void Options_Init(void);
		void Options_Update(void);
		void Options_Exit(void);

		// menus when game is paused (pause, options, quit)
		void PauseMenu();
		void OptionsMenu();
		void QuitMenu();

		void SaveOptions(float current_bgm_vol, float current_se_vol);
		void SaveOptionsInGame(float current_bgm_vol, float current_se_vol, float* prev_bgm_vol, float* prev_se_vol);
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion OPTIONS FUNCTIONS

// MAJOR FUNCTIONS
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init:
	/*-------------------------------------------------------------------------------------------------*/

	/*-------------------------------------------------------------------------------------------------*/


	// Updates:
	/*-------------------------------------------------------------------------------------------------*/

	/*-------------------------------------------------------------------------------------------------*/


	// Renders:
	/*-------------------------------------------------------------------------------------------------*/
		void RENDER_ALL(); // utils.c
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion TEMPLATE FUNCTIONS









// TEMPLATE
/*=====================================================================================================================*/
#pragma region 
	// ENUMS AND STRUCTS
	/*
		Enums:
		- 
	 
		Structs:
		- 
	*/
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 

	#pragma endregion Enums and Structs
	/*---------------------------------------------------------------------------------------------------------*/



	// VARIABLES
	//	
	/*---------------------------------------------------------------------------------------------------------*/
	#pragma region 
		
	#pragma endregion VARIABLES
	/*---------------------------------------------------------------------------------------------------------*/
	
#pragma endregion TEMPLATE
/*=====================================================================================================================*/

// FUNCTIONS
/*---------------------------------------------------------------------------------------------------------*/
#pragma region
	// Init:
	/*-------------------------------------------------------------------------------------------------*/
			
	/*-------------------------------------------------------------------------------------------------*/


	// Updates:
	/*-------------------------------------------------------------------------------------------------*/
		
	/*-------------------------------------------------------------------------------------------------*/


	// Renders:
	/*-------------------------------------------------------------------------------------------------*/
			
	/*-------------------------------------------------------------------------------------------------*/
#pragma endregion TEMPLATE FUNCTIONS


			
