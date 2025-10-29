/*
 * PacMan_Control.h
 *
 *  Created on: 28 Apr 2025
 *      Author: Neeraj
 */

#ifndef PACMAN_CONTROL_H_
#define PACMAN_CONTROL_H_

#include <stdint.h>
#include "DE1SoC_LT24/DE1SoC_LT24.h"

// Defining Constants for Screen dimensions and PacMan size
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
#define PACMAN_SIZE   6  //Increasing the size of PacMan to 6x6
#define TILE_WIDTH    8
#define TILE_HEIGHT   8
#define STEP_SIZE     3

// Defining Directions
#define DIR_NONE  0
#define DIR_UP    1
#define DIR_DOWN  2
#define DIR_LEFT  3
#define DIR_RIGHT 4

// Key mask definitions for detecting direction inputs
#define KEY_UP    (1 << 0)  //Bit 0
#define KEY_DOWN  (1 << 1)  //Bit 1
#define KEY_LEFT  (1 << 2)  //Bit 2
#define KEY_RIGHT (1 << 3)  //Bit 3

// External variables
extern int pacman_x;
extern int pacman_y;
extern int pacman_direction;
extern LT24Ctx_t* lt24_2;
extern volatile uint8_t *KEY_ptr;
extern volatile uint8_t *LED_ptr;


// Functions for PacMan Control
void movdelay(int ms);  //Simple delay function (used for smoother movement on screen)
void initPacMan(void);  //Function to initialise PacMan at desired position
void movePacMan(int direction);  //Function to move PacMan continuously in the specified direction
void clearPacMan(void);  //Function to clear previous PacMan position
void drawPacMan(void);  //Function to draw PacMan at the new position
void alignToTileCenter(int direction);  //Function to ensure PacMan is centered in the path



#endif /* PACMAN_CONTROL_H_ */
