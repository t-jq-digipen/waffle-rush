#pragma once
//---------------------------------------------------------
// file:	credits.h
// author:	AVID WAFFLE ENJOYERS
// email:	-
//
// brief:	Declarations of game stage
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "mainmenu.h"
#include "utils.h"
#include "platform.h"
#include <stdio.h>


//typedef struct playerBuff{  
//	float speedincrease;
//	float 
// increase;
//	float invulnarability;
//}playerBuff;

typedef struct buffs{//we use this to get the quantitative amount of these buffs
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
slowTime = 0b001, 
jumpBuff = 0b010, 
invulBuff = 0b100}; //states/bool

buffs Buff[100];

int is_colliding(float x, float y, float height, float width);


