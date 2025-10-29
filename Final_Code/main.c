/*
 * main.c
 *
 *  Created on: 28 Apr 2025
 *  Updated on: 14 May 2025
 *      Author: Neeraj
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "menu_image.h" // Main menu, control, and about images
#include "about_image.h"
#include "controls_image.h"
#include "video_frames.h" // Video frames
#include "PacMan_Control.h"
#include "Util/watchdog.h"
#include "FPGA_PIO/FPGA_PIO.h"
#include "DE1SoC_Addresses/DE1SoC_Addresses.h"
#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "maze.h"
#include "Ghost.h"
#include "HPS_GPIO/HPS_GPIO.h"
#include "HPS_I2C/HPS_I2C.h"

// Global I/O pointers for use in all functions
volatile uint8_t *KEY_ptr = LSC_BASE_KEYS;
volatile uint8_t  *LED_ptr = LSC_BASE_RED_LEDS;

// LT24 context (added this line to declare lt24)
LT24Ctx_t* lt24_2;  //currently kept as lt24_2, since lt24 is being used in maze generation. During clean-up and optimisation, aim to modify to use same variable globally.

// Seven-segment encodings (active high)
#define SEG_OFF 0x00
#define SEG_0   0x3F // A, B, C, D, E, F
#define SEG_1   0x06 // B, C
#define SEG_8   0x7F // A, B, C, D, E, F, G (for testing)

// Defining Buttons with the keys and directions
#define BUTTON_UP     0x8  // KEY3
#define BUTTON_DOWN   0x4  // KEY2
#define BUTTON_LEFT   0x2  // KEY1
#define BUTTON_RIGHT  0x1  // KEY0

int new_direction = DIR_NONE;

/*// Error handler
void exitOnFail(HpsErr_t status) {
    if (ERR_IS_ERROR(status)) {
        exit((int)status);
    }
}*/

// Simple delay for video frame rate
void delay(int ms) {
    for (volatile int i = 0; i < ms * 1000; i++);
}

// Simple delay
void movdelay(int ms2) {
    for (volatile int k = 0; k < ms2 * 10000; k++) {
        __asm__("nop");
    }
}

// Video frame array
const unsigned short* videoFrames[TOTAL_FRAMES] = {
    EmbPac_0001, EmbPac_0002, EmbPac_0003, EmbPac_0004, EmbPac_0005,
    EmbPac_0006, EmbPac_0007, EmbPac_0008, EmbPac_0009, EmbPac_0010,
    EmbPac_0011, EmbPac_0012, EmbPac_0013, EmbPac_0014, EmbPac_0015,
    EmbPac_0016, EmbPac_0017, EmbPac_0018, EmbPac_0019, EmbPac_0020,
    EmbPac_0021, EmbPac_0022, EmbPac_0023, EmbPac_0024, EmbPac_0025,
    EmbPac_0026, EmbPac_0027, EmbPac_0028, EmbPac_0029, EmbPac_0030,
    EmbPac_0031, EmbPac_0032, EmbPac_0033, EmbPac_0034, EmbPac_0035,
    EmbPac_0036, EmbPac_0037, EmbPac_0038, EmbPac_0039, EmbPac_0040,
    EmbPac_0041, EmbPac_0042, EmbPac_0043, EmbPac_0044, EmbPac_0045,
    EmbPac_0046, EmbPac_0047, EmbPac_0048, EmbPac_0049, EmbPac_0050
};

int readDirection(volatile uint8_t *key_ptr) {
    uint8_t key_val = *key_ptr;

    if (key_val & BUTTON_UP)    return DIR_UP;
    if (key_val & BUTTON_DOWN)  return DIR_DOWN;
    if (key_val & BUTTON_LEFT)  return DIR_LEFT;
    if (key_val & BUTTON_RIGHT) return DIR_RIGHT;

    return DIR_NONE;

}
/////////////GHOST//////////////////

//typedef struct {
//	uint16_t x, y; // Current position
//	uint16_t target_x, target_y; // Target tile (Pacman's location)
//	char mode; // C = Chase, S = Scatter
//} Ghost;

static Ghost blinky = {.x = 12*8 , .y= 9*8, .mode = 'S'};
static Ghost pinky = {.x = 14*8, .y=6*8, .mode= 'S'};
static Ghost inky = {.x = 22*8, .y=12*8, .mode= 'S'};
static Ghost clyde = {.x = 12*8, .y=6*8, .mode= 'S'};

///////////////////////////////////

int main() {
	// Initialise GPIO
	FPGAPIOCtx_t* gpio1;
	FPGAPIOCtx_t* switches;
	FPGAPIOCtx_t* seg0to3; // HEX3-HEX0
    FPGAPIOCtx_t* seg4to5; // HEX5-HEX4
	FPGAPIOCtx_t* leds; // Red LEDs for FIFO debug
	HPSGPIOCtx_t* gpio;

	// Initialise peripherals
	HpsErr_t status = FPGA_PIO_initialise(LSC_BASE_GPIO_JP1, LSC_CONFIG_GPIO, &gpio1);
	// Debugging line - if initialisation fails, turn on all LEDs
	if (ERR_IS_ERROR(status)) {
	    *LED_ptr = 0xFF;
	    exit((int)status);
	}

	// Initialize slide switches
	exitOnFail(FPGA_PIO_initialise(LSC_BASE_SLIDE_SWITCH, LSC_CONFIG_SLIDE_SWITCH, &switches));

	// Initialize seven-segment displays
    exitOnFail(FPGA_PIO_initialise(LSC_BASE_7SEG_0to3, LSC_CONFIG_7SEG, &seg0to3));
    exitOnFail(FPGA_PIO_initialise(LSC_BASE_7SEG_4to5, LSC_CONFIG_7SEG, &seg4to5));

	// Initialize red LEDs for FIFO debug
    exitOnFail(FPGA_PIO_initialise(LSC_BASE_RED_LEDS, LSC_CONFIG_RED_LEDS, &leds));

	// Initialise the LT24 Display
	status = LT24_initialise(&gpio1->gpio, NULL, &lt24_2);
	// Debugging line - if LT24 initialisation fails, turn on all LEDs
	if (ERR_IS_ERROR(status)) {
	    *LED_ptr = 0xFF;
	    exit((int)status);
	}

	//ResetWDT();

	// Turn off seven-segment displays and LEDs initially
    exitOnFail(FPGA_PIO_setOutput(seg0to3, (SEG_OFF << 24) | (SEG_OFF << 16) | (SEG_OFF << 8) | SEG_OFF, 0xFFFFFFFF));
    exitOnFail(FPGA_PIO_setOutput(seg4to5, (SEG_OFF << 8) | SEG_OFF, 0xFFFF));
    exitOnFail(FPGA_PIO_setOutput(leds, 0, UINT32_MAX));

	// Display main menu image initially
	exitOnFail(LT24_copyFrameBuffer(lt24_2, Menu_img, 0, 0, 240, 320));

//Wait for Switch input
	while(1) {
		ResetWDT();

	// Read switch states (active high, bits 0, 1, 2 in LSC_BASE_SLIDE_SWITCH)
	unsigned int switchState;
	exitOnFail(FPGA_PIO_getInput(switches, &switchState, LSC_SLIDE_SWITCH_MASK));

	// Handle seven-segment display for SW3
        if (switchState & 0x8) { // SW3 on: Display 10000
            exitOnFail(FPGA_PIO_setOutput(seg4to5, (SEG_1 << 8) | SEG_0, 0xFFFF)); // HEX5=1, HEX4=0
            exitOnFail(FPGA_PIO_setOutput(seg0to3, (SEG_0 << 24) | (SEG_0 << 16) | (SEG_0 << 8) | SEG_0, 0xFFFFFFFF)); // HEX3-HEX0=0
        } else { // SW3 off: Turn off displays
            exitOnFail(FPGA_PIO_setOutput(seg4to5, (SEG_OFF << 8) | SEG_OFF, 0xFFFF));
            exitOnFail(FPGA_PIO_setOutput(seg0to3, (SEG_OFF << 24) | (SEG_OFF << 16) | (SEG_OFF << 8) | SEG_OFF, 0xFFFFFFFF));
        }

	if (switchState & 0x1) { // SW0 on: Play video
	    // Play video frames while SW0 is on
	    for (int i = 0; i < TOTAL_FRAMES; i++) {
	        exitOnFail(LT24_copyFrameBuffer(lt24_2, videoFrames[i], 0, 0, 240, 320));
	        //delay(100); // ~10 FPS
	        ResetWDT();
	    }
	    break;
        
	} else if (switchState & 0x2) { // SW1 on: Show control image
	    exitOnFail(LT24_copyFrameBuffer(lt24_2, control_img, 0, 0, 240, 320));
	} else if (switchState & 0x4) { // SW2 on: Show about image
	    exitOnFail(LT24_copyFrameBuffer(lt24_2, about_img, 0, 0, 240, 320));
	} else { // All switches off: Show main menu
	    exitOnFail(LT24_copyFrameBuffer(lt24_2, Menu_img, 0, 0, 240, 320));
	}

}

	// Initialise maze and display
    maze_generation();

    // Initialise PacMan in corner
    initPacMan();
    //drawPacMan();    //error code line in terms of order. To be cleaned up
    int direction_bl=0;
    int direction_pi=0;
    int direction_in=0;
    int direction_cl=0;
    // Game loop - - Read key press, set direction, and move PacMan accordingly
    while (1) {

        // Read direction from buttons
        new_direction = readDirection(KEY_ptr);
        if (new_direction != DIR_NONE) {
        	pacman_direction = new_direction;

        }
    	int g=0;
    	g=game_0ver(&inky,&pinky, &blinky, &clyde, pacman_x, pacman_y );
    	if (g==1){
    		break;
    	}
// Blinky
        	Clear_ghost(lt24_2,&blinky,direction_bl);
        	direction_bl=move_blinky(&blinky, pacman_x, pacman_y , 30, 33, maze);



//Pinky
			Clear_ghost(lt24_2,&pinky,direction_pi);
        	direction_pi=move_pinky(&pinky, pacman_x, pacman_y , 30, 33, maze,pacman_direction);
//

//Inky
        	Clear_ghost(lt24_2,&inky,direction_in);
			direction_in=move_inky(&inky,&blinky, pacman_x, pacman_y , 30, 33, maze,pacman_direction);
//
//clyde
			Clear_ghost(lt24_2,&clyde,direction_cl);
			direction_cl=move_clyde(&clyde, pacman_x, pacman_y , 30, 33, maze);

			initblinky(lt24_2,blinky.x,blinky.y);
			initpinky(lt24_2,pinky.x,pinky.y);
			initinky(lt24_2,inky.x,inky.y);
			initclyde(lt24_2,clyde.x,clyde.y);

        	clearPacMan();
        	movePacMan(pacman_direction);
        	drawPacMan();
        	movdelay(2);
            ResetWDT();  // Reset watchdog
            }
            return 0;
        }
