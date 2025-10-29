#include "Util/watchdog.h"
#include "Util/macros.h"
#include "Util/delay.h"
#include "math.h"
#include "time.h"
#include "stdio.h"
#include "stdbool.h"
#include "maze.h"

#include "FPGA_PIO/FPGA_PIO.h"
#include "DE1SoC_Addresses/DE1SoC_Addresses.h"
#include "DE1SoC_LT24/DE1SoC_LT24.h"



const uint16_t Pacman[] = {
		0x1040, 0x1020, 0x6300, 0xCE62, 0xDF03, 0xBE03, 0x4A60, 0x0040, 0x0860, 0x0800, 0x6B23, 0xF7CC, 0xF7A7, 0xF7A8, 0xF78C, 0xF7B3, 0x4A84, 0x0020, 0x2100, 0xDF0E, 0xEF89, 0xF7E9, 0xEF4B, 0xB58A, 0x6B27, 0x2102, 0x0000, 0x4240, 0xEF8D, 0xEFA7, 0x8460, 0x2960, 0x1880, 0x0820, 0x1040, 0x0820, 0x3A00, 0xDF4C, 0xEFA6, 0xCEC0, 0x9D00, 0x5AC0, 0x2940, 0x0020, 0x0840, 0x0840, 0xAD68, 0xF7C9, 0xF7C6, 0xFFE9, 0xEF6B, 0xE752, 0x52A5, 0x0020, 0x0820, 0x2940, 0xAD67, 0xEF6B, 0xF78C, 0xEF6F, 0xA50D, 0x18C0, 0x0000, 0x1021, 0x0800, 0x1060, 0x5260, 0x6300, 0x4200, 0x0820, 0x0800, 0x0821
};
const uint16_t BlinkyR[] = {
		0x0821, 0x2040, 0x8020, 0xD840, 0xD021, 0x8021, 0x2021, 0x0840, 0x1841, 0xC021, 0xF801, 0xE040, 0xE840, 0xF020, 0xB841, 0x2021, 0x7986, 0xDE7B, 0xF391, 0xF041, 0xE493, 0xE5DB, 0xF863, 0x7041, 0xB18C, 0x7C3D, 0xDEBE, 0xF040, 0x6B7F, 0xD71D, 0xF0E4, 0xD820, 0xC928, 0x9CBD, 0xE516, 0xE841, 0x9318, 0xDEBD, 0xE8C3, 0xE821, 0xF021, 0xD944, 0xF040, 0xF040, 0xE8A2, 0xF8E4, 0xE822, 0xF000, 0xF821, 0xF041, 0xF001, 0xF820, 0xF020, 0xE841, 0xF801, 0xF020, 0xE841, 0xE840, 0xF021, 0xE840, 0xE840, 0xF021, 0xF020, 0xE821, 0xB820, 0xF020, 0xA800, 0xE041, 0xD861, 0xA020, 0xF801, 0xB821
};
const uint16_t ClydeU[] = {
  0x0000, 0x18A2, 0x6B0B, 0xD616, 0xCE17, 0x6B0B, 0x18A2, 0x0000, 0x3144, 0xB554, 0x9C2E, 0xE636, 0xDE56, 0x9C4F, 0xB553, 0x3164, 0xA40E, 0xFEF8, 0xFF17, 0xF613, 0xEE13, 0xFF37, 0xFF17, 0xA42C, 0xEDD1, 0xEDB0, 0xFE31, 0xF5AE, 0xF5AE, 0xFE30, 0xEDCF, 0xEDF0, 0xE5AC, 0xEDAC, 0xF58A, 0xF5AA, 0xF5AB, 0xED8B, 0xEDCC, 0xEDAC, 0xEDEC, 0xEDCB, 0xF5CA, 0xFDEB, 0xFDEB, 0xF5CB, 0xEDAC, 0xEDED, 0xE5EF, 0xD54C, 0xF5ED, 0xDD09, 0xDD0A, 0xF5EE, 0xD54C, 0xE5EF, 0x9BC7, 0x3060, 0xCCEA, 0x7A20, 0x7A20, 0xCCEB, 0x3860, 0xA3C8
};
const uint16_t InkyU[] = {
  0x0004, 0x00A7, 0x2BD3, 0x669C, 0x66BC, 0x2411, 0x00E5, 0x0022, 0x0989, 0x7DB9, 0x4D37, 0x6EFE, 0x6EFE, 0x4557, 0x75D8, 0x01C8, 0x3574, 0x8FFF, 0x9FFF, 0x771F, 0x6F3F, 0x97FF, 0x8FFF, 0x2D56, 0x377B, 0x375B, 0x579F, 0x4EDE, 0x46FE, 0x579F, 0x3F3D, 0x375D, 0x0FDC, 0x1FDD, 0x1F7E, 0x277F, 0x1F9F, 0x179E, 0x1FDE, 0x17BD, 0x17DC, 0x17BD, 0x179E, 0x1FBF, 0x17DF, 0x17BE, 0x17BD, 0x1FBD, 0x3F3B, 0x2EFB, 0x377E, 0x1EBB, 0x16DB, 0x377D, 0x2EFB, 0x3F3C, 0x1514, 0x0208, 0x267A, 0x0411, 0x0411, 0x267A, 0x0208, 0x1514
};
const uint16_t PinkyD[] = {
  0x1802, 0x3866, 0xB476, 0xE61C, 0xE61C, 0xB476, 0x3086, 0x1002, 0x3887, 0xDD9B, 0xEE5E, 0xE61D, 0xE61D, 0xEE5E, 0xD5BB, 0x30A7, 0xB497, 0xEE7F, 0xEE9F, 0xE61E, 0xE61E, 0xF67F, 0xF67F, 0xB477, 0xE63E, 0xF69F, 0xE63F, 0xEE5F, 0xEE5F, 0xEE3F, 0xFE7F, 0xEE1F, 0xE61F, 0xCD3B, 0x82D2, 0xEE3F, 0xEE1F, 0x8AD3, 0xD51B, 0xF5FF, 0xF63F, 0xE5BD, 0xD53B, 0xF61F, 0xF61F, 0xD53C, 0xE5BE, 0xFE3F, 0xEDFD, 0xDD7C, 0xF63F, 0xD53B, 0xD53B, 0xF63F, 0xDD7C, 0xEDFE, 0xA394, 0x2805, 0xD51A, 0x720E, 0x720E, 0xD51A, 0x2805, 0xA3B5
};



//uint8_t Row=30;
//uint8_t Col=33;

//matrix for maze

//Ghost properties


typedef struct {
	uint16_t x, y; // Current position
} P;


typedef struct {
	uint16_t x, y; // Current position
	uint16_t target_x, target_y; // Target tile (Pacman's location)
	char mode; // C = Chase, S = Scatter
} Ghost;

//verf_turns
uint32_t * check_ghost_collisions(Ghost *ghost, uint16_t Row, uint16_t Column, uint32_t maze[Row][Column]) {
	// pixel is 8x8 the x and y are flipped due to the maze being flipped in the LCD screen

	//This is to mark the outer of the ghost being 6 by 6 therefore offset from the center becomes 3
	uint16_t pix_y = (ghost->x) / 8;
	uint16_t pix_x = (ghost->y) / 8;

	//printf("%d\n",maze[pix_y][pix_x]);

	//printf("%d & %d\n",pix_y,pix_x);

	// The list is used to give the object all the possible direction which can be called
	static uint32_t turns[4] = {0,0,0,0};
	// To keep it in the memory we use static and make the function pointer so we can call the array to be used

	// Above 3 are all the maze blocks that can't be collided with
	//Right
	if(maze[pix_y+1][pix_x] < 3)
	{
		turns[0]=1;
	}
	else{
		turns[0]=0;
	};

	//Left
	if(maze[pix_y-1][pix_x] < 3)
	{
		turns[1]=1;
		}
		else{
		turns[1]=0;
	};

	//Up
	if(maze[pix_y][pix_x-1] < 3)
	{
		turns[2]=1;
		}
		else{
		turns[2]=0;
	};

	//Down
	if(maze[pix_y][pix_x+1] < 3)
	{
		turns[3]=1;
		}
		else{
		turns[3]=0;
	};


	//printf("%d,%d,%d,%d\n",turns[0],turns[1],turns[2],turns[3]);
	return turns; // Directions allowed
	// The code in the comment below is how the collision is called to know the turn list
	// const uint32_t * verf_turns = check_ghost_collisions(blinky,maze);
}


void initblinky(LT24Ctx_t* ctx, int x, int y){
	LT24_copyFrameBuffer(ctx, BlinkyR, x, y, 8, 8);
}

void initpinky(LT24Ctx_t* ctx, int x, int y){
	LT24_copyFrameBuffer(ctx, PinkyD, x, y, 8, 8);
}
void initinky(LT24Ctx_t* ctx, int x, int y){
	LT24_copyFrameBuffer(ctx, InkyU, x, y, 8, 8);
}

void initclyde(LT24Ctx_t* ctx, int x, int y){
	LT24_copyFrameBuffer(ctx, ClydeU, x, y, 8, 8);
}

int movement_ghost(uint8_t sum, Ghost *ghost, int target_x, int target_y){
    uint8_t step=1;
    uint8_t direction=0;
    float trig_x1,trig_x2,trig_y1,trig_y2;
    int pix_y = (ghost->x) / 8;
	int pix_x = (ghost->y) / 8;

	switch(sum){
        case 1:
        ghost->y+=step;
        ghost->x=pix_y*8;
        //printf("direction=4\n");
        direction=4;
        break;

        case 2:
        ghost->y-=step;
        ghost->x=pix_y*8;
        //printf("direction=3\n");
        direction=3;
        break;

        case 3:
        if(target_y>=ghost->y){
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
        }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
        }
        break;

        case 4:
        ghost->x-=step;
        ghost->y=pix_x*8;
        //printf("direction=2\n");
        direction=2;
        break;

        case 5:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        if(trig_x1>trig_y1)
        {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
        }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
        }
        break;

        case 6:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        if(trig_x1>trig_y1)
        {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
        }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
        }
        break;

        case 7:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        trig_y2=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        if(trig_y1>trig_y2)
        {
            if(trig_x1>trig_y1)
            {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
            }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
            }
        }else{
            if(trig_x1>trig_y1)
            {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
            }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
            }}
        break;

        case 8:
        ghost->x+=step;
        ghost->y=pix_x*8;
        //printf("direction=1\n");
        direction=1;
        break;

        case 9:
        trig_x1=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        if(trig_x1>trig_y1)
        {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
        }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
        }
        break;

        case 10:
        trig_x1=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        if(trig_x1>trig_y1)
        {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
        }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
        }
        break;

        case 11:
        trig_x1=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        trig_y2=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        if(trig_y1>trig_y2)
        {
            if(trig_x1>trig_y1)
            {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
            }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
            }
        }else{
            if(trig_x1>trig_y1)
            {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
            }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
            }}
        break;
        case 12:
        if(target_x>=ghost->x){
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
        }else{
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
        }
        break;
        case 13:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_x2=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        if(trig_x1>trig_x2)
        {
            if(trig_x1>trig_y1)
            {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
            }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
            }
        }else{
            if(trig_x2>trig_y1)
            {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
            }else{
            ghost->y+=step;
            ghost->x=pix_y*8;
            //printf("direction=4\n");
            direction=4;
            }}
        break;
        case 14:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_x2=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        if(trig_x1>trig_x2)
        {
            if(trig_x1>trig_y1)
            {
            ghost->x-=step;
            ghost->y=pix_x*8;
            //printf("direction=2\n");
            direction=2;
            }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
            }
        }else{
            if(trig_x2>trig_y1)
            {
            ghost->x+=step;
            ghost->y=pix_x*8;
            //printf("direction=1\n");
            direction=1;
            }else{
            ghost->y-=step;
            ghost->x=pix_y*8;
            //printf("direction=3\n");
            direction=3;
            }}
        break;
        case 15:
        trig_x1=sqrt((target_x-ghost->x-step)*(target_x-ghost->x-step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_x2=sqrt((target_x-ghost->x+step)*(target_x-ghost->x+step)+(target_y-ghost->y)*(target_y-ghost->y));
        trig_y1=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y-step)*(target_y-ghost->y-step));
        trig_y2=sqrt((target_x-ghost->x)*(target_x-ghost->x)+(target_y-ghost->y+step)*(target_y-ghost->y+step));
        if(trig_x1>trig_x2)
        {
            if(trig_y1>trig_y2)
            {if(trig_x1>trig_y1){
                ghost->x-=step;
                ghost->y=pix_x*8;
                //printf("direction=2\n");
                direction=2;
            }else{
                ghost->y-=step;
                ghost->x=pix_y*8;
                //printf("direction=3\n");
                direction=3;
            }
            }else{if(trig_x1>trig_y2){
                ghost->x-=step;
                ghost->y=pix_x*8;
                //printf("direction=2\n");
                direction=2;
            }else{
                ghost->y+=step;
                ghost->x=pix_y*8;
                //printf("direction=4\n");
                direction=4;
            }
            }
        }else{
            if(trig_y1>trig_y2)
            {if(trig_x2>trig_y1){
                ghost->x+=step;
                ghost->y=pix_x*8;
                //printf("direction=1\n");
                direction=1;
            }else{
                ghost->y-=step;
                ghost->x=pix_y*8;
                //printf("direction=3\n");
                direction=3;
            }
            }else{if(trig_x2>trig_y2){
                ghost->x+=step;
                ghost->y=pix_x*8;
                //printf("direction=1\n");
                direction=1;
            }else{
                ghost->y+=step;
                ghost->x=pix_y*8;
                //printf("direction=4\n");
                direction=4;
            }
            }
    } break;

}return direction;}

void Clear_ghost(LT24Ctx_t* ctx,Ghost *ghost,uint8_t direction){
	uint8_t pix_y = (ghost->x) / 8;
	uint8_t pix_x = (ghost->y) / 8;
	if(direction == 1){
		for(uint16_t j=((pix_x)*8);j<((pix_x)*8+8);j++){
		for(uint16_t i=((pix_y-1)*8);i<((pix_y-1)*8+8);i++){
			LT24_drawPixel(ctx, LT24_BLACK, i, j);
		}}
		LT24_Maze(ctx, maze[pix_y-1][pix_x],(pix_y-1)*8 , (pix_x)*8);

	}else if(direction == 2){
		for(int j=((pix_x)*8);j<((pix_x)*8+8);j++){
		for(int i=((pix_y+1)*8);i<((pix_y+1)*8+8);i++){
			LT24_drawPixel(ctx, LT24_BLACK, i, j);
		}}
		LT24_Maze(ctx, maze[pix_y+1][pix_x],(pix_y+1)*8 , (pix_x)*8);

	} else 	if(direction == 3){
		for(int j=((pix_x+1)*8);j<((pix_x+1)*8+8);j++){
		for(int i=((pix_y)*8);i<((pix_y)*8+8);i++){
			LT24_drawPixel(ctx, LT24_BLACK, i, j);
		}}
		LT24_Maze(ctx, maze[pix_y][pix_x+1],(pix_y)*8 , (pix_x+1)*8);
	} else	if(direction == 4){
		for(int j=((pix_x-1)*8);j<((pix_x-1)*8+8);j++){
		for(int i=((pix_y)*8);i<((pix_y)*8+8);i++){
			LT24_drawPixel(ctx, LT24_BLACK, i, j);
		}}
		LT24_Maze(ctx, maze[pix_y][pix_x-1],(pix_y)*8 , (pix_x-1)*8);
	}
}

//moveblinky
int directionf=0;
uint8_t direction=0; //R=1, L=2, U=3, D=4
uint8_t move_blinky( Ghost *blinky, int pacman_x, int pacman_y , uint16_t Row, uint16_t Column, uint32_t maze[Row][Column]){
	const uint32_t * verf_turns = check_ghost_collisions(blinky,Row,Column,maze);
	float trig_x1,trig_x2,trig_y1,trig_y2;
	uint8_t step=1;
	uint8_t sum=0;

	uint8_t target_x=pacman_x;
	uint8_t target_y=pacman_y;

    sum=verf_turns[3]*1+verf_turns[2]*2+verf_turns[1]*4+verf_turns[0]*8;
    printf("Sum %d\n",sum);
    direction=movement_ghost(sum, blinky, target_x, target_y);
//    if (direction == directionf){
//    	direction = directionf;
//    }else{
//    	Clear_ghost(ctx,blinky,direction);
//    }
    return direction;
}



uint8_t move_pinky( Ghost *pinky,int pacman_x, int pacman_y, uint16_t Row, uint16_t Column, uint32_t maze[Row][Column],int direction_pac){
	uint16_t target_x,target_y;
	const uint32_t * verf_turns = check_ghost_collisions(pinky,Row,Column,maze);
	float trig_x1,trig_x2,trig_y1,trig_y2;
	uint8_t step=1;
	uint8_t sum=0;

    target_x=pacman_x;
    target_y=pacman_y;


    if(direction_pac == 1)
    {
        target_x=pacman_x+4*8;
    }else if(direction_pac == 2)
    {
    	target_x=pacman_x-4*8;
    }else if(direction_pac == 3)
    {
    	target_y=pacman_y-4*8;
    }else if(direction_pac == 4)
    {
    	target_y=pacman_y+4*8;
    }

    //printf("direction %d & target_x %d & target_y %d\n", direction_pac,target_x,target_y);

    sum=verf_turns[3]*1+verf_turns[2]*2+verf_turns[1]*4+verf_turns[0]*8;
    //printf("Sum %d\n",sum);
    direction=movement_ghost(sum, pinky, target_x, target_y);
    return direction;
}


uint8_t move_inky( Ghost *inky,Ghost *blinky,int pacman_x, int pacman_y, uint16_t Row, uint16_t Column, uint32_t maze[Row][Column],int direction_pac){
	const uint32_t * verf_turns = check_ghost_collisions(inky,Row,Column,maze);
	float trig_x1,trig_x2,trig_y1,trig_y2;
	uint8_t step=1;
	uint8_t sum=0;
	uint16_t target_x=0, target_y=0;

    if(direction_pac == 1)
    {

        if(blinky->y>pacman_y){
            if(blinky->x>pacman_x){
            	target_x=pacman_x+2-blinky->x;
            	target_y=pacman_y-blinky->y;
            }else{
            	target_x=pacman_x+2+blinky->x;
            	target_y=pacman_y-blinky->y;
            }
        }else{
        if(blinky->x>pacman_x){
        	target_x=pacman_x+2-blinky->x;
        	target_y=pacman_y+blinky->y;
        }else{
        	target_x=pacman_x+2+blinky->x;
        	target_y=pacman_y+blinky->y;
        }
        }
    }else if(direction_pac == 2){
        if(blinky->y>pacman_y){

        if(blinky->x>pacman_x){
        	target_x=pacman_x-2-blinky->x;
        	target_y=pacman_y-blinky->y;
        }else{
        	target_x=pacman_x-2+blinky->x;
        	target_y=pacman_y-blinky->y;
        }
        }else if (blinky->y<=pacman_y) {
        if(blinky->x>pacman_x){
        	target_x=pacman_x-2-blinky->x;
        	target_y=pacman_y+blinky->y;
        }else{
            //printf("here\n");
        	target_x=pacman_x-2+blinky->x;
        	target_y=pacman_y+blinky->y;
        }
        }
    }else if(direction_pac == 3){
        if(blinky->y>pacman_y){
        if(blinky->x>pacman_x){
        	target_x=pacman_x-blinky->x;
        	target_y=pacman_y-2-blinky->y;
        }else{
        	target_x=pacman_x+blinky->x;
        	target_y=pacman_y-2-blinky->y;
        }
        }else{
        if(blinky->x>pacman_x){
        	target_x=pacman_x-blinky->x;
        	target_y=pacman_y-2+blinky->y;
        }else{
        	target_x=pacman_x+blinky->x;
        	target_y=pacman_y-2+blinky->y;
        }
    }}else if(direction_pac == 4){
        if(blinky->y>pacman_y){
        if(blinky->x>pacman_x){
        	target_x=pacman_x-blinky->x;
        	target_y=pacman_y+2-blinky->y;
        }else{
        	target_x=pacman_x+blinky->x;
        	target_y=pacman_y+2-blinky->y;
        }
        }else{
        if(blinky->x>pacman_x){
        	target_x=pacman_x-blinky->x;
        	target_y=pacman_y+2+blinky->y;
        }else{
        	target_x=pacman_x+blinky->x;
        	target_y=pacman_y+2+blinky->y;
        }
    }}
    //printf("direction %d & target_x %d & target_y %d\n", blinky.x,target_x,target_y);
    sum=verf_turns[3]*1+verf_turns[2]*2+verf_turns[1]*4+verf_turns[0]*8;
    //printf("Sum %d\n",sum);
    direction = movement_ghost(sum, inky, target_x, target_y);
    return direction;
}

uint8_t move_clyde( Ghost *clyde, int pacman_x, int pacman_y , uint16_t Row, uint16_t Column, uint32_t maze[Row][Column]){
	const uint32_t * verf_turns = check_ghost_collisions(clyde,Row,Column,maze);
	float trig_x1,trig_x2,trig_y1,trig_y2;
	uint8_t step=1;
	uint8_t sum=0;
	uint16_t pix_y_cl = (clyde->x) / 8;
	uint16_t pix_x_cl = (clyde->y) / 8;
	uint16_t pix_y_pac = (pacman_x) / 8;
	uint16_t pix_x_pac = (pacman_y) / 8;
	uint16_t direction_clyde=0;
    //Circle equation
    float check=sqrt((pix_y_cl-pix_y_pac)*(pix_y_cl-pix_y_pac)+(pix_x_cl-pix_x_pac)*(pix_x_cl-pix_x_pac));



    //check that clyde is outside of pacman 8 tile radius
    if(check>=8){
    //printf("clyde x %d clyde y %d pac x %d pac y %d\n",pix_y_cl,pix_x_cl,pix_y_pac,pix_x_pac);
    sum=verf_turns[3]*1+verf_turns[2]*2+verf_turns[1]*4+verf_turns[0]*8;
    //printf("Sum %d\n",sum);

    int target_x=pacman_x;
    int target_y=pacman_y;

    direction=movement_ghost(sum, clyde, target_x, target_y);
}else{ if ((direction_clyde ==1)&&(verf_turns[0]==1))
    {
        clyde->x+=1;
        direction=1;
    }else if ((direction_clyde ==2)&&(verf_turns[1]==1))
    {
        clyde->x-=1;
        direction=2;
    }else if ((direction_clyde ==3)&&(verf_turns[2]==1))
    {
        clyde->y-=1;
        direction=3;
    }else if ((direction_clyde ==4)&&(verf_turns[3]==1))
    {
        clyde->y+=1;
        direction=4;
    }else{
        direction_clyde=rand() % (4 + 1 - 1) + 1;
    }

}
return direction;}

//clyde
//pinky

//blinky

uint8_t game_0ver(Ghost *inky, Ghost *pinky, Ghost *blinky, Ghost *clyde, int pacman_x,int pacman_y ){
	int flag=0;
	//printf("equal %d,%d",pacman_x,blinky->x);
	if((inky->x <= pacman_x+2 && inky->x >= pacman_x-2)  && (inky->y <= pacman_y+2 && inky->y >= pacman_y-2) ){
		printf("Game_over");
		flag=1;
	}
	if((pinky->x <= pacman_x+2 && pinky->x >= pacman_x-2)  && (pinky->y <= pacman_y+2 && pinky->y >= pacman_y-2) ){
		printf("Game_over");

		flag=1;
	}
	if((blinky->x <= pacman_x+2 && blinky->x >= pacman_x-2)  && (blinky->y <= pacman_y+2 && blinky->y >= pacman_y-2) ){
		printf("Game_over");

		flag=1;
	}
	if((clyde->x <= pacman_x+2 && clyde->x >= pacman_x-2)  && (clyde->y <= pacman_y+2 && clyde->y >= pacman_y-2)){
		printf("Game_over");
		flag=1;
	}
	return flag;
}

int i=0;

#include <stdint.h>

// For DE1-SoC ARM Cortex-A9 (800 MHz default)
#define CPU_FREQ 800000000  // Adjust if your clock differs

void delay_ms(uint32_t milliseconds) {
    uint32_t cycles_per_ms = CPU_FREQ / 1000;  // Cycles per millisecond
    uint32_t total_cycles = cycles_per_ms * milliseconds;

    // Busy-wait loop
    for (volatile uint32_t i = 0; i < total_cycles; i++);
}



//int main (void) {
//	   // Variables for buttons
//		volatile unsigned int *LEDR_ptr = (unsigned int *)LSC_BASE_RED_LEDS;
//		volatile unsigned int *KEY_ptr= (unsigned int *)LSC_BASE_KEYS;
//
//	   //Variables for LCD
//	    FPGAPIOCtx_t* gpio1;
//	    LT24Ctx_t* lt24;
//	    //Initialise the GPIO1 port
//	    exitOnFail(FPGA_PIO_initialise(LSC_BASE_GPIO_JP1, LSC_CONFIG_GPIO, &gpio1));
//	    //Initialise the LCD Display.
//	    exitOnFail(LT24_initialise(&gpio1->gpio,NULL,&lt24));
//	    ResetWDT();
//	    //Wait a moment
//	    usleep(500000);
//	    ResetWDT();
//	    //Main Run Loop
//    	for(int u=0;u<=29;u++)
//    	{
//    		for( int z=0; z<=32; z++)
//    		{
//    			LT24_Maze(lt24, maze[u][z], (8*u), (8*z));
//    		}
//    	};
//
//
//	    while (1) {
//	        //Set display to solid colour
//	    	uint8_t x;
//	    	uint8_t y;
//
//
//	        LT24_setWindow(lt24, blinky.y, blinky.x, 8, 8);
//	        LT24_setWindow(lt24, blinky.target_y, blinky.target_x, 8, 9);
//
//	    	LT24_copyFrameBuffer(lt24, Pacman, Pac.x, Pac.y, 9, 8);
//
//	    	direction=move_blinky(&blinky,&Pac,Row,Col, maze);
//	    	Clear_ghost(lt24,&blinky,direction);
//	    	direction=move_pinky(&pinky,&Pac,Row,Col,maze);
//	    	Clear_ghost(lt24,&pinky,direction);
//	    	direction=move_inky(&inky,&Pac,Row,Col,maze);
//	    	Clear_ghost(lt24,&inky,direction);
//	    	direction=move_clyde(&clyde,&Pac,Row,Col,maze);
//	    	Clear_ghost(lt24,&clyde,direction);
//	    	move_pacman(&Pac,Row,Col, maze);
//
//	    	LT24_copyFrameBuffer(lt24, ClydeU, clyde.x, clyde.y, 8, 8);
//	    	LT24_copyFrameBuffer(lt24, PinkyD, pinky.x, pinky.y, 8, 8);
//	    	LT24_copyFrameBuffer(lt24, InkyU, inky.x, inky.y, 8, 8);
//	    	LT24_copyFrameBuffer(lt24, BlinkyR, blinky.x, blinky.y, 8, 8);
//	        //LT24_clearDisplay(lt24, LT24_BLUE);
//	        //Finally reset the watchdog
//	        ResetWDT();
//	        *LEDR_ptr=*KEY_ptr;
//	        ResetWDT();
//	        delay_ms(1);
//	        if (blinky.x == Pac.x && blinky.y == Pac.y )
//	        {
//	        	printf("Game over");
//	        	break;
//	        };
////	    	printf("x %d y %d tx %d ty %d\n",blinky.x,blinky.y,blinky.target_x,blinky.target_y);
//}
//	}
