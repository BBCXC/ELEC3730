//     $Date: 2018-05-21 09:11:26 +1000 (Mon, 21 May 2018) $
// $Revision: 1327 $
//   $Author: Peter $

#include "Ass-03.h"

//
// This task can be used as the main pulse rate application as it takes
// input from the front panel.
//
// *** MAKE UPDATES TO THE CODE AS REQUIRED ***
//
// Draw the boxes that make up the buttons and take action when the
// buttons are pressed. See suggested updates for the touch panel task
// that checks for button presses. Can do more in that task.

int popup = 0; // TODO Remove global variable
int touch_pos = NULL;

void Ass_03_Task_02(void const * argument)
{

	uint32_t loop=0;

	Coordinate display;

	osSignalWait(1,osWaitForever);

	safe_printf("Hello from Task 2 (Pulse Rate Application)\n");

while (1)
{
	// Check position touched
	if (getfp(&display) == 0)
    {/*
		// If the position touched matches some given coordinates
		if((display.y > YOFF+5) && (display.y < YOFF+YSIZE-5) &&
			(display.x > XOFF+5) && (display.x < XOFF+XSIZE-5))
		{
			//Do something on that touch
			osMutexWait(myMutex01Handle, osWaitForever);
			BSP_LCD_FillCircle(display.x, display.y, 2);
			osMutexRelease(myMutex01Handle);
			safe_printf("Task 2: %d (touch %3d,%3d)\n", loop, display.x, display.y);
			loop++;
		}*/

		// If the position touched matches some given coordinates
		touch_pos = get_touch_pos(display.x, display.y, popup);
		if(touch_pos != 100)
		{
			//Do something on that touch
			osMutexWait(myMutex01Handle, osWaitForever);
//			BSP_LCD_FillCircle(display.x, display.y, 2);

			osMutexRelease(myMutex01Handle);
			safe_printf("Task 2: %d (touch: %d, %3d,%3d)\n", loop, touch_pos, display.x, display.y);
			loop++;

			//Touched stop
			if(touch_pos == 1){
				Set_Zoom_Coeff_w(0);
				//osMessagePut (myQueue02Handle, 0, 0);
				safe_printf("Set state to stop\n");
			}
			else if(touch_pos == 0){
				Set_Zoom_Coeff_w(1);

				safe_printf("Set state to play\n");
				//osMessagePut (myQueue02Handle, 1, 0);
			}
		}


    }
}
}
