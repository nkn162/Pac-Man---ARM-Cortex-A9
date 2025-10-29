#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "DE1SoC_Addresses/DE1SoC_Addresses.h"
#ifndef MAZE_H_
#define MAZE_H_

// Function to index the maze for maze generation



extern  uint32_t maze[30][33];
void maze_generation();
int maze_index(int x, int y);
void LT24_Maze(LT24Ctx_t* ctx, uint8_t maze_n, uint8_t x, uint16_t y);
void exitOnFail(HpsErr_t status);

#endif /* MAZE_H_ */
