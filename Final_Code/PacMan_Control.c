/*
 * PacMan_Control.c
 *
 *  Created on: 23 Apr 2025
 *  Updated on: 14 May 2025
 *      Author: Neeraj
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "PacMan_Control.h"
#include "Util/watchdog.h"
#include "FPGA_PIO/FPGA_PIO.h"
#include "DE1SoC_Addresses/DE1SoC_Addresses.h"
#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "maze.h"

// Maze tile dimensions (flipped axes: LCD X is logical Y, LCD Y is logical X)
#define TILE_WIDTH  8   // Vertical (screen X)
#define TILE_HEIGHT 8   // Horizontal (screen Y)
#define STEP_SIZE   3   // Fine step size for smooth alignment and movement

// Global Variables - PacMan Position, LT24 initialisation,s and maze dimensions
int pacman_x = 0;
int pacman_y = 0;
int pacman_direction = DIR_NONE;
extern LT24Ctx_t* lt24_2;
extern uint32_t maze[30][33];

bool waitingForNewInput = false;
int lastDirection = -1;

// Initialise PacMan in a walkable corner
void initPacMan() {
	printf("INIT PACMAN CALLED\n");

	    int col = 2; // x index in maze
	    int row = 2; // y index in maze

	    // Check if the tile is walkable
	    uint8_t tile = maze_index(row, col);
	    if (tile == 0 || tile == 1 || tile == 2) {
	        pacman_x = col * TILE_WIDTH + (TILE_WIDTH - PACMAN_SIZE) / 2;
	        pacman_y = row * TILE_HEIGHT + (TILE_HEIGHT - PACMAN_SIZE) / 2;

	    }
}

// Function to clear previous PacMan position
void clearPacMan() {
    for (int i = 0; i < PACMAN_SIZE; i++) {
        for (int j = 0; j < PACMAN_SIZE; j++) {
            LT24_drawPixel(lt24_2, LT24_BLACK, pacman_x + i, pacman_y + j);
        }
    }
}

// Function to draw PacMan at the new position
void drawPacMan() {
    for (int i = 0; i < PACMAN_SIZE; i++) {
        for (int j = 0; j < PACMAN_SIZE; j++) {
            LT24_drawPixel(lt24_2, LT24_YELLOW, pacman_x + i, pacman_y + j);
        }
    }
}

// Function to align PacMan at the center of the tile
void alignToTileCenter(int direction) {
    int offsetX = pacman_x % TILE_HEIGHT;
    int offsetY = pacman_y % TILE_WIDTH;

    int centerOffsetX = (TILE_HEIGHT - PACMAN_SIZE) / 2;
    int centerOffsetY = (TILE_WIDTH - PACMAN_SIZE) / 2;

    pacman_x = (pacman_x / TILE_WIDTH) * TILE_WIDTH + centerOffsetX;
    pacman_y = (pacman_y / TILE_HEIGHT) * TILE_HEIGHT + centerOffsetY;

    waitingForNewInput = true;
}

// Move PacMan continuously in the specified direction, snapping to centre and handling collisions
void movePacMan(int direction) {
	//alignToTileCenter(direction);
	if (waitingForNewInput && direction == lastDirection) {
		return;
	}
	int current_direction = direction;

	// Movement deltas
	int dx = 0, dy = 0;
	switch (direction) {
	    case DIR_RIGHT: dx = 2; break;  // Right
	    case DIR_UP: dy = -2; break; // Up
	    case DIR_DOWN: dy = 2; break;  // Down
	    case DIR_LEFT: dx = -2; break; // Left
	    default: return;
	}

	    int next_x = pacman_x + dx;
	    int next_y = pacman_y + dy;

	    // Define 4 corners of 5x5 PacMan sprite (�2 from center)
	    int corners[4][2] = {
	        { next_x - 1, next_y - 1 }, // Top-left
	        { next_x + 1, next_y - 1 }, // Top-right
	        { next_x - 1, next_y + 1 }, // Bottom-left
	        { next_x + 1, next_y + 1 }  // Bottom-right
	    };

	    // Choose which corners to check based on direction
	    int check_indices[2];
	    switch (direction) {
	        case DIR_RIGHT: // Right
	            check_indices[0] = 1; check_indices[1] = 3;
	            break;
	        case DIR_UP: // Up
	            check_indices[0] = 0; check_indices[1] = 1;
	            break;
	        case DIR_DOWN: // Down
	            check_indices[0] = 2; check_indices[1] = 3;
	            break;
	        case DIR_LEFT: // Left
	            check_indices[0] = 0; check_indices[1] = 2;
	            break;
	        default:
	            return;
	    }

	    // Check selected corners for collision
	    bool blocked = false;
	    for (int i = 0; i < 2; i++) {
	        int cx = corners[check_indices[i]][0];
	        int cy = corners[check_indices[i]][1];

	        int col = cx / TILE_WIDTH;
	        int row = cy / TILE_HEIGHT;

	        if (row < 0 || row >= 33 || col < 0 || col >= 30) {
	            blocked = true;
	            break;
	        }

	        uint8_t tile = maze_index(row, col);

	        if (tile >= 3) { // Walls or other obstacles
	            blocked = true;
	            break;
	        }
	    }

	    if (!blocked) {
	        pacman_x = next_x;
	        pacman_y = next_y;

	        waitingForNewInput = false;
	    } else {
	    	alignToTileCenter(direction);
	    }

	    lastDirection = direction;

	    int cur_col = pacman_x / TILE_WIDTH;
	    int cur_row = pacman_y / TILE_HEIGHT;
        int cur_tile = maze_index(cur_col, cur_row);

	    if (cur_tile == 1 || cur_tile == 2) {
            maze[cur_col][cur_row] = 0;
            LT24_Maze(lt24_2, maze[cur_col][cur_row],(8*cur_col),(8*cur_row));

	    }
}
