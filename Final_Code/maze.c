#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "Util/watchdog.h"
#include "Util/macros.h"
#include "Util/delay.h"
#include "FPGA_PIO/FPGA_PIO.h"
#include "DE1SoC_Addresses/DE1SoC_Addresses.h"

//matrix for maze

uint32_t maze[30][33] =
{
{6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 4, 0, 4, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7},
{4, 6, 3, 3, 3, 3, 3, 3, 3, 3, 7, 0, 0, 0, 4, 0, 4, 0, 0, 0, 6, 3, 3, 3, 3, 7, 6, 3, 3, 3, 3, 7, 4},
{4, 4, 1, 1, 2, 1, 1, 1, 1, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 1, 1, 2, 4, 4, 1, 1, 1, 1, 4, 4},
{4, 4, 1, 6, 3, 7, 1, 6, 7, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 6, 7, 1, 5, 8, 1, 6, 7, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 4, 4, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 4, 4, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 4, 5, 3, 3, 7, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 5, 3, 8, 1, 5, 8, 1, 5, 3, 3, 3, 8, 0, 5, 3, 3, 3, 8, 1, 5, 3, 3, 3, 8, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 6, 3, 7, 1, 6, 3, 3, 3, 3, 3, 3, 7, 0, 6, 3, 3, 3, 7, 1, 6, 7, 1, 6, 3, 3, 8, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 5, 3, 3, 7, 6, 3, 3, 8, 0, 5, 3, 3, 3, 8, 1, 4, 4, 1, 5, 3, 3, 7, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 1, 1, 1, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 6, 7, 1, 4, 4, 0, 6, 3, 3, 3, 7, 0, 6, 7, 1, 4, 4, 1, 6, 7, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 5, 3, 8, 1, 4, 4, 1, 5, 8, 0, 4, 0, 0, 0, 4, 0, 4, 4, 1, 5, 8, 1, 4, 4, 1, 5, 8, 1, 4, 4},
{4, 4, 1, 1, 1, 1, 1, 4, 4, 1, 0, 0, 0, 4, 0, 0, 0, 4, 0, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4},
{4, 5, 3, 3, 3, 7, 1, 4, 5, 3, 3, 7, 0, 9, 0, 0, 0, 4, 0, 4, 5, 3, 3, 7, 1, 4, 5, 3, 3, 7, 1, 4, 4},
{4, 6, 3, 3, 3, 8, 1, 4, 6, 3, 3, 8, 0, 9, 0, 0, 0, 4, 0, 4, 6, 3, 3, 8, 1, 4, 6, 3, 3, 8, 1, 4, 4},
{4, 4, 1, 1, 1, 1, 1, 4, 4, 1, 0, 0, 0, 4, 0, 0, 0, 4, 0, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 4, 4},
{4, 4, 1, 6, 3, 7, 1, 4, 4, 1, 6, 7, 0, 4, 0, 0, 0, 4, 0, 4, 4, 1, 6, 7, 1, 4, 4, 1, 6, 7, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 5, 8, 1, 4, 4, 0, 5, 3, 3, 3, 8, 0, 5, 8, 1, 4, 4, 1, 5, 8, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 1, 1, 1, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 6, 3, 3, 8, 5, 3, 3, 7, 0, 6, 3, 3, 3, 7, 1, 4, 4, 1, 6, 3, 3, 8, 4, 1, 4, 4},
{4, 4, 1, 5, 3, 8, 1, 5, 3, 3, 3, 3, 3, 3, 8, 0, 5, 3, 3, 3, 8, 1, 5, 8, 1, 5, 3, 3, 7, 4, 1, 4, 4},
{4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 6, 3, 7, 1, 6, 7, 1, 6, 3, 3, 3, 7, 0, 6, 3, 3, 3, 7, 1, 6, 3, 3, 3, 7, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 4, 4, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 4, 6, 3, 3, 8, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 4, 0, 4, 1, 4, 4, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 4, 4, 1, 1, 1, 1, 4, 4, 1, 4, 4},
{4, 4, 1, 5, 3, 8, 1, 5, 8, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 5, 8, 1, 6, 7, 1, 5, 8, 1, 4, 4},
{4, 4, 1, 1, 2, 1, 1, 1, 1, 1, 4, 0, 0, 0, 4, 0, 4, 0, 0, 0, 4, 1, 1, 1, 2, 4, 4, 1, 1, 1, 1, 4, 4},
{4, 5, 3, 3, 3, 3, 3, 3, 3, 3, 8, 0, 0, 0, 4, 0, 4, 0, 0, 0, 5, 3, 3, 3, 3, 8, 5, 3, 3, 3, 3, 8, 4},
{5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 4, 0, 4, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8}};


// Converting Numbers in the maze to its equivalent shape, relevant to the PDF
// LCD-pixel map x = 239; y = 319

void LT24_Maze(LT24Ctx_t* ctx, uint8_t maze_n, uint8_t x, uint16_t y){
	int i=0;
	int j=0;

	switch (maze_n)
	{
	case 0:
		LT24_drawPixel(ctx, LT24_BLACK, x+3, y+3);
		break;
	case 1:
		LT24_drawPixel(ctx, LT24_WHITE, x+3, y+3);
    	break;
	case 2:
		LT24_drawPixel(ctx, LT24_WHITE, x+3, y+3);
    	break;
	case 3:
    	for(j=0;j<=7;j++)
		{
    		for(i=3;i<=5;i++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;
	case 4:
    	for(i=0;i<=7;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;

	case 5:
    	for(i=0;i<=3;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	for(j=3;j<=7;j++)
		{
    		for(i=3;i<=5;i++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;

	case 6:
    	for(i=3;i<=7;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	for(j=3;j<=7;j++)
		{
    		for(i=3;i<=5;i++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;

	case 7:
    	for(i=3;i<=7;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	for(j=0;j<=3;j++)
		{
    		for(i=3;i<=5;i++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;

	case 8:
    	for(i=0;i<=3;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	for(j=0;j<=5;j++)
		{
    		for(i=3;i<=5;i++){
    			LT24_drawPixel(ctx, LT24_BLUE, x+i, y+j);
			}
		}
    	break;
	case 9:
    	for(i=0;i<=7;i++)
		{
    		for(j=3;j<=5;j++){
    			LT24_drawPixel(ctx, LT24_WHITE, x+i, y+j);
			}
		}
    	break;
	}
}


void exitOnFail(HpsErr_t status){
    if (ERR_IS_ERROR(status)) {
        exit((int)status); //Add breakpoint here to catch failure
    }
}

// Initialising the maze on the LCD-LT24 display

void maze_generation(){
    FPGAPIOCtx_t* gpio1;
    LT24Ctx_t* lt24_2;
    //Initialise the GPIO1 port
    exitOnFail(FPGA_PIO_initialise(LSC_BASE_GPIO_JP1, LSC_CONFIG_GPIO, &gpio1));
    //Initialise the LCD Display.
    exitOnFail(LT24_initialise(&gpio1->gpio,NULL,&lt24_2));
    ResetWDT();
    //Wait a moment
    usleep(500000);
    ResetWDT();

// Coordinates for maze generation

	for(uint8_t u=0;u<=29;u++)
	{
		for( uint8_t z=0; z<=32; z++)
		{
			LT24_Maze(lt24_2, maze[u][z], (8*u), (8*z));
		}
	};
}

// Pulls value of shape in a specified coordinate which can be called later for other use cases such as collision detetcion

uint8_t maze_index(int x, int y){
	uint8_t index=maze[y][x];
	return index;
}


