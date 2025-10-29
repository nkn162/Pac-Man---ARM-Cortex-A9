

#ifndef GHOST_H
#define GHOST_H

#include <stdint.h>

typedef struct {
	uint16_t x, y; // Current position
	uint16_t target_x, target_y; // Target tile (Pacman's location)
	char mode; // C = Chase, S = Scatter
} Ghost;

void initblinky(LT24Ctx_t* ctx, int x, int y);
uint8_t move_blinky( Ghost *blinky, int pacman_x, int pacman_y , uint16_t Row, uint16_t Column, uint16_t maze[Row][Column]);
void Clear_ghost(LT24Ctx_t* ctx,Ghost *ghost,uint8_t direction);

void initpinky(LT24Ctx_t* ctx, int x, int y);
uint8_t move_pinky( Ghost *pinky,int pacman_x, int pacman_y, uint16_t Row, uint16_t Column, uint32_t maze[Row][Column],int direction_pac);

void initinky(LT24Ctx_t* ctx, int x, int y);
uint8_t move_inky( Ghost *inky,Ghost *blinky,int pacman_x, int pacman_y, uint16_t Row, uint16_t Column, uint32_t maze[Row][Column],int direction_pac);

void initclyde(LT24Ctx_t* ctx, int x, int y);
uint8_t move_clyde( Ghost *clyde, int pacman_x, int pacman_y , uint16_t Row, uint16_t Column, uint32_t maze[Row][Column]);

uint8_t game_0ver(Ghost *inky, Ghost *pinky, Ghost *blinky, Ghost *clyde, int pacman_x, int pacman_y );
#endif
