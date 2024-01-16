/**
 * Example of using the Digilent display drivers for Zybo Z7 HDMI output, with animation
 * Casper Hermans & Jelle de Haas, 11/03/2019
 */

#include <stdio.h>
#include "xil_types.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "zybo_z7_hdmi/display_ctrl.h"
#include "xil_printf.h"
#include <stdlib.h>

#include "xgpio_l.h"
#include "xgpio.h"

// Frame size (based on 1440x900 resolution, 32 bits per pixel)
#define MAX_FRAME (1440*900)
#define FRAME_STRIDE (1440*4)

// Define btns address
#define BTNS_ADDRESS 0x41200000
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

DisplayCtrl dispCtrl; // Display driver struct
u32 frameBuf[DISPLAY_NUM_FRAMES][MAX_FRAME] __attribute__((aligned(0x20))); // Frame buffers for video data
void *pFrames[DISPLAY_NUM_FRAMES]; // Array of pointers to the frame buffers

// Initialise btn value variables
int btn_value = 0;
int prev_btn_value = 0;


void draw_block(int x, int y, int size, int colour, int pixel_stride, u32* frame)
{
	// Draw square on the screen
	for (int pixel_x = x; pixel_x < x+size; pixel_x++) {
		for (int pixel_y = y; pixel_y < y+size; pixel_y++) {
			frame[pixel_y*pixel_stride + pixel_x] = colour;
		}
	}
}


int main(void) {

	// Initialise an array of pointers to the 2 frame buffers
	int i;
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
		pFrames[i] = frameBuf[i];

	// Initialise the display controller
	DisplayInitialize(&dispCtrl, XPAR_AXIVDMA_0_DEVICE_ID, XPAR_VTC_0_DEVICE_ID, XPAR_HDMI_AXI_DYNCLK_0_BASEADDR, pFrames, FRAME_STRIDE);

	// Start with the first frame buffer (of two)
	DisplayChangeFrame(&dispCtrl, 0);

	// Set the display resolution
	DisplaySetMode(&dispCtrl, &VMODE_1440x900);

	// Enable video output
	DisplayStart(&dispCtrl);

	printf("\n\r");
	printf("HDMI output enabled\n\r");
	printf("Current Resolution: %s\n\r", dispCtrl.vMode.label);
	printf("Pixel Clock Frequency: %.3fMHz\n\r", dispCtrl.pxlFreq);
	printf("Starting animation loop...\n\r");

	// Get parameters from display controller struct
	u32 stride = dispCtrl.stride / 4;
	u32 width = dispCtrl.vMode.width;
	u32 height = dispCtrl.vMode.height;

	u32 *frame;

	const int array_size = 100; // Constant value for the array
	int snake_xpos[array_size];
	int snake_ypos[array_size];

	int gridcell_size = 64;
	int grid_width = (width/gridcell_size);
	int grid_height = (height/gridcell_size);

	int btn_time = 3500;
	int btn_delay = btn_time;

	int timer = 0;
		int btn_pressed = 0;
		int snake_turn = 3000;
		int snake_direction = RIGHT;

		snake_xpos[0] = 0;
		snake_ypos[0] = 0;
		snake_xpos[1] = 0;
		snake_ypos[1] = 1;
		//snake_xpos[2] = 1;
		//snake_ypos[2] = 1;
		//snake_xpos[3] = 1;
		//snake_ypos[3] = 2;
		//snake_xpos[4] = 2;
		//snake_ypos[4] = 2;


		int snake_length = 2;
		int game_over = 0;
		int target_falseplace = 0;

		// Generate target position
		int target_xpos = 10;
		int target_ypos = 10;

	u32 buff = dispCtrl.curFrame;

	while (1) {

			timer ++;

			if (btn_delay > 0)
			{
				btn_delay--;
			}

			//Read button value
			btn_value = Xil_In32(BTNS_ADDRESS);
			xil_printf("%d", btn_value);
			if (btn_pressed == 0 && btn_delay == 0)
			{
				if (btn_value == 1) //Right button
				{
					if (snake_direction < 4)
					{
						snake_direction++;//Rotate right
					} else {
						snake_direction = UP; //
					}
				btn_pressed = 1;
				btn_delay = btn_time;
				xil_printf("/n Direction:");
				xil_printf("%d", snake_direction);
				}

				if (btn_value == 8) //Left button
				{
					if (snake_direction == 1)
					{
						snake_direction = 4;
					} else
					{
						snake_direction--; //Rotate left
					}
				btn_pressed = 1;
				btn_delay = btn_time;
				xil_printf("/n Direction:");
				xil_printf("%d", snake_direction);
				}
			}


			if (timer > snake_turn){

				timer = 0;
				btn_pressed = 0;

				int xpos = snake_xpos[snake_length-1]; //head coordinates
				int ypos = snake_ypos[snake_length-1];
				switch(snake_direction) // Change snake direction
				{
				case(RIGHT):
					xpos++;
					break;
				case(UP):
					ypos--;
					break;
				case(LEFT):
					xpos--;
					break;
				case(DOWN):
					ypos++;
					break;
				}

				snake_xpos[snake_length] = xpos;
				snake_ypos[snake_length] = ypos;

				//Check if the head touches the tail
				for(i=0;i<snake_length-1;i++)
				{
					if (xpos == snake_xpos[i] && ypos == snake_ypos[i])
					{ //Game over screen and reset (needs cleaning up)
						game_over = 1;
						timer = 0;
						btn_pressed = 0;
						snake_direction = RIGHT;

						snake_xpos[0] = 0;
						snake_ypos[0] = 0;
						snake_xpos[1] = 0;
						snake_ypos[1] = 1;
						snake_xpos[2] = 1;
						snake_ypos[2] = 1;
						snake_xpos[3] = 1;
						snake_ypos[3] = 2;
						snake_xpos[4] = 2;
						snake_ypos[4] = 2;

						snake_length = 2;
						snake_turn = 3000;

						// Generate target position
						target_xpos = rand()%(grid_width);
						target_ypos = rand()%(grid_height);

						xpos = 0;
						ypos = 0;
						btn_value = 0;
						btn_time = 3500;
					}
				}
				//Check if the snake hits the wall
				if (xpos > (grid_width-1) || xpos < 0 || ypos > (grid_height-1) || ypos < 0)
				{ //Game over screen and reset (needs cleaning up)
					game_over = 1;
						timer = 0;
						btn_pressed = 0;
						snake_direction = RIGHT;

						snake_xpos[0] = 0;
						snake_ypos[0] = 0;
						snake_xpos[1] = 0;
						snake_ypos[1] = 1;
						snake_xpos[2] = 1;
						snake_ypos[2] = 1;
						snake_xpos[3] = 1;
						snake_ypos[3] = 2;
						snake_xpos[4] = 2;
						snake_ypos[4] = 2;

						snake_length = 2;
						snake_turn = 3000;

						// Generate target position
						target_xpos = rand()%(grid_width);
						target_ypos = rand()%(grid_height);

						xpos = 0;
						ypos = 0;
						btn_value = 0;
						btn_time = 3500;
				}

				if (xpos == target_xpos && ypos == target_ypos)
				{
					xil_printf("\n Score: ");
					xil_printf("%d", snake_length-1);
					xil_printf("\n");
					snake_length++;
					snake_turn -= 50; //make the turn quicker
					btn_time -= 30; //make the button delay less
					target_falseplace = 1;
					while(target_falseplace == 1)
					{
						// Generate target position
						target_xpos = 1+rand()%(grid_width-2); //From x = 1 to width-1
						target_ypos = 1+rand()%(grid_height-2);
						target_falseplace = 0;

						for(i=0;i<snake_length-1;i++)
						{

							if (target_xpos == snake_xpos[i] && target_ypos == snake_ypos[i])
							{
								target_falseplace = 1;
							}
						}
					}
				}else{

					for (i = 0;i<array_size-1;i++) //Remove tail
					{
						snake_xpos[i] = snake_xpos[i+1];
						snake_ypos[i] = snake_ypos[i+1];
					}
				}



				// Switch the frame we're modifying to be the back buffer (1 to 0, or 0 to 1)
				buff = !buff;
				frame = (u32 *)dispCtrl.framePtr[buff];

				// Clear the entire frame to white (inefficient, but it works)
				memset(frame, 0x0, MAX_FRAME*4);


				// Draw snake body
				for(i=0;i<snake_length-1;i++)
				{
					draw_block(snake_xpos[i]*gridcell_size, snake_ypos[i]*gridcell_size, gridcell_size, 0x00ffff66, stride, frame);
				}

				// Draw snake head
				draw_block(snake_xpos[snake_length-1]*gridcell_size, snake_ypos[snake_length-1]*gridcell_size, gridcell_size, 0x00ffff00, stride, frame);

				// Draw target
				draw_block(target_xpos*gridcell_size, target_ypos*gridcell_size, gridcell_size, 0x00FF00FB, stride, frame);

				// Flush everything out to DDR from the cache
				Xil_DCacheFlush();

				// Switch active frame to the back buffer (will take place during next vertical blanking period)
				DisplayChangeFrame(&dispCtrl, buff);

			}



			while(game_over == 1)
			{
				btn_value = Xil_In32(BTNS_ADDRESS);
				if (btn_value > 0)
				{
					game_over = 0;
					btn_value = 0;
					btn_delay = btn_time;
				}


				// Switch the frame we're modifying to be the back buffer (1 to 0, or 0 to 1)
				buff = !buff;
				frame = (u32 *)dispCtrl.framePtr[buff];

				// Clear the entire frame to white (inefficient, but it works)
				memset(frame, 0x0, MAX_FRAME*4);

				for (int pixel_x = 0; pixel_x < width-1; pixel_x++) {
					for (int pixel_y = 0; pixel_y < height-1; pixel_y++) {
						frame[pixel_y*stride + pixel_x] = 0x00ff0000;
					}
				}

				// Flush everything out to DDR from the cache
				Xil_DCacheFlush();

				// Switch active frame to the back buffer (will take place during next vertical blanking period)
				DisplayChangeFrame(&dispCtrl, buff);

				// Wait for the frame to switch before continuing (so after current frame has been drawn)
				DisplayWaitForSync(&dispCtrl);
			}


			// Wait for the frame to switch before continuing (so after current frame has been drawn)
			DisplayWaitForSync(&dispCtrl);
	}

	return 0;
}

